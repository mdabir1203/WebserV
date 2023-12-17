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
    std::cout << "ipAddress: " << "\t\t" << serverConfig.ipAddress << std::endl;
    std::cout << "port: " << serverConfig.port << std::endl;

    std::cout << "serverNames:" << "\t\t" << std::endl;
    for (auto it = serverConfig.serverNames.begin(); it != serverConfig.serverNames.end(); ++it) {
        std::cout << " " << *it << std::endl;
    }

    std::cout << "customErrorPages:" << "\t\t" << std::endl;
    for (auto it = serverConfig.customErrorPages.begin(); it != serverConfig.customErrorPages.end(); ++it) {
        std::cout << "  " << it->first << ": " << it->second << std::endl;
    }

    std::cout << "maxClientBodySize: " << "\t\t" << serverConfig.maxClientBodySize << std::endl;
    std::cout << "maxClientBodySizeSet: " << (serverConfig.maxClientBodySizeSet ? "true" : "false") << std::endl;

    // Print information about LocationConfig objects in the locations map
    for (auto it = serverConfig.locations.begin(); it != serverConfig.locations.end(); ++it) {
        std::cout << "LocationConfig Key (path): " << it->first << std::endl;
        std::cout << "LocationConfig Info:" << std::endl;
        printLocationConfig(*it->second);
    }
}

void ConfigParser::printWebServerConfig(const WebServerConfig& webServerConfig) {
    // Print WebServerConfig variables
	std::cout << YELLOW << "=========================== PRINT WebServerConfig class object ========================= " << RESET << std::endl;
    std::cout << "maxClientBodySize: " << "\"" << webServerConfig.maxClientBodySize << "\"" << std::endl;    
    std::cout << "maxClients: " << "\"" << webServerConfig.maxClients << "\"" << std::endl;
    std::cout << "timeout: " << "\"" << webServerConfig.timeout << "\"" << std::endl;
    std::cout << "defaultFolderFile: " << "\"" << webServerConfig.defaultFolderFile << "\"" << std::endl;

    // Print information about defaultErrorPages map
    for (auto it = webServerConfig.defaultErrorPages.begin(); it != webServerConfig.defaultErrorPages.end(); ++it) {
        std::cout << "defaultErrorPages: " << "\"" <<  it->first << "\"" <<  " "  << "\"" << it->second << "\"" << std::endl;
    }

	std::cout << YELLOW << "================================= PRINT Servers map: =================================== " << RESET << std::endl;
    // Print information from servers map
    int server_counter = 0;
    for (std::map<std::pair<uint32_t, uint16_t>, std::vector<ServerConfig*>>::const_iterator it = webServerConfig.servers.begin(); it != webServerConfig.servers.end(); ++it) {
        std::cout << "   ipAddress \t\t\t\""  <<  ipNumberToString(it->first.first) << "\"" << ", port: \"" << it->first.second << "\"" <<  std::endl;
        // Print information from ServerConfig
        for (std::vector<ServerConfig*>::const_iterator serverIt = it->second.begin(); serverIt != it->second.end(); ++serverIt) {
            std::cout << BLUE <<"=================================Server " << server_counter++ << "=================================\n" << RESET << std::endl;
            std::cout << "   ServerNames: ";
            for (std::set<std::string>::const_iterator nameIt = (*serverIt)->serverNames.begin(); nameIt != (*serverIt)->serverNames.end(); ++nameIt) {
                std::cout <<  "\t\t\"" << *nameIt << "\"" <<  " " ;
            }
            std::cout << std::endl;
            for(std::map<uint16_t, std::string>::const_iterator Itr = (*serverIt)->customErrorPages.begin(); Itr != (*serverIt)->customErrorPages.end(); ++Itr)
            {
                std::cout << "   customErrorPages: " << "\t\t\"" << Itr->first << "\"" <<  " "  << "\"" << Itr->second << "\"" << std::endl;
            }
            std::cout << "   maxClientBodySize: " << "\t\t\"" << (*serverIt)->maxClientBodySize << "\"" << std::endl;
            std::cout << "   maxClientBodySizeSet: " << "\t\"" << (*serverIt)->maxClientBodySizeSet << "\"" << std::endl;

            // Print information from locations map
            int location_counter = 0;
            for (std::map<std::string, LocationConfig*>::const_iterator locIt = (*serverIt)->locations.begin(); locIt != (*serverIt)->locations.end(); ++locIt) {
                        std::cout << PURPLE << "                      ========Location " << location_counter++ << "========                     \n" << RESET << std::endl;
                std::cout << "     Path: \t\t\t\"" << locIt->first << "\"" << std::endl;                
                std::cout << "     statusCode: \t\t\"" << locIt->second->statusCode << "\"" <<std::endl;
                std::cout << "     targetUrl: \t\t\"" << locIt->second->targetUrl << "\"" << std::endl;
                std::cout << "     rootDirectory: \t\t\"" << locIt->second->rootDirectory << "\"" << std::endl;   
                std::cout << "     uploadDirectory: \t\t\"" << locIt->second->uploadDirectory << "\"" << std::endl;
                std::cout << "      CGI Config:" << std::endl;
                for (std::set<std::string>::const_iterator Iter = locIt->second->cgiConfig->cgiExtensions.begin(); Iter != locIt->second->cgiConfig->cgiExtensions.end(); ++Iter) {
                    std::cout << "     cgiExtensions: \t\t\"" << *Iter << "\"" << std::endl;
                }
                // begin(); nameIt != (*serverIt)->serverNames.end(); ++nameIt) {
                // std::cout <<  "\t\t\"" << *nameIt << "\"" <<  " " ;
            
                for(std::vector<std::string>::const_iterator Itr = locIt->second->defaultFolderFiles.begin(); Itr != locIt->second->defaultFolderFiles.end(); ++Itr)
                {
                    std::cout << "     defaultFolderFiles: " << "\t\"" << *Itr << "\"" << std::endl;
                }
                std::cout << "     directoryListing: \t\t\"" << locIt->second->directoryListing << "\"" << std::endl;
                std::cout << "     _allowedMethods: \t\t\""  << locIt->second->getMethod(0) << locIt->second->getMethod(1) << locIt->second->getMethod(2) << "\"" <<  std::endl;

                // Print other information from LocationConfig as needed
            }
        }
    }
    std::cout  << "---------------------------------------------------------------------" << RESET << std::endl;    
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
		parser.printWebServerConfig(webserverconfig);
    }
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}	
	return 0;
}
