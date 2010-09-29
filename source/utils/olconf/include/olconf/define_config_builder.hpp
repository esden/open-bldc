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

#ifndef DEFINE_CONFIG_GENERATOR_STRATEGY_HPP__
#define DEFINE_CONFIG_GENERATOR_STRATEGY_HPP__

#include <olconf/define_config_strategy.hpp>
#include <olconf/define_config.hpp>

#include <yamlgen/abstract_config_builder.hpp>

#include <vector>
#include <algorithm>
#include <cctype>


namespace YAMLGen { 
namespace OBLDC { 

class DefineConfigBuilder : public AbstractConfigBuilder<DefineConfigStrategy>
{

private: 

	::std::string m_module; 
	::std::vector<DefineConfig> m_defines; 

public: 

	DefineConfigBuilder() { } 

	virtual ~DefineConfigBuilder() { } 

	::std::vector<DefineConfig> const & defines() const { 
		return m_defines; 
	}

public: 

	void add_define(::std::string const & name, ::std::string const & value) { 
		::std::string up_name = name; 
		::std::transform(up_name.begin(), up_name.end(), up_name.begin(), 
										 (int(*)(int))::std::toupper);
		DefineConfig config = DefineConfig(name, value); 
		m_defines.push_back(config); 
	}

public: 

	virtual void parse(ConfigNode const & config);
	virtual void parse_partial(ConfigNode const & config_node);
	virtual void run(AbstractDefineConfigRunner & runner);

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* DEFINE_CONFIG_GENERATOR_STRATEGY_HPP__ */
