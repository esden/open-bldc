#ifndef CONFIG_NODE_HPP__
#define CONFIG_NODE_HPP__

#include <vector>
#include <string>

class ConfigNode 
{

public: 

	typedef ::std::vector<ConfigNode>::iterator iterator; 
	typedef ::std::vector<ConfigNode>::const_iterator const_iterator; 

private: 

	::std::string m_value; 
	::std::vector<ConfigNode> m_nodes; 

public: 

	ConfigNode() { }
	
	ConfigNode(const char * value) 
	: m_value(value) { } 

	ConfigNode(ConfigNode const & other) 
	: m_value(other.m_value), m_nodes(other.m_nodes) { }

public: 

	::std::string const & value(void) const             { return m_value; }
	::std::vector<ConfigNode> const & nodes(void) const { return m_nodes; } 

	void push_back(ConfigNode const & node)     { m_nodes.push_back(node); } 
	void set_value(::std::string const & value) { m_value = value; } 

public: 

	iterator begin() { return m_nodes.begin(); }
	iterator end()   { return m_nodes.end(); }
	
};

#endif
