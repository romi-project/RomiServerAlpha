// The Romi Project
//
// @ File   :  socketclient.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 14:20:44
//

/*
**  Romi Socket Client Class
**
**    클라이언트 소켓의 정보를 저장할 최상위 부모 클래스의 정의입니다.
**
**    소켓과 관련된 기본적인 이벤트에 대한 프로토타입 및,
**    SendClose, Close 함수 등이 기초적으로 구현됩니다.
*/

#pragma once

#include "std.hpp"
#include "server/io/isocketcallback.hpp"

class RSocketClient : public RISocketCallback
{
private:
    RomiRawSocket _socket;

public:
    RSocketClient();
    virtual ~RSocketClient();

    RSocketClient(const RSocketClient&) = delete;
    RSocketClient(RSocketClient&&) = delete;
    RSocketClient& operator= (const RSocketClient&) = delete;
    RSocketClient& operator= (RSocketClient&&) = delete;

    virtual void OnConnect() = 0;
    virtual void OnSend();
    virtual void OnReceive();
    virtual void OnClose();

    virtual void ProcessPacket() = 0;

    void SendPacket();
    void Close();

protected:

private:

};
