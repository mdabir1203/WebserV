#include "LocationConfig.hpp"

LocationConfig::LocationConfig()
			  : statusCode(0),
			    cgiConfig(NULL),
				rootDirectory(""),
				directoryListing(false),
				_allowedMethods(7) // 111 -> all allowed
{
	cgiConfig = new CGIConfig();
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

// //idea for uri matching:

// #include <iostream>
// #include <map>
// #include <string>

// // Structure representing a web server location configuration
// struct LocationConfig {
//     std::string handler;
//     bool allowAccess;
//     // Add other configuration parameters as needed
// };

// std::string findLongestCompleteMatch(const std::map<std::string, LocationConfig>& locationMap, const std::string& url) {
//     std::string longestMatch;

//     for (auto it = locationMap.lower_bound(url); it != locationMap.end(); ++it) {
//         const std::string& key = it->first;

//         if (key.compare(0, url.length(), url) == 0) {
//             if (key.length() > longestMatch.length()) {
//                 longestMatch = key;
//             }
//         } else {
//             break;  // No longer matching, exit the loop
//         }
//     }

//     return longestMatch;
// }

// int main() {
//     std::map<std::string, LocationConfig> locationMap;
//     locationMap["/images"] = {"/images-handler", true};
//     locationMap["/images/profile"] = {"/profile-images-handler", false};
//     locationMap["/assets/css"] = {"/css-handler", true};
//     locationMap["/assets/js"] = {"/js-handler", true};

//     std::string url = "/images/test/test.jpg";
//     std::string longestMatch = findLongestCompleteMatch(locationMap, url);

//     std::cout << "Longest complete match for '" << url << "': " << longestMatch << std::endl;

//     return 0;
// }
