#ifndef OS_FCNTL_H_
#define OS_FCNTL_H_

int os_open (char const *pathname, int flags, ...);
int os_chmod (char const *filename, int mode);

#endif /* OS_FCNTL_H_ */