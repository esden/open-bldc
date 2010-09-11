
#include "module_config_builder.hpp"
#include "config_node.hpp"
#include "module_config.hpp"
#include "abstract_module_config_runner.hpp"

#include "logging.hpp"

void
ModuleConfigBuilder::parse(Interpreter const & interpreter) 
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
ModuleConfigBuilder::parse(ConfigNode const & config_node) 
throw (GeneratorException)
{
	ConfigNode::const_iterator it_modules;
	ConfigNode::const_iterator end_modules = config_node.end(); 
	for(it_modules = config_node.begin(); it_modules != end_modules; ++it_modules) 
	{ 
		::std::string module_name     = (*it_modules).first; 
		ConfigNode module_config_node = (*it_modules).second; 
		ModuleConfig module_config(module_name, module_config_node);
		m_modules.push_back(module_config);
		LOG_DEBUG_PRINT("-----------------------------------");
		LOG_DEBUG_PRINT("Building module %s", module_name.c_str());
		LOG_DEBUG_PRINT("Module section:\n");
#if defined(LOG) && LOG == DEBUG
		module_config_node.log(); 
#endif
		LOG_DEBUG_PRINT("\n");
		LOG_DEBUG_PRINT("End Module section");
	}
}

void 
ModuleConfigBuilder::run(AbstractModuleConfigRunner & runner) 
throw (RunnerException) 
{ 
	runner.run(this); 
}
