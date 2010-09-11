
#include "config_node.hpp"
#include <string>
#include <map>
#include <iostream>

void 
ConfigNode::log(unsigned char indent) const { 
	unsigned char ic;
	::std::map< ::std::string, ::std::string>::const_iterator v_it  = m_values.begin();
	::std::map< ::std::string, ::std::string>::const_iterator v_end = m_values.end(); 
	for(; v_it != v_end; ++v_it) { 
		for(ic = 0; ic < indent; ++ic) ::std::cerr << " "; 
		::std::cerr << (*v_it).first << " -> " << (*v_it).second << ::std::endl; 
	}
	
	::std::map< ::std::string, ConfigNode>::const_iterator n_it  = m_nodes.begin();
	::std::map< ::std::string, ConfigNode>::const_iterator n_end = m_nodes.end(); 
	for(; n_it != n_end; ++n_it) { 
		for(ic = 0; ic < indent; ++ic) ::std::cerr << " "; 
		::std::cerr << (*n_it).first << " -> " << ::std::endl; 
		(*n_it).second.log(indent+1);
	}
}
