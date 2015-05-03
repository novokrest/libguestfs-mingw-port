#ifdef _WIN32

#include <windows.h>
#include <lmcons.h>
#include <sddl.h>

#include "win-port.h"

/* Returned buffer must be freed by caller */
char *
getusername (void)
{
  char *ret;
  int len;
  char buf[UNLEN + 1];

  len = sizeof (buf);

  if (!GetUserName (buf, (PDWORD) &len)) {
    return NULL;
  }

  ret = malloc (len);
  if (ret == NULL) {
    return NULL;
  }

  strcpy (ret, buf);
  ret[len - 1] = '\0';

  return ret;
}

/* Returned buffer must be freed by caller with LocalFree() */
char *
getusid (void)
{
  HANDLE hProc;
  HANDLE hToken;
  PTOKEN_USER pUserInfo;
  DWORD dwSize = 0, dwResult;
  char *ret;

  hProc = OpenProcess (PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId ());
  if (!hProc) {
    return NULL;
  }

  if (!OpenProcessToken (GetCurrentProcess (), TOKEN_QUERY, &hToken)) {
    return NULL;
  }

  if (!GetTokenInformation (hToken, TokenUser, NULL, dwSize, &dwSize)) {
    dwResult = GetLastError ();
    if (dwResult != ERROR_INSUFFICIENT_BUFFER) {
      return NULL;
    }
  }

  pUserInfo = (PTOKEN_USER) GlobalAlloc (GPTR, dwSize);
  if (!GetTokenInformation (hToken, TokenUser, pUserInfo, dwSize, &dwSize)) {
    if (pUserInfo) {
      GlobalFree (pUserInfo);
    }
    return NULL;
  }

  if (!ConvertSidToStringSid (pUserInfo->User.Sid, &ret)) {
    if (pUserInfo) {
      GlobalFree (pUserInfo);
    }
    return NULL;
  }

  if (pUserInfo) {
    GlobalFree (pUserInfo);
  }

  return ret;
}

int
getuid (void)
{
  PSID sid = NULL;
  TCHAR *domain = NULL;
  DWORD sid_size = 0;
  DWORD domain_size = 0;
  SID_NAME_USE use = SidTypeUnknown;

  TCHAR buffer[UNLEN + 1];
  DWORD buffer_len = UNLEN + 1;

  if (!GetUserName (buffer, &buffer_len)) {
    return 0;
  }

  /* Called once to set the size of the sid pointer and domain */
  LookupAccountName (NULL, buffer, NULL, &sid_size, NULL, &domain_size, &use);

  /* Allocate memory based on sid and domain size */
  sid = (PSID) LocalAlloc (LMEM_FIXED, sid_size);
  domain = (TCHAR*) malloc (domain_size * sizeof (TCHAR));

  /* Initialize sid and domain */
  if (!LookupAccountName (NULL, buffer, sid, &sid_size, domain, &domain_size, &use)) {
    LocalFree (sid);
    free (domain);
    return 0;
  }

  if (!IsValidSid (sid)) {
    LocalFree (sid);
    free (domain);
    return 0;
  }

  return (int) sid;
}

int
geteuid (void)
{
    return getuid ();
}

#endif /* _WIN32 */