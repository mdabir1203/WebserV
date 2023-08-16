#include <iostream>
#include <string>
#include <sstream>


//content length is incorrect ->   cause issues with the client's interpretation of the response
// -> result in truncated or incomplete responses.
// -> / used to escape double quotation marks in the string literal
//  "<img src=\"fun_picture.jpg\">" ->  <img src="fun_picture.jpg">


HTTPResponse::HTTPResponse(const std::string& request)
{
    requestedResource = parseRequest(request);
    std::string responseBodySuccess = "<html><body><h1>!!WeSucceeded!!</h1></body></html>";
    std::string responseBodyFail = "<html><body><h1>!!WeCannotFindYou!!</h1><img src=\"fun_picture.jpg\"></body></html>"; //change here later
}

std::string HTTPResponse::generateResponse()
{
    bool resourceExists = checkResourceExists(requestedResource);
    std::ostringstream responseStream;

    int contentLengthS = responseBodySuccess.length();
    int contentLengthF = responseBodyFail.length();

    if (resourceExists)
    {
        responseStream << "HTTP/1.1 200 OK\r\n";
        responseStream << "Content-Type: text/html\r\n";
        responseStream << "Content-Length: " << contentLengthS << "\r\n";
        responseStream << "Cache-Control: no-cache\r\n";
        responseStream << "Access-Control-Allow-Origin: *\r\n";
        responseStream << "Strict-Transport-Security: max-age=31536000; includeSubDomains\r\n"; // duration of 1 year
        responseStream << "X-Content-Type-Options: nosniff\r\n";
        responseStream << "X-Frame-Options: DENY\r\n";
        responseStream << "X-XSS-Protection: 1; mode=block\r\n";
        responseStream << "\r\n";
        responseStream << responseBodySuccess;
    }
    else
    {
        responseStream << "HTTP/1.1 404 Not Found\r\n";
        responseStream << "Content-Type: text/html\r\n";
        responseStream << "Content-Length: " << contentLengthF << "\r\n";
        responseStream << "Cache-Control: no-cache\r\n";
        responseStream << "Access-Control-Allow-Origin: *\r\n";
        responseStream << "\r\n";
        responseStream << responseBodyFail;
    }

    std::string response = responseStream.str();

    return response;
}

bool HTTPResponse::checkResourceExists(const std::string& requestedResource)
{
    // Replace with the implementation of choice
    return true;
}
