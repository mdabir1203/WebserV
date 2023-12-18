#include "RequestParser.hpp"
#include <stdlib.h>

//TODO: decide for size limit of header fields
HeaderFieldStateMachine::HeaderFieldStateMachine(void)
                        : maxHeaderLength(8192), currentState(HEADER_METHOD),
                        positionInInput(0), paramterLength(0), lastChar('\0'),
                        headerMethod(0), isHttpVersionRight(false)
{
   stateTransitionArray[0] = &HeaderFieldStateMachine::handleStateHeaderMethod;
   stateTransitionArray[1] = &HeaderFieldStateMachine::handleStateHeaderUri;
   stateTransitionArray[2] = &HeaderFieldStateMachine::handleStateHeaderHttpVersion;
   stateTransitionArray[3] = &HeaderFieldStateMachine::handleStateHeaderName;
   stateTransitionArray[4] = &HeaderFieldStateMachine::handleStateHeaderOWS;
   stateTransitionArray[5] = &HeaderFieldStateMachine::handleStateHeaderValue;
   stateTransitionArray[6] = &HeaderFieldStateMachine::handleStateHeaderPairDone;
   stateTransitionArray[7] = &HeaderFieldStateMachine::handleStateHeaderBody;
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

// Function to decode percent-encoded characters in a URI
char decodePercentEncodedChar(const std::string& uri, size_t& index) {
   // Extracting the next two characters as a hexadecimal string
   std::string hex = uri.substr(index + 1, 2);

   // Convert the hexadecimal string to an integer, and cast it to a char
   char decodedChar = static_cast<char>(strtol(hex.c_str(), NULL, 16));

   // Advancing the index by 2, since we've processed two additional characters
   index += 2;

   return decodedChar;
}

static bool isValidSchemeChar(char c) {
    return isalnum(c) || c == '+' || c == '-' || c == '.';
}


static bool isValidPathChar(char c) {
    return isalnum(c) || c == '-' || c == '.' || c == '_' || c == '~' || c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' || c == '*' || c == '+' || c == ',' || c == ';' || c == '=' || c == ':' || c == '@' || c == '/';
}

static bool isValidQueryOrFragmentChar(char c) {
    return isValidPathChar(c) || c == '?' || c == '#';
}

// Function to parse a URI
std::string HeaderFieldStateMachine::parseURI(const std::string& uri)
{
    int state = URI_START;
    std::string parsedURI, scheme, authority;
    size_t index = 0;

    while (index < uri.length()) {
        char c = uri[index];

        // Handle percent-encoded characters
        if (c == '%') {
            c = decodePercentEncodedChar(uri, index);
        }

        switch (state) {
            case URI_START:
                if (isalnum(c)) {
                    state = URI_SCHEME;
                    scheme += c;
                } else {
                    throw std::runtime_error("Invalid start of URI");
                }
                break;
            case URI_SCHEME:
                if (c == ':') {
                    if (scheme.empty()) throw std::runtime_error("Invalid scheme");
                    if (uri.substr(index, 3) == "://") {
                        index += 2; // Skip '://'
                        state = URI_AUTHORITY;
                    } else {
                        throw std::runtime_error("Invalid scheme format");
                    }
                } else if (isValidSchemeChar(c)) {
                    scheme += c;
                } else {
                    throw std::runtime_error("Invalid character in scheme");
                }
                break;
            case URI_AUTHORITY:
                if (c == '/') {
                    state = URI_PATH;
                    parsedURI = scheme + "://" + authority;
                } else {
                    authority += c;
                }
                break;
            case URI_PATH:
                if (c == '?') {
                    state = URI_QUERY;
                } else if (c == '#') {
                    state = URI_FRAGMENT;
                } else if (isValidPathChar(c)) {
                    parsedURI += c;
                } else {
                    throw std::runtime_error("Invalid character in path");
                }
                break;
            case URI_QUERY:
                if (c == '#') {
                    state = URI_FRAGMENT;
                } else if (isValidQueryOrFragmentChar(c)) {
                    parsedURI += c;
                } else {
                    throw std::runtime_error("Invalid character in query");
                }
                break;
            case URI_FRAGMENT:
                if (isValidQueryOrFragmentChar(c)) {
                    parsedURI += c;
                } else {
                    throw std::runtime_error("Invalid character in fragment");
                }
                break;
            default:
               break;
         }

        // Advance to the next character
        index++;

        // Transition to END state if we've reached the end of the string
        if (index == uri.length()) {
            state = URI_END;
        }
    }

    // Check if we successfully reached the END state
    if (state == URI_END) {
      return parsedURI;
    }
    else {
      throw std::runtime_error("Incomplete URI parsing");
   }

    return parsedURI;
}

const std::map<std::string, std::vector<std::string> >& HeaderFieldStateMachine::getParsedHeaders() const
{
   return headers;
}

int HeaderFieldStateMachine::getHeaderMethod() const
{
   return headerMethod;
}

const std::string& HeaderFieldStateMachine::getHeaderUri() const
{
   return headerUri;
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
