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

#include <olconf/define_config_header_runner.hpp>
#include <yamlgen/abstract_config_builder.hpp>

#include <iostream>
#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

void
DefineConfigHeaderRunner::run(DefineConfigBuilder * builder) 
{ 
	::std::vector<DefineConfig> const defines = builder->defines(); 

	::std::vector<DefineConfig>::const_iterator defines_it; 
	::std::vector<DefineConfig>::const_iterator defines_end = defines.end(); 

	for(defines_it = defines.begin(); defines_it != defines_end; ++defines_it) {
		::std::string name  = (*defines_it).name();
		::std::string value = (*defines_it).value(); 

		::std::cout << ::std::endl; 
		::std::cout << "/* Module: " << m_module << " Define: " << name << " */" << ::std::endl;
	
		if(value == "no") { 
			::std::cout << "// "; 
		} 
		::std::cout << "#define " << m_module << "__" << name;
			
		if(value != "yes" && value != "no") { 
			::std::cout << " " << value << ::std::endl;
		}
	}
	::std::cout << ::std::endl; 
}

} /* namespace OBLDC */
} /* namespace YAMLGen */
