#include <windows.h>
#include <winsock2.h>

static void init_wsa (void) __attribute__((constructor));
static void close_wsa (void) __attribute__((destructor));

static void
init_wsa (void)
{
  WSADATA wsData;
  if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
    exit (EXIT_FAILURE);
  }
}

static void
close_wsa (void)
{
  WSACleanup ();
}