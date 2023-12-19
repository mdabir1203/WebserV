#include "ConfigParser.hpp"

ConfigParser::ConfigParser(WebServerConfig* webServerConfig)
			: webServerConfig(webServerConfig),
			  currentServerConfig(NULL),
			  currentLocationConfig(NULL),
			  currentState(CONFIG_PARSER_STATE_WS),
			  previousState(CONFIG_PARSER_STATE_WS),
			  key(""),
			  value(""),
			  lastChar('\0'),
			  isQuoteMode(false),
			  lineCount(1),
			  charCount(0),
			  paramterLength(0)
			
{
   mulitValues.reserve(CONFIG_PARSER_MAX_VALUE_LENGTH);
   key.reserve(CONFIG_PARSER_MAX_KEY_LENGTH);
   value.reserve(CONFIG_PARSER_MAX_VALUE_LENGTH);

   stateTransitionArray[CONFIG_PARSER_STATE_WS] = &ConfigParser::handleStateWs;
   stateTransitionArray[CONFIG_PARSER_STATE_COMMENT] = &ConfigParser::handleStateComment;
   stateTransitionArray[CONFIG_PARSER_STATE_KEY] = &ConfigParser::handleStateKey;
   stateTransitionArray[CONFIG_PARSER_STATE_OWS] = &ConfigParser::handleStateOws;
   stateTransitionArray[CONFIG_PARSER_STATE_VALUE] = &ConfigParser::handleStateValue;
   stateTransitionArray[CONFIG_PARSER_STATE_LOCATION] = &ConfigParser::handleStateLocation;

	httpKeys["client_max_body_size"]   = std::make_pair(0, &ConfigParser::handleClientMaxBodySize); 	// done
	httpKeys["error_page"]             = std::make_pair(0, &ConfigParser::handleDefaultErrorPage); 		// just hardcoded

	serverKeys["client_max_body_size"] = std::make_pair(0, &ConfigParser::handleClientMaxBodySize); 	// done
	serverKeys["error_page"]           = std::make_pair(0, &ConfigParser::handleErrorPage);				// done
	serverKeys["listen"]               = std::make_pair(0, &ConfigParser::handleListen);				// done
	serverKeys["server_name"]          = std::make_pair(0, &ConfigParser::handleServerName);			// done

	serverKeys["location"]             = std::make_pair(0, &ConfigParser::handleLocationPath);				// done
	locationKeys["root"]               = std::make_pair(0, &ConfigParser::handleRoot);					// done
	locationKeys["index"]              = std::make_pair(0, &ConfigParser::handleIndex);					// done
	locationKeys["cgi_extension"]      = std::make_pair(0, &ConfigParser::handleCgiExtension);			// done		
	locationKeys["upload_store"]       = std::make_pair(0, &ConfigParser::handleUploadStore);			// done	
	locationKeys["return"]             = std::make_pair(0, &ConfigParser::handleReturn);				// done	
	locationKeys["allow_methods"]      = std::make_pair(0, &ConfigParser::handleMethods);				// done		
	locationKeys["autoindex"]          = std::make_pair(0, &ConfigParser::handleAutoindex);				// done	
}

ConfigParser::~ConfigParser()
{
	delete currentServerConfig;
	delete currentLocationConfig;
}

void ConfigParser::throwConfigError(const std::string& message, char offendingChar, const std::string& offendingString, bool givePosition)
{
	std::stringstream ss;

	ss << "Error: {" << message << "}";
	if (givePosition)
		ss << " at line {" << lineCount << "}";
	if (offendingChar)
		ss << " at character {" << charCount << "}" << " with char {" << offendingChar << "}";
	if (!offendingString.empty())
		ss << " with string {" << offendingString << "}";
	throw std::runtime_error(ss.str());
}

void ConfigParser::parseConfig(const std::string& configPath)
{
	std::ifstream configFile(configPath.c_str()); //closes automatically when it goes out of scope
	std::string buffer;

	if (!configFile.is_open())
		throw std::runtime_error("Error: Could not open config file");
	buffer.reserve(CONFIG_PARSER_BUFFER_SIZE);
	while (configFile.good())
	{
		configFile.read(&buffer[0], CONFIG_PARSER_BUFFER_SIZE - 1);
		if (configFile.bad())
			break;
		std::streamsize bytesRead = configFile.gcount();
		buffer[bytesRead] = '\0';
		if (bytesRead)
		{
			for (std::streamsize i = 0; i < bytesRead; i++)
			{
				parseChar(buffer[i]);
				lastChar = buffer[i];
			}
		}
	}
	if (!configFile.eof())
		throw std::runtime_error("Error: Could not read config file");
	if (lastChar == '\\')
		throw std::runtime_error("Error: '\\' is last char in config file");
	configFile.close();
	//validate the WebServerConfig
	//check if all location and server blocks are closed meaning currentServerConfig and currentLocationConfig are NULL
}

void ConfigParser::parseChar(char c)
{
	if (c == '\n')
	{
		lineCount++;
		charCount = 0;
	}
	else
	{
		charCount++;
	}
	if (lastChar != '\\' && c == '\\')
		return;
	(this->*stateTransitionArray[currentState])(c);
	if (lastChar == '\\' && c == '\\')
		lastChar = '\0';
	//std::cout << PURPLE << "currentState: " << currentState << " c: " << c << RESET << std::endl;	/////////////////////////////////////////////////////
}

void ConfigParser::stateTransition(int state, int nextState)
{
	this->previousState = state;
	this->currentState = nextState;
}

/* ========================== Save Char ============================ */
void ConfigParser::addCharToKey(char c)
{
	if (paramterLength < CONFIG_PARSER_MAX_KEY_LENGTH)
	{
		paramterLength++;
		key.push_back(std::tolower(c));
	}
	else
	{
		throwConfigError("Key too long", 0, key, true);
	}
}

void ConfigParser::addCharToValue(char c)
{
	if (paramterLength < CONFIG_PARSER_MAX_VALUE_LENGTH)
	{
		paramterLength++;
		value.push_back(c);
	}
	else
	{
		throwConfigError("Value too long", 0, value, true);
	}
}

/* ========================== STATE HANDLERS ============================ */
void	ConfigParser::handleStateWs(char c) // after value, after Block starts and ends
{
	if (isCommentStart(c))
	{
		stateTransition(CONFIG_PARSER_STATE_WS, CONFIG_PARSER_STATE_COMMENT);
	}
	else if (isAllowedKeyChar(c))
	{
		addCharToKey(c);
		stateTransition(CONFIG_PARSER_STATE_WS, CONFIG_PARSER_STATE_KEY);
	}
	else if (isUnescapedChar('{', c)) //start of block
	{
		if (key == "server" && !currentServerConfig) //start of server
		{
			currentServerConfig = new ServerConfig();
		}
		else if (key == "location" && currentServerConfig && !currentLocationConfig) //start of location, after location path
		{
			currentLocationConfig = new LocationConfig();
			handleLocationPath(); //validation
		}
		else if ((key == "location" && (!currentServerConfig || currentLocationConfig)) //location outside of server or inside of location
				|| (key == "server" && currentServerConfig)) //server inside of server
		{
			throwConfigError("Unexpected opening of block", 0, key, true); //unexpected opening of block
		}
		else
		{
			throwConfigError("Unexpected", c, "", true); //unexpected {
		}
		key.clear();
		value.clear();
		paramterLength = 0;
	}
	else if (isUnescapedChar('}', c)) //end of block
	{
		if (currentLocationConfig) //end of location
		{
			validateLocationConfig(currentLocationConfig);
			currentServerConfig->addLocationConfig(currentLocationConfig);
			currentLocationConfig = NULL;
			resetKeyCounts(locationKeys);
		}
		else if (currentServerConfig) //end of server
		{
			validateServerConfig(currentServerConfig);
			webServerConfig->addServerConfig(currentServerConfig);
			currentServerConfig = NULL;
			resetKeyCounts(serverKeys);
		}
		else
		{
			throwConfigError("Unexpected", c, "", true); //unexpected closing of block
		}
	}
	else if (isAllowedWhiteSpace(c))
	{
		return;
	}
	else
	{
		throwConfigError("Unexpected", c, "", true);
	}
}

void ConfigParser::handleStateComment(char c)
{
	if (c == '\n')
		stateTransition(CONFIG_PARSER_STATE_COMMENT, this->previousState);
	else
		stateTransition(this->previousState, CONFIG_PARSER_STATE_COMMENT);
}

void ConfigParser::handleStateKey(char c) //TODO: check edge cases with server{ and server# and ...
{
	if (isCommentStart(c) && key != "server")
	{
		throwConfigError("Comment in key", c, key, true);
	}
	else if (isAllowedKeyChar(c))
	{
		addCharToKey(c);
	}
	else if (isAllowedOws(c) && key == "location" && currentServerConfig && !currentLocationConfig) //start of location path
	{
		paramterLength = 0;
		stateTransition(CONFIG_PARSER_STATE_KEY, CONFIG_PARSER_STATE_LOCATION);
	}
	else if (key == "server")
	{
		paramterLength = 0;
		stateTransition(CONFIG_PARSER_STATE_KEY, CONFIG_PARSER_STATE_WS);
		handleStateWs(c);
	}
	else if (isUnescapedChar(':', c))
	{
		//validate key is appropriate for current block -> move this check to when values are gathered
		stateTransition(CONFIG_PARSER_STATE_KEY, CONFIG_PARSER_STATE_OWS);
	}
	else
	{
		throwConfigError("Invalid character in key", c, key, true);
	}
}

void ConfigParser::handleStateLocation(char c)
{
	if (isUnescapedChar('{', c)) // start of location block after location path
	{
		stateTransition(CONFIG_PARSER_STATE_LOCATION, CONFIG_PARSER_STATE_WS);
		handleStateWs(c);
	}
	else if (toggleQuoteMode(c))
	{
		return;
	}
	else if (isCommentStart(c) && value.empty())
	{
		throwConfigError("Comment in location path", c, key, true);
	}
	else if (isCommentStart(c) && !value.empty())
	{
		stateTransition(CONFIG_PARSER_STATE_WS, CONFIG_PARSER_STATE_COMMENT);
	}
	else if (isAllowedValueChar(c))
	{
		addCharToValue(c);
	}
	else if (isAllowedOws(c) && value.empty())
	{
		return;
	}
	else if (!value.empty() && isAllowedWhiteSpace(c)) 
	{	
		stateTransition(CONFIG_PARSER_STATE_LOCATION, CONFIG_PARSER_STATE_WS);
	}
	else
	{
		throwConfigError("Invalid location path", c, key, true);
	}
}

void	ConfigParser::handleStateOws(char c)	// state 3
{
	if (isUnescapedChar(';', c))
	{	
		handleKeyValuePair();
	}
	else if (isCommentStart(c))
	{
		throwConfigError("Missing semicolon", c, "", true);
	}
	else if (isAllowedValueChar(c))
	{
		addCharToValue(c);
		stateTransition(CONFIG_PARSER_STATE_OWS, CONFIG_PARSER_STATE_VALUE);
	}
	else if (isAllowedOws(c))
	{
		return;
	}
	else if (isUnescapedChar('"', c))
	{
		isQuoteMode = true;
		stateTransition(CONFIG_PARSER_STATE_OWS, CONFIG_PARSER_STATE_VALUE);
	}
	else
	{
		throwConfigError("Missing semicolon", c, "", true);
	}
}

void	ConfigParser::handleStateValue(char c) // state 4
{
	if (isUnescapedChar(';', c))
	{
		handleKeyValuePair();
	}
	else if (toggleQuoteMode(c))
	{
		return;
	}
	else if (isAllowedValueChar(c))
	{
		addCharToValue(c);
	}
	else if (isAllowedOws(c))
	{
		if (!value.empty())
		{
			mulitValues.push_back(value);
			value.clear();
		}
		stateTransition(CONFIG_PARSER_STATE_VALUE, CONFIG_PARSER_STATE_OWS);
	}
	else
	{
		throwConfigError("Invalid character in value", c, value, true);
	}
}

void	ConfigParser::handleKeyValuePair(void)
{
	mulitValues.push_back(value);
	validateAndHandleKey();
	paramterLength = 0;
	key.clear();
	value.clear();
	mulitValues.clear();
	stateTransition(CONFIG_PARSER_STATE_OWS, CONFIG_PARSER_STATE_WS);
}

void	ConfigParser::validateAndHandleKey(void)
{
	if (!currentServerConfig && !currentLocationConfig)
	{
		validateKeyAndCallHandler(httpKeys);
	}
	else if (currentServerConfig && !currentLocationConfig)
	{
		validateKeyAndCallHandler(serverKeys);
	}
	else if (currentServerConfig && currentLocationConfig)
	{
		validateKeyAndCallHandler(locationKeys);
	}
	else
	{
		throwConfigError("Invalid key", 0, key, true);
	}
}

void	ConfigParser::validateKeyAndCallHandler(std::map<std::string, std::pair<int, HandlerFunction> >& keys)
{
	std::map<std::string, std::pair<int, ConfigParser::HandlerFunction> >::iterator it = keys.find(key);
	if (it == keys.end())
	{
		throwConfigError("Invalid key", 0, key, true);
	}
	else if (it->second.first++ != 0)
	{
		throwConfigError("Duplicate key", 0, key, true);
	}
	else
	{
		ConfigParser::HandlerFunction functionPointer = it->second.second;
		(this->*functionPointer)();
	}
}

void	ConfigParser::resetKeyCounts(std::map<std::string, std::pair<int, HandlerFunction> >& keys)
{
	std::map<std::string, std::pair<int, HandlerFunction> >::iterator it = keys.begin();
	for( ; it != keys.end(); ++it)
	{
		it->second.first = 0;
	}
}

void    ConfigParser::validateLocationConfig(LocationConfig* currentLocationConfig)
{
	/* Check location path */
	// std::cout << BLUE << "validateLocationConfig" << currentLocationConfig->rootDirectory << RESET << std::endl;
	if(currentLocationConfig->rootDirectory == "")
		throwConfigError("Location path not set", 0, "", true);
		
	// std::set<std::string>::iterator temp = currentLocationConfig->cgiConfig->cgiExtensions.begin();
	// std::cout << "AAAAAAAAAAA" << *temp << std::endl;
	// if((*temp) == "")
	// {	currentLocationConfig->cgiConfig->cgiExtensions.erase(temp);
	// 	currentLocationConfig->cgiConfig->cgiExtensions.insert(".py");
	// 	currentLocationConfig->cgiConfig->cgiExtensions.insert(".sh");
	// }
}

void    ConfigParser::validateServerConfig(ServerConfig* currentServerConfig)
{
	if (currentServerConfig->ipAddress == 0 || currentServerConfig->port == 0) {        
       throw std::runtime_error("Error: Server has no valid Ip adress or port\n");
    }
	if (currentServerConfig->serverNames.empty()) {
        std::cout << "Set is empty. Adding 'localhost'." << std::endl;
        currentServerConfig->serverNames.insert("localhost");
	}
}

WebServerConfig* ConfigParser::getWebServerConfig(void) const
{
	return (this->webServerConfig);
}
