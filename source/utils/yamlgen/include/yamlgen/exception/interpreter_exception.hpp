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

#ifndef INTERPRETER_EXCEPTION_HPP__
#define INTERPRETER_EXCEPTION_HPP__

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <yaml.h>


namespace YAMLGen { 

class InterpreterException : public ::std::exception 
{

private: 
	int m_yaml_event; 
	const char * m_what; 

public: 
	InterpreterException(yaml_event_t * event, const char * what) throw()
	: m_yaml_event(event->type), m_what(what)
	{ } 

	InterpreterException(const char * what) throw()
	: m_yaml_event(0), m_what(what)
	{ } 

public: 
	virtual ~InterpreterException() throw () { } 

public:
	
	virtual const char * what() const throw () { 
		::std::string s; 
		::std::stringstream ss; 

		ss << ::std::string(m_what); 
		ss << ": (unexpected "; 
		switch(m_yaml_event) { 
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
			default: 
				ss << "unknown event"; 
				break; 
		}
		ss << ")"; 
		ss << ::std::endl;
		s = ss.str(); 

		return s.c_str();
	}

};

} /* namespace YAMLGen */

#endif /* INTERPRETER_EXCEPTION_HPP__ */
