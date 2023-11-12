#ifndef REQUEST_PARSER_NEW_HPP
# define REQUEST_PARSER_NEW_HPP

# include "Server.hpp"

enum ParseState
{
	REQUEST_LINE,
	// METHOD,
	// REQUEST_URI,
	// HTTP_VERSION,
	HEADER_FIELDS,
	// HEADER_NAME,
	// HEADER_VALUE,
	REQUEST_BODY,
	ERROR, // Bad request
	SUCCESS,
	DONE
};

enum HttpMethod
{
	GET,
	POST,
	DELETE,
	UNKNOWN
};

enum ErrorCodes
{
	OK,
	BAD_REQUEST,
	NOT_IMPLEMENTED
};

class RequestParserNew
{
	public:
		RequestParserNew(void);
		RequestParserNew(const RequestParserNew &src);
		~RequestParserNew(void);
		RequestParserNew &operator=(const RequestParserNew &rhs);

		void parse(const std::string& request); // needs to be looped

		struct HttpRequestData
		{
			int			method;
			std::string requestUri;
			std::map<std::string, std::string> headerFields;
			std::string requestBody;
    	};
		HttpRequestData storedHttpRequest;

	private:
		int						_currentStateMachine;
		int						_errorCode;
		std::string::size_type	_currentPositionInInput;
		std::string				_parameter;
		std::string				_value;

		int	_parseRequestLine(const std::string& input);
		int _setMethod(const std::string& input);
		int _parseHeaderFields(const std::string& input);
		int	_setHeaderPair(const std::string& name, const std::string& value);
		int	_parseRequestBody(const std::string& input);
};

#endif /* REQUEST_PARSER_NEW_HPP */
