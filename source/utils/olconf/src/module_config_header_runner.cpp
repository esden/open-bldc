/*
 * olconf - yamlgen based Open-BLDC configuration header generator
 * Copyright (C) 2010 by Tobias Fuchs <twh.fuchs@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <olconf/module_config_header_runner.hpp>
#include <olconf/module_config.hpp>
#include <olconf/register_config_header_runner.hpp>
#include <olconf/register_config_strategy.hpp>
#include <olconf/flag_config_header_runner.hpp>
#include <olconf/flag_config_strategy.hpp>
#include <olconf/define_config_header_runner.hpp>
#include <olconf/define_config_strategy.hpp>

#include <yamlgen/config_generator.hpp>
#include <yamlgen/abstract_config_builder.hpp>
#include <yamlgen/abstract_config_generator.hpp>

#include <iostream>
#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

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

} /* namespace OBLDC */
} /* namespace YAMLGen */
