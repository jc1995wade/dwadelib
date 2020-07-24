#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <mqueue.h>

#define MQ_PATH "/mqtest1"

int main(int argc, char *argv[])
{
    int res;
    int prio;
    mqd_t mq_id;
    char buf[128];
    struct mq_attr new_attr;
   
	mq_id = mq_open(MQ_PATH, O_RDWR);
	if (mq_id == -1) {
		printf("mq_open failed\n");	
	}

    mq_getattr(mq_id, &new_attr);
    printf("%ld, %ld, %ld", new_attr.mq_flags, new_attr.mq_msgsize, new_attr.mq_maxmsg);

    for ( ; ; ) {
        memset(buf, 0, 2048);
        res = mq_receive(mq_id, buf, new_attr.mq_msgsize, &prio);
        printf("res=%d, buf=%s\n", res, buf);
        sleep(1);
    }

    return 0;
}
