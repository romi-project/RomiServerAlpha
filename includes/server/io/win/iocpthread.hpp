// The Romi Project
//
// @ File   :  iocpthread.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 14:20:39
//

/*
**  Romi IOCP Thread
*/

#pragma once

#include "std.hpp"

class RIOCPThread
{
private:
    const RomiDword     _num;
    RomiThreadHandle    _threadHandle;
    RomiDword           _threadId;
    std::atomic<bool>   _isAlive;
    bool                _isBegun;

public:
    RIOCPThread(RomiDword num);
    virtual ~RIOCPThread();

    RIOCPThread(const RIOCPThread&) = delete;
    RIOCPThread& operator= (const RIOCPThread&) = delete;

    RIOCPThread(RIOCPThread&& o);
    RIOCPThread& operator= (RIOCPThread&& o);

    void Shutdown();

protected:

private:
    void MoveAssign(RIOCPThread&& o);

};
