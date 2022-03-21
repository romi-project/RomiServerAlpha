// The Romi Project
//
// @ File   :  socketacceptor.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 14:20:34
//

/*
**  Romi Socket Acceptor Class
**
**    먼저, Listen 및 추후 새로운 클라이언트들을 Accept 할 소켓을 생성합니다.
**    이후, 생성자의 인자로 주어진 host 및 port에 포트를 바인딩 합니다.
**    비동기 이벤트 관리자에 이렇게 생성된 소켓을 등록할 수 있습니다.
**
**    소켓이 Accept 될 때, 템플릿 타입 인자 T 형식이 생성되며,
**    RSocketClient 클래스를 상속한 타입 인자 T가 주어지는 것을 전제로 합니다.
**
**    소켓의 Accept가 성공적으로 이루어졌다면,
**    RSocketClient의 가상 함수인 OnConnect 함수를 실행합니다.
*/

#pragma once

#include <type_traits>
#include <atomic>
#include <memory>
#include "defines.hpp"
#include "isocketacceptorcallback.hpp"
#include "isocketport.hpp"
#include "bsd/queuecontext.hpp"
#include "error/socketexception.hpp"
#include "error/invalidoperationexception.hpp"

class RSocketClient;

template<typename T, typename std::enable_if_t<std::is_convertible<T, RSocketClient>::value, RSocketClient*>* = nullptr>
class RSocketAcceptor : public RISocketAcceptorCallback
{
private:
    std::string         _host;
    std::string         _port;
    RomiRawSocket       _listenSocket;
    std::atomic<bool>   _isAlive;

public:
    RSocketAcceptor() = delete;
    RSocketAcceptor(const RSocketAcceptor&) = delete;
    RSocketAcceptor& operator= (const RSocketAcceptor&) = delete;

    RSocketAcceptor(
        const std::string& host,
        const std::string& port)
        : _listenSocket(RomiInvalidSocket)
        , _host(host)
        , _port(port)
        , _isAlive(false)
    {
    }

    virtual ~RSocketAcceptor()
    {
        Shutdown();
    }

    RSocketAcceptor(RSocketAcceptor&&) = default;
    RSocketAcceptor& operator= (RSocketAcceptor&&) = default;

    virtual void    OnAccept(RomiRawSocket socket, const std::string& remoteAddress, RISocketPort& socketPort)
    {
        SetSocketOpt(socket);
        std::shared_ptr<T> newSocket(T(socket, remoteAddress));
        socketPort.RegisterSocket(socket, context);
    }

    void    Shutdown()
    {
        if (_isAlive.compare_exchange_strong(false, true))
            return;
        LOGD << "SocketAcceptor[" << _host << ":" << _port << "] shutting down has been initiated.";
        // Unregister listen socket
        // close listen socket
        LOGD << "SocketAcceptor[" << _host << ":" << _port << "] has been closed.";
    }

protected:

private:
};
