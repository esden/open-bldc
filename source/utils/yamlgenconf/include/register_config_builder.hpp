#ifndef REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__
#define REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__

#include "abstract_config_builder.hpp"
#include "register_group_config.hpp"
#include "flag_config.hpp"
#include "exception/generator_exception.hpp"
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

	virtual void parse(Interpreter const & interpreter) 
		throw (GeneratorException); 

	/** Expects ConfigNode containing register groups
	 */
	virtual void parse(ConfigNode const & config_node) 
		throw (GeneratorException); 

	virtual void run(AbstractRegisterConfigRunner & runner) 
		throw (RunnerException);

};

#endif /* REGISTER_CONFIG_GENERATOR_STRATEGY_HPP__ */
