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
#include "error_handling.hpp"

void on_parser_error(yaml_parser_t * parser) {

	switch(parser->error) {
		case YAML_MEMORY_ERROR: 
			fprintf(stderr, "Memory error\n");
			break;
		case YAML_READER_ERROR:
			fprintf(stderr, "Reader error\n");

			if (parser->problem_value != -1) {
				fprintf(stderr, "Reader error: %s: #%X at %d\n", parser->problem,
								parser->problem_value, parser->problem_offset);
			}
			else {
				fprintf(stderr, "Reader error: %s at %d\n", parser->problem,
								parser->problem_offset);
			}
			break; 
		case YAML_SCANNER_ERROR:
			fprintf(stderr, "Scanner error\n");
			if(parser->context) { 
				fprintf(stderr, "Scanner error: %s at line %d, column %d\n"
								"%s at line %d, column %d\n", 
								parser->context, 
								parser->context_mark.line+1, parser->context_mark.column+1,
								parser->problem, 
								parser->problem_mark.line+1, parser->problem_mark.column+1);
			}
			else {
				fprintf(stderr, "Scanner error: %s at line %d, column %d\n",
								parser->problem, 
								parser->problem_mark.line+1, parser->problem_mark.column+1);
			}
			break; 
		case YAML_PARSER_ERROR: 
			fprintf(stderr, "Parser error\n");
			if(parser->context) { 
				fprintf(stderr, "Parser error: %s at line %d, column %d\n"
								"%s at line %d, column %d\n", 
								parser->context, 
								parser->context_mark.line+1, parser->context_mark.column+1,
								parser->problem, 
								parser->problem_mark.line+1, parser->problem_mark.column+1);
			}
			else {
				fprintf(stderr, "Parser error: %s at line %d, column %d\n",
								parser->problem, 
								parser->problem_mark.line+1, parser->problem_mark.column+1);
			}
			break; 
		default: 
			fprintf(stderr, "Internal error: %s at line %d, column %d\n",
							parser->problem, 
							parser->problem_mark.line+1, parser->problem_mark.column+1);
			break;
	}
}

