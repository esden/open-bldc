
#include "interpreter.h"
#include "logging.h"

static interpreter_t interpreter;

static inline void interpret_init_mode(yaml_event_t * event);

void interpreter_initialize() { 
	memset(&interpreter, 0, sizeof(interpreter));
	interpreter.mode = INIT;
}

/* 
  INIT -> REGISTER_LIST on event YAML_MAPPING_START_EVENT
*/
static inline void interpret_init_mode(yaml_event_t * event) { 
	if(event->type != YAML_MAPPING_START_EVENT) return;

	interpreter.mode = REGISTER_LIST; 
	LOG_PRINT("  Mapping start (tag: %s, anchor: %s)", 
						event->data.mapping_start.tag, 
						event->data.mapping_start.anchor);
}
static inline void interpret_register_list_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT) return; 

	interpreter.mode = REGISTER_CONFIG; 
	LOG_PRINT("  Scalar '%s' (tag: %s, anchor: %s)", 
						event->data.scalar.value, 
						event->data.mapping_start.tag, 
						event->data.mapping_start.anchor);
}
static inline void interpret_register_config_mode(yaml_event_t * event) { 
	if(event->type != YAML_MAPPING_START_EVENT) return;

	
}

enum interpreter_mode interpret_next_event(yaml_event_t * event) {

	switch(event->type) { 
		case YAML_NO_EVENT: 
			break; 
		case YAML_ALIAS_EVENT:
			LOG_DEBUG(YAML_ALIAS_EVENT);
			break; 
		case YAML_DOCUMENT_START_EVENT:
			LOG_DEBUG(YAML_DOCUMENT_START_EVENT);
			break; 
		case YAML_DOCUMENT_END_EVENT:
			LOG_DEBUG(YAML_DOCUMENT_END_EVENT);
			break; 
		case YAML_SEQUENCE_START_EVENT: 
			LOG_DEBUG(YAML_SEQUENCE_START_EVENT);
			break; 
		case YAML_SEQUENCE_END_EVENT:
			LOG_DEBUG(YAML_SEQUENCE_END_EVENT);
			break; 
		case YAML_SCALAR_EVENT: 
			LOG_DEBUG(YAML_SCALAR_EVENT);
			break; 
		case YAML_MAPPING_START_EVENT: 
			LOG_DEBUG(YAML_MAPPING_START_EVENT);
			break; 
		case YAML_MAPPING_END_EVENT: 
			LOG_DEBUG(YAML_MAPPING_END_EVENT);
			break; 
		case YAML_STREAM_START_EVENT: 
			LOG_DEBUG(YAML_STREAM_START_EVENT);
			break; 
		case YAML_STREAM_END_EVENT: 
			LOG_DEBUG(YAML_STREAM_END_EVENT);
			return DONE;
			break; 
		default: 
			LOG_DEBUG(INTERPRETER_ERROR);
			return ERROR; 
			break; 
	}

	switch(interpreter.mode) { 
		case INIT: 
			// Expecting MAPPING_START
			interpret_init_mode(event);
			break; 
		case REGISTER_LIST: 
			// Expecting SCALAR (name of register)
			interpret_register_list_mode(event); 
			break; 
		case REGISTER_CONFIG: 
			// Expecting SCALAR or MAPPING
//			interpret_register_config_mode(event); 
			break; 
	}

	return CONTINUE; 
}
