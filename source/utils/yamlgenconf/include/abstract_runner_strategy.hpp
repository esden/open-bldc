#ifndef ABSTRACT_RUNNER_STRATEGY_HPP__
#define ABSTRACT_RUNNER_STRATEGY_HPP__

#include "interpreter.hpp"
#include "runner_exception.hpp"
#include "register_group_config.hpp"
#include <vector>

class AbstractRunnerStrategy
{ 

public: 

	virtual void run(::std::vector<RegisterGroupConfig> const & register_groups) throw (RunnerException) = 0; 

};

#endif /* ABSTRACT_GENERATOR_STRATEGY_HPP__ */
