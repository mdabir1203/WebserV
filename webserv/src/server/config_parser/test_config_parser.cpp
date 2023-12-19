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
		parser.parseConfig(configPath);
        parser.getWebServerConfig()->printConfig(true);
        // LookupConfig configuration(parser.getWebServerConfig());
        // configuration.updateCurrentServer(2130771969, 80, "nonsense");
        // configuration.getCurrentServer()->printConfig(false);
    }
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}	
	return 0;
}
