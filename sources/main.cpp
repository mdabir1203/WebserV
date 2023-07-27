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

#include "Socket.hpp"
#include <iostream>

#define PORT 8080

int main() {
    Socket* serverSocket = nullptr;

    try {
        serverSocket = new Socket(AF_INET, SOCK_STREAM, 0, PORT, INADDR_ANY);
    } catch (std::exception & e) {
        std::cerr << e.what() << "\n";
        delete serverSocket;
        return (1);
    }

    if (listen(serverSocket->getSocketDescriptor(), 1) < 0) {
        std::cerr << "Error on listen\n";
        delete serverSocket;
        return 1;
    }

    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    int clientSocket = accept(serverSocket->getSocketDescriptor(), (struct sockaddr*)&clientAddress, &clientAddressLen);

    if (clientSocket < 0) {
        std::cerr << "Error on accept\n";
        delete serverSocket;
        return 1;
    }

    char buffer[1024] = {0};
    read(clientSocket, buffer, sizeof(buffer));
    std::cout << "Received: " << buffer << "\n";

    const char* msg = "Hello from server!\n";
    send(clientSocket, msg, strlen(msg), 0);

    close(clientSocket);
    delete serverSocket;
    return 0;
}
