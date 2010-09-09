#ifndef YAML_GENERATOR_REGISTER_CONFIG_STRATEGY_HPP__
#define YAML_GENERATOR_REGISTER_CONFIG_STRATEGY_HPP__

#include "abstract_yaml_generator_strategy.hpp"
#include "register_group_config.hpp"
#include "register_config.hpp"
#include "widget_config.hpp"
#include "flag_config.hpp"
#include <vector>

class YAMLGeneratorRegisterConfigStrategy : public AbstractYAMLGeneratorStrategy
{

private: 

	FlagConfig m_flags; 
	::std::vector<RegisterGroupConfig> m_register_groups; 

public: 

	YAMLGeneratorRegisterConfigStrategy() { } 
	virtual ~YAMLGeneratorRegisterConfigStrategy() { } 

public: 

	virtual void parse(YAMLInterpreter const & interpreter) throw (YAMLGeneratorException); 

};

#endif /* YAML_GENERATOR_REGISTER_CONFIG_STRATEGY_HPP__ */
