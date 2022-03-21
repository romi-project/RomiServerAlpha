// The Romi Project
//
// @ File   :  isocketacceptorcallback.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-17 14:20:51
//

#pragma once

#include "defines.hpp"

class RISocketAcceptorCallback
{
public:
    virtual void    OnAccept(RomiRawSocket socket, const std::string& remoteAddress) = 0;
};
