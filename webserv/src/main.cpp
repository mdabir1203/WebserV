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

#include "inc/Common_header.hpp"

int main(int ac, char** av)
{
	std::string configFile;

	if(ac == 1)
		configFile = "/config_files/default_conf.conf";
	else if (ac == 2)
		configFile = av[1];
	else
	{
		std::cerr << RED << "Error: wrong number of arguments\n" << RESET;
		return 1;
	}
	try
	{
		/* code */
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	
	return 0;
}