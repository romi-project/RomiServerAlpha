// The Romi Project
//
// @ File   :  inpacket.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 21:09:43
//

#pragma once

#include "std.hpp"
#include "defines.hpp"
#include "error/runtimeexception.hpp"

class InPacket
{
private:
    std::vector<char>   _buffer;
    size_t              _offset;
    size_t              _virtualSize;

public:
    InPacket();
    virtual ~InPacket() = default;
    InPacket(const InPacket&) = default;
    InPacket(InPacket&&) = default;
    InPacket& operator= (const InPacket&) = delete;
    InPacket& operator= (InPacket&&) = delete;

    template<typename T>
    const T&  Read()
    {
        if (_offset + sizeof(T) >= GetSize())
            RaiseEofError();
        const auto& ret = *reinterpret_cast<const T*>(&_buffer[_offset]);
        _offset += sizeof(T);
        return ret;
    }

    void                AppendBuffer(const char* buffer, size_t len);
    const std::string   DumpPacket() const;
    const std::string   ReadStr();
    void                Read(void* dst, size_t len);
    void                SetVirtualSize(size_t virtualSize);
    size_t              GetVirtualSize() const;
    size_t              GetBufferSize() const;
    void                PullUsedPacket(size_t size);

protected:

private:
    static inline void RaiseEofError()
    {
#if _ROMI_WINDOWS
            throw RRuntimeException(ERROR_HANDLE_EOF);
#else
            throw RRuntimeException(EFAULT);
#endif
    }

};
