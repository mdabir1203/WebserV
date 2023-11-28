#include "WebServerConfig.hpp"

WebServerConfig::WebServerConfig()
				: maxClientBodySize(1000000),
				  maxClients(100),
				  timeout(20000), 
				  defaultFolderFile("index.html")
{
	defaultErrorPages[404] = "error_pages/404.html";
}

WebServerConfig::~WebServerConfig()
{
	std::map<std::pair<uint32_t , uint16_t>, std::vector<ServerConfig*>>::iterator mapIterator;
	std::vector<ServerConfig *>::iterator vectorIterator;

	for (mapIterator = this->servers.begin(); mapIterator != this->servers.end(); ++mapIterator)
	{
		for (vectorIterator = mapIterator->second.begin(); vectorIterator != mapIterator->second.end(); ++vectorIterator)
		{
			delete *vectorIterator;
		}
		mapIterator->second.clear();
	}
	this->servers.clear();
}

void WebServerConfig::setClientMaxBodySize(const size_t& value)
{
	this->maxClientBodySize = value;
}

void WebServerConfig::addServerConfig(ServerConfig* serverConfig)
{
	this->servers[std::make_pair(serverConfig->ipAddress, serverConfig->port)].push_back(serverConfig);
}