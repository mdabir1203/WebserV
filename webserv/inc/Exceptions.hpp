/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 18:11:25 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/11 18:43:47 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

#include <string>
#include <exception>

class Ex_CantOpenConfigFile : public std::exception 
{   
    public:
        const char* what() const throw();
};

class Ex_ConfigFileIsEmpty : public std::exception 
{   
    public:
        const char* what() const throw();
};

#endif