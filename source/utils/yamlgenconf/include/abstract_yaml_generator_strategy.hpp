#ifndef ABSTRACT_YAML_GENERATOR_STRATEGY_HPP__
#define ABSTRACT_YAML_GENERATOR_STRATEGY_HPP__

#include "yaml_interpreter.hpp"
#include "yaml_generator_exception.hpp"

class AbstractYAMLGeneratorStrategy
{ 

public: 

	virtual void parse(YAMLInterpreter const & interpreter) throw (YAMLGeneratorException) = 0; 

};

#endif /* ABSTRACT_YAML_GENERATOR_STRATEGY_HPP__ */
