
#include <yaml.h>

#include "yaml_config.hpp"
#include "yaml_interpreter_exception.hpp"
#include "parser_exception.hpp"
#include "yaml_config.hpp"
#include "logging.hpp"
#include "abstract_yaml_generator_strategy.hpp"
#include "yaml_generator_register_config_strategy.hpp"
#include "config_generator.hpp"

void
YAMLConfig::read(char const * filename) throw (ParserException, YAMLInterpreterException) 
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
		
		done = (m_yaml_interpreter.next_event(&event) == YAMLInterpreter::DONE);
		
		/* The application is responsible for destroying the event object. */
		yaml_event_delete(&event);
	}

	/* Destroy the Parser object. */
	yaml_parser_delete(&parser);

	YAMLGeneratorRegisterConfigStrategy generator_strategy; 
	ConfigGenerator config_generator = ConfigGenerator(&generator_strategy);
	config_generator.parse(m_yaml_interpreter); 

}

void
YAMLConfig::on_parse_error(yaml_parser_t * parser) 
{ 
	throw ParserException(parser); 
}

