// The Romi Project
//
// @ File   :  queuecontext.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-18 19:20:20
//

#pragma once

#include "std.hpp"

class RISocketAcceptorCallback;
class RISocketCallback;

struct RQueueContext
{
    RomiRawSocket               Socket;
    RISocketAcceptorCallback*   Acceptor;
    RISocketCallback*           Callback;
};
