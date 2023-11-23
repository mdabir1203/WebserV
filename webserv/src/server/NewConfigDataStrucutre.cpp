#include "NewConfigDataStrucutre.hpp"

bool RouteConfig::isMethodAllowed(HttpMethod method)
{
	return allowedMethods.test(method);
}


void RouteConfig::disableMethod(HttpMethod method) // since default is all allowed
{
	allowedMethods.reset(method);
}

