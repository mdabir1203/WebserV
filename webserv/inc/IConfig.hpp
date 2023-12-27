#ifndef I_CONFIG_HPP
# define I_CONFIG_HPP

#include <string>

class IConfig
{
	public:
		virtual ~IConfig() {}
		virtual void setClientMaxBodySize(const size_t& value) = 0;
};

#endif /* I_CONFIG_HPP */