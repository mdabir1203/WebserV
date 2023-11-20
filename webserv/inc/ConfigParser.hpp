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

/* ========== Locations =========== */
typedef struct Location {
    std::string path;
    std::string root;
    std::string index;
    std::vector<std::string>    cgi_extensions;
    std::string cgi_path;
    std::string upload_dir;
    std::string http_redirect;
    std::vector<std::string>    methods;
    bool autoindex;
}t_Location;

/* ========== Servers =========== */
typedef struct s_serv {
    int         port;
    std::string server_name;
    int         def_timeout;
    int         def_max_clients;
    int         def_max_size_of_file;
    std::map<int, std::string>          	error_pages;
    std::multimap<std::string, Location>	loc;

    //Default constructor:
    s_serv(int Def_timeout, int Def_max_clients, int Def_max_size_of_file);
} t_serv;

enum ParseState {
    STATE_START,
    STATE_SERVER,
    STATE_LOCATION
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
#define EXTENSION 12
#define PATH 13
#define CGI_DIR 14

class ConfigParser
{
    public:
        ConfigParser();
        ~ConfigParser();
        
        std::vector<t_serv> parseConfig(int ac, char **av);

    private:
        void                        checkConfigFile(std::string filename); 
        void                        parseLine(const std::string& line, t_serv& currentServer, std::vector<t_serv>& servers, ParseState& state);
        int                         handleServerVarPort(std::istringstream& iss, std::string &token);
        std::string                 handleServerVarName(std::istringstream& iss, std::string &input);
        bool                        directoryExists(const std::string& path, int specifier);
        int                         checkCodeErrorPage(std::istringstream& iss, std::string &str);
        bool                        checkFileExist(const std::string &filePath, int specifier);
        bool                        checkIfServerDataEnough(t_serv& currentSever);
        std::string                 check_for_double_location(std::istringstream& iss, t_serv& currentServer);
        void                        check_is_token_allowed(std::string &token);
        int                         handleGlobalSettings(std::istringstream& iss, std::string &token, int specifier);
        std::string                 checkToken(std::istringstream& iss, std::string &token, bool check_empty);
        std::vector<std::string>    handleCgiExt(std::istringstream& iss);
        std::vector<std::string>    handleMethods(std::istringstream& iss);

        std::set<int>   InvalidCodesList;
};

#endif /* CONFIG_PARSER_HPP */
