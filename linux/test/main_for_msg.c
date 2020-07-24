#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <errno.h>
#include<sys/mman.h>
#include <pthread.h>

#define SUN_COUNT 4
#define DATA_COUNT 10
#define MSG_TYPE   555

typedef struct data {
    int index;
    int op;
}data_t;

typedef struct msg_data {
    long type;
    data_t data;
}msg_data_t;

typedef struct db {
    int index;
    int count;
}db_t;

static void signal_user(int signo);
static void wait_sun(void);
static int db_init(db_t (*db)[DATA_COUNT]);
static int db_update(db_t (*db)[DATA_COUNT], int index, int op);
static void db_printf(db_t (*db)[DATA_COUNT]);

static pid_t sun[SUN_COUNT];
static pthread_mutex_t *dptr;
static db_t (*db_map)[DATA_COUNT];
static exit_flag = 1;

void sun_run(void)
{
    int pid;
    int id, res;
    msg_data_t msg_d;

    pid = getpid();

    sleep(2);
    id = msgget(0x8888, 0);
    if (id < 0) {
        printf("msgget error, sun1 exit\n");
        exit(1);
    }

    for ( ; ; ) {
        memset(&msg_d, 0, sizeof(msg_data_t));
        /* msgflg 为0表示阻塞方式，设置IPC_NOWAIT 表示非阻塞方式 */
        res = msgrcv(id, &msg_d, sizeof(msg_data_t), MSG_TYPE, 0);
        if (res < 0) {
            if (errno == ENOMSG) {
                printf("No message!\n");
            }
        } else {
            /* 更新共享数据 */
            pthread_mutex_lock(dptr);
            (void)db_update(db_map, msg_d.data.index, msg_d.data.op);
            pthread_mutex_unlock(dptr);
            printf("<<<[%d]recv : type=%ld, index=%d, op=%d\n", pid, msg_d.type, msg_d.data.index, msg_d.data.op);
            usleep(500);
        }
    }
}

int main(int argc, char *argv[])
{
    int id, res, i;
    pthread_mutex_t *mptr;

    dptr = (pthread_mutex_t *)mmap(NULL, sizeof(pthread_mutex_t) + sizeof(db_t)*DATA_COUNT, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);

    db_map = (db_t (*)[DATA_COUNT])(dptr + 1);
    if (dptr == NULL || db_map == NULL) {
        printf("mmap error\n");
        return -1;
    }

    pthread_mutexattr_t  mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(dptr, &mattr);

    /* 创建子进程 */
    for (i=0; i<SUN_COUNT; i++) {
        sun[i] = fork();
        if (sun[i] == 0) {
            /* sun1 */
            printf("sun[%d], pid = %d\n", sun[i], getpid()); 
            sun_run();
            return 0;
        } else if (sun[i] < 0) {
            printf("fork sun[%d] error\n", sun[i]);
        }
    }

    if (signal(SIGTERM, signal_user) == SIG_ERR) {
        printf("cant't catch SIGTERM");
    }
      
    id = msgget(0x8888, IPC_CREAT | 0664);
    if (id < 0) {
        printf("msgget error\n");
        goto EXIT;
    }

    msg_data_t msg_d;
    db_t local_db[DATA_COUNT];
    db_init(&local_db);
    
    while(exit_flag) {
        /* 发送消息 */
        memset(&msg_d, 0, sizeof(msg_data_t));
        srand(time(0));
        msg_d.type = MSG_TYPE;
        msg_d.data.index = rand()%9;
        srand(time(0));
        msg_d.data.op = rand()%2;

        (void)db_update(&local_db, msg_d.data.index, msg_d.data.op);

        res = msgsnd(id, &msg_d, sizeof(msg_data_t), 0);
        if (res < 0) {
            printf("msg send error\n");
        }
        else {
            printf(">>>[%d]send : type=%ld, index=%d, op=%d\n",getpid(), msg_d.type, msg_d.data.index, msg_d.data.op);
        }
        
        sleep(1);
    }

    /* 对比本地数据和共享数据 */
    printf("local db");
    db_printf(&local_db);
    printf("share db");
    db_printf(db_map);

EXIT:
    wait_sun();
    munmap(db_map, sizeof(db_t)*DATA_COUNT);
    pthread_mutexattr_destroy(&mattr);
    printf("exit\n");
    return 0;
}

static int db_init(db_t (*db)[DATA_COUNT])
{
    int i;
    if (db == NULL) {
        return -1;
    }

    for (i=0; i<DATA_COUNT; i++) {
        db[i]->index = i;
        db[i]->count = 0;
    }
    return 0;
}

static int db_update(db_t (*db)[DATA_COUNT], int index, int op)
{
    if (db == NULL) {
        return -1;
    }

    if (op == 1) {
        db[index]->count++;
    } else {
        db[index]->count--;
    }

    return 0;
}

static void db_printf(db_t (*db)[DATA_COUNT])
{
    int i;
    if (db == NULL) {
        return ;
    }
    printf("--------------------\n");
    printf("  index  |  count\n");
    for (i=0; i<DATA_COUNT; i++) {
        printf("   %d   |   %d\n", db[i]->index, db[i]->count);
    }
}

static void wait_sun(void)
{
    int i, status = 0;
    /* 通知子进程退出 */
    for (i=0; i<SUN_COUNT; i++){
        kill(sun[i], SIGABRT);
        waitpid(sun[i], &status, 0);
        printf("%d\n", WEXITSTATUS(status));
    }
}

static void signal_user(int signo)
{
    if (signo == SIGTERM) {
        printf("recevied SIGTERM\n");
        exit_flag = 0;
    }
    else {
        printf("other sig =%d\n" , signo);
    }
    return ;
}
