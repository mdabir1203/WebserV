#include "Server.hpp"

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


  SocketServer server(8080);
  server.start();
  std::cout << "Server is running: " << server.isRunning() << std::endl;
  return 0;
}
