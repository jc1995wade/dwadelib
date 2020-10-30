#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include "common.h"
#include "logutil.h"

void *get_memory(int size)
{
    void *p = malloc(size);
    if (p == NULL) {
        return NULL;
    }

    memset(p, 0, size);
    return p;
}

void free_memory(void *p)
{
    free(p);
}

/*
 *   功能：      获取当前系统时间
 *   返回值：    0-成功，-1-失败
 *   out:       保存返回的系统时间，格式由fmt决定
 *   fmt:       0-返回：yyyy-mm-dd hh24:mi:ss, 1-返回：yyyy-mm-dd, 2-返回：hh24:mi:ss
 */
int get_systime(char *out, int fmt)
{
    if(out == NULL) {
        return -1;
    }
 
    time_t t;
    struct tm *tp;
    t = time(NULL);
 
    tp = localtime(&t);
    if(fmt == 0) {
        sprintf(out, "%2.2d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d", tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);
    } else if(fmt == 1) {
        sprintf(out, "%2.2d-%2.2d-%2.2d", tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday);
    } else if(fmt == 2) {
        sprintf(out, "%2.2d:%2.2d:%2.2d", tp->tm_hour, tp->tm_min, tp->tm_sec);
    } else {
        return -1;
    }

    return 0;
}

/*
*      执行shell命令，并且返回执行结果
*/
int system_call(const char *cmd, char *out)
{
    FILE *fp = NULL;
    char temp[1024] = {0};

    fp = popen(cmd, "r");
    if (fp == NULL) {
        CLogUtil::info("popen failed\n");
        return -1;
    } else {
        if (fread(temp, sizeof(char), sizeof(temp) - 1, fp) == 0) {
            CLogUtil::info("Nothing to return cmd", cmd);
        }

        pclose(fp);

        memcpy(out, temp, strlen(temp));
    }

    return 0;
}

void net_ulong_to_char(char *p_szt, unsigned long ul_value)
{
	if (p_szt == NULL) {
		CLogUtil::warn(_T("ulong_to_char, wrong params."));
		return;
	}

	struct in_addr ip_addr;
	ip_addr.s_addr = ul_value;
	strncpy(p_szt, inet_ntoa(ip_addr), MAX_PATH);
}
