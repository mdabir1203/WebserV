/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:05:15 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/06 13:05:15 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Common_header.hpp"
							
std::vector<t_serv> parseConfig(const std::string& filename);

/////////////////////////////////////////////////////////////// DEBUG  PRINTER ////////////////////////////////////////////////////////////////////////////

void DEBUG_print_config_file( std::vector<t_serv> parsedConfig)
{
		std::cout << BG_GREEN << "servers: " << parsedConfig.size() << RESET << ":\n";
        for (size_t i = 0; i < parsedConfig.size(); ++i)
		{   std::cout << "== Global Variables ==\n";
			std::cout << "def_timeout " << parsedConfig[i].def_timeout << "\n";
			std::cout << "def_max_clients " << parsedConfig[i].def_max_clients << ":\n";
			std::cout << "def_max_size_of_file " << parsedConfig[i].def_max_size_of_file << ":\n";
			std::cout << "\n";
            std::cout << "server #" << i + 1 << " {\n";
            std::cout << "  port: " << parsedConfig[i].port << "\n";            
			std::cout << "  server_name: " << parsedConfig[i].server_name << "\n";
			std::cout << "\n";
            
            /* Error pages */
            for (std::map<int, std::string>::iterator it = parsedConfig[i].error_pages.begin(); it != parsedConfig[i].error_pages.end(); ++it) 
				std::cout << "  error_page " << it->first << " " << it->second << "\n";
			std::cout << "\n";
                
            /* Locations */
            for (std::multimap<std::string, Location>::iterator it = parsedConfig[i].loc.begin(); it != parsedConfig[i].loc.end(); ++it)
			{	
				std::cout <<  "  location "  << it->first << " { \n";
				std::cout << "      root: " << it->second.root << "\n";
				std::cout << "      index: " << it->second.index << "\n";
				std::cout << "      cgi_ext: ";
				for (std::vector<std::string>::const_iterator iter = it->second.cgi_extensions.begin(); iter != it->second.cgi_extensions.end(); ++iter)
				{
        			std::cout << *iter << " ";
				}
				std::cout << "\n";
				std::cout << "      cgi_path: ";
				for (std::vector<std::string>::const_iterator iter = it->second.cgi_paths.begin(); iter != it->second.cgi_paths.end(); ++iter)
				{
        			std::cout << *iter << " ";
				}
				std::cout << "\n";
				std::cout << "      upload_dir: " << it->second.upload_dir << "\n";
				std::cout << "      http_redirect: " << it->second.http_redirect << "\n";
				std::cout << "      methods: ";
				for (std::vector<std::string>::const_iterator iter = it->second.methods.begin(); iter != it->second.methods.end(); ++iter)
				{
        			std::cout << *iter << " ";
				}
				std::cout << "\n";
				std::cout << "      autoindex: " << (bool)it->second.autoindex << "\n";
				std::cout << "  } \n";
    		}
			std::cout << "} \n";
		}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int ac, char** av)
{	
	ConfigurationParser parser;
	try
	{	
        std::vector<t_serv> parsedConfig = parser.parseConfig(ac, av);
		DEBUG_print_config_file(parsedConfig);
    }
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}	
	return 0;
}
