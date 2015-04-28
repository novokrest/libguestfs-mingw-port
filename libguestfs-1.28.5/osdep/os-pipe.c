#include "os-pipe.h"

#ifdef _WIN32

int
os_pipe_end__is_valid (HANDLE pipe_end)
{
  return pipe_end != NULL;
}

int
os_pipe_end__close (HANDLE pipe_end)
{
  return CloseHandle (pipe_end) ? 0 : -1;
}

#else

#include <unistd.h>

int
os_pipe_end__is_valid (int pipe_end)
{
  return pipe_end >= 0;
}

int
os_pipe_end__close (int pipe_end)
{
  return close (pipe_end);
}

#endif /* _WIN32 */
