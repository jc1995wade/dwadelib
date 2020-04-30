#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "sunday.h"

extern int * get_step(int *step, char *a, char *b);
extern int sunday_search(char *mainstr, char *substr, int *step);

int main(int argc, char *argv[])
{
	int i = 1;
	int ret;
	FILE *fp = NULL;
	char *fret = NULL;
	int step[256];
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

	fclose(fp);
	return 0;
}


