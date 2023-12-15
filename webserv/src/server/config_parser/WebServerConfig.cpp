#include "WebServerConfig.hpp"

WebServerConfig::WebServerConfig()
				: maxClientBodySize(1000000),
				  maxClients(100),
				  timeout(20000), 
				  defaultFolderFile("index.html")
{

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


const ServerConfig* WebServerConfig::getServerConfig(uint32_t ipAddress, uint16_t port, const std::string& host) const
{
    std::pair<uint32_t, uint16_t> key(ipAddress, port);
	const ServerConfig* responsibleServerConfig = NULL;

    // Find exact match for IP address and port
    std::map<std::pair<uint32_t, uint16_t>, std::vector<ServerConfig *>>::const_iterator it = servers.find(key);
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

ServerConfig* WebServerConfig::findServerConfig(const std::string& host, )
{
    std::vector<ServerConfig*>::const_iterator serverIt;
    for (serverIt = it->second.begin(); serverIt != it->second.end(); ++serverIt)
    {
        if ((*serverIt)->isHostMatched(host))
            return (*serverIt);
        if (!responsibleServerConfig)
            responsibleServerConfig = *serverIt;
    }
    return responsibleServerConfig;
}