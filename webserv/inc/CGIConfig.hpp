#ifndef NEWCONFIGDATASTRUCUTRE_HPP
#define NEWCONFIGDATASTRUCUTRE_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include <bitset>
#include "RequestParser.hpp"

class CGIConfig //not sure what needs to be in here
{
public:
    std::set<std::string> cgiExtensions;
    CGIConfig();
    ~CGIConfig();
    //std::string cgiPath;
    //std::string cgiDirectory;
    // Add more CGI-related configurations if needed
};

#endif