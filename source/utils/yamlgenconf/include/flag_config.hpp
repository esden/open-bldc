#ifndef FLAG_CONFIG_HPP__
#define FLAG_CONFIG_HPP__

#include <string>
#include "config.hpp"
#include "widget_config.hpp"

class FlagConfig : public Config 
{ 

private: 

	unsigned char m_bit; 
	WidgetConfig m_widget_config;
	::std::string m_name; 

public: 

	FlagConfig() { 
		m_bit  = 0; 
		m_name = ""; 
	}

public:

	inline void set_bit(unsigned char bit) { m_bit = bit; } 
	inline unsigned char bit(void) { return m_bit; } 

	inline void set_name(::std::string const & name) { m_name = name; }
	inline ::std::string const & name(void) const { return m_name; }

};

#endif
