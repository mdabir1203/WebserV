#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include <set>
#include <stdint.h>

#include "LookupConfig.hpp"

class SocketServer
{
public:
    ~SocketServer(void);

    static SocketServer*    getInstance(const std::set<uint16_t>& ports);
    static SocketServer*    getServer(void);

    void    start();
    void    stop();
    bool    isRunning() const;
    void    setConfiguration(LookupConfig* configuration);

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

    //LookupConfig _configuration; 
    LookupConfig* _configuration;   
};

#endif /* SERVER_HPP_INCLUDED */
