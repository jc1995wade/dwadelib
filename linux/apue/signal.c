#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static void sig_usr(int);


int main(void)
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        printf("cant't catch SIGUSR1");
    if (signal(SIGUSR2, sig_usr) == SIG_ERR) 
        printf("cant't catch SIGUSR2");

    for ( ; ; )
        pause();
}


static void sig_usr(int signo)
{
    if (signo == SIGUSR1)
        printf("recevied SIGUSR1\n");
    else if ( signo == SIGUSR2)
        printf("receviced SIGUSR2\n");
    else
        printf("other sig =%d\n" , signo);
    return ;
}
