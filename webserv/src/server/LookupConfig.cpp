#include "LookupConfig.hpp"

#include <iostream>
#include <stdexcept>

#include "WebServerConfig.hpp"
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"
#include "Response.hpp"

extern char **environ;

LookupConfig::LookupConfig(void)
			:  CGIExt(""),
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
			: CGIExt(""),
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

const std::string & LookupConfig::getUriPath() const
{
	return (_uriPath);
}


void LookupConfig::_updateEnvVarVector(const std::string authTypePrefix, const std::string newAuthType)
{
    // Поиск элемента, начинающегося с "AUTH_TYPE="
    for (std::vector<std::string>::iterator it = _envVars.begin(); it != _envVars.end(); ++it)
	{
        if (it->find(authTypePrefix) == 0)
		{
            *it = authTypePrefix + newAuthType;
            return;
        }
    }
    _envVars.push_back(authTypePrefix + "\"\"");
	return;
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



#include <sstream>
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
	_updateEnvVarVector("PATH_TRANSLATED=", _retrievePathInfo(_uriPath));

	// _envVars.push_back("PATH_TRANSLATED=" + _scriptPath);
	// _envVars.push_back("QUERY_STRING=" + _queryString);
	// // _envVars.push_back("REMOTE_ADDR=" + _clientSocket.getIP());
	// _envVars.push_back("REMOTE_IDENT=");
	// _envVars.push_back("REMOTE_USER=");
	// _envVars.push_back("REQUEST_METHOD=" + _header["Method"]);
	// _envVars.push_back("REQUEST_URI=" + _header["Path"]);
	// _envVars.push_back("SCRIPT_NAME=" + _scriptPath);
	// _envVars.push_back("SERVER_NAME=" + _serverConfig.serverName);
	// _envVars.push_back("SERVER_PORT=" + toString(_serverConfig.port));
	// _envVars.push_back("SERVER_PROTOCOL=" + _header["HTTP-Version"]);
	// _envVars.push_back("SERVER_SOFTWARE=webserv/1.0");
	/* Test: */
	for (std::vector<std::string>::iterator it = _envVars.begin(); it != _envVars.end(); it++) {
		std::cout << "header:" << *it << std::endl;
	}
}