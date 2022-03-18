// The Romi Project
//
// @ File   :  queuethread.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-18 15:23:10
//

#pragma once

#include "std.hpp"

class RISocketAcceptorCallback;
struct RQueueContext;

class RQueueThread
{
private:
    enum { kevent_Read = 1 << 0, kevent_Write = 1 << 1 };

    typedef std::unordered_set<RomiRawSocket>   AcceptorSet;

    const int           _num;
    RomiThreadHandle    _thread;
    RomiDword           _threadId;
    RomiRawSocket       _eventfd;
    std::atomic<bool>   _isAlive;

    AcceptorSet         _acceptors;

public:
    RQueueThread() = delete;
    RQueueThread& operator= (const RQueueThread&) = delete;
    RQueueThread& operator= (RQueueThread&&) = delete;
    RQueueThread(const RQueueThread&) = delete;
    RQueueThread(RQueueThread&&) = delete;

    RQueueThread(int num);
    virtual ~RQueueThread();

    void    RegisterSocket(RomiRawSocket socketfd);
    void    RegisterAcceptor(RomiRawSocket listenfd, RQueueContext& context);

    static THREAD_ROTUINE Run(RomiVoidPtr selfPtr);

protected:

private:
    void    SetEvent(RomiRawSocket socketfd, RQueueContext& context, int events, bool mod);
    void    SetSocketOpt(RomiRawSocket socketfd);
    void    DoAccept(int socketfd, RQueueContext& context);
    void    DoRead(int socketfd, RQueueContext& context);

};
