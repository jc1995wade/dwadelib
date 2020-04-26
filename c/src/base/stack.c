#include <stdio.h>
#include <stdlib.h>
#include "list.h"

list_msg_t head;

list_msg_t *stack_create(int data)
{
	list_msg_t *msg;
	msg = malloc(sizeof(list_msg_t));
	msg->cmd = data;
	return msg;
}

void stack_push(list_msg_t *head, list_msg_t *node)
{
	list_add_head(node, head);
}

list_msg_t *stack_pop(list_msg_t *head)
{
	list_msg_t *tmp;

	if (list_empty(head)) {
		return NULL;
	}
	tmp = head->next;
	list_del(head, head->next->next);

	return tmp;
}

void destory_list(list_msg_t *head)
{
	list_msg_t *tmp, *del;
	
	tmp = head->prev;
	while (tmp != head) {
		del = tmp;
		tmp = tmp->prev;
		free(del);
	}
	list_head_init(head);
}

int main(void)
{
	int i;
	list_msg_t *tmp;
	list_msg_t *a, *b;

	// init
	list_head_init(&head);

	// add
	for(i=0; i<20; i++) {
		stack_push(&head, stack_create(i));
	}

	// out
	for(i=0; i<10; i++) {
		tmp = stack_pop(&head);
		if (!tmp) break;
		free(tmp);
	}

	// add
	for(i=0; i<10; i++) {
		stack_push(&head, stack_create(i+100));
	}

	// print
	tmp = head.next;
	while (tmp != &head) {
		printf("cmd=%d, last=%d, empty=%d\n", \
					tmp->cmd, \
					list_is_last(tmp, &head),
					list_empty(&head));
		tmp = tmp->next;
	}
	destory_list(&head);
	printf("empty=%d\n", list_empty(&head));
	return 0;
}


