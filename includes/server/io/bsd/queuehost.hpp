// The Romi Project
//
// @ File   :  queuehost.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-18 15:23:05
//

#pragma once

#include "../../../defines.hpp"
#include "../socketacceptor.hpp"
#include "queuethread.hpp"

class RQueueHost
{
private:
    const std::string host;
    const std::string name;

public:
    RQueueHost& operator= (const RQueueHost&) = delete;
    RQueueHost& operator= (RQueueHost&&) = delete;

    RQueueHost() = default;
    virtual ~RQueueHost() = default;
    RQueueHost(const RQueueHost&) = default;
    RQueueHost(RQueueHost&&) = default;

    RQueueThread&   GetNextQueueThread() const;

    template<typename T>
    void    RegisterAcceptor(RSocketAcceptor<T> acceptor)
    {
        (void)acceptor;
        // get thread -> register acceptor
    }

protected:

private:

};
