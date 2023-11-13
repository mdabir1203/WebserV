/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 18:17:33 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/13 14:50:28 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Common_header.hpp"

const char* Ex_CantOpenConfigFile::what() const throw()
{
    return ("Error: Can't open configuration file");
}

const char* Ex_ConfigFileIsEmpty::what() const throw()
{
    return ("Error: Empty configuration file");
}

const char* Ex_InvalidArgument::what() const throw()
{
    return ("Error: Invalid glodal variable argument in configuration file");
}
  
const char* Ex_LocUnclosBracket::what() const throw()
{
    return ("Error: there are unclosed bracket in location block in configuration file");
}

const char* Ex_InvalidServerVarPort::what() const throw()
{
    return ("Error: invalid variable \"port\" in configuration file");
}

const char* Ex_InvalidServerVarName::what() const throw()
{
    return ("Error: invalid variable \"server_name\" in configuration file");
}

const char* Ex_InvalidServerVarErrPageCode::what() const throw()
{
    return ("Error: invalid error page code in configuration file");
}

const char* Ex_InvalidServerVarErrPage::what() const throw()
{
    return ("Error: invalid error page in configuration file");
}

const char* Ex_InvalidRootDir::what() const throw()
{
    return ("Error: invalid root directory in location unit of configuration file");
}

const char* Ex_InvalidCgiExec::what() const throw()
{   
        return ("Error: invalid CGI directory in location unit of configuration file");
}

const char* Ex_InvalidIndexPage::what() const throw()
{   
        return ("Error: invalid Index page in location unit of configuration file");
}

const char* Ex_InvalidRootPage::what() const throw()
{   
        return ("Error: invalid root page in location unit of configuration file");
}

const char* Ex_InvalidAutoindex::what() const throw()
{   
        return ("Error: invalid autoindex value in location unit of configuration file");
}

const char* Ex_InvalidMethod::what() const throw()
{   
        return ("Error: invalid method value in location unit of configuration file");
}

const char* Ex_InvalidCgiExt::what() const throw()
{   
        return ("Error: invalid CGI extension in location unit of configuration file");
}

const char* Ex_InvalidUploadDir::what() const throw()
{   
        return ("Error: invalid upload directory in location unit of configuration file");
}