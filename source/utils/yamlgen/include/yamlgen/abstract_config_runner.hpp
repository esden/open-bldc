/*
 * yamlgen - YAML to everything generator framework
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

#ifndef ABSTRACT_CONFIG_RUNNER_HPP__
#define ABSTRACT_CONFIG_RUNNER_HPP__


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
