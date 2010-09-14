#ifndef CONFIG_GENERATOR_HPP__
#define CONFIG_GENERATOR_HPP__

#include "abstract_config_generator.hpp"
#include "abstract_config_builder.hpp"
#include "abstract_config_runner.hpp"
#include "interpreter.hpp"
#include "postprocessor.hpp"
#include "register_config_strategy.hpp"
#include "exception/interpreter_exception.hpp"
#include "exception/config_exception.hpp"
#include "exception/runner_exception.hpp"

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
