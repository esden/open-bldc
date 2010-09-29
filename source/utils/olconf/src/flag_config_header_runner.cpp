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

#include <olconf/flag_config_header_runner.hpp>
#include <olconf/flag_config.hpp>

#include <yamlgen/abstract_config_builder.hpp>

#include <iostream>
#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

void
FlagConfigHeaderRunner::run(FlagConfigBuilder * builder) 
{ 
	::std::vector<FlagConfig> const flags = builder->flags(); 

	::std::vector<FlagConfig>::const_iterator flags_it; 
	::std::vector<FlagConfig>::const_iterator flags_end = flags.end(); 

	for(flags_it = flags.begin(); flags_it != flags_end; ++flags_it) {
		FlagConfig flag = (*flags_it);

		::std::cout << ::std::endl; 
		::std::cout << "/* Module: " << m_module << " Flag: " << flag.name() << " */" << ::std::endl;
		::std::cout << "#define " << m_module << "__FLAG_" << flag.name();
		::std::cout << " " << flag.bit() << ::std::endl;
	}
	::std::cout << ::std::endl; 
}

} /* namespace OBLDC */
} /* namespace YAMLGen */
