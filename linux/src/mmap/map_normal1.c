/*-------------map_normalfile1.c-----------*/
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

typedef struct{
    char name[4];
    int age;
}people;

int main(int argc, char *argv[])//map a normal file as shared mem:
{
    int fd,i;
    people *p_map;
    char temp;
    fd = open(argv[1], O_CREAT|O_RDWR|O_TRUNC, 00777);
    lseek(fd, sizeof(people)*5-1, SEEK_SET);
    write(fd, "", 1);
    p_map=(people*)mmap(NULL, sizeof(people)*10, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (p_map == (void *)-1) {
        fprintf(stderr, "mmap: %s\n", strerror(errno));
        return -1;
    }
    close(fd);
    temp='a';
    for(i=0; i<10; i++){
        temp+=1;
        (*(p_map+i)).name[1] = '\0';
        memcpy((*(p_map+i)).name,&temp,1);
        (*(p_map+i)).age=20+i;
    }
    printf("initializeover\n");
    sleep(10);
    munmap(p_map, sizeof(people)*10);
    printf("umapok\n");
    return 0;
}
