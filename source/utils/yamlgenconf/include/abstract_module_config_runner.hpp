#ifndef ABSTRACT_MODULE_CONFIG_RUNNER_HPP__
#define ABSTRACT_MODULE_CONFIG_RUNNER_HPP__

#include "abstract_config_runner.hpp"
#include "module_config_builder.hpp"
#include "module_config_strategy.hpp"


class AbstractModuleConfigRunner : public AbstractConfigRunner<ModuleConfigStrategy>
{

protected: 
	::std::string m_module; 

public: 
	AbstractModuleConfigRunner(::std::string module_name) 
		: m_module(module_name) { } 

	AbstractModuleConfigRunner()
		: m_module("GLOBAL") { } 

	virtual ~AbstractModuleConfigRunner() { } 

public: 
	virtual void run(ModuleConfigBuilder * const builder) throw (RunnerException) = 0; 

};

#endif /* ABSTRACT_MODULE_CONFIG_RUNNER_HPP__ */
