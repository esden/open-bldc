#ifndef REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__
#define REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__

#include "abstract_config_builder.hpp"
#include "register_group_config.hpp"
#include "flag_config.hpp"
#include "exception/generator_exception.hpp"
#include <vector>


class RegisterConfigBuilder : public AbstractConfigBuilder
{

private: 

	::std::string m_module; 
	FlagConfig m_flags; 
	::std::vector<RegisterGroupConfig> m_register_groups; 

public: 

	RegisterConfigBuilder() { 
		m_module = "CPROTMOD";
	} 
	RegisterConfigBuilder(AbstractConfigBuilder & abs) { } 
	virtual ~RegisterConfigBuilder() { } 

	::std::vector<RegisterGroupConfig> const & register_groups() const { 
		return m_register_groups; 
	}
	::std::string const & module() const {
		return m_module;
	}

public: 

	virtual void parse(Interpreter const & interpreter) throw (GeneratorException); 

};

#endif /* REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__ */
