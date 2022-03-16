// The Romi Project
//
// @ Author :  Tensiya(T2SU)
// @ Since  :  2022-03-14 14:05:49
//
// @ Last Modified      :  Tensiya(T2SU)
// @ Last Modified time :  2022-03-14 14:05:49
//

#pragma once

#include "std.hpp"

enum class IOOperation { IO_Recv, IO_Send, IO_Accept, IO_Close };

class RSocketBase
{
protected:
    RomiRawSocket   _socket;

public:
    RSocketBase() = delete;
    virtual ~RSocketBase() = default;
    RSocketBase(const RSocketBase&) = delete;
    RSocketBase(RSocketBase&&) = delete;
    RSocketBase& operator= (const RSocketBase&) = delete;
    RSocketBase& operator= (RSocketBase&&) = delete;

    RSocketBase(RomiRawSocket socket);
};

#if _ROMI_WINDOWS

typedef struct ROverlapped
{
    WSAOVERLAPPED   Overlapped;
    RSocketBase     Socket;
    WSABUF          Buf;
    IOOperation     Op;
} RSocketOp;

#else

struct RSocketOp
{

};

#endif