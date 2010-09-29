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

#ifndef DEFINE_CONFIG_HPP__
#define DEFINE_CONFIG_HPP__

#include <olconf/define_config_strategy.hpp>

#include <yamlgen/abstract_config_builder.hpp>

#include <vector>
#include <algorithm>
#include <cctype>


namespace YAMLGen { 
namespace OBLDC { 

class DefineConfig 
{

private: 

	::std::string m_name; 
	::std::string m_value; 

public: 

	DefineConfig(::std::string const & name, ::std::string const & value) 
	: m_name(name), m_value(value) 
	{ }

public: 

	::std::string const & name(void) const { return m_name; } 
	::std::string const & value(void) const { return m_value; } 

};

} /* namespace ODLDC */
} /* namespace YAMLGen */

#endif /* DEFINE_CONFIG_HPP__ */
