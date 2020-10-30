#ifndef __MPOPEN_H__
#define __MPOPEN_H__
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

FILE *  m_popen(const char *cmdstring, const char *type);

int m_pclose(FILE *fp);

int m_stop_cmd(FILE *fp);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __MPOPEN_H__ */