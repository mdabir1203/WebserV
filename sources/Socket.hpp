/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:06:42 by cschmied          #+#    #+#             */
/*   Updated: 2023/07/26 15:08:48 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

class Socket {
public:

    class creationFailureException : public std::exception {
        virtual const char * what() const _NOEXCEPT;
    };

    class bindingFailureException : public std::exception {
        virtual const char * what() const _NOEXCEPT;
    };

    // Default constructor
    Socket(int domain, int service, int protocol, int port, unsigned long interface);

    // Destructor
    ~Socket();

    // Copy constructor
    Socket(const Socket& other);

    // Copy assignment operator
    Socket& operator=(const Socket& other);
    int getSocketDescriptor() const;
    const sockaddr_in& getSockaddr() const;

private:
    // Private members and methods
    Socket();
    int _descriptor;
    struct sockaddr_in _address;
};
#endif
