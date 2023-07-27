/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cschmied <cschmied@student.42wolfsburg.d>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 12:01:45 by cschmied          #+#    #+#             */
/*   Updated: 2023/07/27 12:01:50 by cschmied         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include <iostream>
#include "Common.hpp"
#include "Socket.hpp"

class Server {
public:
    Server();
    ~Server();
    const Socket& getSocket() const;


private:
    Server(const Server& other);
    Server& operator=(const Server& other);

    const Socket       _socket;
};


#endif//WEBSERV_SERVER_HPP
