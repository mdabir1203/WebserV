#ifndef LOOKUP_CONFIG_HPP
#define LOOKUP_CONFIG_HPP

#include "WebServerConfig.hpp"

class LookupConfig
{
	public:
		LookupConfig(void);
		LookupConfig(const WebServerConfig* webServer);
		LookupConfig(const LookupConfig& src);
		LookupConfig& operator=(const LookupConfig& rhs);
		~LookupConfig(void);

		void	setCurrentWebServer(const WebServerConfig* webServer);

		void	updateCurrentServer(const uint32_t& ipv4Address, const uint16_t& port, const std::string& host);
		void	updateCurrentLocation(const std::string& uri);
		void	updateUriWithLocationPath(std::string& uri);

		const WebServerConfig*	getCurrentWebServer(void) const;
		const ServerConfig*		getCurrentServer(void) const;
		const LocationConfig*	getCurrentLocation(void) const;
		const CGIConfig*		getCurrentCGI(void) const;

		const std::set<uint16_t>	getServerPorts(void) const;

	private:
		const WebServerConfig*	currentWebServer;
		const ServerConfig*		currentServer;
		const LocationConfig*	currentLocation;
		const CGIConfig*		currentCGI;
};

#endif /* LOOKUP_CONFIG_HPP */
