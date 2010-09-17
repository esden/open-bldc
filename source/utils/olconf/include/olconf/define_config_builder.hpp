#ifndef DEFINE_CONFIG_GENERATOR_STRATEGY_HPP__
#define DEFINE_CONFIG_GENERATOR_STRATEGY_HPP__

#include <olconf/define_config_strategy.hpp>
#include <olconf/define_config.hpp>

#include <yamlgen/abstract_config_builder.hpp>

#include <vector>
#include <algorithm>
#include <cctype>


namespace YAMLGen { 
namespace OBLDC { 

class DefineConfigBuilder : public AbstractConfigBuilder<DefineConfigStrategy>
{

private: 

	::std::string m_module; 
	::std::vector<DefineConfig> m_defines; 

public: 

	DefineConfigBuilder() { } 

	virtual ~DefineConfigBuilder() { } 

	::std::vector<DefineConfig> const & defines() const { 
		return m_defines; 
	}

public: 

	void add_define(::std::string const & name, ::std::string const & value) { 
		::std::string up_name = name; 
		::std::transform(up_name.begin(), up_name.end(), up_name.begin(), 
										 (int(*)(int))::std::toupper);
		DefineConfig config = DefineConfig(name, value); 
		m_defines.push_back(config); 
	}

public: 

	virtual void parse(ConfigNode const & config);
	virtual void parse_partial(ConfigNode const & config_node);
	virtual void run(AbstractDefineConfigRunner & runner);

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* DEFINE_CONFIG_GENERATOR_STRATEGY_HPP__ */
