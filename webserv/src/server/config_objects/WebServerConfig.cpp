#include "WebServerConfig.hpp"

#include <iostream>

#include "Colors.hpp"
#include "ServerConfig.hpp"

WebServerConfig *WebServerConfig::_instancePtr = NULL;

WebServerConfig* WebServerConfig::getInstance(void)
{
	//TODO: implement singleton here
	return (WebServerConfig::_instancePtr);
}

WebServerConfig::WebServerConfig()
				: maxClientBodySize(1000000),
				  defaultFolderFile("index.html"),
				  maxClients(100),
				  timeout(20000)
{
	defaultErrorPages[404] = "error_pages/404.html";
	WebServerConfig::_instancePtr = this;
}

WebServerConfig::~WebServerConfig()
{
	std::map<std::pair<uint32_t , uint16_t>, std::vector<ServerConfig*> >::iterator mapIterator;
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


const ServerConfig* WebServerConfig::getServerConfig(uint32_t ipAddress, uint16_t port, const std::string& host) const
{
    std::pair<uint32_t, uint16_t> key(ipAddress, port);
	const ServerConfig* responsibleServerConfig = NULL;

    // Find exact match for IP address and port
    std::map<std::pair<uint32_t, uint16_t>, std::vector<ServerConfig *> >::const_iterator it = servers.find(key);
    if (it != servers.end())
	{
		std::vector<ServerConfig*>::const_iterator serverIt;
		for (serverIt = it->second.begin(); serverIt != it->second.end(); ++serverIt)
		{
			if ((*serverIt)->isHostMatched(host))
				return (*serverIt);
			if (!responsibleServerConfig)
				responsibleServerConfig = *serverIt;
		}
    }

	// Find match for IP-address = Any and port
    key = std::make_pair(0, port);
    it = servers.find(key);

    if (it != servers.end())
	{
		std::vector<ServerConfig*>::const_iterator serverIt;
		for (serverIt = it->second.begin(); serverIt != it->second.end(); ++serverIt)
		{
			if ((*serverIt)->isHostMatched(host))
				return (*serverIt);
			if (!responsibleServerConfig)
				responsibleServerConfig = *serverIt;
		}
    }

    return (responsibleServerConfig); // TODO: What to respond to client if no match is found?
}

const std::set<uint16_t> WebServerConfig::getServerPorts(void) const
{
	std::set<uint16_t> ports;
	std::map<std::pair<uint32_t, uint16_t>, std::vector<ServerConfig *> >::const_iterator it;
	for (it = this->servers.begin(); it != this->servers.end(); ++it)
	{
		ports.insert(it->first.second);
	}
	return (ports);
}

void	WebServerConfig::printConfig(bool printServers) const
{
	std::cout << BLUE << "============WebServerConfig============" << RESET <<std::endl;
	std::cout << "maxClientBodySize: " << this->maxClientBodySize << std::endl;
	std::cout << "defaultFolderFile: " << this->defaultFolderFile << std::endl;
	std::cout << "maxClients: " << this->maxClients << std::endl;
	std::cout << "timeout: " << this->timeout << std::endl;
	std::cout << YELLOW << "defaultErrorPages: " << RESET <<std::endl;
	std::map<uint16_t, std::string>::const_iterator it;
	for (it = this->defaultErrorPages.begin(); it != this->defaultErrorPages.end(); ++it)
	{
		std::cout << "    " << it->first << ": " << it->second << std::endl;
	}
	//Print information from servers
	if (!printServers)
		return ;
	std::map<std::pair<uint32_t , uint16_t>, std::vector<ServerConfig*> >::const_iterator mapIterator;
	std::vector<ServerConfig *>::const_iterator vectorIterator;
	for (mapIterator = this->servers.begin(); mapIterator != this->servers.end(); ++mapIterator)
	{
		for (vectorIterator = mapIterator->second.begin(); vectorIterator != mapIterator->second.end(); ++vectorIterator)
		{
			(*vectorIterator)->printConfig(true);
		}
	}
}
