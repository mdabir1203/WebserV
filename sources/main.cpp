/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:04:42 by cschmied          #+#    #+#             */
/*   Updated: 2023/07/26 15:14:37 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "Common.hpp"
#include "Socket.hpp"
#include "Server.hpp"

typedef struct s_client {
    struct sockaddr_in  clientAddress;
    socklen_t           clientAddressLen = sizeof(clientAddress);
    int                 socket_fd;
} t_client;

int main() {
    Server* server = nullptr;

    try {
        server = new Server();
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
        delete server;
        return 1;
    }

    try {
        server->getSocket().setTolisten();
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
        delete server;
        return 1;
    }

        struct sockaddr_in clientAddress;
        socklen_t clientAddressLen = sizeof(clientAddress);
        int clientSocket = accept(server->getSocket().getSocketDescriptor(), (struct sockaddr*) &clientAddress, &clientAddressLen);

        if (clientSocket < 0) {
            std::cerr << "Error on accept\n";
            delete server;
            return 1;
        }

        char buffer[1024] = {0};
        read(clientSocket, buffer, sizeof(buffer));
        std::cout << "Received: " << buffer << "\n";
        const char* msg = "Hello from server!\n";
        send(clientSocket, msg, strlen(msg), 0);
        close(clientSocket);

    delete server;
    return 0;
}
