#ifndef CONFIG_NODE_HPP__
#define CONFIG_NODE_HPP__

#include <vector>
#include <string>
#include <map>
#include "logging.hpp"

class ConfigNode 
{

public: 

	typedef ::std::map< ::std::string, ConfigNode>::iterator iterator; 
	typedef ::std::map< ::std::string, ConfigNode>::const_iterator const_iterator; 

private: 

	::std::map< ::std::string, ::std::string> m_values; 
	::std::map< ::std::string, ConfigNode> m_nodes; 

public: 

	ConfigNode() { }

	ConfigNode(ConfigNode const & other) 
	: m_values(other.m_values), m_nodes(other.m_nodes) { }

public: 

	void set_node(::std::string const & key, 
								ConfigNode const & node) { 
		m_nodes.insert(::std::pair< ::std::string, ConfigNode>(key, node));
	} 

	void set_value(::std::string const & key, 
								 ::std::string const & value) { 
		m_values.insert(::std::pair< ::std::string, ::std::string>(key,value)); 
	} 
	void set_value(const char * key, const char * value) { 
		::std::string skey(key); 
		::std::string svalue(value); 
		m_values.insert(::std::pair< ::std::string, ::std::string>(skey, svalue)); 
	}
	void set_value(::std::string & key, const char * value) { 
		::std::string svalue(value); 
		m_values.insert(::std::pair< ::std::string, ::std::string>(key, svalue)); 
	}
	void set_value(const char * key, ::std::string & value) { 
		::std::string skey(key); 
		m_values.insert(::std::pair< ::std::string, ::std::string>(skey, value)); 
	}

public: 

	void update_nodes(::std::map< ::std::string, ConfigNode> const & new_nodes) { 
		ConfigNode::const_iterator nodes_it; 
		ConfigNode::const_iterator nodes_end = new_nodes.end(); 
		for(nodes_it = new_nodes.begin(); nodes_it != nodes_end; ++nodes_it) { 
			set_node((*nodes_it).first, (*nodes_it).second); 
		}
	}

public: 

	::std::map< ::std::string, ::std::string> const & values(void) const { 
		return m_values; 
	}
	::std::map< ::std::string, ConfigNode> const & nodes(void) const { 
		return m_nodes; 
	}

public: 

	void inject(ConfigNode const & other); 

public: 

	void log(unsigned char indent) const; 
	inline void log(void) const {
		log(0);
	}

public: 

	const_iterator begin() const { return m_nodes.begin(); }
	const_iterator end() const   { return m_nodes.end(); } 

	iterator hbegin() { return m_nodes.begin(); }
	iterator hend()   { return m_nodes.end(); } 

	const_iterator find(const char * key) const { 
		return m_nodes.find(::std::string(key));
	}
	const_iterator find(::std::string const & key) const { 
		return m_nodes.find(key);
	}
	iterator hfind(::std::string const & key) { 
		return m_nodes.find(key);
	}
	
};

#endif
