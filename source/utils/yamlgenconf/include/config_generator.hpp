#ifndef CONFIG_GENERATOR_HPP__
#define CONFIG_GENERATOR_HPP__

#include "abstract_yaml_generator_strategy.hpp"

class ConfigGenerator
{

private: 

	AbstractYAMLGeneratorStrategy * m_gen_strategy; 

public: 

	ConfigGenerator(AbstractYAMLGeneratorStrategy * gen_strategy)
	: m_gen_strategy(gen_strategy) { } 

public: 

	void parse(YAMLInterpreter const & interpreter) throw (YAMLInterpreterException) { 
		m_gen_strategy->parse(interpreter); 
	}

};

#endif /* CONFIG_GENERATOR_HPP__ */
