#include "LookupConfig.hpp"

#include <iostream>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <arpa/inet.h>

#include "WebServerConfig.hpp"
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "Response.hpp"

extern char **environ;

LookupConfig::LookupConfig(void)
			:	CGIExt(""),
				env(NULL),
				currentWebServer(NULL),
			  	currentServer(NULL),			 
			  	currentLocation(NULL),			  
			  	currentCGI(NULL)

{
	int i = 0;
	while (environ[i])
		_envVars.push_back(environ[i++]);
}

LookupConfig::LookupConfig(const WebServerConfig* webServer)
			:	CGIExt(""),
			 	env(NULL),
				currentWebServer(webServer),
			  	currentServer(NULL),			  
			  	currentLocation(NULL),				
			 	currentCGI(NULL)
{
	int i = 0;
	while (environ[i])
		_envVars.push_back(environ[i++]);
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
	this->_envVars = rhs._envVars;
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
	if (!this->currentWebServer)
		throw std::logic_error("LookupConfig::updateCurrentServer: currentWebServer is NULL");
	this->currentServer = this->currentWebServer->getServerConfig(ipv4Address, port, host);
}

void LookupConfig::updateCurrentServer(const ServerConfig* server)
{
	this->currentServer = server;
}

void LookupConfig::updateCurrentLocation(const std::string& uri)
{
	if (!this->currentServer)
		throw std::logic_error("LookupConfig::updateCurrentLocation: currentServer is NULL");
	this->currentLocation = this->currentServer->getLocation(uri);
	
}

void	LookupConfig::updateUriWithLocationPath(const std::string& uri)
{
	if (!this->currentLocation)
		throw std::logic_error("LookupConfig::updateUriWithLocationPath: currentLocation is NULL");	
	_uriPath = uri;
	_uriPath.insert(0, this->currentLocation->rootDirectory);
}

void	LookupConfig::updateCurrentCGI()
{
	if (!this->currentLocation)
		throw std::logic_error("LookupConfig::updateCurrentCGI: currentLocation is NULL");
	this->currentCGI = this->currentLocation->cgiConfig;
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

const std::set<uint16_t> LookupConfig::getServerPorts(void) const
{
	if (!this->currentWebServer)
		throw std::logic_error("LookupConfig::getServerPorts: currentWebServer is NULL");
	return (this->currentWebServer->getServerPorts());
}

time_t	LookupConfig::getTimeout(void) const
{
	if (!this->currentWebServer)
		throw std::logic_error("LookupConfig::getServerPorts: currentWebServer is NULL");
	return (this->currentWebServer->timeout); //TODO: adjust data type in parsing
}
// void LookupConfig::updateCurrentCGI(???)

bool LookupConfig::isAutoindex() const
{
	return(currentLocation->directoryListing);
}

void LookupConfig::_updateEnvVarVector(const std::string authTypePrefix, const std::string newAuthType)
{
    for (std::vector<std::string>::iterator it = _envVars.begin(); it != _envVars.end(); ++it)
	{
        if (it->find(authTypePrefix) == 0) //если нашли
		{
            *it = authTypePrefix + newAuthType;
            return;
        }
    }
    _envVars.push_back(authTypePrefix + newAuthType);
	return;
}


void	LookupConfig::updateReqestHeaders(const std::map<std::string, std::vector<std::string> >& setOfHeaders)
{
	_Headers = setOfHeaders;
}

void 	LookupConfig::updateHttpVersion(std::string htttpVersion)// TODO: check 
{
	_httpVersion = htttpVersion;
}

std::string LookupConfig::_retrievePathInfo(const std::string& path)
{
	size_t dotPos = path.find('.');
	size_t addPath = path.find('/', dotPos);
	size_t queryPos = path.find('?', dotPos);
	std::string _pathInfo = "";
	if(queryPos != std::string::npos && addPath != std::string::npos)
		_pathInfo = path.substr(addPath, queryPos - addPath);
	else if(addPath != std::string::npos)
		_pathInfo = path.substr(addPath);
	return _pathInfo;
}


void LookupConfig::setEnvVars(int method, HttpResponse &response)
{
	_updateEnvVarVector("AUTH_TYPE=", "");
	if (method == GET_METHOD)
		_updateEnvVarVector("CONTENT_LENGTH=", "");	
	else if(method == POST_METHOD)
	{
		std::ostringstream oss;
    	oss << response.contentLength;
    	std::string contentLengthStr = oss.str();
		_updateEnvVarVector("CONTENT_LENGTH=", contentLengthStr);// TO DO - ADD LENGTH FOR POST 
	}
	_updateEnvVarVector("CONTENT_TYPE=", response.contentType);	
	_updateEnvVarVector("GATEWAY_INTERFACE=", "CGI/1.1");
	_updateEnvVarVector("PATH_INFO=", _retrievePathInfo(_uriPath));
	_updateEnvVarVector("PATH_TRANSLATED=", (currentLocation->rootDirectory + _cgiScriptPath));
	_updateEnvVarVector("QUERY_STRING=", _queryString);
	//_updateEnvVarVector("REMOTE_ADDR=", _ipAdress); TO DO - FIX IT
	_updateEnvVarVector("REMOTE_IDENT=", "");
	_updateEnvVarVector("REMOTE_USER=", "");
	if(method == GET_METHOD)
		_updateEnvVarVector("REQUEST_METHOD=", "GET");
	else if(method == POST_METHOD)
		_updateEnvVarVector("REQUEST_METHOD=", "POST");
	else if(method == DELETE_METHOD)
		_updateEnvVarVector("REQUEST_METHOD=", "DELETE");

	if(_queryString != "")
		_updateEnvVarVector("REQUEST_URI=", _uriPath + "?" + _queryString);
	else
		_updateEnvVarVector("REQUEST_URI=", _uriPath);
	_updateEnvVarVector("SCRIPT_NAME=", _cgiScriptPath);
	_updateEnvVarVector("SERVER_NAME=", response.getserverName());
	//_updateEnvVarVector("SERVER_PORT=",);TO DO - FIX IT
	_updateEnvVarVector("SERVER_PROTOCOL=", _httpVersion);
	_updateEnvVarVector("SERVER_SOFTWARE=", "Fancy_Webserv/42.0");

	/* Cast to char* string C-style for environment to run execve */
	env = new char*[_envVars.size() + 1];
	for (long unsigned int i = 0; i < _envVars.size(); i++)
	{
		env[i] = new char[_envVars[i].size() + 1];
		env[i] = (char*)(_envVars[i].c_str());
		env[i][_envVars[i].size()] = '\0';		
	}
	env[_envVars.size()] = NULL;
	/*----------------------*/


	/*------test print -----------------*/
	typedef std::map<std::string, std::vector<std::string> > HeadersMap;	
	std::cout << "=========_Headers:==========" << std::endl;
	for (HeadersMap::const_iterator it = _Headers.begin(); it != _Headers.end(); ++it) {
        std::cout << "===Header: " << it->first << ", Values: ";
        const std::vector<std::string>& values = it->second;
        for (std::vector<std::string>::const_iterator vecIt = values.begin(); vecIt != values.end(); ++vecIt) {
            std::cout << *vecIt << " ";
        }
        std::cout << std::endl;
    }
	std::cout << "===========================" << std::endl;
	/*----------------------*/

	/* Debug print : */
	// for (std::vector<std::string>::iterator it = _envVars.begin(); it != _envVars.end(); it++) {
	// 	std::cout << "header:" << *it << std::endl;
	// }
}





/* Geters and setters */

std::string	LookupConfig::getCgiScriptPath(void) const
{
	return (_cgiScriptPath);
}

std::string	LookupConfig::getUriPath(void) const
{
	return (_uriPath);
}

std::string	LookupConfig::getIpAdress(void) const
{
	return (_ipAdress);
}

void	LookupConfig::setCgiScriptPath(std::string path)
{
	_cgiScriptPath = path;
}

void	LookupConfig::setQueryString(std::string path)
{
	_queryString = path;
}

void	LookupConfig::setUriPath(std::string path)
{
	_uriPath = path;
}

void	LookupConfig::setIpAdress(std::string &ip)
{	
	std::cout << "111ip: " << ip << std::endl;
	_ipAdress = ip;
}

void LookupConfig::setSocketServer(SocketServer* server)
{
	socketServer = server;
}

