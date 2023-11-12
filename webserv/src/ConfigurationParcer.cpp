/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParcer.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 18:53:00 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/12 20:53:17 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Common_header.hpp"

int flag_open_bracket = 0;
int flag_open_server_bracket = 0;
int fl_location_created = 0;
Location* current_location;
std::string location_name;


ConfigurationParser::ConfigurationParser()
{
    std::cout << "ConfigurationParcer object created" << std::endl;
}

ConfigurationParser::~ConfigurationParser()
{
    std::cout << "ConfigurationParcer object deleted" << std::endl;
}



 std::vector<t_serv> ConfigurationParser::parseConfig(const std::string& filename) 
 {      
        std::vector<t_serv> servers;
        std::ifstream file(filename.c_str());
        std::string line;
        t_serv currentServer;
        ParseState state = STATE_START;
        
        checkConfigFile(filename);								/* Checks if file can be opened and if it's empty */ 
        
        while (std::getline(file, line))						/* read file line by line */
        {
            parseLine(line, currentServer, servers, state);		/* read line*/
        }
        file.close();
        return servers;											/* return vector of servers */
 }

    void ConfigurationParser::parseLine(const std::string& line, t_serv& currentServer, std::vector<t_serv>& servers, ParseState& state) 
    {
        std::istringstream iss(line);
        std::string token;

        while (iss >> token)
        { 
			std::cout << GREEN << "token: " << token << RESET << std::endl;
            switch (state)
            {
                case STATE_START:
					if (token == "timeout:") 
					{
						iss >> token;
                        std::cout <<  BLUE << "token tiemout: " << token << RESET << std::endl;
						handleGlobalVars(token, TIMEOUT);
					}
					else if (token == "max_clients:") 
					{
						iss >> token;
                        std::cout <<  BLUE << "token maxclients: " << token << RESET << std::endl;
                        handleGlobalVars(token, MAX_CLIENTS);
					} 
					else if (token == "max_size_of_file:") 
					{
						iss >> token;
                        std::cout <<  BLUE << "token max size of file: " << token << RESET << std::endl;
						handleGlobalVars(token, MAX_SIZE_OF_FILE);
					}
					if (token == "server")
                    {   
						std::cout << BG_YELLOW << "token == server" << token << RESET << std::endl;
                        state = STATE_SERVER;
                        currentServer = t_serv();
						iss >> token;
						std::cout << YELLOW << "token after token \"server\"" << token << RESET << std::endl;
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
                        std::cout << RED << "token == listen " << token << RESET << std::endl;
                        currentServer.port = handleServerVarPort(token);
                    } 
                    else if (token == "server_name:")
                    {   
                        iss >> token; 
                        std::cout << RED << "token == server_name " << token << RESET << std::endl;
                        currentServer.server_name = handleServerVarName(token);
                    }
                    else if (token == "error_page")
                    {                        
                        iss >> token;
                        std::cout << RED << "token == error_page " << token << RESET << std::endl;
                        checkCodeErrorPage(token);
                        int errorCode = std::stoi(token);
                        iss >> token;                        
                        std::cout << token << std::endl;
                        if (token[0] == '/') 
                            token.erase(0, 1);
                        if(checkAdressErrorPage(token) == true)
                            currentServer.error_pages[errorCode] = token;
                    }
                    else if (token == "location")
                    {                
                        iss >> token;
                        std::cout << RED << "token == location " << token << RESET << std::endl;   
                        location_name = token;
                        if (iss >> token && token == "{")                        
                        {  
                            std::cout << RED << " open bracket" << token << RESET << std::endl;
                            flag_open_bracket = 1;   
                        }
                        state = STATE_LOCATION;
                    }
                    else if (token == "}" && flag_open_server_bracket == 1 && flag_open_bracket == 0)
                        flag_open_server_bracket = 0;
                    else if (token == "}" && flag_open_server_bracket == 1 && flag_open_bracket == 1)
                        throw Ex_LocUnclosBracket();                    
                    break;
                case STATE_LOCATION:                    
                    if (token == "{")
                    {  
                        std::cout << PURPLE << "(token == {)" << token << RESET << std::endl;
                        flag_open_bracket = 1;                      
                    }
                    else if (token == "}" && flag_open_bracket == 0)
                        throw Ex_LocUnclosBracket(); 
                    else if (token == "}" && flag_open_bracket == 1)
                    {   
                        std::cout << PURPLE << "else if (token == })" << token << RESET << std::endl;
                        state = STATE_SERVER;  
                        currentServer.loc.insert(std::make_pair(location_name, *current_location)); 
                        flag_open_bracket = 0;
                        fl_location_created = 0;                       
                       break;
                    }
                    else if (token != "{" && token != "}" && flag_open_bracket == 1)
                    {   
                        std::cout << PURPLE << "token != { && token != } && flag_open_bracket == 1, " << token << RESET << std::endl;
                        if(fl_location_created == 0)
                        {
                            Location location;
                            current_location = &location;
                            fl_location_created = 1;
                        }
                        if(token == "root:")
                        {
                            iss >> token;                        
                            std::cout << PURPLE << "root: " << token << RESET << std::endl;
                            if (token[0] == '/') 
                                token.erase(0, 1);
                            if(checkAdressRootPage(token) == true)
                                current_location->root = token;
                        }                    
                        if(token == "cgi_path:")
                        {   std::cout << PURPLE << " if(token == cgi_path:) " << token << RESET << std::endl;
                            iss >> token;
                            current_location->cgi_path = token;
                            std::cout << PURPLE << token << RESET << std::endl;
                        }
                        else if(token == "index:")
                        {   std::cout << PURPLE << " if(token == cgi_path:) " << token << RESET << std::endl;
                            iss >> token;
                            current_location->index = token;
                        }                                               
                    }
                    break;
            }
            if (state == STATE_SERVER && (currentServer.port != 0) && !currentServer.server_name.empty()
                 && flag_open_server_bracket == 0 && flag_open_bracket == 0)
            {   
                std::cout << BG_BRIGHT_GREEN<< "== add server ==" << token << RESET << std::endl;
                servers.push_back(currentServer);
                currentServer = t_serv();
            }
        }
    }
