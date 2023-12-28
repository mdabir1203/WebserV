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
	if (mulitValues.size() > 1)
	{
		throwConfigError("Multiple values for key", 0, key, true);
	}
	else if (isRequired && (mulitValues.empty() || mulitValues[0].empty()))
	{
		throwConfigError("No value for key", 0, key, true);
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
	// std::cout << GREEN <<  "maxClientBodySize:" << webServerConfig->maxClientBodySize << RESET << std::endl;
}



void	ConfigParser::handleListen()
{
	std::string& numberString = extractSingleValueFromValueVector(true);
	//std::cout << "numberString: " << numberString << std::endl;
	std::istringstream iss(numberString);
    // size_t result;

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
	//std::cout << GREEN << "ipAddress " << currentServerConfig->ipAddress << RESET << std::endl;	
	//  std::cout << "currentServerConfig->port " << currentServerConfig->port << std::endl;
	// std::cout << GREEN << "ipAddress back " << ipNumberToString(currentServerConfig->ipAddress) << RESET << std::endl;
	// std::cout << GREEN << "port back " << uint16_to_ip_port(currentServerConfig->port)<< RESET << std::endl;
}


void	ConfigParser::handleServerName()
{
    if (mulitValues.empty()) {
        	
			mulitValues.push_back("localhost");
    }
    // std::string& serverName = mulitValues[0];	
	for (std::vector<std::string>::iterator it = mulitValues.begin(); it != mulitValues.end(); ++it) {
		const std::string& serverName = *it;
		if (serverName.empty())	{
			currentServerConfig->serverNames.insert("localhost");
			break;
		} else {
			currentServerConfig->serverNames.insert(serverName);
		}
	}
	// /* Print: */
	// for (std::set<std::string>::iterator it = currentServerConfig->serverNames.begin(); it != currentServerConfig->serverNames.end(); ++it) {
    //     std::cout << GREEN << "serverName " << *it << RESET << std::endl;
	// }
}

void	ConfigParser::handleErrorPage()
{
	// std::cout << YELLOW << "handleErrorPage:" << "\"" << mulitValues[0] <<"," << mulitValues [1] << "\"" << RESET << std::endl;
	// std::cout << YELLOW << "previousState" << "\"" << previousState << RESET << std::endl;
	if (mulitValues.size() == 0 || previousState == 1)
	{
       for (std::map<uint16_t, std::string>::iterator it = webServerConfig->defaultErrorPages.begin(); it != webServerConfig->defaultErrorPages.end(); ++it) //TODO: make it proper for map with find
	   { 
	 	    currentServerConfig->customErrorPages[it->first] = it->second;
    	}
	}
	else if (mulitValues.size() > 2) {
       throwConfigError("There must be 2 values for key: ", 0, key, true);
    }
	else if (( (mulitValues.size() == 2) && (mulitValues[0].empty() || mulitValues[1].empty()) ) || (mulitValues.size() == 1) ) {
	   throwConfigError("Empty value for key: ", 0, key, true);
	}
	else
	{
		uint16_t numb = stringToUint16(mulitValues[0]);
		currentServerConfig->customErrorPages[numb] = mulitValues[1];
	}	
	// /* Print: */
	// for (std::map<uint16_t, std::string>::iterator it = currentServerConfig->customErrorPages.begin(); it != currentServerConfig->customErrorPages.end(); ++it) {
	// 	std::cout << GREEN << "Custom Error Page " << it->first << " " << it->second << std::endl;
	// }
}

void	ConfigParser::handleDefaultErrorPage()
{	
	//TO DO: some script that assigns a page? 
}

void    ConfigParser::handleRoot()
{
	std::string& str = extractSingleValueFromValueVector(true);
	if (str.at(str.size() - 1) != '/')
		str.push_back('/');
	currentLocationConfig->rootDirectory = str;
	// /* Print: */
	// std::cout << GREEN <<  "(Loc)root:" << currentLocationConfig->rootDirectory << RESET << std::endl;
}

void    ConfigParser::handleLocationPath()
{
	std::string& locationPath = extractSingleValueFromValueVector(true);

	if (!currentServerConfig->isLocationPathUnique(locationPath))
		throwConfigError("Duplicate location path", 0, locationPath, true);
	currentLocationConfig->path = locationPath;
}

void    ConfigParser::handleIndex()
{
	std::string& locationIndex = extractSingleValueFromValueVector(true);

	currentLocationConfig->defaultFolderFiles.push_back(locationIndex);
    // /* Print: */
	// std::cout << GREEN <<  "(Loc)Index: " << currentLocationConfig->defaultFolderFiles[0] << RESET << std::endl;
	
}

void    ConfigParser::handleCgiExtension()
{

	// if(mulitValues[0]!= ".py" && mulitValues[0] != ".sh"){
	// 	std::cout << PURPLE << "JOPA CGI: " <<  mulitValues[0] <<  RESET << std::endl;
	// 	return;
	// 	// currentLocationConfig->cgiConfig->cgiExtensions.erase(".py");
	// 	// currentLocationConfig->cgiConfig->cgiExtensions.erase(".sh");}
	// }
	// std::cout << PURPLE << "!!!!!=======!!!!!!!CGI PROB: " <<mulitValues[0] << mulitValues[1] << std::endl;
	// std::cout << PURPLE << "!!!!!=======!!!!!!!CGI PROB val: " <<value << std::endl;
	if(mulitValues[0] == ".py" || mulitValues[0] == ".sh")
	{
		for (std::vector<std::string>::iterator ir = mulitValues.begin(); ir != mulitValues.end(); ++ir) 
		{
			// std::cout << PURPLE << "CGI ir: " << *ir << RESET << std::endl;
			if(*ir == ".sh" || *ir == ".py")
			{
				const std::string& CgiExt = *ir;
				currentLocationConfig->cgiConfig->cgiExtensions.insert(CgiExt);
			}		
		}
	}
	// /* Print: */
	// for (std::set<std::string>::iterator it = currentLocationConfig->cgiConfig->cgiExtensions.begin(); it != currentLocationConfig->cgiConfig->cgiExtensions.end(); ++it) {
    //     std::cout << GREEN << "(Loc)CgiExtension " << *it << RESET << std::endl;
	// }
	// //exit(1);
}

void    ConfigParser::handleUploadStore()
{
	std::string& locationUpload = extractSingleValueFromValueVector(false);
	if(locationUpload.empty())
	{
		//TO DO: ???
	}
	else		
		currentLocationConfig->uploadDirectory = locationUpload;
    // /* Print: */
	// std::cout << GREEN << "(Loc)Upload store: " << currentLocationConfig->uploadDirectory << RESET << std::endl;
	
}

void	ConfigParser::handleReturn() //TODO: throw error if return has no value?
{
	if (mulitValues.size() == 0 || previousState == 1)
	{
		currentLocationConfig->statusCode = 0;
		currentLocationConfig->targetUrl = "";
	}
	else if (mulitValues.size() > 2) {
       throwConfigError("There must be 2 values for key: ", 0, key, true);
    }
	else if (( (mulitValues.size() == 2) && (mulitValues[0].empty() || mulitValues[1].empty()) ) || (mulitValues.size() == 1) ) {
	   throwConfigError("Empty value for key: ", 0, key, true);
	}
	else
	{
		currentLocationConfig->statusCode = stringToUint16(mulitValues[0]);
		currentLocationConfig->targetUrl = mulitValues[1];
	}	
	// /* Print: */	
	// std::cout << GREEN << "(Loc)return " << currentLocationConfig->statusCode  << currentLocationConfig->targetUrl << std::endl;
	
}

void    ConfigParser::handleMethods()
{
	currentLocationConfig->allowedMethods.reset();
	if ((mulitValues[0].empty()))
		return;
	for (std::vector<std::string>::iterator it = mulitValues.begin(); it != mulitValues.end(); ++it)
	{
		if(*it == "GET")
			currentLocationConfig->allowedMethods.set(GET);
		else if(*it == "POST")
			currentLocationConfig->allowedMethods.set(POST);
		else if(*it == "DELETE")
			currentLocationConfig->allowedMethods.set(DELETE);
		else
			throwConfigError("Only GET, POST and DELETE are allowed", 0, *it, true);
	}
}

void    ConfigParser::handleAutoindex()
{
	const std::string& autoIndex = extractSingleValueFromValueVector(true);
	if (autoIndex.empty())
		return;
	else if (autoIndex == "on")
		currentLocationConfig->directoryListing = true;
	else if (autoIndex == "off")
		currentLocationConfig->directoryListing = false;
	else
		throwConfigError("autoindex must be on or off", 0, autoIndex, true);
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

uint16_t ConfigParser::ip_port_to_uint16(const std::string& ip_port) {
   std::size_t pos = ip_port.find(':');
   if (pos == std::string::npos) {
       throw std::invalid_argument("Error: Invalid IP:port format");
   }

   std::string port_str = ip_port.substr(pos + 1);
	if(port_str.empty())
		throw std::invalid_argument("Error: Invalid IP:port format");
   uint16_t port = static_cast<uint16_t>(std::atoi(port_str.c_str())); //check for correct error handling
	
   return port;
}

// std::string ConfigParser::uint16_to_ip_port(uint16_t port) {
//    std::stringstream ss;
//    ss  << std::to_string(port);

//    return ss.str();
// }


uint16_t ConfigParser::stringToUint16(const std::string& str) {
    std::stringstream ss(str);
    uint16_t result = 0;

    if (!(ss >> result)) {
        std::cerr << "Error: Unable to convert string to uint16_t." << std::endl;
        return 0;
    }
    return result;
}
