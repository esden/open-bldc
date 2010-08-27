#ifndef FLAG_CONFIG_HPP__
#define FLAG_CONFIG_HPP__

#include <string>
#include "config.h"

class FlagConfig : public Config 
{ 

private: 

	uint8_t m_bit; 
	WidgetConfig m_widget_config;

public: 

	FlagConfig(); 

public:

	inline void set_name(::std::string const & name) { m_name = &name; }
	inline ::std::string const & name(void) const { return m_name; }

};

#endif
