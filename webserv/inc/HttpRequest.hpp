#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

#include "Server.hpp"

enum HttpMethod {
  GET,
  POST,
  DELETE,
  UNKNOWN
};

class HttpRequest
{
	public:
		HttpRequest(const char* requestBuffer);
		HttpMethod method() const;
		const std::string& path() const;
		const std::map<std::string, std::string>& headers() const;
		const std::string& body() const;

	private:
		HttpMethod method_;
		std::string path_;
		std::map<std::string, std::string> headers_;
		std::string body_;

		HttpMethod ParseMethod(const char* requestBuffer);
		std::string ParsePath(const char* requestBuffer);
		std::map<std::string, std::string> ParseHeaders(const char* requestBuffer);
		std::string ParseBody(const char* requestBuffer);
};

#endif /* HTTP_REQUEST_HPP */
