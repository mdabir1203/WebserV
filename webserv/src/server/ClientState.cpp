#include "ClientState.hpp"

#include <ctime>
#include <limits>

ClientState::ClientState(const int clientSocket, const WebServerConfig& webServerConfig)
			: _clientSocket(clientSocket),
			  state(CLIENT_STATE_REQUEST_PARSING)
{

	//TODO: initialize LookupConfig with webServerConfig change to mandatory in construcor of lookupConfig
}

ClientState::ClientState(const ClientState& src)
{
	//TODO: implement
}

ClientState& ClientState::operator=(const ClientState& rhs)
{
	//TODO: implement
}

ClientState::~ClientState(void)
{

}

int	ClientState::getClientSocket(void) const
{
	return (this->_clientSocket);
}

bool	ClientState::isTimeout(void) const
{
	time_t currentTime = std::time(NULL);
	time_t timeout = serverConfiguration.getTimeout();

	if (timeout > std::numeric_limits<time_t>::max() - this->_lastInteraction)
		return (false);
	else if (currentTime > _lastInteraction + timeout)
		return (true);
	return (false);
}

void ClientState::updateLastInteractionTime(void)
{
	this->_lastInteraction = std::time(NULL);
}

