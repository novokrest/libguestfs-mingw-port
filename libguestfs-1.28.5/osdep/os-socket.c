#include <stdio.h>
#include <string.h>
#include "os-socket.h"

#ifdef _WIN32

int
os_socket__is_valid (SOCKET sock)
{
  return sock != INVALID_SOCKET;
}

int os_socket__close (SOCKET sock)
{
  return closesocket (sock) ? 0 : -1;
}

void
get_socket_info_string (struct tcp_socket_info *sock_info, char *buf, size_t len)
{
  snprintf (buf, len, "host=%s,port=%s", sock_info->host, sock_info->port);
}

void
os_socket_info__free (struct tcp_socket_info *sock_info)
{
  free (sock_info->host);
  free (sock_info->port);
}

#else

#include <stdlib.h>
#include <unistd.h>

int
os_socket__is_valid (int sock)
{
  return sock >= 0;
}

int os_socket__close (int sock)
{
  return close (sock);
}

void
get_socket_info_string (struct unix_socket_info *sock_info, char *buf, size_t len)
{
  snprintf (buf, len, "path=%s", sock_info->path);
}

void
os_socket_info__free (struct unix_socket_info *sock_info)
{
  free (sock_info->path);
}


#endif /* _WIN32 */
