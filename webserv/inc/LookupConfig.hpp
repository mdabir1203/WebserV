#ifndef LOOKUP_CONFIG_HPP_INCLUDED
#define LOOKUP_CONFIG_HPP_INCLUDED

class WebServerConfig;
class ServerConfig;
class LocationConfig;
class CGIConfig;

#include <string>
#include <set>
#include <stdint.h>

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
		void	updateUriWithLocationPath(const std::string& uri);
		void	updateCurrentCGI();

		const WebServerConfig*	getCurrentWebServer(void) const;
		const ServerConfig*		getCurrentServer(void) const;
		const LocationConfig*	getCurrentLocation(void) const;
		const CGIConfig*		getCurrentCGI(void) const;

		const std::set<uint16_t>	getServerPorts(void) const;

		bool isAutoindex() const;

		const std::string & getUriPath() const;
		std::string 			CGIExt;

	private:
		const WebServerConfig*	currentWebServer;
		const ServerConfig*		currentServer;
		const LocationConfig*	currentLocation;
		const CGIConfig*		currentCGI;
		std::string 			_uriPath;
		
};

#endif /* LOOKUP_CONFIG_HPP_INCLUDED */
