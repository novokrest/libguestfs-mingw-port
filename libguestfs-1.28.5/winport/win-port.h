#ifndef WIN_PORT_H_
#define WIN_PORT_H_

#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>

#include "win-defs.h"

/* win-vaprintf.c */
extern int asprintf (char** __restrict ret, const char * __restrict format, ...) __attribute__((format(ms_printf, 2, 3)));
extern int vasprintf (char** __restrict ret, const char * __restrict format, va_list ap) __attribute__((format(ms_printf, 2, 0)));

/* win-symlink.c */
extern int symlink (const char *path1, const char *path2);

/* win-strndup.c */
extern char *strndup (const char *s, size_t n);

/* win-realpath.c */
extern char *realpath (const char *path, char *resolved_path);

/* win-mem.c */
extern void* memmem (const void *l, size_t l_len, const void *s, size_t s_len);
extern FILE *open_memstream (char **ptr, size_t *sizeloc);

/* win-getuid.c */
extern char *getusername (void);
extern char *getusid (void);
extern int getuid (void);
extern int geteuid (void);

/* win-error.h */
extern int strerror_r (int errnum, char *buf, size_t buflen);

/* win-gettimeofday.c */
//extern int gettimeofday (struct timeval * tp, struct timezone * tzp)

#endif /* WIN_PORT_H_ */