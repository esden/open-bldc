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

#ifndef CONFIG_GENERATOR_HPP__
#define CONFIG_GENERATOR_HPP__

#include <yamlgen/abstract_config_generator.hpp>
#include <yamlgen/abstract_config_builder.hpp>
#include <yamlgen/abstract_config_runner.hpp>
#include <yamlgen/interpreter.hpp>
#include <yamlgen/postprocessor.hpp>
#include <yamlgen/exception/interpreter_exception.hpp>
#include <yamlgen/exception/config_exception.hpp>
#include <yamlgen/exception/runner_exception.hpp>

/*
  Usage: 

		Interpreter interpreter; 
		interpreter.read(yaml_file); 

		ConfigGenerator<RegisterConfigStrategy> generator(interpreter); 
		// generator now only accepts runners derived from 
		// RegisterConfigStrategy::Runner and uses builder 
		// instance of class RegisterConfigStrategy::Builder. 
		
		generator.run(RegisterConfigFooRunner); // implements RegisterConfigRunner
		generator.run(RegisterConfigBarRunner); // implements RegisterConfigRunner

*/

namespace YAMLGen { 

template <class TGeneratorStrategy>
class ConfigGenerator : public AbstractConfigGenerator
{

private: 
	
	/* Meta-template types need 'typename' here */
	typedef typename TGeneratorStrategy::Builder builder_t; 
	typedef typename TGeneratorStrategy::Runner runner_t; 

	builder_t m_builder;

public: 

	ConfigGenerator(Interpreter const & interpreter) 
	{ 
		Postprocessor postprocessor(interpreter.config());
		postprocessor.run(); 
		m_builder.parse(postprocessor.config()); 
	} 
	ConfigGenerator(ConfigNode const & config_node) 
	{ 
		m_builder.parse_partial(config_node);
	} 
	virtual ~ConfigGenerator(void) throw() { } 

public: 
	
	void run(runner_t & runner) {
		m_builder.run(runner);
	}

};

} /* namespace YAMLGen */

#endif /* CONFIG_GENERATOR_HPP__ */
