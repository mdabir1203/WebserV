#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Server.hpp"
#include <ctime>
#include <cerrno>

class HttpResponse
{
	public:
		HttpResponse();
		HttpResponse(int statusCode, const std::string& contentType, const std::string& content);
		int WriteToBuffer(char* buffer, size_t bufferSize) const;
		void sendBasicHeaderResponse(const int clientSocket);

		std::string convertNumberToString(const long int number);
		
		void setStatusCode(const int statusCode);
		void setCurrentDate(std::string& date);


		static const std::string server;
		long int contentLength;
		std::string date;

	private:
		int statusCode;
		std::string contentType;
		std::string content;
};

#endif /* RESPONSE_HPP */
