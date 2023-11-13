/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParcerUtils.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 12:42:01 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/13 14:51:38 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Common_header.hpp"

extern int def_timeout;

void ConfigurationParser::checkConfigFile(std::string filename)
{   
    std::ifstream fin(filename);
    if(!fin.is_open())
    {
        throw Ex_CantOpenConfigFile();
    }
    if (fin.peek() == std::ifstream::traits_type::eof())
    {
        throw Ex_ConfigFileIsEmpty();
    }
    fin.close();
}

void ConfigurationParser::handleGlobalVars(std::string &token, int specifier) 
{	
		int number;
		char leftover;
		
		if (token.empty())
        	return;				
		std::istringstream stream(token);
    	if (!(stream >> number))
        	throw Ex_InvalidArgument();		
    	if (stream >> leftover)
        	throw Ex_InvalidArgument();
		if(specifier == TIMEOUT && (number < 0 || number > 60))
			throw Ex_InvalidArgument();
		else if(specifier == MAX_CLIENTS && (number < 0 || number > 200))
			throw Ex_InvalidArgument();
		else if(specifier == MAX_SIZE_OF_FILE && (number < 0 || number > 1000000))
			throw Ex_InvalidArgument();
		if(specifier == TIMEOUT)
			def_timeout = number;
		else if(specifier == MAX_CLIENTS)
			def_max_clients = number;
		else if(specifier == MAX_SIZE_OF_FILE)
			def_max_size_of_file = number;
		return ;		
}

int ConfigurationParser::handleServerVarPort(std::string &str) 
{	
    int number; 
    std::size_t index = 0;

    while (index < str.length() && std::isspace(static_cast<unsigned char>(str[index]))) 
        ++index;

    if (index == str.length())
        throw Ex_InvalidServerVarPort();

    while (index < str.length()) 
	{
        if (!std::isdigit(static_cast<unsigned char>(str[index]))) 
			throw Ex_InvalidServerVarPort();
        ++index;
    }

    number = std::atoi(str.c_str());
	
    if (number < 0 || number > 65535)
		throw Ex_InvalidServerVarPort();

	return number;
}

std::string ConfigurationParser::handleServerVarName(std::string &input) 
{	
    std::string trimmedString = input;
	bool containsLetter = false;
    size_t startPos = trimmedString.find_first_not_of(" \t\r\n");
    size_t endPos = trimmedString.find_last_not_of(" \t\r\n");

    if (startPos != std::string::npos && endPos != std::string::npos)
        trimmedString = trimmedString.substr(startPos, endPos - startPos + 1);
	else 
       throw Ex_InvalidServerVarName();
    if (trimmedString.empty())	
        throw Ex_InvalidServerVarName();
		
    
    for (size_t i = 0; i < trimmedString.length(); ++i)
	{
        if (std::isalpha(trimmedString[i]))
		{
            containsLetter = true;
            break;
        }
    }
    if (containsLetter)
        return trimmedString;
	else 
        throw Ex_InvalidServerVarName();
}

int ConfigurationParser::checkCodeErrorPage(std::string &str) 
{   
    int number; 
    std::string::iterator it = str.begin(); // Создаем итератор на начало строки

    while (std::isspace(static_cast<unsigned char>(*it))) 
        ++it;

    if (it == str.end()) // Проверяем, достигли ли конца строки
        throw Ex_InvalidServerVarErrPageCode();

    while (it != str.end()) 
    {
        if (!std::isdigit(static_cast<unsigned char>(*it))) 
            throw Ex_InvalidServerVarErrPageCode();
        ++it;
    }

    number = std::atoi(str.c_str()); // Преобразуем строку в число

    if(number != 404 && number != 403 && number != 400 && number != 405 && number != 410 
        && number != 413 && number != 500 && number != 502)
    {
        throw Ex_InvalidServerVarErrPageCode();
    }

    return number;
}

bool ConfigurationParser::checkFileExist(std::string &filePath, int specifier)
{
    std::string trimmedFilePath = filePath;
    size_t startPos = trimmedFilePath.find_first_not_of(" \t\n\r");
    size_t endPos = trimmedFilePath.find_last_not_of(" \t\n\r");

    if (startPos != std::string::npos && endPos != std::string::npos)
        trimmedFilePath = trimmedFilePath.substr(startPos, endPos - startPos + 1);
	else
    {
        if(specifier == ERR_PAGE)
            throw Ex_InvalidServerVarErrPage();
        else if (specifier == ROOT_PAGE)
            throw Ex_InvalidRootPage();
        else if (specifier == INDEX_PAGE)
            throw Ex_InvalidIndexPage();  
        else if (specifier == CGI_EXEC)
            throw Ex_InvalidCgiExec();                  
    }		
    if (trimmedFilePath.empty())
    {
        if(specifier == ERR_PAGE)
            throw Ex_InvalidServerVarErrPage();
        else if (specifier == ROOT_PAGE)
            throw Ex_InvalidRootPage();
        else if (specifier == INDEX_PAGE)
            throw Ex_InvalidIndexPage();
        else if (specifier == CGI_EXEC)
            throw Ex_InvalidCgiExec();  
    }	
    std::ifstream file(trimmedFilePath.c_str());
	std::cout  << trimmedFilePath.c_str() << RESET << std::endl;
    if (!file.good()) 
	{
        if(specifier == ERR_PAGE)
            throw Ex_InvalidServerVarErrPage();
        else if (specifier == ROOT_PAGE)
            throw Ex_InvalidRootPage();
        else if (specifier == INDEX_PAGE)
            throw Ex_InvalidIndexPage();
        else if (specifier == CGI_EXEC)
            throw Ex_InvalidCgiExec();  
    }
	return true;
}

bool ConfigurationParser::directoryExists(const std::string& path, int specifier)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
    {
        if(specifier == ROOT_DIR)
            throw Ex_InvalidRootDir();
        else if(specifier == UPLOAD_DIR)
            throw Ex_InvalidUploadDir();
    }
    return (info.st_mode & S_IFDIR) != 0;
}
