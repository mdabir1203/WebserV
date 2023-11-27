#include "ConfigParser.hpp"

		//validate key is appropriate for current block -> move this check to when values are gathered
			//!currentServerConfig && !currentLocationConfig
				//client_max_body_size
				//error_page
			//currentServerConfig && !currentLocationConfig
				//listen
				//server_name
				//error_page
				//client_max_body_size
				//location
			//currentServerConfig && currentLocationConfig
				//root
				//index
				//cgi_extension
				//upload_store
				//return
				//allow_methods
				//autoindex


bool	ConfigParser::isLocationBlockValid()
{
	//check uniquness of location path earlier
	if (!currentLocationConfig->rootDirectory.empty())
		return (true);
	return (false); //throw error no root directory
}

std::string&	ConfigParser::extractSingleValueFromValueVector(const bool isRequired)
{
	std::string& value = mulitValues[0];
	std::cout << "value: " << value << std::endl;
	if (mulitValues.size() > 1)
	{
		throwConfigError("Multiple values for key: ", 0, key, true);
	}
	else if (isRequired && (mulitValues.empty() || mulitValues[0].empty()))
	{
		throwConfigError("No value for key: ", 0, key, true);
	}
	return (mulitValues[0]);
}

void	ConfigParser::handleClientMaxBodySize()
{
	std::string& numberString = extractSingleValueFromValueVector(true);
	std::istringstream iss(numberString);
    size_t result;

	if (!std::isdigit(numberString[0]))
	{
		throwConfigError("Only digits", 0, numberString, true);
	}
	else if (!(iss >> result) || result > 10000000000) // max size 10GB
	{
		throwConfigError("Value out of range 0 - 10 GB", 0, numberString, true);
    }
	else if (iss.eof())
	{
		if (currentServerConfig)
			currentServerConfig->setClientMaxBodySize(result);
		else
			webServerConfig->setClientMaxBodySize(result);
    }
	else
	{
		throwConfigError("Only digits", 0, numberString, true);
	}
}



void	ConfigParser::handleListen()
{
	std::string& numberString = extractSingleValueFromValueVector(true);
	//std::cout << "numberString: " << numberString << std::endl;
	std::istringstream iss(numberString);
    size_t result;

	for (std::size_t i = 0; i < numberString.length(); ++i)
	{
        char currentChar = numberString[i];
        if (!isdigit(currentChar) && currentChar != '.' && currentChar != ':')
		{
            throwConfigError("Only digits", 0, numberString, true);
        }
	}
	if (currentServerConfig)
	{
		currentServerConfig->ipAddress = ipStringToNumber(numberString);
		currentServerConfig->port = ip_port_to_uint16(numberString);
	}
	//  std::cout << "currentServerConfig->ipAddress " << currentServerConfig->ipAddress << std::endl;	
	//  std::cout << "currentServerConfig->port " << currentServerConfig->port << std::endl;
	//  std::cout << "currentServerConfig->ipAddress back " << ipNumberToString(currentServerConfig->ipAddress) << std::endl;
	//  std::cout << "currentServerConfig->port back " << uint16_to_ip_port(currentServerConfig->port) << std::endl;
}


void	ConfigParser::handleServerName()
{
	std::string& numberString = extractSingleValueFromValueVector(true);
	std::cout << "!!handleServerName: " << numberString << std::endl;
	// std::istringstream iss(numberString);
    // size_t result;

	// for (std::size_t i = 0; i < numberString.length(); ++i)
	// {
    //     char currentChar = numberString[i];
    //     if (!isdigit(currentChar) && currentChar != '.' && currentChar != ':')
	// 	{
    //         throwConfigError("Only digits", 0, numberString, true);
    //     }
	// }
	// if (currentServerConfig)
	// {
	// 	currentServerConfig->ipAddress = ipStringToNumber(numberString);
	// 	currentServerConfig->port = ip_port_to_uint16(numberString);
	// }
	//  std::cout << "currentServerConfig->ipAddress " << currentServerConfig->ipAddress << std::endl;	
	//  std::cout << "currentServerConfig->port " << currentServerConfig->port << std::endl;
	//  std::cout << "currentServerConfig->ipAddress back " << ipNumberToString(currentServerConfig->ipAddress) << std::endl;
	//  std::cout << "currentServerConfig->port back " << uint16_to_ip_port(currentServerConfig->port) << std::endl;
}







//////////////////////// converters Ip adress and port to UintXX_t and back ///////////////////////////

uint32_t ConfigParser::ipStringToNumber(const std::string& ip) {
   std::istringstream iss(ip);
   uint32_t ipv4 = 0;
   for(uint32_t i = 0; i < 4; ++i) {
       uint32_t part;
       iss >> part;
       if (iss.fail() || part > 255) {
           throw std::runtime_error("Invalid IP address - Expected [0, 255]");
       }
       ipv4 |= part << (8 * (3 - i));
       if (i != 3) {
           char delimiter;
           iss >> delimiter;
           if (iss.fail() || delimiter != '.') {
               throw std::runtime_error("Invalid IP address - Expected '.' delimiter");
           }
       }
   }
   return ipv4;
}

std::string ConfigParser::ipNumberToString(uint32_t ip) {
   std::ostringstream oss;
   oss << ((ip >> 24) & 0xFF) << "."
       << ((ip >> 16) & 0xFF) << "."
       << ((ip >> 8) & 0xFF) << "."
       << (ip & 0xFF);
   return oss.str();
}

uint16_t ConfigParser::ip_port_to_uint16(const std::string& ip_port) {
   std::size_t pos = ip_port.find(':');
   if (pos == std::string::npos) {
       throw std::invalid_argument("Invalid IP:port format");
   }

   std::string port_str = ip_port.substr(pos + 1);
   uint16_t port = static_cast<uint16_t>(std::stoi(port_str));

   return port;
}

std::string ConfigParser::uint16_to_ip_port(uint16_t port) {
   std::stringstream ss;
   ss  << std::to_string(port);

   return ss.str();
}
