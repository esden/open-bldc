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

void
HeaderGenerationRunner::run(DefineConfigBuilder * builder) 
{ 
	::std::map< ::std::string, ::std::string> const defines = builder->defines(); 

	::std::map< ::std::string, ::std::string>::const_iterator defines_it; 
	::std::map< ::std::string, ::std::string>::const_iterator defines_end = defines.end(); 

	for(defines_it = defines.begin(); defines_it != defines_end; ++defines_it) {
		::std::string name  = (*defines_it).first; 
		::std::string value = (*defines_it).second; 

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
