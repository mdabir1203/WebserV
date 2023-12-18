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

#include "LocationConfig.hpp"
#include "IConfig.hpp"

class ServerConfig : public IConfig
{
public:
    ServerConfig();
    ~ServerConfig();

    uint32_t ipAddress; //needed to from the key later
    uint16_t port; //needed to from the key later

    std::set<std::string> serverNames;  // Use std::set for faster lookups
    std::map<uint16_t, std::string> customErrorPages;
    // const std::map<uint16_t, std::string>& defaultErrorPages;
    size_t maxClientBodySize; // it is not askes for each route, but not specified further -> so in http and server
    bool maxClientBodySizeSet; // necessary to check if it was set in the config file
    std::map<std::string, LocationConfig*> locations; // faster loopup than vector // key is the route

    bool isLocationPahtUnique(const std::string& path);
    void addLocationConfig(LocationConfig* locationConfig); // init methods allowed to 111

    void    setClientMaxBodySize(const size_t& value);
};

#endif