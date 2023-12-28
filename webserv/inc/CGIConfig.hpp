#ifndef CGI_CONFIG_HPP_INCLUDED
#define CGI_CONFIG_HPP_INCLUDED

#include <string>
#include <set>

#include "Colors.hpp"

class CGIConfig // not sure what needs to be in here
{
public:
    std::set<std::string> cgiExtensions; // by default empty will be filled in the handler (.py .sh)
    CGIConfig();
    ~CGIConfig();
    // std::string cgiPath;
    // std::string cgiDirectory;
    //  Add more CGI-related configurations if needed

    void printConfig(void) const;
};

#endif /* CGI_CONFIG_HPP_INCLUDED */
