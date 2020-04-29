#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "ringbuf.h"
#include <stdio.h>
#define is_power_of_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))

#define min(a, b) ((a) < (b) ? (a) : (b))


ringbuf_t *ringbuf_create(unsigned int size)
{
	char *buffer;
	ringbuf_t *rbuf;

	if (!is_power_of_2(size))
		return NULL;
		
	buffer = malloc(size*(sizeof(char)));
	rbuf = malloc(sizeof(ringbuf_t));

	rbuf->buffer = buffer;
	memset(rbuf->buffer, 0, size);
	rbuf->size = size;
	rbuf->in = rbuf->out = 0;
	pthread_mutex_init(&rbuf->mutex, NULL);
	return rbuf;
}

void ringbuf_free(ringbuf_t *rb)
{
	pthread_mutex_destroy(&rb->mutex);
	free(rb->buffer);
	free(rb);
}

unsigned int ringbuf_available(ringbuf_t *rb)
{
	return (rb->size - rb->in + rb->out);
}

unsigned int ringbuf_put(ringbuf_t *rb, unsigned char *buf, unsigned int len)
{
	unsigned int after_l;
	unsigned int put_data_l;
	pthread_mutex_lock(&rb->mutex);

	// UTF-8
	// 实际可写入的数据长度
	put_data_l = min(len, rb->size - rb->in + rb->out); 

	// in到缓冲区尾部长度
	after_l = min(put_data_l, rb->size - (rb->in & (rb->size - 1)));

	// 第一次写入数据直到缓冲区尾部
	memcpy(rb->buffer + (rb->in & (rb->size - 1)), buf, after_l);

	// 从缓冲区起始位置开始写入数据
	memcpy(rb->buffer, buf + after_l, put_data_l - after_l);

	rb->in += put_data_l;
	pthread_mutex_unlock(&rb->mutex);
	return put_data_l;
}

unsigned int ringbuf_get(ringbuf_t *rb, unsigned char *buf, unsigned int len)
{
	unsigned int after_l;  
	unsigned int get_data_l;   
	pthread_mutex_lock(&rb->mutex);

	// 实际可获取的数据长度
	get_data_l = min(len, rb->in - rb->out); 

	// out位置到缓冲区尾部长度
	after_l = min(get_data_l, rb->size - (rb->out & (rb->size - 1)));

	// 第一次获取数据直到缓冲区尾部
	memcpy(buf, rb->buffer + (rb->out & (rb->size - 1)), after_l);

	// 从缓冲区起始位置开始获取数据
	memcpy(buf + after_l, rb->buffer, get_data_l - after_l);
	
	rb->out += get_data_l;
	if (rb->in == rb->out) {
		rb->in = rb->out = 0;
	}
	pthread_mutex_unlock(&rb->mutex);
	return after_l;
}


