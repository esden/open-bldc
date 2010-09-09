
#include "register_config_header_runner_strategy.hpp"
#include "register_group_config.hpp"
#include "register_config.hpp"
#include <iostream>

void
RegisterConfigHeaderRunnerStrategy::run(::std::vector<RegisterGroupConfig> const & register_groups) 
throw (RunnerException) 
{ 
	::std::vector<RegisterGroupConfig>::const_iterator groups_it; 
	::std::vector<RegisterGroupConfig>::const_iterator groups_end = register_groups.end(); 

	::std::cout << "#ifndef REGISTER_CONFIG_HPP__" << ::std::endl;
	::std::cout << "#define REGISTER_CONFIG_HPP__" << ::std::endl;

	for(groups_it = register_groups.begin(); groups_it != groups_end; ++groups_it) {
		RegisterGroupConfig group = (*groups_it);
		::std::vector<RegisterConfig> const registers = group.registers();

		::std::cout << ::std::endl; 
		::std::cout << "/* Register group: " << group.name() << " */" << ::std::endl;

		::std::vector<RegisterConfig>::const_iterator register_it; 
		::std::vector<RegisterConfig>::const_iterator register_end = registers.end();
		for(register_it = registers.begin(); register_it != register_end; ++register_it) { 
			::std::cout << "#define GPROT_" << (*register_it).name();
			::std::cout << " " << (*register_it).register_nr() << ::std::endl;
		}
	}
	::std::cout << ::std::endl; 
	::std::cout << "#endif /* REGISTER_CONFIG_HPP__ */" << ::std::endl;
}
