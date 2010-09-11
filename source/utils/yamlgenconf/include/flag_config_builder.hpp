#ifndef FLAG_CONFIG_GENERATOR_STRATEGY_HPP__
#define FLAG_CONFIG_GENERATOR_STRATEGY_HPP__

#include "abstract_config_builder.hpp"
#include "register_group_config.hpp"
#include "flag_config.hpp"
#include "exception/generator_exception.hpp"
#include "flag_config_strategy.hpp"
#include <vector>


class FlagConfigBuilder : public AbstractConfigBuilder<FlagConfigStrategy>
{

public: 
// Could become necessary again: 
//	typedef FlagConfigStrategy Strategy; 

private: 

	::std::string m_module; 
	::std::vector<FlagConfig> m_flags; 

public: 

	FlagConfigBuilder() { } 

	virtual ~FlagConfigBuilder() { } 

	::std::vector<FlagConfig> const & flags() const { 
		return m_flags; 
	}

public: 

	virtual void parse(Interpreter const & interpreter) throw (GeneratorException); 
	virtual void run(AbstractFlagConfigRunner & runner) throw (RunnerException);

};

#endif /* FLAG_CONFIG_GENERATOR_STRATEGY_HPP__ */
