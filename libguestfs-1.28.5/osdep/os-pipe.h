#ifndef OS_PIPE_H_
#define OS_PIPE_H_

#include <stdlib.h>

#ifdef _WIN32

#include <windows.h>

#define os_pipe_end_t HANDLE
#define OS_PIPE_END_INVALID_VALUE NULL

#else

#define os_pipe_end_t int
#define OS_PIPE_END_INVALID_VALUE -1

#endif /* _WIN32 */

int os_pipe_end__is_valid (os_pipe_end_t pipe_end);
int os_pipe_end__close (os_pipe_end_t pipe_end);

#endif /* OS_PIPE_H_ */