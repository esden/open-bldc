#ifndef YAMLGEN__OBLDC__REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__
#define YAMLGEN__OBLDC__REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__

#include <olconf/register_group_config.hpp>
#include <olconf/flag_config.hpp>
#include <olconf/register_config_strategy.hpp>

#include <yamlgen/abstract_config_builder.hpp>

#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

class RegisterConfigBuilder : public AbstractConfigBuilder<RegisterConfigStrategy>
{

private: 

	::std::string m_module; 
	::std::vector<RegisterGroupConfig> m_register_groups; 

public: 

	RegisterConfigBuilder() { } 

	virtual ~RegisterConfigBuilder() { } 

	::std::vector<RegisterGroupConfig> const & register_groups() const { 
		return m_register_groups; 
	}

public: 

	virtual void parse(ConfigNode const & config);
	virtual void parse_partial(ConfigNode const & config_node);
	virtual void run(AbstractRegisterConfigRunner & runner);

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* YAMLGEN__OBLDC__REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__ */
