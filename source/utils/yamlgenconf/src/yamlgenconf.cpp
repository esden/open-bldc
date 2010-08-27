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
#include "logging.h"
#include "error_handling.h"
#include "interpreter.h"

void usage(void);
void on_error(void);

static yaml_parser_t parser;
static yaml_event_t event;
static yaml_emitter_t emitter;
static yaml_document_t document;

int main(int argc, char * argv[]) {

	int done = 0;

	if(argc < 2) { 
		usage(); 
		return 1; 
	}

	memset(&parser,   0, sizeof(parser));
	memset(&event,    0, sizeof(event));
	memset(&emitter,  0, sizeof(emitter));
	memset(&document, 0, sizeof(document));

	/* Create the Parser object */
	if(!yaml_parser_initialize(&parser)) { on_parser_error(&parser); }
	if(!yaml_emitter_initialize(&emitter)) { on_emitter_error(&emitter); }

	/* Set a file input */
	FILE * input = fopen(argv[1], "rb");
	if(!input) { 
		fprintf(stderr, "Could not open file");
	}

	yaml_parser_set_input_file(&parser, input);
	yaml_emitter_set_output_file(&emitter, stdout); 

	/* Read the event sequence */
	while (!done) {

			/* Get the next event */
			if (!yaml_parser_parse(&parser, &event)) {
					on_parser_error(&parser);
					on_error(); 
					yaml_event_delete(&event);
					return 0; 
			}
	
			done = (interpret_next_event(&event) == DONE);

			/* The application is responsible for destroying the event object. */
			yaml_event_delete(&event);
	}

	/* Destroy the Parser object. */
	yaml_parser_delete(&parser);
	yaml_emitter_delete(&emitter);

	return 1;
}

void on_error(void) { 
	yaml_parser_delete(&parser);
	yaml_emitter_delete(&emitter);
}

void usage(void) { 
	printf("OpenBLDC - yamlgenconf\n"
		   "YAML based register configuration generator\n\n"
		   "Usage: \n\n"
		   "  yamlgenconf <yaml config file>\n\n");
}


