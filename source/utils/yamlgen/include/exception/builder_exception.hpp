#ifndef BUILDER_EXCEPTION_HPP__
#define BUILDER_EXCEPTION_HPP__

#include "config_node.hpp"

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <yaml.h>


class BuilderException : public ::std::exception 
{

private: 
	YAMLContext m_context; 
	const char * m_what; 

public: 
	BuilderException(const char * what) throw()
	: m_what(what)
	{ } 
	BuilderException(const char * what, ConfigNode const & node) throw()
	: m_context(node.context()), m_what(what)
	{ } 

public: 
	virtual ~BuilderException() throw () { } 

public:
	
	virtual const char * what() const throw () { 
		::std::stringstream ss; 
		::std::string s; 
		ss << m_what << " in " << m_context.to_string();
		s = ss.str(); 
		return s.c_str(); 
	}

};

#endif /* BUILDER_EXCEPTION_HPP__ */
