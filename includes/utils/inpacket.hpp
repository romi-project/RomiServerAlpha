// The Romi Project
//
// @ File   :  inpacket.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 21:09:43
//

#pragma once

#include "std.hpp"
#include "error/runtimeexception.hpp"

class InPacket
{
private:
    const char* const   _buffer;
    const size_t        _len;
    size_t              _offset;

public:
    InPacket() = delete;

    InPacket(const void* buffer, size_t len);

    virtual ~InPacket() = default;
    InPacket(const InPacket&) = default;
    InPacket(InPacket&&) = default;
    InPacket& operator= (const InPacket&) = delete;
    InPacket& operator= (InPacket&&) = delete;

    const std::string DumpPacket() const;

    template<typename T>
    const T&  Read()
    {
        if (_offset + sizeof(T) >= _len)
#if _ROMI_WINDOWS
            throw RRuntimeException(ERROR_HANDLE_EOF);
#else
            throw RRuntimeException(EFAULT);
#endif
        const auto& ret = *reinterpret_cast<const T*>(&_buffer[_offset]);
        _offset += sizeof(T);
        return ret;
    }

    const std::string  ReadStr()
    {
        const auto len = Read<unsigned short>();
        const auto ret = std::string(&_buffer[_offset]);
        _offset += len;
        return ret;
    }

    void    Read(void* dst, size_t len)
    {
        std::memcpy(dst, &_buffer[_offset], len);
        _offset += len;
    }

protected:

private:

};
