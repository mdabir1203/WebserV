/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParcer.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 18:50:54 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/13 14:39:54 by aputiev          ###   ########.fr       */
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
      
    std::vector<t_serv> parseConfig(const std::string& filename);
    

private:
    void checkConfigFile(std::string filename); 
    void parseLine(const std::string& line, t_serv& currentServer, std::vector<t_serv>& servers, ParseState& state);
    void handleGlobalVars(std::string &token, int specifier);
    int handleServerVarPort(std::string &str);
    std::string handleServerVarName(std::string &input);
    bool directoryExists(const std::string& path, int specifier);
    int checkCodeErrorPage(std::string &str);
    bool checkFileExist(std::string &filePath, int specifier);
    
};

#endif
