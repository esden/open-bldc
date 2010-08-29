#ifndef REGISTER_CONFIG_HPP__
#define REGISTER_CONFIG_HPP__

#include <string>
#include <map>

#include "config.hpp"
#include "logging.hpp"

class RegisterConfig : public Config 
{
	
private: 

	::std::string m_name; 
	::std::map< ::std::string, ::std::string > m_properties; 
	WidgetConfig m_widget_config;

public: 

	RegisterConfig(::std::string & name)
	: m_name(name) { }

	RegisterConfig(yaml_char_t * name)	{ 
		m_name = ::std::string((const char *)(name));
	}

	RegisterConfig() 
	: m_name("") { } 

	~RegisterConfig() { } 

public:

	inline void log(void) const { 
		::std::map< ::std::string, ::std::string>::const_iterator it; 
		::std::map< ::std::string, ::std::string>::const_iterator end = m_properties.end(); 

		LOG_INFO_PRINT("     Register %s", m_name.c_str());
		for(it = m_properties.begin(); it != end; ++it) { 
			LOG_INFO_PRINT("     |- %s = %s", 
					(*it).first.c_str(), (*it).second.c_str());
		}
		m_widget_config.log(); 
	}

	inline void set_properties(const ::std::map< ::std::string, ::std::string> & props) { 
		m_properties = props;
	}

	inline void set_property(const ::std::string & name, 
													 const ::std::string & value) 
	{
		m_properties.insert(::std::pair< ::std::string, ::std::string >(name, value));
	}

	inline void set_widget(WidgetConfig const & widget_config) {
		m_widget_config = widget_config; 
	}
	inline WidgetConfig const &  widget(void) const { 
		return m_widget_config; 
	}
	
};

#endif
