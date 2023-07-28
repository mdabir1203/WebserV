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
#include <sstream>
#include <fstream>

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

        if ((clientDescriptor = accept(serverSocket->getSocketDescriptor(), \
                   (struct sockaddr *)&clientSocket, &socklen)) < 0) {
            std::cerr << "accepting client socket failed\n";
            exit(1);
        }



        std::string httpResponse =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "\r\n"
                "<html>\r\n"
                "<body>\r\n"
                "<h1>Hello, World!</h1>\r\n"
                "</body>\r\n"
                "</html>\r\n";
        write(clientDescriptor, httpResponse.c_str(), httpResponse.size());
        shutdown(clientDescriptor, SHUT_RDWR);
    }
    delete serverSocket;
    return 0;
}
