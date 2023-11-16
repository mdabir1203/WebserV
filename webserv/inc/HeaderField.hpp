#ifndef HEADER_FIELD_HPP
# define HEADER_FIELD_HPP

#include "Server.hpp"
#include "RequestParser.hpp"

#include <map>
#include <string>
#include <cctype>
#include <stdexcept>

enum HTTPHeaderParserState // do not move without changing the function pointer array
{
   HEADER_NAME,
   HEADER_OWS,
   HEADER_VALUE,
   HEADER_PAIR_DONE,
   HEADER_BODY,
   HEADER_END
};

class HeaderFieldStateMachine {
public:
   HeaderFieldStateMachine(void);

   int   parseOneHeaderLine(const std::string& input);
   void  parseChar(char input);
   const std::map<std::string, std::string>& getParsedHeaders() const;
   void  setCurrentState(const int state);
   void  setInputPosition(const size_t position);
   void  reset(void);

private:
   const size_t                         maxHeaderLength;  // To prevent buffer overflow attacks
   int                                  currentState;
   std::string                          headerName;
   std::string                          headerValue;
   std::map<std::string, std::string>   headers;
   size_t                               positionInInput;
   size_t                               paramterLength;
   char                                 lastChar;

   // Define the function pointer type
   typedef void (HeaderFieldStateMachine::*StateHandler)(char);

   // Define the array of function pointers
   StateHandler stateTransitionArray[5];// number of trnasition fuctions
   void  stateTransition(int state, int nextState);
   void	handleStateHeaderName(char c);
   void	handleStateHeaderOWS(char c);
   void  handleStateHeaderValue(char c);
   void  handleStateHeaderPairDone(char c);
   void  handleStateHeaderBody(char c);
};

#endif /* HEADER_FIELD_HPP */
