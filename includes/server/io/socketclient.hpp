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

#include <atomic>
#include <queue>
#include <mutex>
#include "defines.hpp"
#include "bsd/queuecontext.hpp"
#include "isocketcallback.hpp"
#include "utils/inpacket.hpp"
#include "utils/outpacket.hpp"

class RISocketPort;

class RSocketClient : public RISocketCallback
{
private:
    enum class ComposeMode { Header, Content };
    const size_t            HEADER_SIZE = 6;

    const RomiRawSocket     _socket;
    const uint32_t          _socketId;
    const std::string       _remoteAddress;
    RISocketPort* const     _socketPort;

    InPacket                _inPacket;
    ComposeMode             _composeMode;
    RQueueContext           _context;
    std::atomic<bool>       _triggeredClose;
    std::atomic<bool>       _triggeredSend;

    std::mutex              _sendMutex;
    std::vector<char>       _writeBuffer;
    std::queue<OutPacket>   _writePackets;

public:
    RSocketClient(RomiRawSocket socket, uint32_t socketId, const std::string& remoteAddress, RISocketPort* socketPort);
    virtual ~RSocketClient();

    RSocketClient(RSocketClient&& o);
    RSocketClient(const RSocketClient&) = delete;
    RSocketClient& operator= (const RSocketClient&) = delete;
    RSocketClient& operator= (RSocketClient&&) = delete;

    uint32_t    GetSocketId() const;
    const RQueueContext&    GetContext() const;

    void    SendPacket(const OutPacket& outPacket);

    virtual void OnConnect() = 0;
    virtual void OnEvent(int event);
    virtual void ProcessPacket(const InPacket& inPacket) = 0;

protected:
    virtual void OnSend();
    virtual void OnReceive();
    virtual void OnClose();

private:
    static uint16_t    CalculateHash(const char* buffer, size_t len);

    void    ComposePacket();
    void    TriggerSend(bool newState);
    void    TriggerClose();
    void    Terminate();

};
