#ifndef MODULE_CONFIG_HPP__
#define MODULE_CONFIG_HPP__

#include "config_node.hpp"


namespace YAMLGen { 
namespace OBLDC { 

class ModuleConfig
{

private: 

	::std::string m_name; 
	ConfigNode m_config_node; 

public: 

	ModuleConfig(::std::string const & name, 
							 ConfigNode const & config_node) 
	: m_name(name), m_config_node(config_node) 
	{ } 

public: 

	::std::string const & name(void) const { return m_name; }
	ConfigNode const & config(void) const  { return m_config_node; } 

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif
