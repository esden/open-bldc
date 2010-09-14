#ifndef REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__
#define REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__

#include "abstract_config_builder.hpp"
#include "register_group_config.hpp"
#include "flag_config.hpp"
#include "exception/builder_exception.hpp"
#include "register_config_strategy.hpp"
#include <vector>


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

#endif /* REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__ */
