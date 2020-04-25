/**
 * list.h - 双向循环链表 
 * 
 */
#ifndef __LIST_H__
#define __LIST_H__

typedef struct  list_msg {
	int  cmd;
	struct list_msg *next, *prev;
}list_msg_t;


/**
 * list_head_init - 链表初始化
 * @head: 表头 
 */
void list_head_init(list_msg_t *head);

/**
 * list_add_head - 头插
 * @new: 新元素
 * @head: 表头 
 * 
 * 这对于实现堆栈很有用
 */
void list_add_head(list_msg_t *new, list_msg_t *head);

/**
 * list_add_tail - 尾插
 * @new: 新元素
 * @head: 表头 
 * 
 * 这对于实现队列很有用
 */
void list_add_tail(list_msg_t *new, list_msg_t *head);

/**
 * list_del - 删除元素
 * @prev: 要删除元素的前一节点 
 * @next: 要删除元素的后一节点
 * 
 * 如果是最后一个节点，@next = NULL
 */
void list_del(list_msg_t *prev, list_msg_t *next);

/**
 * list_empty - 判断链表是否为空
 * @head: 测试链表
 */
int list_empty(list_msg_t *head);

/**
 * list_is_last - 判断元素是否为最后一个
 * @list: 被测试元素
 * @head: 测试链表
 */
int list_is_last(list_msg_t *list, list_msg_t *head);

/**
 * list_swap - 交换元素
 * @a: 前元素
 * @b: 后元素
 */
void list_swap(list_msg_t *a, list_msg_t *b);

#endif
