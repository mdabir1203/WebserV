/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParcer.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 18:53:00 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/11 20:04:43 by aputiev          ###   ########.fr       */
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

void ConfigurationParser::checkConfigFile(std::string filename)
{   
    std::ifstream fin(filename);
    if(!fin.is_open())
    {
        throw Ex_CantOpenConfigFile();
    }
    if (fin.peek() == std::ifstream::traits_type::eof())
    {
        throw Ex_ConfigFileIsEmpty();
    }
    fin.close();
}
	void ConfigurationParser::handleTimeout(std::string &token) 
	{	
		int number;
		if (token.empty())
        	return;				
		std::istringstream stream(token);
    	if (!(stream >> number))
        	throw Ex_InvalidArgument();
		char leftover;
    	if (stream >> leftover)
        	throw Ex_InvalidArgument();
		if(number < 0 || number > 60)
			throw Ex_InvalidArgument();
		def_timeout = number;
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
						handleTimeout(token);

					} 
					else if (token == "max_clients:") 
					{
						iss >> token;
						def_max_clients = std::stoi(token);
					} 
					else if (token == "max_size_of_file:") 
					{
						iss >> token;
						def_max_size_of_file = std::stoi(token);
					}
					if (token == "server") 
                    {   std::cout << RED << "token == server" << token << RESET << std::endl;
                        state = STATE_SERVER;
                        currentServer = t_serv();
                    }
					break;
                case STATE_SERVER:
                    if(token == "{")
                    {
                        flag_open_server_bracket = 1;
                    }
                    else if (token == "listen:") 
                    {
                        iss >> token;
                        std::cout << RED << "token == listen " << token << RESET << std::endl;
                        currentServer.port.push_back(std::stoi(token));
                    } 
                    else if (token == "host:") 
                    {   
                        iss >> token;
                        std::cout << RED << "token == host " << token << RESET << std::endl;
                        currentServer.host = token;
                    }
                     else if (token == "server_name:") 
                    {   
                        iss >> token; 
                        std::cout << RED << "token == server_name " << token << RESET << std::endl;
                        currentServer.name = token;
                    }
                    else if (token == "error_page:")
                    {   
                        iss >> token;
                        std::cout << RED << "token == error_page " << token ;
                        int errorCode = std::stoi(token);
                        iss >> token;
                        std::cout << token << RESET << std::endl;
                        currentServer.errorPages[errorCode] = token;
                    }
                    else if (token == "root:")
                    {   
                        iss >> token;
                        std::cout << RED << "token == root " << token << RESET << std::endl;
                        currentServer.Mroot = token;
                    } 
                    else if (token == "location:")
                    {                
                        iss >> token;
                        std::cout << PURPLE << "token == location " << token << RESET << std::endl;   
                        location_name = token;
                        if (iss >> token && token == "{")                        
                        {   std::cout << RED << " if (iss >> token && token == {)" << token << RESET << std::endl;
                            flag_open_bracket = 1;    
                            std::cout << RED << "token: " << token << RESET << std::endl;
                        }
                        state = STATE_LOCATION;
                    }
                    else if (token == "}" && flag_open_server_bracket == 1 && flag_open_bracket == 0)
                    {
                        flag_open_server_bracket = 0;
                    }
                    break;
                case STATE_LOCATION:
                    if (token == "{")
                    {   std::cout << RED << "(token == {)" << token << RESET << std::endl;
                        flag_open_bracket = 1;                      
                        std::cout << YELLOW << "token: " << token << RESET << std::endl;
                        break;
                    }
                    else if (token == "}" && flag_open_bracket == 1)
                    {   std::cout << YELLOW << "else if (token == })" << token << RESET << std::endl;
                       state = STATE_SERVER;  
                        currentServer.loc.insert(std::make_pair(location_name, *current_location)); 
                       flag_open_bracket = 0;
                       fl_location_created = 0;
                       
                       break;
                    }
                    else if (token != "{" && token != "}" && flag_open_bracket == 1)
                    {   std::cout << YELLOW << "token != { && token != } && flag_open_bracket == 1, " << token << RESET << std::endl;
                        if(fl_location_created == 0)
                        {
                            Location location;
                            current_location = &location;
                            fl_location_created = 1;
                        }
                      
                        
                        if(token == "cgi_path:")
                        {   std::cout << YELLOW << " if(token == cgi_path:) " << token << RESET << std::endl;
                            iss >> token;
                            current_location->cgiPath = token;
                            std::cout << BG_BRIGHT_YELLOW << token << RESET << std::endl;
                        }
                        else if(token == "index:")
                        {
                            iss >> token;
                            current_location->index = token;
                        }
                    
                                               
                    }
                    break;
            }
            if (state == STATE_SERVER && !currentServer.port.empty() && !currentServer.host.empty() && !currentServer.Mroot.empty()
                 && !currentServer.errorPages.empty() && !currentServer.loc.empty() && flag_open_server_bracket == 0)
            {   std::cout << GREEN<< "== add server ==" << token << RESET << std::endl;
                servers.push_back(currentServer);
                currentServer = t_serv();
            }
        }
    }
