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

#ifndef MODULE_CONFIG_STRATEGY_HPP__
#define MODULE_CONFIG_STRATEGY_HPP__

namespace YAMLGen { 
namespace OBLDC { 

class AbstractModuleConfigRunner; 
class ModuleConfigBuilder; 

class ModuleConfigStrategy
{
public: 
	typedef AbstractModuleConfigRunner Runner; 
	typedef ModuleConfigBuilder Builder; 
};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif /* MODULE_CONFIG_STRATEGY_HPP__ */
