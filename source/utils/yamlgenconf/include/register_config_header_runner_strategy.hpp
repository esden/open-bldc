#ifndef REGISTER_CONFIG_HEADER_RUNNER_STRATEGY_HPP__
#define REGISTER_CONFIG_HEADER_RUNNER_STRATEGY_HPP__

#include "abstract_runner_strategy.hpp"
#include "register_group_config.hpp"
#include "runner_exception.hpp"
#include <vector>

class RegisterConfigHeaderRunnerStrategy : public AbstractRunnerStrategy
{ 

public: 

	RegisterConfigHeaderRunnerStrategy() { } 
	virtual ~RegisterConfigHeaderRunnerStrategy() { } 

	virtual void run(::std::vector<RegisterGroupConfig> const & register_groups) throw (RunnerException);

};


#endif /* REGISTER_CONFIG_HEADER_RUNNER_STRATEGY_HPP__ */
