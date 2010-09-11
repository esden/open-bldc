#ifndef ABSTRACT_REGISTER_CONFIG_RUNNER_HPP__
#define ABSTRACT_REGISTER_CONFIG_RUNNER_HPP__

#include "abstract_config_runner.hpp"
#include "register_config_builder.hpp"
#include "register_config_strategy.hpp"

/* 
 	Base interface for all runners depending from RegisterConfigGeneratorStrategy. 
*/

class AbstractRegisterConfigRunner : public AbstractConfigRunner<RegisterConfigStrategy>
{

protected: 
	::std::string m_module; 

public: 
	AbstractRegisterConfigRunner(::std::string module_name) 
		: m_module(module_name) { } 

	AbstractRegisterConfigRunner()
		: m_module("GLOBAL") { } 

	typedef RegisterConfigStrategy Strategy; 

	virtual ~AbstractRegisterConfigRunner() { } 

	virtual void run(Strategy::Builder * const builder) throw (RunnerException) = 0; 
};

#endif /* ABSTRACT_REGISTER_CONFIG_RUNNER_HPP__ */
