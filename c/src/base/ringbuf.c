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

unsigned int ringbuf_put(ringbuf_t *rb, unsigned char *buf, unsigned int len)
{
	unsigned int length;
	pthread_mutex_lock(&rb->mutex);

	len = min(len, rb->size - rb->in + rb->out); //buffer中空数据的长度
	
	length = min(len, rb->size - (rb->in & (rb->size -1)));
	memcpy(rb->buffer + (rb->in & rb->size -1), buf, length);
	memcpy(rb->buffer, buf + length, len - length);

	rb->in += len;
	pthread_mutex_unlock(&rb->mutex);
	return len;
}

unsigned int ringbuf_get(ringbuf_t *rb, unsigned char *buf, unsigned int len)
{
	unsigned int length;
	pthread_mutex_lock(&rb->mutex);

	len = min(len, rb->in + rb->out);
	
	length = min(len, rb->size - (rb->out & (rb->size - 1)));
	memcpy(buf, rb->buffer + (rb->out & (rb->size - 1)), length);
	memcpy(buf + length, rb->buffer, len - length);
	
	rb->out += len;
	if (rb->in == rb->out)
		rb->in = rb->out = 0;
	pthread_mutex_unlock(&rb->mutex);
	return len;
}


