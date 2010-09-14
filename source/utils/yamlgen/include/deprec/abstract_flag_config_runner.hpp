#ifndef ABSTRACT_FLAG_CONFIG_RUNNER_HPP__
#define ABSTRACT_FLAG_CONFIG_RUNNER_HPP__

#include "abstract_config_runner.hpp"
#include "flag_config_builder.hpp"
#include "flag_config_strategy.hpp"


namespace YAMLGen { 
namespace OBLDC { 

class AbstractFlagConfigRunner : public AbstractConfigRunner<FlagConfigStrategy>
{

protected: 
	::std::string m_module; 

public: 
	AbstractFlagConfigRunner(::std::string const & module_name) 
	: m_module(module_name) { } 

	AbstractFlagConfigRunner()
	: m_module("GLOBAL") { } 

	virtual ~AbstractFlagConfigRunner() { } 

public: 
	virtual void run(FlagConfigBuilder * const builder) = 0; 

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* ABSTRACT_FLAG_CONFIG_RUNNER_HPP__ */
