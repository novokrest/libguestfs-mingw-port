#ifdef __MINGW32__

#include "win-port.h"

int
symlink (const char *path1, const char *path2)
{
  return -1;
}

#elif defined _WIN32

#include <windows.h>
#include <sys/stat.h>

#include "win-port.h"

int
symlink (const char *path1, const char *path2)
{
  struct stat st;
  int flags;
   
  if (!stat (path1, &st)) {
    return -1;
  }

  if (st.st_mode & S_IFREG) {
    flags = 0;
  }
  else if (st.st_mode & S_IFDIR) {
    flags = SYMBOLIC_LINK_FLAG_DIRECTORY;
  }
  else {
    return -1;
  }

  if (!CreateSymbolicLink (path2, path1, flags)) {
    return -1;
  }

  return 0;
}

#endif
