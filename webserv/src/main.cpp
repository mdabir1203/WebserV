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

// #include <iostream>

// int main()
// {
//   int i = 0;
//   while (i++ < 8)
//     std::cout << "print here bullshitS" << std::endl;
//   return (0);
// }

#include "../inc/Common_header.hpp"

int def_timeout = 5;									
int def_max_clients = 100;							
int def_max_size_of_file = 1000000;					
							
std::vector<t_serv> parseConfig(const std::string& filename);

int main(int ac, char** av)
{	
	std::string			config_file;
	ConfigurationParser parser;

	if(ac == 1)
		config_file = "src/config_files/default.conf";
	else if (ac == 2)
		config_file = av[1];
	else
	{
		std::cerr << "Error: wrong number of arguments\n";
		return 1;
	}
	try
	{	
        std::vector<t_serv> parsedConfig = parser.parseConfig(config_file);

		std::cout << BG_GREEN << "servers: " << parsedConfig.size() << RESET << ":\n";
        for (size_t i = 0; i < parsedConfig.size(); ++i)
		{
			std::cout << "Server " << i + 1 << ":\n";
			std::cout << "Name: " << parsedConfig[i].server_name << "\n";
			
			std::cout << "Ports:";
			for (size_t j = 0; j < parsedConfig[i].port.size(); ++j) {
				std::cout << " " << parsedConfig[i].port[j];
			}
			std::cout << "\n";

			std::cout << "Root: " << parsedConfig[i].Mroot << "\n";

			std::cout << "Error Pages:\n";
			for (std::map<int, std::string>::iterator it = parsedConfig[i].errorPages.begin(); it != parsedConfig[i].errorPages.end(); ++it) {
				std::cout << "  " << it->first << ": " << it->second << "\n";
			}
			int j = 0;
			std::cout << "Locations:\n";
				for (std::multimap<std::string, Location>::iterator it = parsedConfig[i].loc.begin(); it != parsedConfig[i].loc.end(); ++it) {
				std::cout << "  Path: " << it->first << ", CGI Path: " << it->second.cgiPath << "\n";

				std::cout << "  Path: " << it->first << ", index: " << it->second.index << "\n";

				std::cout << "  Path: " << it->first << ", root: " << it->second.root << "\n";
				std::cout <<  j++ << "\n";
			}

			// std::cout << "IP Ports:";
			// for (size_t j = 0; j < parsedConfig[i].ipPort.size(); ++j) {
			// 	std::cout << " " << parsedConfig[i].ipPort[j];
			// }
			std::cout << "\n";

			std::cout << "\n";
		}
    }
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	
	return 0;
}
