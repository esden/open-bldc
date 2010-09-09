#ifndef ABSTRACT_GENERATOR_STRATEGY_HPP__
#define ABSTRACT_GENERATOR_STRATEGY_HPP__

#include "interpreter.hpp"
#include "generator_exception.hpp"
#include "runner_exception.hpp"

class AbstractRunnerStrategy;

class AbstractGeneratorStrategy
{ 

public: 

	virtual void parse(Interpreter const & interpreter) throw (GeneratorException) = 0; 
	virtual void run(AbstractRunnerStrategy & runner) throw (RunnerException) = 0; 

};

#endif /* ABSTRACT_GENERATOR_STRATEGY_HPP__ */
