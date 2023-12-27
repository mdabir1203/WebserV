#include "RequestParser.hpp"

// Function to decode percent-encoded characters in a URI
char decodePercentEncodedChar(const std::string &uri, size_t &index)
{
	// Extracting the next two characters as a hexadecimal string
	std::string hex = uri.substr(index + 1, 2);

	// Convert the hexadecimal string to an integer, and cast it to a char
	char decodedChar = static_cast<char>(strtol(hex.c_str(), NULL, 16));

	// Advancing the index by 2, since we've processed two additional characters
	index += 2;

	return decodedChar;
}

static bool isValidSchemeChar(char c)
{
	return isalnum(c) || c == '+' || c == '-' || c == '.';
}

static bool isValidPathChar(char c)
{
	return isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~' || c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' || c == '*' || c == '+' || c == ',' || c == ';' || c == '=' || c == ':' || c == '@' || c == '/';
}

void	HeaderFieldStateMachine::parseURI(void)
{
	std::cout << "Input URI parser: " << headerUri << std::endl;
	while (uriIndex < headerUri.length())
	{
		char c = headerUri[uriIndex];
		// Handle percent-encoded characters
		if (c == '%')
			c = decodePercentEncodedChar(headerUri, uriIndex);
		(this->*uriStateTransitionArray[currentUriState])(c);
		// Advance to the next character
		uriIndex++;
	}
	if (uriParts.path.empty())
		throwUriParseError("the provided uri path is empty");
}

void	HeaderFieldStateMachine::throwUriParseError(const std::string message)
{
	throw std::runtime_error("Error: current state -> " + std::to_string(currentUriState) + " " + message);
}

void 	HeaderFieldStateMachine::uriStateTransition(int state, int nextState)
{
	(void)state;
	currentUriState = nextState;
}

void	HeaderFieldStateMachine::handleStateUriStart(char c)
{
	if (c == '/')
	{
		uriStateTransition(URI_START, URI_PATH);
		(this->*uriStateTransitionArray[URI_PATH])(c);
	}
	else if (c == 'h')
	{
		uriStateTransition(URI_START, URI_SCHEME);
		(this->*uriStateTransitionArray[URI_SCHEME])(c);
	}
	else
		throwUriParseError("Invalid URI start");
}

void	HeaderFieldStateMachine::handleStateUriScheme(char c)
{
	if (c == ':')
	{
		if (!uriParts.scheme.empty() && headerUri.compare(uriIndex, 3, "://") == 0)
		{
			uriIndex += 2; // Skip '://'
			uriStateTransition(URI_SCHEME, URI_AUTHORITY);
		}
		else
		{
			throwUriParseError("Invalid scheme format");
		}
	}
	else if (isValidSchemeChar(c))
	{
		uriParts.scheme += c;
	}
	else
	{
		throwUriParseError("Invalid character in scheme");
	}
}

void	HeaderFieldStateMachine::handleStateUriAuthority(char c)
{
	if (c == '/')
	{
		uriStateTransition(URI_AUTHORITY, URI_PATH);
		(this->*uriStateTransitionArray[URI_PATH])(c);
	}
	else if (isValidPathChar(c)) //TODO: check if this is right
	{
		uriParts.authority += c;
	}
	else
	{
		throwUriParseError("Invalid character in authority");
	}
}

void	HeaderFieldStateMachine::handleStateUriPath(char c)
{
	if (c == '?')
		uriStateTransition(URI_PATH, URI_QUERY);
	else if (c == '#')
		uriStateTransition(URI_PATH, URI_FRAGMENT);
	else if (isValidPathChar(c))
		uriParts.path += c;
	else
		throwUriParseError("Invalid character in path");
}

void	HeaderFieldStateMachine::handleStateUriQuery(char c)
{
	if (c == '#')
		uriStateTransition(URI_PATH, URI_FRAGMENT);
	else if (isValidPathChar(c))
		uriParts.query += c;
	else
		throwUriParseError("Invalid character in query");
}

void	HeaderFieldStateMachine::handleStateUriFragment(char c)
{
	if (isValidPathChar(c))
		uriParts.fragment += c;
	else
		throwUriParseError("Invalid character in fragment");
}
