/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 * This software is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this software under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this software under a commercial
 * license, as set out in <http://cesanta.com/>.
 */

#ifndef NS_COMMON_HEADER_INCLUDED
#define NS_COMMON_HEADER_INCLUDED

#define NS_FOSSA_VERSION "2.0.0"

#ifdef __AVR__
/* -I<path_to_avrsupport> should be specified */
#include <avrsupport.h>
#endif

#if !defined(NS_DISABLE_FILESYSTEM) && defined(AVR_NOFS)
#define NS_DISABLE_FILESYSTEM
#endif

#undef UNICODE                  /* Use ANSI WinAPI functions */
#undef _UNICODE                 /* Use multibyte encoding on Windows */
#define _MBCS                   /* Use multibyte encoding on Windows */
#define _INTEGRAL_MAX_BITS 64   /* Enable _stati64() on Windows */
#define _CRT_SECURE_NO_WARNINGS /* Disable deprecation warning in VS2005+ */
#undef WIN32_LEAN_AND_MEAN      /* Let windows.h always include winsock2.h */
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600    /* For flockfile() on Linux */
#define __STDC_FORMAT_MACROS /* <inttypes.h> wants this for C++ */
#define __STDC_LIMIT_MACROS  /* C++ wants that for INT64_MAX */
#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE /* Enable fseeko() and ftello() functions */
#endif
#define _FILE_OFFSET_BITS 64 /* Enable 64-bit file offsets */

/*
 * MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
 * MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
 * MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
 * MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
 * MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
 * MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio 2003)
 * MSVC++ 7.0  _MSC_VER == 1300
 * MSVC++ 6.0  _MSC_VER == 1200
 * MSVC++ 5.0  _MSC_VER == 1100
 */
#ifdef _MSC_VER
#pragma warning(disable : 4127) /* FD_SET() emits warning, disable it */
#pragma warning(disable : 4204) /* missing c99 support */
#endif

#ifndef AVR_LIBC
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#endif

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef va_copy
#ifdef __va_copy
#define va_copy __va_copy
#else
#define va_copy(x, y) (x) = (y)
#endif
#endif

#ifdef _WIN32
#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib") /* Linking with winsock library */
#endif
#include <windows.h>
#include <process.h>
#ifndef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif
#ifndef __func__
#define STRX(x) #x
#define STR(x) STRX(x)
#define __func__ __FILE__ ":" STR(__LINE__)
#endif
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define sleep(x) Sleep((x) *1000)
#define to64(x) _atoi64(x)
#define popen(x, y) _popen((x), (y))
#define pclose(x) _pclose(x)
#if defined(_MSC_VER) && _MSC_VER >= 1400
#define fseeko(x, y, z) _fseeki64((x), (y), (z))
#else
#define fseeko(x, y, z) fseek((x), (y), (z))
#endif
typedef int socklen_t;
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned __int64 uint64_t;
typedef __int64 int64_t;
typedef SOCKET sock_t;
typedef uint32_t in_addr_t;
#define INT64_FMT "I64d"
#ifdef __MINGW32__
typedef struct stat ns_stat_t;
#else
typedef struct _stati64 ns_stat_t;
#endif
#ifndef S_ISDIR
#define S_ISDIR(x) ((x) &_S_IFDIR)
#endif
#define DIRSEP '\\'
#else /* not _WIN32 */
#ifndef AVR_LIBC
#include <dirent.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h> /* For inet_pton() when NS_ENABLE_IPV6 is defined */
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#endif
#include <errno.h>
#include <inttypes.h>
#include <stdarg.h>
#ifndef AVR_LIBC
#define closesocket(x) close(x)
#define __cdecl
#define INVALID_SOCKET (-1)
#define INT64_FMT PRId64
#define to64(x) strtoll(x, NULL, 10)
typedef int sock_t;
typedef struct stat ns_stat_t;
#define DIRSEP '/'
#endif
#ifdef __APPLE__
int64_t strtoll(const char* str, char** endptr, int base);
#endif
#endif /* _WIN32 */

#ifdef NS_ENABLE_DEBUG
#define DBG(x)                  \
  do {                          \
    printf("%-20s ", __func__); \
    printf x;                   \
    putchar('\n');              \
    fflush(stdout);             \
  } while (0)
#else
#define DBG(x)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#endif

#endif /* NS_COMMON_HEADER_INCLUDED */
