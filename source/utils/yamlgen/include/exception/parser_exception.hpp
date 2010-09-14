#ifndef PARSER_EXCEPTION_HPP__
#define PARSER_EXCEPTION_HPP__

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <yaml.h>

class ParserException : public ::std::exception 
{

private: 
	yaml_parser_t * m_yaml_parser; 

public: 
	ParserException(yaml_parser_t * parser) throw()
	: m_yaml_parser(parser)
	{ } 

public: 
	virtual ~ParserException() throw () { } 

public:
	
	virtual const char * what() const throw () { 
		::std::string s; 
		::std::stringstream ss; 

		switch(m_yaml_parser->error) {
			case YAML_MEMORY_ERROR: 
				ss << "Memory error";
				break;
			case YAML_READER_ERROR:
				ss << "Reader error ";
				ss << m_yaml_parser->problem; 
				ss << " at line " << m_yaml_parser->problem_offset;
				if (m_yaml_parser->problem_value != -1) {
					ss << m_yaml_parser->problem_value; 
				}
				break; 
			case YAML_SCANNER_ERROR:
				ss << "Parser error";
				// passthrough
			case YAML_PARSER_ERROR: 
				ss << m_yaml_parser->problem;
				ss << " at line " << m_yaml_parser->problem_mark.line+1; 
				ss << " in column " << m_yaml_parser->problem_mark.column+1; 
				if(m_yaml_parser->context) { 
					ss << ", at line " << m_yaml_parser->context_mark.line+1; 
					ss << " in column " << m_yaml_parser->context_mark.column+1; 
				}
				break; 
			default: 
				ss << "Internal error";
				break; 
		}
		s = ss.str(); 
		return s.c_str(); 
	}

};

#endif
