#ifndef DEFINE_CONFIG_STRATEGY_HPP__
#define DEFINE_CONFIG_STRATEGY_HPP__

namespace YAMLGen { 
namespace OBLDC { 

class AbstractDefineConfigRunner; 
class DefineConfigBuilder; 

class DefineConfigStrategy
{
public: 
	typedef AbstractDefineConfigRunner Runner; 
	typedef DefineConfigBuilder Builder; 
};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* DEFINE_CONFIG_STRATEGY_HPP__ */
