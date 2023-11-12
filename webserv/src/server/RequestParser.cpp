#include "RequestParser.hpp"

void skipSpaces(std::istringstream& iss);

RequestParserNew::RequestParserNew(void) : _currentStateMachine(REQUEST_LINE), _errorCode(OK), _currentPositionInInput(0), _parameter(""), _value("")
{

}

RequestParserNew::RequestParserNew(const RequestParserNew &src)
{
	*this = src;
}

RequestParserNew::~RequestParserNew(void)
{

}

RequestParserNew &RequestParserNew::operator=(const RequestParserNew &rhs)
{
	if (this == &rhs)
		return (*this);
	//copy data
	return (*this);
}

void	RequestParserNew::parse(const std::string& input)
{
	if (this->_currentStateMachine == REQUEST_LINE)
	{
		if (this->_parseRequestLine(input) == ERROR)
		{
			this->_errorCode = BAD_REQUEST;
			std::cout << "Bad request" << std::endl; 
			return ;
		}
		// parse request line
		this->_currentStateMachine = HEADER_FIELDS;
	}
	else if (this->_currentStateMachine == HEADER_FIELDS)
	{
		// parse header fields
		this->_currentStateMachine = REQUEST_BODY;
	}
	else if (this->_currentStateMachine == REQUEST_BODY)
	{
		// parse request body
		this->_currentStateMachine = DONE;
	}
}

//how to continue if the request is chunked? -> smaller groups of processing with individual states
//where to set limits for the sizes? -> URI size
//What extra checking is required for the URI -> charcaters with special meaning like '?'
int	RequestParserNew::_parseRequestLine(const std::string& input) // reduce code duplication
{
    std::istringstream iss(input);

	// Read method
	if (!std::getline(iss, this->_parameter, ' ')) // case sensitive!
		return (ERROR); // Internal server error or bad request?
	if (this->_setMethod(this->_parameter) == ERROR)
		return (ERROR);
	std::cout << "Method: " << this->storedHttpRequest.method << std::endl;
	skipSpaces(iss);

    // Read URI
	if (!std::getline(iss, this->storedHttpRequest.requestUri, ' ')) // not case sensitive?
		return (ERROR); // Internal server error or bad request?
	std::cout << "Request URI: " << this->storedHttpRequest.requestUri << std::endl;
	skipSpaces(iss);

    // Read HTTP version
	if (!std::getline(iss, this->_parameter, '\r')) // case sensitive?
		return (ERROR); // Internal server error or bad request?
	if (this->_parameter != "HTTP/1.1") // is this case sensitive?
		return (ERROR);

    // Check for CRLF or EOF
	if (!iss.eof() && iss.get() != '\n')
		return (ERROR);
	return (SUCCESS);
}

void skipSpaces(std::istringstream& iss)
{
    while (iss.peek() == ' ')
        iss.get();
}

int	RequestParserNew::_setMethod(const std::string& input)
{
	if (input == "GET")
		this->storedHttpRequest.method = GET;
	else if (input == "POST")
		this->storedHttpRequest.method = POST;
	else if (input == "DELETE")
		this->storedHttpRequest.method = DELETE;
	else
	{
		this->_errorCode = NOT_IMPLEMENTED;
		return (ERROR);
	}
	return (SUCCESS);
}

//limits for the sizes? -> name and value
int RequestParserNew::_parseHeaderFields(const std::string& input)
{
	// what error checking is needed?
		//define or search for a rule set for the header fields
		//mandatory fields
		//useful fields
		//ignore the rest
	(void)input;
	return (SUCCESS);
}

int	RequestParserNew::_setHeaderPair(const std::string& name, const std::string& value)
{
	// store it in a map
	// storedHttpRequest.headerFields[name] = value;
	(void)name;
	(void)value;
	return (SUCCESS);
}

//when do we need it?
//where do we store it?
int	RequestParserNew::_parseRequestBody(const std::string& input)
{
	// what error checking is needed?
	(void)input;
	return (SUCCESS);
}
