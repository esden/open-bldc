/**
  Simplicistic implementation of an interpreter based on libyaml
	as a state machine mapping libyaml parser events to a ConfigNode
	tree. 
*/

#include "interpreter.hpp"
#include "logging.hpp"
#include "exception/interpreter_exception.hpp"
#include <yaml.h>


void Interpreter::init_mode(yaml_event_t * event) {
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
Interpreter::mapping_start_mode(yaml_event_t * event) { 
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
Interpreter::key_mode(yaml_event_t * event) { 
	LOG_DEBUG_PRINT(" -> Handling KEY");
  if(event->type != YAML_SCALAR_EVENT &&
  	 event->type != YAML_DOCUMENT_END_EVENT && 
  	 event->type != YAML_MAPPING_END_EVENT) { 
    throw InterpreterException(event, "KEY");
  }

	if(event->type == YAML_SCALAR_EVENT) { 
		LOG_DEBUG_PRINT("    key: %s", (const char *)(event->data.scalar.value));
		m_key_stack.push_back(::std::string((const char *)(event->data.scalar.value)));
		
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
Interpreter::value_mode(yaml_event_t * event) { 
	LOG_DEBUG_PRINT(" -> Handling VALUE");
  if(event->type != YAML_SCALAR_EVENT && 
     event->type != YAML_MAPPING_START_EVENT) { 
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
	
}

void
Interpreter::completing_mode(yaml_event_t * event) {
}

Interpreter::interpreter_mode_t
Interpreter::next_event(yaml_event_t * event) throw (InterpreterException)
{
  if (event->type == YAML_STREAM_END_EVENT) {
    return DONE;
  }
  (this->*m_mode_handlers[m_mode])(event);
  return CONTINUE;
}
