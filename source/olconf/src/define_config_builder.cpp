
#include <olconf/define_config_builder.hpp>
#include <olconf/abstract_define_config_runner.hpp>
#include <yamlgen/config_node.hpp>
#include <yamlgen/abstract_config_runner.hpp>
#include <yamlgen/exception/builder_exception.hpp>


namespace YAMLGen { 
namespace OBLDC { 

void
DefineConfigBuilder::parse(ConfigNode const & config)
{
	ConfigNode root;
	ConfigNode::const_iterator root_it = config.find("config_root");
	if(root_it != config.end()) { 
		root = (*root_it).second;
	}
	else { 
		throw BuilderException("Could not find config_root");
	}
	parse_partial(root);
}

void 
DefineConfigBuilder::parse_partial(ConfigNode const & config_node)
{
	::std::map< ::std::string, ::std::string>::const_iterator it_defines;
	::std::map< ::std::string, ::std::string>::const_iterator end_defines;
	end_defines = config_node.values().end(); 
	for(it_defines = config_node.values().begin(); it_defines != end_defines; ++it_defines) 
	{ 
		::std::string name  = (*it_defines).first; 
		::std::string value = (*it_defines).second; 
		
		add_define(name, value); 
	}
}

void 
DefineConfigBuilder::run(AbstractDefineConfigRunner & runner) 
{
	runner.run(this); 
}

} /* namespace OBLDC */
} /* namespace YAMLGen */
