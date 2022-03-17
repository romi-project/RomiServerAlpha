// The Romi Project
//
// @ File   :  eventportthread.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 14:20:39
//

/*
**  Romi Event / Port Thread
**
**    이벤트큐(kqueue 등) 및 포트(iocp 등)에서 발생되는 이벤트를
**    작업자 스레드에서 처리합니다.
**
**    kevent64, GetQueuedCompletionStatus 등 에서 얻어진 이벤트들에 대해
**    그에 알맞은 콜백을 실행하여 처리합니다.
*/

#pragma once

#include "std.hpp"

class REventPortThread
{
private:
    RomiDword           _num;
    RomiThreadHandle    _threadHandle;
    RomiDword           _threadId;
    std::atomic<bool>   _isAlive;
    bool                _isBegun;

public:
    REventPortThread(RomiDword num);
    virtual ~REventPortThread();

    REventPortThread(const REventPortThread&) = delete;
    REventPortThread& operator= (const REventPortThread&) = delete;

    REventPortThread(REventPortThread&& o);
    REventPortThread& operator= (REventPortThread&& o);

    void Shutdown();

protected:

private:
    void MoveAssign(REventPortThread&& o);

};
