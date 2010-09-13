
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

	ConfigNode::const_seq_iterator s_it  = m_seqs.begin();
	ConfigNode::const_seq_iterator s_end = m_seqs.end(); 
	for(; s_it != s_end; ++s_it) { 
		for(ic = 0; ic < indent; ++ic) ::std::cerr << " "; 
		::std::cerr << (*s_it).first << " -> [" << ::std::endl; 

		::std::vector< ::std::string>::const_iterator sv_it  = (*s_it).second.begin(); 
		::std::vector< ::std::string>::const_iterator sv_end = (*s_it).second.end(); 
		for(; sv_it != sv_end; ++sv_it) { 
			for(ic = 0; ic < indent+2; ++ic) ::std::cerr << " "; 
			::std::cerr << (*sv_it) << ::std::endl; 
		}
		for(ic = 0; ic < indent; ++ic) ::std::cerr << " "; 
		::std::cerr << "]" << ::std::endl;
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
		::std::string key = (*inject_nodes_it).first; 
		ConfigNode::iterator existing = hfind(key); 
		if(existing == end()) { // Key not present yet
			set_node(key, (*inject_nodes_it).second);
		}
		else { // Key is present, merge nodes
			(*existing).second.inject((*inject_nodes_it).second);
		}
	}
	
	::std::map< ::std::string, ::std::string>::const_iterator inject_values_it; 
	::std::map< ::std::string, ::std::string>::const_iterator inject_values_end; 
	inject_values_it  = other.values().begin();
	inject_values_end = other.values().end(); 
	for(; inject_values_it != inject_values_end; ++inject_values_it) { 
		set_value((*inject_values_it).first, (*inject_values_it).second); 
	}
}
