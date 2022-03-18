// The Romi Project
//
// @ File   :  netutils.cpp
// @ Author :  Tensiya(T2SU)
// @ Email  :  tsi@romi-project.net
// @ Since  :  2022-03-18 18:46:32
//

#include "utils/errorutils.hpp"
#include "utils/netutils.hpp"

//XXX
//  고려 사항?
//    - const std::string 반환 대신, 불필요한 복사를 줄이기 위해서
//      unique_ptr<std::string>를 고려해보기?

const std::string   NetUtils::ConvertAddressToString(int af, const void* addr)
{
    char buffer[INET6_ADDRSTRLEN];

    if (inet_ntop(af, addr, buffer, sizeof(buffer)) == nullptr)
    {
        LOGD << "Failed to convert address to string. (" << GetRomiLastSocketError() << ")";
        return "?.?.?.?";
    }
    return buffer;
}

const std::string    NetUtils::GetPeerName(RomiRawSocket socket)
{
    sockaddr_in addr;
    socklen_t   addrlen = sizeof(addr);

    if (getpeername(socket, reinterpret_cast<sockaddr*>(&addr), &addrlen) < 0)
    {
        LOGD << "Failed on getpeername socket-" << socket << ". (" << ErrorUtils::GetString(GetRomiLastSocketError()) << ")";
        return "UNKNOWN";
    }
    return ConvertAddressToString(addr.sin_family, &addr.sin_addr);
}
