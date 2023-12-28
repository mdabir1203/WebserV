#ifndef REQUEST_PARSER_HPP
# define REQUEST_PARSER_HPP

#include "Server.hpp"
#include "RequestParser.hpp"

#include <map>
#include <string>
#include <cctype>
#include <stdexcept>
#include <set>
#include <cstdlib>

#define HEADER_URI_MAX_LENGTH 8000
#define HEADER_METHOD_MAX_LENGTH 6
#define HEADER_HTTP_VERSION_MAX_LENGTH 8
#define HEADER_HTTP_VERSION_STRING "HTTP/1.1"

enum HTTPHeaderParserState // do not move without changing the function pointer array
{
   HEADER_METHOD,
   HEADER_URI,
   HEADER_HTTP_VERSION,
   HEADER_NAME,
   HEADER_OWS,
   HEADER_VALUE,
   HEADER_PAIR_DONE,
   HEADER_BODY,
   HEADER_END
};

enum HttpMethod
{
	GET,
	POST,
	DELETE,
	UNKNOWN
};

enum ErrorCodes
{
	OK,
	BAD_REQUEST,
	NOT_IMPLEMENTED
};

enum URI_Parsing_State //Dont change order
{
   URI_START,
   URI_SCHEME,
   URI_AUTHORITY,
   URI_PATH,
   URI_QUERY,
   URI_FRAGMENT,
   URI_END
};

class HeaderFieldStateMachine {
public:
   HeaderFieldStateMachine(void);

   int         parseRequestHeaderChunk(const std::string& input);
   void        parseChar(char input);
   void        parseURI(void);

   const std::map<std::string, std::vector<std::string> >& getParsedHeaders() const;
   int getHeaderMethod() const;
   const std::string& getHeaderUriPath() const;
   const bool& getIsHttpVersionRight() const;
   void  setCurrentState(const int state);
   void  setInputPosition(const size_t position);
   void  reset(void);

private:
   const size_t                         maxHeaderLength;  // To prevent buffer overflow attacks
   int                                  currentState;
   std::string                          headerName;
   std::string                          headerValue;
   std::map<std::string, std::vector<std::string> > headers;
   size_t                               positionInInput;
   size_t                               paramterLength;
   char                                 lastChar;

   std::string                          headerUri;
   int                                  headerMethod;
   bool                                 isHttpVersionRight;

   const char**                         headerFieldsForMethod[3]; // [GET, POST, DELETE]

   typedef void (HeaderFieldStateMachine::*StateHandler)(char);
   StateHandler stateTransitionArray[8];// number of transition functions

   //header request line parsing
   void  handleStateHeaderMethod(char c);
   bool  setMethod(const std::string& input);
   void  handleStateHeaderUri(char c);
   void  handleStateHeaderHttpVersion(char c);

   //header fields parsing
   void  stateTransition(int state, int nextState);
   void	handleStateHeaderName(char c);
   void	handleStateHeaderOWS(char c);
   void  handleStateHeaderValue(char c);
   void  handleStateHeaderPairDone(char c);
   void  handleStateHeaderBody(char c);
   void  storeHeaderPair(void);

   //URI parsing
   struct   UriComponents
   {
      std::string scheme;
      std::string authority;
      std::string path;
      std::string query;
      std::string fragment;
   };
   typedef void (HeaderFieldStateMachine::*UriStateHandler)(char);
   UriStateHandler uriStateTransitionArray[6];// number of transition functions

   int               currentUriState;
   size_t            uriIndex;
   UriComponents     uriParts;

   void  throwUriParseError(const std::string message);

   void  uriStateTransition(int state, int nextState);
   void	handleStateUriStart(char c);
   void	handleStateUriScheme(char c);
   void  handleStateUriAuthority(char c);
   void  handleStateUriPath(char c);
   void  handleStateUriQuery(char c);
   void  handleStateUriFragment(char c);
};

#endif /* REQUEST_PARSER_HPP */
