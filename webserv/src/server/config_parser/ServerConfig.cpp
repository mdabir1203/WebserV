#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : maxClientBodySizeSet(false)
{
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
