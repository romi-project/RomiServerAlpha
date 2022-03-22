// The Romi Project
//
// @ File   :  socketclient.cpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 14:20:44
//

#include "std.hpp"
#include "utils/errorutils.hpp"
#include "utils/netutils.hpp"
#include "server/io/isocketport.hpp"
#include "server/io/socketclient.hpp"

RSocketClient::RSocketClient(RomiRawSocket socket, uint32_t socketId, const std::string& remoteAddress, RISocketPort* socketPort)
    : _socket()
    , _socketId(socketId)
    , _remoteAddress(remoteAddress)
    , _socketPort(socketPort)
    , _inPacket()
    , _composeMode(ComposeMode::Header)
    , _context({socket, nullptr, this})
    , _triggeredClose(false)
    , _triggeredSend(false)
    , _sendMutex()
    , _writeBuffer()
    , _writePackets()
{
    LOGD << "New RSocketClient-" << socket << "[" << socketId << "] (" << remoteAddress << ")";
}

RSocketClient::RSocketClient(RSocketClient&& o)
    : _socket(o._socket)
    , _socketId(o._socketId)
    , _remoteAddress(std::move(o._remoteAddress))
    , _socketPort(o._socketPort)
    , _inPacket(std::move(o._inPacket))
    , _composeMode(o._composeMode)
    , _context({o._socket, nullptr, this})
    , _triggeredClose(o._triggeredClose.load())
    , _triggeredSend(o._triggeredSend.load())
    , _sendMutex()
    , _writeBuffer(std::move(o._writeBuffer))
    , _writePackets(std::move(o._writePackets))
{
    LOGV << "Moved RSocketClient-" << _socket << "[" << _socketId << "] (" << _remoteAddress << ")";
}

RSocketClient::~RSocketClient()
{
    LOGD << "Destroy RSocketClient-" << _socket << "[" << _socketId << "] (" << _remoteAddress << ")";
    Terminate();
}

const RQueueContext&    RSocketClient::GetContext() const
{
    return _context;
}

uint32_t    RSocketClient::GetSocketId() const
{
    return _socketId;
}

void    RSocketClient::OnEvent(int event)
{
    switch (event)
    {
        case RomiIOEvent_Read:
            OnReceive();
            break;
        case RomiIOEvent_Write:
            OnSend();
            break;
        case RomiIOEvent_Custom:
            OnClose();
            break;
    }
}

static bool is_continuable_io_error(int err)
{
    switch (err)
    {
        case EAGAIN:
#if EAGAIN != EWOULDBLOCK
        case EWOULDBLOCK:
#endif
            return true;
        default:
            return false;
    }
}

void    RSocketClient::OnSend()
{
    {
        std::scoped_lock<std::mutex> lock(_sendMutex);
        while (!_writePackets.empty())
        {
            const auto& packet = _writePackets.front();
            const auto  buf = packet.GetBuffer();
            const auto  len = packet.GetBufferSize();

            _writeBuffer.insert(_writeBuffer.end(), buf, std::next(buf, len));
            _writePackets.pop();
        }
        TriggerSend(false);
    }

    while (!_writeBuffer.empty())
    {
        const auto len = std::min<size_t>(_writeBuffer.size(), TCP_MTU);
        ssize_t n = write(_socket, &_writeBuffer[0], len);
        if (n < 0)
        {
            if (!is_continuable_io_error(GetRomiLastSocketError()))
            {
                LOGV << "Socket write failed-" << _socket << " (" << _remoteAddress << ")";
                TriggerClose();
            }
            return;
        }
        _writeBuffer.erase(_writeBuffer.begin(), std::next(_writeBuffer.begin(), n));
    }
}

void    RSocketClient::OnReceive()
{
    char    buf[TCP_MTU];
    ssize_t n = 0;

    do
    {
        n = read(_socket, buf, sizeof(buf));
        if (n > 0)
        {
            _inPacket.AppendBuffer(buf, static_cast<size_t>(n));
            LOGV << "Append received " << n << " bytes into buffer. (" << _remoteAddress << ")";
            ComposePacket();
        }
    }
    while (n > 0);

    if (n < 0 && is_continuable_io_error(GetRomiLastSocketError()))
        return;

    if (n == 0)
    {
        LOGD << "Socket closed from " << _remoteAddress;
    }
    else if (n < 0)
    {
        LOGD << "Socket error (" << ErrorUtils::GetString(errno) << ") from " << _remoteAddress;
    }

    TriggerClose();
}

void    RSocketClient::OnClose()
{
    try
    {
        LOGV << "Socket closed event fired.(" << _remoteAddress << ")";
    }
    catch (const std::exception& ex)
    {
        LOGE << "Error on processing socket closed event. " << ex.what() << " (" << _remoteAddress << ")";
    }
    catch (const RException& rex)
    {
        LOGE << "Error on processing socket closed event. " << rex.GetMessage() << " (" << _remoteAddress << ")";
    }
    catch (...)
    {
        LOGE << "Error on processing socket closed event. (Unknown Error) (" << _remoteAddress << ")";
    }
}

void    RSocketClient::ComposePacket()
{
    if (_composeMode == ComposeMode::Header)
    {
        if (_inPacket.GetRawBufferSize() < HEADER_SIZE)
        {
            LOGV << "Lack header packet size. (current: " << _inPacket.GetRawBufferSize() << " / need: " << HEADER_SIZE << ") (" << _remoteAddress << ")";
            return;
        }
        _composeMode = ComposeMode::Content;
        LOGV << "Switched to ComposeMode::Content mode. (" << _remoteAddress << ")";
    }
    else if (_composeMode == ComposeMode::Content)
    {
        do
        {
            const auto receivedHash = _inPacket.Read<uint16_t>();
            const auto size = _inPacket.Read<uint32_t>();
            if (size + HEADER_SIZE > _inPacket.GetRawBufferSize())
            {
                LOGV << "Lack content packet size. (current: " << _inPacket.GetRawBufferSize() << " / need: " << (size + HEADER_SIZE) << ") (" << _remoteAddress << ")";
                return;
            }
            const auto expectedHash = CalculateHash(_inPacket.GetRawBuffer(), size);
            if (expectedHash != receivedHash)
            {
                LOGD.printf("Invalid Packet Hash (Expected: %04hX) (Received: %04hX)\n", expectedHash, receivedHash);
                TriggerClose();
                return;
            }
            LOGV << "Begin ProcessPacket routine. (" << _remoteAddress << ")";
            _inPacket.SetVirtualSize(size);
            try
            {
                ProcessPacket(_inPacket);
            }
            catch (const std::exception& ex)
            {
                LOGE << "Error on process packet " << ex.what() << " (" << _remoteAddress << ")";
            }
            catch (const RException& rex)
            {
                LOGE << "Error on process packet " << rex.GetMessage() << " (" << _remoteAddress << ")";
            }
            catch (...)
            {
                LOGE << "Error on process packet (Unknown Error) (" << _remoteAddress << ")";
            }
            _inPacket.SetVirtualSize(0);
            LOGV << "End ProcessPacket routine. (" << _remoteAddress << ")";
            LOGV << "Erased " << size << " bytes from _inPacket vector. (" << _remoteAddress << ")";
        }
        while (_inPacket.GetRawBufferSize() >= HEADER_SIZE);
        _composeMode = ComposeMode::Header;
        LOGV << "Switched to ComposeMode::Header mode. (" << _remoteAddress << ")";
    }
}

uint16_t    RSocketClient::CalculateHash(const char* buffer, size_t len)
{
    uint16_t    ret = static_cast<uint16_t>(~_ROMI_VERSION * _ROMI_VERSION);

    for (size_t i = 0; i < len; ++i)
    {
        ret *= static_cast<uint16_t>(buffer[i] * 0x33);
        ret += static_cast<uint16_t>(ret * 1441);
    }
    return ret;
}

void    RSocketClient::SendPacket(const OutPacket& outPacket)
{
    OutPacket   oPacket;

    oPacket.Write<uint16_t>(CalculateHash(outPacket.GetBuffer(), outPacket.GetBufferSize()));
    oPacket.Write<uint32_t>(static_cast<uint32_t>(outPacket.GetBufferSize()));
    oPacket.Write(outPacket.GetBuffer(), 0, outPacket.GetBufferSize());
    {
        std::scoped_lock<std::mutex> lock(_sendMutex);
        _writePackets.emplace(std::move(oPacket));
    }
    TriggerSend(true);
}

void    RSocketClient::TriggerClose()
{
    bool    triggeredCloseDesired = false;

    if (_triggeredClose.compare_exchange_strong(triggeredCloseDesired, true))
    {
        LOGV << "Socket closed event triggered.(" << _remoteAddress << ")";
        _socketPort->TriggerEvent(_context, RomiIOEvent_Custom, RomiIOInterested_Enable);
    }
}

void    RSocketClient::TriggerSend(bool newState)
{
    bool    triggeredSendDesired = !newState;

    if (_triggeredSend.compare_exchange_strong(triggeredSendDesired, newState))
    {
        if (newState)
            _socketPort->TriggerEvent(_context, RomiIOEvent_Write, RomiIOInterested_Enable);
        else
            _socketPort->TriggerEvent(_context, RomiIOEvent_Write, RomiIOInterested_Disable);
    }
}

void    RSocketClient::Terminate()
{
    shutdown(_socket, SHUT_RDWR);
    close(_socket);
    LOGD << "Terminated socket-" << _socket << " (" << _remoteAddress << ")";
}
