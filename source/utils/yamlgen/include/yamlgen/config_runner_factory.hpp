#ifndef CONFIG_RUNNER_FACTORY_HPP__
#define CONFIG_RUNNER_FACTORY_HPP__

#include "abstract_config_runner.hpp"
#include "register_config_header_runner.hpp"
//#include "flag_config_header_runner.hpp"

#include <string>

class ModuleConfig; 

class ConfigRunnerFactory { 

public:

	static AbstractConfigRunner const * get(ModuleConfig const & module, 
																					::std::string const & mode) const 
	{ 
		if(mode == "register_groups") { 
			return new RegisterConfigHeaderRunner(module.name());
		}
		else if(mode == "flags") { 
//			return new FlagConfigHeaderRunner();
		}
		else if(mode == "define") { 
//			return new DefineConfigHeaderRunner(); 
		}
		return 0; 
	}

};

#endif
