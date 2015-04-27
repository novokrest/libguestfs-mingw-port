#ifndef OS_ERROR_H_
#define OS_ERROR_H_

#ifdef _WIN32

#define os_perrorf perrorf_win

#else

#define os_perrorf perrorf

#endif /* _WIN32 */

#endif /* OS_ERROR_H_ */