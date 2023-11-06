
// -> without reference we can't chain the class instance
#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


class SocketServer {
    public:
        static SocketServer* getServer();
        void start();
        void stop();
        bool isRunning() const;
    private:
        SocketServer(int port);
        ~SocketServer();
        int acceptClient();
        void handleClient(int clientSocket);

        int m_socket;
        int m_port;
        bool m_isRunning;
        static HTTPSocketServer* _server;
};

#endif