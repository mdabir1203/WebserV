#ifndef HTTP_RESPONSE_HPP
# define HTTP_RESPONSE_HPP

#include <string>
#include <sstream>

class HTTPResponse
{
    public:
        HTTPResponse(const std::string& request);
        std::string generateResponse();

    private:
        bool checkResourceExists(const std::string& requestedResource);
        std::string requestedResource;
        std::string responseBodySuccess = "<html><body><h1>!!WeSucceeded!!</h1></body></html>";
        std::strin responseBodyFail = "<html><body><h1>!!WeCannotFindYou!!</h1><img src=\"fun_picture.jpg\"></body></html>";
}

#endif