#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int i;
	int an1, an2, tmp;
	an1 = an2 = 1;
	printf("1\n1\n");
	for(i=2; i<10; i++) {
		printf("%d\n", an1+an2);
		tmp = an2;
		an2 += an1;
		an1 = tmp;
	}


	return 0;
}



