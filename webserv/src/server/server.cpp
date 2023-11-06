#include "server.hpp"

SocketServer* SocketServer::_server = NULL;

void setupSocketAddr(sockaddr_in& address, int port) {
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
    setupSocketAddr(m_address, 80);  // 80 is the default port for http
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

SocketServer::~SocketServer(){
		close(this->m_socket);
	}

// opens a unique socket for each connection of client
int SocketServer::acceptClient(){
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    int clientSocket = accept(this->m_socket, (struct sockaddr *)&client_addr, &len);
    if (clientSocket == -1)
    {
        throw std::runtime_error("Socket accept failed");
    }
    return clientSocket;
  }

void SocketServer::handleClient(int clientSocket)
{
  // Read the HTTP request from the client
  char request_buff[1024]; // ? (we can try static / stream of parsing)
  int bytes_received = recv(clientSocket, request_buff, 1024, 0);
  if (bytes_received == -1)
  {
    throw std::runtime_error("Error in recv()");
  }
  // parse the request
  HttpRequest request(request_buff, bytesRead);
  // generate responses
  HttpResponse httpResponse;
  HttpResponse.setStatusCode(HttpStatusCode::OK); // add 200
  HttpResponse.setContentType("text/html");
  HttpResponse.setContent("<h1>Welcom to our sercer</h1>");

  // Send the response
  char response_buff[1024];
  int bytesWritten = httpResponse.toString(response_buff, sizeof(response_buff));
  send(clientSocket, response_buff, bytesWritten, 0);
  if (send < 0)
  {
    throw std::runtime_error("Error in send()");
  }
  close(clientSocket);
}

// TODO:Handle client 



// TODO:getServer()

// TODO:start()

// TODO:stop()

// TODO:isRunning() const