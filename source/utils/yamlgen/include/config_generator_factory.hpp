#ifndef CONFIG_GENERATOR_FACTORY_HPP__
#define CONFIG_GENERATOR_FACTORY_HPP__

#include "abstract_config_generator.hpp"

// #include "register_config_strategy.hpp"
// #include "flag_config_strategy.hpp"
// #include "define_config_strategy.hpp"

#include "config_generator.hpp"


template <class TGeneratorStrategy>
class ConfigGenerator; // : public AbstractConfigGenerator;

class ConfigGeneratorFactory { 

public:

	static AbstractConfigGenerator const * get(ModuleConfig const & module, 
								          									 ::std::string const & mode) 
	{ 
		if(mode == "register_groups") { 
			return new ConfigGenerator<RegisterConfigStrategy>(module.config());
		}
		else if(mode == "flags") { 
//			return new ConfigGenerator<FlagConfigStrategy>(module.config());
		}
		else if(mode == "define") { 
//			return new ConfigGenerator<DefineConfigStrategy>(module.config());
		}
	}

};

#endif
