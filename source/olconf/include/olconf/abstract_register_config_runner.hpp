#ifndef ABSTRACT_REGISTER_CONFIG_RUNNER_HPP__
#define ABSTRACT_REGISTER_CONFIG_RUNNER_HPP__

#include <olconf/register_config_builder.hpp>
#include <olconf/register_config_strategy.hpp>

#include <yamlgen/abstract_config_runner.hpp>


namespace YAMLGen { 
namespace OBLDC { 

class AbstractRegisterConfigRunner : public AbstractConfigRunner<RegisterConfigStrategy>
{

protected: 
	::std::string m_module; 

public: 
	AbstractRegisterConfigRunner(::std::string const & module_name) 
	: m_module(module_name) { } 

	AbstractRegisterConfigRunner()
	: m_module("GLOBAL") { } 

	virtual ~AbstractRegisterConfigRunner() { } 

public: 
	virtual void run(RegisterConfigBuilder * const builder) = 0; 

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* ABSTRACT_REGISTER_CONFIG_RUNNER_HPP__ */
