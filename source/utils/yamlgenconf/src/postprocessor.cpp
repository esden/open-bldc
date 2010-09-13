
#include "postprocessor.hpp"
#include "interpreter.hpp"
#include "config_node.hpp"
#include "logging.hpp"

#include <string>
#include <map>

void 
Postprocessor::merge(ConfigNode & node, const char * filename) const 
{ 
	Interpreter injection; 
	// Parse file given in filename value: 
	injection.read(filename); 
	Postprocessor postproc(injection.config());
	postproc.run(); 
	ConfigNode inject_node = postproc.config(); 
	// Root node in injected config node always has key 
	// 'config_root'. 
	// Add every child node of config root as child node 
	// to current node. 
	ConfigNode::const_iterator root = inject_node.find("config_root"); 
	// root.first is key ("config_root"), root.second is 
	// the actual root node: 
	node.inject((*root).second); 
}

void
Postprocessor::run()
{
	LOG_DEBUG_PRINT("POSTPROC: Processing node: ");
#if defined(LOG) && LOG == DEBUG
	m_config.log(); 
#endif
	LOG_DEBUG_PRINT("POSTPROC: Begin post-processing");
	m_config = process_node(m_config); 
}

ConfigNode &  
Postprocessor::process_node(ConfigNode & node)
{
	ConfigNode::iterator nodes_it; 
	ConfigNode::const_iterator nodes_end = node.end(); 
	for(nodes_it = node.hbegin(); nodes_it != nodes_end; ++nodes_it) { 
		::std::string key = (*nodes_it).first; 
		LOG_DEBUG_PRINT("POSTPROC: Processing %s", key.c_str());
		process_node((*nodes_it).second);
	}

	ConfigNode::const_seq_iterator seqs_it; 
	ConfigNode::const_seq_iterator seqs_end = node.seqs().end(); 
	for(seqs_it = node.seqs().begin(); seqs_it != seqs_end; ++seqs_it) { 
		::std::string key = (*seqs_it).first; 
		LOG_DEBUG_PRINT("POSTPROC: Include list");
		if(key == "include_yaml") { 
			::std::vector< ::std::string>::const_iterator inc_it  = (*seqs_it).second.begin(); 
			::std::vector< ::std::string>::const_iterator inc_end = (*seqs_it).second.end(); 
			for(; inc_it != inc_end; ++inc_it) { 
				LOG_DEBUG_PRINT("POSTPROC: Processing include file %s", (*inc_it).c_str());
				merge(node, *inc_it);
			}
		} 
	}
	
	::std::map< ::std::string, ::std::string>::const_iterator v_it; 
	::std::map< ::std::string, ::std::string>::const_iterator v_end;
	v_end = node.values().end(); 
	// Iterate over this nodes values. On every key 'include_yaml', 
	// run an interpreter on its value (the filename), and inject 
	// the interpreter's resulting config node. 
	for(v_it = node.values().begin(); v_it != v_end; ++v_it) { 
		LOG_DEBUG_PRINT("POSTPROC: Key is %s", (*v_it).first.c_str());
		::std::string key = (*v_it).first; 
		if(key == "include_yaml") { 
			LOG_DEBUG_PRINT("POSTPROC: Processing include file %s", (*v_it).second.c_str());
			merge(node, (*v_it).second);
/*
			Interpreter injection; 
			// Parse file given in filename value: 
			injection.read((*v_it).second.c_str());
			Postprocessor postproc(injection.config());
			postproc.run(); 
			ConfigNode inject_node = postproc.config(); 
			// Root node in injected config node always has key 
			// 'config_root'. 
			// Add every child node of config root as child node 
			// to current node. 
			ConfigNode::const_iterator root = inject_node.find("config_root"); 
			// root.first is key ("config_root"), root.second is 
			// the actual root node: 
			node.inject((*root).second); 
*/
		}
	}

	return node; 
}
