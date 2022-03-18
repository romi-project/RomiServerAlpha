// The Romi Project
//
// @ File   :  iocpthread.cpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 14:20:39
//

#include "server/io/win/iocpthread.hpp"
#include "error/invalidoperationexception.hpp"

RIOCPThread::RIOCPThread(int num)
    : _num(num)
    , _threadHandle(RomiInvalidHandle)
    , _threadId()
    , _isAlive(false)
    , _isBegun(false)
{
}

RIOCPThread::~RIOCPThread()
{
    Shutdown();
}

void RIOCPThread::Shutdown()
{
    bool    stop = true;

    LOGD << "RIOCPThread-" << _num << " shutting down has been initiated.";

    if (!_isAlive.compare_exchange_strong(stop, false))
        return;


    LOGD << "RIOCPThread-" << _num << " Died.";
}
