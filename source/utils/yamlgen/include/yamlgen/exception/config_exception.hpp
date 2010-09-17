#ifndef CONFIG_EXCEPTION_HPP__
#define CONFIG_EXCEPTION_HPP__

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <yamlgen/yaml_context.hpp>
#include <yaml.h>


namespace YAMLGen {

class ConfigException : public ::std::exception 
{

protected: 
	const char * m_what; 
	YAMLContext m_context; 

public: 
	ConfigException(const char * what) throw()
	: m_what(what)
	{ } 
	
	ConfigException(const char * what, YAMLContext const & context) throw()
	: m_what(what), m_context(context)
	{ } 

public: 
	virtual ~ConfigException() throw () { } 

public:
	
	virtual const char * what() const throw () { 
		::std::stringstream ss; 
		::std::string s; 
		ss << m_what << " in " << m_context.to_string();
		s = ss.str(); 
		return s.c_str(); 
	}

};

} /* namespace YAMLGen */

#endif /* CONFIG_EXCEPTION_HPP__ */
