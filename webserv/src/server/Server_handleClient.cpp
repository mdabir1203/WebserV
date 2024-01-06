#include "Server.hpp"

#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

#include "ClientState.hpp"
#include "Methods.hpp"
#include "Colors.hpp"

void SocketServer::_handleClientAsync(ClientState& clientState)
{
    switch (clientState.state)
    {
		case CLIENT_STATE_REQUEST_PARSING:
			_readRequestAsync(clientState);
			break;
		case CLIENT_STATE_REQUEST_PROCESSING:
			_processRequest(clientState);
			break;

		case CLIENT_STATE_SENDING:
			_writeResponseAsync(clientState);
			break;

		case CLIENT_STATE_CLOSE_CONNECTION:
			_closeConnection(clientState);
			break;
    }
}

void    SocketServer::_readRequestAsync(ClientState& client)
{
	std::string buffer;
	int			bytesReceived;

	buffer.reserve(8192);
	bytesReceived = recv(client.getClientSocket(), &buffer[0], buffer.size(), 0);
	if (bytesReceived < 0)
	{
		client.response.setStatusCode(500); //TODO: check if right code
		client.state = CLIENT_STATE_SENDING;
	}
	else if (bytesReceived == 0)
	{
		client.state = CLIENT_STATE_REQUEST_PROCESSING;
		_processRequest(client);
	}
	if (client.requestParser.parseRequestHeaderChunk(buffer) == BAD_REQUEST)
	{
		client.response.setStatusCode(400);
		// client.response.sendBasicHeaderResponse(client.getClientSocket(), UNKNOWN);
		client.state = CLIENT_STATE_SENDING;
		return;
	}
}

void    SocketServer::_processRequest(ClientState& client)
{
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
		client.state = CLIENT_STATE_SENDING;
		return ;
		// client.response.sendBasicHeaderResponse(client.getClientSocket(), UNKNOWN);
	}
	client.state = CLIENT_STATE_CLOSE_CONNECTION;
}

void    SocketServer::_writeResponseAsync(ClientState& client)
{
	client.response.sendBasicHeaderResponse(client.getClientSocket(), GET);
}

void    SocketServer::_closeConnection(ClientState& client)
{
	close(client.getClientSocket());

	//delete client from list
	std::map<int, ClientState*>::iterator clientIt;
	clientIt = _clientStates.find(client.getClientSocket());
	delete clientIt->second;
	clientIt->second = NULL;
	_clientStates.erase(clientIt);
}
