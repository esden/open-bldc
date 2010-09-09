#ifndef CONFIG_GENERATOR_HPP__
#define CONFIG_GENERATOR_HPP__

#include "abstract_generator_strategy.hpp"
#include "abstract_runner_strategy.hpp"
#include "interpreter_exception.hpp"
#include "interpreter.hpp"

class ConfigGenerator
{

private: 

	AbstractGeneratorStrategy * m_gen_strategy; 

public: 

	ConfigGenerator(AbstractGeneratorStrategy * gen_strategy)
	: m_gen_strategy(gen_strategy) { } 

public: 

	void parse(Interpreter const & interpreter) throw (InterpreterException) { 
		m_gen_strategy->parse(interpreter); 
	}
	void run(AbstractRunnerStrategy & runner) throw (RunnerException) { 
		m_gen_strategy->run(runner);
	}

};

#endif /* CONFIG_GENERATOR_HPP__ */
