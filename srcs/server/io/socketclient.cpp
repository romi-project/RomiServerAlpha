// The Romi Project
//
// @ File   :  socketclient.cpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 14:20:44
//

#include "server/io/socketclient.hpp"
#include "server/io/isocketport.hpp"

RSocketClient::RSocketClient(RomiRawSocket socket, const std::string& remoteAddress, RISocketPort* socketPort)
    : _socket()
    , _remoteAddress(remoteAddress)
    , _inPacket()
    , _assembleMode(AssembleMode::Header)
    , _context({socket, nullptr, this})
    , _socketPort(socketPort)
    , _overlappedSend(false)
    , _shouldClose(false)
{
    LOGD << "New RSocketClient-" << socket << " (" << remoteAddress << ")";
}

RSocketClient::RSocketClient(RSocketClient&& o)
    : _socket(o._socket)
    , _remoteAddress(std::move(o._remoteAddress))
    , _inPacket(std::move(o._inPacket))
    , _assembleMode(o._assembleMode)
    , _context({o._socket, nullptr, this})
    , _socketPort(o._socketPort)
    , _overlappedSend(o._overlappedSend.load())
    , _shouldClose(o._shouldClose.load())
{
    LOGV << "Moved RSocketClient-" << _socket << " (" << _remoteAddress << ")";
}

RSocketClient::~RSocketClient()
{
    LOGD << "Deleted RSocketClient-" << _socket << " (" << _remoteAddress << ")";
}

const RQueueContext&  RSocketClient::GetContext() const
{
    return _context;
}

void    RSocketClient::OnSend()
{
    if (_writePackets.empty())
        return;
    const auto& packet = _writePackets.front();
    ssize_t n = _socketPort->Write(_socket, packet.GetBuffer(), packet.GetBufferSize());
    if (n < 0)
    {
        LOGV << "Socket write failed-" << _socket << " (" << _remoteAddress << ")";
        Close();
    }
}

void    RSocketClient::OnReceive(const char* buffer, size_t len)
{
    if (buffer == nullptr)
    {
        LOGV << "Socket closed event fired.(" << _remoteAddress << ")";
        OnClose();
    }
    else
    {
        _inPacket.AppendBuffer(buffer, len);
        LOGV << "Append received " << len << " bytes into buffer. (" << _remoteAddress << ")";
        AssemblePacket();
    }
}

void    RSocketClient::AssemblePacket()
{
    if (_assembleMode == AssembleMode::Header)
    {
        if (_inPacket.GetBufferSize() < HEADER_SIZE)
        {
            LOGV << "Lack header packet size. (current: " << _inPacket.GetBufferSize() << " / need: " << HEADER_SIZE << ") (" << _remoteAddress << ")";
            return;
        }
        _assembleMode = AssembleMode::Content;
        LOGV << "Switched to AssembleMode::Content mode. (" << _remoteAddress << ")";
    }
    else if (_assembleMode == AssembleMode::Content)
    {
        while (true)
        {
            auto hash = _inPacket.Read<uint16_t>();
            auto size = _inPacket.Read<uint32_t>();
            if (size + HEADER_SIZE > _inPacket.GetBufferSize())
            {
                LOGV << "Lack content packet size. (current: " << _inPacket.GetBufferSize() << " / need: " << size + HEADER_SIZE << ") (" << _remoteAddress << ")";
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
                const auto& curex = std::current_exception();
                LOGE << "Error on process packet (Unknown Error) (" << _remoteAddress << ")";
            }
            _inPacket.SetVirtualSize(0);
            LOGV << "End ProcessPacket routine. (" << _remoteAddress << ")";
            LOGV << "Erased " << size << " bytes from _inPacket vector. (" << _remoteAddress << ")";
            if (_inPacket.GetBufferSize() < HEADER_SIZE)
            {
                _assembleMode = AssembleMode::Header;
                LOGV << "Switched to AssembleMode::Header mode. (" << _remoteAddress << ")";
                return;
            }
        }

    }
}

uint16_t    RSocketClient::CalculateHash(const char* buffer, size_t len)
{
    uint16_t    ret = 0x107f;

    for (size_t i = 0; i < len; ++i)
    {
        ret *= (static_cast<size_t>(buffer[i]) * 33);
        ret += (static_cast<char>(ret) ^ buffer[i]);
    }
    return ret;
}

void    RSocketClient::SendPacket(const OutPacket& outPacket)
{
    OutPacket   oPacket;

    oPacket.Write<uint16_t>(CalculateHash(outPacket.GetBuffer(), outPacket.GetBufferSize()));
    oPacket.Write<uint32_t>(static_cast<uint32_t>(outPacket.GetBufferSize()));
    oPacket.Write(outPacket.GetBuffer(), 0, outPacket.GetBufferSize());
    _writePackets.emplace(std::move(oPacket));
    TriggerEnableSend();
}

void    RSocketClient::TriggerEnableSend()
{
    bool    overlappedSendDesired = false;

    if (_overlappedSend.compare_exchange_strong(overlappedSendDesired, true))
        _socketPort->EnableSend(_context);
}

void    RSocketClient::Terminate()
{
    shutdown(_socket, SHUT_RDWR);
    close(_socket);
    LOGD << "Terminated socket-" << _socket << " (" << _remoteAddress << ")";
}
