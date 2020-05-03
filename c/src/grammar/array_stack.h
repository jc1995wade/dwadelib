#ifndef __ARRAY_STACK_H__
#define __ARRAY_STACK_H__

#define MIN_STACK_SIZE  (128)
#define EMPTY_STACK  (-1)

typedef char  array_t; 

typedef struct stack_record {
    int capacity;
    int top;
    array_t *array;
}stack_t;

int stack_create(stack_t *s);

int stack_destory(stack_t *s);

int stack_is_empty(stack_t *s);

int stack_is_full(stack_t *s);

int stack_pop(stack_t *s, array_t *data);

int stack_push(stack_t *s, array_t data);


#endif
