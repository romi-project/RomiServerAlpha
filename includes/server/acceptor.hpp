

#pragma once

#include "std.hpp"

class RServer;

class RAcceptor
{
private:
    std::weak_ptr<RServer>      _server;
	std::string                 _host;
	std::string                 _port;
    RomiRawSocket               _listenSocket;

public:
    RAcceptor(std::weak_ptr<RServer> server, std::string host, std::string port);
    virtual ~RAcceptor();

    RAcceptor() = delete;
    RAcceptor(const RAcceptor&) = delete;
    RAcceptor(RAcceptor&&) = delete;
    RAcceptor& operator=(const RAcceptor&) = delete;
    RAcceptor& operator=(RAcceptor&&) = delete;

    void Listen(int backlog = 20);
    void Shutdown();

private:
    void Accept();
};