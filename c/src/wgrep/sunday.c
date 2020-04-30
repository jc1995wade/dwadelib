#include <string.h>


int *get_step(int *step, char *a, char *b)
{
	int i, sublen;
    for (i=0; i<256; i++) {
        step[i] = strlen(b) + 1;
    }
    sublen = strlen(b);
    for (i=0; i<sublen; i++) {
        step[b[i]] = sublen - i;
    }
    return step;
}

int sunday_search(char *mainstr, char *substr, int *step)
{
    int i = 0,j = 0;
	int temp;
    int mainlen = strlen(mainstr);
    int sublen = strlen(substr);    
    while (i < mainlen) {
        temp = i;
        while (j < sublen) {
            if (mainstr[i] == substr[j]) {
                i++;
                j++;
                continue;
            }
            else {
                i = temp + step[mainstr[temp + sublen]];
                if (i+sublen > mainlen) 
					return -1;
                j = 0;
                break;
            }
        }
        if (j == sublen) 
			return temp;
    }    
}



