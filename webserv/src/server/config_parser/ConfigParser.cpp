#include "ConfigParser.hpp"


// Location* current_location;

/* ===================  HELPERS =================== */
static void initializeInvalidCodesList(std::set<int>& InvalidCodesList)
{
    InvalidCodesList.insert(200);
    InvalidCodesList.insert(201);
    InvalidCodesList.insert(204);
    InvalidCodesList.insert(400);
    InvalidCodesList.insert(401);
    InvalidCodesList.insert(403);
    InvalidCodesList.insert(404);
    InvalidCodesList.insert(500);
    InvalidCodesList.insert(502);
    InvalidCodesList.insert(503);
}

/* static std::string getReasonPhrase1xx(const int statusCode)
{
    switch (statusCode)
    {
        case 100:
            return (" Continue");
        case 101:
            return (" Switching Protocols");
        case 102:
            return (" Processing");
        case 103:
            return (" Early Hints");
        default:
            return ("");
    }
}

static std::string getReasonPhrase2xx(const int statusCode)
{
    switch (statusCode)
    {
        case 200:
            return (" OK");
        case 201:
            return (" Created");
        case 202:
            return (" Accepted");
        case 203:
            return (" Non-Authoritative Information");
        case 204:
            return (" No Content");
        case 205:
            return (" Reset Content");
        case 206:
            return (" Partial Content");
        case 207:
            return (" Multi-Status");
        case 208:
            return (" Already Reported");
        case 226:
            return (" IM Used");
        default:
            return ("");
    }
}

static std::string getReasonPhrase3xx(const int statusCode)
{
    switch (statusCode)
    {
        case 300:
            return (" Multiple Choices");
        case 301:
            return (" Moved Permanently");
        case 302:
            return (" Found");
        case 303:
            return (" See Other");
        case 304:
            return (" Not Modified");
        case 305:
            return (" Use Proxy (deprecated)");
        case 306:
            return (" Switch Proxy (no longer used)");
        case 307:
            return (" Temporary Redirect");
        case 308:
            return (" Permanent Redirect");
        default:
            return ("");
    }
}

static std::string getReasonPhrase4xx(const int statusCode)
{
    switch (statusCode)
    {
        case 400:
            return (" Bad Request");
        case 401:
            return (" Unauthorized");
        case 402:
            return (" Payment Required");
        case 403:
            return (" Forbidden");
        case 404:
            return (" Not Found");
        case 405:
            return (" Method Not Allowed");
        case 406:
            return (" Not Acceptable");
        case 407:
            return (" Proxy Authentication Required");
        case 408:
            return (" Request Timeout");
        case 409:
            return (" Conflict");
        case 410:
            return (" Gone");
        case 411:
            return (" Length Required");
        case 412:
            return (" Precondition Failed");
        case 413:
            return (" Payload Too Large");
        case 414:
            return (" URI Too Long");
        case 415:
            return (" Unsupported Media Type");
        case 416:
            return (" Range Not Satisfiable");
        case 417:
            return (" Expectation Failed");
        case 418:
            return (" I'm a teapot");
        case 421:
            return (" Misdirected Request");
        case 422:
            return (" Unprocessable Entity");
        case 423:
            return (" Locked");
        case 424:
            return (" Failed Dependency");
        case 425:
            return (" Too Early");
        case 426:
            return (" Upgrade Required");
        case 428:
            return (" Precondition Required");
        case 429:
            return (" Too Many Requests");
        case 431:
            return (" Request Header Fields Too Large");
        case 451:
            return (" Unavailable For Legal Reasons");
        default:
            return ("");
    }
}

static std::string getReasonPhrase5xx(const int statusCode)
{
    switch (statusCode)
    {
        case 500:
            return (" Internal Server Error");
        case 501:
            return (" Not Implemented");
        case 502:
            return (" Bad Gateway");
        case 503:
            return (" Service Unavailable");
        case 504:
            return (" Gateway Timeout");
        case 505:
            return (" HTTP Version Not Supported");
        case 506:
            return (" Variant Also Negotiates");
        case 507:
            return (" Insufficient Storage");
        case 508:
            return (" Loop Detected");
        case 510:
            return (" Not Extended");
        case 511:
            return (" Network Authentication Required");
        default:
            return ("");
    }
}

std::string getReasonPhrase(const int statusCode)
{
    switch (statusCode / 100)
    {
       case 1: return getReasonPhrase1xx(statusCode);
       case 2: return getReasonPhrase2xx(statusCode);
       case 3: return getReasonPhrase3xx(statusCode);
       case 4: return getReasonPhrase4xx(statusCode);
       case 5: return getReasonPhrase5xx(statusCode);
       default: return "";
    }
}
*/

static void initializeDefaultErrorPages(std::map<int, std::string>& error_pages)
{
    /* /workspaces/WebserV/webserv/error_pages */
    error_pages[200] = "/workspaces/WebserV/webserv/error_pages/200.html";
}

s_serv::s_serv(int Def_timeout, int Def_max_clients, int Def_max_size_of_file)
        :   port(0),
            server_name("default"),
            def_timeout(Def_timeout),
            def_max_clients(Def_max_clients),
            def_max_size_of_file(Def_max_size_of_file),
            error_pages(),
            loc() 
{
    // std::cout << "t_serv constructor called"  << std::endl;
}

ConfigParser::ConfigParser()
{
    initializeInvalidCodesList(InvalidCodesList);
    //std::cout << "ConfigurationParcer object created" << std::endl;
}

ConfigParser::~ConfigParser()
{
    //std::cout << "ConfigurationParcer object deleted" << std::endl;
}

std::vector<t_serv> ConfigParser::getServers() const
{
    return servers;
}

void    ConfigParser::parseConfig(int ac, char **av) 
{      
        std::string			filename;       
        std::string 		line;
        t_serv 				currentServer(5, 200 , 1000000);
        ParseState state = STATE_START;  
        
	    if(ac == 1)
		    filename = "src/config_files/default.conf";
	    else if (ac == 2)
		    filename = av[1];
	    else
		   throw ErrorException("Error: Error: wrong number of arguments");
        std::ifstream file(filename.c_str());    
        checkConfigFile(filename.c_str());														
        while (std::getline(file, line))
            parseLine(line, currentServer, servers, state); 
        file.close();
        return ;
}

void ConfigParser::parseLine(const std::string& line, t_serv& currentServer, std::vector<t_serv>& servers, ParseState& state) 
{
    std::istringstream	iss(line);
    std::string 		token;
    static 				std::string location_name;
    static 				Location*   current_location;
    static int 			def_timeout                 = 5;
    static int 			def_max_clients             = 200;
    static int 			def_max_size_of_file        = 1000000;
    static int 			flag_open_server_bracket    = 0;
    static int 			flag_open_location_bracket  = 0;
    static int 			fl_location_created         = 0;
    static int			flag_server_end             = 0;
	
    while (iss >> token)
    {
        check_is_token_allowed(token);
        //std::cout << GREEN << "token: " << token << RESET << std::endl;
        switch (state)
        {
            case STATE_START:
                if(token == "timeout:") 
                    def_timeout = handleGlobalSettings(iss, token, TIMEOUT);
                else if (token == "max_clients:")
                    def_max_clients = handleGlobalSettings(iss, token, MAX_CLIENTS);
                else if (token == "max_size_of_file:")
                    def_max_size_of_file = handleGlobalSettings(iss, token, MAX_SIZE_OF_FILE);
                else if (token == "server")
                {   
                    state = STATE_SERVER;
                    currentServer = t_serv(def_timeout, def_max_clients, def_max_size_of_file);
                    iss >> token;
                    flag_server_end = 0;
                    if(token == "{")
                        flag_open_server_bracket = 1;
                }        
                break;
            case STATE_SERVER:
                if(token == "{")
                    flag_open_server_bracket = 1;
                else if (token == "port:")
                {                     
                    currentServer.port = handleServerVarPort(iss, token);
                } 
                else if (token == "server_name:")
                {   
                    currentServer.server_name = handleServerVarName(iss, token);
                }
                else if (token == "error_page")
                {                        
                    int errorCode = checkCodeErrorPage(iss, token);
                    iss >> token;
                    token = checkToken(iss, token, true);
                    if(checkFileExist(token, ERR_PAGE) == true)
                        currentServer.error_pages[errorCode] = token;
                }
                else if (token == "location")
                {         
                    location_name = check_for_double_location(iss, currentServer);
                    if (iss >> token && token == "{")                
                        flag_open_location_bracket = 1;
                    state = STATE_LOCATION;
                }
                else if ((token == "}" && flag_open_server_bracket == 0))
                    throw ErrorException("Unclosed brackets found"); 
                else if (token == "}" && flag_open_server_bracket == 1 && flag_open_location_bracket == 0)
                    flag_open_server_bracket = 0;
                else if (token == "<server_end>")
                {   
                    flag_server_end = 1;
                    if((flag_open_server_bracket == 1 || flag_open_location_bracket == 1) && state == STATE_SERVER)                       
                        throw ErrorException("Unclosed brackets found");
                    if ( (checkIfServerDataEnough(currentServer) == true) && flag_open_server_bracket == 0 && flag_open_location_bracket == 0 && flag_server_end == 1)
                    {   
                        servers.push_back(currentServer);
                        currentServer = t_serv(def_timeout, def_max_clients, def_max_size_of_file);
                    }
                }
                break;
            case STATE_LOCATION:                    
                if (token == "{")
                    flag_open_location_bracket = 1;
                else if ((token == "}" && flag_open_location_bracket == 0))
                    throw ErrorException("Unclosed brackets found"); 
                else if (token == "}" && flag_open_location_bracket == 1)
                { 
                    state = STATE_SERVER;              
                    currentServer.loc.insert(std::make_pair(location_name, *current_location)); //TODO: if possible remove copying the allocated memory
                    delete current_location;
                    current_location = NULL;
                    flag_open_location_bracket = 0;
                    fl_location_created = 0;                       
                }
                else if ( token == "}}" && flag_open_location_bracket == 1 && flag_open_server_bracket == 1)
                {
                    flag_open_location_bracket = 0;
                    flag_open_server_bracket = 0;
                    state = STATE_SERVER;
                    currentServer.loc.insert(std::make_pair(location_name, *current_location)); //TODO: if possible remove copying the allocated memory
                    delete current_location;
                    current_location = NULL;
                    fl_location_created = 0;    
                }
                else if (token != "{" && token != "}" && flag_open_location_bracket == 1)
                {   
                    if(fl_location_created == 0)
                    {
                    //     Location location;
                    //     current_location = &location;
                        Location* location = new Location();
                        current_location = location;
                        fl_location_created = 1;
                    }
                    if(token == "root:")
                    {
                        iss >> token;
                        token = checkToken(iss, token, true);
                        if(directoryExists(token, ROOT_DIR) == true)
                        {
                            if (location_name == "/")
                                current_location->root = token;
                            else 
                                 current_location->root = token + location_name;
                        }
                    }
                    else if(token == "index:")
                    {   
                        iss >> token;
                        token = checkToken(iss, token, true);
                        if(checkFileExist( (current_location->root + "/" + token), INDEX_PAGE) == true)
                            current_location->index = (current_location->root + "/" + token);                            
                    }
                    else if(token == "cgi_ext:")
                    {     
                        current_location->cgi_extensions = handleCgiExt(iss);        
                    }
                    else if (token == "cgi_path:")
                    {   
                        iss >> token;                       
                        token = checkToken(iss, token, true);                        
                        if(directoryExists((current_location->root + "/" + token), CGI_DIR) == true)
                        {
                            current_location->cgi_path = current_location->root + "/" + token;
                        }			
                    }
                    else if (token == "upload_dir:")
                    {    
                        iss >> token; 
                        token = checkToken(iss, token, false);                              
                        if(directoryExists((current_location->root + "/" + token), UPLOAD_DIR) == true)
                            current_location->upload_dir = current_location->root + "/" + token;
                    }
                    else if (token == "http_redirect:")
                    {                          
                        iss >> token;
                        token = checkToken(iss, token, false); 
                        current_location->http_redirect = token;
                        if (token.empty() == true)
                            current_location->http_redirect = "";                        
                    }
                    else if(token == "methods:")
                    {   
                        current_location->methods = handleMethods(iss);
                    }
                    else if(token == "autoindex:")
                    {   
                        iss >> token;
                        token = checkToken(iss, token, false); 
                        if (token == "on")
                            current_location->autoindex = true;
                        else if (token == "off")
                            current_location->autoindex = false;
                        else
                            throw ErrorException("Error: invalid autoindex value in location unit of configuration file");            
                    }					                                          
                }
                break;
        }
    }
}

bool ConfigParser::checkIfServerDataEnough(t_serv& currentServer)
{   
    std::multimap<std::string, Location>::iterator it = currentServer.loc.find("/");

    if((currentServer.port != 0) && (it != currentServer.loc.end()))
    {     
        if(!(it->second.root.empty()) && !(it->second.index.empty()))            
            return true;
    }
    throw ErrorException("Error: missed variable in configuration file");
    return false;
}
