#include "os-socketpair.h"

void
os_socketpair__close (struct os_socketpair *sp)
{
  if (os_socketpair_end__is_valid (sp->left))
    os_socketpair_end__close (sp->left);
  if (os_socketpair_end__is_valid (sp->right))
    os_socketpair_end__close (sp->right);
}

#ifdef _WIN32

int
os_socketpair_end__is_valid (HANDLE sp_end)
{
  return sp_end != NULL;
}

int
os_socketpair_end__close (HANDLE sp_end)
{
  return CloseHandle (sp_end) ? 0 : -1;
}

#else

#include <unistd.h>

int
os_socketpair_end__is_valid (int sp_end)
{
  return sp_end >= 0;
}

int
os_socketpair_end__close (int sp_end)
{
  return close (sp_end);
}

#endif /* _WIN32 */
