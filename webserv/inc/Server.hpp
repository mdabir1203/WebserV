
// -> without reference we can't chain the class instance
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

class SocketServer
{
    public:
        SocketServer(int port);
        ~SocketServer();

        static SocketServer* getServer();
        void start();
        void stop();
        bool isRunning() const;

        int acceptClient();
        void HandleClient(int clientSocket);

        int m_socket;
        int m_port;
        bool m_isRunning;
        static SocketServer* _server;
    private:
        void run();
};

#endif /* SERVER_HPP */
