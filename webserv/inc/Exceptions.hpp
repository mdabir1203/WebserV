/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 18:11:25 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/19 14:52:33 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

#include <string>
#include <exception>

class ErrorException : public std::exception
{
    private:
        std::string _message;
    public:
        ErrorException(std::string message) throw();       
        virtual const char* what() const throw();
        virtual ~ErrorException() throw() {}
};


#endif