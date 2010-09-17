#ifndef BUILDER_EXCEPTION_HPP__
#define BUILDER_EXCEPTION_HPP__

#include <yamlgen/config_node.hpp>
#include <yamlgen/exception/config_exception.hpp>

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <yamlgen/yaml_context.hpp>
#include <yamlgen/exception/config_exception.hpp>
#include <yaml.h>


namespace YAMLGen { 

class BuilderException : public ConfigException
{

private: 
	const char * m_what; 

public: 
	BuilderException(const char * what) throw()
	: ConfigException(what)
	{ }

	BuilderException(const char * what, YAMLContext const & context) throw()
	: ConfigException(what, context)
	{ } 

public: 
	virtual ~BuilderException() throw () { } 

};

} /* namespace YAMLGen */

#endif /* BUILDER_EXCEPTION_HPP__ */
