
#include "module_config_header_runner.hpp"
#include "abstract_config_builder.hpp"
#include "abstract_config_generator.hpp"
#include "module_config.hpp"
#include "abstract_config_generator.hpp"
#include "config_generator.hpp"

#include "register_config_strategy.hpp"
#include "register_config_header_runner.hpp"
#include "flag_config_strategy.hpp"
#include "flag_config_header_runner.hpp"
#include "define_config_strategy.hpp"
#include "define_config_header_runner.hpp"

#include <iostream>
#include <vector>

void
ModuleConfigHeaderRunner::run(ModuleConfigBuilder * builder) 
{ 
	::std::vector<ModuleConfig> const modules = builder->modules(); 

	::std::vector<ModuleConfig>::const_iterator modules_it; 
	::std::vector<ModuleConfig>::const_iterator modules_end = modules.end(); 

	::std::cout << "#ifndef MODULE_CONFIG_HPP__" << ::std::endl;
	::std::cout << "#define MODULE_CONFIG_HPP__" << ::std::endl;

	for(modules_it = modules.begin(); modules_it != modules_end; ++modules_it) { 
		::std::string const module_name = (*modules_it).name(); 
		ConfigNode const module_config  = (*modules_it).config(); 

		ConfigNode::const_iterator modes_it; 
		ConfigNode::const_iterator modes_end = module_config.end(); 
		
		for(modes_it = module_config.begin(); modes_it != modes_end; ++modes_it) { 
			::std::string mode_name = (*modes_it).first;
			ConfigNode mode_config  = (*modes_it).second; 
			LOG_DEBUG_PRINT("Building mode %s in module %s", 
											mode_name.c_str(), module_name.c_str());

			if(mode_name == "register_groups") { 
				ConfigGenerator<RegisterConfigStrategy> generator(mode_config);
				RegisterConfigHeaderRunner runner(module_name); 
				generator.run(runner); 
			}
			if(mode_name == "flags") { 
 				ConfigGenerator<FlagConfigStrategy> generator(mode_config);
				FlagConfigHeaderRunner runner(module_name); 
				generator.run(runner); 
			}
			if(mode_name == "defines") { 
 				ConfigGenerator<DefineConfigStrategy> generator(mode_config);
				DefineConfigHeaderRunner runner(module_name); 
				generator.run(runner); 
			}

			/*
			AbstractConfigGenerator * generator;
			generator = ConfigGeneratorFactory.get(module_config, *mode_it);
			::std::vector<generator::Runner> runners;
			runners   = ConfigRunnerFactory.get(module_config, *mode_id); 
			
			::std::vector<generator::Runner>::const_iterator runners_it; 
			::std::vector<generator::Runner>::const_iterator runners_end = runners.end(); 
			for(runners_it = runners.begin(); runners_it != runners_end; ++runners_it) { 
				generator.run(*runners_it);
			}
			*/
		}
	}

	::std::cout << ::std::endl; 
	::std::cout << "#endif /* MODULE_CONFIG_HPP__ */" << ::std::endl;
}

