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

#ifndef MODULE_CONFIG_GENERATOR_STRATEGY_HPP__
#define MODULE_CONFIG_GENERATOR_STRATEGY_HPP__

#include <olconf/module_config_strategy.hpp>
#include <olconf/module_config.hpp>

#include <yamlgen/abstract_config_builder.hpp>

#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

class ModuleConfigBuilder : public AbstractConfigBuilder<ModuleConfigStrategy>
{

private: 

	::std::vector<ModuleConfig> m_modules; 
	::std::string m_target_name; 

public: 

	ModuleConfigBuilder() 
	: m_target_name("TheTarget") { } 
	virtual ~ModuleConfigBuilder() { } 

public: 

	virtual void parse(ConfigNode const & config);
	virtual void parse_partial(ConfigNode const & config_node);
	virtual void run(AbstractModuleConfigRunner & runner);

public: 

	::std::string const & target_name() const { return m_target_name; } 
	::std::vector<ModuleConfig> const & modules(void) const { return m_modules; }

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* MODULE_CONFIG_GENERATOR_STRATEGY_HPP__ */
