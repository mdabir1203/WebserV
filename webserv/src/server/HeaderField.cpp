#ifndef HEADER_FIELDS_HPP
# define HEADER_FIELDS_HPP

#include "HeaderField.hpp"


//TODO: decide for size limit of header fields
HeaderFieldStateMachine::HeaderFieldStateMachine(void)
                        : maxHeaderLength(8192), currentState(HEADER_NAME), positionInInput(0)
{

}

//this function should read one header line of the input and if neccessary continue reading with the next chunk of data
int   HeaderFieldStateMachine::parseOneHeaderLine(const std::string& input)
{
   try
   {
      while (positionInInput < input.length() && currentState != HEADER_END)
         parseChar(input[positionInInput++]);
   }
   catch(const std::exception& e)
   {
      std::cerr << e.what() << std::endl;
      return (BAD_REQUEST);
   }
   return (OK);
}

//currently not able to handle multiple occurences of the same header_field_name
void HeaderFieldStateMachine::parseChar(char input) {
      if (headerName.length() > maxHeaderLength || headerValue.length() > maxHeaderLength) {
         throw std::length_error("Header field is too long");
      }

      switch (currentState) {
         case HEADER_NAME:
               if (input == ':') {
                  currentState = HEADER_LWS;  // Move to whitespace handling state
               } else if (std::isalnum(input) || input == '-' || input == '_') {
                  headerName.push_back(std::tolower(input));  // Canonicalize header name
               } else {
                  throw std::runtime_error("Invalid character in header name");
               }
               break;
               
         case HEADER_LWS:
               if (input == ' ' || input == '\t') {
                  // Skip leading whitespace in header value
               } else if (input == '\r') {
                  // Do nothing, expect line feed next
               } else if (input == '\n') {
                  // End of line, transition states
                  currentState = HEADER_VALUE;
               } else {
                  headerValue.push_back(input);  // Start of header value
                  currentState = HEADER_VALUE;  // Transition to value state
               }
               break;

         case HEADER_VALUE:
               if (input == '\r') {
                  // Do nothing, expect line feed next
               } else if (input == '\n') {
                  // Remove trailing whitespace
                  headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
                  // Check for existing header and append with a comma if necessary --> Multiple occurences Problem <--
                  if (headers.find(headerName) != headers.end()) {
                     headers[headerName] += ", " + headerValue;
                  } else {
                     headers[headerName] = headerValue;
                  }
                  // Clear temporary storage and reset state
                  headerName.clear();
                  headerValue.clear();
                  currentState = HEADER_END;
               } else {
                  headerValue.push_back(input);
               }
               break;
               
         case HEADER_END:
               if (input == '\n') {
                  currentState = HEADER_NAME;
               } else {
                  throw std::runtime_error("Unexpected character after header line");
               }
               break;
      }
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

#endif /* HTTP_HEADER_FIELDS_PARSER_HPP */