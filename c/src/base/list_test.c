#include <stdio.h>
#include <stdlib.h>
#include "list.h"

list_msg_t head;

int main(void)
{
	int i;
	list_msg_t *msg, *tmp;

	// init
	list_head_init(&head);

	// add
	for(i=0; i<20; i++) {
		msg = malloc(sizeof(list_msg_t));
		msg->cmd = i;
		list_add_tail(msg, &head);
	}

	// delete
	tmp = head.next;
	while (tmp != &head) {
		if (5 == tmp->cmd) {
			list_del(tmp->prev, tmp->next);
			free(tmp);
			break;
		}
		tmp = tmp->next;
	}

	// swap
	tmp = head.next->next;
	list_swap(tmp, tmp->next);

	// print
	tmp = head.next;
	while (tmp != &head) {
		printf("cmd=%d, last=%d, empty=%d\n", \
					tmp->cmd, \
					list_is_last(tmp, &head),
					list_empty(&head));
		tmp = tmp->next;
	}

    return 0;
}



