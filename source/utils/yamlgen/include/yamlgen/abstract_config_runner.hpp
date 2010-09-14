#ifndef ABSTRACT_CONFIG_RUNNER_HPP__
#define ABSTRACT_CONFIG_RUNNER_HPP__

#include "exception/runner_exception.hpp"


namespace YAMLGen { 

template <class TStrategy>
class AbstractConfigRunner
{
	typedef typename TStrategy::Builder Builder; 

public: 
	virtual ~AbstractConfigRunner() { } 

	virtual void run(Builder * const builder) = 0; 
};

} /* namespace YAMLGen */

#endif /* ABSTRACT_CONFIG_RUNNER_HPP__ */
