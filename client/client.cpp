#include "Socket.hpp"
#include <cstring>

#define PORT 8080

int main() {
    Socket* clientSocket = nullptr;

    try {
        clientSocket = new Socket(AF_INET, SOCK_STREAM, 0, PORT, INADDR_LOOPBACK);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        delete clientSocket;
        return 1;
    }

    if (connect(clientSocket->getSocketDescriptor(), (struct sockaddr*)&clientSocket->getSockaddr(), sizeof(clientSocket->getSockaddr())) < 0) {
        std::cerr << "Error on connect\n";
        delete clientSocket;
        return 1;
    }

    const char* msg = "Hello from client!\n";
    send(clientSocket->getSocketDescriptor(), msg, strlen(msg), 0);

    char buffer[1024] = {0};
    read(clientSocket->getSocketDescriptor(), buffer, sizeof(buffer));
    std::cout << "Received: " << buffer << "\n";

    delete clientSocket;

    return 0;
}
