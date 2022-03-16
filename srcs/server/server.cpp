// The Romi Project
//
// @ Author :  Tensiya(T2SU)
// @ Since  :  2022-03-14 15:49:37
//
// @ Last Modified      :  Tensiya(T2SU)
// @ Last Modified time :  2022-03-14 15:49:37
//

#include "std.hpp"
#include "server/server.hpp"
#include "error/socketexception.hpp"
#include "error/runtimeexception.hpp"

RServer::RServer(int serverThreadCount)
    : _iocpHandle(NULL)
    , _serverThreadCount(serverThreadCount)
    , _serverThreads(serverThreadCount)
{
}

RServer::~RServer()
{
    WSACleanup();
}

void RServer::Init()
{
    WSADATA wsaData;
    SYSTEM_INFO sysInfo;
    int ret;

    ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ret != 0)
        throw RSocketException(WSAGetLastError());
    _iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (_iocpHandle == NULL)
        throw RSocketException(GetLastError());

    GetNativeSystemInfo(&sysInfo);
    int coreNum = static_cast<int>(sysInfo.dwNumberOfProcessors);
    LOGD << "System processors number: " << coreNum << ".";
    
    for (int i = 0; i < coreNum; ++i)
        _serverThreads.emplace_back(RServerThread(i));
    LOGD << "Created " << coreNum << " RServerThread instance(s).";



}

RomiRawHandle   RServer::GetIocpHandle() const
{
    return _iocpHandle;
}