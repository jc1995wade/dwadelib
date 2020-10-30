#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#ifdef __cplusplus
extern "C" {
#endif

int hardware_info_get(char *info, int len);

int hardware_hostname_get(char *hostname, int len);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* END _HARDWARE_H_ */
