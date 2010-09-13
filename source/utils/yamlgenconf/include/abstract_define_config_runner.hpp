#ifndef ABSTRACT_DEFINE_CONFIG_RUNNER_HPP__
#define ABSTRACT_DEFINE_CONFIG_RUNNER_HPP__

#include "abstract_config_runner.hpp"
#include "define_config_builder.hpp"
#include "define_config_strategy.hpp"


class AbstractDefineConfigRunner : public AbstractConfigRunner<DefineConfigStrategy>
{

protected: 
	::std::string m_module; 

public: 
	AbstractDefineConfigRunner(::std::string const & module_name) 
	: m_module(module_name) { } 

	AbstractDefineConfigRunner()
	: m_module("GLOBAL") { } 

	virtual ~AbstractDefineConfigRunner() { } 

public: 
	virtual void run(DefineConfigBuilder * const builder) throw (RunnerException) = 0; 

};

#endif /* ABSTRACT_DEFINE_CONFIG_RUNNER_HPP__ */
