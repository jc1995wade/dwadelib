#include "list.h"

void list_head_init(list_msg_t *head)
{
	head->next = head;
	head->prev = head;
}

/**
 * 在任意节点直接插入新元素
 *
 */
static void list_add(list_msg_t *new, list_msg_t *prev, list_msg_t *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

void list_add_head(list_msg_t *new, list_msg_t *head)
{
	list_add(new, head, head->next);
}

void list_add_tail(list_msg_t *new, list_msg_t *head)
{
	list_add(new, head->prev, head);
}

void list_del(list_msg_t *prev, list_msg_t *next)
{
	if (next) {
		prev->next = next;
		next->prev = prev;
	}
	else {	
		prev->next = prev;
	}
}

int list_empty(list_msg_t *head)
{
	return head->next == head;
}

int list_is_last(list_msg_t *list, list_msg_t *head)
{
	return list->next == head;
}

void list_swap(list_msg_t *a, list_msg_t *b)
{
	a->prev->next = b;
	b->prev = a->prev;

	a->next = b->next;
	b->next->prev = a;

	b->next = a;
	a->prev = b;
}


