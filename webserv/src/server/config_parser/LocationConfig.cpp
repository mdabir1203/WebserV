#include "LocationConfig.hpp"

LocationConfig::LocationConfig()
			  : redirectConfig(NULL),
			    cgiConfig(NULL),
				_allowedMethods(7) // 111 -> all allowed
{
	// std::cout << "LocationConfig constructor called" << std::endl;
}

LocationConfig::~LocationConfig()
{
	// std::cout << "LocationConfig destructor called" << std::endl;
	// if (redirectConfig)
	// 	delete redirectConfig;
	// if (cgiConfig)
		// delete cgiConfig;
}

bool LocationConfig::isMethodAllowed(HttpMethod method)
{
	return (_allowedMethods.test(method));
}

void LocationConfig::disableMethod(HttpMethod method)
{
	_allowedMethods.reset(method);
}