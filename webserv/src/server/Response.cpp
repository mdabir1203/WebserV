#include "Response.hpp"

HttpResponse::HttpResponse()
            : statusCode(0), contentType(""), content("")
{
}

HttpResponse::HttpResponse(int statusCode, const std::string& contentType, const std::string& content)
			: statusCode(statusCode), contentType(contentType), content(content)
{
}

//TODO: put server field like -> server: webserv/42.0
int	HttpResponse::WriteToBuffer(char* buffer, size_t bufferSize) const
{
    std::ostringstream oss;
    oss << "HTTP/1.1 " << statusCode << "\r\n"
        << "Content-Type: " << contentType << "\r\n"
        << "Content-Length: " << content.size() << "\r\n\r\n"
        << content;
    std::string responseStr = oss.str();
    if (responseStr.size() >= bufferSize) {
      throw std::runtime_error("Buffer is too small");
    }
	responseStr.copy(buffer, responseStr.size() + 1);
	buffer[responseStr.size()] = '\0';
    return responseStr.size();
}
