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

#include <olconf/register_config_header_runner.hpp>
#include <olconf/register_group_config.hpp>
#include <olconf/register_config.hpp>

#include <yamlgen/abstract_config_builder.hpp>

#include <iostream>
#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

void
RegisterConfigHeaderRunner::run(RegisterConfigBuilder * builder) 
{ 
	::std::vector<RegisterGroupConfig> const register_groups = builder->register_groups(); 

	::std::vector<RegisterGroupConfig>::const_iterator groups_it; 
	::std::vector<RegisterGroupConfig>::const_iterator groups_end = register_groups.end(); 

	for(groups_it = register_groups.begin(); groups_it != groups_end; ++groups_it) {
		RegisterGroupConfig group = (*groups_it);
		::std::vector<RegisterConfig> const registers = group.registers();

		::std::cout << ::std::endl; 
		::std::cout << "/* Module: " << m_module << " Register group: " << group.name() << " */" << ::std::endl;

		::std::vector<RegisterConfig>::const_iterator register_it; 
		::std::vector<RegisterConfig>::const_iterator register_end = registers.end();
		for(register_it = registers.begin(); register_it != register_end; ++register_it) { 
			::std::cout << "#define " << m_module << "__" << (*register_it).name();
			::std::cout << " " << (*register_it).register_nr() << ::std::endl;
		}
	}
	::std::cout << ::std::endl; 
}

} /* namespace OBLDC */
} /* namespace YAMLGen */
