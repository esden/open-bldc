
#include "define_config_header_runner.hpp"
#include "abstract_config_builder.hpp"
#include <iostream>
#include <vector>

void
DefineConfigHeaderRunner::run(DefineConfigBuilder * builder) 
throw (RunnerException) 
{ 
	::std::map< ::std::string, ::std::string> const defines = builder->defines(); 

	::std::map< ::std::string, ::std::string>::const_iterator defines_it; 
	::std::map< ::std::string, ::std::string>::const_iterator defines_end = defines.end(); 

	for(defines_it = defines.begin(); defines_it != defines_end; ++defines_it) {
		::std::string name  = (*defines_it).first; 
		::std::string value = (*defines_it).second; 

		::std::cout << ::std::endl; 
		::std::cout << "/* Define: " << name << " */" << ::std::endl;
	
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

