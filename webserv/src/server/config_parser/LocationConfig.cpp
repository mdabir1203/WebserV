#include "LocationConfig.hpp"

LocationConfig::LocationConfig()
			  : statusCode(0),
			    cgiConfig(NULL),
				directoryListing(false),
				_allowedMethods(7) // 111 -> all allowed
{

}

LocationConfig::~LocationConfig()
{
	delete cgiConfig;
}

bool LocationConfig::isMethodAllowed(HttpMethod method)
{
	return (_allowedMethods.test(method));
}

void LocationConfig::disableMethod(HttpMethod method)
{
	_allowedMethods.reset(method);
}