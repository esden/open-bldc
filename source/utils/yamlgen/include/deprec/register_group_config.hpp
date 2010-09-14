#ifndef REGISTER_GROUP_CONFIG_HPP__
#define REGISTER_GROUP_CONFIG_HPP__

#include "property_config.hpp"
#include "register_config.hpp"

#include <string>
#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

class RegisterGroupConfig : public PropertyConfig
{

private: 
	
	::std::string m_description;
	::std::vector<RegisterConfig> m_register_configs; 

public: 

	RegisterGroupConfig() 
	: m_description("")	{ }

	RegisterGroupConfig(::std::string const & name)
	: PropertyConfig(name), m_description("") { }

	~RegisterGroupConfig() { }

	inline void add_register(const RegisterConfig & register_config) { 
		m_register_configs.push_back(register_config); 
	}
	inline ::std::vector<RegisterConfig> const & registers(void) const { 
		return m_register_configs;
	}

	inline void set_description(const ::std::string & description) { 
		m_description = description; 
	}
	inline void set_description(yaml_char_t * description) {
		m_description = ::std::string((const char * )(description)); 
	}

	inline void log(void) const { 
		LOG_INFO_PRINT("|- Register group %s", m_name.c_str());
		
		PropertyConfig::log();

		::std::vector<RegisterConfig>::const_iterator it; 
		::std::vector<RegisterConfig>::const_iterator end = m_register_configs.end(); 
		for(it = m_register_configs.begin(); it != end; ++it) { 
			(*it).log(); 
		}
	}
	
};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif
