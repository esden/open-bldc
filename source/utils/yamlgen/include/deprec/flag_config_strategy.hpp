#ifndef FLAG_CONFIG_STRATEGY_HPP__
#define FLAG_CONFIG_STRATEGY_HPP__

namespace YAMLGen { 
namespace OBLDC { 

class AbstractFlagConfigRunner; 
class FlagConfigBuilder; 

class FlagConfigStrategy
{
public: 
	typedef AbstractFlagConfigRunner Runner; 
	typedef FlagConfigBuilder Builder; 
};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* FLAG_CONFIG_STRATEGY_HPP__ */
