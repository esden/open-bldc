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

	inline void add_register(const RegisterConfig & register_config) { 
		m_register_configs.push_back(register_config); 
	}
	inline ::std::vector<RegisterConfig> & registers(void) { 
		return m_register_configs;
	}

	inline void set_description(const ::std::string & description) { 
		m_description = description; 
	}
	inline void set_description(yaml_char_t * description) {
		m_description = ::std::string((const char * )(description)); 
	}

	inline void log(void) const { 
		LOG_INFO_PRINT("Register group %s, description: %s", m_name.c_str(), m_description.c_str());
		::std::vector<RegisterConfig>::const_iterator it; 
		::std::vector<RegisterConfig>::const_iterator end = m_register_configs.end(); 
		for(it = m_register_configs.begin(); it != end; ++it) { 
					(*it).log(); 
		}
	}

};

#endif
