#include "Server.hpp"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "RequestParser.hpp"
#include "Response.hpp"
#include "Methods.hpp"
#include "LocationConfig.hpp"
#include "Colors.hpp"
#include "ClientState.hpp"
#include "WebServerConfig.hpp"

SocketServer *SocketServer::_instancePtr = NULL;

SocketServer *SocketServer::getInstance(void)
{
	if (!SocketServer::_instancePtr)
		SocketServer::_instancePtr = new SocketServer();
	return (SocketServer::_instancePtr);
}

SocketServer::SocketServer(void)
	: _isRunning(false),
	  _epollFd(-1)
{
	
}

SocketServer& SocketServer::operator=(const SocketServer& rhs)
{
	(void)rhs;
	return (*this);
}

SocketServer::~SocketServer()
{
	std::set<int>::iterator it;
	for (it = _serverSockets.begin(); it != _serverSockets.end(); it++)
		close(*it);
	
	std::map<int, ClientState*>::iterator clientStateIt;
	for (clientStateIt = _clientStates.begin(); clientStateIt != _clientStates.end(); clientStateIt++)
		delete clientStateIt->second;

	if (_epollFd != -1)
		close(_epollFd);
}

void SocketServer::start(const std::set<uint16_t> &ports)
{
	if (this->_isRunning)
		throw std::logic_error("Server is already running");
	this->_isRunning = true;
	_initServerSockets(ports);
	_run(); // -> used to start the main server loop
}

void	SocketServer::_initServerSockets(const std::set<uint16_t> &ports)
{
	sockaddr_in m_address;
	std::memset(&m_address, 0, sizeof(m_address));

	m_address.sin_family = AF_INET;			// AF_INET is the Internet address family for IPv4.
	m_address.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY -> binds the socket to all available interfaces(e.g. wifi, ethernet, etc.)

	std::set<uint16_t>::iterator it = ports.begin();
	for (; it != ports.end(); it++)
	{
		m_address.sin_port = htons(*it); // htons() -> For transforming uniform byte order.
		std::pair<std::set<int>::iterator, bool> socketSetRet;
		socketSetRet = _serverSockets.insert(socket(AF_INET, SOCK_STREAM, 0));
		if (*(socketSetRet.first) == -1)
			throw std::runtime_error("Socket creation failed");
		if (bind(*(socketSetRet.first), (struct sockaddr *)&m_address, sizeof(m_address)) == -1)
			throw std::runtime_error("Socket binding failed for port"); //TODO: add port print
		if (listen(*(socketSetRet.first), 10) == -1) // 10 is the maximum number of connections
			throw std::runtime_error("Socket listening failed for port"); //TODO: add port print
	}
}

void SocketServer::stop()
{
	if (!this->_isRunning)
	{
		throw std::logic_error("Server is not running");
	}
	this->_isRunning = false;
	std::cout << "Server stopped." << std::endl;
}

bool SocketServer::isRunning() const
{
	return this->_isRunning;
}

void SocketServer::_run()
{
	int _epollFd = epoll_create(1); // Create epoll instance
	if (_epollFd == -1)
		throw std::runtime_error("Error creating epoll instance");

	std::set<int>::iterator it;
	for (it = _serverSockets.begin(); it != _serverSockets.end(); it++)
	{
		struct epoll_event event;
		event.events = EPOLLIN; // Monitor for read events
		event.data.fd = *it;	// Add server socket to epoll set
		if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, *it, &event) == -1)
		{
			throw std::runtime_error("Error adding server socket to epoll set");
		}
	}

	std::cout << "Server started." << std::endl;
	while (this->_isRunning)
	{
		struct epoll_event events[10];							// Array to store events
		int num_events = epoll_wait(_epollFd, events, 10, 150); // Wait for events // TODO: define timeout
		if (num_events == -1 && this->_isRunning)
			throw std::runtime_error("Error in epoll_wait");

		// Process events
		int i = 0;
		while (i < num_events)
		{
			int fd = events[i].data.fd;

			// If server socket has an event, accept new client connection
			if (this->_serverSockets.find(fd) != this->_serverSockets.end())
			{
				struct epoll_event event;
				int clientSocket = _acceptClient(fd); // Socket for connections between client and server
				event.events = EPOLLIN | EPOLLOUT;		// Monitor for read and write events
				event.data.fd = clientSocket;
				if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientSocket, &event) == -1)
				{
					throw std::runtime_error("Error adding client socket to epoll set");
				}
			}
			else
			{
				// Handle client request
				ClientState& client = _getClientState(fd);
				_handleClient(client);
				close(fd); // Close client socket after handling request
			}
			i++;
		}
	}
}

// opens a unique socket for each connection of client
int SocketServer::_acceptClient(int serverSocket)
{
	struct sockaddr_in client_addr;
	std::memset(&client_addr, 0, sizeof(client_addr));
	socklen_t len = sizeof(client_addr);
	int clientSocket = accept(serverSocket, (struct sockaddr *)&client_addr, &len);
	if (clientSocket == -1)
		throw std::runtime_error("Socket accept failed");
	ClientState& client = _getClientState(clientSocket);
	client.serverConfiguration.updateCurrentServer(client.serverConfiguration.getCurrentWebServer()->defaultServerBlock); //TODO: pick the right block to answer if the request does not make it till the host header
	client.ipv4 = ntohl(client_addr.sin_addr.s_addr);
	client.port = ntohs(client_addr.sin_port);
	return (clientSocket);
}

ClientState&	SocketServer::_getClientState(const int clientSocket)
{
	std::pair<std::map<int, ClientState*>::iterator, bool> insertResult;
	insertResult = this->_clientStates.insert(std::make_pair(clientSocket, (ClientState*)NULL));
	if (insertResult.second)
		insertResult.first->second = new ClientState(clientSocket, WebServerConfig::getInstance());
	return (*(insertResult.first->second));
}

void SocketServer::_handleClient(ClientState& client)
{
	// Read the HTTP request from the client
	char requestBuffer[8192];
	int bytesReceived = recv(client.getClientSocket(), requestBuffer, 8192, 0); // change 8192 ResponseBufferSize
	if (bytesReceived < 0)
	{
		throw std::runtime_error("Error in recv()");
	}
	// Temporarily parse the HTTP request

	if (client.requestParser.parseRequestHeaderChunk(requestBuffer) == BAD_REQUEST)
	{
		client.response.setStatusCode(400);
		client.response.sendBasicHeaderResponse(client.getClientSocket(), UNKNOWN);
		return;
	}

	Methods methodHandler;
	methodHandler.setConfiguration(&client.serverConfiguration);
	try
	{
		client.serverConfiguration.updateCurrentServer(client.ipv4, client.port, client.requestParser.getParsedHeaders().find("host")->second.front()); // TODO: make sure host is there, think about this: localhost:8082
		client.serverConfiguration.updateCurrentLocation(client.requestParser.getHeaderUriPath());
		client.serverConfiguration.updateUriWithLocationPath(client.requestParser.getHeaderUriPath());
		client.serverConfiguration.updateCurrentCGI();

		std::cout << PURPLE << "Requested Path in methodHandler: " << client.serverConfiguration.getUriPath() << RESET << std::endl;
		methodHandler.handleMethod(client.requestParser, client.getClientSocket(), client.response); // TODO: rapid request spamming leads to server failure
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error processing request: " << e.what() << std::endl; // TODO: send back 500 error
		client.response.setStatusCode(500);
		client.response.sendBasicHeaderResponse(client.getClientSocket(), UNKNOWN);
	}
	// ----- ------- Testing Parer ------------  -----------
	// std::cout << "Method: " << ToString((HttpMethod)parser.getHeaderMethod()) << std::endl;
	// std::cout << "Uri: " << parser.getHeaderUriPath() << std::endl;
	// std::cout << "Is HTTP version right: " << parser.getIsHttpVersionRight() << std::endl;
	// std::cout << "Headers: " << std::endl;
	// std::map<std::string, std::vector<std::string> > headers = parser.getParsedHeaders();
	// for (std::map<std::string, std::vector<std::string> >::iterator it = headers.begin(); it != headers.end(); ++it){
	// 	std::cout << it->first << ": ";
	// 	std::vector<std::string> values = it->second;
	// 	for (std::vector<std::string>::iterator it2 = values.begin(); it2 != values.end(); ++it2){
	// 		std::cout << *it2 << " ";
	// 	}
	// 	std::cout << std::endl;
	// }
	// std::cout << std::endl;
	// ---- ------- -------- -------- -------

	// TODO:Function required to handle METHODS (GET, POST, DELETE)

	// HttpRequest request(requestBuffer); // TODO: Send our parser here
	//  Generate and send the HTTP response
	//  HttpResponse httpResponse(200, "text/html; charset=utf-8", ART);
	//  // Send the response
	//  char responseBuffer[8192];
	//  int bytesWritten = httpResponse.WriteToBuffer(responseBuffer, sizeof(responseBuffer));
	//  send(clientSocket, responseBuffer, bytesWritten, 0);
	
	close(client.getClientSocket());

	//delete client from list
	std::map<int, ClientState*>::iterator clientIt;
	clientIt = _clientStates.find(client.getClientSocket());
	delete clientIt->second;
	clientIt->second = NULL;
	_clientStates.erase(clientIt);
}
