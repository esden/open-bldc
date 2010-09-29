/*
 * olconf - yamlgen based Open-BLDC configuration header generator
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

#ifndef YAML_CONFIG_HPP__
#define YAML_CONFIG_HPP__

#include <yamlgen/logging.hpp>
#include <yamlgen/interpreter.hpp>
#include <yamlgen/exception/interpreter_exception.hpp>
#include <yamlgen/exception/parser_exception.hpp>
#include <yamlgen/exception/builder_exception.hpp>
#include <yamlgen/exception/config_exception.hpp>

#include <yaml.h>


namespace YAMLGen { 
namespace OBLDC { 

class YAMLConfig
{
private: 

	Interpreter m_interpreter; 

public: 

	YAMLConfig() { 
	}

public: 

	void read(char const * filename);

	inline void log(void) const { 
		m_interpreter.log(); 
	}

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif
