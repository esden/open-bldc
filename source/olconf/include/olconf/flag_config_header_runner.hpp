#ifndef FLAG_CONFIG_HEADER_RUNNER_HPP__
#define FLAG_CONFIG_HEADER_RUNNER_HPP__

#include <olconf/flag_config.hpp>
#include <olconf/abstract_flag_config_runner.hpp>

#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

class FlagConfigHeaderRunner: public AbstractFlagConfigRunner
{ 

public: 

	FlagConfigHeaderRunner() { } 
	
	FlagConfigHeaderRunner(::std::string const & module_name) 
	: AbstractFlagConfigRunner(module_name) { } 

	virtual ~FlagConfigHeaderRunner() { } 
	
	virtual void run(FlagConfigBuilder * const builder);

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* FLAG_CONFIG_HEADER_RUNNER_HPP__ */
