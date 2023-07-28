/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   networkSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 13:17:32 by cschmied          #+#    #+#             */
/*   Updated: 2023/07/28 14:26:38 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

class networkSocket {

public:
    networkSocket(int port);
    networkSocket(networkSocket &other);
    ~networkSocket();
    networkSocket &operator=(networkSocket rhs);

    int     getSocketDescriptor();
    void    setToListen(int maxBacklog);
    struct  sockaddr_in getAddr();

    class socketCreationException : public std::exception {
        virtual const char *what() const _NOEXCEPT;
    };

    class socketBindingException : public std::exception {
        virtual const char *what() const _NOEXCEPT;
    };

    class socketListenException : public std::exception {
        virtual const char *what() const _NOEXCEPT;
    };

private:
    networkSocket();

    struct sockaddr_in _addr;
    int _socketDescriptor;
};

#endif
