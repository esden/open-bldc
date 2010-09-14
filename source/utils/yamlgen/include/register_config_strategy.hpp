#ifndef REGISTER_CONFIG_STRATEGY_HPP__
#define REGISTER_CONFIG_STRATEGY_HPP__

class AbstractRegisterConfigRunner; 
class RegisterConfigBuilder; 

class RegisterConfigStrategy
{
public: 
	typedef AbstractRegisterConfigRunner Runner; 
	typedef RegisterConfigBuilder Builder; 
};

#endif /* REGISTER_CONFIG_STRATEGY_HPP__ */
