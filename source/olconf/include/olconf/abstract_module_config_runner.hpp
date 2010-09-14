#ifndef ABSTRACT_MODULE_CONFIG_RUNNER_HPP__
#define ABSTRACT_MODULE_CONFIG_RUNNER_HPP__

#include <olconf/module_config_builder.hpp>
#include <olconf/module_config_strategy.hpp>

#include <yamlgen/abstract_config_runner.hpp>


namespace YAMLGen {
namespace OBLDC { 

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
	virtual void run(ModuleConfigBuilder * const builder) = 0;

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* ABSTRACT_MODULE_CONFIG_RUNNER_HPP__ */
