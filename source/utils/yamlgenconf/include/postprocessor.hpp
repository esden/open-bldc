#ifndef POSTPROCESSOR_HPP__
#define POSTPROCESSOR_HPP__

#include "config_node.hpp"

class Postprocessor { 

private: 

	ConfigNode m_config; 

public: 

	Postprocessor(ConfigNode const & config) 
	:	m_config(config) { 
	}

public: 

	void run(); 

	ConfigNode const & config() { return m_config; } 

};

#endif
