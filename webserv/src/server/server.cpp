#include "Server.hpp"


class HttpResponse {
public:
  HttpResponse(int statusCode, const std::string& contentType, const std::string& content)
    : statusCode(statusCode), contentType(contentType), content(content) {}

  int WriteToBuffer(char* buffer, size_t bufferSize) const {
    std::ostringstream oss;
    oss << "HTTP/1.1 " << statusCode << "\r\n"
        << "Content-Type: " << contentType << "\r\n"
        << "Content-Length: " << content.size() << "\r\n\r\n"
        << content;
    std::string responseStr = oss.str();
    if (responseStr.size() >= bufferSize) {
      throw std::runtime_error("Buffer is too small");
    }
    std::strcpy(buffer, responseStr.c_str());
    return responseStr.size();
  }

private:
  int statusCode;
  std::string contentType;
  std::string content;
};


enum HttpMethod {
  GET,
  POST,
  PUT,
  DELETE,
  UNKNOWN
};

std::string ToString(HttpMethod method) {
  switch (method) {
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

class HttpRequest {
public:
  HttpRequest(const char* requestBuffer)
      : method_(ParseMethod(requestBuffer)), path_(ParsePath(requestBuffer)),
        headers_(ParseHeaders(requestBuffer)), body_(ParseBody(requestBuffer)) {}

  HttpMethod method() const { return method_; }
  const std::string& path() const { return path_; }
  const std::map<std::string, std::string>& headers() const { return headers_; }
  const std::string& body() const { return body_; }

private:
  HttpMethod method_;
  std::string path_;
  std::map<std::string, std::string> headers_;
  std::string body_;


  HttpMethod ParseMethod(const char* requestBuffer) {
    (void)requestBuffer;
  // For testing, always return GET
  return GET;
  }

  std::string ParsePath(const char* requestBuffer) {
    (void)requestBuffer;
    // For testing, always return a specific path
    return "/config_files/index.html";
  }

  std::map<std::string, std::string> ParseHeaders(const char* requestBuffer) {
    (void)requestBuffer;
    // For testing, always return a map with some specific headers
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "text/html";
    headers["Connection"] = "keep-alive";
    return headers;
  }

  std::string ParseBody(const char* requestBuffer) {
    (void)requestBuffer;
    // For testing, always return a specific body
    return "This is a test body";
  }
};








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

void SocketServer::HandleClient(int clientSocket) {
  // Read the HTTP request from the client
  char requestBuffer[8192];
  int bytesReceived = recv(clientSocket, requestBuffer, 8192, 0); // change 8192 ResponseBufferSize
  if (bytesReceived < 0) {
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
