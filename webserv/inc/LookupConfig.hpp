#ifndef LOOKUP_CONFIG_HPP_INCLUDED
#define LOOKUP_CONFIG_HPP_INCLUDED

class CGIConfig;
class WebServerConfig;
class ServerConfig;
class LocationConfig;
class SocketServer;

#define GET_METHOD 0
 #define POST_METHOD 1
 #define DELETE_METHOD 2

#include <string>
#include <set>
#include <stdint.h>
#include <vector>
#include "Response.hpp"
#include <map>

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
		void	updateCurrentServer(const ServerConfig* server);
		void	updateCurrentLocation(const std::string& uri);
		void	updateUriWithLocationPath(const std::string& uri);
		void	updateCurrentCGI();
		void	updateReqestHeaders(const std::map<std::string, std::vector<std::string> >& setOfHeaders);

		const WebServerConfig*	getCurrentWebServer(void) const;
		const ServerConfig*		getCurrentServer(void) const;
		const LocationConfig*	getCurrentLocation(void) const;
		const CGIConfig*		getCurrentCGI(void) const;
		std::string				getCgiScriptPath(void) const;
		std::string  			getUriPath() const;
		std::string				getIpAdress(void) const;

		void	setCgiScriptPath(std::string path);
		void	setUriPath(std::string path);	
		void	setEnvVars(int method, HttpResponse &response);
		void	setQueryString(std::string path);
		void	setIpAdress(std::string &ipAdress);
		void setSocketServer(SocketServer* server);

		const std::set<uint16_t>	getServerPorts(void) const;
		time_t						getTimeout(void) const;

		bool isAutoindex() const;
		
		std::string _retrievePathInfo(const std::string& path);
		
		std::string 			CGIExt;
		SocketServer*			socketServer;

	private:
		const WebServerConfig*	currentWebServer;
		const ServerConfig*		currentServer;
		const LocationConfig*	currentLocation;
		const CGIConfig*		currentCGI;
		std::string 			_uriPath;
		std::vector<std::string>_envVars;
		std::string				_cgiScriptPath;
		std::string				_queryString;
		std::string				_ipAdress;
		std::map<std::string, std::vector<std::string> > _Headers;


		void _updateEnvVarVector(const std::string authTypePrefix, const std::string newAuthType);
		
};

#endif /* LOOKUP_CONFIG_HPP_INCLUDED */
