#ifndef DEFINE_CONFIG_STRATEGY_HPP__
#define DEFINE_CONFIG_STRATEGY_HPP__

class AbstractDefineConfigRunner; 
class DefineConfigBuilder; 

class DefineConfigStrategy
{
public: 
	typedef AbstractDefineConfigRunner Runner; 
	typedef DefineConfigBuilder Builder; 
};

#endif /* DEFINE_CONFIG_STRATEGY_HPP__ */
