#include "Server.hpp"
#include "Response.hpp"
#include "RequestParser.hpp"
#include "Methods.hpp"


SocketServer* SocketServer::_server = NULL;

std::string ToString(HttpMethod method)
{
	switch (method)
	{
		case GET:
		return "GET";
		case POST:
		return "POST";
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
	this->m_isRunning = false;
}

SocketServer::~SocketServer()
{
	close(this->m_socket);
}

SocketServer* SocketServer::getServer()
{
    if (_server == NULL)
    {
        throw std::logic_error("Server instance not created");
    }
    return _server;
}

void SocketServer::start()
{
    if (this->m_isRunning)
    {
        throw std::logic_error("Server is already running");
    }
    this->m_isRunning = true;
    _server = this;
	run(); // -> used to start the main server loop
}

void SocketServer::stop()
{
    if (!this->m_isRunning)
    {
        throw std::logic_error("Server is not running");
    }
    this->m_isRunning = false;
}

bool SocketServer::isRunning() const
{
    return this->m_isRunning;
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

// Add GET Method to match exactly to the URI (open , access , stat, read)



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
	HeaderFieldStateMachine parser;
	HttpResponse response;
	
	parser.parseOneHeaderLine(requestBuffer);

	Methods methodHandler;

	methodHandler.handleMethod(parser, clientSocket, response); //TODO: rapid request spamming leads to server failure

	// ----- ------- Testing Parer ------------  -----------
	// std::cout << "Method: " << ToString((HttpMethod)parser.getHeaderMethod()) << std::endl;
	// std::cout << "Uri: " << parser.getHeaderUri() << std::endl;
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


	//HttpRequest request(requestBuffer); // TODO: Send our parser here 
	// Generate and send the HTTP response
	// HttpResponse httpResponse(200, "text/html; charset=utf-8", ART);
	// // Send the response
	// char responseBuffer[8192];
	// int bytesWritten = httpResponse.WriteToBuffer(responseBuffer, sizeof(responseBuffer));
	// send(clientSocket, responseBuffer, bytesWritten, 0);
	close(clientSocket);
}

void	SocketServer::run()
{
	int epoll_fd = epoll_create(1); // Create epoll instance
	int i;
	if (epoll_fd == -1) {
		throw std::runtime_error("Error creating epoll instance");
	}

	struct epoll_event event;
	event.events = EPOLLIN; // Monitor for read events
	event.data.fd = this->m_socket; // Add server socket to epoll set
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, this->m_socket, &event) == -1)
	{
		throw std::runtime_error("Error adding server socket to epoll set");
	}

	while (this->m_isRunning)
	{
		struct epoll_event events[10]; // Array to store events
		int num_events = epoll_wait(epoll_fd, events, 10, 150); // Wait for events // TODO: define timeout

		if (num_events == -1) {
			// close(epoll_fd);
			std::cout << "socket_fd: " << this->m_socket << std::endl;
			close(this->m_socket);
			throw std::runtime_error("Error in epoll_wait");
		}

		// Process events
		i = 0;
		while (i < num_events) {
			int fd = events[i].data.fd;

			// If server socket has an event, accept new client connection
			if (fd == this->m_socket) {
				int clientSocket = acceptClient();
				event.events = EPOLLIN; // Monitor for read events
				event.data.fd = clientSocket;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
					throw std::runtime_error("Error adding client socket to epoll set");
				}
			} else {
				// Handle client request
				HandleClient(fd);
				close(fd); // Close client socket after handling request
			}
			i++;
		}
	}
}
