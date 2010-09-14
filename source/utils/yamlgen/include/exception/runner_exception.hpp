#ifndef RUNNER_EXCEPTION_HPP__
#define RUNNER_EXCEPTION_HPP__

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <yaml.h>

class RunnerException : public ::std::exception 
{

private: 
	const char * m_what; 

public: 
	RunnerException(const char * what) throw()
	: m_what(what)
	{ } 

public: 
	virtual ~RunnerException() throw () { } 

public:
	
	virtual const char * what() const throw () { 
		return m_what; 
	}

};

#endif /* RUNNER_EXCEPTION_HPP__ */
