#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int i = 0;
    char *env;

    extern char **environ;
    while(environ[i]) {
        printf("%s\n", environ[i]);
        i++;
    }
    
    env = getenv("PWD");
    printf("env=%s\n", env);

    return 0;
}
