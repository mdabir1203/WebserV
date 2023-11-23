#ifndef NEWCONFIGDATASTRUCUTRE_HPP
#define NEWCONFIGDATASTRUCUTRE_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include <bitset>
#include "RequestParser.hpp"

class CGIConfig //not sure what needs to be in here
{
public:
    std::string cgiExtension;
    std::string cgiPath;
    std::string cgiDirectory;
    // Add more CGI-related configurations if needed
};

// Multiple return Directives:
// If you have multiple return directives in a location block, only the last one will take effect. For example:

// nginx

// location /example {
//     return 301 http://example.com/new-path;
//     return 302 http://example.com/another-path;
// }
// In this case, only the second return directive will be effective, and the first one will be ignored.
class RedirectConfig // name in the config file is "return code url" || "return code"
{
public:
    uint16_t statusCode;  // e.g., 301, 302, 303, 307, 308
    std::string targetUrl;
};

// Without specifying the upload_store directive in Nginx for direct handling of file uploads, the server won't know where to store the uploaded files, 
// likely resulting in an incomplete or failed upload process. This could lead to an HTTP error status returned to the client, 
// such as 500 Internal Server Error or 404 Not Found.

// ------------
// No Specified Root:
// If no system-wide default root is set, and the configuration doesn't specify a root directory, 
// Nginx may not be able to locate the files to serve. This can result in failed attempts to serve content,
// and clients will likely receive HTTP error responses, such as 404 Not Found or 403 Forbidden, 
// indicating that the requested resource cannot be found or accessed.

// --------------
// To ensure proper functioning, it's recommended to explicitly specify the root directive in your Nginx configuration. 
// This allows you to set the base directory from which Nginx should locate and serve files, 
// providing clarity and control over the file-serving behavior.

// --------
// When Nginx performs a redirect using the return directive without a specific status code, and the target is an external domain, the response header includes the following information:

//     Status Line:
//         Nginx will provide a status line indicating the HTTP status code. Without a specific status code, the default is 200 OK.

//     Location Header:
//         The Location header will be included in the response header, specifying the new URL to which the client's browser should make a new request. This header is crucial for indicating the redirection target.

// Here's an example response header for a redirect without a specific status code to an external domain:

// http

// HTTP/1.1 200 OK
// Server: nginx
// Date: [Date]
// Content-Type: text/html
// Content-Length: [Length]
// Connection: keep-alive
// Location: http://www.google.com

// In this example:

//     The status line indicates a 200 OK status code, which is the default when a specific code is not provided.
//     The Location header specifies the new URL (http://www.google.com) to which the client's browser should make a new request.

// It's important to note that while the response header provides instructions for redirection, the client's 
// browser interprets these instructions based on its security policies, such as the Same-Origin Policy (SOP).

// ------------
class RouteConfig
{
public:
    // std::string route; not needed since its the key// only one per block
    RedirectConfig redirectConfig;
    std::string rootDirectory; // if not specified best of luck finding the file //-> only one per block
    std::string uploadDirectory; // if not specified respond with error 500 //-> only one per block // -> named "upload_store" in config
    std::vector<std::string> defaultFolderFiles; // default file to be served if no file is specified in the path
    // -> named "index" in config, default is "index.html" // searched for in the directory requested left to right // for now limit to 1 according to subject interpretation
    //in nginx there can only be one "index" directive per block
    bool directoryListing; // directory listing will come into play if no default file is specified or present // only one per block
    CGIConfig cgiConfig;  // Include CGIConfig for CGI execution details
    // Add more route-specific configurations as needed

    bool isMethodAllowed(HttpMethod method);
    void disableMethod(HttpMethod method);

    private:
        std::bitset<3> allowedMethods; // 0 = GET, 1 = POST, 2 = DELETE // init to 111 -> all allowed
        // -> change name in config to  allowed_methods
};

// The setup of default error pages and limiting client body size can be done not only in the http context but also in the server and location contexts, allowing for more specific configurations. Here are examples for each:
// 1. Setup Default Error Pages in the server Context:

// nginx

// server {
//     listen 80;
//     server_name example.com;

//     # Other server configuration...

//     error_page 404 /404.html;
//     error_page 500 502 503 504 /50x.html;

//     location = /404.html {
//         root /path/to/your/error/pages;
//         internal;
// # Without this directive, there is no limit on the client request body size
//     # client_max_body_size 1M;
//     }

//     location = /50x.html {
//         root /path/to/your/error/pages;
//         internal;
//     }
    
//     # Other server configuration...
// }

// 2. Limit Client Body Size in the server Context:

// nginx

// server {
//     listen 80;
//     server_name example.com;

//     # Other server configuration...

//     client_max_body_size 10M; # Adjust the size as needed

//     # Other server configuration...
// }
class ServerConfig {
public:
    // std::string ipAddress;  // not needed since its the key
    // int port; // not needed since its the key
    std::set<std::string> serverNames;  // Use std::set for faster lookups
    std::map<uint16_t, std::string> customErrorPages;
    const std::map<uint16_t, std::string>& defaultErrorPages;
    size_t maxClientBodySize; // it is not askes for each route, but not specified further -> so in http and server
    bool maxClientBodySizeSet; // necessary to check if it was set in the config file
    std::map<std::string, RouteConfig> routes; // faster loopup than vector // key is the route

    //addRouteConfig() // init methods allowed to 111
};

class WebServerConfig {
public:
    //whate happens if no ip Address is specified?? -> use 0 as default since it means listen to any.
	std::map<std::pair<uint32_t , uint16_t>, std::vector<ServerConfig>> servers; //uses Ipv4-Addres and port as key. Vector is needed for multiple servers on same IP and port
    std::map<uint16_t, std::string> defaultErrorPages; // as switch statement?? // either custom or set in http context of config file // if not specified build on the go in fucntion for default error pages??
    size_t maxClientBodySize; // it is not askes for each route, but not specified further -> so in http and server // init to custom start value in bytes

    // not part of config file
    size_t maxClients; // init to custom start value
    size_t timeout; // in ms // init to custom start value
    // size_t maxHeaderFieldsSize; // init to custom start value

    //addServerConfig() //set the defaultErrorPages Reference
};

#endif /* NEWCONFIGDATASTRUCUTRE_HPP */
