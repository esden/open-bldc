/**
  Simplicistic implementation of an interpreter based on libyaml
	as a state machine mapping libyaml parser events to a ConfigNode
	tree. 
*/

#include <yaml.h>

#include "interpreter.hpp"
#include "logging.hpp"
#include "exception/interpreter_exception.hpp"
#include "exception/parser_exception.hpp"
#include <yaml.h>


void Interpreter::init_mode(yaml_event_t * event) 
throw (InterpreterException) 
{ 
	LOG_DEBUG_PRINT(" -> Handling INIT");
  if(event->type != YAML_MAPPING_START_EVENT &&
     event->type != YAML_STREAM_START_EVENT &&
     event->type != YAML_DOCUMENT_START_EVENT) {
    throw InterpreterException(event, "INIT");
  }

  // Wait for first mapping (global scope):
  if(event->type == YAML_MAPPING_START_EVENT) {
		m_cur_node = ConfigNode(); 
		m_key_stack.push_back( ::std::string("config_root"));

		LOG_DEBUG_PRINT(" -> Transition to MAPPING_START");
    m_mode = MAPPING_START;
  }
}

void 
Interpreter::mapping_start_mode(yaml_event_t * event) 
throw (InterpreterException) 
{ 
	LOG_DEBUG_PRINT(" -> Handling MAPPING_START");
  if(event->type != YAML_SCALAR_EVENT) { 
    throw InterpreterException(event, "MAPPING_START");
  }
	
	LOG_DEBUG_PRINT("    key: %s", (const char *)(event->data.scalar.value));
	m_key_stack.push_back(::std::string((const char *)(event->data.scalar.value)));

	LOG_DEBUG_PRINT("    PUSH node");
	m_node_stack.push_back(m_cur_node); 
	m_cur_node = ConfigNode(); 
	
	LOG_DEBUG_PRINT(" -> Transition to VALUE");
	m_mode = VALUE; 
}

void 
Interpreter::key_mode(yaml_event_t * event) 
throw (InterpreterException) 
{ 
	LOG_DEBUG_PRINT(" -> Handling KEY");
  if(event->type != YAML_SCALAR_EVENT &&
  	 event->type != YAML_DOCUMENT_END_EVENT && 
  	 event->type != YAML_MAPPING_END_EVENT) { 
    throw InterpreterException(event, "KEY");
  }

	if(event->type == YAML_SCALAR_EVENT) { 
		LOG_DEBUG_PRINT("    key: %s", (const char *)(event->data.scalar.value));
		::std::string key = ::std::string((const char *)(event->data.scalar.value));
		m_key_stack.push_back(key);
		LOG_DEBUG_PRINT(" -> Transition to VALUE");
		m_mode = VALUE; 
	} 
	else if(event->type == YAML_MAPPING_END_EVENT) { 
		LOG_DEBUG_PRINT(" -> Transition to MAPPING_END");
		ConfigNode parent_node = m_node_stack.back(); 
		::std::string cur_key  = m_key_stack.back(); 
		parent_node.set_node(cur_key, m_cur_node);
		m_cur_node = parent_node; 
		
		LOG_DEBUG_PRINT("    POP node");
		m_node_stack.pop_back(); 
		m_key_stack.pop_back(); 
		
		m_mode = KEY;
	}
	else if(event->type == YAML_DOCUMENT_END_EVENT) { 
		LOG_DEBUG_PRINT(" -> Transition to COMPLETING");
		m_mode = COMPLETING;
	}
}

void 
Interpreter::value_mode(yaml_event_t * event) 
throw (InterpreterException) 
{ 
	LOG_DEBUG_PRINT(" -> Handling VALUE");
  if(event->type != YAML_SCALAR_EVENT && 
     event->type != YAML_MAPPING_START_EVENT && 
		 event->type != YAML_SEQUENCE_START_EVENT) { 
    throw InterpreterException(event, "VALUE");
  }

	if(event->type == YAML_SCALAR_EVENT) { 
		m_cur_node.set_value(m_key_stack.back(), (const char *)(event->data.scalar.value));
		m_key_stack.pop_back(); 

		LOG_DEBUG_PRINT("    value: %s", (const char *)(event->data.scalar.value));
		LOG_DEBUG_PRINT(" -> Transition to KEY");
		m_mode = KEY; 
	}
	else if(event->type == YAML_MAPPING_START_EVENT) { 
		LOG_DEBUG_PRINT(" -> Transition to MAPPING_START");
		m_mode = MAPPING_START; 
	}
	else if(event->type == YAML_SEQUENCE_START_EVENT) { 
		LOG_DEBUG_PRINT(" -> Transition to SEQUENCE_START");
		m_mode     = SEQUENCE_START; 
	}
	
}

void 
Interpreter::sequence_start_mode(yaml_event_t * event) 
throw (InterpreterException) 
{
	LOG_DEBUG_PRINT(" -> Handling SEQUENCE");
  if(event->type != YAML_SCALAR_EVENT && 
  	 event->type != YAML_SEQUENCE_END_EVENT) { 
    throw InterpreterException(event, "SEQUENCE");
	}
	if(event->type == YAML_SCALAR_EVENT) { 
		::std::string seq_value = ::std::string((const char *)(event->data.scalar.value));
		m_cur_node.push_seq_value(m_key_stack.back(), seq_value); 
		LOG_DEBUG_PRINT("    Sequence value: %s -> %s",
										m_key_stack.back().c_str(), seq_value.c_str());
	}
	else if(event->type == YAML_SEQUENCE_END_EVENT) { 
		m_mode = KEY; 
		m_key_stack.pop_back(); 
	}
}

void
Interpreter::completing_mode(yaml_event_t * event) 
throw (InterpreterException) 
{ 
}

Interpreter::interpreter_mode_t
Interpreter::next_event(yaml_event_t * event) 
throw (InterpreterException, ParserException)
{
  if (event->type == YAML_STREAM_END_EVENT) {
    return DONE;
  }
  (this->*m_mode_handlers[m_mode])(event);
  return CONTINUE;
}

void
Interpreter::read(const char * filename) 
throw (InterpreterException, ParserException)
{ 
	yaml_parser_t parser;
	yaml_event_t event;
	yaml_document_t document;
	int done = 0;

	memset(&parser,   0, sizeof(parser));
	memset(&event,    0, sizeof(event));
	memset(&document, 0, sizeof(document));

	if(!yaml_parser_initialize(&parser)) { throw ParserException(&parser); }
	FILE * input = fopen(filename, "rb");
	if(!input) { 
		throw InterpreterException("Could not open file");
	}
	yaml_parser_set_input_file(&parser, input);

	/* Read the event sequence */
	while (!done) {

		/* Get the next event */
		if (!yaml_parser_parse(&parser, &event)) {
			yaml_event_delete(&event);
			throw ParserException(&parser);
		}
		done = (next_event(&event) == Interpreter::DONE);
		
		/* The application is responsible for destroying the event object. */
		yaml_event_delete(&event);
	}
	/* Destroy the Parser object. */
	yaml_parser_delete(&parser);
}



