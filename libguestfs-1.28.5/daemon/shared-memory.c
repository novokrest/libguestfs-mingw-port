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

#include "shared-memory.h"

struct daemon_shared_memory
{
  struct shared_memory_ops *ops;

  const char *name;
  uint64_t size;
  int fd;
  void *ptr;
};

static int
daemon_shared_memory__open (struct shared_memory *shmemv)
{
  struct daemon_shared_memory *shmem = (struct daemon_shared_memory *) shmemv;
  
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
  uint64_t size;
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
daemon_shared_memory__close (struct shared_memory *shmemv)
{
  struct daemon_shared_memory *shmem = (struct daemon_shared_memory *) shmemv;
  
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
daemon_shared_memory__get_name (struct shared_memory *shmemv)
{
  struct daemon_shared_memory *shmem = (struct daemon_shared_memory *) shmemv;
  
  return shmem->name;
}

static uint64_t
daemon_shared_memory__get_size (struct shared_memory *shmemv)
{
  struct daemon_shared_memory *shmem = (struct daemon_shared_memory *) shmemv;
  
  return shmem->size;
}

static void *
daemon_shared_memory__get_ptr (struct shared_memory *shmemv)
{
  struct daemon_shared_memory *shmem = (struct daemon_shared_memory *) shmemv;
  
  return shmem->ptr;
}

static struct shared_memory_ops daemon_ops = {
  .open = daemon_shared_memory__open,
  .close = daemon_shared_memory__close,
  .get_name = daemon_shared_memory__get_name,
  .get_size = daemon_shared_memory__get_size,
  .get_ptr = daemon_shared_memory__get_ptr
};

struct shared_memory *
daemon_shared_memory__new (const char *name, uint64_t size)
{
  struct daemon_shared_memory *shmem;
  
  shmem = malloc (sizeof (struct daemon_shared_memory));
  if (!shmem) {
    return NULL;
  }
  
  shmem->ops = &daemon_ops;
  
  shmem->name = name;
  shmem->size = size;
  shmem->fd = -1;
  shmem->ptr = NULL;
  
  return (struct shared_memory *) shmem;
}

void
daemon_shared_memory__free (struct shared_memory *shmemv)
{
  struct daemon_shared_memory *shmem = (struct daemon_shared_memory *) shmemv;
  
  free ((void *) shmem->name);
  free (shmem);
}
