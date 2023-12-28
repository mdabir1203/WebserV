#include "Methods.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include <sys/socket.h>

#include "RequestParser.hpp"
#include "Response.hpp"
#include "LookupConfig.hpp"

Methods::Methods()
{
}

Methods::Methods(const Methods& copy)
{
	(void)copy;
}

Methods& Methods::operator=(const Methods& rhs)
{
	(void)rhs;
	return (*this);
}

Methods::~Methods()
{
}


void Methods::handleMethod(const HeaderFieldStateMachine& parser, const int clientSocket, HttpResponse& response)
{
	switch (parser.getHeaderMethod())
	{
		case GET:
			handleGET(parser, clientSocket, response);
			break ;
		case POST:
			handlePOST(parser, clientSocket, response);
			break ;
		case DELETE:
			handleDELETE(parser, clientSocket, response);
			break ;
		default:
			break ;
	}
}



// // stat struct INFO 
//     dev_t st_dev: The device ID of the device containing the file.
//     ino_t st_ino: The inode number of the file.
//     mode_t st_mode: The mode of the file.
//     nlink_t st_nlink: The number of hard links to the file.
//     uid_t st_uid: The user ID of the file.
//     gid_t st_gid: The group ID of the file.
//     dev_t st_rdev: The device ID of the device (if the file is a character or block special file).
//     off_t st_size: The size of the file in bytes. -> Important for Content-Length 
//     time_t st_atime: The last access time of the file.
//     time_t st_mtime: The last modification time of the file. -> important for Cache-Control
//     time_t st_ctime: The last change time of the file. -> important for Cache-Control

void Methods::sendFile(const int clientSocket, const std::string& filePath)
{
	std::ifstream file(filePath.c_str(), std::ios::binary);
	std::string buffer;

	buffer.reserve(1000);

	if (!file.is_open())
	{
		throw std::runtime_error("Error opening file: " + filePath);
	}
	// TODO: send file in chunks of xxxx bytes and not in one go
	while (file.good())
	{
		file.read(&buffer[0], 1000); //TODO: make enum for buffer size && maybe check for allocation errors
		if (file.bad())
		{
			break;
		}
		std::streamsize bytesRead = file.gcount();
		int error = 0;
		socklen_t len = sizeof(error);
		if (getsockopt(clientSocket, SOL_SOCKET, SO_ERROR, &error, &len) == -1)
		{
			file.close();
			throw std::runtime_error("Couldnt receive socket error");
		}
		if (error)
		{
			file.close();
			throw std::runtime_error("Socket error: " + std::string(strerror(error)));
		}
		else if (bytesRead && send(clientSocket, buffer.c_str(), bytesRead, 0) == -1)
		{
			file.close();
			throw std::runtime_error("Error sending file: " + filePath);
		}
	}
	if (!file.eof())
	{
		file.close();
		throw std::runtime_error("Error reading file: " + filePath);
	}
	file.close();
}

bool	Methods::isCGI(const std::string& filePath)
{
	(void)filePath;
	
	return (false);
}

void Methods::handleGET(const HeaderFieldStateMachine& parser, const int clientSocket, HttpResponse& response) //TODO: store answers in a queue and send them in a loop
{	
	std::cout << " PATH:: " << (_configuration->getUriPath().c_str()) << std::endl;
	struct stat fileInfo;

	if (stat(_configuration->getUriPath().c_str(), &fileInfo) != 0) 
	{
		if (errno == EACCES) //TODO: allowed?
			response.setStatusCode(403);
		else
			response.setStatusCode(404);
		response.sendBasicHeaderResponse(clientSocket, UNKNOWN);
		std::cout << " GET method processed 404" << std::endl; // TODO:Provide error page 404
		return ;
	}
	else if (isCGI(_configuration->getUriPath()))
	{
		if (fileInfo.st_mode & S_IXUSR)
		{
			response.setStatusCode(200);
			response.sendBasicHeaderResponse(clientSocket, parser.getHeaderMethod()); //after execution started
			std::cout << " GET method processed 200 - CGI" << std::endl;
			//TODO: execute cgi script
			return ;
		}
		else // File is a cgi script and is not executable
		{
			response.setStatusCode(403);
			response.sendBasicHeaderResponse(clientSocket, UNKNOWN);
			std::cout << " GET method processed 403 - CGI" << std::endl; // TODO:Provide error page 403
			return ;
		}
	}
	else
	{
		if (S_ISDIR(fileInfo.st_mode))
		{
			if (isDefaultDirectoryPageExisting(_configuration->getUriPath(), response))
			{
				response.setStatusCode(200);
				response.sendBasicHeaderResponse(clientSocket, parser.getHeaderMethod());
				sendFile(clientSocket, defaultDirectoryPage);
				std::cout << " GET method processed 200 - Directory default page" << std::endl;
				return ;
			}
			// else if (isDirectoryLocked(parser.getHeaderUri())) //by another process
			// {
			// 	response.setStatusCode(403); //internal server error?
			// 	response.sendBasicHeaderResponse(clientSocket, UNKNOWN);
			// 	std::cout << " GET method processed 403 - Directory" << std::endl;
			// 	return ;
			// }
			if (parser.isDirectoryListing() && (fileInfo.st_mode & S_IRUSR))
			{
				sendDirectoryListing(_configuration->getUriPath(), response, clientSocket);
				std::cout << " GET method processed 200 - Directory Listing" << std::endl;
				return ;
			}
			response.setStatusCode(403); // no listing and no default page
			response.sendBasicHeaderResponse(clientSocket, parser.getHeaderMethod());
			std::cout << " GET method processed 403 - Directory" << std::endl;
			return ;
		}
		else if (S_ISREG(fileInfo.st_mode) && (fileInfo.st_mode & S_IRUSR))
		{
			response.contentLength = fileInfo.st_size;
			response.setStatusCode(200);
			response.sendBasicHeaderResponse(clientSocket, parser.getHeaderMethod());
			sendFile(clientSocket, _configuration->getUriPath());
			std::cout << " GET method processed 200 - File" << std::endl;
		}
		else  // File is not regular or is not readable
		{
			response.setStatusCode(403);
			response.sendBasicHeaderResponse(clientSocket, UNKNOWN);
			std::cout << " GET method processed 403" << std::endl;
			return ;
		}
	}
}

void Methods::handlePOST(const HeaderFieldStateMachine& parser, const int clientSocket, HttpResponse& response)
{
	(void)parser;
	response.setStatusCode(201);
	response.sendBasicHeaderResponse(clientSocket, parser.getHeaderMethod());
	std::cout << " POST method processed" << std::endl;
}


// void Methods::handleDELETE(const HeaderFieldStateMachine& parser, const int clientSocket, HttpResponse& response)
// {
// 	(void)parser;
// 	response.setStatusCode(204);
// 	response.sendBasicHeaderResponse(clientSocket, parser.getHeaderMethod());
// 	std::cout << " DELETE method processed" << std::endl;
// }

void Methods::handleDELETE(const HeaderFieldStateMachine& parser, const int clientSocket, HttpResponse& response)
{
	struct stat fileInfo;

	if (stat(_configuration->getUriPath().c_str(), &fileInfo) != 0) 
	{
		if (errno == EACCES) //TODO: allowed?
			response.setStatusCode(403);
		else
			response.setStatusCode(404);
		response.sendBasicHeaderResponse(clientSocket, UNKNOWN);
		std::cout << " Delete method processed 404" << std::endl; // TODO:Provide error page 404
		return ;
	}
	else if (S_ISDIR(fileInfo.st_mode))
	{
		response.setStatusCode(405);
		response.sendBasicHeaderResponse(clientSocket, parser.getHeaderMethod());
		std::cout << " DELETE method processed 405 - Directory" << std::endl;
		return ;
	}
	else if (S_ISREG(fileInfo.st_mode) && (fileInfo.st_mode & S_IWUSR) && _configuration->getUriPath() == "/workspaces/WebserV/webserv/var/www/dogs.com/delete_test/deleteMe") //TODO: remove hardcoded path when URI parsing is implemented
	{
		if (std::remove(_configuration->getUriPath().c_str()) == 0)
		{
			response.setStatusCode(204);
			response.sendBasicHeaderResponse(clientSocket, parser.getHeaderMethod());
			std::cout << " DELETE method processed 204 - File" << std::endl;
			return ;
		}
		else if (errno == EACCES)
		{
			response.setStatusCode(403);
			response.sendBasicHeaderResponse(clientSocket, parser.getHeaderMethod());
			std::cout << " DELETE method processed 403 - File" << std::endl;
			return ;
		}
		else
		{
			response.setStatusCode(500);
			response.sendBasicHeaderResponse(clientSocket, parser.getHeaderMethod());
			std::cout << " DELETE method processed 500 - File" << std::endl;
			return ;
		}
	}
	else if (!(fileInfo.st_mode & S_IWUSR))
	{
		response.setStatusCode(403);
		response.sendBasicHeaderResponse(clientSocket, parser.getHeaderMethod());
		std::cout << " DELETE method processed 403 - File" << std::endl;
		return ;
	}
}

void    Methods::setConfiguration(LookupConfig* configuration)
{
	_configuration = configuration;
}

