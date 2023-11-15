#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Server.hpp"

class HttpResponse
{
	public:
		HttpResponse(int statusCode, const std::string& contentType, const std::string& content);
		int WriteToBuffer(char* buffer, size_t bufferSize) const;

	private:
		int statusCode;
		std::string contentType;
		std::string content;
};

#endif /* RESPONSE_HPP */