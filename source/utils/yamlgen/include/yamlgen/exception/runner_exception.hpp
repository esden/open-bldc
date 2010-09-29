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

#ifndef RUNNER_EXCEPTION_HPP__
#define RUNNER_EXCEPTION_HPP__

#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <yaml.h>


namespace YAMLGen { 

class RunnerException : public ::std::exception 
{

private: 
	const char * m_what; 

public: 
	RunnerException(const char * what) throw()
	: m_what(what)
	{ } 

public: 
	virtual ~RunnerException() throw () { } 

public:
	
	virtual const char * what() const throw () { 
		return m_what; 
	}

};

} /* namespace YAMLGen */

#endif /* RUNNER_EXCEPTION_HPP__ */
