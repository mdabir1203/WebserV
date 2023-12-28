#include "LocationConfig.hpp"

#include <iostream>

#include "Colors.hpp"
#include "CGIConfig.hpp"
#include "RequestParser.hpp"

LocationConfig::LocationConfig()
			  : statusCode(0),
			    cgiConfig(NULL),
				rootDirectory(""),
				directoryListing(false),
				allowedMethods(7) // 111 -> all allowed
{
	cgiConfig = new CGIConfig();
}

LocationConfig::~LocationConfig()
{
	delete cgiConfig;
}

bool LocationConfig::isMethodAllowed(int method) const
{
	return (allowedMethods.test(method));
}

void	LocationConfig::printConfig(bool printCGI) const
{
	std::cout << RED << "======location======" << RESET << std::endl;
	std::cout << "path: " << "\t\t\t\t" << this->path << std::endl;
	std::cout << "statusCode: " << "\t\t\t" << this->statusCode << std::endl;
	std::cout << "targetUrl: " << "\t\t" << this->targetUrl << std::endl;
	std::cout << "rootDirectory: " << "\t\t\t" << this->rootDirectory << std::endl;
	std::cout << "uploadDirectory: " << "\t\t" << this->uploadDirectory << std::endl;
	std::cout << "defaultFolderFiles: "<< "\t\t";
	std::vector<std::string>::const_iterator it;
	for (it = this->defaultFolderFiles.begin(); it != this->defaultFolderFiles.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << std::boolalpha << "directoryListing: " << "\t\t"<< this->directoryListing << std::endl;
	std::cout << "allowedMethods: " << "\t\t";
	if (this->isMethodAllowed(GET))
		std::cout << "GET ";
	if (this->isMethodAllowed(POST))
		std::cout << "POST ";
	if (this->isMethodAllowed(DELETE))
		std::cout << "DELETE ";
	std::cout << std::endl;
	//Print information from CGI
	if (!printCGI)
		return ;
	this->cgiConfig->printConfig();
}
