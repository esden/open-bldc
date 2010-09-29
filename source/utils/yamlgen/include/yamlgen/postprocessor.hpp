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
