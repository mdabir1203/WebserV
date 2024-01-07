#ifndef CONFIG_PARSER_HPP_INCLUDED
# define CONFIG_PARSER_HPP_INCLUDED

class WebServerConfig;
class LocationConfig;
class ServerConfig;

#include <string>
#include <vector>
#include <map>
#include <stdint.h>

#define CONFIG_PARSER_BUFFER_SIZE 1000
#define CONFIG_PARSER_MAX_KEY_LENGTH 100
#define CONFIG_PARSER_MAX_VALUE_LENGTH 1000

enum ParseState
{
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
        WebServerConfig*  webServerConfig;

    private:
        void        throwConfigError(const std::string& message, char offendingChar, const std::string& offendingString, bool givePosition);

        void        parseChar(char c);

        bool        isServerBlockValid();
        bool        isLocationBlockValid();


        typedef void (ConfigParser::*StateHandler)(char);
        StateHandler stateTransitionArray[8];// number of trnasition fuctions

        void  stateTransition(int state, int nextState);

        void        handleStateKey(char c);
        void        handleStateOws(char c);
        void	    handleStateWs(char c);
        void        handleStateComment(char c);
        void	    handleStateValue(char c);
        void        handleStateLocation(char c);

        void	    handleKeyValuePair(void);

        

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
        bool        isUnescapedChar(char expected, char actual);
        bool        toggleQuoteMode(char c);

        std::string&	extractSingleValueFromValueVector(const bool isRequired);
        //std::string&    extractNextValueFromValueVector(const bool isRequired);//<--

        //handlers
        void	handleClientMaxBodySize();
        void	handleListen();
        void	handleServerName();
        void    handleErrorPage();
        void    handleDefaultErrorPage();
        void    handleRoot();
        void    handleLocationPath();
        void    handleIndex();
        void    handleCgiExtension();
        void    handleUploadStore();
        void    handleReturn();
        void    handleMethods();
        void    handleAutoindex();

        void    validateLocationConfig(LocationConfig* currentLocationConfig);
        void    validateServerConfig(ServerConfig* currentServerConfig);
        
        
        uint32_t    extractIp(std::istringstream& iss);
        uint16_t    extractPort(std::istringstream& iss);

        typedef void (ConfigParser::*HandlerFunction)(void);
        void        validateAndHandleKey(void);
        void        validateKeyAndCallHandler(std::map<std::string, std::pair<int, HandlerFunction> >& keys);
        void        resetKeyCounts(std::map<std::string, std::pair<int, HandlerFunction> >& keys);

        
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

#endif /* CONFIG_PARSER_HPP_INCLUDED */
