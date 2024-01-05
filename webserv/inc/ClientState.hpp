#ifndef CLIENT_STATE_INCLUDED
# define CLIENT_STATE_INCLUDED

class WebServerConfig;

#include "RequestParser.hpp"
#include "Response.hpp"
#include "LookupConfig.hpp"

enum CLIENT_STATES
{
	CLIENT_STATE_REQUEST_PARSING,
	CLIENT_STATE_NUM
};

class ClientState
{
	public:
		ClientState(const int clientSocket, const WebServerConfig* webServerConfig);
		ClientState(const ClientState& src);
		ClientState& operator=(const ClientState& rhs);
		~ClientState(void);

		int				getClientSocket(void) const;
		bool			isTimeout(void) const;
		void			updateLastInteractionTime(void); // update the _lastInteraction time  with the current time

		HeaderFieldStateMachine requestParser;
		HttpResponse			response;
		LookupConfig			serverConfiguration;
		
		int						state;
		uint32_t				ipv4;
		uint16_t				port;
	private:
		int			_clientSocket;
		time_t		_lastInteraction;

};

#endif /* CLIENT_STATE_INCLUDED */