#ifndef ABSTRACT_DEFINE_CONFIG_RUNNER_HPP__
#define ABSTRACT_DEFINE_CONFIG_RUNNER_HPP__

#include <olconf/define_config_builder.hpp>
#include <olconf/define_config_strategy.hpp>

#include <yamlgen/abstract_config_runner.hpp>


namespace YAMLGen { 
namespace OBLDC { 

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
	virtual void run(DefineConfigBuilder * const builder) = 0; 

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* ABSTRACT_DEFINE_CONFIG_RUNNER_HPP__ */
