#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 申请已初始化的内存*/
void *get_memory(int size);

/* 释放内存 */
void free_memory(void *p);

/* 获取系统时间 */
int get_systime(char *out, int fmt);

int system_call(const char *cmd, char *out);

void net_ulong_to_char(char *p_szt, unsigned long ul_value);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* END _COMMON_H_ */
