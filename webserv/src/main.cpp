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


int main(int ac, char** av)
{
	std::string			config_file;
	std::vector<t_serv> servers;
	ConfigurationParcer	config;

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
		config.configParsing(config_file, servers);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	
	return 0;
}
