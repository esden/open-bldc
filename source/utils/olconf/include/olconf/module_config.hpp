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

#ifndef MODULE_CONFIG_HPP__
#define MODULE_CONFIG_HPP__

#include <olconf/register_config.hpp>
#include <olconf/register_config_builder.hpp>
#include <olconf/flag_config.hpp>
#include <olconf/flag_config_builder.hpp>
#include <olconf/define_config_builder.hpp>
#include <olconf/define_config.hpp>

#include <yamlgen/config_node.hpp>


namespace YAMLGen { 
namespace OBLDC { 

class ModuleConfig
{

private: 

	::std::string m_name; 
	ConfigNode m_config_node; 

private: 

	::std::vector<RegisterGroupConfig> m_registers;
	::std::vector<FlagConfig> m_flags; 
	::std::vector<DefineConfig> m_defines; 

public: 

	::std::vector<RegisterGroupConfig> const & registers() const { 
		return m_registers;
	}
	::std::vector<FlagConfig> & flags() { 
		return m_flags;
	}
	::std::vector<DefineConfig> & defines() { 
		return m_defines;
	}

public: 

	ModuleConfig(::std::string const & name, 
							 ConfigNode const & config_node) 
	: m_name(name), m_config_node(config_node) 
	{ } 

public: 

	inline void log(void) const { 
		LOG_INFO_PRINT("|- Module");
		::std::vector<RegisterGroupConfig>::const_iterator reg_it; 
		::std::vector<RegisterGroupConfig>::const_iterator reg_end; 
		reg_end = m_registers.end(); 
		for(reg_it = m_registers.begin(); reg_it != reg_end; ++reg_it) { 
			(*reg_it).log(); 
		}
	}

public: 

	void add(RegisterConfigBuilder const & builder) { 
		::std::vector<RegisterGroupConfig> added = builder.register_groups();
		m_registers.insert(m_registers.end(), added.begin(), added.end());
	}
	void add(DefineConfigBuilder const & builder) { 
		::std::vector<DefineConfig> added = builder.defines();
		m_defines.insert(m_defines.end(), added.begin(), added.end());
	}
	void add(FlagConfigBuilder const & builder) { 
		::std::vector<FlagConfig> added = builder.flags();
		m_flags.insert(m_flags.end(), added.begin(), added.end());
	}

public: 

	::std::string const & name(void) const { return m_name; }
	ConfigNode const & config(void) const  { return m_config_node; } 

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif
