/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 14:02:24 by cschmied          #+#    #+#             */
/*   Updated: 2023/07/28 14:12:43 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "networkSocket.hpp"
#define PORT 8080
#define MAX_BACKLOG 5

int main() {
    networkSocket* serverSocket;

    try {
        serverSocket = new networkSocket(PORT);
    } catch (std::exception& e) {
        std::cerr << e.what();
        return 1;
    }
    try {
        serverSocket->setToListen(MAX_BACKLOG);
    } catch (std::exception & e) {
        std::cerr << e.what();
        return 1;
    }

    while (1) {
        struct sockaddr_in clientSocket;
        int                clientDescriptor;
        socklen_t          socklen;
        char c;

        if ((clientDescriptor = accept(serverSocket->getSocketDescriptor(), \
                   (struct sockaddr *)&clientSocket, &socklen)) < 0) {
            std::cerr << "accepting client socket failed\n";
            exit(1);
        }
        while (read(clientDescriptor, &c, 1) > 0) {
            std::cout << c;
        }
        std::cout << "\n";
    }
    delete serverSocket;
    return 0;
}
