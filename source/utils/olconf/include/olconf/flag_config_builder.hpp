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

#ifndef FLAG_CONFIG_BUILDER_HPP__
#define FLAG_CONFIG_BUILDER_HPP__

#include <olconf/register_group_config.hpp>
#include <olconf/flag_config.hpp>
#include <olconf/flag_config_strategy.hpp>

#include <yamlgen/abstract_config_builder.hpp>

#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

class FlagConfigBuilder : public AbstractConfigBuilder<FlagConfigStrategy>
{

private: 

	::std::string m_module; 
	::std::vector<FlagConfig> m_flags; 

public: 

	FlagConfigBuilder() { } 

	virtual ~FlagConfigBuilder() { } 

	::std::vector<FlagConfig> const & flags() const { 
		return m_flags; 
	}

public: 

	virtual void parse(ConfigNode const & config);
	virtual void parse_partial(ConfigNode const & config_node);
	virtual void run(AbstractFlagConfigRunner & runner);

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* FLAG_CONFIG_BUILDER_HPP__ */
