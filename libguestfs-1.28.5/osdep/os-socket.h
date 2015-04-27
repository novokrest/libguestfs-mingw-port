#ifndef OS_SOCKET_H_
#define OS_SOCKET_H_

struct tcp_socket_info {
    char *host;
    char *port;
};

typedef struct tcp_socket_info tcp_scoket_info_t;

struct unix_socket_info {
    char *path;
};

typedef struct unix_socket_info unix_socket_info_t;

#ifdef _WIN32

#include <windows.h>
#include <winsock2.h>

#define os_socket_t SOCKET
#define OS_SOCKET_INVALID_VALUE INVALID_SOCKET

#define os_socket_info tcp_socket_info
#define os_socket_info_t tcp_socket_info_t

#else

#define os_socket_t int
#define OS_SOCKET_INVALID_VALUE -1

#define os_socket_info unix_socket_info
#define os_socket_info_t unix_socket_info_t

#endif /* _WIN32 */

int os_socket__is_valid (os_socket_t sock);
int os_socket__close (os_socket_t sock);

void get_socket_info_string (struct os_socket_info *sock_info, char *buf, size_t len);
void os_socket_info__free (struct os_socket_info *sock_info);

#endif /* OS_SOCKET_H_ */