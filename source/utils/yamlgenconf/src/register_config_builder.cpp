
#include "register_config_builder.hpp"
#include "config_node.hpp"
#include "register_config.hpp"
#include "register_group_config.hpp"
#include "abstract_config_runner.hpp"
#include "abstract_register_config_runner.hpp"
#include "exception/generator_exception.hpp"

void
RegisterConfigBuilder::parse(ConfigNode const & config)
throw (GeneratorException)
{
	ConfigNode root;
	ConfigNode::const_iterator root_it = config.find("config_root");
	if(root_it != config.end()) { 
		root = (*root_it).second;
	}
	else { 
		throw GeneratorException("Could not find config_root");
	}
	parse_partial(root);
}

void
RegisterConfigBuilder::parse_partial(ConfigNode const & config_node)
throw (GeneratorException)
{
	ConfigNode::const_iterator it_groups  = config_node.begin(); 
	ConfigNode::const_iterator end_groups = config_node.end(); 
	for(; it_groups != end_groups; ++it_groups) 
	{ 
		RegisterGroupConfig group = RegisterGroupConfig((*it_groups).first);
		
		ConfigNode group_config = (*it_groups).second;
		group.set_properties(group_config.values());
		
		ConfigNode registers = (*it_groups).second; 
		ConfigNode::const_iterator reglist = registers.find("registers"); 
		ConfigNode register_list = (*reglist).second; 
		
		ConfigNode::const_iterator it_regs  = register_list.begin(); 
		ConfigNode::const_iterator end_regs = register_list.end(); 
		
		for(; it_regs != end_regs; ++it_regs) { 
			RegisterConfig reg = RegisterConfig((*it_regs).first);
			ConfigNode properties = (*it_regs).second; 
			reg.set_properties(properties.values());
			group.add_register(reg); 
		}
		m_register_groups.push_back(group);
#if defined(LOG) && LOG == DEBUG
		group.log(); 
#endif
	}
}

void 
RegisterConfigBuilder::run(AbstractRegisterConfigRunner & runner) 
throw (RunnerException) 
{
	runner.run(this); 
}
