#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <sstream>
#include <sys/epoll.h>
#include <signal.h>
#include <string.h>
#include <set>

#include <filesystem>

#include "Colors.hpp"

class SocketServer
{
public:
    ~SocketServer(void);

    static SocketServer*    getInstance(const std::set<uint16_t>& ports);
    static SocketServer*    getServer(void);

    void    start();
    void    stop();
    bool    isRunning() const;

private:
    static SocketServer* _instancePtr;

    SocketServer(const SocketServer& src);
    SocketServer& operator=(const SocketServer& rhs);
    SocketServer(const std::set<uint16_t>& ports);

    void    _run();
    int     _acceptClient(int serverSocket);
    void    _HandleClient(int clientSocket);

    std::set<int>   _serverSockets;
    std::set<int>   _clientSockets; //implement it to store open client sockets
    bool            _isRunning;
    int             _epollFd;
};

#endif /* SERVER_HPP */
