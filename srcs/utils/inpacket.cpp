// The Romi Project
//
// @ File   :  inpacket.cpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 21:09:43
//

#include "std.hpp"
#include "utils/inpacket.hpp"

InPacket::InPacket()
    : _buffer()
    , _offset(0)
{
}

void    InPacket::AppendBuffer(const char* buffer, size_t len)
{
    const auto  previousSize = _buffer.size();

    _buffer.reserve(previousSize + len);
    std::memcpy(&_buffer[previousSize], buffer, len);
}

const std::string InPacket::DumpPacket() const
{
    return ""; // TODO
}

const std::string  InPacket::ReadStr()
{
    const auto len = Read<unsigned short>();
    if (_offset + len >= _buffer.size())
        RaiseEofError();
    const auto ret = std::string(&_buffer[_offset], static_cast<size_t>(len));
    _offset += len;
    return ret;
}

void    InPacket::Read(void* dst, size_t len)
{
    if (_offset + len >= _buffer.size())
        RaiseEofError();
    std::memcpy(dst, &_buffer[_offset], len);
    _offset += len;
}
