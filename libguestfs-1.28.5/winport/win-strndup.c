#ifdef _WIN32

#include <stdlib.h>
#include <string.h>

char *
strndup (const char *s, size_t n)
{
  char* result;
  size_t len;
    
  if (!s) {
    return NULL;
  }

  len = strlen (s);
  if (n < len) {
    len = n;
  }

  result = (char*) malloc(len + 1);
  if (!result) {
    return NULL;
  }

  result[len] = '\0';
  memcpy (result, s, len);

  return result;
}

#endif /* _WIN32 */