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

#ifndef REGISTER_CONFIG_HPP__
#define REGISTER_CONFIG_HPP__

#include <olconf/widget_config.hpp>

#include <yamlgen/property_config.hpp>
#include <yamlgen/logging.hpp>

#include <string>
#include <map>
#include <algorithm>
#include <cctype>


namespace YAMLGen { 
namespace OBLDC { 

class RegisterConfig : public PropertyConfig 
{
	
private: 

	WidgetConfig m_widget_config;

public: 

	RegisterConfig(::std::string const & name) { 
		m_name = name; 
		::std::transform(m_name.begin(), m_name.end(), m_name.begin(), 
										 (int(*)(int))::std::toupper);
	}

	~RegisterConfig() { } 

public:

	inline void log(void) const { 
		LOG_INFO_PRINT("|- Register");
		PropertyConfig::log(); 
		m_widget_config.log(); 
	}

	inline void set_widget(WidgetConfig const & widget_config) {
		m_widget_config = widget_config; 
	}
	inline WidgetConfig const & widget(void) const { 
		return m_widget_config; 
	}

public: 

	inline const char * register_nr(void) const { 
		property_map::const_iterator prop_it  = m_properties.find(::std::string("register")); 
		property_map::const_iterator prop_end = m_properties.end(); 
		if (prop_it == prop_end) { 
			throw ConfigException("Could not find property 'register' in register settings");
		}
		::std::string register_nr = (*prop_it).second; 
		return register_nr.c_str(); 
	}

public:

	::std::string const & label(void) const { 
		property_map::const_iterator prop_it  = m_properties.find(::std::string("label")); 
		property_map::const_iterator prop_end = m_properties.end(); 
		if (prop_it == prop_end) { 
			throw ConfigException("Could not find property 'register' in register settings");
		}
		return (*prop_it).second;
	}
	
};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif
