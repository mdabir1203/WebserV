#include "Response.hpp"

HttpResponse::HttpResponse(int statusCode, const std::string& contentType, const std::string& content)
			: statusCode(statusCode), contentType(contentType), content(content)
{
	
}

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
    std::strcpy(buffer, responseStr.c_str());
    return responseStr.size();
}
