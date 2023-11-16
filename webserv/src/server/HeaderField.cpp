#ifndef HEADER_FIELDS_HPP
# define HEADER_FIELDS_HPP

#include "HeaderField.hpp"

//TODO: decide for size limit of header fields
HeaderFieldStateMachine::HeaderFieldStateMachine(void)
                        : maxHeaderLength(8192), currentState(HEADER_NAME), positionInInput(0), paramterLength(0), lastChar('\0')
{
   stateTransitionArray[0] = &HeaderFieldStateMachine::handleStateHeaderName;
   stateTransitionArray[1] = &HeaderFieldStateMachine::handleStateHeaderOWS;
   stateTransitionArray[2] = &HeaderFieldStateMachine::handleStateHeaderValue;
   stateTransitionArray[3] = &HeaderFieldStateMachine::handleStateHeaderPairDone;
   stateTransitionArray[4] = &HeaderFieldStateMachine::handleStateHeaderBody;
}

//this function should read one header line of the input and if neccessary continue reading with the next chunk of data
int   HeaderFieldStateMachine::parseOneHeaderLine(const std::string& input)
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

const std::map<std::string, std::string>& HeaderFieldStateMachine::getParsedHeaders() const
{
   return headers;
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

#endif /* HTTP_HEADER_FIELDS_PARSER_HPP */