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

#ifndef YAMLGEN__OBLDC__REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__
#define YAMLGEN__OBLDC__REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__

#include <olconf/register_group_config.hpp>
#include <olconf/flag_config.hpp>
#include <olconf/register_config_strategy.hpp>

#include <yamlgen/abstract_config_builder.hpp>

#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

class RegisterConfigBuilder : public AbstractConfigBuilder<RegisterConfigStrategy>
{

private: 

	::std::string m_module; 
	::std::vector<RegisterGroupConfig> m_register_groups; 

public: 

	RegisterConfigBuilder() { } 

	virtual ~RegisterConfigBuilder() { } 

	::std::vector<RegisterGroupConfig> const & register_groups() const { 
		return m_register_groups; 
	}

public: 

	inline void log(void) const { 
		LOG_INFO_PRINT("|- RegisterBuilder");
		::std::vector<RegisterGroupConfig>::const_iterator reg_it; 
		::std::vector<RegisterGroupConfig>::const_iterator reg_end; 
		reg_end = m_register_groups.end(); 
		for(reg_it = m_register_groups.begin(); reg_it != reg_end; ++reg_it) {
			(*reg_it).log(); 
		}
	}

public: 

	virtual void parse(ConfigNode const & config);
	virtual void parse_partial(ConfigNode const & config_node);
	virtual void run(AbstractRegisterConfigRunner & runner);

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* YAMLGEN__OBLDC__REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__ */
