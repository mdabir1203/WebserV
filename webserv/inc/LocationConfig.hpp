#ifndef LOCATION_CONFIG_HPP
# define LOCATION_CONFIG_HPP

#include "RequestParser.hpp"
#include "CGIConfig.hpp"
#include "RedirectConfig.hpp"

class LocationConfig
{
public:

    LocationConfig();
    ~LocationConfig();

    // std::string route; not needed since its the key// only one per block
    RedirectConfig* redirectConfig;
    CGIConfig*      cgiConfig;  // Include CGIConfig for CGI execution details
    std::string rootDirectory; // if not specified best of luck finding the file //-> only one per block
    std::string uploadDirectory; // if not specified respond with error 500 //-> only one per block // -> named "upload_store" in config
    std::vector<std::string> defaultFolderFiles; // default file to be served if no file is specified in the path
    // -> named "index" in config, default is "index.html" // searched for in the directory requested left to right // for now limit to 1 according to subject interpretation
    //in nginx there can only be one "index" directive per block
    bool directoryListing; // directory listing will come into play if no default file is specified or present // only one per block
    // Add more route-specific configurations as needed

    bool isMethodAllowed(HttpMethod method);
    void disableMethod(HttpMethod method);

    private:
        std::bitset<3> _allowedMethods; // 0 = GET, 1 = POST, 2 = DELETE // init to 111 -> all allowed
        // -> change name in config to  allowed_methods
};

#endif /* LOCATION_CONFIG_HPP */