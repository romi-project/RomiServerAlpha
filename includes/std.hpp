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
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <type_traits>
#include <algorithm>
#include <mutex>
#include <cstdio>
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>

#define TCP_MTU (1500 - 20 - 20)

#if _ROMI_WINDOWS

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

inline void Close(RomiRawSocket socket) { closesocket(socket); }
inline int GetRomiLastError() { return GetLastError(); }
inline int GetRomiLastSocketError() { return WSAGetLastError(); }

#else

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cerrno>

inline void Close(RomiRawSocket socket) { close(socket); }
inline int GetRomiLastError() { return errno; }
inline int GetRomiLastSocketError() { return errno; }

#endif
