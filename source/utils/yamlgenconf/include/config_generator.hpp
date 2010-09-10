#ifndef CONFIG_GENERATOR_HPP__
#define CONFIG_GENERATOR_HPP__

#include "abstract_config_builder.hpp"
#include "abstract_config_runner.hpp"
#include "interpreter.hpp"
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

template <class TGeneratorStrategy=RegisterConfigStrategy>
class ConfigGenerator
{

private: 
	
	/* Meta-template types need 'typename' here */
	typedef typename TGeneratorStrategy::Builder builder_t; 
	typedef typename TGeneratorStrategy::Runner runner_t; 

	builder_t m_builder;
	Interpreter m_interpreter; 

public: 

	ConfigGenerator(Interpreter const & interpreter) 
	: m_interpreter(interpreter)
	{ 
		m_builder.parse(interpreter); 
	} 

public: 
	
	void run(runner_t & runner) 
	throw (RunnerException, InterpreterException, ConfigException) {
		m_builder.run(runner);
	}

};

#endif /* CONFIG_GENERATOR_HPP__ */
