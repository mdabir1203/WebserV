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

	void handleMethod(const HeaderFieldStateMachine &parser, const int clientSocket, HttpResponse &response, const std::string& body);
	bool isCGI(const std::string &filePath);

private:
	void handleGET(const HeaderFieldStateMachine &parser, const int clientSocket, HttpResponse &response);
	void handlePOST(const HeaderFieldStateMachine &parser, const int clientSocket, HttpResponse &response);
	void handleDELETE(const HeaderFieldStateMachine &parser, const int clientSocket, HttpResponse &response);
	void sendFile(const int clientSocket, const std::string &filePath);
    bool doesResourceExist(const std::string& targetResource);
    bool IfResourceExists(const std::string& targetResource);
    void writeToFile(const std::string& targetResource, const std::string& payload);
    void createResource(const std::string& targetResource, const std::string& payload);
    void updateResource(const std::string& targetResource, const std::string& payload);

};

#endif /* METHODS_HPP_INCLUDED */