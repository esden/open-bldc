#ifndef DEFINE_CONFIG_HPP__
#define DEFINE_CONFIG_HPP__

#include <olconf/define_config_strategy.hpp>

#include <yamlgen/abstract_config_builder.hpp>

#include <vector>
#include <algorithm>
#include <cctype>


namespace YAMLGen { 
namespace OBLDC { 

class DefineConfig 
{

private: 

	::std::string m_name; 
	::std::string m_value; 

public: 

	DefineConfig(::std::string const & name, ::std::string const & value) 
	: m_name(name), m_value(value) 
	{ }

public: 

	::std::string const & name(void) const { return m_name; } 
	::std::string const & value(void) const { return m_value; } 

};

} /* namespace ODLDC */
} /* namespace YAMLGen */

#endif /* DEFINE_CONFIG_HPP__ */
