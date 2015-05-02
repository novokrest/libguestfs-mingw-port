#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>

#include "guestfs.h"
#include "guestfs-internal.h"
#include "guestfs-internal-actions.h"

#define GUESTFS_SHARED_MEMORY_DEFAULT_SIZE 512
#ifdef _WIN32
#define GUESTFS_SHARED_MEMORY_DEFAULT_NAME "GuestfsShm"
#else
#define GUESTFS_SHARED_MEMORY_DEFAULT_NAME "guestfs_shm"
#endif /* _WIN32 */

static int is_power_of_2 (int v);

int
guestfs__set_shared_memory (guestfs_h *g, int enable,
                            const struct guestfs_set_shared_memory_argv *optargs)
{
  int size;
  const char *name;

  if (g->shmem) {
    os_shared_memory__free (g->shmem);
    g->shmem = NULL;
  }
  
  if (!enable) {
    return 0;
  }

  if (optargs->bitmask & GUESTFS_SET_SHARED_MEMORY_SIZE_BITMASK) {
    if (!is_power_of_2(optargs->size)) {
      error(g, _("size of shared memory must be power of 2"));
      return -1;
    }
  }

  size = optargs->bitmask & GUESTFS_SET_SHARED_MEMORY_SIZE_BITMASK ? optargs->size : GUESTFS_SHARED_MEMORY_DEFAULT_SIZE;
  name = optargs->bitmask & GUESTFS_SET_SHARED_MEMORY_NAME_BITMASK ? optargs->name : GUESTFS_SHARED_MEMORY_DEFAULT_NAME;

  g->shmem = os_shared_memory__new (name, size << 20);

  return 0;
}

int
guestfs__get_shared_memory (guestfs_h *g)
{
  return g->shmem != NULL;
}

static int
is_power_of_2 (int v)
{
  return v && (v & (v - 1)) == 0;
}

