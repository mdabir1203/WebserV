#include "ConfigParser.hpp"

#include <iostream>
#include <sstream>
#include <cstdlib>

#include "WebServerConfig.hpp"
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "CGIConfig.hpp"
#include "RequestParser.hpp"

		//validate key is appropriate for current block -> move this check to when values are gathered
			//!currentServerConfig && !currentLocationConfig
				//client_max_body_size
				//error_page
			//currentServerConfig && !currentLocationConfig
				//listen
				//server_name
				//error_page
				//client_max_body_size
				//location
			//currentServerConfig && currentLocationConfig
				//root
				//index
				//cgi_extension
				//upload_store
				//return
				//allow_methods
				//autoindex


bool	ConfigParser::isLocationBlockValid()
{
	//check uniquness of location path earlier
	if (!currentLocationConfig->rootDirectory.empty())
		return (true);
	return (false); //throw error no root directory
}

std::string&	ConfigParser::extractSingleValueFromValueVector(const bool isRequired)
{
	if (mulitValues.size() > 1)
	{
		throwConfigError("Multiple values for key", 0, key, true);
	}
	else if (isRequired && (mulitValues.empty() || mulitValues[0].empty()))
	{
		throwConfigError("No value for key", 0, key, true);
	}
	return (mulitValues[0]);
}

void	ConfigParser::handleClientMaxBodySize()
{	
	std::string& numberString = extractSingleValueFromValueVector(true);
	
	std::istringstream iss(numberString);
    size_t result;

	if (!std::isdigit(numberString[0]))
	{
		throwConfigError("Only digits", 0, numberString, true);
	}
	else if (!(iss >> result) || result > 10000000000) // max size 10GB
	{
		throwConfigError("Value out of range 0 - 10 GB", 0, numberString, true);
    }
	else if (iss.eof())
	{
		if (currentServerConfig)
			currentServerConfig->setClientMaxBodySize(result);
		else
			webServerConfig->setClientMaxBodySize(result);
    }
	else
	{
		throwConfigError("Only digits", 0, numberString, true);
	}
	// std::cout << GREEN <<  "maxClientBodySize:" << webServerConfig->maxClientBodySize << RESET << std::endl;
}

void	ConfigParser::handleListen()
{
	std::string& numberString = extractSingleValueFromValueVector(true);

	if (numberString.find_first_of("+-") != std::string::npos)
		throwConfigError("No signs allowed (+-)", 0, numberString, true);

	std::istringstream iss(numberString);
	if (numberString.find(':') != std::string::npos)
		currentServerConfig->ipAddress = extractIp(iss);
	currentServerConfig->port = extractPort(iss);
}

uint32_t	ConfigParser::extractIp(std::istringstream& iss)
{
	int			i = 0;
	uint32_t	ipv4 = 0;
	uint32_t	part;

	while (i < 4)
	{
		iss >> part;
		if (iss.fail() || part > 255)
			throwConfigError("Invalid IP address", 0, iss.str(), true);
		ipv4 |= part << (8 * (3 - i));
		if (i < 3 && iss.get() != '.')
			throwConfigError("Invalid IP address", 0, iss.str(), true);
		i++;
	}
	if (iss.get() != ':')
		throwConfigError("Expected ':' ", 0, iss.str(), true);
	return (ipv4);
}

uint16_t	ConfigParser::extractPort(std::istringstream& iss)
{
	uint16_t port = 0;

	iss >> port;
	if (iss.fail() || !iss.eof())
		throwConfigError("invalid port", 0, iss.str(), true);
	return (port);
}

void	ConfigParser::handleServerName()
{
	for (std::vector<std::string>::iterator it = mulitValues.begin(); it != mulitValues.end(); ++it)
	{
		std::string& serverName = *it;
		currentServerConfig->serverNames.insert(serverName);
	}
	// /* Print: */
	// for (std::set<std::string>::iterator it = currentServerConfig->serverNames.begin(); it != currentServerConfig->serverNames.end(); ++it)
	// {
    //     	std::cout << GREEN << "serverName " << *it << RESET << std::endl;
	// }
}

void	ConfigParser::handleErrorPage()
{
	// std::cout << YELLOW << "handleErrorPage:" << "\"" << mulitValues[0] <<"," << mulitValues [1] << "\"" << RESET << std::endl;
	// std::cout << YELLOW << "previousState" << "\"" << previousState << RESET << std::endl;
	if (mulitValues.size() == 0 || previousState == 1)
	{
       for (std::map<uint16_t, std::string>::iterator it = webServerConfig->defaultErrorPages.begin(); it != webServerConfig->defaultErrorPages.end(); ++it) //TODO: make it proper for map with find
	   { 
	 	    currentServerConfig->customErrorPages[it->first] = it->second;
    	}
	}
	else if (mulitValues.size() > 2) {
       throwConfigError("There must be 2 values for key: ", 0, key, true);
    }
	else if (( (mulitValues.size() == 2) && (mulitValues[0].empty() || mulitValues[1].empty()) ) || (mulitValues.size() == 1) ) {
	   throwConfigError("Empty value for key: ", 0, key, true);
	}
	else
	{
		uint16_t numb = stringToUint16(mulitValues[0]);
		currentServerConfig->customErrorPages[numb] = mulitValues[1];
	}	
	// /* Print: */
	// for (std::map<uint16_t, std::string>::iterator it = currentServerConfig->customErrorPages.begin(); it != currentServerConfig->customErrorPages.end(); ++it) {
	// 	std::cout << GREEN << "Custom Error Page " << it->first << " " << it->second << std::endl;
	// }
}

void	ConfigParser::handleDefaultErrorPage()
{	
	//TO DO: some script that assigns a page? 
}

void    ConfigParser::handleRoot()
{
	std::string& str = extractSingleValueFromValueVector(true);
	if (str.at(str.size() - 1) != '/')
		str.push_back('/');
	currentLocationConfig->rootDirectory = str;
	// /* Print: */
	// std::cout << GREEN <<  "(Loc)root:" << currentLocationConfig->rootDirectory << RESET << std::endl;
}

void    ConfigParser::handleLocationPath()
{
	std::string& locationPath = extractSingleValueFromValueVector(true);

	if (!currentServerConfig->isLocationPathUnique(locationPath))
		throwConfigError("Duplicate location path", 0, locationPath, true);
	currentLocationConfig->path = locationPath;
}

void    ConfigParser::handleIndex()
{
	std::string& locationIndex = extractSingleValueFromValueVector(true);

	currentLocationConfig->defaultFolderFiles.push_back(locationIndex);
    // /* Print: */
	// std::cout << GREEN <<  "(Loc)Index: " << currentLocationConfig->defaultFolderFiles[0] << RESET << std::endl;
	
}

void    ConfigParser::handleCgiExtension()
{

	// if(mulitValues[0]!= ".py" && mulitValues[0] != ".sh"){
	// 	std::cout << PURPLE << "JOPA CGI: " <<  mulitValues[0] <<  RESET << std::endl;
	// 	return;
	// 	// currentLocationConfig->cgiConfig->cgiExtensions.erase(".py");
	// 	// currentLocationConfig->cgiConfig->cgiExtensions.erase(".sh");}
	// }
	// std::cout << PURPLE << "!!!!!=======!!!!!!!CGI PROB: " <<mulitValues[0] << mulitValues[1] << std::endl;
	// std::cout << PURPLE << "!!!!!=======!!!!!!!CGI PROB val: " <<value << std::endl;
	if(mulitValues[0] == ".py" || mulitValues[0] == ".sh")
	{
		for (std::vector<std::string>::iterator ir = mulitValues.begin(); ir != mulitValues.end(); ++ir) 
		{
			// std::cout << PURPLE << "CGI ir: " << *ir << RESET << std::endl;
			if(*ir == ".sh" || *ir == ".py")
			{
				const std::string& CgiExt = *ir;
				currentLocationConfig->cgiConfig->cgiExtensions.insert(CgiExt);
			}		
		}
	}
	// /* Print: */
	// for (std::set<std::string>::iterator it = currentLocationConfig->cgiConfig->cgiExtensions.begin(); it != currentLocationConfig->cgiConfig->cgiExtensions.end(); ++it) {
    //     std::cout << GREEN << "(Loc)CgiExtension " << *it << RESET << std::endl;
	// }
	// //exit(1);
}

void    ConfigParser::handleUploadStore()
{
	std::string& locationUpload = extractSingleValueFromValueVector(false);
	if(locationUpload.empty())
	{
		//TO DO: ???
	}
	else		
		currentLocationConfig->uploadDirectory = locationUpload;
    // /* Print: */
	// std::cout << GREEN << "(Loc)Upload store: " << currentLocationConfig->uploadDirectory << RESET << std::endl;
	
}

void	ConfigParser::handleReturn() //TODO: throw error if return has no value?
{
	if (mulitValues.size() == 0 || previousState == 1)
	{
		currentLocationConfig->statusCode = 0;
		currentLocationConfig->targetUrl = "";
	}
	else if (mulitValues.size() > 2) {
       throwConfigError("There must be 2 values for key: ", 0, key, true);
    }
	else if (( (mulitValues.size() == 2) && (mulitValues[0].empty() || mulitValues[1].empty()) ) || (mulitValues.size() == 1) ) {
	   throwConfigError("Empty value for key: ", 0, key, true);
	}
	else
	{
		currentLocationConfig->statusCode = stringToUint16(mulitValues[0]);
		currentLocationConfig->targetUrl = mulitValues[1];
	}	
	// /* Print: */	
	// std::cout << GREEN << "(Loc)return " << currentLocationConfig->statusCode  << currentLocationConfig->targetUrl << std::endl;
	
}

void    ConfigParser::handleMethods()
{
	currentLocationConfig->allowedMethods.reset();
	if ((mulitValues[0].empty()))
		return;
	for (std::vector<std::string>::iterator it = mulitValues.begin(); it != mulitValues.end(); ++it)
	{
		if(*it == "GET")
			currentLocationConfig->allowedMethods.set(GET);
		else if(*it == "POST")
			currentLocationConfig->allowedMethods.set(POST);
		else if(*it == "DELETE")
			currentLocationConfig->allowedMethods.set(DELETE);
		else
			throwConfigError("Only GET, POST and DELETE are allowed", 0, *it, true);
	}
}

void    ConfigParser::handleAutoindex()
{
	const std::string& autoIndex = extractSingleValueFromValueVector(true);
	if (autoIndex.empty())
		return;
	else if (autoIndex == "on")
		currentLocationConfig->directoryListing = true;
	else if (autoIndex == "off")
		currentLocationConfig->directoryListing = false;
	else
		throwConfigError("autoindex must be on or off", 0, autoIndex, true);
}
