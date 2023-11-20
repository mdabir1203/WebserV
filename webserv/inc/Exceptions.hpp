#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

#include <Colors.hpp>
#include <string>
#include <exception>

class ErrorException : public std::exception
{
    private:
        std::string _message;
    public:
        ErrorException(std::string message) throw();       
        virtual const char* what() const throw();
        virtual ~ErrorException() throw() {}
};


#endif