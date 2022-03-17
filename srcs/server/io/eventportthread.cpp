// The Romi Project
//
// @ File   :  eventportthread.cpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 14:20:39
//

#include "eventportthread.hpp"
#include "error/invalidoperationexception.hpp"

REventPortThread::REventPortThread(int num)
    : _num(num)
    , _threadHandle(RomiInvalidHandle)
    , _threadId()
    , _isAlive(false)
    , _isBegun(false)
{
}

REventPortThread::~REventPortThread()
{
    Shutdown();
}

void REventPortThread::MoveAssign(REventPortThread&& o)
{
    if (_isBegun || o._isBegun)
        throw RInvalidOperationException("Already begun port thread instance");
    if (_isAlive.load() || o._isAlive.load())
        throw RInvalidOperationException("Cannot move alive port thread instance");
    _num = o._num;
    _threadHandle = o._threadHandle;
    _threadId = o._threadId;
    o._num = 0;
    o._threadHandle = RomiInvalidHandle;
    o._threadId = 0;
}

REventPortThread::REventPortThread(REventPortThread&& o)
{
    MoveAssign(std::move(o));
}

REventPortThread& REventPortThread::operator= (REventPortThread&& o)
{
    if (this == &o)
        return *this;
    MoveAssign(std::move(o));
    return *this;
}

void REventPortThread::Shutdown()
{
    bool    stop = true;

    LOGD << "REventPortThread-" << _num << " shutting down has been initiated.";

    if (!_isAlive.compare_exchange_strong(stop, false))
        return;


    LOGD << "REventPortThread-" << _num << " Died.";
}
