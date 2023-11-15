/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParcer.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 18:53:00 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/15 22:00:02 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Common_header.hpp"


// Location* current_location;

ConfigurationParser::ConfigurationParser()
{
    std::cout << "ConfigurationParcer object created" << std::endl;
}

ConfigurationParser::~ConfigurationParser()
{
    std::cout << "ConfigurationParcer object deleted" << std::endl;
}

std::vector<t_serv> ConfigurationParser::parseConfig(int ac, char **av) 
{      
        std::vector<t_serv> servers;
        std::string			filename;       
        std::string 		line;
        t_serv 				currentServer;
        ParseState state = STATE_START;  
        
	    if(ac == 1)
		    filename = "src/config_files/default.conf";
	    else if (ac == 2)
		    filename = av[1];
	    else
		   throw Ex_WrongNumArguments();
        std::ifstream file(filename.c_str());   /* Checks if file can be opened and if it's empty */      
        checkConfigFile(filename.c_str());  	/* read file line by line */															
        while (std::getline(file, line))						/* read file line by line */
            parseLine(line, currentServer, servers, state); 	/* read line*/
        file.close();
        return servers;							               	/* return vector of servers */
}

void ConfigurationParser::parseLine(const std::string& line, t_serv& currentServer, std::vector<t_serv>& servers, ParseState& state) 
{
    std::istringstream	iss(line);
    std::string 		token;
    static 				std::string location_name;
    static 				Location* current_location;
    static int 			def_timeout = 5;
    static int 			def_max_clients = 200;
    static int 			def_max_size_of_file = 1000000;
    static int 			flag_open_server_bracket = 0;
    static int 			flag_open_location_bracket = 0;
    static int 			fl_location_created = 0;
    static int			flag_server_end = 0;
	
    while (iss >> token)
    { 
       //std::cout << GREEN << "token: " << token << RESET << std::endl;
        switch (state)
        {
            case STATE_START:
                if (token == "timeout:") 
                {
                    iss >> token;          
                    def_timeout = handleGlobalVars(token, TIMEOUT);     
                }
                else if (token == "max_clients:")
                {
                    iss >> token;
                    def_max_clients = handleGlobalVars(token, MAX_CLIENTS);
                } 
                else if (token == "max_size_of_file:") 
                {
                    iss >> token;
                    def_max_size_of_file = handleGlobalVars(token, MAX_SIZE_OF_FILE);
                }
                if (token == "server")
                {   
                    state = STATE_SERVER;
                    currentServer = t_serv();
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
                    iss >> token;
                    currentServer.port = handleServerVarPort(token);
                    currentServer.def_timeout = def_timeout;
                    currentServer.def_max_clients = def_max_clients;
                    currentServer.def_max_size_of_file = def_max_size_of_file;
                } 
                else if (token == "server_name:")
                {   
                    iss >> token; 
                    currentServer.server_name = handleServerVarName(token);
                }
                else if (token == "error_page")
                {                        
                    iss >> token;
                    checkCodeErrorPage(token);
                    int errorCode = std::stoi(token);
                    iss >> token;                        
                    if (token[0] == '/') 
                        token.erase(0, 1);
                    if(checkFileExist(token, ERR_PAGE) == true)
                        currentServer.error_pages[errorCode] = token;
                }
                else if (token == "location")
                {                
                    iss >> token;
                    location_name = token;
                    check_for_double_location(currentServer, location_name);
                    if (iss >> token && token == "{")                
                        flag_open_location_bracket = 1;
                    state = STATE_LOCATION;
                }
                else if ((token == "}" && flag_open_server_bracket == 0))
                    throw Ex_LocUnclosBracket(); 
                else if (token == "}" && flag_open_server_bracket == 1 && flag_open_location_bracket == 0)
                    flag_open_server_bracket = 0;
                else if (token == "<server_end>")
                {   
                    flag_server_end = 1;
                    if((flag_open_server_bracket == 1 || flag_open_location_bracket == 1) && state == STATE_SERVER)                       
                        throw Ex_LocUnclosBracket();
                    if ( (checkIfServerDataEnough(currentServer) == true) && flag_open_server_bracket == 0 && flag_open_location_bracket == 0 && flag_server_end == 1)
                    {   
                        servers.push_back(currentServer);
                        currentServer = t_serv();
                    }
                }
                break;
            case STATE_LOCATION:                    
                if (token == "{")
                    flag_open_location_bracket = 1;
                else if ((token == "}" && flag_open_location_bracket == 0))
                    throw Ex_LocUnclosBracket(); 
                else if (token == "}" && flag_open_location_bracket == 1)
                {   
                    state = STATE_SERVER;
                    currentServer.loc.insert(std::make_pair(location_name, *current_location)); 
                    flag_open_location_bracket = 0;
                    fl_location_created = 0;                       
                }
                else if ( token == "}}" && flag_open_location_bracket == 1 && flag_open_server_bracket == 1)
                {
                    flag_open_location_bracket = 0;
                    flag_open_server_bracket = 0;
                    state = STATE_SERVER;
                    currentServer.loc.insert(std::make_pair(location_name, *current_location)); 
                    fl_location_created = 0;    
                }
                else if (token != "{" && token != "}" && flag_open_location_bracket == 1)
                {   
                    if(fl_location_created == 0)
                    {
                        Location location;
                        current_location = &location;
                        fl_location_created = 1;
                    }
                    if(token == "root:")
                    {
                        iss >> token;                        
                        if (token[0] == '/')
                            token.erase(0, 1);
                        if(directoryExists(token, ROOT_DIR) == true)
                            current_location->root = token;
                    }
                    else if(token == "index:")
                    {   
                        iss >> token;  
                        if (token[0] == '/') 
                            token.erase(0, 1);
                        if(checkFileExist(token, INDEX_PAGE) == true)
                            current_location->index = token;                            
                    }
                    else if(token == "cgi_ext:")
                    {   
                        while (iss >> token)
                        {
                            if (token[0] != '.')
                                throw Ex_InvalidCgiExt();
                            current_location->cgi_extensions.push_back(token);
                        } 
                    }
                    else if (token == "cgi_path:")
                    {   
                        while (iss >> token)
                        {
                            if(checkFileExist(token, CGI_EXEC) == true)
                                current_location->cgi_paths.push_back(token);
                        }								
                    }
                    else if (token == "upload_dir:")
                    {    
                        iss >> token;            
                        if (token[0] == '/')
                            token.erase(0, 1);                            
                        if(directoryExists(token, UPLOAD_DIR) == true)
                            current_location->upload_dir = token;
                    }
                    else if (token == "http_redirect:")
                    {   
                        if(iss >> token)
                            current_location->http_redirect = token;
                        else
                            current_location->http_redirect = "";
                    }
                    else if(token == "methods:")
                    {   
                        while (iss >> token)
                        {
                            if (token != "GET" && token != "POST" && token != "DELETE")
                                throw Ex_InvalidMethod();
                            current_location->methods.push_back(token);
                        } 
                    }
                    else if(token == "autoindex:")
                    {   
                        iss >> token;
                        if (token == "on")
                            current_location->autoindex = true;
                        else if (token == "off")
                            current_location->autoindex = false;
                        else
                            throw Ex_InvalidAutoindex();            
                    }					                                          
                }
                break;
        }
    }
}

bool ConfigurationParser::checkIfServerDataEnough(t_serv& currentServer)
{   
    std::multimap<std::string, Location>::iterator it = currentServer.loc.find("/");

    if((currentServer.port != 0) && !currentServer.server_name.empty() && (it != currentServer.loc.end()))
    {     
        if(!(it->second.root.empty()) && !(it->second.index.empty()))            
            return true;
    }
    throw Ex_MissedVarInConfig();
    return false;
}
