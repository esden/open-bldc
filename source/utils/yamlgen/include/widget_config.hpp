#ifndef WIDGET_CONFIG_HPP
#define WIDGET_CONFIG_HPP

#include <string>
#include <map>

#include "config.hpp"
#include "logging.hpp"

class WidgetConfig : Config
{

private: 

	::std::map< ::std::string, ::std::string > m_properties; 

public:

	WidgetConfig() { 
	} 

	inline void log(void) const { 
		::std::map< ::std::string, ::std::string>::const_iterator it; 
		::std::map< ::std::string, ::std::string>::const_iterator end = m_properties.end(); 

		LOG_INFO_PRINT("       | Widget");
		for(it = m_properties.begin(); it != end; ++it) { 
			LOG_INFO_PRINT("       |- %s = %s", 
					(*it).first.c_str(), (*it).second.c_str());
		}
	}

	inline void set_property(const ::std::string & name, 
													 const ::std::string & value) 
	{
		m_properties.insert(::std::pair< ::std::string, ::std::string >(name, value));	
	}
};

#endif
