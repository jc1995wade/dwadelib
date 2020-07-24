#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <mqueue.h>

#define MQ_PATH "/mqtest1"
int main(int argc, char *argv[])
{
    mqd_t mq_id;

    mq_unlink(MQ_PATH);
	mq_id = mq_open(MQ_PATH, O_WRONLY | O_CREAT |O_NONBLOCK, S_IRWXU | S_IRWXG, NULL);
	if (mq_id == -1) {
		printf("mq_open failed\n");	
	}

    struct mq_attr new_attr;
    mq_getattr(mq_id, &new_attr);
    printf("%ld, %ld, %ld\n", new_attr.mq_flags, new_attr.mq_msgsize, new_attr.mq_maxmsg);

    while (1) {
        if (mq_send(mq_id, "hello", 6, 1) == -1) {
            printf("mq_send failed \n");
        } else {
            printf("send ok\n");
        }
        sleep(2); 
    }

    return 0;
}
