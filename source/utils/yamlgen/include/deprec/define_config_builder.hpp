#ifndef DEFINE_CONFIG_GENERATOR_STRATEGY_HPP__
#define DEFINE_CONFIG_GENERATOR_STRATEGY_HPP__

#include "abstract_config_builder.hpp"
#include "exception/builder_exception.hpp"
#include "define_config_strategy.hpp"
#include <vector>
#include <algorithm>
#include <cctype>

namespace YAMLGen { 
namespace OBLDC { 

class DefineConfigBuilder : public AbstractConfigBuilder<DefineConfigStrategy>
{

private: 

	::std::string m_module; 
	::std::map< ::std::string, ::std::string> m_defines; 

public: 

	DefineConfigBuilder() { } 

	virtual ~DefineConfigBuilder() { } 

	::std::map< ::std::string, ::std::string> const & defines() const { 
		return m_defines; 
	}

public: 

	void add_define(::std::string const & name, ::std::string const & value) { 
		::std::string up_name = name; 
		::std::transform(up_name.begin(), up_name.end(), up_name.begin(), 
										 (int(*)(int))::std::toupper);
		m_defines.insert(::std::pair< ::std::string, ::std::string>(up_name, value));
	}

public: 

	virtual void parse(ConfigNode const & config);
	virtual void parse_partial(ConfigNode const & config_node);
	virtual void run(AbstractDefineConfigRunner & runner);

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* DEFINE_CONFIG_GENERATOR_STRATEGY_HPP__ */
