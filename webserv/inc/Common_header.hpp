/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Common_header.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:06:15 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/15 18:11:43 by aputiev          ###   ########.fr       */
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
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <iostream>


//extern  int def_timeout;									
// extern  int def_max_clients;							
// extern  int def_max_size_of_file;

/* ========== Locations =========== */
typedef struct Location {
    std::string path;
    std::string root;
    std::string index;
    std::vector<std::string> cgi_extensions;
    std::vector<std::string> cgi_paths;
    std::string upload_dir;
    std::string http_redirect;
    std::vector<std::string>  methods;
    bool autoindex;
}t_Location;

/* ========== Servers =========== */
typedef struct s_serv {
    int port;
    std::string server_name;
    int def_timeout;
    int def_max_clients;
    int def_max_size_of_file;
    std::map<int, std::string> error_pages;
    std::multimap<std::string, Location> loc;

    //Default constructor:
    s_serv() : port(0), server_name(""), error_pages(), loc() {
        std::cout << "t_serv default constructor called"  << std::endl;
        std::cout << "Port: " << port << std::endl;
        std::cout << "Server Name: \"" << server_name << "\""<< std::endl;
    }
} t_serv;





enum ParseState {
    STATE_START,
    STATE_SERVER,
    STATE_LOCATION,
};


#define TIMEOUT 1
#define MAX_CLIENTS 2
#define MAX_SIZE_OF_FILE 3
#define PORT 4
#define SERVER_NAME 5
#define ROOT_DIR 6
#define CGI_EXEC 7
#define UPLOAD_DIR 8
#define INDEX_PAGE 9
#define ROOT_PAGE 10
#define ERR_PAGE 11



/* ========= my header files ======== */
#include "Colors.hpp"
#include "ConfigurationParcer.hpp"
#include "Exceptions.hpp"

#endif
