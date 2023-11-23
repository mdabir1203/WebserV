#include "ConfigParser.hpp"

void ConfigParser::checkConfigFile(std::string filename)
{  
    int open_bracket = 0;
    bool empty_bracket = false;
    int i = 0;
    
    FILE* fin = fopen(filename.c_str(), "r");
    if (!fin)
    {
        throw ErrorException("Error: Can't open configuration file");
    }
    if (fgetc(fin) == EOF)
    {
        throw ErrorException("Error: Empty configuration file");
    }
    std::string line;
    std::ifstream file(filename.c_str());
    while (std::getline(file, line))
    {  
        i = 0;
        while(line[i] != '\0' && line[i] != '\n')
        {   if(line[i] == '#')
                throw ErrorException("Error: comments are not allowed");
            else if(line[i] == '{')
            {
                open_bracket++;
                empty_bracket = true;
            }
            else if(line[i] == '}')
            {
                open_bracket--;
                if(empty_bracket == true)
                    throw ErrorException("Error: empty brackets in configuration file");
            }
            else if( line[i] != ' ' && line[i] != '\t')
            {
                empty_bracket = false;
            }
            i++;            
        }       
    }
    if(open_bracket != 0)
        throw ErrorException("Unclosed brackets found");
    fclose(fin);
}



int ConfigParser::handleServerVarPort(std::istringstream& iss, std::string &token) 
{	
    int number; 
    std::size_t index = 0;
    
    iss >> token;
    token = checkToken(iss, token, true);
    while (index < token.length()) 
	{
        if (!std::isdigit(static_cast<unsigned char>(token[index]))) 
			throw ErrorException("Error: invalid variable \"port\" in configuration file");
        ++index;
    }
    number = std::atoi(token.c_str());	
    if (number < 0 || number > 65535)
		throw ErrorException("Error: invalid variable \"port\" in configuration file");
	return number;
}

std::string ConfigParser::handleServerVarName(std::istringstream& iss, std::string &token) 
{	
    //std::cout <<RED<< "tokken" << token << "\n" << RESET << std::endl;
    iss >> token;
       
	bool containsLetter = false;
    std::string temp;

    temp = checkToken(iss, token, false);
    if (token.empty())
        temp = "default";
    for (size_t i = 0; i < temp.length(); ++i)
	{
        if (std::isalpha(temp[i]))
		{
            containsLetter = true;
            break;
        }
    }
    if (containsLetter == true)
        return temp;
	else 
        throw ErrorException("Variable server_name must consist at least one a-z character");
}

bool isErrorCodeValidForErrorPage(int code)
{
    if (code >= 100 && code <= 599)
        return true;
    return false;
}

int ConfigParser::checkCodeErrorPage(std::istringstream& iss, std::string &str) 
{   
    int number; 
    std::string::iterator it = str.begin();

    iss >> str;
    while (std::isspace(static_cast<unsigned char>(*it))) 
        ++it;
    if (it == str.end()) 
       throw ErrorException("Error: invalid error page code in configuration file");
    while (it != str.end()) 
    {
        if (!std::isdigit(static_cast<unsigned char>(*it))) 
           throw ErrorException("Error: invalid error page code in configuration file");
        ++it;
    }
    number = std::atoi(str.c_str());
    if (InvalidCodesList.find(number) != InvalidCodesList.end())
        return number;
   throw ErrorException("Error: invalid error page code in configuration file");
}

bool ConfigParser::checkFileExist(const std::string &filePath, int specifier)
{
    std::string trimmedFilePath = filePath;
    size_t startPos = trimmedFilePath.find_first_not_of(" \t\n\r");
    size_t endPos = trimmedFilePath.find_last_not_of(" \t\n\r");
    
    
    if(filePath[0] == '.')
        throw ErrorException("Error: adress shouldn't start with dot");
    if (startPos != std::string::npos && endPos != std::string::npos)
        trimmedFilePath = trimmedFilePath.substr(startPos, endPos - startPos + 1);
	else
    {
        if(specifier == ERR_PAGE)
            throw ErrorException("Error: invalid error page in configuration file");
        else if (specifier == ROOT_PAGE)
            throw ErrorException("Error: invalid root page in location unit of configuration file");
        else if (specifier == INDEX_PAGE)
            throw ErrorException(" Error: invalid Index page in location unit of configuration file"); 
        else if (specifier == CGI_EXEC)
            throw ErrorException("Error: invalid CGI directory in location unit of configuration file");                  
    }		
    if (trimmedFilePath.empty())
    {
        if(specifier == ERR_PAGE)
            throw ErrorException("Error: invalid error page in configuration file");
        else if (specifier == ROOT_PAGE)
            throw ErrorException("Error: invalid root page in location unit of configuration file");
        else if (specifier == INDEX_PAGE)
            throw ErrorException(" Error: invalid Index page in location unit of configuration file");
        else if (specifier == CGI_EXEC)
            throw ErrorException("Error: invalid CGI directory in location unit of configuration file");  
    }	
    std::ifstream file(trimmedFilePath.c_str());
    if (!file.good()) 
	{
        if(specifier == ERR_PAGE)
            throw ErrorException("Error: invalid error page in configuration file");
        else if (specifier == ROOT_PAGE)
            throw ErrorException("Error: invalid root page in location unit of configuration file");
        else if (specifier == INDEX_PAGE)
            throw ErrorException(" Error: invalid Index page in location unit of configuration file");
        else if (specifier == CGI_EXEC)
            throw ErrorException("Error: invalid CGI directory in location unit of configuration file");  
    }
	return true;
}

bool ConfigParser::directoryExists(const std::string& path, int specifier)
{
    struct stat info;
    if(path[0] == '.')
        throw ErrorException("Error: adress shouldn't start with dot");
    if (stat(path.c_str(), &info) != 0)
    {
        if(specifier == ROOT_DIR)
        {   
            throw ErrorException("Root directory not found");
        }
        else if(specifier == CGI_DIR)
        {   
            throw ErrorException("CGI directory not found");
        }
        else if(specifier == UPLOAD_DIR)
        {   
            if(mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
                throw ErrorException("Upload directory can't be created");
        }
    }
    return (info.st_mode & S_IFDIR) != 0;
}

std::string ConfigParser::check_for_double_location(std::istringstream& iss, t_serv& currentServer)
{   
    std::string location_name;
    iss >> location_name;
    if(location_name.empty())
        return "default";
    for (std::multimap<std::string, Location>::iterator it = currentServer.loc.begin(); it != currentServer.loc.end(); ++it)
    {

        if(it->first == location_name)
        {   
            throw ErrorException("Error: two locations have the same name");
        }            
    }
    return location_name;
}

void ConfigParser::check_is_token_allowed(std::string &token)
{
	const char* validParams[] = {
        "timeout:", "timeout:;",
        "max_clients:",  "max_clients:;",
        "max_size_of_file:",  "max_size_of_file:;",
        "server",
        "port:",
        "server_name:", "server_name:;",
        "error_page", "error_page;",
        "location",
        "root:",
        "index:",  "index:;",
        "cgi_ext:",  "cgi_ext:;",
        "cgi_path:", "cgi_path:;",
        "upload_dir:",  "upload_dir:;",
        "http_redirect:", "http_redirect:;",
        "methods:", "methods:;",
        "autoindex:", "autoindex:;",
		"<server_end>",
		"{",
		"}",
		"}}",
		""
    };
    
    int numParams = sizeof(validParams) / sizeof(validParams[0]);
    for (int i = 0; i < numParams; ++i) 
	{
        if (token == validParams[i]) 
		{
            return;
        }
    }
  	throw ErrorException("Error: invalid variable in configuration file");  
}


int ConfigParser::handleGlobalSettings(std::istringstream& iss, std::string &token, int specifier)
{
    int number;

    iss >> token;
    if (token.empty() && specifier == TIMEOUT)            
        return 5;
    else if ((token.empty())&& specifier == MAX_CLIENTS)
        return 200;
    else if ((token.empty()) && specifier == MAX_SIZE_OF_FILE)
        return 1000000;	
    token = checkToken(iss, token, false);
    std::istringstream stream(token);
    if (!(stream >> number))
    {
        throw ErrorException("Invalid global variable argument in configuration file");
    }
    if(specifier == TIMEOUT && (number < 0 || number > 60))
        throw ErrorException("Invalid timeout value in configuration file. Please enter between 0 and 60 seconds");
    else if(specifier == MAX_CLIENTS && (number < 0 || number > 200))
        throw ErrorException("Invalid max clients value in configuration file. Please enter between 0 and 200 clients");
    else if(specifier == MAX_SIZE_OF_FILE && (number < 0 || number > 1000000))
        throw ErrorException("Invalid max size of file in configuration file. Please enter between 0 and 1000000 bytes");
    return number;    
}
 
std::string ConfigParser::checkToken(std::istringstream& iss, std::string &token, bool check_empty)
{   
    std::string temp;
    if (check_empty == true)
    {
        if (token.empty())
            throw ErrorException("Empty variable");
    }
    if (!token.empty() && token[token.length() - 1] == ';') 
    {
        token.erase(token.length() - 1);
        return token;
    }
    else if (token.find(';') == std::string::npos)
    {
        temp = token;
        while (iss >> token)
        {
            if (token.find(';') != std::string::npos)
                break ;
        }
        if (token.find(';') == std::string::npos)
            throw ErrorException("Semicolon missed");
    }
    // std::cout <<RED<< "tok" << token << "\n" << RESET << std::endl;
    // std::cout <<RED<< "tok" << temp << "\n" << RESET << std::endl;
    return temp;
}

std::vector<std::string> ConfigParser::handleCgiExt(std::istringstream& iss)
{   
    std::string token;
    std::string temp;
    std::vector<std::string>  vektor;
    bool flag_stop = false;
    
    while (iss >> token && flag_stop == false)
    {
        temp = token;
        if (!token.empty() && token[token.length() - 1] == ';') 
        {
            token.erase(token.length() - 1);
            flag_stop = true;
        }
        if (token != ".py" && token != ".php" && token != ".pl" && 
            token != ".rb" && token != ".sh")
                throw ErrorException("Invalid CGI extension type"); 
        else
            vektor.push_back(token);
    }
    if (temp.find(';') == std::string::npos)
        throw ErrorException("Semicolon missed");   
    return vektor;    
}

std::vector<std::string> ConfigParser::handleMethods(std::istringstream& iss)
{   
    std::string token;
    std::string temp;
    std::vector<std::string>  vektor;
    bool flag_stop = false;
    
    while (iss >> token && flag_stop == false)
    {
        temp = token;
        if (!token.empty() && token[token.length() - 1] == ';') 
        {
            token.erase(token.length() - 1);
            flag_stop = true;
        }
        if (token != "GET" && token != "POST" && token != "DELETE")
                throw ErrorException("Invalid method"); 
        else
            vektor.push_back(token);
    }
    if (temp.find(';') == std::string::npos)
        throw ErrorException("Semicolon missed");   
    return vektor;    
}
