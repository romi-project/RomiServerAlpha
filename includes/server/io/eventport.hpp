// The Romi Project
//
// @ File   :  eventport.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 14:18:02
//

/*
**  Romi Event / Port
**
**    이벤트큐(kqueue 등) 및 포트(iocp 등)를 등록하고, 관리합니다.
**
**    SocketAcceptor를 등록할 수 있습니다.
**    또한, REventPortThread를 생성하고, 이를 관리합니다.
*/

#pragma once

#include "std.hpp"
#include "socketacceptor.hpp"

class REventPort
{
private:
    // Registered SocketAcceptor
    // WorkerThreads

public:
    REventPort();
    virtual ~REventPort();

    REventPort(const REventPort&) = delete;
    REventPort(REventPort&&) = delete;
    REventPort& operator= (const REventPort&) = delete;
    REventPort& operator= (REventPort&&) = delete;

    template<typename T>
    void RegisterAcceptor(RSocketAcceptor<T>& acceptor)
    {

    }

    void Init();
    void Begin();
    void End();

};
