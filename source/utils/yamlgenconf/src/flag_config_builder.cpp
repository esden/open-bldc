
#include "flag_config_builder.hpp"
#include "config_node.hpp"
#include "flag_config.hpp"
#include "abstract_config_runner.hpp"
#include "abstract_flag_config_runner.hpp"
#include "exception/generator_exception.hpp"

void
FlagConfigBuilder::parse(Interpreter const & interpreter)
throw (GeneratorException)
{
	ConfigNode root;
	ConfigNode config = interpreter.config(); 
	ConfigNode::const_iterator root_it = config.find("config_root");
	if(root_it != config.end()) { 
		root = (*root_it).second;
	}
	else { 
		throw GeneratorException("Could not find config_root");
	}
	parse(root);
}

void 
FlagConfigBuilder::parse(ConfigNode const & config_node)
throw (GeneratorException)
{
	ConfigNode::const_iterator it_flags;
	ConfigNode::const_iterator end_flags = config_node.end(); 
	for(it_flags = config_node.begin(); it_flags != end_flags; ++it_flags) 
	{ 
		::std::string flag_name = (*it_flags).first; 
		ConfigNode flag_config  = (*it_flags).second; 
		
		FlagConfig flag = FlagConfig(flag_name); 
		flag.set_properties(flag_config.values());

		m_flags.push_back(flag);
	}
}

void 
FlagConfigBuilder::run(AbstractFlagConfigRunner & runner) 
throw (RunnerException) 
{
	runner.run(this); 
}
