// The Romi Project
//
// @ File   :  inpacket.cpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 21:09:43
//

#include "std.hpp"
#include "utils/inpacket.hpp"

InPacket::InPacket(const void* buffer, size_t len)
    : _buffer(reinterpret_cast<const char*>(buffer))
    , _len(len)
{
}

const std::string InPacket::DumpPacket() const
{
    return ""; // TODO
}
