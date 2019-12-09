#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    int ret = 0;
    int select = 0;
    
    while(1) {
        printf("-------------------------------------\n");
        printf("            0 exit                   \n");
        printf("            1 test                   \n");
        printf("-------------------------------------\n");
        scanf("%d", &select);
        getchar();
        switch(select) {
        case 0:
            exit(0);
        case 1:
            printf("hello world\n");
            break;
        default:
            break;
        }
    }
    return 0;
}