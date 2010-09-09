#ifndef REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__
#define REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__

#include "abstract_generator_strategy.hpp"
#include "register_group_config.hpp"
#include "register_config.hpp"
#include "flag_config.hpp"
#include "generator_exception.hpp"
#include <vector>

class RegisterConfigGeneratorStrategy : public AbstractGeneratorStrategy
{

private: 

	FlagConfig m_flags; 
	::std::vector<RegisterGroupConfig> m_register_groups; 

public: 

	RegisterConfigGeneratorStrategy() { } 
	virtual ~RegisterConfigGeneratorStrategy() { } 

public: 

	virtual void parse(Interpreter const & interpreter) throw (GeneratorException); 
	virtual void run(AbstractRunnerStrategy & runner) throw (RunnerException); 

};

#endif /* REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__ */
