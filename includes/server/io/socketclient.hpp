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
#include "bsd/queuecontext.hpp"
#include "isocketcallback.hpp"
#include "utils/inpacket.hpp"
#include "utils/outpacket.hpp"

class RISocketPort;

class RSocketClient : public RISocketCallback
{
private:
    enum class AssembleMode { Header, Content };
    const size_t            HEADER_SIZE = 6;

    RomiRawSocket           _socket;
    const std::string       _remoteAddress;
    InPacket                _inPacket;
    std::queue<OutPacket>   _writePackets;
    bool                    _overlappedSend;
    AssembleMode            _assembleMode;
    RISocketPort*           _socketPort;
    RQueueContext           _context;
    std::atomic<bool>       _shouldClose;

public:
    RSocketClient(RomiRawSocket socket, const std::string& remoteAddress, RISocketPort* socketPort);
    virtual ~RSocketClient();

    RSocketClient(RSocketClient&& o);
    RSocketClient(const RSocketClient&) = delete;
    RSocketClient& operator= (const RSocketClient&) = delete;
    RSocketClient& operator= (RSocketClient&&) = delete;

    const RQueueContext&  GetContext() const;

    void    SendPacket(const OutPacket& outPacket);
    void    TriggerSocketQueue();

    virtual void OnConnect() = 0;
    virtual void OnSend();
    virtual void OnReceive(const char* buffer, size_t len);
    virtual void OnClose();

    virtual void ProcessPacket(const InPacket& inPacket) = 0;


protected:

private:
    void    AssemblePacket();
    static uint16_t    CalculateHash(const char* buffer, size_t len);

    void    DoClose();

};
