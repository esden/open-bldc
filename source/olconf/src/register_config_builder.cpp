
#include <olconf/register_config_builder.hpp>
#include <olconf/register_config.hpp>
#include <olconf/register_group_config.hpp>
#include <olconf/abstract_register_config_runner.hpp>

#include <yamlgen/config_node.hpp>
#include <yamlgen/abstract_config_runner.hpp>
#include <yamlgen/exception/builder_exception.hpp>


namespace YAMLGen { 
namespace OBLDC { 

void
RegisterConfigBuilder::parse(ConfigNode const & config)
{
	ConfigNode root;
	ConfigNode::const_iterator root_it = config.find("config_root");
	if(root_it != config.end()) { 
		root = (*root_it).second;
	}
	else { 
		throw BuilderException("Could not find config_root", config);
	}
	parse_partial(root);
}

void
RegisterConfigBuilder::parse_partial(ConfigNode const & config_node)
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
		if(reglist == registers.end()) { 
			throw BuilderException("No entry 'registers:' found", registers);
		}
		ConfigNode register_list = (*reglist).second; 
		
		ConfigNode::const_iterator it_regs  = register_list.begin(); 
		ConfigNode::const_iterator end_regs = register_list.end(); 
		
		for(; it_regs != end_regs; ++it_regs) { 
			RegisterConfig reg = RegisterConfig((*it_regs).first);
			ConfigNode properties = (*it_regs).second; 
			reg.set_properties(properties.values());
			
			if(!reg.has_property("register")) { 
				throw BuilderException("No property 'register: <index>' found", (*it_regs).second);
			}
			if(!reg.has_property("label")) { 
				throw BuilderException("No property 'label: <string>' found", (*it_regs).second);
			}
			
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
{
	runner.run(this); 
}

} /* namespace OBLDC */
} /* namespace YAMLGen */
