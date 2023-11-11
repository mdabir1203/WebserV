#pragma once

#include <iostream>
#include <string>
#include "Server.hpp"

enum ParseState {
    METHOD,
    REQUEST_URI,
    HTTP_VERSION,
    HEADER_FIELD,
    HEADER_VALUE,
    REQUEST_BODY,
    DONE
};




class HttpRequestParser {
public:

	void parseToken(ParseState state, const std::string& input, size_t& pos, std::string& currentToken);
    void parse(const std::string& request);

private:

	// member variables for storing parsed HTTP request
	struct HttpRequestData {
        HttpMethod method;
        std::string requestUri;
        std::string httpVersion;
        std::map<std::string, std::string> headers;
        std::string requestBody;
		
    };

	HttpRequestData storedHttpRequest;


};