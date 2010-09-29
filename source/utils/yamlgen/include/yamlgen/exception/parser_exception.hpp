/*
 * yamlgen - YAML to everything generator framework
 * Copyright (C) 2010 by Tobias Fuchs <twh.fuchs@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PARSER_EXCEPTION_HPP__
#define PARSER_EXCEPTION_HPP__

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <yaml.h>


namespace YAMLGen { 

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

} /* namespace YAMLGen */

#endif
