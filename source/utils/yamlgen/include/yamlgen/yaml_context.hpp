#ifndef YAML_CONTEXT_HPP__
#define YAML_CONTEXT_HPP__

#include <yaml.h>
#include <string>
#include <sstream>

class YAMLContext { 

private: 

	::std::string m_file; 
	int m_line; 
	int m_column; 

public: 

	YAMLContext() 
	: m_file("unknown file"), m_line(0), m_column(0) { } 

	YAMLContext(yaml_event_t * event) 
	: m_file("unknown file") { 
		m_line   = event->start_mark.line; 
		m_column = event->start_mark.column; 
	}
	YAMLContext(yaml_event_t * event, ::std::string const & filename)
	: m_file(filename) { 
		m_line   = event->start_mark.line; 
		m_column = event->start_mark.column; 
	}

public: 

	::std::string const & file(void) const { return m_file; } 
	int line(void) const                   { return m_line; } 
	int column(void) const                 { return m_column; } 

public: 

	::std::string to_string(void) const { 
		::std::stringstream ss; 
		ss << m_file << " "; 
		if(m_line != 0) { 
			ss << "line: " << m_line;
		}
		return ss.str(); 
	}

};

#endif /* YAML_CONTEXT_HPP__ */
