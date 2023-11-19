# include "Common_header.hpp"

ErrorException::ErrorException(std::string message) throw()
{
    _message = RED "CONFIG PARSER: " + message + RESET;
}

const char* ErrorException::what() const throw()
{
    return (_message.c_str());
}