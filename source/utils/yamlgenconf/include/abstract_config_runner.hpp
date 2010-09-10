#ifndef ABSTRACT_CONFIG_RUNNER_HPP__
#define ABSTRACT_CONFIG_RUNNER_HPP__

#include "exception/runner_exception.hpp"

class AbstractConfigBuilder; // fwd-declare

class AbstractConfigRunner
{
public: 
	virtual ~AbstractConfigRunner() { } 

	virtual void run(AbstractConfigBuilder * builder) throw (RunnerException) = 0; 
};

#endif /* ABSTRACT_CONFIG_RUNNER_HPP__ */
