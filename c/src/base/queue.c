#include <stdio.h>
#include <stdlib.h>
#include "list.h"

list_msg_t head;

void queue_init(list_msg_t *head)
{
	list_head_init(head);
}

list_msg_t *queue_create(int data)
{
	list_msg_t *msg;
	msg = malloc(sizeof(list_msg_t));
	msg->cmd = data;
	return msg;
}

void queue_in(list_msg_t *head, list_msg_t *new)
{
	list_add_tail(new, head);
}

list_msg_t *queue_out(list_msg_t *head)
{
	list_msg_t *tmp;

	if (list_empty(head)) {
		return NULL;
	}
	tmp = head->next;
	list_del(head, head->next->next);

	return tmp;
}

int queue_empty(list_msg_t *head)
{
	return list_empty(head);
}

void queue_destory(list_msg_t *head)
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

void main(void)
{
	int i;
	list_msg_t *tmp;
	list_msg_t *a, *b;

	queue_init(&head);

	// int
	for(i=0; i<20; i++) {
		queue_in(&head, queue_create(i));
	}

	// out
	for(i=0; i<10; i++) {
		tmp = queue_out(&head);
		if (!tmp) break;
		free(tmp);
	}

	// int
	for(i=0; i<2; i++) {
		queue_in(&head, queue_create(i));
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
}


