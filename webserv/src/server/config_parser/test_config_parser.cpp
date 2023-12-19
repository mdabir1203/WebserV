#include "ConfigParser.hpp"
#include "LookupConfig.hpp"

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
		configPath = "src/config_files/default.conf";
	std::cout << "configPath: " << configPath << std::endl;
	try
	{	
		std::string uri = "/up/nonsense";
		parser.parseConfig(configPath);
        parser.getWebServerConfig()->printConfig(true);
		std::cout << std::setfill('-') << std::setw(80) << "-" << std::endl;
        LookupConfig configuration(parser.getWebServerConfig());
        configuration.updateCurrentServer(2130771969, 80, "nonsense");
		configuration.updateCurrentLocation(uri);
        configuration.getCurrentServer()->printConfig(false);
		if (!configuration.getCurrentLocation())
			std::cout << RED << "location is NULL" << RESET << std::endl;
		else
			configuration.getCurrentLocation()->printConfig(false);
		configuration.updateUriWithLocationPath(uri);
		std::cout << "\nupdateUriWithLocationPath: " << uri << std::endl; 
    }
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}	
	return 0;
}
