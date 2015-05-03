#ifdef _WIN32

#include <windows.h>

#include "win-port.h"

char * 
realpath (const char *path, char *resolved_path)
{
  char buf[1];
  char* ret;
  size_t len, path_len;

  if (strlen (path) > MAX_PATH) {
    return NULL;
  }

  len = GetFullPathName (path, sizeof (buf), buf, NULL);
  if (len == 0) {
    return NULL;
  }

  path_len = len > sizeof (buf) ? len : len + 1;
  ret = resolved_path == NULL ? malloc (path_len) : resolved_path;

  if (!GetFullPathName (path, path_len, ret, NULL)) {
    return NULL;
  }

  return ret;
}

#endif /* _WIN32 */