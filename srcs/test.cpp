/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smun <smun@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/12 18:08:37 by smun              #+#    #+#             */
/*   Updated: 2022/02/12 21:47:20 by smun             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "std.hpp"

static int error(const char* format, ...)
{
    va_list va;

    va_start(va, format);
    vfprintf(stderr, format, va);
    va_end(va);
    return 1;
}

static int set_nonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
        return error("fcntl get flags error (%s)\n", strerror(errno));
    int setfcntlres = fcntl(fd, F_SETFL, flags, O_NONBLOCK);
    if (setfcntlres < 0)
        return error("fcntl set flags error (%s)\n", strerror(errno));
    return 0;
}

static int set_event(int eventfd, int fd, int filter, int events)
{
    kevent64_s env;
    EV_SET64(&env, fd, filter, events, 0, 0, static_cast<uint64_t>(fd), 0, 0);
    int evregist = kevent64(eventfd, &env, 1, nullptr, 0, 0, nullptr);
    if (evregist < 0)
        return error("register kevent64 error (%s)\n", strerror(errno));
    return 0;
}

int main()
{
    // make kqueue
    int eventfd = ::kqueue();
    if (eventfd < 0)
        return error("kqueue error (%s)\n", strerror(errno));

    // make server socket (ipv4, stream(for TCP), protocol TCP)
    int listenfd = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenfd < 0)
        return error("listener create socket error (%s)\n", strerror(errno));

    // define listen addr
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = PF_INET;
    addr.sin_port = htons(7874);
    addr.sin_addr.s_addr = INADDR_ANY;

    sockaddr* paddr = reinterpret_cast<sockaddr*>(&addr);

    // bind address and port for listening
    int bindres = ::bind(listenfd, paddr, sizeof(*paddr));
    if (bindres < 0)
        return error("socket bind error (%s)\n", strerror(errno));

    // listen for accepting
    int listenres = ::listen(listenfd, 100);
    if (listenres < 0)
        return error("socket listen error (%s)\n", strerror(errno));

    // set socket as nonblocking
    if (set_nonblock(listenfd) != 0)
        return 1;

    // register read event
    if (set_event(eventfd, listenfd, EVFILT_READ, EV_ADD | EV_ENABLE) != 0)
        return 1;

    const int MaxEvents = 20;
    kevent64_s events[MaxEvents];

    while (true)
    {
        int numbers = ::kevent64(eventfd, NULL, 0, events, MaxEvents, 0, nullptr);
        for (int i = 0; i < numbers; i++)
        {
            kevent64_s& event = events[i];
            int socketfd = static_cast<int>(event.udata);
            int filter = event.filter;
            // accept/read
            if (filter == EVFILT_READ)
            {
                // accept
                if (socketfd == listenfd)
                {
                    sockaddr_in remoteaddr;
                    socklen_t remoteaddr_size = sizeof(remoteaddr);
                    int clientfd = ::accept(socketfd, reinterpret_cast<sockaddr*>(&remoteaddr), &remoteaddr_size);
                    if (clientfd < 0)
                        return error("failed accepting (%s)\n", strerror(errno));
                    std::printf("incoming a connection (%d) at %s\n", clientfd, inet_ntoa(remoteaddr.sin_addr));
                    set_nonblock(clientfd);
                    if (set_event(eventfd, clientfd, EVFILT_READ, EV_ADD | EV_ENABLE) != 0)
                        return 1;
                }

                // read
                else
                {
                    char buf[4096];
                    int n = 0;
                    while (true)
                    {
                        n = ::read(socketfd, buf, sizeof(buf));
                        if (n < 0 && errno == EAGAIN) // non blocking mode (no data can be read for now)
                            goto NEXT_LOOP;
                        if (n < 0)
                        {
                            error("read error (%s)\n", strerror(errno));
                            break;
                        }
                        if (n == 0)
                            break;
                        write(STDOUT_FILENO, buf, n);
                    }
                    printf("client %d is closed.\n", socketfd);
                    shutdown(socketfd, SHUT_RDWR);
                    close(socketfd);
                }
            }
            // write
            else if (filter == EVFILT_WRITE)
            {
                if (set_event(eventfd, socketfd, EVFILT_WRITE, EV_DELETE) != 0)
                    return 1;
            }
            else
                return error("invalid event (%d)\n", filter);
        }
        NEXT_LOOP:
        continue;
    }

    return 0;
}
