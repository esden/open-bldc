#ifndef REGISTER_GROUP_CONFIG_HPP__
#define REGISTER_GROUP_CONFIG_HPP__

#include <string>
#include <vector>

#include "register_config.hpp"

class RegisterGroupConfig : public Config
{

private: 
	
	::std::string m_name; 
	::std::vector<RegisterConfig> m_register_configs; 

	::std::string m_description;

public: 

	RegisterGroupConfig() 
	: m_name(""), m_description("")	{ }

	RegisterGroupConfig(::std::string & name)
	: m_name(name), m_description("") { }

	RegisterGroupConfig(yaml_char_t * name)	{ 
		m_name = ::std::string((const char *)(name));
	}

	~RegisterGroupConfig() { }

	inline void add_register_config(const RegisterConfig & register_config) { 
		m_register_configs.push_back(register_config); 
	}
	inline ::std::vector<RegisterConfig> & register_configs(void) { 
		return m_register_configs;
	}

	inline void set_description(const ::std::string & description) { 
		m_description = description; 
	}
	inline void set_description(yaml_char_t * description) {
		m_description = ::std::string((const char * )(description)); 
	}


};

#endif
