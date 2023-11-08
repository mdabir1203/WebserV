#include "server/server.hpp"

int main()
{
  try {
    SocketServer server(8080);

    while (true)
    {
      int clientSocket =server.acceptClient();
      server.HandleClient(clientSocket);
    }
  } catch (const std::exception& e){
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
