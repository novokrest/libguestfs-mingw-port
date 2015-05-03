#ifdef _WIN32

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "win-port.h"

void*
memmem (const void *l, size_t l_len, const void *s, size_t s_len)
{
  register char *cur, *last;
  const char *cl = (const char *) l;
  const char *cs = (const char *) s;

  if (l_len == 0 || s_len == 0) {
    return NULL;
  }

  if (s_len > l_len) {
    return NULL;
  }

  if (s_len == 1) {
    return memchr (l, (int) *cs, l_len);
  }

  last = (char *) cl + l_len - s_len;

  for (cur = (char *) cl; cur <= last; ++cur) {
    if (cur[0] == cs[0] && memcmp (cur, cs, s_len) == 0) {
      return cur;
    }
  }

  return NULL;
}

/*
 * There is mock implementation. Must be research and implemented.
 * https://cells-source.cs.columbia.edu/plugins/gitiles/platform/system/core/+/a570312a56938f6ebfcadc3ec9830ab8a2407654/libcutils/open_memstream.c
 * http://stackoverflow.com/questions/10305095/can-i-replace-a-call-to-open-memstream-with-a-malloc-and-an-implicit-cast?answertab=votes#tab-top
 * http://stackoverflow.com/questions/10339473/using-mapviewoffile-pointer-eventually-walks-out-of-memory-space
 *
 * Windows file streams:
 * https://msdn.microsoft.com/ru-ru/library/windows/desktop/aa364404(v=vs.85).aspx
 * https://msdn.microsoft.com/ru-ru/library/windows/desktop/aa366761(v=vs.85).aspx
*/

FILE * 
open_memstream (char **ptr, size_t *sizeloc)
{
  return NULL;
}

#endif /* _WIN32 */