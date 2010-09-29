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

#ifndef BUILDER_EXCEPTION_HPP__
#define BUILDER_EXCEPTION_HPP__

#include <yamlgen/config_node.hpp>
#include <yamlgen/exception/config_exception.hpp>

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <yamlgen/yaml_context.hpp>
#include <yamlgen/exception/config_exception.hpp>
#include <yaml.h>


namespace YAMLGen { 

class BuilderException : public ConfigException
{

private: 
	const char * m_what; 

public: 
	BuilderException(const char * what) throw()
	: ConfigException(what)
	{ }

	BuilderException(const char * what, YAMLContext const & context) throw()
	: ConfigException(what, context)
	{ } 

public: 
	virtual ~BuilderException() throw () { } 

};

} /* namespace YAMLGen */

#endif /* BUILDER_EXCEPTION_HPP__ */
