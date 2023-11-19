/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 18:17:33 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/19 14:52:43 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Common_header.hpp"

ErrorException::ErrorException(std::string message) throw()
{
    _message = RED "CONFIG PARSER: " + message + RESET;
}

const char* ErrorException::what() const throw()
{
    return (_message.c_str());
}