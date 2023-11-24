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