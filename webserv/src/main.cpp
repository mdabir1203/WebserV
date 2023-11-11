#include "Server.hpp"
#include <RequestParser.hpp>

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

  std::string request = "get / HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.64.1\r\nAccept: */*\r\n\r\n";
  HttpRequestParser parser;
  parser.parse(request);
  return 0;
}
