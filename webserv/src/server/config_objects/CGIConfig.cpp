#include "CGIConfig.hpp"

#include <iostream>

#include "Colors.hpp"

CGIConfig::CGIConfig()
{
    // TODO: change this to be handled accordingly with the handler and the validator of the object Location
    //  cgiExtensions.insert(".sh");
    //  cgiExtensions.insert(".py");
    cgiExtensions.insert("");
}

CGIConfig::~CGIConfig()
{
}


CGIConfig::CGIConfig(const CGIConfig& source) : cgiExtensions(source.cgiExtensions)
{    
}

CGIConfig& CGIConfig::operator=(const CGIConfig& source)
{
    if (this != &source)
    {
        cgiExtensions = source.cgiExtensions;
    }
    return *this;
}

void CGIConfig::printConfig(void) const
{
    std::cout << CYAN << "===cgiConfig===" << RESET << std::endl;
    std::cout << "cgiExtensions: ";
    std::set<std::string>::const_iterator it;
    for (it = this->cgiExtensions.begin(); it != this->cgiExtensions.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
}
