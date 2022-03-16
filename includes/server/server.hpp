// The Romi Project
//
// @ Author :  Tensiya(T2SU)
// @ Since  :  2022-03-14 15:36:09
//
// @ Last Modified      :  Tensiya(T2SU)
// @ Last Modified time :  2022-03-14 15:36:09
//

#pragma once

#include "std.hpp"
#include "serverthread.hpp"

class RServer
{
private:
    RomiRawHandle               _iocpHandle;
    int                         _serverThreadCount;
    std::vector<RServerThread>  _serverThreads;
    
public:
    RServer(int serverThreadCount);
    virtual ~RServer();

    RServer()                           = delete;
    RServer(const RServer&)             = delete;
    RServer(RServer&&)                  = delete;
    RServer& operator=(const RServer&)  = delete;
    RServer& operator=(RServer&&)       = delete;

    RomiRawHandle   GetIocpHandle() const;

    void        Init();
};