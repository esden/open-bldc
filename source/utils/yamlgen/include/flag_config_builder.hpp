#ifndef FLAG_CONFIG_BUILDER_HPP__
#define FLAG_CONFIG_BUILDER_HPP__

#include "abstract_config_builder.hpp"
#include "register_group_config.hpp"
#include "flag_config.hpp"
#include "exception/builder_exception.hpp"
#include "flag_config_strategy.hpp"
#include <vector>


class FlagConfigBuilder : public AbstractConfigBuilder<FlagConfigStrategy>
{

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

	virtual void parse(ConfigNode const & config);
	virtual void parse_partial(ConfigNode const & config_node);
	virtual void run(AbstractFlagConfigRunner & runner);

};

#endif /* FLAG_CONFIG_BUILDER_HPP__ */
