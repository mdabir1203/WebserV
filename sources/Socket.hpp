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

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "Common.hpp"

class Socket {
public:
    class creationFailureException : public std::exception {
        virtual const char* what() const _NOEXCEPT;
    };

    class bindingFailureException : public std::exception {
        virtual const char* what() const _NOEXCEPT;
    };

    class setToListenFailureException : public std::exception {
        virtual const char* what() const _NOEXCEPT;
    };

    Socket(int domain, int service, int protocol, int port, unsigned long interface);
    ~Socket();
    int getSocketDescriptor() const;
    const sockaddr_in& getSockaddr() const;
    void setTolisten(void) const;

    Socket(const Socket& other); // should be private --> duplicating fds is dangerous

private:
    Socket& operator=(const Socket& other);
    Socket();

    int _descriptor;
    struct sockaddr_in _address;
};

#endif
