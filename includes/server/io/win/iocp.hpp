// The Romi Project
//
// @ File   :  iocp.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 14:18:02
//

/*
**  Romi Event / Port
*/

#pragma once

#include "std.hpp"
#include "socketacceptor.hpp"

class RIOCP
{
private:
    // Registered SocketAcceptor
    // WorkerThreads

public:
    RIOCP();
    virtual ~RIOCP();

    RIOCP(const RIOCP&) = delete;
    RIOCP(RIOCP&&) = delete;
    RIOCP& operator= (const RIOCP&) = delete;
    RIOCP& operator= (RIOCP&&) = delete;

    template<typename T>
    void RegisterAcceptor(RSocketAcceptor<T>& acceptor)
    {

    }

    void Init();
    void Begin();
    void End();

};
