#include "RequestParser.hpp"

#include <iostream>

//TODO: decide for size limit of header fields
HeaderFieldStateMachine::HeaderFieldStateMachine(void)
                        : maxHeaderLength(8192), currentState(HEADER_METHOD),
                        positionInInput(0), paramterLength(0), lastChar('\0'),
                        headerMethod(0), isHttpVersionRight(false),
                        currentUriState(URI_START), uriIndex(0)
{
   stateTransitionArray[0] = &HeaderFieldStateMachine::handleStateHeaderMethod;
   stateTransitionArray[1] = &HeaderFieldStateMachine::handleStateHeaderUri;
   stateTransitionArray[2] = &HeaderFieldStateMachine::handleStateHeaderHttpVersion;
   stateTransitionArray[3] = &HeaderFieldStateMachine::handleStateHeaderName;
   stateTransitionArray[4] = &HeaderFieldStateMachine::handleStateHeaderOWS;
   stateTransitionArray[5] = &HeaderFieldStateMachine::handleStateHeaderValue;
   stateTransitionArray[6] = &HeaderFieldStateMachine::handleStateHeaderPairDone;
   stateTransitionArray[7] = &HeaderFieldStateMachine::handleStateHeaderBody;

   uriStateTransitionArray[0] = &HeaderFieldStateMachine::handleStateUriStart;
   uriStateTransitionArray[1] = &HeaderFieldStateMachine::handleStateUriScheme;
   uriStateTransitionArray[2] = &HeaderFieldStateMachine::handleStateUriAuthority;
   uriStateTransitionArray[3] = &HeaderFieldStateMachine::handleStateUriPath;
   uriStateTransitionArray[4] = &HeaderFieldStateMachine::handleStateUriQuery;
   uriStateTransitionArray[5] = &HeaderFieldStateMachine::handleStateUriFragment;
}

//this function should read one header line of the input and if neccessary continue reading with the next chunk of data
int   HeaderFieldStateMachine::parseRequestHeaderChunk(const std::string& input)
{
   try
   {
      while (positionInInput < input.length() && currentState != HEADER_END)
      {
         // std::cout << "current state: " << currentState << std::endl;
         // std::cout << "current char: " << input[positionInInput] << std::endl;
         parseChar(input[positionInInput]);
         lastChar = input[positionInInput];
         positionInInput++;
      }
   }
   catch(const std::exception& e)
   {
      std::cerr << e.what() << std::endl;
      return (BAD_REQUEST);
   }
   return (OK);
}

//currently not able to handle multiple occurences of the same header_field_name
void HeaderFieldStateMachine::parseChar(char input)
{
   (this->*stateTransitionArray[currentState])(input);
}

const std::map<std::string, std::vector<std::string> >& HeaderFieldStateMachine::getParsedHeaders() const
{
   return headers;
}

int HeaderFieldStateMachine::getHeaderMethod() const
{
   return headerMethod;
}

const std::string& HeaderFieldStateMachine::getHeaderUriPath() const
{
   return (uriParts.path);
}

const bool& HeaderFieldStateMachine::getIsHttpVersionRight() const
{
   return isHttpVersionRight;
}

void HeaderFieldStateMachine::setCurrentState(const int state)
{
   currentState = state;
}

void  HeaderFieldStateMachine::reset()
{
   currentState = HEADER_NAME;
   headerName.clear();
   headerValue.clear();
   headers.clear();
}

void  HeaderFieldStateMachine::setInputPosition(const size_t position)
{
   positionInInput = position;
}

void HeaderFieldStateMachine::stateTransition(int state, int nextState)
{
   (void)state;
   this->currentState = nextState;
}
