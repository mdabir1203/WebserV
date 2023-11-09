/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParcer.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 18:53:00 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/09 18:12:40 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Common_header.hpp"

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

        while (std::getline(file, line)) 
        {
            parseLine(line, currentServer, servers, state);
        }
        file.close();
        return servers;
 }

     void ConfigurationParser::parseLine(const std::string& line, t_serv& currentServer, std::vector<t_serv>& servers, ParseState& state) 
    {
        std::istringstream iss(line);
        std::string token;

        while (iss >> token) {
            switch (state) {
                case STATE_START:
                    if (token == "server") 
                    {
                        state = STATE_SERVER;
                        currentServer = t_serv(); // current t_serv struct object reset;
                    }
                    break;
                case STATE_SERVER:
                    if (token == "listen:") 
                    {
                        iss >> token;
                        currentServer.port.push_back(std::stoi(token));
                    } 
                    else if (token == "host:") 
                    {
                        iss >> currentServer.host;
                    }
                     else if (token == "server_name:") 
                    {
                        iss >> currentServer.name;
                    }
                    else if (token == "error_page:")
                    {
                        iss >> token;
                        int errorCode = std::stoi(token);
                        iss >> token; // Assuming it's a relative path to the error page
                        currentServer.errorPages[errorCode] = token;
                    }
                    else if (token == "root:")
                    {
                        iss >> currentServer.Mroot;
                    } 
                    else if (token == "location:")
                    {
                        state = STATE_LOCATION;
                    }
                    break;
                case STATE_LOCATION:
                    if (token == "{") 
                    {
                        Location location;
                        while (iss >> token && token != "}") 
                        {
                            if (token == "cgi_path:") 
                            {
                                iss >> token;
                                location.cgiPath = token;
                            }
                            // Добавь обработку других параметров location, если необходимо
                        }
                        currentServer.loc.insert(std::make_pair("/", location)); // Assuming the location is "/"
                    } 
                    else if (token == "}") 
                    {
                        state = STATE_SERVER;
                    }
                    break;
                // Добавь обработку других состояний, если необходимо
            }
        }

        //if (state == STATE_SERVER && !currentServer.port.empty()) 
        if (state == STATE_SERVER && !currentServer.port.empty() && !currentServer.host.empty() && !currentServer.Mroot.empty())
        {
            servers.push_back(currentServer);
        }
    }