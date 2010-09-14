#ifndef MODULE_CONFIG_STRATEGY_HPP__
#define MODULE_CONFIG_STRATEGY_HPP__

class AbstractModuleConfigRunner; 
class ModuleConfigBuilder; 

class ModuleConfigStrategy
{
public: 
	typedef AbstractModuleConfigRunner Runner; 
	typedef ModuleConfigBuilder Builder; 
};

#endif /* MODULE_CONFIG_STRATEGY_HPP__ */
