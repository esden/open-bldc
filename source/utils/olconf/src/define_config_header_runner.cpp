
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
