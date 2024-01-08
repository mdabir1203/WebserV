#include <iostream>
#include <string>
#include <stdexcept>
#include <signal.h>

#include "Server.hpp"
#include "WebServerConfig.hpp"
#include "ConfigParser.hpp"
#include "LookupConfig.hpp"

void signalHandler(int signum)
{
  (void)signum;
  SocketServer::getInstance()->stop();
  // _exit(signum); // TODO: remove this
}

int main(int argc, char **argv, char **envp)
{
  std::string configPath = "src/config_files/default.conf";
  (void)envp;
  if (argc > 2)
    throw std::runtime_error("Error: wrong number of arguments");
  if (argc == 2)
    configPath = argv[1];

  signal(SIGINT, signalHandler);

  SocketServer *server = NULL;
  WebServerConfig webserverconfig;
  ConfigParser parser(&webserverconfig);
  LookupConfig configuration;

  try
  {
    parser.parseConfig(configPath);
    configuration.setCurrentWebServer(parser.getWebServerConfig());
    server = SocketServer::getInstance();
    server->setConfiguration(configuration);
    configuration.setSocketServer(server);
    server->start(configuration.getServerPorts());
  }
  catch (const std::exception &e)
  {
    std::cerr << "Server failed: " << e.what() << std::endl;
  }
  delete server;
}
