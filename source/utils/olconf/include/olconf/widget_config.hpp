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

#ifndef WIDGET_CONFIG_HPP
#define WIDGET_CONFIG_HPP

#include <string>
#include <map>

#include <yamlgen/property_config.hpp>
#include <yamlgen/logging.hpp>


namespace YAMLGen { 
namespace OBLDC { 

class WidgetConfig : public PropertyConfig
{

public:

	WidgetConfig() { 
	} 

	inline void log(void) const { 
		LOG_INFO_PRINT("|- Widget");
		PropertyConfig::log(); 
	}

public: 

	::std::string const & classname(void) const { 
		property_map::const_iterator prop_it  = m_properties.find(::std::string("class")); 
		property_map::const_iterator prop_end = m_properties.end(); 
		if (prop_it == prop_end) { 
			throw ConfigException("Could not find property 'class' in widget settings");
		}
		return (*prop_it).second;
	}

public: 

        inline int int_property(::std::string const & key) const {
		property_map::const_iterator prop_it  = m_properties.find(::std::string(key)); 
		property_map::const_iterator prop_end = m_properties.end(); 
		if (prop_it == prop_end) { 
			return 0; 
		}
                int value = atoi((*prop_it).second.c_str());
		return value; 
	}

        inline int min(void) const {
		return int_property("min"); 
	}
        inline int max(void) const {
		return int_property("max"); 
	}
        inline int step(void) const {
		return int_property("step"); 
	}
        inline int default_value(void) const {
		return int_property("default"); 
	}

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif
