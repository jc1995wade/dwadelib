#include <stdio.h>
#include <stdlib.h>
#include "array_stack.h"


int stack_create(stack_t *s)
{
    s->array = malloc(sizeof(stack_t));
    if (!s->array) {
        return -1;
    }
    s->capacity = MIN_STACK_SIZE;
    s->top = EMPTY_STACK;
    return 0;
}

int stack_destory(stack_t *s)
{
    free(s->array);
    s->array = NULL;
    s->capacity = MIN_STACK_SIZE;
    s->top = EMPTY_STACK;
    return 0;
}

int stack_is_empty(stack_t *s)
{
    printf("stack status=%d\n", (s->top == EMPTY_STACK));
    return (s->top == EMPTY_STACK);
}

int stack_is_full(stack_t *s)
{
    return (s->top == MIN_STACK_SIZE);
}

int stack_pop(stack_t *s, array_t *data)
{
    if (stack_is_empty(s)) {
        printf("stack is empty\n");
        return -1;
    }
    
    *data = s->array[s->top];
    s->top--;
    return 0;
}

int stack_push(stack_t *s, array_t data)
{
    if (stack_is_full(s)) {
        printf("stack is full\n");
        return -1;
    }
    s->top++;
    s->array[s->top] = data;
    return 0;
}



