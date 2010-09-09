
#include "register_config_generator_strategy.hpp"
#include "generator_exception.hpp"
#include "config_node.hpp"
#include "abstract_runner_strategy.hpp"

void
RegisterConfigGeneratorStrategy::parse(Interpreter const & interpreter)
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
	
	ConfigNode::const_iterator it_groups  = root.begin(); 
	ConfigNode::const_iterator end_groups = root.end(); 
	for(; it_groups != end_groups; ++it_groups) { 
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
		group.log(); 
	}
}

void
RegisterConfigGeneratorStrategy::run(AbstractRunnerStrategy & runner)
throw (RunnerException)
{
	runner.run(m_register_groups);
}
