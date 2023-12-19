#include "LookupConfig.hpp"

LookupConfig::LookupConfig(void)
			: currentWebServer(NULL),
			  currentServer(NULL),
			  currentLocation(NULL),
			  currentCGI(NULL)
{

}

LookupConfig::LookupConfig(const WebServerConfig* webServer)
			: currentWebServer(webServer),
			  currentServer(NULL),
			  currentLocation(NULL),
			  currentCGI(NULL)
{

}

LookupConfig::LookupConfig(const LookupConfig& src)
{
	*this = src;
}

LookupConfig& LookupConfig::operator=(const LookupConfig& rhs)
{
	if (this == &rhs)
		return (*this);
	this->currentWebServer = rhs.currentWebServer;
	this->currentServer = rhs.currentServer;
	this->currentLocation = rhs.currentLocation;
	this->currentCGI = rhs.currentCGI;
	return (*this);
}

LookupConfig::~LookupConfig(void)
{

}

void LookupConfig::setCurrentWebServer(const WebServerConfig* webServer)
{
	this->currentWebServer = webServer;
}

void LookupConfig::updateCurrentServer(const uint32_t& ipv4Address, const uint16_t& port, const std::string& host)
{
	this->currentServer = this->currentWebServer->getServerConfig(ipv4Address, port, host);
}

void LookupConfig::updateCurrentLocation(std::string& uri)
{
	// this->currentLocation = this->currentServer->getLocation(uri);
	(void)uri;
}

const WebServerConfig* LookupConfig::getCurrentWebServer(void) const
{
	return (this->currentWebServer);
}

const ServerConfig* LookupConfig::getCurrentServer(void) const
{
	return (this->currentServer);
}

const LocationConfig* LookupConfig::getCurrentLocation(void) const
{
	return (this->currentLocation);
}

const CGIConfig* LookupConfig::getCurrentCGI(void) const
{
	return (this->currentCGI);
}

// void LookupConfig::updateCurrentCGI(???)