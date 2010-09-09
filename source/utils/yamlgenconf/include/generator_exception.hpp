#ifndef GENERATOR_EXCEPTION_HPP__
#define GENERATOR_EXCEPTION_HPP__

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <yaml.h>

class GeneratorException : public ::std::exception 
{

private: 
	const char * m_what; 

public: 
	GeneratorException(const char * what) throw()
	: m_what(what)
	{ } 

public: 
	virtual ~GeneratorException() throw () { } 

public:
	
	virtual const char * what() const throw () { 
		return m_what; 
	}

};

#endif /* GENERATOR_EXCEPTION_HPP__ */
