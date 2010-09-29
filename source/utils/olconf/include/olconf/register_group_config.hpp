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

#ifndef REGISTER_GROUP_CONFIG_HPP__
#define REGISTER_GROUP_CONFIG_HPP__

#include <olconf/register_config.hpp>

#include <yamlgen/property_config.hpp>

#include <string>
#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

class RegisterGroupConfig : public PropertyConfig
{

private: 
	
	::std::string m_description;
	::std::vector<RegisterConfig> m_register_configs; 

public: 

	RegisterGroupConfig() 
	: m_description("")	{ }

	RegisterGroupConfig(::std::string const & name)
	: PropertyConfig(name), m_description("") { }

	~RegisterGroupConfig() { }

	inline void add_register(const RegisterConfig & register_config) { 
		m_register_configs.push_back(register_config); 
	}
	inline ::std::vector<RegisterConfig> const & registers(void) const { 
		return m_register_configs;
	}

	inline void set_description(const ::std::string & description) { 
		m_description = description; 
	}
	inline void set_description(yaml_char_t * description) {
		m_description = ::std::string((const char * )(description)); 
	}

	inline ::std::string const & description(void) const { 
		return m_description; 
	}

	inline void log(void) const { 
		LOG_INFO_PRINT("|- Register group %s", m_name.c_str());
		
		PropertyConfig::log();

		::std::vector<RegisterConfig>::const_iterator it; 
		::std::vector<RegisterConfig>::const_iterator end = m_register_configs.end(); 
		for(it = m_register_configs.begin(); it != end; ++it) { 
			(*it).log(); 
		}
	}
	
};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif
