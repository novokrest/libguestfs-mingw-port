#include "os-process.h"

#ifdef _WIN32

void
os_process_info__init (PROCESS_INFORMATION *proc)
{
  ZeroMemory (proc, sizeof (PROCESS_INFORMATION));
}

void
os_process_info__close (PROCESS_INFORMATION *proc)
{
  if (proc->hProcess)
    CloseHandle (proc->hProcess);
  if (proc->hThread)
    CloseHandle (proc->hThread);
}

void
os_process_info__set_default (PROCESS_INFORMATION *proc)
{
  if (proc->hProcess)
    CloseHandle (proc->hProcess);
  if (proc->hThread)
    CloseHandle (proc->hThread);

  ZeroMemory (proc, sizeof (PROCESS_INFORMATION));
}

int
os_process_info__is_valid (PROCESS_INFORMATION *proc)
{
  return proc->hProcess != NULL;
}

int
os_process_info__get_id (PROCESS_INFORMATION *proc)
{
  return GetProcessId (proc->hProcess);
}

int
os_process_info__kill (PROCESS_INFORMATION *proc, int signal)
{
  return TerminateProcess (proc->hProcess, signal) ? 0 : -1;
}

int
os_process_info__wait (PROCESS_INFORMATION *proc, int *status, int options)
{
  BOOL bSuccess;
  DWORD ret;

  ret = WaitForSingleObject(proc->hProcess, INFINITE);

  if (status == NULL) {
    return ret == WAIT_FAILED ? -1 : 0;
  }

  bSuccess = GetExitCodeProcess (proc->hProcess, (PDWORD) status);
  if (!bSuccess) {
    return -1;
  }

  return proc->dwProcessId;
}

#else

#include <sys/wait.h>
#include <sys/signal.h>

void
os_process_info__init (pid_t *pid)
{
  *pid = 0;
}

void
os_process_info__close (pid_t *pid)
{
}

void
os_process_info__set_default (pid_t *pid)
{
  *pid = 0;
}

int
os_process_info__is_valid (pid_t *pid)
{
  return *pid > 0;
}

int
os_process_info__get_id (pid_t *pid)
{
  return *pid;
}

int
os_process_info__kill (pid_t *pid, int signal)
{
  return kill (*pid, signal);
}

int
os_process_info__wait (pid_t *pid, int *status, int options)
{
  return waitpid (*pid, status, options);
}

#endif /* _WIN32 */