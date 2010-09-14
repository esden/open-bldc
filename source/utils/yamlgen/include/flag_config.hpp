#ifndef FLAG_CONFIG_HPP__
#define FLAG_CONFIG_HPP__

#include "config.hpp"
#include "widget_config.hpp"
#include <string>


class FlagConfig : public Config 
{ 
public: 

	typedef ::std::map< ::std::string, ::std::string> property_map_t;
	typedef ::std::pair< ::std::string, ::std::string> property_entry_t; 

private: 

	property_map_t m_properties; 
	WidgetConfig m_widget_config;
	::std::string m_name; 

public: 

	FlagConfig() { 
		m_name = ""; 
	}
	
	FlagConfig(::std::string const & name) { 
		m_name = name; 
		::std::transform(m_name.begin(), m_name.end(), m_name.begin(), 
										 (int(*)(int))::std::toupper);
	}

public:

	inline void set_name(::std::string const & name) { m_name = name; }
	inline ::std::string const & name(void) const { return m_name; }

	inline void set_widget_config(WidgetConfig const & widget_config) { 
		m_widget_config = widget_config; 
	}
	WidgetConfig const & widget_config(void) const { return m_widget_config; } 

public: 

	inline void set_property(const ::std::string & name, 
													 const ::std::string & value) 
	{
		m_properties.insert(property_entry_t(name, value));
	}

	inline void set_properties(const property_map_t & props) { 
		m_properties = props;
	}

	inline property_map_t const & properties(void) const { 
		return m_properties;
	}

	inline unsigned short bit(void) const throw (ConfigException) {
		property_map_t::const_iterator prop_it  = m_properties.find(::std::string("bit")); 
		property_map_t::const_iterator prop_end = m_properties.end(); 
		if (prop_it == prop_end) { 
			throw ConfigException("Could not find property 'bit' in flag settings");
		}
		::std::string bit = (*prop_it).second; 
		return (unsigned short)(atoi(bit.c_str())); 
	}

};

#endif
