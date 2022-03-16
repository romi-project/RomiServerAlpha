
#include "std.hpp"
#include "server/acceptor.hpp"
#include "server/server.hpp"
#include "error/socketexception.hpp"

RAcceptor::RAcceptor(std::weak_ptr<RServer> server, std::string host, std::string port)
    : _listenSocket(RomiInvalidSocket)
    , _server(server)
    , _host(host)
    , _port(port)
{
}

RAcceptor::~RAcceptor()
{
}

void RAcceptor::Listen(int backlog)
{
    ADDRINFO    hints{ 0, };
    LPADDRINFO  local;

    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_IP;

    if (getaddrinfo(_host.c_str(), _port.c_str(), &hints, &local) != 0)
        throw RSocketException(WSAGetLastError());
    LOGD << "Local address (" << _host << ":" << _port << ") - "
         << "family:"    << local->ai_family 
         << " socktype:" << local->ai_socktype 
         << " protocol:" << local->ai_protocol;
    if (local == NULL)
        throw RSocketException(WSAGetLastError());

    _listenSocket = WSASocket(local->ai_family, local->ai_socktype, local->ai_protocol,
        NULL, 0, WSA_FLAG_OVERLAPPED);
    if (_listenSocket == INVALID_SOCKET)
        throw RSocketException(WSAGetLastError());
    LOGD << "Created a listening socket";

    if (bind(_listenSocket, local->ai_addr, static_cast<int>(local->ai_addrlen)) == SOCKET_ERROR)
        throw RSocketException(WSAGetLastError());
    LOGD << "Bound a listening socket";

    if (listen(_listenSocket, backlog) == SOCKET_ERROR)
        throw RSocketException(WSAGetLastError());
    LOGD << "Started a listening socket";

    freeaddrinfo(local);
}

void RAcceptor::Shutdown()
{
    Close(_listenSocket);
    LOGD << "Closed a listening socket (" << _host << ":" << _port << ")";
    _listenSocket = INVALID_SOCKET;
}