/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 18:17:33 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/11 18:44:40 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Common_header.hpp"

const char* Ex_CantOpenConfigFile::what() const throw()
{
    return ("Can't open config file");
}

const char* Ex_ConfigFileIsEmpty::what() const throw()
{
    return ("Empty configuration file");
}


