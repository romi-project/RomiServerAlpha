// The Romi Project
//
// @ File   :  inpacket.cpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 21:09:43
//

#include "std.hpp"
#include "utils/inpacket.hpp"
#include "utils/netutils.hpp"

InPacket::InPacket()
    : _buffer()
    , _offset(0)
    , _virtualSize(0)
{
}

void    InPacket::AppendBuffer(const char* buffer, size_t len)
{
    const auto  previousSize = _buffer.size();

    if (previousSize + len > _buffer.capacity())
        _buffer.reserve(previousSize + len);
    std::memcpy(&_buffer[previousSize], buffer, len);
}

const std::string   InPacket::DumpPacket() const
{
    return NetUtils::DumpMemory(&_buffer[0], GetVirtualSize());
}

const std::string   InPacket::ReadStr()
{
    const auto len = Read<unsigned short>();
    if (_offset + len >= GetVirtualSize())
        RaiseEofError();
    const auto ret = std::string(&_buffer[_offset], static_cast<size_t>(len));
    _offset += len;
    return ret;
}

void    InPacket::Read(void* dst, size_t len)
{
    if (_offset + len >= GetVirtualSize())
        RaiseEofError();
    std::memcpy(dst, &_buffer[_offset], len);
    _offset += len;
}

void    InPacket::SetVirtualSize(size_t virtualSize)
{
    _virtualSize = virtualSize;
}

size_t  InPacket::GetVirtualSize() const
{
    return _virtualSize;
}

size_t  InPacket::GetBufferSize() const
{
    return _buffer.size();
}

void    InPacket::PullUsedPacket(size_t size)
{
    _buffer.erase(_buffer.begin(), std::next(_buffer.begin(), size));
}
