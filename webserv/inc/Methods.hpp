#ifndef METHODS_HPP_INCLUDED
#define METHODS_HPP_INCLUDED

class HeaderFieldStateMachine;
class HttpResponse;

#include <string>

class Methods
{
public:
	Methods();
	Methods(const Methods &copy);
	Methods &operator=(const Methods &rhs);
	~Methods();

	void handleMethod(const HeaderFieldStateMachine &parser, const int clientSocket, HttpResponse &response);
	bool isCGI(const std::string &filePath);

private:
	void handleGET(const HeaderFieldStateMachine &parser, const int clientSocket, HttpResponse &response);
	void handlePOST(const HeaderFieldStateMachine &parser, const int clientSocket, HttpResponse &response);
	void handleDELETE(const HeaderFieldStateMachine &parser, const int clientSocket, HttpResponse &response);
	void sendFile(const int clientSocket, const std::string &filePath);
};

#endif /* METHODS_HPP_INCLUDED */