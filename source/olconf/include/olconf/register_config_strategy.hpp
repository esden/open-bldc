#ifndef REGISTER_CONFIG_STRATEGY_HPP__
#define REGISTER_CONFIG_STRATEGY_HPP__

namespace YAMLGen { 
namespace OBLDC { 

class AbstractRegisterConfigRunner; 
class RegisterConfigBuilder; 

class RegisterConfigStrategy
{
public: 
	typedef AbstractRegisterConfigRunner Runner; 
	typedef RegisterConfigBuilder Builder; 
};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* REGISTER_CONFIG_STRATEGY_HPP__ */
