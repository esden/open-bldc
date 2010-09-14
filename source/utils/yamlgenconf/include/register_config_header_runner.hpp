#ifndef REGISTER_CONFIG_HEADER_RUNNER_HPP__
#define REGISTER_CONFIG_HEADER_RUNNER_HPP__

#include "abstract_register_config_runner.hpp"
#include "register_group_config.hpp"
#include "exception/runner_exception.hpp"
#include <vector>


class RegisterConfigHeaderRunner: public AbstractRegisterConfigRunner
{ 

public: 

	RegisterConfigHeaderRunner() { } 
	
	RegisterConfigHeaderRunner(::std::string const & module_name) 
	: AbstractRegisterConfigRunner(module_name) { } 

	virtual ~RegisterConfigHeaderRunner() { } 
	
	virtual void run(RegisterConfigBuilder * const builder);

};

#endif /* REGISTER_CONFIG_HEADER_RUNNER_HPP__ */
