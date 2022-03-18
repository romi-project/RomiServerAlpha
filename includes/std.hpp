// The Romi Project
//
// @ Author :  Tensiya(T2SU)
// @ Since  :  2022-03-14 14:05:32
//
// @ Last Modified      :  Tensiya(T2SU)
// @ Last Modified time :  2022-03-14 14:05:32
//

#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <type_traits>
#include <algorithm>
#include <mutex>
#include <cstdio>
#include <cstdint>
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#define TCP_MTU (1500 - 20 - 20)

#if defined(_WIN32) || defined(_WIN64)

#define _ROMI_WINDOWS 1

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

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

#define _ROMI_WINDOWS 0

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

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
