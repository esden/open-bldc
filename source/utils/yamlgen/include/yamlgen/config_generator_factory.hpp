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

#ifndef CONFIG_GENERATOR_FACTORY_HPP__
#define CONFIG_GENERATOR_FACTORY_HPP__

#include "abstract_config_generator.hpp"

// #include "register_config_strategy.hpp"
// #include "flag_config_strategy.hpp"
// #include "define_config_strategy.hpp"

#include "config_generator.hpp"

namespace YAMLGen { 

template <class TGeneratorStrategy>
class ConfigGenerator; // : public AbstractConfigGenerator;

class ConfigGeneratorFactory { 

public:

	static AbstractConfigGenerator const * get(ModuleConfig const & module, 
								          									 ::std::string const & mode) 
	{ 
		if(mode == "register_groups") { 
			return new ConfigGenerator<RegisterConfigStrategy>(module.config());
		}
		else if(mode == "flags") { 
//			return new ConfigGenerator<FlagConfigStrategy>(module.config());
		}
		else if(mode == "define") { 
//			return new ConfigGenerator<DefineConfigStrategy>(module.config());
		}
	}

};

} /* namespace YAMLGen */

#endif
