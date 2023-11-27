#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

/* ====== std lib header files ====== */
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <sstream>
#include <exception>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <set>

// #include "Colors.hpp"
// #include "Exceptions.hpp"
#include "WebServerConfig.hpp"

#define CONFIG_PARSER_BUFFER_SIZE 1000
#define CONFIG_PARSER_MAX_KEY_LENGTH 100
#define CONFIG_PARSER_MAX_VALUE_LENGTH 1000

enum ParseState
{
    CONFIG_PARSER_STATE_START,
    CONFIG_PARSER_STATE_KEY,
    CONFIG_PARSER_STATE_WS, // white space -> ' ' or '\t' or '\n' or '\r'
    CONFIG_PARSER_STATE_OWS, // optional white space -> ' ' or '\t'
    CONFIG_PARSER_STATE_VALUE,
    CONFIG_PARSER_STATE_SERVER,
    CONFIG_PARSER_STATE_LOCATION,
    CONFIG_PARSER_STATE_COMMENT,
    CONFIG_PARSER_STATE_NUM_STATES
};

class ConfigParser
{
    public:
        ConfigParser(WebServerConfig* webServerConfig);
        ~ConfigParser();
        
        void             parseConfig(const std::string& configPath);
        WebServerConfig* getWebServerConfig(void) const;

    private:
        void        throwConfigError(const std::string& message, char offendingChar, const std::string& offendingString, bool givePosition);

        void        parseChar(char c);

        bool        isServerBlockValid();
        bool        isLocationBlockValid();


        typedef void (ConfigParser::*StateHandler)(char);
        StateHandler stateTransitionArray[8];// number of trnasition fuctions

        void  stateTransition(int state, int nextState);

        void        handleStateStart(char c);
        void        handleStateKey(char c);
        void        handleStateOws(char c);
        void	    handleStateWs(char c);
        void        handleStateComment(char c);
        void	    handleStateValue(char c);
        void        handleStateLocation(char c);

        //string managers
        void        addCharToKey(char c);
        void        addCharToValue(char c);

        //helpers
        bool        isAllowedOws(char c);
        bool        isAllowedWhiteSpace(char c);
        bool        isCommentStart(char c);
        bool        isQuoteStart(char c);
        bool        isAllowedKeyChar(char c);
        bool        isAllowedValueChar(char c);
        
        void        doNothing(void);

        std::string&	extractSingleValueFromValueVector(const bool isRequired);

        //handlers
        void	handleClientMaxBodySize();
        void	handleListen();
        void	handleServerName();

        uint32_t ipStringToNumber(const std::string& ip);
        std::string ipNumberToString(uint32_t ip);
        uint16_t ip_port_to_uint16(const std::string& ip_port) ;
        std::string uint16_to_ip_port(uint16_t port);




        typedef void (ConfigParser::*HandlerFunction)(void);
        void        validateAndHandleKey(void);
        void        validateKeyAndCallHandler(std::map<std::string, std::pair<int, HandlerFunction> >& keys);
        void        resetKeyCounts(std::map<std::string, std::pair<int, HandlerFunction> >& keys);

        WebServerConfig*  webServerConfig;
        ServerConfig*     currentServerConfig;
        LocationConfig*   currentLocationConfig;
        int               currentState;
        int               previousState;
        std::string       key;
        std::string       value;
        std::vector<std::string> mulitValues;
        char              lastChar; //reset after comment ends and after '\\' // throw an error if '\\' is last char in file
        bool              isQuoteMode;
        size_t            lineCount; // for error messages
        size_t            charCount; // for error messages
        size_t            paramterLength;

        std::map<std::string, std::pair<int, HandlerFunction> > httpKeys;
        std::map<std::string, std::pair<int, HandlerFunction> > serverKeys;
        std::map<std::string, std::pair<int, HandlerFunction> > locationKeys;

};

#endif /* CONFIG_PARSER_HPP */
