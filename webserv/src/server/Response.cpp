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

void HttpResponse::setCurrentDate(std::string& date) //TODO: is error checking needed?
{
	std::time_t	currentTime = std::time(NULL);
	std::tm		*localTime = std::localtime(&currentTime);

	date.resize(80);
	size_t len = std::strftime(&date[0], 80, "%a, %d %b %Y %H:%M:%S GMT", localTime);
	date.resize(len); // to remove the trailing '\0'
}

std::string HttpResponse::convertNumberToString(const long int number)
{
	std::stringstream ss;
	ss << number;
	return (ss.str());
}

void HttpResponse::sendBasicHeaderResponse(const int clientSocket) //TODO:: Implement MSG_DONTWAIT flag
{
	std::string header;

	setCurrentDate(date);

	header.reserve(1024);
	// header += "HTTP/1.1 " + response.statusCode + " " + response.statusMessage + "\r\n";
	header += "HTTP/1.1 ";
    header +=  convertNumberToString(statusCode) + " OK\r\n"; //TODO: implement reason phrase
	header += "Date: " + date + "\r\n";
	// header += "Server: " + response.server + "\r\n";
	header += "Content-Length: " + convertNumberToString(contentLength) + "\r\n";
	header += "Content-Type: text/html; charset=utf-8\r\n";
	// header += "content-type: text/plain\r\n";
	// header += "Content-Type: " + response.contentType + "\r\n";
	// header += "Last-Modified: " + response.lastModified + "\r\n";
	// header += "Connection: " + response.connection + "\r\n";
	// header += "Cache-Control: " + response.cacheControl + "\r\n";
	header += "\r\n";

	if (send(clientSocket, header.c_str(), header.size(), 0) == -1)
	{
		throw std::runtime_error("Error sending basic header response" + std::string(strerror(errno))); //TODO: forbidden to check errno?
	}
}

void HttpResponse::setStatusCode(const int statusCode)
{
    this->statusCode = statusCode;
}