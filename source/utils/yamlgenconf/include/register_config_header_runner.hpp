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
	virtual ~RegisterConfigHeaderRunner() { } 
	
	virtual void run(AbstractConfigBuilder * builder) throw (RunnerException);

};

#endif /* REGISTER_CONFIG_HEADER_RUNNER_HPP__ */
