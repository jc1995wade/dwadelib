#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "array_stack.h"

/**
 * () [] {} <> "" '' 
 *
 *
 */

char bs[10][2] = { '(', ')', '[', ']', '<', '>', '\"', '\'', 0, 0};

void set_bs_flag(int *flag)
{
    flag['('] = 1;
    flag[')'] = 2;
    flag['['] = 1;
    flag[']'] = 2;
    flag['{'] = 1;
    flag['}'] = 2;
    flag['<'] = 1;
    flag['>'] = 2;
    //flag['\"'] = 1;
    //flag['\''] = 1;

}



int main(int argc, char *argv[])
{
    int i = 1;
	int ret;
	FILE *fp = NULL;
	char *fret = NULL; 
	char line[1024];
    int bs_flag[256];
    char data;
    
    stack_t stack;
    stack_create(&stack);

	if (argc < 2) {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		return -1;
    }

	if (access(argv[1], F_OK|W_OK) == -1) {
		printf("File does not exist.\n");
		return -1;
	}

	fp = fopen(argv[1], "r");
	if (NULL == fp) {
		printf("Error opening file.\n");
		return -1;
	}
	
    memset(bs_flag, 0, sizeof(bs_flag));
    
    set_bs_flag(bs_flag);

    char *temp;
	do {
		memset(line, 0, sizeof(line));
		fret = fgets(line, sizeof(line), fp);
		if (!fret) {
			break;
		}
        temp = line;
        printf("line=%s", line);
        while(*temp) {
            switch(bs_flag[*temp]) {
            case 1:
                printf("push=%c\n", *temp);
                stack_push(&stack, *temp);
                break;
            case 2:
       
                if (stack_is_empty(&stack)) {
                    printf("code is fail\n");
                    goto OUT;
                }
                stack_pop(&stack, &data);
                printf("pop =%c\n", data);
                break;
            default:
                break;
            }

            temp++;
        }
	} while(fret);

    if (stack_is_empty(&stack)) 
        printf("code is ok\n");
    else 
        printf("code fail\n");

OUT:
    stack_destory(&stack);
    return 0;
}

#if 0
int main(int argc, char *argv[])
{
	int i = 1;
	int ret;
	FILE *fp = NULL;
	char *fret = NULL; int step[256];
	char line[1024];

	if (argc < 3) {
		fprintf(stderr, "Usage: %s \"abc\" filename\n", argv[0]);
		return -1;
	}

	if (access(argv[2], F_OK|W_OK) == -1) {
		printf("File does not exist.\n");
		return -1;
	}

	fp = fopen(argv[2], "r");
	if (NULL == fp) {
		printf("Error opening file.\n");
		return -1;
	}
	
	do {
		memset(line, 0, sizeof(line));
		fret = fgets(line, sizeof(line), fp);
		if (!fret) {
			break;
		}
		get_step(step, fret, argv[1]);
		ret =  sunday_search(fret, argv[1], step);
		if (ret != -1) {
			printf("%d %s", i, line);
		}
		i++;
	} while(fret);
#endif


