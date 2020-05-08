/**
 * POSXI sem
 *
 *
 */


#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdlib.h>


#define SEM_KEY "mysem"

int main(void)
{
	sem_t *sem;

	sem = sem_open(SEM_KEY, O_CREAT, 0666, 0);

	printf("post sem\n");
	sem_post(sem);
	printf("post sem ok\n");

}
