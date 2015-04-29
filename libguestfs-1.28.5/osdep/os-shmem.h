#ifndef OS_SHARED_MEMORY_H_
#define OS_SHARED_MEMORY_H_

#include <stdlib.h>
#include <stdint.h>

struct os_shared_memory;

struct os_shared_memory_ops
{
    int (*open) (struct os_shared_memory *shmem);
    int (*close) (struct os_shared_memory *shmem);
    const char *(*get_name) (struct os_shared_memory *shmem);
    uint64_t (*get_size) (struct os_shared_memory *shmem);
    void * (*get_ptr) (struct os_shared_memory *shmem);
    void (*print) (struct os_shared_memory *shmem, int n);
};

struct os_shared_memory
{
    struct os_shared_memory_ops *ops;
};

extern struct os_shared_memory *os_shared_memory__new (const char *name, uint64_t size);
extern void os_shared_memory__free (struct os_shared_memory *shmemv);

#endif /* OS_SHARED_MEMORY_H_ */

