#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "os-fcntl.h"

#ifdef _WIN32

#include <win-defs.h>

int
os_open (char const *pathname, int flags, ...)
{
  int mode, pmode, oflags;
  va_list vl;

  oflags = flags | O_BINARY;
  
  if (oflags & O_CREAT) {
    va_start (vl, flags);
    mode = va_arg (vl, int);
    va_end (vl);

    pmode = S_IREAD;

    if (mode & S_IWUSR || mode & S_IWGRP || mode & S_IWOTH) {
        pmode |= S_IWRITE;
    }

    return open (pathname, oflags, mode);
  }

  return open (pathname, oflags, pmode);
}

int
os_chmod (char const *filename, int mode)
{
  int pmode = S_IREAD;
  
  if (mode & S_IWUSR || mode & S_IWGRP || mode & S_IWOTH) {
    pmode |= S_IWRITE;
  }

  return chmod (filename, pmode);
}

#else

int
os_open (char const *pathname, int flags, ...)
{
  int mode;
  va_list vl;
  
  if (flags & O_CREAT) {
    va_start (vl, flags);
    mode = va_arg (vl, int);
    va_end (vl);
    return open (pathname, flags, mode);
  }

  return open (pathname, flags);
}

int
os_chmod (char const *filename, int mode)
{
  return chmod (filename, mode);
}

#endif