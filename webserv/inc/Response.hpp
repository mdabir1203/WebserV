#ifndef RESPONSE_HPP_INCLUDED
# define RESPONSE_HPP_INCLUDED

#include <string>

class HttpResponse
{
	public:
		HttpResponse();
		HttpResponse(int statusCode, const std::string& contentType, const std::string& content);
		int WriteToBuffer(char* buffer, size_t bufferSize) const;
		void sendBasicHeaderResponse(const int clientSocket, const int method);

		std::string convertNumberToString(const long int number);
		
		void setStatusCode(const int statusCode);
		void setCurrentDate(std::string& date);


		static const std::string server;	//
		long int contentLength;
		std::string date;

	private:
		int statusCode;
		std::string contentType;
		std::string content;
};

#endif /* RESPONSE_HPP_INCLUDED */
