#include "Methods.hpp"

void Methods::setDefaultDirectoryPage(const std::string& defaultPagePath)
{
    this->defaultDirectoryPage = defaultPagePath;
}

bool Methods::isDefaultDirectoryPageExisting(const std::string& path, HttpResponse& response)
{
    struct stat fileInfo;

    setDefaultDirectoryPage(path + "/index.html");
    if (stat(defaultDirectoryPage.c_str(), &fileInfo) == 0 && S_ISREG(fileInfo.st_mode) && (fileInfo.st_mode & S_IRUSR))
    {
        response.contentLength = fileInfo.st_size;
        return (true);
    }
    defaultDirectoryPage.clear();
    return (false);
}

bool    Methods::isDirectoryLocked(const std::string& path) const //TODO: only need if delete and post can modify folders??
{
    (void)path;
    return (false); // TODO: implement correct check for directory locking
}

void Methods::sendDirectoryListing(const std::string& path, HttpResponse& response, const int clientSocket) //TODO: maybe add sorting of entries
{
    struct dirent *entry = NULL;
    DIR *dir = NULL;
    std::ostringstream directoryListingHtml;

    dir = opendir(path.c_str());
    if (dir == NULL) 
    {
        throw std::runtime_error("Error opening directory: " + path + ".\n ->" + strerror(errno));
        //TODO: send internal server error
    }

    //TODO: do we need to be careful with memory here??
    directoryListingHtml << "<html><body><h1>Directory Listing</h1><ul>";
    while (1)
    {
        errno = 0; //important to not read old errno value
        entry = readdir(dir);
        if (entry == NULL && errno == EBADF) //TODO: allowed?
        {
            closedir(dir);
            throw std::runtime_error("Error reading directory: " + path + ".\n ->" + strerror(errno));
        }
        else if (entry == NULL)
            break;
        directoryListingHtml << "<li><a href=\"" << entry->d_name << "\">" << entry->d_name << "</a></li>";
    }
    directoryListingHtml << "</ul></body></html>";

    if (closedir(dir) == -1) // TODO: what to do here if error?
        throw std::runtime_error("Error closing directory: " + path + ".\n ->" + strerror(errno));

    response.setStatusCode(200);
    response.contentLength = directoryListingHtml.str().length();
    response.sendBasicHeaderResponse(clientSocket, GET);

    int error = 0;
	socklen_t len = sizeof(error);
	if (getsockopt(clientSocket, SOL_SOCKET, SO_ERROR, &error, &len) == -1)
		throw std::runtime_error("Couldnt receive socket error");
	if (error)
		throw std::runtime_error("Socket error: " + std::string(strerror(error)));
	else if (send(clientSocket, directoryListingHtml.str().c_str(), directoryListingHtml.str().length(), 0) == -1) //TODO: do we need to chunk it?
		throw std::runtime_error("Error sending file: " + path);
}
