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

#ifndef FLAG_CONFIG_HPP__
#define FLAG_CONFIG_HPP__

#include <olconf/widget_config.hpp>

#include <yamlgen/property_config.hpp>

#include <string>
#include <algorithm>
#include <cctype>


namespace YAMLGen { 
namespace OBLDC { 

class FlagConfig : public PropertyConfig 
{ 

private: 

	WidgetConfig m_widget_config;

public: 

	FlagConfig() { 
	}
	
	FlagConfig(::std::string const & name) { 
		m_name = name; 
		::std::transform(m_name.begin(), m_name.end(), m_name.begin(), 
										 (int(*)(int))::std::toupper);
	}

public:

	inline void set_widget_config(WidgetConfig const & widget_config) { 
		m_widget_config = widget_config; 
	}
	WidgetConfig const & widget_config(void) const { return m_widget_config; } 

public: 

	inline void log(void) const { 
		LOG_INFO_PRINT("|- Flag");
		PropertyConfig::log(); 
		m_widget_config.log(); 
	}

	inline unsigned short bit(void) const throw (ConfigException) {
		::std::string bit = property("bit");
		return (unsigned short)(atoi(bit.c_str())); 
	}

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif
