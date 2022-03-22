// The Romi Project
//
// @ File   :  queuethread.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-18 15:23:10
//

#pragma once

#include <thread>
#include <atomic>
#include <unordered_set>
#include "../../../defines.hpp"
#include "../isocketport.hpp"

struct RQueueContext;

class RQueueThread : public RISocketPort
{
private:

    typedef std::unordered_set<RomiRawSocket>   AcceptorSet;

    const int           _num;
    std::thread         _thread;
    std::thread::id     _threadId;
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

    void    Begin();
    void    End();
    void    RegisterSocket(RomiRawSocket socketfd, const RQueueContext& context);
    void    UnregisterSocket(RomiRawSocket socketfd, const RQueueContext& context);
    void    RegisterAcceptor(RomiRawSocket listenfd, const RQueueContext& context);
    void    TriggerEvent(const RQueueContext& context, int type, int flags);

    static THREAD_ROTUINE Run(RomiVoidPtr selfPtr);

protected:

private:
    void    SetEvent(RomiRawSocket socketfd, const RQueueContext& context, int events, int flags);
    void    SetSocketOpt(RomiRawSocket socketfd);
    void    DoAccept(int socketfd, const RQueueContext& context);

};
