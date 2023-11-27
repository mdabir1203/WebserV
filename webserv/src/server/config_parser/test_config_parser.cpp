#include "ConfigParser.hpp"
							
// std::vector<t_serv> parseConfig(const std::string& filename);

/////////////////////////////////////////////////////////////// DEBUG  PRINTER ////////////////////////////////////////////////////////////////////////////
	#include <iostream>

void printLocationConfig(const LocationConfig& locationConfig) {
    std::cout << "path: " << locationConfig.path << std::endl;
    std::cout << "statusCode: " << locationConfig.statusCode << std::endl;
    std::cout << "targetUrl: " << locationConfig.targetUrl << std::endl;
    std::cout << "rootDirectory: " << locationConfig.rootDirectory << std::endl;
    std::cout << "uploadDirectory: " << locationConfig.uploadDirectory << std::endl;

    std::cout << "defaultFolderFiles:" << std::endl;
    for (size_t i = 0; i < locationConfig.defaultFolderFiles.size(); ++i) {
        std::cout << "  " << locationConfig.defaultFolderFiles[i] << std::endl;
    }

    std::cout << "directoryListing: " << (locationConfig.directoryListing ? "true" : "false") << std::endl;
}

void printServerConfig(const ServerConfig& serverConfig) {
    std::cout << "ipAddress: " << serverConfig.ipAddress << std::endl;
    std::cout << "port: " << serverConfig.port << std::endl;

    std::cout << "serverNames:" << std::endl;
    for (auto it = serverConfig.serverNames.begin(); it != serverConfig.serverNames.end(); ++it) {
        std::cout << "  " << *it << std::endl;
    }

    std::cout << "customErrorPages:" << std::endl;
    for (auto it = serverConfig.customErrorPages.begin(); it != serverConfig.customErrorPages.end(); ++it) {
        std::cout << "  " << it->first << ": " << it->second << std::endl;
    }

    std::cout << "maxClientBodySize: " << serverConfig.maxClientBodySize << std::endl;
    std::cout << "maxClientBodySizeSet: " << (serverConfig.maxClientBodySizeSet ? "true" : "false") << std::endl;

    // Print information about LocationConfig objects in the locations map
    for (auto it = serverConfig.locations.begin(); it != serverConfig.locations.end(); ++it) {
        std::cout << "LocationConfig Key (path): " << it->first << std::endl;
        std::cout << "LocationConfig Info:" << std::endl;
        printLocationConfig(*it->second);
    }
}

void printWebServerConfig(const WebServerConfig& webServerConfig) {
    // Print WebServerConfig variables
	std::cout << YELLOW << "=========================== PRINT WEBSERV CONFIG ========================= " << RESET << std::endl;
    std::cout << "maxClientBodySize: " << webServerConfig.maxClientBodySize << std::endl;
    std::cout << "defaultFolderFile: " << webServerConfig.defaultFolderFile << std::endl;
    std::cout << "maxClients: " << webServerConfig.maxClients << std::endl;
    std::cout << "timeout: " << webServerConfig.timeout << std::endl;

    // Print information about ServerConfig objects in the servers map
    for (auto it = webServerConfig.servers.begin(); it != webServerConfig.servers.end(); ++it) {
        std::cout << "ServerConfig Key (ipAddress:port): " << it->first.first << ":" << it->first.second << std::endl;
        std::cout << "ServerConfig Info:" << std::endl;
        for (size_t i = 0; i < it->second.size(); ++i) {
            printServerConfig(*it->second[i]);
        }
    }

    // Print information about defaultErrorPages map
    for (auto it = webServerConfig.defaultErrorPages.begin(); it != webServerConfig.defaultErrorPages.end(); ++it) {
        std::cout << "Error Page Key (statusCode): " << it->first << std::endl;
        std::cout << "Error Page Value: " << it->second << std::endl;
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int ac, char** av)
{	
	std::string			configPath;
	WebServerConfig webserverconfig;
	ConfigParser parser(&webserverconfig);
	if (ac > 2)
		throw std::runtime_error("Error: wrong number of arguments");
	if (ac == 2)
		configPath = av[1];
	else
		configPath = "src/config_files/new_default.conf";
	std::cout << "configPath: " << configPath << std::endl;
	try
	{	
		parser.parseConfig(configPath);
        // std::vector<t_serv> parsedConfig = parser.getServers();
		printWebServerConfig(webserverconfig);
    }
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}	
	return 0;
}
