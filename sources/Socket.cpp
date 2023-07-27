/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:06:40 by cschmied          #+#    #+#             */
/*   Updated: 2023/07/26 15:12:06 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int domain, int service, int protocol, int port, unsigned long interface) {
    std::cout << "Socket constructor called\n";

    bzero(&_address, sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = interface;
    _address.sin_port = htons(port);

    _descriptor = socket(domain, service, protocol);
    if (_descriptor < 0) {
        throw(Socket::creationFailureException());
    }
    if (bind(_descriptor, (struct sockaddr *) &_address, sizeof(_address)) < 0) {
        throw(Socket::bindingFailureException());
    }
}

Socket::Socket() {
    std::cout << "Socket constructor called\n";
}

Socket::~Socket() {
    std::cout << "Socket destructor called\n";
    close(_descriptor);
}

Socket::Socket(Socket const & other) {
    (void)other;
    _address = other.getSockaddr();
    _descriptor = other.getSocketDescriptor();
    std::cout << "Socket copy constructor called\n";
}

Socket& Socket::operator=(const Socket &other) {
    _address = other.getSockaddr();
    _descriptor = other.getSocketDescriptor();
    return *this;
}

int Socket::getSocketDescriptor() const {
    return _descriptor;
}

const sockaddr_in &Socket::getSockaddr() const {
    return _address;
}

const char * Socket::bindingFailureException::what() const throw() {
    return "Socket binding failed\n";
}

const char * Socket::creationFailureException::what() const throw() {
    return "Socket creation failed\n";
}
