#ifndef ABSTRACT_REGISTER_CONFIG_RUNNER_HPP__
#define ABSTRACT_REGISTER_CONFIG_RUNNER_HPP__

#include "abstract_config_runner.hpp"
#include "register_config_builder.hpp"
#include "register_config_strategy.hpp"


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

#endif /* ABSTRACT_REGISTER_CONFIG_RUNNER_HPP__ */
