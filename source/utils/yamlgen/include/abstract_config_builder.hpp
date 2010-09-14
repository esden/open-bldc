#ifndef ABSTRACT_CONFIG_BUILDER_HPP__
#define ABSTRACT_CONFIG_BUILDER_HPP__

#include "interpreter.hpp"
#include "abstract_config_runner.hpp"
#include "exception/builder_exception.hpp"
#include "exception/runner_exception.hpp"


template <class TStrategy>
class AbstractConfigBuilder
{ 
	typedef typename TStrategy::Runner Runner; 

public: 
	virtual ~AbstractConfigBuilder() { } 

public: 
	virtual void parse(ConfigNode const & config) = 0;
	
	/** Expects ConfigNode as section from interpreter
	 */
	virtual void parse_partial(ConfigNode const & config_node) = 0;

	virtual void run(Runner & runner) = 0;
};

#endif /* ABSTRACT_CONFIG_BUILDER_HPP__ */
