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

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif
