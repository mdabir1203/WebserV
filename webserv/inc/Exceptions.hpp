/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 18:11:25 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/13 14:50:28 by aputiev          ###   ########.fr       */
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

class Ex_InvalidArgument : public std::exception 
{   
    public:
        const char* what() const throw();
};

class Ex_LocUnclosBracket : public std::exception 
{   
    public:
        const char* what() const throw();
};

class Ex_InvalidServerVarPort : public std::exception 
{   
    public:
        const char* what() const throw();
};

class Ex_InvalidServerVarName : public std::exception 
{   
    public:
        const char* what() const throw();
};

class Ex_InvalidServerVarErrPageCode : public std::exception 
{   
    public:
        const char* what() const throw();
};

class Ex_InvalidServerVarErrPage : public std::exception 
{   
    public:
        const char* what() const throw();
};

class Ex_InvalidRootPage : public std::exception 
{   
    public:
        const char* what() const throw();
};

class Ex_InvalidRootDir : public std::exception 
{   
    public:
        const char* what() const throw();
};

class Ex_InvalidCgiExec : public std::exception 
{   
    public:
        const char* what() const throw();
};

class Ex_InvalidIndexPage : public std::exception 
{   
    public:
        const char* what() const throw();
};

class Ex_InvalidAutoindex : public std::exception 
{   
    public:
        const char* what() const throw();
};

class Ex_InvalidMethod : public std::exception 
{   
    public:
        const char* what() const throw();
};

class Ex_InvalidCgiExt : public std::exception 
{   
    public:
        const char* what() const throw();
};

class Ex_InvalidUploadDir : public std::exception 
{   
    public:
        const char* what() const throw();
};


#endif