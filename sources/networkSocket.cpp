/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   networkSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 13:26:33 by cschmied          #+#    #+#             */
/*   Updated: 2023/07/28 14:29:22 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "networkSocket.hpp"

networkSocket::networkSocket(int port) {
    std::cout << "networkSocket constructor called\n";
    _socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketDescriptor < 0) {
        throw networkSocket::socketCreationException();
    }

    bzero(&_addr, sizeof(_addr));
    _addr.sin_family = AF_INET;               // IPv4
    _addr.sin_port = htons(port);             // port
    _addr.sin_addr.s_addr = htonl(INADDR_ANY);// IP address currently default
    if (bind(_socketDescriptor, (struct sockaddr *) &_addr, sizeof(_addr)) < 0) {
        throw networkSocket::socketBindingException();
    }
}

networkSocket::~networkSocket() {
    std::cout << "networkSocket destructor called\n";
    close(_socketDescriptor);
}

networkSocket &networkSocket::operator=(networkSocket rhs) {
    _addr = rhs.getAddr();
    _socketDescriptor = rhs.getSocketDescriptor();
    return *this;
}

networkSocket::networkSocket(networkSocket &other) {
    std::cout << "networkSocket copy constructor called\n";
    _addr = other.getAddr();
    _socketDescriptor = other.getSocketDescriptor();
}

void    networkSocket::setToListen(int maxBacklog) {
    if (listen(_socketDescriptor, maxBacklog) < 0) {
        throw networkSocket::socketListenException();
    }
}

int networkSocket::getSocketDescriptor() {
    return _socketDescriptor;
}

struct sockaddr_in networkSocket::getAddr() {
    return _addr;
}

networkSocket::networkSocket() {
    std::cout << "networkSocket default constructor called\n";
}


const char *networkSocket::socketBindingException::what() const _NOEXCEPT {
    return "socket binding failed\n";
}

const char *networkSocket::socketCreationException::what() const _NOEXCEPT {
    return "socket creation failed\n";
}

const char *networkSocket::socketListenException::what() const _NOEXCEPT {
    return "socket listen failed\n";
}
