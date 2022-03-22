#pragma once

#include <iosfwd>
#include <cstdint>
#include <cstddef>

enum : int
{
    RomiIOEvent_Read = 1 << 0,
    RomiIOEvent_Write = 1 << 1,
    RomiIOEvent_Custom = 1 << 2,
    RomiIOEvent_All = -1
};
enum : int
{
    RomiIOInterested_Add = 1 << 0,
    RomiIOInterested_Remove = 1 << 1,
    RomiIOInterested_Enable = 1 << 2,
    RomiIOInterested_Disable = 1 << 3
};

#ifndef _ROMI_VERSION
#define _ROMI_VERSION 101
#endif

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

inline void Close(RomiRawSocket socket) { closesocket(socket); }
inline int GetRomiLastError() { return GetLastError(); }
inline int GetRomiLastSocketError() { return WSAGetLastError(); }

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

inline void Close(RomiRawSocket socket) { close(socket); }
inline int GetRomiLastError() { return errno; }
inline int GetRomiLastSocketError() { return errno; }

#endif
