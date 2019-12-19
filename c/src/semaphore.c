#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t  g_sem;

// 信号量
static void* thread_run_sem_1(void* _val)
{
    printf("%s\n", (char*)_val);
    int i = 3;
    while(i) {
        sem_wait(&g_sem);
        printf("sem_wait count =%d\n", i);
        i--;
    }
    pthread_exit("thread_run_sem_1 exit");
}

static void* thread_run_sem_11(void* _val)
{
    printf("%s\n", (char*)_val);

    int i = 8;
    for(; i>0; i--) {
        sleep(1);
        printf("sem post\n");
        sem_post(&g_sem);
    }
    pthread_exit("thread_run_sem_11 exit");
}

int m_create_thread_sem(void)
{
    int ret;
    pthread_t pid_1;
    pthread_t pid_11;

    //int sem_init(sem_t *sem, int pshared, unsigned int value);
    //其中sem是要初始化的信号量，pshared表示此信号量是在进程间共享还是线程间共享，value是信号量的初始值。
    sem_init(&g_sem, 0, 0);

    ret = pthread_create(&pid_1, NULL, thread_run_sem_1, "thread thread_run_sem_11 ...");
    if (-1 == ret) {
        printf("create error!\n");
        return -1;
    }
    ret = pthread_create(&pid_11, NULL, thread_run_sem_11, "tthread_run_sem_11 ...");
    if (-1 == ret) {
        printf("create error!\n");
        return -1;
    }
    printf("wait ...\n");
    pthread_join(pid_1, NULL);
    pthread_join(pid_11, NULL);
    int sval = 0;
    //获取信号量 sem 的当前值，把该值保存在 sval，
    //若有 1 个或者多个线程正在调用 sem_wait 阻塞在该信号量上，该函数返回阻塞在该信号量上进程或线程个数。
    sem_getvalue(&g_sem, &sval);
    printf("sval=%d\n", sval);
    sem_destroy(&g_sem);
    return 0;
}



int main(int argc, char *argv[])
{
    int ret = 0;
    int select = 0;
    
    while(1) {
        printf("-------------------------------------\n");
        printf("            0 exit                   \n");
        printf("            1 m_create_thread_sem    \n");
        printf("-------------------------------------\n");
        scanf("%d", &select);
        getchar();
        switch(select) {
        case 0:
            exit(0);
        case 1:
            m_create_thread_sem();
            break;
        default:
            break;
        }
    }
    return 0;
}