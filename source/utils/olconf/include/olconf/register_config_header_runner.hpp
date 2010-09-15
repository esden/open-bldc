#ifndef YAMLGEN__OBLDC__REGISTER_CONFIG_HEADER_RUNNER_HPP__
#define YAMLGEN__OBLDC__REGISTER_CONFIG_HEADER_RUNNER_HPP__

#include <olconf/abstract_register_config_runner.hpp>
#include <olconf/register_group_config.hpp>

#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

class RegisterConfigHeaderRunner: public AbstractRegisterConfigRunner
{ 

public: 

	RegisterConfigHeaderRunner() { } 
	
	RegisterConfigHeaderRunner(::std::string const & module_name) 
	: AbstractRegisterConfigRunner(module_name) { } 

	virtual ~RegisterConfigHeaderRunner() { } 
	
	virtual void run(RegisterConfigBuilder * const builder);

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* YAMLGEN__OBLDC__REGISTER_CONFIG_HEADER_RUNNER_HPP__ */
