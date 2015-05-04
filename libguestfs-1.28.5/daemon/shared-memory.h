#ifndef SHARED_MEMORY_H_
#define SHARED_MEMORY_H_

struct shared_memory;

struct shared_memory_ops
{
  int (*open) (struct shared_memory *shmem);
  int (*close) (struct shared_memory *shmem);
  const char *(*get_name) (struct shared_memory *shmem);
  uint64_t (*get_size) (struct shared_memory *shmem);
  void *(*get_ptr) (struct shared_memory *shmem); 
  void (*print) (struct shared_memory *shmem, size_t n);
};

struct shared_memory
{
  struct shared_memory_ops *ops;
};

extern struct shared_memory *daemon_shared_memory__new (const char *name, uint64_t size);
extern void daemon_shared_memory__free (struct shared_memory *shmem);

#endif /* SHARED_MEMORY_H_ */
