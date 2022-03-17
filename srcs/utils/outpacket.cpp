// The Romi Project
//
// @ File   :  outpacket.cpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 20:39:38
//

#include "std.hpp"
#include "utils/outpacket.hpp"

OutPacket::OutPacket(size_t size)
    : _buffer(size)
{
    LOGD << "Created " << size << " sized OutPacket.";
}

const char* OutPacket::GetBuffer() const
{
    return &_buffer[0];
}

size_t      OutPacket::GetBufferSize() const
{
    return _buffer.size();
}

void    OutPacket::WriteStr(const std::string& str)
{
    Write(static_cast<uint16_t>(str.length()));
    Write(str.c_str(), 0, str.length());
}

void    OutPacket::Write(const void* buffer, size_t offset, size_t len)
{
    const char* cbuffer = reinterpret_cast<const char*>(buffer);

    std::memcpy(Current(), &cbuffer[offset], len);
}

const std::string OutPacket::DumpPacket() const
{
    std::stringstream   ss;
    size_t              size = GetBufferSize();
    const char*         buf = GetBuffer();

    while (size-- != 0)
    {
        ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << *(buf++);
        if (size == 0)
            break;
        ss << ' ';
    }
    return ss.str();
}
