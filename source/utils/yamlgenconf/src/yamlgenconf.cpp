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

#include <yaml.h>
#include "yaml_config.hpp"
#include "yaml_interpreter_exception.hpp"
#include "parser_exception.hpp"

void usage(void);

int main(int argc, char * argv[]) {

	if(argc < 2) { 
		usage(); 
		return 1; 
	}

	YAMLConfig config;
	try { 
		config.read(argv[1]);
	} catch(ParserException pe) { 
		fprintf(stderr, "%s\n", pe.what());
	} catch(YAMLInterpreterException ie) { 
		fprintf(stderr, "%s\n", ie.what());
	}

	config.log(); 

}

void usage(void) { 
	printf("OpenBLDC - yamlgenconf\n"
		   "YAML based register configuration generator\n\n"
		   "Usage: \n\n"
		   "  yamlgenconf <yaml config file>\n\n");
}


