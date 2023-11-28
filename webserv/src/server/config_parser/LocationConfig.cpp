#include "LocationConfig.hpp"

LocationConfig::LocationConfig()
			  : statusCode(0),
			    cgiConfig(NULL),
				directoryListing(false),
				_allowedMethods(7), // 111 -> all allowed
				rootDirectory("")
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



void LocationConfig::setMethod(int bit, bool value)
{	
	if (value == true)
		_allowedMethods.set(bit);
	else
		_allowedMethods.reset(bit);
}

int LocationConfig::getMethod(int bit)
{
	return _allowedMethods[bit];
}