#ifndef PROPERTY_CONFIG_HPP__
#define PROPERTY_CONFIG_HPP__

class PropertyConfig 
{ 

protected: 

	::std::string m_name; 

public: 

	typedef ::std::map< ::std::string, ::std::string> property_map;
	typedef ::std::pair< ::std::string, ::std::string> property_entry; 

	property_map m_properties; 

public: 

	PropertyConfig() 
	: m_name("") { } 

	PropertyConfig(::std::string const & name) 
	: m_name(name) { } 

	virtual ~PropertyConfig() { } 

public: 

	inline void set_properties(const property_map & props) { 
		m_properties = props;
	}

	inline void set_property(const ::std::string & name, 
													 const ::std::string & value) 
	{
		m_properties.insert(property_entry(name, value));
	}

	inline property_map const & properties(void) const { 
		return m_properties;
	}

	inline bool has_property(::std::string const & key) const { 
		return (m_properties.find(key) != m_properties.end());
	}

	inline ::std::string property(::std::string const & key) const { 
		property_map::const_iterator prop_it  = m_properties.find(key); 
		property_map::const_iterator prop_end = m_properties.end(); 
		if (prop_it == prop_end) { 
			throw ConfigException("Could not find property 'bit' in flag settings");
		}
		::std::string value = (*prop_it).second; 
		return value; 
	}

public: 

	inline void name(::std::string const & name) { 
		m_name = name; 
	}
	inline ::std::string const & name(void) const { 
		return m_name; 
	}

public: 

	virtual void log(void) const { 
		property_map::const_iterator it; 
		property_map::const_iterator end = m_properties.end(); 

		LOG_INFO_PRINT("Config %s", m_name.c_str());
		for(it = m_properties.begin(); it != end; ++it) { 
			LOG_INFO_PRINT("|- %s = %s", 
					(*it).first.c_str(), (*it).second.c_str());
		}
	}

};

#endif /* PROPERTY_CONFIG_HPP__ */
