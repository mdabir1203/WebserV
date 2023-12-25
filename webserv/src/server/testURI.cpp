#include <iostream>
#include <string>
#include <map>
#include "RequestParser.hpp"



bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isSchemeChar(char c) {
    return isAlpha(c) || isDigit(c) || c == '+' || c == '-' || c == '.';
}

// -> Empty, Invalid, Incomplete
 

State transition(State currentState, char c) {
   switch (currentState) {
       case State::START:
           if (isAlpha(c)) return State::SCHEME;
           break;
       case State::SCHEME:
           if (c == ':') return State::HOST;
           if (!isSchemeChar(c)) throw std::invalid_argument("Invalid scheme character");
           break;
       case State::PORT:
           if (isDigit(c)) break;
           if (c == '/') return State::PATH;
           if (!isDigit(c)) throw std::invalid_argument("Invalid port number");
           break;
       case State::PATH:
           if (isDigit(c)) break; // Stay in PORT
           if (c == '/') 
           {
               std::string encoded = percentEncode(c);
           }
           return State::QUERY;
           if (!isDigit(c)) throw std::invalid_argument("Invalid path character");
           break;
       case State::QUERY:
           if (c == '#') return State::FRAGMENT;
           // Additional QUERY logic
           break;
       default:
           break;
   }
   return currentState;
}

std::string URIParser::percentEncode(char c)
{
   std::ostringstream oss;
   oss << '%' << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(c);
   return oss.str();
}


void URIParser::parse(const std::string& uri) {
   if (uri.empty()) {
      throw std::invalid_argument("Empty URI");
   }

   State currentState = State ::START;
   size_t i = 0;
   while (i < uri.size()) {
      currentState = transition(currentState, uri[i]);
      if (currentState == State::INVALID) {
         throw std::invalid_argument("Invalid URI");
      }
      i++;
   }
   if (currentState != State::FRAGMENT) {
      throw std::invalid_argument("Incomplete URI");
   }

}
int main() {
    URIParser parser("http://www.example.com/path?param1=value1&param2=value2");
    // Test by retrieving parts of the URI and printing them
    return 0;
}