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

#ifndef YAMLGEN__OBLDC__REGISTER_CONFIG_HEADER_RUNNER_HPP__
#define YAMLGEN__OBLDC__REGISTER_CONFIG_HEADER_RUNNER_HPP__

#include <olconf/abstract_register_config_runner.hpp>
#include <olconf/register_group_config.hpp>

#include <vector>


namespace YAMLGen { 
namespace OBLDC { 

class RegisterConfigHeaderRunner: public AbstractRegisterConfigRunner
{ 

public: 

	RegisterConfigHeaderRunner() { } 
	
	RegisterConfigHeaderRunner(::std::string const & module_name) 
	: AbstractRegisterConfigRunner(module_name) { } 

	virtual ~RegisterConfigHeaderRunner() { } 
	
	virtual void run(RegisterConfigBuilder * const builder);

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* YAMLGEN__OBLDC__REGISTER_CONFIG_HEADER_RUNNER_HPP__ */
