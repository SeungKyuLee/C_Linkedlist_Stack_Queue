#ifdef _WIN32
#include <WINSOCK2.H>
#include <windows.h>
#include <process.h>
#include <stdint.h>

#define popen _popen
#define pclose _pclose
typedef unsigned int uint32_t;
typedef signed int ssize_t;

typedef int32_t pid_t;
#endif

#define SUCCESS 0
#define FAIL -1

#define FREE(s) if((s) != NULL) { free(s); s = NULL;}