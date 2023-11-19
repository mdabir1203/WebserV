#include "RequestParser.hpp"

bool isHeaderNameChar(char c) //tchar
{
	static const std::string validChars = "!#$%&'*+-.^_`|~";
    return (std::isalnum(c) || validChars.find(c) != std::string::npos);
}

bool isOWS(char c) //optional whitespace
{
	return (c == ' ' || c == '\t');
}

bool isHeaderValueChar(char c) //vchar and OWS
{
	return ((c >= 33 && c <= 126) || isOWS(c));
}

void trimTrailingOWS(std::string& str)
{
	size_t pos = str.find_last_not_of(" \t");
	if (pos != std::string::npos)
	{
		str.erase(pos + 1);
	}
	else
	{
		str.clear();
	}
}

// effective string comparison
bool isHeaderFieldAllowed(const std::string& input, int headerMethod)
{
   size_t hash = 0;
   if (input.empty())
      return (false);
   hash = input.length() + input[0];
   if (hash < 103 || hash > 133)
	  return (false);
   switch (hash)
   {
      case 111:
        if (headerMethod == POST && input == "content-type")
            return (true);
		break ;
      case 113:
        if (headerMethod == POST && input == "content-length")
            return (true);
		break ;
      case 108:
        if (input == "host")
            return (true);
		break ;
      case 103:
        if (headerMethod == GET && input == "accept")
            return (true);
		break ;
      case 110:
        if (input == "authorization")
            return (true);
		break ;
      case 105:
        if (input == "cookie")
            return (true);
		break ;
      case 133:
        if (headerMethod == POST && input == "transfer-encoding")
            return (true);
		break ;
      case 112:
        if (input == "cache-control")
            return (true);
		break ;
      case 109:
        if (input == "connection")
            return (true);
		break ;
      case 107:
        if (headerMethod != GET && input == "expect")
            return (true);
		break ;
      case 127:
        if (input == "user-agent")
            return (true);
		break ;
    }
    return (false);
}

/*-----------------------header request line parsing----------------------*/

bool	HeaderFieldStateMachine::setMethod(const std::string& input)
{
	if (input == "GET")
		headerMethod = GET;
	else if (input == "POST")
		headerMethod = POST;
	else if (input == "DELETE")
		headerMethod = DELETE;
	else
		return (false);
	return (true);
}

void	HeaderFieldStateMachine::handleStateHeaderMethod(char c)
{
	if (c == ' ' && !headerValue.empty() && setMethod(headerValue))
	{
		paramterLength = 0;
		headerValue.clear();
		stateTransition(HEADER_METHOD, HEADER_URI);
	}
	else if (c == ' ' && headerValue.empty())
	{
		throw std::runtime_error("Empty header method");
	}
	else if (paramterLength < HEADER_METHOD_MAX_LENGTH)
	{
		paramterLength++;
		headerValue.push_back(c);
	}
	else if (paramterLength > HEADER_METHOD_MAX_LENGTH)
	{
		throw std::length_error("Header method is too long");
	}
	else
	{
		throw std::runtime_error("Header Method not supported");
	}
}

void	HeaderFieldStateMachine::handleStateHeaderUri(char c)
{
	if (c == '\r' && lastChar != '\r')
	{
		// Do nothing, expect line feed next
	}
	else if (c == '\n' && lastChar == '\r')
	{
		throw std::runtime_error("Wrong HTTP Version");
	}
	if (c == ' ' && !headerUri.empty())
	{
		paramterLength = 0;
		stateTransition(HEADER_URI, HEADER_HTTP_VERSION);
	}
	else if (c == ' ' && headerUri.empty())
	{
		throw std::runtime_error("Empty header uri");
	}
	else if (paramterLength < HEADER_URI_MAX_LENGTH)
	{
		paramterLength++;
		headerUri.push_back(c);
	}
	else if (paramterLength > HEADER_URI_MAX_LENGTH)
	{
		throw std::length_error("Header uri is too long");
	}
	else
	{
		throw std::runtime_error("Invalid character in header uri");
	}
}

bool	isHttpVersionCorrect(const std::string& httpVersion)
{
	return (httpVersion == HEADER_HTTP_VERSION_STRING);
}

void	HeaderFieldStateMachine::handleStateHeaderHttpVersion(char c)
{
	if (c == '\r' && lastChar != '\r')
	{
		// Do nothing, expect line feed next
	}
	else if (c == '\n' && lastChar == '\r' && isHttpVersionCorrect(headerValue))
	{
		// End of line, transition states
		isHttpVersionRight = true;
		paramterLength = 0;
		headerValue.clear();
		stateTransition(HEADER_HTTP_VERSION, HEADER_NAME);
	}
	else if (c == '\n' && lastChar == '\r')
	{
		throw std::runtime_error("Wrong header http version");
	}
	else if (paramterLength < HEADER_HTTP_VERSION_MAX_LENGTH)
	{
		paramterLength++;
		headerValue.push_back(c);
	}
	else
	{
		throw std::runtime_error("Wrong header http version");
	}
}

/*---------------------header fields parsing----------------*/

void	HeaderFieldStateMachine::handleStateHeaderName(char c)
{
	if (c == '\r' && lastChar != '\r' && headerName.empty())
	{
		// Do nothing, expect line feed next
	}
	else if (c == '\n' && lastChar == '\r' && headerName.empty())
	{
		// End of line, transition states
		stateTransition(HEADER_NAME, HEADER_BODY);
	}
	else if (c == ':' && !headerName.empty())
	{
		stateTransition(HEADER_NAME, HEADER_OWS);
	}
	else if (c == ':' && !headerName.empty())
	{
		throw std::runtime_error("Empty header name");
	}
	else if (isHeaderNameChar(c) && paramterLength < maxHeaderLength)
	{
		paramterLength++;
		headerName.push_back(std::tolower(c));
	}
	else if (paramterLength > maxHeaderLength)
	{
		throw std::length_error("Header field is too long");
	}
	else
	{
		throw std::runtime_error("Invalid character in header name");
	}
}

void	HeaderFieldStateMachine::handleStateHeaderOWS(char c)
{
	if (isOWS(c))
	{
		// Skip leading whitespace in header value
	}
	else if (!isOWS(c) && isHeaderValueChar(c) && paramterLength < maxHeaderLength) //TODO: define a max len is it for all or jsut one? Currently it is for one
	{
		paramterLength++;
		headerValue.push_back(c);  // Start of header value
		stateTransition(HEADER_OWS, HEADER_VALUE);  // Transition to value state
	}
	else if (paramterLength >= maxHeaderLength)
	{
		throw std::length_error("Header field is too long");
	}
	else if (c == '\r' && lastChar != '\r')
	{
		// Do nothing, expect line feed next
	}
	else if (c == '\n' && lastChar == '\r' && !headerValue.empty())
	{
		// End of line, transition states
		stateTransition(HEADER_OWS, HEADER_PAIR_DONE);
	}
	else if (c == '\n' && lastChar == '\r' && headerValue.empty())
	{
		throw std::runtime_error("Empty header value");
	}
	else
	{
		throw std::runtime_error("Invalid character in header value");
	}
}

void	HeaderFieldStateMachine::handleStateHeaderValue(char c)
{
	if (c == '\r' && lastChar != '\r')
	{
		// Do nothing, expect line feed next
	}
	else if (c == '\n' && lastChar == '\r' && !headerValue.empty())
	{
		// End of line, transition states
		storeHeaderPair();
		headerName.clear();
		headerValue.clear();
		paramterLength = 0;
		stateTransition(HEADER_VALUE, HEADER_NAME);
	}
	else if (c == '\n' && lastChar == '\r' && headerValue.empty())
	{
		throw std::runtime_error("Empty header value");
	}
	else if (isHeaderValueChar(c) && paramterLength < maxHeaderLength)
	{
		paramterLength++;
		headerValue.push_back(c);
	}
	else if (paramterLength > maxHeaderLength)
	{
		throw std::length_error("Header field is too long");
	}
	else
	{
		throw std::runtime_error("Invalid character in header value");
	}
}

bool	isOnlyOnceAllowed(const std::string& headerName)
{
	return (headerName == "host" || headerName == "authorization" || headerName == "user-agent");
}

void	HeaderFieldStateMachine::storeHeaderPair(void)
{
	trimTrailingOWS(headerValue);
	if (headerValue.empty() && headerName != "host")
		throw std::runtime_error("Empty header value");
	// std::cout << "headerName: " << headerName << std::endl;
	// std::cout << "headerValue: " << headerValue << std::endl;
	// std::cout << "headerMethod: " << headerMethod << std::endl;
	if (!isHeaderFieldAllowed(headerName, headerMethod))
		return ; // not needed for the current method
	if (isOnlyOnceAllowed(headerName) && headers.find(headerName) != headers.end())
		throw std::runtime_error("Duplicate header field");
	headers[headerName].push_back(headerValue); 
}

void	HeaderFieldStateMachine::handleStateHeaderPairDone(char c)
{
	if (c == '\r' && lastChar != '\r')
	{
		// Do nothing, expect line feed next
	}
	else if (c == '\n' && lastChar == '\r')
	{
		// End of line, transition states
		stateTransition(HEADER_PAIR_DONE, HEADER_BODY);
	}
	else
	{
		throw std::runtime_error("Invalid character after header pair");
	}
}

/*---------------------header Body parsing----------------*/

void	HeaderFieldStateMachine::handleStateHeaderBody(char c)
{
	(void)c;
	stateTransition(HEADER_BODY, HEADER_END);
}
