/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Common_header.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:06:15 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/09 18:27:52 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_HEADER_HPP
# define COMMON_HEADER_HPP

/* ====== std lib header files ====== */
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <sstream>





struct Location {
    std::string cgiPath;
    // Добавь сюда поля, связанные с location, если необходимо
};

typedef struct s_serv {
    std::string name;
    std::string host;
    std::vector<int> port;
    std::string Mroot;
    std::map<int, std::string> errorPages;
    std::multimap<std::string, Location> loc;
    //std::vector<std::string> ipPort;
} t_serv;

enum ParseState {
    STATE_START,
    STATE_SERVER,
    STATE_LOCATION,
    // STATE_ERROR_PAGE,
    // Добавь здесь другие состояния, если необходимо
};


/* ========= my header files ======== */
#include "Colors.hpp"
#include "ConfigurationParcer.hpp"

#endif
