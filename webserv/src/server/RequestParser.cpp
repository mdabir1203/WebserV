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
		if (this->_parseHeaderFields(input) == ERROR)
		{
			this->_errorCode = BAD_REQUEST;
			std::cout << "Bad request" << std::endl; 
			return ;
		}
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

// test my HTTPHeader Parser with request.

int RequestParserNew::_parseHeaderFields(const std::string& input)
{
	HttpHeaderParser test;

	std::string::const_iterator it = input.begin();

	std::advance(it, 19);
	for (; it != input.end(); ++it)
	{
		std::cout << "char: " << *it << std::endl;
		test.parse(*it);
		std::cout << test.isComplete() << std::endl;
		if (test.isComplete())
		{
			std::cout << "Header name: " << test.getHeaderName() << std::endl;
			std::cout << "Header value: " << test.getHeaderValue() << std::endl;
			test.reset();
		}
	}
	
	
	
	// what error checking is needed?
		//define or search for a rule set for the header fields
		//mandatory fields
		//useful fields
		//ignore the rest
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




HttpHeaderParser::HttpHeaderParser() : state_(START) {}



void HttpHeaderParser::parse(char c) {
	std::cout << "state: " << state_ << std::endl;
  if (state_ ==  START && c == ':') {
      state_ = HEADER_NAME;
  } else if (state_ == HEADER_NAME && (c == '\r' || c == '\n')) {
      if (c == '\r' && !m_header_value.empty() && m_header_value[m_header_value.length() - 1] == '\n') {
          // Folded header
          m_header_value.erase(m_header_value.length() - 1);
          state_ = HEADER_VALUE;
      } else {
          state_ = HEADER_DONE;
      }
  } else if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
      if (state_ == START || state_ == DONE) {
          m_header_name += c;
      } else if (state_ == HEADER_VALUE) {
          if (c == '"' && !m_header_value.empty() && m_header_value[m_header_value.length() - 1]!= '\\') {
              // Start or end of a quoted string
              if (m_header_value[m_header_value.length() - 1] == '"') {
                 m_header_value.erase(m_header_value.length() - 1);
              } else {
                 m_header_value += c;
              }
          } else {
              // Handle special characters
              if (c == '\\' && !m_header_value.empty() && m_header_value[m_header_value.length() - 1]== '\\') {
                 m_header_value.erase(m_header_value.length() - 1);
              }
              m_header_value += c;
          }
      }
  }
}





bool HttpHeaderParser::isComplete() {
   // Implement logic to check if parsing is complete
   return state_ == HEADER_DONE;
}

std::string HttpHeaderParser::getHeaderName() {
   // Implement logic to get header name
   return m_header_name;
}

std::string HttpHeaderParser::getHeaderValue() {
   // Implement logic to get header value
   return m_header_value;
}

void HttpHeaderParser::reset() {
   // Implement logic to reset the parser
   state_ = START;
   m_header_name.clear();
   m_header_value.clear();
}