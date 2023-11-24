#include "WebServerConfig.hpp"

WebServerConfig::WebServerConfig()
{
// 	this->maxClientBodySize = 0;
// 	this->maxClients = 0;
// 	this->timeout = 0;
}

WebServerConfig::~WebServerConfig()
{
}

void WebServerConfig::setClientMaxBodySize(const std::string& value)
{
	(void)value;
}

void WebServerConfig::addServerConfig(ServerConfig* serverConfig)
{
	this->servers[std::make_pair(serverConfig->ipAddress, serverConfig->port)].push_back(serverConfig);
}
