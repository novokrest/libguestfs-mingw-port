#ifdef _WIN32

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "win-port.h"

int 
vasprintf (char** __restrict ret, const char* __restrict format, va_list ap)
{
  int len;

  len = _vscprintf (format, ap);
  if (len < 0) {
    return -1;
  }

  *ret = (char*) malloc (len + 1);
  if (!*ret) {
    return -1;
  }

  vsnprintf (*ret, len + 1, format, ap);
  //(*ret)[len] = '\0';

  return len;
}

int 
asprintf (char** __restrict ret, const char* __restrict format, ...)
{
  va_list ap;
  int len;

  va_start (ap, format);
  len = vasprintf (ret, format, ap);
  va_end (ap);

  return len;
}

#endif /* _WIN32 */