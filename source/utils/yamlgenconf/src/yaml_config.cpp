
#include <yaml.h>

#include "yaml_config.hpp"
#include "yaml_config.hpp"
#include "logging.hpp"

#include "config_generator.hpp"
#include "register_config_strategy.hpp"
#include "register_config_header_runner.hpp"

#include "exception/interpreter_exception.hpp"
#include "exception/parser_exception.hpp"
#include "exception/config_exception.hpp"

void
YAMLConfig::read(char const * filename) throw (ParserException, InterpreterException) 
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
			return; 
		}
		
		done = (m_yaml_interpreter.next_event(&event) == Interpreter::DONE);
		
		/* The application is responsible for destroying the event object. */
		yaml_event_delete(&event);
	}

	/* Destroy the Parser object. */
	yaml_parser_delete(&parser);

	ConfigGenerator<RegisterConfigStrategy> generator(m_yaml_interpreter); 

	RegisterConfigHeaderRunner runner_strategy; 
	generator.run(runner_strategy); 

/* 
	Note that a ConfigGenerator can parse several 
	interpreters, and you can run several runners 
	on the same configurator: 

		config_generator.parse(first_interpreter); 
		config_generator.parse(second_interpreter); 

		config_generator.run(first_strategy); 
		config_generator.run(second_strategy); 
		config_generator.run(third_strategy); 
*/

}

void
YAMLConfig::on_parse_error(yaml_parser_t * parser) 
{ 
	throw ParserException(parser); 
}

