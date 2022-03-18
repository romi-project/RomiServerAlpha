// The Romi Project
//
// @ File   :  queuethread.cpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-18 15:23:10
//

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
{
    LOGD << "Created RQueueThread-" << num << ".";
}

RQueueThread::~RQueueThread()
{
    _acceptors.clear();
    LOGD << "Deleted RQueueThread-" << _num << ".";
}

// kqueue에 accept되어온 소켓 등록
void    RQueueThread::RegisterSocket(RomiRawSocket socketfd)
{

}

// kqueue에 액셉터 등록
void    RQueueThread::RegisterAcceptor(RomiRawSocket listenfd, RQueueContext& context)
{
    if (_isAlive.load())
        throw RRuntimeException("Cannot register an acceptor since the RQueueThread is already begun.");
    _acceptors.emplace(listenfd);
    SetEvent(listenfd, context, kevent_Read, false);
    LOGD << "Registered AcceptorSocket [" << listenfd << "] to RQueueThread-" << _num << ".";
}

void    RQueueThread::SetEvent(RomiRawSocket socketfd, RQueueContext& context, int events, bool mod)
{
    kevent64_s  ev[2];
    int         eventNum = 0;

    if (events & kevent_Read)
        EV_SET64(&ev[eventNum++], socketfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, reinterpret_cast<uint64_t>(&context), 0, 0);
    else if (mod)
        EV_SET64(&ev[eventNum++], socketfd, EVFILT_READ, EV_DELETE, 0, 0, reinterpret_cast<uint64_t>(&context), 0, 0);
    if (events & kevent_Write)
        EV_SET64(&ev[eventNum++], socketfd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, reinterpret_cast<uint64_t>(&context), 0, 0);
    else if (mod)
        EV_SET64(&ev[eventNum++], socketfd, EVFILT_WRITE, EV_DELETE, 0, 0, reinterpret_cast<uint64_t>(&context), 0, 0);
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

void    RQueueThread::DoAccept(int socketfd, RQueueContext& context)
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

void    RQueueThread::DoRead(int socketfd, RQueueContext& context)
{
    char    buf[TCP_MTU];
    ssize_t n = 0;

    while (true)
    {
        n = read(socketfd, buf, sizeof(buf));
        if (n < 0 && errno == EAGAIN) // non blocking mode (no data can be read for now)
            return;
        if (n < 0)
        {
            LOGD << "Socket error (" << ErrorUtils::GetString(errno) << ") from " << NetUtils::GetPeerName(socketfd);
            break;
        }
        if (n == 0)
            break;
        context.Callback->OnReceive(buf, static_cast<size_t>(n));
    }
    LOGD << "Socket closed from " << NetUtils::GetPeerName(socketfd);
    shutdown(socketfd, SHUT_RDWR);
    close(socketfd);
    context.Callback->OnReceive(nullptr, 0);
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
            RQueueContext& context = *reinterpret_cast<RQueueContext*>(event.udata);
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
                    $this->DoRead(socketfd, context);
            }

            // write
            else if (filter == EVFILT_WRITE)
            {
                if (set_event(eventfd, socketfd, EVFILT_WRITE, EV_DELETE) != 0)
                    return 1;
            }
            else
                return error("invalid event (%d)\n", filter);
        }
        NEXT_LOOP:
        continue;
    }

    return THREAD_ROUTINE_RETURN;
}
