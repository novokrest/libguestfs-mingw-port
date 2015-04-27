#include <string.h>
#include <errno.h>

#include "win-port.h"

#if defined __MINGW32__

int 
strerror_r (int errnum, char *buf, size_t buflen)
{
  char *error_msg;

  error_msg = strerror (errno);
  strncpy (buf, error_msg, buflen);

  return 0;
}

#elif defined _WIN32

int 
strerror_r (int errnum, char *buf, size_t buflen)
{
  return strerror_s (buf, buflen, errnum);
}

#endif /* _WIN32 */