#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
			: ipAddress(0),
			  port(0),
			  maxClientBodySizeSet(false)
{
	this->customErrorPages[404] = "error_pages/404.html";
}

// ServerConfig::ServerConfig(const std::map<uint16_t, std::string>& defaultErrorPages) : defaultErrorPages(defaultErrorPages)
// {
// 	this->maxClientBodySizeSet = false;
// }



ServerConfig::~ServerConfig()
{
	std::map<std::string, LocationConfig*>::iterator it;
	for (it = this->locations.begin(); it != this->locations.end(); ++it)
	{
		delete it->second;
	}
	this->locations.clear();
}

void ServerConfig::setClientMaxBodySize(const size_t& value)
{
	this->maxClientBodySize = value;
	this->maxClientBodySizeSet = true;
}

void	ServerConfig::addLocationConfig(LocationConfig* locationConfig) // assuming that the location path is unique
{
	this->locations[locationConfig->path] = locationConfig;
}

bool	ServerConfig::isLocationPahtUnique(const std::string& path)
{
	std::map<std::string, LocationConfig*>::iterator it;
	it = this->locations.find(path);
	if (it != this->locations.end())
		return (false);
	return (true);
}

bool ServerConfig::isHostMatched(const std::string& host) const
{
    return (serverNames.find(host) != serverNames.end());
}

void	ServerConfig::printConfig(bool printLocations) const
{
	std::cout << PURPLE << "=========server=========" << RESET << std::endl;
	std::cout << "ipAddress: " << "\t\t" << this->ipv4ToString(this->ipAddress) << std::endl;
	std::cout << "port: " << "\t\t\t" << this->port  << std::endl;
	std::cout << "serverNames: "<< "\t\t";
	std::set<std::string>::const_iterator it;
	for (it = this->serverNames.begin(); it != this->serverNames.end(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	std::cout << "maxClientBodySize: " << "\t" << this->maxClientBodySize << std::endl;
	std::cout << std::boolalpha << "maxClientBodySizeSet: " << "\t" << this->maxClientBodySizeSet  << std::endl;
	std::cout << YELLOW << "customErrorPages: " << RESET << std::endl;
	std::map<uint16_t, std::string>::const_iterator it2;
	for (it2 = this->customErrorPages.begin(); it2 != this->customErrorPages.end(); ++it2)
	{
		std::cout << "    " << it2->first << ": " << it2->second << std::endl;
	}
	//Print information from locations
	if (!printLocations)
		return ;
	std::map<std::string, LocationConfig*>::const_iterator it3;
	for (it3 = this->locations.begin(); it3 != this->locations.end(); ++it3)
	{
		it3->second->printConfig(true);
	}
}

std::string ServerConfig::ipv4ToString(const uint32_t& ipv4) const
{
   std::ostringstream oss;

   oss << ((ipv4 >> 24) & 0xFF) << "."
       << ((ipv4 >> 16) & 0xFF) << "."
       << ((ipv4 >> 8) & 0xFF) << "."
       << (ipv4 & 0xFF);
   return (oss.str());
}
