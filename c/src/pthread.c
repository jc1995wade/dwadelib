#include <stdio.h>
#include <error.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* thread_run_1(void* _val)
{
    printf("%s\n", (char*)_val);
    sleep(2);
    printf("%s\n", (char*)_val);
    pthread_exit("thread_run_1 exit"); //或者使用return NULL退出
}

void* thread_run_2(void* _val)
{
    while(1) {
        printf("%s\n", (char*)_val);
        sleep(1);
    }
    
    return NULL;
}

void* thread_run_3(void* _val)
{
    printf("%s\n", (char*)_val);
    sleep(1);
    printf("thread_run_3 exit\n");
    return NULL;
}

void* thread_run_4(void* _val)
{
    pthread_detach(pthread_self());// 设置分离线程，线程退出后由系统回收
    printf("%s\n", (char*)_val);
    sleep(1);
    printf("thread_run_4 exit\n");
    return NULL;
}


int main(int argc, char *argv[])
{
    int ret;
#if 0
    //1.默认创建可结合的（joinable），需要pthread_join()来回收资源
    pthread_t tidp;
    ret = pthread_create(&tidp, NULL, thread_run_1, "thread 1 run ...");
    if (-1 == ret) {
        printf("create error!\n");
        return 1;
    }
    printf("wait ...\n");
    char *tret = NULL;
    pthread_join(tidp, (void**)&tret);
    printf("tread_run ret=%s\n", tret);
#endif
#if 0
    //2.可以调用pthread_detach(pthread_self())回收资源
    pthread_t pid_2;
    ret = pthread_create(&pid_2, NULL, thread_run_2, "thread 2 run ...");
    if (-1 == ret) {
        printf("create error!\n");
        return 1;
    }
    printf("wait ...\n");
    sleep(3);
    pthread_cancel(pid_2);
    pthread_join(pid_2, NULL);
#endif
#if 0
    //3.设置线程属性为detach，在线程结束时，由系统释放线程资源
    pthread_t pid_3;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create( &pid_3, &attr, thread_run_3, "thread 3 run ...");
    if (-1 == ret) {
        printf("create error!\n");
        return 1;
    }
    printf("wait ...\n");
    sleep(3);
#endif
    //4.状态改为unjoinable状态，确保资源的释放
    pthread_t pid_4;
    ret = pthread_create(&pid_4, NULL, thread_run_4, "thread 4 run ...");
    if (-1 == ret) {
        printf("create error!\n");
        return 1;
    }
    printf("wait ...\n");
    sleep(3);

    printf("thread exit\n");
    return 0;
}