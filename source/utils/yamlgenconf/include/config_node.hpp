#ifndef CONFIG_NODE_HPP__
#define CONFIG_NODE_HPP__

#include <vector>
#include <string>
#include <map>
#include "logging.hpp"

class ConfigNode 
{

public: 

	typedef enum { UNDEFINED=0, VALUE, SEQUENCE, MAPPING } node_type; 

	typedef ::std::map< ::std::string, ConfigNode>::iterator iterator; 
	typedef ::std::map< ::std::string, ConfigNode>::const_iterator const_iterator; 

	typedef ::std::map< ::std::string, ::std::vector< ::std::string > >::iterator seq_iterator; 
	typedef ::std::map< ::std::string, ::std::vector< ::std::string > >::const_iterator const_seq_iterator; 

private: 

	node_type m_type; 

	::std::map< ::std::string, ::std::string> m_values; 
	::std::map< ::std::string, ::std::vector< ::std::string> > m_seqs; 
	::std::map< ::std::string, ConfigNode> m_nodes; 

public: 

	ConfigNode()
	: m_type(UNDEFINED) { }

	ConfigNode(ConfigNode const & other) 
	: m_type(UNDEFINED), m_values(other.m_values), m_nodes(other.m_nodes) { }

public: 

	node_type type(void) const { 
		return m_type; 
	}

public: 

	void set_node(::std::string const & key, 
								ConfigNode const & node) { 
		m_nodes.insert(::std::pair< ::std::string, ConfigNode>(key, node));
		m_type = MAPPING; 
	} 

	void set_value(::std::string const & key, 
								 ::std::string const & value) { 
		m_values.insert(::std::pair< ::std::string, ::std::string>(key,value)); 
		m_type = VALUE; 
	} 
	void set_value(const char * key, const char * value) { 
		::std::string skey(key); 
		::std::string svalue(value); 
		set_value(skey, svalue); 
	}
	void set_value(::std::string & key, const char * value) { 
		::std::string svalue(value); 
		set_value(key, svalue); 
	}
	void set_value(const char * key, ::std::string & value) { 
		::std::string skey(key); 
		set_value(skey, value); 
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

	void push_seq_value(::std::string const & key, ::std::string const & value) { 
		seq_iterator seq = m_seqs.find(key); 
		if(seq != m_seqs.end()) { 
			(*seq).second.push_back(value); 
		}
		m_type = SEQUENCE; 
	}

public: 

	::std::map< ::std::string, ::std::string> const & values(void) const { 
		return m_values; 
	}
	::std::map< ::std::string, ConfigNode> const & nodes(void) const { 
		return m_nodes; 
	}
	::std::map< ::std::string, ::std::vector< ::std::string> > const & seqs(void) const { 
		return m_seqs; 
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
