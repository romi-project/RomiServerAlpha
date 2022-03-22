// The Romi Project
//
// @ File   :  queuethread.cpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-18 15:23:10
//

#include "std.hpp"
#include "server/io/bsd/queuethread.hpp"
#include "server/io/bsd/queuecontext.hpp"
#include "server/io/isocketcallback.hpp"
#include "server/io/isocketacceptorcallback.hpp"
#include "utils/netutils.hpp"
#include "utils/errorutils.hpp"
#include "error/socketexception.hpp"
#include "error/runtimeexception.hpp"

RQueueThread::RQueueThread(int num)
    : _num(num)
    , _thread()
    , _threadId()
    , _eventfd()
    , _isAlive(true)
    , _acceptors()
{
    LOGD << "Created RQueueThread-" << num << ".";
}

RQueueThread::~RQueueThread()
{
    _acceptors.clear();
    LOGD << "Deleted RQueueThread-" << _num << ".";
}

void    RQueueThread::Begin()
{
    bool    expected = false;

    if (!_isAlive.compare_exchange_strong(expected, true))
        throw RRuntimeException("RQueueThread has already begun.");
    _eventfd = kqueue();
    if (_eventfd < 0)
        throw RSocketException(GetRomiLastSocketError());
    _thread = std::thread(&RQueueThread::Run, this);
    _threadId = _thread.get_id();
    LOGV << "RQueueThread-" << _num << " has begun.";
}

void    RQueueThread::End()
{
    bool    expected = true;

    if (_isAlive.compare_exchange_strong(expected, false))
        LOGV << "RQueueThread-" << _num << " has stopped.";
    else
        LOGV << "RQueueThread-" << _num << " was tried to stop, but already died.";
}

// kqueue에 accept되어온 소켓 등록
void    RQueueThread::RegisterSocket(RomiRawSocket socketfd, const RQueueContext& context)
{
    SetSocketOpt(socketfd);
    SetEvent(socketfd, context, RomiIOEvent_Read, RomiIOInterested_Add | RomiIOInterested_Enable);
    SetEvent(socketfd, context, RomiIOEvent_Write | RomiIOEvent_Custom, RomiIOInterested_Add);
}

void    RQueueThread::UnregisterSocket(RomiRawSocket socketfd, const RQueueContext& context)
{
    SetEvent(socketfd, context, RomiIOEvent_Read | RomiIOEvent_Write | RomiIOEvent_Custom, RomiIOInterested_Remove);
}

// kqueue에 액셉터 등록
void    RQueueThread::RegisterAcceptor(RomiRawSocket listenfd, const RQueueContext& context)
{
    if (_isAlive.load())
        throw RRuntimeException("Cannot register an acceptor since the RQueueThread is already begun.");
    _acceptors.emplace(listenfd);
    SetEvent(listenfd, context, RomiIOEvent_Read, RomiIOInterested_Add | RomiIOInterested_Enable);
    LOGD << "Registered AcceptorSocket [" << listenfd << "] to RQueueThread-" << _num << ".";
}

void    RQueueThread::SetEvent(RomiRawSocket socketfd, const RQueueContext& context, int events, int flags)
{
    kevent64_s  ev[3];
    int         eventNum = 0;
    int         eventFlags = 0;

    if (flags & RomiIOInterested_Add)
        eventFlags |= EV_ADD;
    if (flags & RomiIOInterested_Remove)
        eventFlags |= EV_DELETE;
    if (flags & RomiIOInterested_Enable)
        eventFlags |= EV_ENABLE;
    if (flags & RomiIOInterested_Disable)
        eventFlags |= EV_DISABLE;

    if (events & RomiIOEvent_Read)
         EV_SET64(&ev[eventNum++], socketfd, EVFILT_READ, eventFlags, 0, 0, reinterpret_cast<uint64_t>(&context), 0, 0);
    if (events & RomiIOEvent_Write)
         EV_SET64(&ev[eventNum++], socketfd, EVFILT_WRITE, eventFlags, 0, 0, reinterpret_cast<uint64_t>(&context), 0, 0);
    if (events & RomiIOEvent_Custom)
         EV_SET64(&ev[eventNum++], socketfd, EVFILT_USER, eventFlags, 0, 0, reinterpret_cast<uint64_t>(&context), 0, 0);

    int evregist = kevent64(_eventfd, ev, eventNum, nullptr, 0, 0, nullptr);
    if (evregist < 0)
        throw RSocketException(GetRomiLastSocketError());
}

void    RQueueThread::SetSocketOpt(RomiRawSocket socketfd)
{
    int flags = fcntl(socketfd, F_GETFL, 0);
    if (flags < 0)
        throw RSocketException(GetRomiLastSocketError());
    int setfcntlres = fcntl(socketfd, F_SETFL, flags, O_NONBLOCK);
    if (setfcntlres < 0)
        throw RSocketException(GetRomiLastSocketError());
}

static bool is_uncontinuable_accept_error(int err)
{
    switch (err)
    {
        case EBADF:
        case EFAULT:
        case EINVAL:
        case ENOTSOCK:
        case EOPNOTSUPP:
            return true;
        default:
            return false;
    }
}

void    RQueueThread::DoAccept(int socketfd, const RQueueContext& context)
{
    sockaddr_in addr;
    socklen_t addr_size = sizeof(addr);

    int clientfd = accept(socketfd, reinterpret_cast<sockaddr*>(&addr), &addr_size);
    if (clientfd < 0)
        if (is_uncontinuable_accept_error(GetRomiLastSocketError()))
            throw RSocketException(GetRomiLastSocketError());

    const auto addressString = NetUtils::ConvertAddressToString(addr.sin_family, &addr.sin_addr);
    LOGD << "Incoming a connection (" << clientfd << ") from " << addressString << ".";

    SetSocketOpt(clientfd);
    context.Acceptor->OnAccept(clientfd, addressString);
}

THREAD_ROTUINE RQueueThread::Run(RomiVoidPtr selfPtr)
{
    RQueueThread* $this = reinterpret_cast<RQueueThread*>(selfPtr);
    const int MaxEvents = 20;
    kevent64_s events[MaxEvents];

    while ($this->_isAlive.load())
    {
        int numbers = kevent64($this->_eventfd, NULL, 0, events, MaxEvents, 0, nullptr);
        for (int i = 0; i < numbers; i++)
        {
            kevent64_s& event = events[i];
            const RQueueContext& context = *reinterpret_cast<const RQueueContext*>(event.udata);
            RomiRawSocket socketfd = context.Socket;
            int filter = event.filter;

            // accept/read
            if (filter == EVFILT_READ)
            {
                const auto  acceptor = $this->_acceptors.find(socketfd);
                const bool  isAccept = acceptor != $this->_acceptors.end();

                if (isAccept)
                    $this->DoAccept(socketfd, context);

                else
                    context.Callback->OnEvent(RomiIOEvent_Read);
            }

            // write
            else if (filter == EVFILT_WRITE)
                context.Callback->OnEvent(RomiIOEvent_Write);

            // close
            else if (filter == EVFILT_USER)
                context.Callback->OnEvent(RomiIOEvent_Custom);
        }
    }
    return THREAD_ROUTINE_RETURN;
}

void    RQueueThread::TriggerEvent(const RQueueContext& context, int type, int flags)
{
    SetEvent(context.Socket, context, type, flags);
}
