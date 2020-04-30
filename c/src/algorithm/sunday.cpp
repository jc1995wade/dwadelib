#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int * getStep(int *step, char *a, char *b)
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

int sundaySearch(char *mainstr, char *substr, int *step)
{
    int i=0,j=0;
	int temp;
    int mainlen=strlen(mainstr);
    int sublen=strlen(substr);    
    while (i<mainlen){
        temp = i;
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

int main(int argc, char *argv[])
{
	int ret;
    char a[]="LESSONS TEARNED IN SOFTWARE TE";
    char b[]="SOFTWsdjfARE";
	int step[256];
	FILE *fp = NULL;
	char *fret = NULL;
	int i = 0;
	char line[1024];
	
	access(argv[1], F_OK|W_OK);
	printf("argv[1]=%s\n", argv[1]);

	fp = fopen(argv[1], "r");
	if (NULL == fp) {
		printf("Error opening file.\n");
		return -1;
	}
	
	do {
		memset(line, 0, sizeof(line));
		fret = fgets(line, sizeof(line), fp);
		printf("%d %s", i++, line);
	} while(fret);

	getStep(step, a, b);
    ret =  sundaySearch(a, b, step);
	printf("ret=%d\n", ret);

	fclose(fp);
	return 0;
}


