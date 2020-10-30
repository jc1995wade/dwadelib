#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

/* �����ѳ�ʼ�����ڴ�*/
void *get_memory(int size);

/* �ͷ��ڴ� */
void free_memory(void *p);

/* ��ȡϵͳʱ�� */
int get_systime(char *out, int fmt);

int system_call(const char *cmd, char *out);

void net_ulong_to_char(char *p_szt, unsigned long ul_value);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* END _COMMON_H_ */
