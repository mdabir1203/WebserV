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

Location::~Location()
{
    //std::cout << BG_BLUE << "~Location destructor called" << RESET << std::endl;
}



s_serv::s_serv(int Def_timeout, int Def_max_clients, int Def_max_size_of_file) : port(0), server_name("default"), error_pages(), loc() {
    // std::cout << GREEN << "t_serv default constructor called" << RESET << std::endl;
    def_timeout          = Def_timeout;
    def_max_clients      = Def_max_clients;
    def_max_size_of_file = Def_max_size_of_file;
}

s_serv::~s_serv()
{   
    //std::cout << GREEN << "t_serv destructor called" << RESET << std::endl;
}


ConfigParser::ConfigParser()
{  
    initializeInvalidCodesList(InvalidCodesList);
    //std::cout << YELLOW << "ConfigurationParcer object created" << RESET << std::endl;
}

ConfigParser::~ConfigParser()
{   
    //delete &servers;
    //std::cout << YELLOW <<  "ConfigurationParcer object deleted" << RESET << std::endl;
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
                    token = checkToken(iss, token, true, nullptr);
                    if(checkFileExist(token, ERR_PAGE, nullptr) == true)
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
                        Location* location = new Location();
                        current_location = location;
                        fl_location_created = 1;
                    }
                    if(token == "root:")
                    {
                        iss >> token;
                        token = checkToken(iss, token, true, current_location);
                        if(directoryExists(token, ROOT_DIR, current_location) == true)
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
                        token = checkToken(iss, token, true, current_location);
                        if(checkFileExist((current_location->root + "/" + token), INDEX_PAGE, current_location) == true)
                            current_location->index = (current_location->root + "/" + token);                            
                    }
                    else if(token == "cgi_ext:")
                    {     
                        current_location->cgi_extensions = handleCgiExt(iss, current_location);        
                    }
                    else if (token == "cgi_path:")
                    {   
                        iss >> token;                       
                        token = checkToken(iss, token, true, current_location);                        
                        if(directoryExists((current_location->root + "/" + token), CGI_DIR, current_location) == true)
                        {
                            current_location->cgi_path = current_location->root + "/" + token;
                        }			
                    }
                    else if (token == "upload_dir:")
                    {    
                        iss >> token; 
                        token = checkToken(iss, token, false, current_location);                              
                        if(directoryExists((current_location->root + "/" + token), UPLOAD_DIR, current_location) == true)
                            current_location->upload_dir = current_location->root + "/" + token;
                    }
                    else if (token == "http_redirect:")
                    {                          
                        iss >> token;
                        token = checkToken(iss, token, false, current_location); 
                        current_location->http_redirect = token;
                        if (token.empty() == true)
                            current_location->http_redirect = "";                        
                    }
                    else if(token == "methods:")
                    {   
                        current_location->methods = handleMethods(iss, current_location);
                    }
                    else if(token == "autoindex:")
                    {   
                        iss >> token;
                        token = checkToken(iss, token, false, current_location); 
                        if (token == "on")
                            current_location->autoindex = true;
                        else if (token == "off")
                            current_location->autoindex = false;
                        else
                        {
                            if(current_location)
            					delete current_location;
							throw ErrorException("Error: invalid autoindex value in location unit of configuration file");            
                        }                            
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

