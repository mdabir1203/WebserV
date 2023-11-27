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
	std::cout << "!!!numberString: " << numberString << std::endl;
	std::istringstream iss(numberString);
    size_t result;

	// for (std::size_t i = 0; i < numberString.length(); ++i)
	// {
    //     char currentChar = numberString[i];
    //     if (!isdigit(currentChar) && currentChar != '.' && currentChar != ':')
	// 	{
    //         throwConfigError("Only digits", 0, numberString, true);
    //     }
	// }

	if (!std::isdigit(numberString[0]))
	{

		throwConfigError("Only digits", 0, numberString, true);
	}
	else if (!(iss >> result) || result > 10000000000) // max size 10GB
	{
		throwConfigError("Value out of range 0 - 10 GB", 0, numberString, true);
    }
	else {
        // Проверка успешности операции чтения
        char nextChar;
        if (iss >> nextChar) {
            throwConfigError("Unexpected character found", 0, numberString, true);
        }

        if (currentServerConfig)
            currentServerConfig->setClientMaxBodySize(result);
        else
            webServerConfig->setClientMaxBodySize(result);
    }
	// else if (iss.eof())
	// {
	// 	if (currentServerConfig)
	// 		currentServerConfig->setClientMaxBodySize(result);
	// 	else
	// 		webServerConfig->setClientMaxBodySize(result);
    // }
	// else
	// {
	// 	throwConfigError("!!Only digits", 0, numberString, true);
	// }
}
