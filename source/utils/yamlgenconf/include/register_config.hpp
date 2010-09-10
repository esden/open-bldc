#ifndef REGISTER_CONFIG_HPP__
#define REGISTER_CONFIG_HPP__

#include "config.hpp"
#include "logging.hpp"
#include "widget_config.hpp"
#include "exception/config_exception.hpp"

#include <string>
#include <map>
#include <algorithm>
#include <cctype>

class RegisterConfig : public Config 
{
public: 

	typedef ::std::map< ::std::string, ::std::string> property_map_t;
	typedef ::std::pair< ::std::string, ::std::string> property_entry_t; 
	
private: 

	::std::string m_name; 
	property_map_t m_properties; 
	WidgetConfig m_widget_config;

public: 

	RegisterConfig(::std::string const & name) { 
		m_name = name; 
		::std::transform(m_name.begin(), m_name.end(), m_name.begin(), 
										 (int(*)(int))::std::toupper);
	}

	RegisterConfig() 
	: m_name("") { } 

	~RegisterConfig() { } 

public:

	inline void log(void) const { 
		property_map_t::const_iterator it; 
		property_map_t::const_iterator end = m_properties.end(); 

		LOG_INFO_PRINT("     Register %s", m_name.c_str());
		for(it = m_properties.begin(); it != end; ++it) { 
			LOG_INFO_PRINT("     |- %s = %s", 
					(*it).first.c_str(), (*it).second.c_str());
		}
		m_widget_config.log(); 
	}

	inline void set_properties(const property_map_t & props) { 
		m_properties = props;
	}

	inline void set_property(const ::std::string & name, 
													 const ::std::string & value) 
	{
		m_properties.insert(property_entry_t(name, value));
	}

	inline void set_widget(WidgetConfig const & widget_config) {
		m_widget_config = widget_config; 
	}
	inline WidgetConfig const & widget(void) const { 
		return m_widget_config; 
	}

	inline property_map_t const & properties(void) const { 
		return m_properties;
	}

public: 

	inline ::std::string const & name(void) const { 
		return m_name; 
	}

	inline const char * register_nr(void) const throw (ConfigException) {
		property_map_t::const_iterator prop_it  = m_properties.find(::std::string("register")); 
		property_map_t::const_iterator prop_end = m_properties.end(); 
		if (prop_it == prop_end) { 
			throw ConfigException("Could not find property 'register'");
		}
		::std::string register_nr = (*prop_it).second; 
		return register_nr.c_str(); 
//		return (unsigned char)(atoi(register_nr.c_str())); 
	}
	
};

#endif
