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

	printf("wait sem\n");
	sem_wait(sem);
	printf("wait sem ok\n");

	sem_close(sem);
	sem_unlink(SEM_KEY);
}
