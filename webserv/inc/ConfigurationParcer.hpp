/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationParcer.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aputiev <aputiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 18:50:54 by aputiev           #+#    #+#             */
/*   Updated: 2023/11/08 16:56:37 by aputiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONPARCER_HPP
# define CONFIGURATIONPARCER_HPP

#include <iostream>
#include "Common_header.hpp"

class ConfigurationParcer
{
    private:
    
        std::vector<std::string> readConfiguration(std::string config_file_name);
        
        t_serv tokens_processing(std::vector<std::strings>& tokens, size_t & pos); 
        
    public:

        ConfigurationParcer();
        ~ConfigurationParcer();
        
        int configParsing(std::string config_file, std::vector<t_serv> & servers);
    };


#endif
