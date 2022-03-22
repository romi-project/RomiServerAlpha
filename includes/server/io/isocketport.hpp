// The Romi Project
//
// @ File   :  isocketport.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-19 19:57:54
//

#pragma once

#include "defines.hpp"

class OutPacket;
struct RQueueContext;

class RISocketPort
{
public:
    virtual void    RegisterSocket(RomiRawSocket socketfd, const RQueueContext& context) = 0;
    virtual void    UnregisterSocket(RomiRawSocket socketfd, const RQueueContext& context) = 0;
    virtual void    TriggerEvent(const RQueueContext& context, int type, int flags) = 0;
};
