#ifndef REGISTER_GROUP_CONFIG_HPP__
#define REGISTER_GROUP_CONFIG_HPP__

#include <string>
#include <vector>
#include "widget_config.h"

class RegisterGroupConfig : public Config
{

private: 
	
	::std::string name; 
	::std::vector<RegisterConfig> m_register_configs; 

public: 

	RegisterGroupConfig() { }

	inline void add_register_config(const RegisterConfig & register_config) { 
		m_widget_configs.push_back(register_config); 
	}
	inline ::std::vector & register_configs(void) { 
		return m_register_configs;
	}

};

#endif
