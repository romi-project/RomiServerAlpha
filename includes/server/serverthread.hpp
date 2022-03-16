// The Romi Project
//
// @ File   :  serverthread.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-16 16:59:28
//

#pragma once

#include "std.hpp"

class RServer;

class RServerThread
{
private:
    int32_t             _num;
    RomiThreadHandle    _thread;
    RomiDword           _threadId;
    volatile size_t     _isAlive;
    bool                _isBegun;
    RServer*            _server;

public:
    RServerThread(RServer* server, int32_t num);
    virtual ~RServerThread();
    RServerThread(const RServerThread&) = delete;
    RServerThread(RServerThread&& o);
    RServerThread& operator= (const RServerThread&) = delete;
    RServerThread& operator= (RServerThread&& o);

    void Begin();
    void End();

protected:

private:
    static THREAD_ROUTINE WorkerThread(RomiVoidPtr threadPtr);

};
