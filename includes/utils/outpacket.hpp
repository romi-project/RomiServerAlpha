// The Romi Project
//
// @ File   :  outpacket.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 20:39:38
//

#pragma once

#include "std.hpp"

class OutPacket
{
private:
    std::vector<char> _buffer;

public:
    OutPacket() = default;
    OutPacket(size_t size);
    virtual ~OutPacket() = default;
    OutPacket(const OutPacket&) = default;
    OutPacket(OutPacket&&) = default;
    OutPacket& operator= (const OutPacket&) = default;
    OutPacket& operator= (OutPacket&&) = default;

    const char* GetBuffer() const;
    size_t      GetBufferSize() const;

    template<typename T>
    void    Write(const T& data)
    {
        _buffer.reserve(_buffer.size() + sizeof(T));
        *reinterpret_cast<T*>(Current()) = data;
    }

    void    WriteStr(const std::string& str);
    void    Write(const void* buffer, size_t offset, size_t len);

    const std::string DumpPacket() const;

private:
    inline char*    Current()
    {
        return &_buffer[_buffer.size()];
    }
};
