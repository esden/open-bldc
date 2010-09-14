#ifndef DEFINE_CONFIG_HEADER_RUNNER_HPP__
#define DEFINE_CONFIG_HEADER_RUNNER_HPP__

#include <olconf/abstract_define_config_runner.hpp>

#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

class DefineConfigHeaderRunner: public AbstractDefineConfigRunner
{ 

public: 

	DefineConfigHeaderRunner() { } 
	
	DefineConfigHeaderRunner(::std::string const & module_name) 
	: AbstractDefineConfigRunner(module_name) { } 

	virtual ~DefineConfigHeaderRunner() { } 
	
	virtual void run(DefineConfigBuilder * const builder);

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* DEFINE_CONFIG_HEADER_RUNNER_HPP__ */
