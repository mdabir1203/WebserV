#include "Server.hpp"
#include "Response.hpp"
#include "HttpRequest.hpp"

SocketServer* SocketServer::_server = NULL;

std::string ToString(HttpMethod method)
{
	switch (method)
	{
		case GET:
		return "GET";
		case POST:
		return "POST";
		case PUT:
		return "PUT";
		case DELETE:
		return "DELETE";
		default:
		return "UNKNOWN";
	}
}

void setupSocketAddr(sockaddr_in& address, int port)
{
	// setup the socket address structure
	//AF_INET is the Internet address family for IPv4.
	address.sin_family = AF_INET;
	//htons() -> For transforming uniform byte order.
	address.sin_port = htons(port);
	//INADDR_ANY -> binds the socket to all available interfaces(e.g. wifi, ethernet, etc.)
	address.sin_addr.s_addr = INADDR_ANY;
}

SocketServer::SocketServer(int port)
{
    this->m_socket = socket(AF_INET, SOCK_STREAM, 0);  // ipv4, tcp, default protocol
    if (this->m_socket == -1)
    {
       throw std::runtime_error("Socket creation failed");
    }
    sockaddr_in m_address;
    setupSocketAddr(m_address, port);  // 80 is the default port for http
    if (bind(m_socket, (struct sockaddr *)&m_address, sizeof(m_address)) == -1)
	{
		throw std::runtime_error("Socket binding failed");
	}
	if (listen(m_socket, 10) == -1) // 10 is the maximum number of connections
	{
		throw std::runtime_error("Socket listening failed");
	}
	this->m_port = port;
	this->m_isRunning = true;
}

SocketServer::~SocketServer()
{
	close(this->m_socket);
}

// opens a unique socket for each connection of client
int SocketServer::acceptClient()
{
	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	int clientSocket = accept(this->m_socket, (struct sockaddr *)&client_addr, &len);
	if (clientSocket == -1)
	{
		throw std::runtime_error("Socket accept failed");
	}
	return clientSocket;
}

void SocketServer::HandleClient(int clientSocket)
{
	// Read the HTTP request from the client
	char requestBuffer[8192];
	int bytesReceived = recv(clientSocket, requestBuffer, 8192, 0); // change 8192 ResponseBufferSize
	if (bytesReceived < 0)
	{
		throw std::runtime_error("Error in recv()");
	}
	// Temporarily parse the HTTP request
	HttpRequest request(requestBuffer);
	// Generate and send the HTTP response
	HttpResponse httpResponse(200, "text/html; charset=utf-8", "<h1>Here goes the Fear Blasters</h1>");
	// Send the response
	char responseBuffer[8192];
	int bytesWritten = httpResponse.WriteToBuffer(responseBuffer, sizeof(responseBuffer));
	send(clientSocket, responseBuffer, bytesWritten, 0);
	close(clientSocket);
}
