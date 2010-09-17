#ifndef MODULE_CONFIG_HPP__
#define MODULE_CONFIG_HPP__

#include <olconf/register_config_builder.hpp>
#include <olconf/flag_config_builder.hpp>
#include <olconf/define_config_builder.hpp>

#include <yamlgen/config_node.hpp>


namespace YAMLGen { 
namespace OBLDC { 

class ModuleConfig
{

private: 

	::std::string m_name; 
	ConfigNode m_config_node; 

private: 

	::std::vector<RegisterConfigBuilder> m_registers;
	::std::vector<FlagConfigBuilder> m_flags; 
	::std::vector<DefineConfigBuilder> m_defines; 

public: 

	::std::vector<RegisterConfigBuilder> & registers() { 
		return m_registers;
	}
	::std::vector<FlagConfigBuilder> & flags() { 
		return m_flags;
	}
	::std::vector<DefineConfigBuilder> & defines() { 
		return m_defines;
	}

public: 

	ModuleConfig(::std::string const & name, 
							 ConfigNode const & config_node) 
	: m_name(name), m_config_node(config_node) 
	{ } 

public: 

	inline void log(void) const { 
		LOG_INFO_PRINT("|- Module");
		::std::vector<RegisterConfigBuilder>::const_iterator reg_it; 
		::std::vector<RegisterConfigBuilder>::const_iterator reg_end; 
		reg_end = m_registers.end(); 
		for(reg_it = m_registers.begin(); reg_it != reg_end; ++reg_it) { 
			(*reg_it).log(); 
		}
	}

public: 

	void add(RegisterConfigBuilder const & builder) { 
		m_registers.push_back(builder); 
	}
	void add(DefineConfigBuilder const & builder) { 
		m_defines.push_back(builder); 
	}
	void add(FlagConfigBuilder const & builder) { 
		m_flags.push_back(builder); 
	}

public: 

	::std::string const & name(void) const { return m_name; }
	ConfigNode const & config(void) const  { return m_config_node; } 

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif
