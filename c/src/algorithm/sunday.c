#include <stdio.h>
#include <string.h>


int * getStep(char *a, char *b)
{
    static int step[256];
    for (int i=0;i<256;i++){
        step[i]=strlen(b)+1;
    }
    int sublen = strlen(b);
    for (int i=0;i<sublen;i++){
        step[b[i]]=sublen-i;
    }
    return step;
}

int sundaySearch(char *mainstr, char *substr, int *step)
{
    int i=0,j=0;
    int mainlen=strlen(mainstr);
    int sublen=strlen(substr);    
    while (i<mainlen){
        int temp = i;
        while (j<sublen){
            if (mainstr[i]==substr[j]){
                i++;
                j++;
                continue;
            }
            else {
                i = temp + step[mainstr[temp+sublen]];
                if (i+sublen>mainlen) return -1;
                j=0;
                break;
            }
        }
        if (j==sublen) return temp;
    }
}

int main(void)
{
	int ret;
    char a[]="LESSONS TEARNED IN SOFTWARE TE";
    char b[]="SO";
    int * step = getStep(a,b);
    ret = sundaySearch(a,b,step);
	printf("%d\n", ret);
}