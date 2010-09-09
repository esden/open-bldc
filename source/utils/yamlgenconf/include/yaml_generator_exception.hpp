#ifndef YAML_GENERATOR_EXCEPTION_HPP__
#define YAML_GENERATOR_EXCEPTION_HPP__

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <yaml.h>

class YAMLGeneratorException : public ::std::exception 
{

private: 
	const char * m_what; 

public: 
	YAMLGeneratorException(const char * what) throw()
	: m_what(what)
	{ } 

public: 
	virtual ~YAMLGeneratorException() throw () { } 

public:
	
	virtual const char * what() const throw () { 
		return m_what; 
	}

};

#endif /* YAML_GENERATOR_EXCEPTION_HPP__ */
