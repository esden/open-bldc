#ifndef FLAG_CONFIG_HEADER_RUNNER_HPP__
#define FLAG_CONFIG_HEADER_RUNNER_HPP__

#include "abstract_flag_config_runner.hpp"
#include "flag_config.hpp"
#include "exception/runner_exception.hpp"
#include <vector>


class FlagConfigHeaderRunner: public AbstractFlagConfigRunner
{ 

public: 

	FlagConfigHeaderRunner() { } 
	
	FlagConfigHeaderRunner(::std::string const & module_name) 
	: AbstractFlagConfigRunner(module_name) { } 

	virtual ~FlagConfigHeaderRunner() { } 
	
	virtual void run(FlagConfigBuilder * const builder);

};

#endif /* FLAG_CONFIG_HEADER_RUNNER_HPP__ */
