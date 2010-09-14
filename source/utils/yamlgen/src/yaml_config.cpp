
#include "yaml_config.hpp"
#include "logging.hpp"

#include "config_generator.hpp"
#include "module_config_strategy.hpp"
#include "module_config_header_runner.hpp"

#include "exception/interpreter_exception.hpp"
#include "exception/parser_exception.hpp"
#include "exception/builder_exception.hpp"
#include "exception/config_exception.hpp"


namespace YAMLGen { 
namespace OBLDC { 

void
YAMLConfig::read(char const * filename) 
{
	m_interpreter.read(filename); 

	ConfigGenerator<ModuleConfigStrategy> generator(m_interpreter); 
	ModuleConfigHeaderRunner runner_strategy; 
	generator.run(runner_strategy); 

/* 
	Note that a ConfigGenerator can parse several 
	interpreters, and you can run several runners 
	on the same configurator: 

		config_generator.parse(first_interpreter); 
		config_generator.parse(second_interpreter); 

		config_generator.run(first_strategy); 
		config_generator.run(second_strategy); 
		config_generator.run(third_strategy); 
*/

}

} /* namespace OBLDC */
} /* namespace YAMLGen */
