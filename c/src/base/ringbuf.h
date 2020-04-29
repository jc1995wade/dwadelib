#ifndef __RINGBUF_H__
#define __RINGBUF_H__

typedef struct ringbuf {
	unsigned char *buffer;
	unsigned int size;
	unsigned int in;
	unsigned int out;
	pthread_mutex_t mutex;
}ringbuf_t;


ringbuf_t *ringbuf_create(unsigned int size);

void ringbuf_free(ringbuf_t *rb);

unsigned int ringbuf_available(ringbuf_t *rb);

unsigned int ringbuf_put(ringbuf_t *rb, unsigned char *buf, unsigned int len);

unsigned int ringbuf_get(ringbuf_t *rb, unsigned char *buf, unsigned int len);



#endif
