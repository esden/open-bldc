/*
 * yamlgen - YAML to everything generator framework
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
