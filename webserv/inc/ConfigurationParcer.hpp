/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParcer.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 18:50:54 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/15 21:50:25 by aputiev          ###   ########.fr       */
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
    int handleGlobalVars(std::string &token, int specifier);
    int handleServerVarPort(std::string &str);
    std::string handleServerVarName(std::string &input);
    bool directoryExists(const std::string& path, int specifier);
    int checkCodeErrorPage(std::string &str);
    bool checkFileExist(std::string &filePath, int specifier);
    bool checkIfServerDataEnough(t_serv& currentSever);
	void check_for_double_location(t_serv& currentServer, std::string location_name);
    
};

#endif
