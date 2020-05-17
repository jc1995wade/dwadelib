#include <stdio.h>


int cmp_fun(const void *a, const void *b)
{
	return ( *(int*)a - *(int*)b );
}



int main(void)
{
	int n;
	int values[] = {10, 88, 45, 0, 34, 100, 30, 10, 2, 50};

	for(n=0; n<10; n++) {
		printf("%d ", values[n]);
	}
	printf("\n");

	qsort(values, 10, sizeof(int), cmp_fun);
	
	for(n=0; n<10; n++) {
		printf("%d ", values[n]);
	}
	printf("\n");


	return 0;
}
