#include "Server.hpp"

int main()
{
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
  server.stop();
  return 0;
}
