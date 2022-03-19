// The Romi Project
//
// @ File   :  isocketport.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-19 19:57:54
//

#pragma once

#include "std.hpp"

class OutPacket;
class RQueueContext;

class RISocketPort
{
public:
    virtual void    RegisterSocket(RomiRawSocket socket, RQueueContext& context) = 0;
    virtual ssize_t Write(RomiRawSocket socket, const char* buf, size_t len) = 0;
    virtual void    EnableSend(RQueueContext& context) = 0;
};
