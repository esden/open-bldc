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

#include <olconf/yaml_config.hpp>
#include <olconf/module_config_strategy.hpp>
#include <olconf/module_config_header_runner.hpp>

#include <yamlgen/logging.hpp>
#include <yamlgen/config_generator.hpp>
#include <yamlgen/exception/interpreter_exception.hpp>
#include <yamlgen/exception/parser_exception.hpp>
#include <yamlgen/exception/builder_exception.hpp>
#include <yamlgen/exception/config_exception.hpp>


namespace YAMLGen { 
namespace OBLDC { 

void
YAMLConfig::read(char const * filename) 
{
	m_interpreter.read(filename); 

	ConfigGenerator<ModuleConfigStrategy> generator(m_interpreter); 

	
	ModuleConfigHeaderRunner runner_strategy; 
	generator.run(runner_strategy); 

/* 
	Note that a ConfigGenerator can parse several 
	interpreters, and you can run several runners 
	on the same configurator: 

		config_generator.parse(first_interpreter); 
		config_generator.parse(second_interpreter); 

		config_generator.run(first_strategy); 
		config_generator.run(second_strategy); 
		config_generator.run(third_strategy); 
*/

}

} /* namespace OBLDC */
} /* namespace YAMLGen */
