#ifndef MODULE_CONFIG_HPP__
#define MODULE_CONFIG_HPP__

#include <olconf/register_config.hpp>
#include <olconf/register_config_builder.hpp>
#include <olconf/flag_config.hpp>
#include <olconf/flag_config_builder.hpp>
#include <olconf/define_config_builder.hpp>
#include <olconf/define_config.hpp>

#include <yamlgen/config_node.hpp>


namespace YAMLGen { 
namespace OBLDC { 

class ModuleConfig
{

private: 

	::std::string m_name; 
	ConfigNode m_config_node; 

private: 

	::std::vector<RegisterGroupConfig> m_registers;
	::std::vector<FlagConfig> m_flags; 
	::std::vector<DefineConfig> m_defines; 

public: 

	::std::vector<RegisterGroupConfig> & registers() { 
		return m_registers;
	}
	::std::vector<FlagConfig> & flags() { 
		return m_flags;
	}
	::std::vector<DefineConfig> & defines() { 
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
		::std::vector<RegisterGroupConfig>::const_iterator reg_it; 
		::std::vector<RegisterGroupConfig>::const_iterator reg_end; 
		reg_end = m_registers.end(); 
		for(reg_it = m_registers.begin(); reg_it != reg_end; ++reg_it) { 
			(*reg_it).log(); 
		}
	}

public: 

	void add(RegisterConfigBuilder const & builder) { 
		::std::vector<RegisterGroupConfig> added = builder.register_groups();
		m_registers.insert(m_registers.end(), added.begin(), added.end());
	}
	void add(DefineConfigBuilder const & builder) { 
		::std::vector<DefineConfig> added = builder.defines();
		m_defines.insert(m_defines.end(), added.begin(), added.end());
	}
	void add(FlagConfigBuilder const & builder) { 
		::std::vector<FlagConfig> added = builder.flags();
		m_flags.insert(m_flags.end(), added.begin(), added.end());
	}

public: 

	::std::string const & name(void) const { return m_name; }
	ConfigNode const & config(void) const  { return m_config_node; } 

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif
