#ifndef REGISTER_CONFIG_HPP__
#define REGISTER_CONFIG_HPP__

#include <string>
#include "config.h"

class RegisterConfig : public Config 
{
	
private: 
	unint8_t 				m_register; 
	::std::string 	m_name; 
	::std::string 	m_label; 
	bool 						m_readonly;
	WidgetConfig		m_widget_config;

public: 

	RegisterConfig() { 
		m_register = -1;
		m_name     = ""; 
		m_label    = ""; 
		m_readonly = false; 
	} 

public:

	inline void set_name(::std::string const & name) { m_name = &name; }
	inline ::std::string const & name(void) const { return m_name; }
	
	inline void set_label(::std::string const & label) { m_label = &label; }
	inline ::std::string const & label(void) const { return m_label; }

	inline void set_readonly(const bool readonly) { m_readonly = readonly; }
	inline bool readonly(void) const { return m_readonly; } 

	inline void set_widget_config(WidgetConfig const & widget_config) {
		m_widget_config = widget_config; 
	}
	inline WidgetConfig const &  widget_config(void) const { 
		return m_widget_config; 
	}
	
};

#endif
