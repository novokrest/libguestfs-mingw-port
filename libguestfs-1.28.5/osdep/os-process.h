#ifndef OS_PROCESS_INFO_H_
#define OS_PROCESS_INFO_H_

#if defined _WIN32

#include <windows.h>

#define os_process_info_t PROCESS_INFORMATION

#else

#include <sys/types.h>

#define os_process_info_t pid_t

#endif /* _WIN32 */

void os_process_info__init (os_process_info_t *proc);
void os_process_info__close (os_process_info_t *proc);
void os_process_info__set_default (os_process_info_t *proc);
int os_process_info__is_valid (os_process_info_t *proc);
int os_process_info__get_id (os_process_info_t *proc);
int os_process_info__kill (os_process_info_t *proc, int signal);
int os_process_info__wait (os_process_info_t *proc, int *status, int options);

#endif /* OS_PROCESS_INFO_H_ */