#ifndef MODULE_CONFIG_HEADER_RUNNER_HPP__
#define MODULE_CONFIG_HEADER_RUNNER_HPP__

#include <olconf/abstract_module_config_runner.hpp>
#include <olconf/module_config.hpp>

#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

class ModuleConfigHeaderRunner: public AbstractModuleConfigRunner
{ 

private: 

public: 

	ModuleConfigHeaderRunner() { } 
	virtual ~ModuleConfigHeaderRunner() { } 
	
	virtual void run(ModuleConfigBuilder * const builder);

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* MODULE_CONFIG_HEADER_RUNNER_HPP__ */
