#include "HttpRequest.hpp"

HttpRequest::HttpRequest(const char* requestBuffer)
			: method_(ParseMethod(requestBuffer)), path_(ParsePath(requestBuffer)),
        		headers_(ParseHeaders(requestBuffer)), body_(ParseBody(requestBuffer))
{

}

HttpMethod	HttpRequest::method() const
{
	return method_;
}

const std::string& HttpRequest::path() const
{
	return path_;
}

const std::map<std::string, std::string>& HttpRequest::headers() const
{
	return headers_;
}

const std::string& HttpRequest::body() const
{
	return body_;
}

HttpMethod HttpRequest::ParseMethod(const char* requestBuffer)
{
	(void)requestBuffer;
	// For testing, always return GET
	return GET;
}

std::string	HttpRequest::ParsePath(const char* requestBuffer)
{
	(void)requestBuffer;
	// For testing, always return a specific path
	return "/config_files/index.html";
}

std::map<std::string, std::string> HttpRequest::ParseHeaders(const char* requestBuffer)
{
	(void)requestBuffer;
	// For testing, always return a map with some specific headers
	std::map<std::string, std::string> headers;
	headers["Content-Type"] = "text/html";
	headers["Connection"] = "keep-alive";
	return headers;
}

std::string HttpRequest::ParseBody(const char* requestBuffer)
{
	(void)requestBuffer;
	// For testing, always return a specific body
	return "This is a test body";
}
