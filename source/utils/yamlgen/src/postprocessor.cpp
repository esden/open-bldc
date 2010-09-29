/*
 * yamlgen - YAML to everything generator framework
 * Copyright (C) 2010 by Tobias Fuchs <twh.fuchs@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "postprocessor.hpp"
#include "interpreter.hpp"
#include "config_node.hpp"
#include "logging.hpp"

#include <string>
#include <map>


namespace YAMLGen { 

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
		process_node((*nodes_it).second);
	}

	ConfigNode::const_seq_iterator seqs_it; 
	ConfigNode::const_seq_iterator seqs_end = node.seqs().end(); 
	for(seqs_it = node.seqs().begin(); seqs_it != seqs_end; ++seqs_it) { 
		::std::string key = (*seqs_it).first; 
		if(key == "import_config") { 
			::std::vector< ::std::string>::const_iterator inc_it  = (*seqs_it).second.begin(); 
			::std::vector< ::std::string>::const_iterator inc_end = (*seqs_it).second.end(); 
			for(; inc_it != inc_end; ++inc_it) { 
				merge(node, *inc_it);
			}
		} 
	}
	
	::std::map< ::std::string, ::std::string>::const_iterator v_it; 
	::std::map< ::std::string, ::std::string>::const_iterator v_end;
	v_end = node.values().end(); 
	// Iterate over this nodes values. On every key 'import_config', 
	// run an interpreter on its value (the filename), and inject 
	// the interpreter's resulting config node. 
	for(v_it = node.values().begin(); v_it != v_end; ++v_it) { 
		::std::string key = (*v_it).first; 
		if(key == "import_config") { 
			merge(node, (*v_it).second.c_str());
		}
	}

	return node; 
}

} /* namespace YAMLGen */
