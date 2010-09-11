#ifndef REGISTER_CONFIG_STRATEGY_HPP__
#define REGISTER_CONFIG_STRATEGY_HPP__

//#include "abstract_register_config_runner.hpp"
//#include "register_config_builder.hpp"

class AbstractRegisterConfigRunner; 
class RegisterConfigBuilder; 

class RegisterConfigStrategy
{
public: 
	typedef AbstractRegisterConfigRunner Runner; 
	typedef RegisterConfigBuilder Builder; 
};

#endif /* REGISTER_CONFIG_STRATEGY_HPP__ */
