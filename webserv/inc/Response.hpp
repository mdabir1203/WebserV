#ifndef RESPONSE_HPP_INCLUDED
# define RESPONSE_HPP_INCLUDED

#include <string>

class HttpResponse
{
	public:
		HttpResponse();
		HttpResponse(const std::string& contentType, int statusCode, const std::string& content, const std::string& lastModifiedTime, const std::string& serverName);
		int WriteToBuffer(char* buffer, size_t bufferSize) const;
		void sendBasicHeaderResponse(const int clientSocket, const int method);

		std::string convertNumberToString(const long int number);
		
		void setStatusCode(const int statusCode);
		void setCurrentDate(std::string& date);
		void setContentType(const std::string& contentType);
		void setLastModifiedTime(const std::string& time);
		void setServerName(const std::string& servName);
		std::string getContentType() const;
		
		static const std::string server;
		long int contentLength;
		std::string date;
		std::string contentType;

	private:
		int statusCode;
		
		std::string content;
		std::string lastModifiedTime;	// Implemented only for files
		std::string serverName;

};

#endif /* RESPONSE_HPP_INCLUDED */
