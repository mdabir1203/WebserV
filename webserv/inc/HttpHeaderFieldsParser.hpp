#ifndef HTTP_HEADER_FIELDS_PARSER_HPP
# define HTTP_HEADER_FIELDS_PARSER_HPP

#include "Server.hpp"

enum HTTPHeaderParserState
{
	START,
	HEADER_NAME,
	HEADER_VALUE,
	HEADER_DONE
};


class HttpHeaderParser {
public:
   HttpHeaderParser();
   void parse(char c);
   bool isComplete();
   std::string getHeaderName();
   std::string getHeaderValue();
   void		   reset();

private:
   int 		   state_;
   std::string m_header_name;
   std::string m_header_value;
};

#endif /* HTTP_HEADER_FIELDS_PARSER_HPP */