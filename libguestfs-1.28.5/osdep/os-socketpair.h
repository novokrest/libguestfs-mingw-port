#ifndef OS_SOCKETPAIR_H_
#define OS_SOCKETPAIR_H_

#include <stdlib.h>

#ifdef _WIN32

#include <windows.h>

#define os_socketpair_end_t HANDLE
#define OS_SOCKETPAIR_END_INVALID_VALUE NULL

#else

#define os_socketpair_end_t int
#define OS_SOCKETPAIR_END_INVALID_VALUE -1

#endif /* _WIN32 */

struct os_socketpair
{
  os_socketpair_end_t left;
  os_socketpair_end_t right;
};

typedef struct os_socketpair os_socketpair_t;

void os_socketpair__close (struct os_socketpair *sp);
int os_socketpair_end__is_valid (os_socketpair_end_t sp_end);
int os_socketpair_end__close (os_socketpair_end_t sp_end);

#endif /* OS_SOCKETPAIR_H_ */