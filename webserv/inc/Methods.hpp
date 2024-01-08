#ifndef METHODS_HPP_INCLUDED
#define METHODS_HPP_INCLUDED

class HeaderFieldStateMachine;
class HttpResponse;
class LookupConfig;
#include <string>

class Methods

// #define GET_METHOD 0
// #define POST_METHOD 1
// #define DELETE_METHOD 2
{
public:
	Methods();
	Methods(const Methods &copy);
	Methods &operator=(const Methods &rhs);
	~Methods();

	void handleMethod(const HeaderFieldStateMachine &parser, const int clientSocket, HttpResponse &response);
	bool isCGI(const std::string &filePath);
	bool isCGIAllowed(const std::string& cgiExtension);
	void setConfiguration(LookupConfig* configuration);

private:
	void handleGET(const HeaderFieldStateMachine &parser, const int clientSocket, HttpResponse &response);
	void handlePOST(const HeaderFieldStateMachine &parser, const int clientSocket, HttpResponse &response);
	void handleDELETE(const HeaderFieldStateMachine &parser, const int clientSocket, HttpResponse &response);
	void _handleCGI(const HeaderFieldStateMachine &parser, const int clientSocket, HttpResponse &response, int method);
	void sendFile(const int clientSocket, const std::string &filePath);
	LookupConfig* _configuration;   
	
	bool isDefaultDirectoryPageExisting(const std::string& path, HttpResponse& response);
	bool isDirectoryLocked(const std::string& path) const;
	void sendDirectoryListing(const std::string& path, HttpResponse& response, const int clientSocket);

	void setDefaultDirectoryPage(const std::string& defaultPagePath);
	/* GET Method */
	std::string defineContentType(const std::string& UriPath);
	std::string getLastModifiedTime(const std::string& file_path);
	std::string getServerName();

	/* CGI */
	std::string _retrieveCgiScriptPath(const std::string&UriPath, HttpResponse& response);
	

	std::string	defaultDirectoryPage;
};

#endif /* METHODS_HPP_INCLUDED */