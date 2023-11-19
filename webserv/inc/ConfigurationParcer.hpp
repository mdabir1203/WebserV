/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParcer.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 18:50:54 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/19 13:12:05 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONPARCER_HPP
# define CONFIGURATIONPARCER_HPP

#include <iostream>
#include "Common_header.hpp"
using namespace std;

class ConfigurationParser
{
public:
    ConfigurationParser();
    ~ConfigurationParser();
      
    std::vector<t_serv> parseConfig(int ac, char **av);
    

private:
    void checkConfigFile(std::string filename); 
    void parseLine(const std::string& line, t_serv& currentServer, std::vector<t_serv>& servers, ParseState& state);
    int handleServerVarPort(std::istringstream& iss, std::string &token);
    std::string handleServerVarName(std::istringstream& iss, std::string &input);
    bool directoryExists(const std::string& path, int specifier);
    int checkCodeErrorPage(std::istringstream& iss, std::string &str);
    bool checkFileExist(const std::string &filePath, int specifier);
    bool checkIfServerDataEnough(t_serv& currentSever);
	std::string check_for_double_location(std::istringstream& iss, t_serv& currentServer);
    void check_is_token_allowed(std::string &token);
    int handleGlobalSettings(std::istringstream& iss, std::string &token, int specifier);
    std::string checkToken(std::istringstream& iss, std::string &token, bool check_empty);
    std::vector<std::string> handleCgiExt(std::istringstream& iss);
    
};

#endif
