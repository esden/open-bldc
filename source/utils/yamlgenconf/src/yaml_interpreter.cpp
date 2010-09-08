/**
  Simplicistic implementation of an interpreter based on libyaml
	as a state machine mapping libyaml parser events to an config
	instances tree like:
	- Config
	   [ RegisterGroupConfig
		 		[ RegisterConfig
					[ WidgetConfig ]
				]+
		 ]+
*/

#include "yaml_interpreter.hpp"
#include "yaml_interpreter_exception.hpp"
#include "logging.hpp"
#include <yaml.h>


void YAMLInterpreter::init_mode(yaml_event_t * event) {
	LOG_DEBUG_PRINT(" -> Handling INIT");
  if(event->type != YAML_MAPPING_START_EVENT &&
     event->type != YAML_STREAM_START_EVENT &&
     event->type != YAML_DOCUMENT_START_EVENT) {
    throw YAMLInterpreterException(event, "INIT");
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
YAMLInterpreter::mapping_start_mode(yaml_event_t * event) { 
	LOG_DEBUG_PRINT(" -> Handling MAPPING_START");
  if(event->type != YAML_SCALAR_EVENT) { 
    throw YAMLInterpreterException(event, "MAPPING_START");
  }
	
	LOG_DEBUG_PRINT("    key: %s", (const char *)(event->data.scalar.value));
	m_key_stack.push_back(::std::string((const char *)(event->data.scalar.value)));

	m_node_stack.push_back(m_cur_node); 
	m_cur_node = ConfigNode(); 
	
	LOG_DEBUG_PRINT(" -> Transition to VALUE");
	m_mode = VALUE; 
}

void 
YAMLInterpreter::mapping_end_mode(yaml_event_t * event) { 
	LOG_DEBUG_PRINT(" -> Handling MAPPING_END");
  if(event->type != YAML_MAPPING_END_EVENT &&
  	 event->type != YAML_DOCUMENT_END_EVENT && 
  	 event->type != YAML_SCALAR_EVENT) { 
    throw YAMLInterpreterException(event, "MAPPING_END");
  }
	if(event->type == YAML_DOCUMENT_END_EVENT) { 
		LOG_DEBUG_PRINT(" -> Transition to COMPLETING");
		m_mode = COMPLETING; 
	}
	else if(event->type == YAML_MAPPING_END_EVENT) { 
		// Current config node finished, assign to parent node
		ConfigNode parent_node = m_node_stack.back(); 
		::std::string cur_key  = m_key_stack.back(); 
		parent_node.set_node(cur_key, m_cur_node);
		m_cur_node = parent_node; 
		
		m_node_stack.pop_back(); 
		m_key_stack.pop_back(); 
		
		LOG_DEBUG_PRINT(" -> Transition to KEY");
		m_mode = KEY; 
	}
	else if(event->type == YAML_SCALAR_EVENT) { 
		LOG_DEBUG_PRINT("    key: %s", (const char *)(event->data.scalar.value));

		// Current config node finished, assign to parent node
		ConfigNode parent_node = m_node_stack.back(); 
		::std::string cur_key  = m_key_stack.back(); 
		parent_node.set_node(cur_key, m_cur_node);
		m_cur_node = parent_node; 
		
		m_node_stack.pop_back(); 
		m_key_stack.pop_back(); 
		
		// Assign this key to parent node
		m_key_stack.push_back(::std::string((const char *)(event->data.scalar.value)));
		
		LOG_DEBUG_PRINT(" -> Map Transition to VALUE");
		m_mode = VALUE; 
	}
}

void 
YAMLInterpreter::key_mode(yaml_event_t * event) { 
	LOG_DEBUG_PRINT(" -> Handling KEY");
  if(event->type != YAML_SCALAR_EVENT &&
  	 event->type != YAML_DOCUMENT_END_EVENT && 
  	 event->type != YAML_MAPPING_END_EVENT) { 
    throw YAMLInterpreterException(event, "KEY");
  }

	if(event->type == YAML_SCALAR_EVENT) { 
		LOG_DEBUG_PRINT("    key: %s", (const char *)(event->data.scalar.value));
		m_key_stack.push_back(::std::string((const char *)(event->data.scalar.value)));
		
		LOG_DEBUG_PRINT(" -> Transition to VALUE");
		m_mode = VALUE; 
	} 
	else if(event->type == YAML_MAPPING_END_EVENT) { 
		LOG_DEBUG_PRINT(" -> Transition to MAPPING_END");
		m_mode = MAPPING_END;
	}
	else if(event->type == YAML_DOCUMENT_END_EVENT) { 
		LOG_DEBUG_PRINT(" -> Transition to COMPLETING");
		m_mode = COMPLETING;
	}
}

void 
YAMLInterpreter::value_mode(yaml_event_t * event) { 
	LOG_DEBUG_PRINT(" -> Handling VALUE");
  if(event->type != YAML_SCALAR_EVENT && 
     event->type != YAML_MAPPING_START_EVENT) { 
    throw YAMLInterpreterException(event, "VALUE");
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
YAMLInterpreter::completing_mode(yaml_event_t * event) {
	log(); 
}

YAMLInterpreter::interpreter_mode_t
YAMLInterpreter::next_event(yaml_event_t * event) throw (YAMLInterpreterException)
{
	switch(event->type) { 
		case YAML_STREAM_START_EVENT: 
			LOG_DEBUG_EVT("STREAM_START_EVENT");
			break; 
		case YAML_DOCUMENT_START_EVENT: 
			LOG_DEBUG_EVT("DOCUMENT_START_EVENT");
			break; 
		case YAML_MAPPING_START_EVENT: 
			LOG_DEBUG_EVT("MAPPING_START_EVENT");
			break; 
		case YAML_MAPPING_END_EVENT: 
			LOG_DEBUG_EVT("MAPPING_END_EVENT");
			break; 
		case YAML_SCALAR_EVENT: 
			LOG_DEBUG_EVT("SCALAR_EVENT");
			break; 

		case YAML_DOCUMENT_END_EVENT: 
			LOG_DEBUG_EVT("DOCUMENT_END_EVENT");
			break; 
		case YAML_STREAM_END_EVENT: 
			LOG_DEBUG_EVT("STREAM_END_EVENT");
			break; 

		case YAML_NO_EVENT: 
			LOG_DEBUG_EVT("NO_EVENT");
			return CONTINUE; 
	}

  if (event->type == YAML_STREAM_END_EVENT) {
    return DONE;
  }
  (this->*m_mode_handlers[m_mode])(event);
  return CONTINUE;
}
