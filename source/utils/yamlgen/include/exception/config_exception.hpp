#ifndef CONFIG_EXCEPTION_HPP__
#define CONFIG_EXCEPTION_HPP__

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <yaml.h>


namespace YAMLGen {

class ConfigException : public ::std::exception 
{

private: 
	const char * m_what; 

public: 
	ConfigException(const char * what) throw()
	: m_what(what)
	{ } 

public: 
	virtual ~ConfigException() throw () { } 

public:
	
	virtual const char * what() const throw () { 
		return m_what; 
	}

};

} /* namespace YAMLGen */

#endif /* CONFIG_EXCEPTION_HPP__ */
