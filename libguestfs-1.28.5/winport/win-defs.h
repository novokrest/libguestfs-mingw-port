#ifndef WIN_DEFS_H_
#define WIN_DEFS_H_

#ifdef _WIN32

#define WIFEXITED(x)   1
#define WEXITSTATUS(x) (x)
#define WIFSIGNALED(x) 0
#define WIFSTOPPED(x)  0
#define WTERMSIG(x)    0
#define WSTOPSIG(x)    0

#define mode_t int

#define zu Iu

#define O_NOCTTY 0
#define O_CLOEXEC 0

#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010
#define S_IRWXO 00007
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001

/* POSIX SUPPLEMENT */
//#define EADDRINUSE      100
//#define EADDRNOTAVAIL   101
//#define EAFNOSUPPORT    102
//#define EALREADY        103
//#define EBADMSG         104
//#define ECANCELED       105
//#define ECONNABORTED    106
//#define ECONNREFUSED    107
//#define ECONNRESET      108
//#define EDESTADDRREQ    109
//#define EHOSTUNREACH    110
//#define EIDRM           111
//#define EINPROGRESS     112
//#define EISCONN         113
//#define ELOOP           114
//#define EMSGSIZE        115
//#define ENETDOWN        116
//#define ENETRESET       117
//#define ENETUNREACH     118
//#define ENOBUFS         119
//#define ENODATA         120
//#define ENOLINK         121
//#define ENOMSG          122
//#define ENOPROTOOPT     123
//#define ENOSR           124
//#define ENOSTR          125
//#define ENOTCONN        126
//#define ENOTRECOVERABLE 127
//#define ENOTSOCK        128
#define ENOTSUP         129
//#define EOPNOTSUPP      130
//#define EOTHER          131
//#define EOVERFLOW       132
//#define EOWNERDEAD      133
//#define EPROTO          134
//#define EPROTONOSUPPORT 135
//#define EPROTOTYPE      136
//#define ETIME           137
//#define ETIMEDOUT       138
//#define ETXTBSY         139
//#define EWOULDBLOCK     140

#endif /* _WIN32 */

#endif /* WIN_DEFS_H_ */