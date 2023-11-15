#ifndef HEADER_FIELDS_HPP
# define HEADER_FIELDS_HPP


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
   HeaderFieldStateMachine(size_t maxLength = 8192) : currentState(HEADER_NAME), maxHeaderLength(maxLength) {}

   void parseChar(char input) {
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
                  // Check for existing header and append with a comma if necessary
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

    const std::map<std::string, std::string>& getParsedHeaders() const {
        return headers;
    }
   int currentState;

   std::map<std::string, std::string> headers;
private:

   std::string headerName;
   std::string headerValue;
   const size_t maxHeaderLength;  // To prevent buffer overflow attacks

};

#endif /* HTTP_HEADER_FIELDS_PARSER_HPP */