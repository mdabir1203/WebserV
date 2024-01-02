#ifndef I_CONFIG_HPP_INCLUDED
#define I_CONFIG_HPP_INCLUDED

class IConfig
{
public:
	virtual ~IConfig() {}
	virtual void setClientMaxBodySize(const size_t &value) = 0;
};

#endif
