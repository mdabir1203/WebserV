#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

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
#include <set>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <bitset>
#include "RequestParser.hpp"

#include "Colors.hpp"
#include "Exceptions.hpp"

class ServerConfig {
public:
    // std::string ipAddress;  // not needed since its the key
    // int port; // not needed since its the key
    std::set<std::string> serverNames;  // Use std::set for faster lookups
    std::map<uint16_t, std::string> customErrorPages;
    const std::map<uint16_t, std::string>& defaultErrorPages;
    size_t maxClientBodySize; // it is not askes for each route, but not specified further -> so in http and server
    bool maxClientBodySizeSet; // necessary to check if it was set in the config file
    std::map<std::string, RouteConfig> routes; // faster loopup than vector // key is the route

    //addRouteConfig() // init methods allowed to 111
};

#endif