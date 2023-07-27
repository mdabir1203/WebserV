/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:01:45 by cschmied          #+#    #+#             */
/*   Updated: 2023/07/27 12:02:43 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() : _socket(AF_INET, SOCK_STREAM, 0, PORT, INADDR_ANY) {
    std::cout << "Server constructor called\n";
}

Server::~Server() {
    std::cout << "Server destructor called\n";
}

Server& Server::operator=(const Server &other) {
    (void)other;
    return *this;
}

Server::Server(const Server &other) : _socket(Socket(other._socket)) {
    std::cout << "copy constructor called\n";
    (void)other;
}

const Socket &Server::getSocket() const {
    return _socket;
}
