#include "Response.hpp"
#include <iostream>
#include <sstream>
#include <ctime>
#include <stdexcept>
#include <cstring>
#include <sys/socket.h>
#include <errno.h>

#include "RequestParser.hpp"

HttpResponse::HttpResponse()
            :  contentType(""), statusCode(0), content(""), lastModifiedTime(""), serverName("")
{
}

HttpResponse::HttpResponse(const std::string& contentType, int statusCode,  const std::string& content, const std::string& lastModifiedTime, const std::string& serverName)
			: contentType(contentType), statusCode(statusCode),  content(content), lastModifiedTime(lastModifiedTime), serverName(serverName)
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

static std::string getReasonPhrase1xx(const int statusCode)
{
    switch (statusCode)
    {
        case 100:
            return (" Continue");
        case 101:
            return (" Switching Protocols");
        case 102:
            return (" Processing");
        case 103:
            return (" Early Hints");
        default:
            return ("");
    }
}

static std::string getReasonPhrase2xx(const int statusCode)
{
    switch (statusCode)
    {
        case 200:
            return (" OK");
        case 201:
            return (" Created");
        case 202:
            return (" Accepted");
        case 203:
            return (" Non-Authoritative Information");
        case 204:
            return (" No Content");
        case 205:
            return (" Reset Content");
        case 206:
            return (" Partial Content");
        case 207:
            return (" Multi-Status");
        case 208:
            return (" Already Reported");
        case 226:
            return (" IM Used");
        default:
            return ("");
    }
}

static std::string getReasonPhrase3xx(const int statusCode)
{
    switch (statusCode)
    {
        case 300:
            return (" Multiple Choices");
        case 301:
            return (" Moved Permanently");
        case 302:
            return (" Found");
        case 303:
            return (" See Other");
        case 304:
            return (" Not Modified");
        case 305:
            return (" Use Proxy (deprecated)");
        case 306:
            return (" Switch Proxy (no longer used)");
        case 307:
            return (" Temporary Redirect");
        case 308:
            return (" Permanent Redirect");
        default:
            return ("");
    }
}

static std::string getReasonPhrase4xx(const int statusCode)
{
    switch (statusCode)
    {
        case 400:
            return (" Bad Request");
        case 401:
            return (" Unauthorized");
        case 402:
            return (" Payment Required");
        case 403:
            return (" Forbidden");
        case 404:
            return (" Not Found");
        case 405:
            return (" Method Not Allowed");
        case 406:
            return (" Not Acceptable");
        case 407:
            return (" Proxy Authentication Required");
        case 408:
            return (" Request Timeout");
        case 409:
            return (" Conflict");
        case 410:
            return (" Gone");
        case 411:
            return (" Length Required");
        case 412:
            return (" Precondition Failed");
        case 413:
            return (" Payload Too Large");
        case 414:
            return (" URI Too Long");
        case 415:
            return (" Unsupported Media Type");
        case 416:
            return (" Range Not Satisfiable");
        case 417:
            return (" Expectation Failed");
        case 418:
            return (" I'm a teapot");
        case 421:
            return (" Misdirected Request");
        case 422:
            return (" Unprocessable Entity");
        case 423:
            return (" Locked");
        case 424:
            return (" Failed Dependency");
        case 425:
            return (" Too Early");
        case 426:
            return (" Upgrade Required");
        case 428:
            return (" Precondition Required");
        case 429:
            return (" Too Many Requests");
        case 431:
            return (" Request Header Fields Too Large");
        case 451:
            return (" Unavailable For Legal Reasons");
        default:
            return ("");
    }
}

static std::string getReasonPhrase5xx(const int statusCode)
{
    switch (statusCode)
    {
        case 500:
            return (" Internal Server Error");
        case 501:
            return (" Not Implemented");
        case 502:
            return (" Bad Gateway");
        case 503:
            return (" Service Unavailable");
        case 504:
            return (" Gateway Timeout");
        case 505:
            return (" HTTP Version Not Supported");
        case 506:
            return (" Variant Also Negotiates");
        case 507:
            return (" Insufficient Storage");
        case 508:
            return (" Loop Detected");
        case 510:
            return (" Not Extended");
        case 511:
            return (" Network Authentication Required");
        default:
            return ("");
    }
}

std::string getReasonPhrase(const int statusCode)
{
    switch (statusCode / 100)
    {
       case 1: return getReasonPhrase1xx(statusCode);
       case 2: return getReasonPhrase2xx(statusCode);
       case 3: return getReasonPhrase3xx(statusCode);
       case 4: return getReasonPhrase4xx(statusCode);
       case 5: return getReasonPhrase5xx(statusCode);
       default: return "";
    }
}

void HttpResponse::sendBasicHeaderResponse(const int clientSocket, const int method) //TODO:: Implement MSG_DONTWAIT flag
{
	std::string header;
    int res;

	setCurrentDate(date);

	header.reserve(1024);
	//header += "HTTP/1.1 " + response.statusCode + " " + response.statusMessage + "\r\n";
	header += "HTTP/1.1 ";
    header +=  convertNumberToString(statusCode) + getReasonPhrase(statusCode) + "\r\n";
	header += "Date: " + date + "\r\n";
    if (method == GET)
    {	
        header += "Content-Length: " + convertNumberToString(contentLength) + "\r\n";
        header += "Content-Type: ";
        header += contentType;
        if(contentType == "text/plain" || contentType == "text/html" || contentType == ".css" || contentType == "application/javascript")
            header += "; charset=utf-8";
        header += "\r\n";   
        header += "Last-Modified: " + lastModifiedTime;
        header += "\r\n";        
        header += "Connection: сlose\r\n";
        header += "Server: \"WebServ 42 " + serverName + "\"";
        header += "\r\n";
        // header += "Cache-Control: " + response.cacheControl + "\r\n";
    }
    else if (method == POST)
    {
        // header += "Content-Length: " + convertNumberToString(contentLength) + "\r\n";
        // header += "Content-Type: text/html; charset=utf-8\r\n";
    }
    else if (method == DELETE)
    {
        // header += "Content-Length: " + convertNumberToString(contentLength) + "\r\n";
        // header += "Content-Type: text/html; charset=utf-8\r\n";
    }
	header += "\r\n";
    std::cout << "!!!header: " << header << std::endl;   
    res = send(clientSocket, header.c_str(), header.size(), 0);
	if (res == -1)
		throw std::runtime_error("Error sending basic header response" + std::string(strerror(errno))); //TODO: forbidden to check errno?
    else if(res == 0)
        throw std::runtime_error("Error sending basic header response: client closed connection" + std::string(strerror(errno)));
}

void HttpResponse::setStatusCode(const int statusCode)
{
    this->statusCode = statusCode;
}

void HttpResponse::setContentType(const std::string& contentType)
{
    this->contentType = contentType;
}

void HttpResponse::setLastModifiedTime(const std::string& time)
{
	this->lastModifiedTime = time;
}

void HttpResponse::setServerName(const std::string& servName)
{
    this->serverName = servName;
}

std::string HttpResponse::getserverName() const
{
    return (this->serverName);
}