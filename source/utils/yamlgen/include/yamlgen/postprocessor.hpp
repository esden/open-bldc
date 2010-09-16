#ifndef POSTPROCESSOR_HPP__
#define POSTPROCESSOR_HPP__

#include <yamlgen/config_node.hpp>


namespace YAMLGen { 

class Postprocessor { 

private: 

	ConfigNode m_config; 

public: 

	Postprocessor(ConfigNode const & config) 
	:	m_config(config) { 
	}

public: 

	virtual void run(); 

	ConfigNode const & config() { return m_config; } 

private: 

	ConfigNode & process_node(ConfigNode & config_node); 
	void merge(ConfigNode & node, const char * filename) const; 
	void merge(ConfigNode & node, ::std::string const &  filename) const { 
		merge(node, filename.c_str());
	}

};

} /* namespace YAMLGen */

#endif
