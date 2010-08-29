
#include <yaml.h>

#include "yaml_config.hpp"
#include "interpreter_exception.hpp"
#include "parser_exception.hpp"
#include "yaml_config.hpp"
#include "logging.hpp"

void
YAMLConfig::read(const char * filename) throw (ParserException, InterpreterException) 
{
	yaml_parser_t parser;
	yaml_event_t event;
	yaml_document_t document;
	int done = 0;

	memset(&parser,   0, sizeof(parser));
	memset(&event,    0, sizeof(event));
	memset(&document, 0, sizeof(document));

	if(!yaml_parser_initialize(&parser)) { on_parser_error(&parser); }
	FILE * input = fopen(filename, "rb");
	if(!input) { 
		fprintf(stderr, "Could not open file");
	}
	yaml_parser_set_input_file(&parser, input);

	/* Read the event sequence */
	while (!done) {

			/* Get the next event */
			if (!yaml_parser_parse(&parser, &event)) {
					on_parser_error(&parser);
					yaml_event_delete(&event);
					return 0; 
			}
	
			try { 
				done = (interpreter.next_event(&event) == Interpreter::DONE);
			} catch (InterpreterException ie) { 
				on_parse_error(&parser);
				fprintf(stderr, "ERROR: %s", ie.what());
			}

			/* The application is responsible for destroying the event object. */
			yaml_event_delete(&event);
	}

	/* Destroy the Parser object. */
	yaml_parser_delete(&parser);
}

void
YAMLConfig::on_parse_error(yaml_parser_t * parser) 
{ 
	switch(parser->error) {
		case YAML_MEMORY_ERROR: 
			throw ParserException("Memory error");
			break;
		case YAML_READER_ERROR:
			throw ParserException("Reader error");

			if (parser->problem_value != -1) {
				throw ParserException("Reader error", parser->problem, 
															parser->problem_value, parser->problem_offset);
			}
			else {
				throw ParserException("Reader error", parser->problem, 
															parser->problem_offset);
			}
			break; 
		case YAML_SCANNER_ERROR:
			if(parser->context) { 
				throw ParserException("Scanner error",
															parser->context, 
															parser->context_mark.line+1, parser->context_mark.column+1,
															parser->problem, 
															parser->problem_mark.line+1, parser->problem_mark.column+1);
			}
			else {
				throw ParserException("Scanner error",
															parser->problem, 
															parser->problem_mark.line+1, parser->problem_mark.column+1);
			}
			break; 
		case YAML_PARSER_ERROR: 
			if(parser->context) { 
				throw ParserException("Parser error",
															parser->context, 
															parser->context_mark.line+1, parser->context_mark.column+1,
															parser->problem, 
															parser->problem_mark.line+1, parser->problem_mark.column+1);
			}
			else {
				throw ParserException("Parser error",
															parser->problem, 
															parser->problem_mark.line+1, parser->problem_mark.column+1);
			}
			break; 
		default: 
			throw ParserException("Internal error");
	}
}
