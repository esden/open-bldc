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
