#ifndef FLAG_CONFIG_HPP__
#define FLAG_CONFIG_HPP__

#include "property_config.hpp"
#include "widget_config.hpp"

#include <string>


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

	inline unsigned short bit(void) const throw (ConfigException) {
		::std::string bit = property("bit");
		return (unsigned short)(atoi(bit.c_str())); 
	}

};

#endif
