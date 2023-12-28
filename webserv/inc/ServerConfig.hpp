#ifndef SERVER_CONFIG_HPP_INCLUDED
# define SERVER_CONFIG_HPP_INCLUDED

class LocationConfig;

#include <set>
#include <map>
#include <string>
#include <stdint.h>

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

    bool isLocationPathUnique(const std::string& path);
    void addLocationConfig(LocationConfig* locationConfig); // init methods allowed to 111

    void    setClientMaxBodySize(const size_t& value);

    //Lookup config data
    bool isHostMatched(const std::string& host) const;

    const LocationConfig* getLocation(const std::string& uri) const;

    void                printConfig(bool printLocations) const;
    std::string         ipv4ToString(const uint32_t& ipv4) const;
};

#endif /* SERVER_CONFIG_HPP_INCLUDED */