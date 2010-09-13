
#include "config_node.hpp"
#include "logging.hpp"
#include <string>

#include <map>
#include <iostream>


void 
ConfigNode::log(unsigned char indent) const 
{ 
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

void 
ConfigNode::inject(ConfigNode const & other) 
{ 
	ConfigNode::const_iterator inject_nodes_it  = other.begin(); 
	ConfigNode::const_iterator inject_nodes_end = other.end(); 
	for(; inject_nodes_it != inject_nodes_end; ++inject_nodes_it) { 
//		if(key_not_present) { 
			set_node((*inject_nodes_it).first, (*inject_nodes_it).second);
//		}
//		else { 
//			push_nodes_to_existing_node_vector(); 
//		}
	}
	
	::std::map< ::std::string, ::std::string>::const_iterator inject_values_it; 
	::std::map< ::std::string, ::std::string>::const_iterator inject_values_end; 
	inject_values_it  = other.values().begin();
	inject_values_end = other.values().end(); 
	for(; inject_values_it != inject_values_end; ++inject_values_it) { 
		set_value((*inject_values_it).first, (*inject_values_it).second); 
	}
}
