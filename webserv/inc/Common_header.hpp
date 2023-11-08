/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Common_header.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 13:06:15 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/08 16:38:45 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_HEADER_HPP
# define COMMON_HEADER_HPP

/* ====== std lib header files ====== */
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>



/* ============ my typedefs ============ */
typedef struct s_serv
{
	std::string								name;
	std::string 							host;
	std::vector<int>						port;
	std::string								root;

} t_serv;


/* ========= my header files ======== */
#include "Colors.hpp"
#include "ConfigurationParcer.hpp"

#endif
