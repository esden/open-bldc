/*
 * OpenBLDC yamlgenconfig - YAML-based configuration for OpenBLDC
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

#include "yaml_config.hpp"
#include "exception/interpreter_exception.hpp"
#include "exception/parser_exception.hpp"
#include "exception/config_exception.hpp"
#include "exception/builder_exception.hpp"

#include <yaml.h>
#include <exception>


void usage(void);

int main(int argc, char * argv[]) {

	if(argc < 2) { 
		usage(); 
		return 1; 
	}

	try { 
		YAMLConfig config;
		config.read(argv[1]);
		return 0; 
	} 
	catch(ParserException pe) { 
		::std::cerr << "Syntax error: " << ::std::endl;
		::std::cerr << "   " << pe.what() << ::std::endl;
	} 
	catch(InterpreterException ie) { 
		::std::cerr << "Interpreter error: " << ::std::endl;
		::std::cerr << "   " << ie.what() << ::std::endl;
	}
	catch(BuilderException be) { 
		::std::cerr << "Builder error: " << ::std::endl;
		::std::cerr << "   " << be.what() << ::std::endl;
	}
	catch(::std::exception se) {
		::std::cerr << "Error: " << ::std::endl;
		::std::cerr << "   " << se.what() << ::std::endl;
	}
	return 1; 

}

void usage(void) { 
	printf("yamlgen\n"
		   "YAML based generator\n\n"
		   "Usage: \n\n"
		   "  yamlgen <yaml config file>\n\n");
}


