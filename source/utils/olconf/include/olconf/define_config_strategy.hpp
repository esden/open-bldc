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

#ifndef DEFINE_CONFIG_STRATEGY_HPP__
#define DEFINE_CONFIG_STRATEGY_HPP__

namespace YAMLGen { 
namespace OBLDC { 

class AbstractDefineConfigRunner; 
class DefineConfigBuilder; 

class DefineConfigStrategy
{
public: 
	typedef AbstractDefineConfigRunner Runner; 
	typedef DefineConfigBuilder Builder; 
};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* DEFINE_CONFIG_STRATEGY_HPP__ */
