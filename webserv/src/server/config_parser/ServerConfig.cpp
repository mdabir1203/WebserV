#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : maxClientBodySizeSet(false)
{
}

// ServerConfig::ServerConfig(const std::map<uint16_t, std::string>& defaultErrorPages) : defaultErrorPages(defaultErrorPages)
// {
// 	this->maxClientBodySizeSet = false;
// }



// ServerConfig::~ServerConfig()
// {
// }

void ServerConfig::setClientMaxBodySize(const std::string& value)
{
	(void)value;
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
