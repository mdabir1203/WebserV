#ifndef HEADER_FIELD_HPP
# define HEADER_FIELD_HPP

#include <map>
#include <string>
#include <cctype>
#include <stdexcept>

enum HTTPHeaderParserState
{
   HEADER_NAME,
   HEADER_VALUE,
   HEADER_LWS,
   HEADER_END
};


class HeaderFieldStateMachine {
public:
   HeaderFieldStateMachine(void);

   void  parseChar(char input);
   const std::map<std::string, std::string>& getParsedHeaders() const;
   void  setCurrentState(const int state);

private:
   const size_t                         maxHeaderLength;  // To prevent buffer overflow attacks
   int                                  currentState;
   std::string                          headerName;
   std::string                          headerValue;
   std::map<std::string, std::string>   headers;
};

#endif /* HEADER_FIELD_HPP */
