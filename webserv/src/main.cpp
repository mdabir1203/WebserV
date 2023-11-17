#include "Server.hpp"
#include "RequestParser.hpp"
#include "HeaderField.hpp"


#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <cstddef>
#include <cstring>

void signalHandler(int signum)
{
  (void)signum;
  SocketServer::getServer()->stop();
}

int main()
{
  signal(SIGINT, signalHandler);
  // try {
  //   SocketServer server(8080);

  //   while (true)
  //   {
  //     int clientSocket =server.acceptClient();
  //     server.HandleClient(clientSocket);
  //   }
  // } catch (const std::exception& e){
  //   std::cerr << e.what() << std::endl;
  //   return 1;
  // }


  // SocketServer server(8080);
  // server.start();
  // std::cout << "Server is running: " << server.isRunning() << std::endl;

  //std::string request = "DELETE / HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.64.1\r\nAccept: */*\r\n\r\n";
  // RequestParserNew parser;
  // parser.parse(request);
  // parser.parse(request);

//   std::string request = "Host: localhost:8080\r\nUser-Agent: curl/7.68.0\r\nAccept: */*\r\n";

//   HttpHeaderParser parser;
// for (std::string::iterator it = request.begin(); it != request.end(); ++it) {
//     parser.parse(*it);
// }

//   std::cout << "Header name: " << parser.getHeaderName() << std::endl;
//   std::cout << "Header value: " << parser.getHeaderValue() << std::endl;
//   return 0;

}
