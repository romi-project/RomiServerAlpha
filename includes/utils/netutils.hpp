// The Romi Project
//
// @ File   :  netutils.hpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-18 18:46:32
//

#pragma once

#include "std.hpp"
#include "defines.hpp"

class NetUtils
{
public:
    NetUtils() = delete;
    virtual ~NetUtils() = delete;
    NetUtils(const NetUtils&) = delete;
    NetUtils(NetUtils&&) = delete;
    NetUtils& operator= (const NetUtils&) = delete;
    NetUtils& operator= (NetUtils&&) = delete;

    static const std::string    ConvertAddressToString(int af, const void* addr);
    static const std::string    GetPeerName(int fd);
    static const std::string    DumpMemory(const void* buffer, size_t len);
};
