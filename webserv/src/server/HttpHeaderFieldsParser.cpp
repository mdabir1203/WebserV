#include "HeaderField.hpp"

HttpHeaderParser::HttpHeaderParser() : state_(START) {}



void HttpHeaderParser::parse(char c) {
	std::cout << "state: " << state_ << std::endl;
  if (state_ ==  START && c == ':') {
      state_ = HEADER_NAME;
  } else if (state_ == HEADER_NAME && (c == '\r' || c == '\n')) {
      if (c == '\r' && !m_header_value.empty() && m_header_value[m_header_value.length() - 1] == '\n') {
          // Folded header
          m_header_value.erase(m_header_value.length() - 1);
          state_ = HEADER_VALUE;
      } else {
          state_ = HEADER_DONE;
      }
  } else if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
      if (state_ == START || state_ == HEADER_DONE) {
          m_header_name += c;
      } else if (state_ == HEADER_VALUE) {
          if (c == '"' && !m_header_value.empty() && m_header_value[m_header_value.length() - 1]!= '\\') {
              // Start or end of a quoted string
              if (m_header_value[m_header_value.length() - 1] == '"') {
                 m_header_value.erase(m_header_value.length() - 1);
              } else {
                 m_header_value += c;
              }
          } else {
              // Handle special characters
              if (c == '\\' && !m_header_value.empty() && m_header_value[m_header_value.length() - 1]== '\\') {
                 m_header_value.erase(m_header_value.length() - 1);
              }
              m_header_value += c;
          }
      }
  }
}

bool HttpHeaderParser::isComplete() {
   // Implement logic to check if parsing is complete
   return state_ == HEADER_DONE;
}

std::string HttpHeaderParser::getHeaderName() {
   // Implement logic to get header name
   return m_header_name;
}

std::string HttpHeaderParser::getHeaderValue() {
   // Implement logic to get header value
   return m_header_value;
}

void HttpHeaderParser::reset() {
   // Implement logic to reset the parser
   state_ = START;
   m_header_name.clear();
   m_header_value.clear();
}