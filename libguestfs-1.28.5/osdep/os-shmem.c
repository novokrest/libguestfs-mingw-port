#include "os-shmem.h"

#ifdef _WIN32

#include <windows.h>

struct windows_shared_memory
{
  struct os_shared_memory_ops *ops;

  const char *name;
  uint64_t size;
  
  HANDLE h;
  void *ptr;
};

static int
windows_shared_memory__open (struct os_shared_memory *shmemv)
{
  struct windows_shared_memory *shmem = (struct windows_shared_memory *) shmemv;

  HANDLE h;
  void *ptr;
  char sGlobalName[MAX_PATH];

  sprintf (sGlobalName, "Global\\%s", shm->name);

  h = OpenFileMapping (
    FILE_MAP_ALL_ACCESS,
    FALSE,
    sGlobalName);

  if (h == NULL) {
    return -1;
  }

  ptr = MapViewOfFile (
    h,
    FILE_MAP_ALL_ACCESS,
    0,
    0,
    shmem->size);

  if (ptr == NULL) {
    return -1;
  }

  shm->h = h;
  shm->ptr = ptr;

  return 0;
}

static int
windows_shared_memory__close (struct os_shared_memory *shmemv)
{
  struct windows_shared_memory *shmem = (struct windows_shared_memory *) shmemv;
  
  if (!UnmapViewOfFile (shm->ptr) || !CloseHandle (shm->h))
    return -1;

  shm->h = NULL;
  shm->ptr = NULL;

  return 0;
}

static const char *
windows_shared_memory__get_name (struct os_shared_memory *shmemv)
{
    struct windows_shared_memory *shmem = (struct windows_shared_memory *) shmemv;

    return shmem->name;
}

static uint64_t
windows_shared_memory__get_size (struct os_shared_memory *shmemv)
{
    struct windows_shared_memory *shmem = (struct windows_shared_memory *) shmemv;

    return shmem->size;
}

static void *
windows_shared_memory__get_ptr(struct os_shared_memory *shmemv)
{
  struct windows_shared_memory *shmem = (struct windows_shared_memory *) shmemv;

  return shmem->ptr;
}

static void
windows_shared_memory__print(struct os_shared_memory *shmemv, int n)
{
  struct windows_shared_memory *shmem = (struct windows_shared_memory *) shmemv;
  char *buf;

  buf = malloc (n + 1);
  if (!buf)
    return;

  memcpy (buf, (char *) shmem->ptr, n);
  buf[n] = '\0';

  printf ("shared memory info:\n"
          "  name: %s\n"
          "  size: %d\n"
          "  first %d bytes: %s\n",
          shmem->name, shmem->size, n, buf);

  free (buf);
}

static struct os_shared_memory_ops ops = {
  .open = windows_shared_memory__open,
  .close = windows_shared_memory__close,
  .get_name = windows_shared_memory__get_name,
  .get_size = windows_shared_memory__get_size,
  .get_ptr = windows_shared_memory__get_ptr,
  .print = windows_shared_memory__print,
};

struct os_shared_memory *
new_shared_memory (const char *name, uint64_t size)
{
  struct windows_shared_memory *shmem;

  if (size == 0) {
    return NULL;
  }

  shmem = malloc (sizeof (struct windows_shared_memory));
  if (!shmem) {
    return NULL;
  }

  shmem->ops = &ops;
  shmem->name = strdup (name);
  shmem->size = size;
  shmem->h = -1;
  shmem->ptr = NULL;

  return (struct os_shared_memory *) shmem;
}

void
free_shared_memory (struct os_shared_memory *shmemv)
{
  struct windows_shared_memory *shmem = (struct windows_shared_memory *) shmemv;

    free (shmem->name);
    free (shmem);
}

#else

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>

struct posix_shared_memory
{
  struct os_shared_memory_ops *ops;

  const char *name;
  uint64_t size;
  int fd;
  void *ptr;
};

static int
posix_shared_memory__open (struct os_shared_memory *shmemv)
{
  struct posix_shared_memory *shmem = (struct posix_shared_memory *) shmemv;
  uint64_t size;
  int fd;
  void *map = NULL;
  struct stat st;

  if ((fd = open (shmem->name, O_RDWR)) < 0) {
    return -1;
  }

  if (fstat (fd, &st) == -1) {
    close(fd);
    return -1;
  }

#if 0
  size = st.st_size;
  shmem->size = shmem->size < size ? shmem->size : size;
  if (ftruncate (shmem->size) == -1) {
    close(fd);
    return -1;
  }
#endif

  map = mmap (0, shmem->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 1 * getpagesize());
  if (map == MAP_FAILED) {
    close (fd);
    return -1;
  }

  shmem->fd = fd;
  shmem->ptr = map;

  return 0;
}

static int
posix_shared_memory__close (struct os_shared_memory *shmemv)
{
  struct posix_shared_memory *shmem = (struct posix_shared_memory *) shmemv;
  
  if ((munmap (shmem->ptr, shmem->size)) < 0) {
    return -1;
  }

  if (close (shmem->fd)) {
    return -1;
  }

  shmem->ptr = NULL;
  shmem->fd = 0;

  return 0;
}

static const char *
posix_shared_memory__get_name (struct os_shared_memory *shmemv)
{
    struct posix_shared_memory *shmem = (struct posix_shared_memory *) shmemv;

    return shmem->name;
}

static uint64_t
posix_shared_memory__get_size (struct os_shared_memory *shmemv)
{
    struct posix_shared_memory *shmem = (struct posix_shared_memory *) shmemv;

    return shmem->size;
}

static void *
posix_shared_memory__get_ptr(struct os_shared_memory *shmemv)
{
  struct posix_shared_memory *shmem = (struct posix_shared_memory *) shmemv;

  return shmem->ptr;
}

static void
posix_shared_memory__print(struct os_shared_memory *shmemv, int n)
{
  struct posix_shared_memory *shmem = (struct posix_shared_memory *) shmemv;
  char *buf;

  buf = malloc (n + 1);
  if (!buf)
    return;

  memcpy (buf, (char *) shmem->ptr, n);
  buf[n] = '\0';

  printf ("shared memory info:\n"
          "  name: %s\n"
          "  size: %" PRIu64 "\n"
          "  first %d bytes: %s\n",
          shmem->name, shmem->size, n, buf);

  free (buf);
}

static struct os_shared_memory_ops ops = {
  .open = posix_shared_memory__open,
  .close = posix_shared_memory__close,
  .get_name = posix_shared_memory__get_name,
  .get_size = posix_shared_memory__get_size,
  .get_ptr = posix_shared_memory__get_ptr,
  .print = posix_shared_memory__print,
};

struct os_shared_memory *
os_shared_memory__new (const char *name, uint64_t size)
{
  struct posix_shared_memory *shmem;

  if (size == 0) {
    return NULL;
  }

  shmem = malloc (sizeof(struct posix_shared_memory));
  if (!shmem) {
    return NULL;
  }

  shmem->ops = &ops;
  shmem->name = strdup (name);
  shmem->size = size;
  shmem->fd = -1;
  shmem->ptr = NULL;

  return (struct os_shared_memory *) shmem;
}

void
os_shared_memory__free (struct os_shared_memory *shmemv)
{
  struct posix_shared_memory *shmem = (struct posix_shared_memory *) shmemv;

    free ((char *) shmem->name);
    free (shmem);
}

#endif /* _WIN32 */

