#ifndef MODULE_CONFIG_STRATEGY_HPP__
#define MODULE_CONFIG_STRATEGY_HPP__

namespace YAMLGen { 
namespace OBLDC { 

class AbstractModuleConfigRunner; 
class ModuleConfigBuilder; 

class ModuleConfigStrategy
{
public: 
	typedef AbstractModuleConfigRunner Runner; 
	typedef ModuleConfigBuilder Builder; 
};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* MODULE_CONFIG_STRATEGY_HPP__ */
