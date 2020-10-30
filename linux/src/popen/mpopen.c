/* 
 *  Copy from popen.c     Written by W. Richard Stevens 
 */  
#include <unistd.h>
#include <sys/wait.h>  
#include <errno.h>  
#include <fcntl.h>
#include <stdlib.h>
#include "mpopen.h"
#include "logutil.h"

#ifdef  OPEN_MAX
static long openmax = OPEN_MAX;
#else
static long openmax = 0;
#endif

/*
 * If OPEN_MAX is indeterminate, we're not
 * guaranteed that this is adequate.
 */
#define OPEN_MAX_GUESS 256

static pid_t    *childpid = NULL;  
                        /* ptr to array allocated at run-time */  
static int      maxfd;  /* from our open_max(), {Prog openmax} */  

#define SHELL   "/bin/sh"  

long open_max(void)
{
    if (openmax == 0) {      /* first time through */
        errno = 0;
        if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
           if (errno == 0)
               openmax = OPEN_MAX_GUESS;    /* it's indeterminate */
           else
               printf("sysconf error for _SC_OPEN_MAX");
        }
    }

    return(openmax);
}

FILE *  m_popen(const char *cmdstring, const char *type)  
{  
    int     i, pfd[2];  
    pid_t   pid;  
    FILE    *fp;  

            /* only allow "r" or "w" */  
    if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0) {  
        errno = EINVAL;     /* required by POSIX.2 */  
        return(NULL);  
    }  

    if (childpid == NULL) {     /* first time through */  
                /* allocate zeroed out array for child pids */  
        maxfd = open_max();
        CLogUtil::info("open_max %d\n", maxfd);
        if ( (childpid = (pid_t *)calloc(maxfd, sizeof(pid_t))) == NULL)  
            return(NULL);  
    }  

    if (pipe(pfd) < 0)  
        return(NULL);        /* errno set by pipe() */  

    if ( (pid = fork()) < 0)  
        return(NULL);        /* errno set by fork() */  
    else if (pid == 0) {     /* child */  
        setpgrp();           /* 设置子进程为同一组 */
        if (*type == 'r') {  
            close(pfd[0]);  
            if (pfd[1] != STDOUT_FILENO) {  
                dup2(pfd[1], STDOUT_FILENO);  
                close(pfd[1]);  
            }  
        } else {  
            close(pfd[1]);  
            if (pfd[0] != STDIN_FILENO) {  
                dup2(pfd[0], STDIN_FILENO);  
                close(pfd[0]);  
            }  
        }  
            /* close all descriptors in childpid[] */  
        for (i = 0; i < maxfd; i++)  
            if (childpid[ i ] > 0)  
                close(i);  

        execl(SHELL, "sh", "-c", cmdstring, (char *) 0);  
        _exit(127);  
    }  
                                /* parent */  
    if (*type == 'r') {  
        close(pfd[1]);  
        if ( (fp = fdopen(pfd[0], type)) == NULL)  
            return(NULL);  
    } else {  
        close(pfd[0]);  
        if ( (fp = fdopen(pfd[1], type)) == NULL)  
            return(NULL);  
    }  
    childpid[fileno(fp)] = pid; /* remember child pid for this fd */  
    return(fp);  
}  

int m_pclose(FILE *fp)  
{  
    int     fd, stat;  
    pid_t   pid;  

    if (childpid == NULL)  
        return(-1);     /* popen() has never been called */  

    fd = fileno(fp);  
    if ( (pid = childpid[fd]) == 0)  
        return(-1);     /* fp wasn't opened by popen() */  

    childpid[fd] = 0;  
    if (fclose(fp) == EOF)  
        return(-1);  

    while (waitpid(pid, &stat, 0) < 0)  
        if (errno != EINTR)  
            return(-1); /* error other than EINTR from waitpid() */  

    return(stat);   /* return child's termination status */  
} 

int m_stop_cmd(FILE *fp)
{
    int     fd;  
    pid_t   pid;  

    if (childpid == NULL)  
        return(-1);     /* popen() has never been called */  

    fd = fileno(fp);  
    if ( (pid = childpid[fd]) == 0)  
        return(-1);     /* fp wasn't opened by popen() */  

    CLogUtil::info("m_stop_cmd %d\n", pid);
    if (pid <= 1) {
        return (-1);
    }

    kill(-pid, SIGTERM);

    return 0;
}