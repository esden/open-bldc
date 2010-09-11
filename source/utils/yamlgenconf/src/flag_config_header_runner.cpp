
#include "flag_config_header_runner.hpp"
#include "abstract_config_builder.hpp"
#include "flag_config.hpp"
#include <iostream>
#include <vector>

void
FlagConfigHeaderRunner::run(FlagConfigBuilder * builder) 
throw (RunnerException) 
{ 
	::std::vector<FlagConfig> const flags = builder->flags(); 

	::std::vector<FlagConfig>::const_iterator flags_it; 
	::std::vector<FlagConfig>::const_iterator flags_end = flags.end(); 

	::std::cout << "#ifndef FLAG_CONFIG_HPP__" << ::std::endl;
	::std::cout << "#define FLAG_CONFIG_HPP__" << ::std::endl;

	for(flags_it = flags.begin(); flags_it != flags_end; ++flags_it) {
		FlagConfig flag = (*flags_it);

		::std::cout << ::std::endl; 
		::std::cout << "/* Flag: " << flag.name() << " */" << ::std::endl;
		::std::cout << "#define " << m_module << "_FLAG_" << flag.name();
		::std::cout << " " << flag.bit() << ::std::endl;
	}
	::std::cout << ::std::endl; 
	::std::cout << "#endif /* FLAG_CONFIG_HPP__ */" << ::std::endl;
	::std::cout << ::std::endl; 
}

