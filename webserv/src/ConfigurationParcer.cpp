/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParcer.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 18:53:00 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/08 16:58:19 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Common_header.hpp"

    ConfigurationParcer::ConfigurationParcer()
    {
        std::cout << "ConfigurationParcer object created" << std::endl;
    }

    ConfigurationParcer::~ConfigurationParcer()
    {
        std::cout << "ConfigurationParcer object deleted" << std::endl;
    }
    
    std::vector<std::string> ConfigurationParcer::readConfiguration(std::string config_file_name)
    {       
        std::ifstream config_file(config_file_name.c_str());
        std::string buffer;
        std::string line_buffer;
        std::vector<std::string> tokens;
          
        if(!config_file.is_open())
        {
            std::cerr << "Configuration file can't be open" << std::endl;
            exit(1);
        }
        while (std::getline(config_file, buffer))
            line_buffer += buffer + "\n";
        
        char* temp_buffer = new char[line_buffer.length() + 1];
        std::strcpy(temp_buffer, line_buffer.c_str());
        
        char* token = std::strtok(temp_buffer, "\n\t");
        while(token != NULL)
        {
            std::string temp_token = token;
            tokens.push_back(temp_token);
            token = std::strtok(NULL, "\n\t");
        }
        for (size_t i = 0; i < tokens.size(); i++)
        {
            size_t j = tokens[i].find_first_not_of(" ");
            
            if(j != std::string::npos)
                tokens[i].erase(0, j);
            if(tokens[i].find("server") != std::string::npos)
                tokens[i].erase(std::remove(tokens[i].begin(), tokens[i].end(), ' '), tokens[i].end());
                // trim(tokens[i], ' ');                           
        }
        // for (size_t m = 0; m < tokens.size(); m++)   //for test purposes
        //     std::cout << tokens[m] << std::endl;
        delete[] temp_buffer;
        return tokens;
                   
        
    }

    t_serv ConfigurationParcer::tokens_processing(std::vector<std::strings>& tokens, size_t & pos)
    {
        t_serv server;
        
        
    }



    int ConfigurationParcer::configParsing(std::string config_file, std::vector<t_serv> & servers)
    {
        std::vector<std::string> tokens = readConfiguration(config_file);
        for(size_t i = 0; i < tokens.size(); i++)
        {
            if (tokens[i] == "server" && tokens[i+1] == "{")        
            {
                t_serv server = tokens_processing(tokens, i+2); 
                if(tokens[i] != "}")
                {
                    std::cerr << RED << "Error: configuration file has invalid format" << std::endl;
                    exit (1);
                }
                servers.push_back(server);
            }
            else
            {
                std::cerr << RED << "Error: configuration file has invalid format" << std::endl;
                exit (1);
            }
        }
        return 0;
    }
    

    