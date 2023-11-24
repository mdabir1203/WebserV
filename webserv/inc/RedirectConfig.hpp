#ifndef REDIRECT_CONFIG_HPP
#define REDIRECT_CONFIG_HPP

#include "Server.hpp"

class RedirectConfig // name in the config file is "return code url" || "return code"
{
public:
    uint16_t statusCode;  // e.g., 301, 302, 303, 307, 308
    std::string targetUrl;
};

#endif