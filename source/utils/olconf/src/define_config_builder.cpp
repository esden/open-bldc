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

#include <olconf/define_config_builder.hpp>
#include <olconf/abstract_define_config_runner.hpp>
#include <yamlgen/config_node.hpp>
#include <yamlgen/abstract_config_runner.hpp>
#include <yamlgen/exception/builder_exception.hpp>


namespace YAMLGen { 
namespace OBLDC { 

void
DefineConfigBuilder::parse(ConfigNode const & config)
{
	ConfigNode root;
	ConfigNode::const_iterator root_it = config.find("config_root");
	if(root_it != config.end()) { 
		root = (*root_it).second;
	}
	else { 
		throw BuilderException("Could not find config_root");
	}
	parse_partial(root);
}

void 
DefineConfigBuilder::parse_partial(ConfigNode const & config_node)
{
	::std::map< ::std::string, ::std::string>::const_iterator it_defines;
	::std::map< ::std::string, ::std::string>::const_iterator end_defines;
	end_defines = config_node.values().end(); 
	for(it_defines = config_node.values().begin(); it_defines != end_defines; ++it_defines) 
	{ 
		::std::string name  = (*it_defines).first; 
		::std::string value = (*it_defines).second; 
		
		add_define(name, value); 
	}
}

void 
DefineConfigBuilder::run(AbstractDefineConfigRunner & runner) 
{
	runner.run(this); 
}

} /* namespace OBLDC */
} /* namespace YAMLGen */
