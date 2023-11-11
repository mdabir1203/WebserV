#pragma once

#include <iostream>
#include <string>
#include "Server.hpp"

enum ParseState {
    METHOD,
    REQUEST_URI,
    HTTP_VERSION,
    // HEADER_FIELD,
    // HEADER_VALUE,
    // REQUEST_BODY,
    ERROR,
    DONE
};

enum HttpMethod {
    GET,
    POST,
    DELETE,
    UNKNOWN
};

class HttpRequestParser {
public:

    void parseToken(int& state, const std::string& input);
    void parse(const std::string& request);

private:

	// member variables for storing parsed HTTP request
	struct HttpRequestData {
        int         method;
        std::string requestUri;
        std::string httpVersion;
        std::map<std::string, std::string> headers;
        std::string requestBody;
		
    };

    std::string getToken(const std::string& request, size_t& pos);
    int parseMethod(const std::string& methodStr);
    int nextState(int currentState);

	HttpRequestData storedHttpRequest;


};