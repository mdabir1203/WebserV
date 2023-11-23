#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

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

#include "Colors.hpp"
#include "Exceptions.hpp"
#include "WebServerConfig.hpp"


enum ParseState {
    STATE_START,
    STATE_SERVER,
    STATE_LOCATION
};


class ConfigParser
{
    public:
        ConfigParser();
        ConfigParser(WebServerConfig &webserverconfig);
        ~ConfigParser();
        
        void         parseConfig(int ac, char **av);

    private:

        WebServerConfig &webserverconfig;
        void ConfigParser::checkConfigFile(std::string filename);
};

#endif
