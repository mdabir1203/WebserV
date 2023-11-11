/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Common_header.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:06:15 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/11 19:38:52 by aputiev          ###   ########.fr       */
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
#include <exception>

extern  int def_timeout;									
extern  int def_max_clients;							
extern  int def_max_size_of_file;

/* ========== structures =========== */
typedef struct Location {
    std::string path;
    std::string root;
    std::string index;
    std::string cgi_extension;
    std::string cgi_path;
    std::string upload_dir;
    std::string http_redirect;
    std::string methods;
    bool autoindex;
}t_Location;

typedef struct s_serv {
    int port;
    std::string server_name;
    std::map<int, std::string> error_pages;
    std::multimap<std::string, Location> loc;
} t_serv;

enum ParseState {
    STATE_START,
    STATE_SERVER,
    STATE_LOCATION,
    STATE_GLOBAL_VARS,

};

/* ========= my header files ======== */
#include "Colors.hpp"
#include "ConfigurationParcer.hpp"
#include "Exceptions.hpp"

#endif
