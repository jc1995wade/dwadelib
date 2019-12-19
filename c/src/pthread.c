#include <stdio.h>
#include <error.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

static void* thread_run_1(void* _val)
{
    printf("%s\n", (char*)_val);
    sleep(2);
    printf("%s\n", (char*)_val);
    pthread_exit("thread_run_1 exit"); //或者使用return NULL退出
}

static void* thread_run_2(void* _val)
{
    while(1) {
        printf("%s\n", (char*)_val);
        sleep(1);
    }
    
    return NULL;
}

static void* thread_run_3(void* _val)
{
    printf("%s\n", (char*)_val);
    sleep(1);
    printf("thread_run_3 exit\n");
    return NULL;
}

static void* thread_run_4(void* _val)
{
    pthread_detach(pthread_self());// 设置分离线程，线程退出后由系统回收
    printf("%s\n", (char*)_val);
    sleep(1);
    printf("thread_run_4 exit\n");
    return NULL;
}

//1.默认创建可结合的（joinable），需要pthread_join()来回收资源
int m_create_thread_joinable(void)
{
    int ret;
    pthread_t pid_1;
    ret = pthread_create(&pid_1, NULL, thread_run_1, "thread 1 run ...");
    if (-1 == ret) {
        printf("create error!\n");
        return -1;
    }
    printf("wait ...\n");
    char *tret = NULL;
    pthread_join(pid_1, (void**)&tret);
    printf("tread_run ret=%s\n", tret);
    return 0;
}

//2.可以调用pthread_detach(pthread_self())回收资源
int m_create_thread_detached(void)
{
    int ret;
    pthread_t pid_2;
    ret = pthread_create(&pid_2, NULL, thread_run_2, "thread 2 run ...");
    if (-1 == ret) {
        printf("create error!\n");
        return -1;
    }
    printf("wait ...\n");
    sleep(3);
    pthread_cancel(pid_2);
    pthread_join(pid_2, NULL);
    return 0;
}

 //3.设置线程属性为detach，在线程结束时，由系统释放线程资源
int m_create_thread_attr(void)
{
    int ret;
    pthread_t pid_3;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&pid_3, &attr, thread_run_3, "thread 3 run ...");
    if (-1 == ret) {
        printf("create error!\n");
        return -1;
    }
    printf("wait ...\n");
    sleep(3);
    return 0;
}

//4.状态改为unjoinable状态，确保资源的释放
int m_create_thread_unjoinable(void)
{
    int ret;
    pthread_t pid_4;
    ret = pthread_create(&pid_4, NULL, thread_run_4, "thread 4 run ...");
    if (-1 == ret) {
        printf("create error!\n");
        return 1;
    }
    printf("wait ...\n");
    sleep(3);

    printf("thread exit\n");
}


// 5.mutex
static pthread_mutex_t g_mutex_lock;
static int g_count;

static void* thread_run_mutex_1(void* _val)
{
    printf("%s\n", (char*)_val);

    pthread_mutex_lock(&g_mutex_lock);
    g_count++;
    printf("g_count=%d\n", g_count);
    pthread_mutex_unlock(&g_mutex_lock);

    pthread_exit("thread_run_1 exit");
}

static void* thread_run_mutex_11(void* _val)
{
    printf("%s\n", (char*)_val);

    pthread_mutex_lock(&g_mutex_lock);
    g_count++;
    printf("g_count=%d\n", g_count);
    pthread_mutex_unlock(&g_mutex_lock);

    pthread_exit("thread_run_1 exit");
}

int m_create_thread_mutex(void)
{
    int ret;
    pthread_t pid_1;
    pthread_t pid_11;

    // mutex init
    ret = pthread_mutex_init(&g_mutex_lock, NULL);
    if (ret != 0) {
        printf("mutex init failed\n");
        return -1;
    }

    ret = pthread_create(&pid_1, NULL, thread_run_mutex_1, "thread mutex 1 ...");
    if (-1 == ret) {
        printf("create error!\n");
        return -1;
    }
    ret = pthread_create(&pid_11, NULL, thread_run_mutex_11, "thread mutex 11 ...");
    if (-1 == ret) {
        printf("create error!\n");
        return -1;
    }
    printf("wait ...\n");
    pthread_join(pid_1, NULL);
    pthread_join(pid_11, NULL);
    pthread_mutex_destroy(&g_mutex_lock);
    return 0;
}


// 6.设置mutex类型
int m_set_mutex_para(void)
{
    // https://www.cnblogs.com/wuyepeng/p/9749956.html
    // 1. PTHREAD_MUTEX_FAST_NP 快速型
    // 2. PTHREAD_MUTEX_RECURSIVE_NP 递归型
    // 3. PTHREAD_MUTEX_ERRORCHECK_NP 错误检测型
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&mutex,&attr);
    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&attr);
    return 0;
}

// 7. 静态初始化，无需释放
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

// 8. 静态初始化，无需释放
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; //条件变量结构


// 9. 条件变量
pthread_mutex_t g_mutex;
pthread_cond_t g_cond;
static void* thread_run_cond_1(void* _val)
{
    printf("%s\n", (char*)_val);
    int i = 3;
    while(i) {
        pthread_cond_wait(&g_cond, &g_mutex);
        printf("wait count =%d\n", i);
        i--;
    }
    pthread_exit("thread_run_cond_1 exit");
}

static void* thread_run_cond_11(void* _val)
{
    printf("%s\n", (char*)_val);

    int i = 8;
    for(; i>0; i--) {
        printf("send  =%d\n", i);
        //pthread_mutex_lock(&g_mutex);
        pthread_cond_signal(&g_cond);
        //pthread_mutex_unlock(&g_mutex);
        sleep(1);
    }
    pthread_exit("thread_run_cond_11 exit");
}

int m_create_thread_cond(void)
{
    int ret;
    pthread_t pid_1;
    pthread_t pid_11;

    // mutex init
    ret = pthread_mutex_init(&g_mutex, NULL);
    if (ret != 0) {
        printf("g_mutex init failed\n");
        return -1;
    }
    ret = pthread_cond_init(&g_cond, NULL);
    if (ret != 0) {
        printf("g_cond init failed\n");
        return -1;
    }

    ret = pthread_create(&pid_1, NULL, thread_run_cond_1, "thread cond 1 ...");
    if (-1 == ret) {
        printf("create error!\n");
        return -1;
    }
    ret = pthread_create(&pid_11, NULL, thread_run_cond_11, "thread cond 11 ...");
    if (-1 == ret) {
        printf("create error!\n");
        return -1;
    }
    printf("wait ...\n");
    pthread_join(pid_1, NULL);
    pthread_join(pid_11, NULL);
    pthread_mutex_destroy(&g_mutex);
    pthread_cond_destroy(&g_cond);
    return 0;
}


int main(int argc, char *argv[])
{
    int ret = 0;
    int select = 0;
    
    while(1) {
        printf("--------------------------------------------------\n");
        printf("            0 exit                                \n");
        printf("            1 m_create_thread_joinable            \n");
        printf("            2 m_create_thread_detached            \n");
        printf("            3 m_create_thread_attr                \n");
        printf("            4 m_create_thread_unjoinable          \n");
        printf("            5 m_create_thread_mutex               \n");
        printf("            6 m_set_mutex_para                    \n");
        printf("            7 m_create_thread_cond                \n");
        printf("--------------------------------------------------\n");
        scanf("%d", &select);
        getchar();
        switch(select) {
        case 0:
            exit(0);
        case 1:
            m_create_thread_joinable();
            break;
        case 2:
            m_create_thread_detached();
            break;
        case 3:
            m_create_thread_attr();
            break;
        case 4:
            m_create_thread_unjoinable();
            break;
        case 5:
            m_create_thread_mutex();
            break;
        case 6:
            m_set_mutex_para();
            break;
        case 7:
            m_create_thread_cond();
            break;
        default:
            break;
        }
    }
    return 0;
}