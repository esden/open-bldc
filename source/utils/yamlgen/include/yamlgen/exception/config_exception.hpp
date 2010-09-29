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

#ifndef CONFIG_EXCEPTION_HPP__
#define CONFIG_EXCEPTION_HPP__

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <yamlgen/yaml_context.hpp>
#include <yaml.h>


namespace YAMLGen {

class ConfigException : public ::std::exception 
{

protected: 
	const char * m_what; 
	YAMLContext m_context; 

public: 
	ConfigException(const char * what) throw()
	: m_what(what)
	{ } 
	
	ConfigException(const char * what, YAMLContext const & context) throw()
	: m_what(what), m_context(context)
	{ } 

public: 
	virtual ~ConfigException() throw () { } 

public:
	
	virtual const char * what() const throw () { 
		::std::stringstream ss; 
		::std::string s; 
		ss << m_what << " in " << m_context.to_string();
		s = ss.str(); 
		return s.c_str(); 
	}

};

} /* namespace YAMLGen */

#endif /* CONFIG_EXCEPTION_HPP__ */
