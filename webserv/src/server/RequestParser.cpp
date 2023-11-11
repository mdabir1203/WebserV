#include "RequestParser.hpp"









void HttpRequestParser::parse(const std::string& request) {
    ParseState state = ParseState::METHOD;
    size_t pos = 0;
    std::string currentToken;

    while (state != ParseState::DONE) {
        parseToken(state, request, pos, currentToken);
        state = nextState(state);
    }
}





void HttpRequestParser::parseToken(ParseState state, const std::string& input, size_t& pos, std::string& currentToken) {
    switch (state) {
        case ParseState::METHOD:
            // Process the parsed HTTP method
            HttpMethod method = parseMethod(currentToken);
            std::cout << "HTTP Method: " << currentToken << std::endl;
            // Store the HTTP method (you can define a member variable for storage)
            storedHttpRequest.method = method;
            break;

        case ParseState::REQUEST_URI:
            // Process the parsed request URI
            std::cout << "Request URI: " << currentToken << std::endl;
            // Store the request URI
            storedHttpRequest.requestUri = currentToken;
            break;

        case ParseState::HTTP_VERSION:
            // Process the parsed HTTP version
            std::cout << "HTTP Version: " << currentToken << std::endl;
            // Store the HTTP version
            storedHttpRequest.httpVersion = currentToken;
            break;

        case ParseState::HEADER_FIELD:
            // Process the parsed header field
            std::cout << "Header Field: " << currentToken << std::endl;
            // Store the current header field
            currentHeaderField = currentToken;
            break;

        case ParseState::HEADER_VALUE:
            // Process the parsed header value
            std::cout << "Header Value: " << currentToken << std::endl;
            // Store the header field and value (you can define a member variable for storage)
            storedHttpRequest.headers[currentHeaderField] = currentToken;
            break;

        case ParseState::REQUEST_BODY:
            // Process the parsed request body
            std::cout << "Request Body: " << currentToken << std::endl;
            // Store the request body
            storedHttpRequest.requestBody = currentToken;
            break;

        case ParseState::DONE:
            // Parsing is complete
            break;
    }

    // Clear the token for the next state
    currentToken.clear();
}




	ParseState HttpRequestParser::nextState(ParseState currentState) {
        // Logic to determine the next state based on the current state
        // (e.g., transition to REQUEST_URI after parsing METHOD)
        // ...

        // For simplicity, just return DONE for now
        return ParseState::DONE;
    }