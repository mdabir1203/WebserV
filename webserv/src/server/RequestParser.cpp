#include "RequestParser.hpp"









void HttpRequestParser::parse(const std::string& request)
{
    int state = METHOD;
    size_t pos = 0;
    std::string currentToken;

    // state not changing after first iteration
    while (state != DONE && state != ERROR)
    {
        currentToken = getToken(request, pos);
        parseToken(state, currentToken);
        state = nextState(state);
    }
}

// Implement the main parse function
std::string HttpRequestParser::getToken(const std::string& request, size_t& pos)
{
    std::string currentToken;

    // Extract the next token
    while (pos < request.size() && !std::isspace(request[pos]))
        currentToken += request[pos++];
    // Skip the trailing whitespace
    while (pos < request.size() && std::isspace(request[pos]))
        ++pos;
    //transform to lower case
    return currentToken;
}

int HttpRequestParser::parseMethod(const std::string& methodStr)
{
    if (methodStr == "get")
        return GET;
    else if (methodStr == "post")
        return POST;
    else if (methodStr == "delete")
        return DELETE;
    else
        return UNKNOWN;
}





void HttpRequestParser::parseToken(int& state, const std::string& input)
{
    switch (state) {
        case METHOD:
            // currentToken = getToken(input, pos);
            storedHttpRequest.method = parseMethod(input);
            // Process the parsed HTTP method
            std::cout << "HTTP Method: " << input << std::endl;
            std::cout << "HTTP Method: " << storedHttpRequest.method << std::endl;
            // Store the HTTP method (you can define a member variable for storage)
            break;

        case REQUEST_URI:
            // Process the parsed request URI
            // input = getToken(input, pos);
            std::cout << "Request URI: " << input << std::endl;
            // Store the request URI
            storedHttpRequest.requestUri = input;
            break;

        case HTTP_VERSION:
            // input = getToken(input, pos);
            if (input != "HTTP/1.1")
            {
                std::cout << "Wrong HTTP Version: " << input << std::endl;
                state = ERROR;
            }
            else
                std::cout << "HTTP Version: " << input << std::endl;
            break;

        // case ParseState::HEADER_FIELD:
        //     // Process the parsed header field
        //     std::cout << "Header Field: " << input << std::endl;
        //     // Store the current header field
        //     currentHeaderField = input;
        //     break;

        // case ParseState::HEADER_VALUE:
        //     // Process the parsed header value
        //     std::cout << "Header Value: " << input << std::endl;
        //     // Store the header field and value (you can define a member variable for storage)
        //     storedHttpRequest.headers[currentHeaderField] = input;
        //     break;

        // case ParseState::REQUEST_BODY:
        //     // Process the parsed request body
        //     std::cout << "Request Body: " << input << std::endl;
        //     // Store the request body
        //     storedHttpRequest.requestBody = input;
        //     break;

        case DONE:
            std::cout << "Done parsing" << std::endl;
            // Parsing is complete
            break;
    }
    // Clear the token for the next state
}


//

int HttpRequestParser::nextState(int currentState)
{
        // Logic to determine the next state based on the current state
        if (currentState == METHOD)
            return REQUEST_URI;
        else if (currentState == REQUEST_URI)
            return HTTP_VERSION;
        else if (currentState == ERROR)
            return ERROR;
        else
            return DONE;
        
        // (e.g., transition to REQUEST_URI after parsing METHOD)
        // ...

        // For simplicity, just return DONE for now
}

