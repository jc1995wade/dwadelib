#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int system_call(const char *cmd, char *out)
{
    FILE *fp = NULL;
    char temp[1024] = {0};
    int i = 0;

    fp = popen(cmd, "r");
    if (fp == NULL) {
        printf("popen failed\n");
        return -1;
    }
	while(1) {
		if (fread(&temp[i], sizeof(char), 1, fp) == -1) {
			printf("fread %s failed\n", cmd);
			pclose(fp);
		}
		
		if (i > 1023 || temp[i] == '\n') {
			printf("%s", temp);
			memset(temp, 0, sizeof(temp));
			i = 0;
		} else {
			i++;
		}
	}
	
	pclose(fp);
    return 0;
}

int main(int argc, char *argv[])
{
	char out[1024];

	system_call("ip monitor route", out);

	return 0;
}
