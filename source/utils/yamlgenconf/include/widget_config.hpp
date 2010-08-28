#ifndef WIDGET_CONFIG_HPP
#define WIDGET_CONFIG_HPP

class WidgetConfig : Config
{

private: 

	::std::string m_classname; 
	::std::string m_ctype; 
	::std::string m_dtype; 
	float m_min; 
	float m_max; 
	float m_default; 

public:

	void set_classname(::std::string const & classname) { m_classname = classname; }
	::std::string const & classname(void) const { return m_classname; }

	void set_ctype(::std::string const & ctype) { m_ctype = ctype; }
	::std::string const & ctype(void) const { return m_ctype; }

	void set_dtype(::std::string const & dtype) { m_dtype = dtype; }
	::std::string const & dtype(void) const { return m_dtype; }

};

#endif
