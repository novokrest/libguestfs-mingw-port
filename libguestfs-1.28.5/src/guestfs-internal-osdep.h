#ifndef GUESTFS_INTERNAL_OSDEP_H_
#define GUESTFS_INTERNAL_OSDEP_H_

#include <os-error.h>
#include <os-fcntl.h>
#include <os-socket.h>
#include <os-socketpair.h>
#include <os-pipe.h>
#include <os-process.h>
#include <os-shmem.h>

#ifdef _WIN32

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "win-port.h"

#endif /* _WIN32 */

#endif /* GUESTFS_INTERNAL_OSDEP_H_ */
