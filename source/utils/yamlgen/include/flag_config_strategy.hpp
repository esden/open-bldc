#ifndef FLAG_CONFIG_STRATEGY_HPP__
#define FLAG_CONFIG_STRATEGY_HPP__

class AbstractFlagConfigRunner; 
class FlagConfigBuilder; 

class FlagConfigStrategy
{
public: 
	typedef AbstractFlagConfigRunner Runner; 
	typedef FlagConfigBuilder Builder; 
};

#endif /* FLAG_CONFIG_STRATEGY_HPP__ */
