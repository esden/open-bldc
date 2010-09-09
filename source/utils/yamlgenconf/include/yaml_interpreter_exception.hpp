#ifndef YAML_INTERPRETER_EXCEPTION_HPP__
#define YAML_INTERPRETER_EXCEPTION_HPP__

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <yaml.h>

class YAMLInterpreterException : public ::std::exception 
{

private: 
	yaml_event_t * m_yaml_event; 
	const char * m_what; 

public: 
	YAMLInterpreterException(yaml_event_t * event, const char * what) throw()
	: m_yaml_event(event), m_what(what)
	{ } 

public: 
	virtual ~YAMLInterpreterException() throw () { } 

public:
	
	virtual const char * what() const throw () { 
		::std::string s; 
		::std::stringstream ss; 

		ss << ::std::string(m_what); 
		ss << " (unexpected "; 
		switch(m_yaml_event->type) { 
			case YAML_NO_EVENT: 
				ss << "no event";
				break; 
			case YAML_ALIAS_EVENT: 
				ss << "alias event ";
				break; 
			case YAML_STREAM_START_EVENT: 
				ss << "stream start";
				break; 
			case YAML_STREAM_END_EVENT: 
				ss << "stream end";
				break; 
			case YAML_DOCUMENT_START_EVENT: 
				ss << "document start";
				break; 
			case YAML_DOCUMENT_END_EVENT: 
				ss << "document end";
				break; 
			case YAML_SEQUENCE_START_EVENT: 
				ss << "sequence start";
				break; 
			case YAML_SEQUENCE_END_EVENT: 
				ss << "sequence end";
				break; 
			case YAML_SCALAR_EVENT: 
				ss << "scalar value";
				break; 
			case YAML_MAPPING_START_EVENT: 
				ss << "start of mapping";
				break; 
			case YAML_MAPPING_END_EVENT: 
				ss << "end of mapping";
				break; 
		}
		ss << ")"; 
		ss << ::std::endl;
		s = ss.str(); 

		return s.c_str();
	}

};

#endif /* YAML_INTERPRETER_H__ */
