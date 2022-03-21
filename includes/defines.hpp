#pragma once

#include <iosfwd>
#include <cstdint>
#include <cstddef>

#if _ROMI_WINDOWS

typedef SOCKET  RomiRawSocket;
typedef HANDLE  RomiRawHandle;
typedef HANDLE  RomiThreadHandle;
typedef LPVOID  RomiVoidPtr;
typedef DWORD   RomiDword;

#define THREAD_ROUTINE DWORD WINAPI
#define THREAD_ROUTINE_RETURN 0

constexpr RomiRawSocket RomiInvalidSocket = INVALID_SOCKET;
constexpr RomiRawHandle RomiInvalidHandle = INVALID_HANDLE_VALUE;

#else

typedef int     RomiRawSocket;
typedef int     RomiRawHandle;
typedef pid_t   RomiThreadHandle;
typedef void*   RomiVoidPtr;
typedef int     RomiDword;

#define THREAD_ROTUINE void
#define THREAD_ROUTINE_RETURN

constexpr RomiRawSocket RomiInvalidSocket = -1;
constexpr RomiRawHandle RomiInvalidHandle = -1;

#endif
