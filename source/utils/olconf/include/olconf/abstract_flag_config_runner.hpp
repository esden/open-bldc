/*
 * olconf - yamlgen based Open-BLDC configuration header generator
 * Copyright (C) 2010 by Tobias Fuchs <twh.fuchs@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ABSTRACT_FLAG_CONFIG_RUNNER_HPP__
#define ABSTRACT_FLAG_CONFIG_RUNNER_HPP__

#include <olconf/flag_config_builder.hpp>
#include <olconf/flag_config_strategy.hpp>

#include <yamlgen/abstract_config_runner.hpp>


namespace YAMLGen { 
namespace OBLDC { 

class AbstractFlagConfigRunner : public AbstractConfigRunner<FlagConfigStrategy>
{

protected: 
	::std::string m_module; 

public: 
	AbstractFlagConfigRunner(::std::string const & module_name) 
	: m_module(module_name) { } 

	AbstractFlagConfigRunner()
	: m_module("GLOBAL") { } 

	virtual ~AbstractFlagConfigRunner() { } 

public: 
	virtual void run(FlagConfigBuilder * const builder) = 0; 

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* ABSTRACT_FLAG_CONFIG_RUNNER_HPP__ */
