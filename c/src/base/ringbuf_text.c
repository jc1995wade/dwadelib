#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "ringbuf.h"

ringbuf_t *ring_buf;


static void* thread_write(void* _val)
{
	unsigned int len;
	char *buffer = "abc";
	while(1) {
		sleep(1);
		len = ringbuf_put(ring_buf, buffer, 3);
		printf("write [%u][%s], in[%u], out[%u]\n", len, buffer, ring_buf->in, ring_buf->out);
	}

	pthread_exit("thread_write exit");
}

static void* thread_read(void* _val)
{
	unsigned int len;
	unsigned char buffer[10];
	sleep(3);
	while(1) {
		sleep(2);
		memset(&buffer, 0, sizeof(buffer));
		len = ringbuf_get(ring_buf, buffer, 2);
		printf("read [%u][%s],  in[%u], out[%u]\n", len, buffer, ring_buf->in, ring_buf->out);
	}
	
	pthread_exit("thread_read exit");
}


int main(void)
{
	int ret;
    pthread_t pid_1;
	pthread_t pid_2;

	ring_buf = ringbuf_create(8);
	if (!ring_buf) {
		printf("ring_buf create error\n");
		return -1;
	}
	
	ret = pthread_create(&pid_1, NULL, thread_write, "thread write run ...");
    if (-1 == ret) {
        printf("create error!\n");
        return -1;
    }

	ret = pthread_create(&pid_2, NULL, thread_read, "thread read run ...");
    if (-1 == ret) {
        printf("create error!\n");
        return -1;
    }

	pthread_join(pid_1, NULL);
	pthread_join(pid_2, NULL);
	ringbuf_free(ring_buf);
	return 0;
}
