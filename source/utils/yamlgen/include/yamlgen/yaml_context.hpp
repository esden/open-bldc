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
