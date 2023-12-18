#include "ConfigParser.hpp"

bool ConfigParser::isAllowedWhiteSpace(char c)
{
	return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
}

bool ConfigParser::isAllowedOws(char c)
{
	return (c == ' ' || c == '\t');
}

bool ConfigParser::isCommentStart(char c)
{
	return (lastChar != '\\' && c == '#');
}

bool ConfigParser::isQuoteStart(char c)
{
	return (lastChar != '\\' && c == '"');
}

bool ConfigParser::isUnescapedChar(char expected, char actual)
{
	if (actual == expected && !isQuoteMode && lastChar != '\\')
		return true;
	return false;
}

bool ConfigParser::isAllowedKeyChar(char c)
{
	return (isalnum(c) || c == '_');
}

bool ConfigParser::isAllowedValueChar(char c)
{
	if ((lastChar == '\\' || isQuoteMode) && (c == '{' || c == '}' || c == ' ' || c == '"' || c == ';'))
		return true;
	return (c >= 33 && c <= 126 && c != '{' && c != '}' && c != ';' && c != '"');
}

bool ConfigParser::toggleQuoteMode(char c)
{
	if (isUnescapedChar('"', c))
		isQuoteMode = true;
	else if (c == '"' && isQuoteMode && lastChar != '\\')
		isQuoteMode = false;
	else
		return false;
	return true;
}