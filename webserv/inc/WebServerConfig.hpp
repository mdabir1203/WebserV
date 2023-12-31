#ifndef WEB_SERVER_CONFIG_HPP_INCLUDED
# define WEB_SERVER_CONFIG_HPP_INCLUDED

class ServerConfig;

#include <map>
#include <vector>
#include <set>
#include <string>
#include <stdint.h>

#include "IConfig.hpp"

class WebServerConfig : public IConfig
{
	public:
	WebServerConfig();
	~WebServerConfig();

    //whate happens if no ip Address is specified?? -> use 0 as default since it means listen to any.

	std::map<std::pair<uint32_t , uint16_t>, std::vector<ServerConfig *> > servers; //uses Ipv4-Addres and port as key. Vector is needed for multiple servers on same IP and port
    std::map<uint16_t, std::string> defaultErrorPages;                              // as switch statement?? // either custom or set in http context of config file // if not specified build on the go in fucntion for default error pages??
    size_t maxClientBodySize;      //--PRINTS:OK  --VALIDATION:OK (0-10Gb)        // in bytes //it is not askes for each route, but not specified further -> so in http and server // init to custom start value


    std::string defaultFolderFile; //--PRINTS:OK     --VALIDATION:   N/A       index.html // init to custom start value 
    const size_t maxClients;       //--PRINTS:OK     --VALIDATION:   N/A              // init to custom start value
    const size_t timeout;          //--PRINTS:OK     --VALIDATION:   N/A                                     // in ms // init to custom start value
                                                                        // size_t maxHeaderFieldsSize; // init to custom start value

    void addServerConfig(ServerConfig* serverConfig);                   //set the defaultErrorPages Reference?

    void setClientMaxBodySize(const size_t& value);



    //Lookup config data
    const ServerConfig*         getServerConfig(uint32_t ipAddress, uint16_t port, const std::string& host) const;
    const std::set<uint16_t>	getServerPorts(void) const;

    // std::string getUriPathFromConfig(const std::string& uri) const;

    void    printConfig(bool printServers) const;
};

#endif /* WEB_SERVER_CONFIG_HPP_INCLUDED */


