// The Romi Project
//
// @ File   :  serverthread.cpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-16 16:59:28
//

#include "server/server.hpp"
#include "server/serverthread.hpp"
#include "error/runtimeexception.hpp"
#include "error/invalidoperationexception.hpp"
#include "concurrent/interlocked.hpp"

RServerThread::RServerThread(RServer* server, int32_t num)
    : _num(num)
    , _server(server)
    , _threadId()
    , _thread()
    , _isBegun(false)
    , _isAlive(true)
{
}

RServerThread::RServerThread(RServerThread&& o)
    : _num(o._num)
    , _server(o._server)
    , _threadId(o._threadId)
    , _thread(o._thread)
    , _isBegun(o._isBegun)
    , _isAlive(o._isAlive)
{
    if (_isBegun)
        throw RInvalidOperationException("Couldn't move RServerThread instance if it already begun.");
    o._num = -1;
    o._server = NULL;
    o._threadId = UINT_MAX;
    o._thread = static_cast<RomiThreadHandle>(NULL);
    o._isBegun = true;
    o._isAlive = false;
}

RServerThread::~RServerThread()
{
}

RServerThread& RServerThread::operator= (RServerThread&& o)
{
    if (this == &o)
        return *this;
    if (_isBegun)
        throw RInvalidOperationException("Couldn't move RServerThread instance if it already begun.");
    _num = o._num;
    _server = o._server;
    _threadId = o._threadId;
    _thread = o._thread;
    _isBegun = o._isBegun;
    _isAlive = o._isAlive;
    o._num = -1;
    o._server = NULL;
    o._threadId = UINT_MAX;
    o._thread = static_cast<RomiThreadHandle>(NULL);
    o._isBegun = true;
    o._isAlive = false;
    return *this;
}

void RServerThread::Begin()
{
    if (_isBegun || !_isAlive)
        return;
    RomiThreadHandle thread = CreateThread(NULL, 0, RServerThread::WorkerThread, this, 0, &_threadId);
    if (thread == NULL)
        throw RRuntimeException(GetLastError());

}

void RServerThread::End()
{
    if (_isBegun)
        RInterlocked::CompareExchange(&_isAlive, true, false);
}

THREAD_ROUTINE  RServerThread::WorkerThread(RomiVoidPtr threadPtr)
{
    RServerThread& $this = *reinterpret_cast<RServerThread*>(threadPtr);
    auto iocpHandle = $this._server->GetIocpHandle();

    while ($this._isAlive)
    {
        RomiDword bytes;

        /*if (GetQueuedCompletionStatus(iocpHandle, &bytes, &key, )

        bSuccess = GetQueuedCompletionStatus(hIOCP, &dwIoSize,
            (PDWORD_PTR)&lpPerSocketContext,
            (LPOVERLAPPED*)&lpOverlapped,
            INFINITE);
        if (!bSuccess)
            myprintf("GetQueuedCompletionStatus() failed: %d\n", GetLastError());*/
    }
    return THREAD_ROUTINE_RETURN;
}