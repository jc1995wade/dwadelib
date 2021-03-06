#include <stdio.h>
#include <stdlib.h>
#include "list.h"

list_msg_t head;

list_msg_t *node_create(int data)
{
	list_msg_t *msg;
	msg = malloc(sizeof(list_msg_t));
	msg->cmd = data;
	return msg;
}

void node_add(list_msg_t *head, list_msg_t *node)
{
	list_add_tail(node, head);
}

list_msg_t *node_find(list_msg_t *head, int data)
{
	list_msg_t *tmp = head->next;
	while (tmp != head) {
		if (tmp->cmd == data) {
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

void node_del(list_msg_t *head, int data)
{
	list_msg_t *msg;
	msg = node_find(head, data);
	if (msg) {
		list_del(msg->prev, msg->next);
		free(msg);
	}
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
		node_add(&head, node_create(i));
	}

	// swap
	a = node_find(&head, 8);
	b = node_find(&head, 9);
	list_swap(a, b);

	// delete
	node_del(&head, 19);
	node_del(&head, 15);
	node_del(&head, 10);

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



