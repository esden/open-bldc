/*
 * olconf - yamlgen based Open-BLDC configuration header generator
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

#include <olconf/flag_config_builder.hpp>
#include <olconf/flag_config.hpp>
#include <olconf/abstract_flag_config_runner.hpp>

#include <yamlgen/config_node.hpp>
#include <yamlgen/abstract_config_runner.hpp>
#include <yamlgen/exception/builder_exception.hpp>


namespace YAMLGen {
namespace OBLDC { 

void
FlagConfigBuilder::parse(ConfigNode const & config)
{
	ConfigNode root;
	ConfigNode::const_iterator root_it = config.find("config_root");
	if(root_it != config.end()) { 
		root = (*root_it).second;
	}
	else { 
		throw BuilderException("Could not find config_root", config.context());
	}
	parse_partial(root);
}

void 
FlagConfigBuilder::parse_partial(ConfigNode const & config_node)
{
	ConfigNode::const_iterator it_flags;
	ConfigNode::const_iterator end_flags = config_node.end(); 
	for(it_flags = config_node.begin(); it_flags != end_flags; ++it_flags) 
	{ 
		::std::string flag_name = (*it_flags).first; 
		ConfigNode flag_config  = (*it_flags).second; 
		
		FlagConfig flag = FlagConfig(flag_name); 
		flag.set_properties(flag_config.values());
		
		m_flags.push_back(flag);
	}
}

void 
FlagConfigBuilder::run(AbstractFlagConfigRunner & runner) 
{
	runner.run(this); 
}

} /* namespace OBLDC */
} /* namespace YAMLGen */
 
