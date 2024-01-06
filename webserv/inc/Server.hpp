#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

class ClientState;

#include <set>
#include <map>
#include <stdint.h>
#include <map>

#include "LookupConfig.hpp"

class SocketServer
{
public:
    ~SocketServer(void);

    static SocketServer*    getInstance(void);


    void    start(const std::set<uint16_t> &ports);
    void    stop();
    bool    isRunning() const;

private:
    static SocketServer* _instancePtr;

    SocketServer(void);
    SocketServer(const SocketServer& src);
    SocketServer& operator=(const SocketServer& rhs);

    void    _initServerSockets(const std::set<uint16_t> &ports);
    void    _run(void);
    void    _enterEpollEventLoop(void);
    void    _handleEpollEvents(struct epoll_event* event, int numEvents);
    void    _handleServerSocketEvent(int serverSocket);
    void    _handleClientSocketEvent(int clientSocket);

    ClientState&    _getClientState(const int clientSocket);

    int     _acceptClient(int serverSocket);
    void    _handleClient(ClientState& client);

    void    _handleClientAsync(ClientState& clientState);
    void    _readRequestAsync(ClientState& client);
    void    _processRequest(ClientState& client);
    void    _writeResponseAsync(ClientState& client);
    void    _closeConnection(ClientState& client);


    std::map<int, ClientState*> _clientStates;
    std::set<int>               _serverSockets;
    bool                        _isRunning;
    int                         _epollFd;
};

#endif /* SERVER_HPP_INCLUDED */
