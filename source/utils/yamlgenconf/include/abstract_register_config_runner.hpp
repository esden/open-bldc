#ifndef ABSTRACT_REGISTER_CONFIG_RUNNER_HPP__
#define ABSTRACT_REGISTER_CONFIG_RUNNER_HPP__

#include "abstract_config_runner.hpp"
#include "register_config_builder.hpp"

/* 
 	Base interface for all runners depending from RegisterConfigGeneratorStrategy. 
*/

class AbstractRegisterConfigRunner : public AbstractConfigRunner
// Better:
// class AbstractRegisterConfigRunner : public AbstractConfigRunner<RegisterConfigStrategy>
// and there
// typedef typename TGeneratorStrategy::Builder builder_t; 
// virtual void run(builder_t * builder) ...
{
public: 
	virtual ~AbstractRegisterConfigRunner() { } 

	virtual void run(AbstractConfigBuilder * builder) throw (RunnerException) = 0; 
// Better: 
// virtual void run(AbstractRegisterConfigBuilder * builder) throw (RunnerException) = 0; 
};

#endif /* ABSTRACT_REGISTER_CONFIG_RUNNER_HPP__ */
