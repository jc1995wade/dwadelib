#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct person 
{ 
    /* data */
    int age; 
    int weight; 
    
    /* list */
    struct list_head list; 
}; 
 
int main(int argc, char* argv[]) 
{ 
    struct person *tmp; 
    struct list_head *pos; 
    int age_i, weight_j; 
 
    struct person person_node; 
    INIT_LIST_HEAD( &(person_node.list) ); 
 
    for(age_i = 10, weight_j = 35; age_i < 40; age_i += 5, weight_j += 5) 
    { 
        tmp =(struct person*)malloc(sizeof(struct person)); 
        tmp->age = age_i; 
        tmp->weight = weight_j; 
        /* 头插 */
        list_add( &(tmp->list), &(person_node.list) ); 
    } 
 
    // 下面把这个链表中各个节点的值打印出来 
    printf("\n"); 
    printf("=========== print the list ===============\n"); 
    list_for_each( pos, &(person_node.list) )
    { 
        // 这里我们用list_entry来取得pos所在的结构的指针 
        tmp = list_entry(pos, struct person, list); 
        printf("age:%d, weight: %d \n", tmp->age, tmp->weight); 
    } 
    printf("\n"); 
 
    // 下面删除一个节点中，age为20的节点 
    printf("========== print list after delete a node which age is 20 ==========\n"); 
    struct list_head *del_tmp; 
    list_for_each_safe( pos, del_tmp, &(person_node.list) )
    { 
 
        tmp = list_entry(pos, struct person, list); 
        if(tmp->age == 20) 
        { 
            list_del_init(pos); 
            free(tmp); 
        } 
 
    } 
 
    list_for_each( pos, &(person_node.list) )
    { 
        tmp = list_entry(pos, struct person, list); 
        printf("age:%d, weight: %d \n", tmp->age, tmp->weight); 
    } 
 
    // 释放资源 
    list_for_each_safe( pos, del_tmp, &(person_node.list) ) 
    { 
        tmp = list_entry(pos, struct person, list); 
        list_del_init(pos); 
        free(tmp); 
    } 
 
       return 0; 
}

/**————————————————
 * 版权声明：本文为CSDN博主「江山美人1」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
 * 原文链接：https://blog.csdn.net/u011285208/article/details/92733532
 */