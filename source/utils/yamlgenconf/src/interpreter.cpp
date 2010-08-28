
#include "interpreter.hpp"
#include "logging.hpp"
#include <yaml.h>
#include <stdarg.h>

// REGISTER_LIST -> REGISTER_CONFIG -> REGISTER_SETTING -> REGISTER_SETTING_VALUE

/* 
  INIT -> REGISTER_LIST on event YAML_MAPPING_START_EVENT
*/
void 
Interpreter::init_mode(yaml_event_t * event) { 
	if(event->type != YAML_MAPPING_START_EVENT) return;

	LOG_DEBUG_PRINT("%i  BEGIN register group list", m_mode);
	m_mode = REGISTER_GROUP_LIST; 
}

void 
Interpreter::register_group_list_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT) return;

	LOG_INFO_PRINT("Register group '%s'", event->data.scalar.value);
	m_cur_register_group = RegisterGroupConfig(event->data.scalar.value);
	m_mode = REGISTER_GROUP_CONFIG; 
}

void 
Interpreter::register_group_config_mode(yaml_event_t * event) { 
	if(event->type != YAML_MAPPING_START_EVENT) return;

	LOG_DEBUG_PRINT("%i  BEGIN register group config", m_mode);
	m_mode = REGISTER_GROUP_SETTING;
}

void 
Interpreter::register_group_setting_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT) return;

	m_cur_register_group_setting_name = ::std::string((const char *)(event->data.scalar.value));

	LOG_DEBUG_PRINT("%i  Register group setting name '%s'", m_mode, 
						event->data.scalar.value);
	m_mode = REGISTER_GROUP_SETTING_VALUE;
}

void 
Interpreter::register_group_setting_value_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT &&
		 event->type != YAML_MAPPING_START_EVENT) return;
	if(event->type == YAML_SCALAR_EVENT) { 
		LOG_DEBUG_PRINT("%i    Register group setting value '%s' -> '%s'", m_mode, 
							m_cur_register_group_setting_name.c_str(), 
							event->data.scalar.value);
		if(strcmp(m_cur_register_group_setting_name.c_str(), "description") == 0) { 
			m_cur_register_group.set_description(event->data.scalar.value); 
			LOG_INFO_PRINT("     Register group description is '%s'" , event->data.scalar.value);
			m_mode = REGISTER_GROUP_SETTING; // Register group description saved, 
																			 // look for further settings
			return; 
		}
		if(strcmp(m_cur_register_group_setting_name.c_str(), "registers") == 0) { 
			LOG_INFO_PRINT("     Reading register '%s'", event->data.scalar.value);
			m_cur_register = RegisterConfig(event->data.scalar.value); 
			m_mode = REGISTER_SETTING; // We expect a mapping of register settings now
			return; 
		}
	}
	if(event->type == YAML_MAPPING_START_EVENT) { 
		LOG_DEBUG_PRINT("%i    Register group setting mapping value", m_mode);
	}
}

void Interpreter::register_list_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT) return; 

	if(event->type == YAML_SCALAR_EVENT) { 
		LOG_DEBUG_PRINT("%i  Register name '%s'", m_mode, 
							event->data.scalar.value);
	}
}

void Interpreter::register_config_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT &&
		 event->type != YAML_MAPPING_START_EVENT) return;
}

void Interpreter::register_setting_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT &&
		 event->type != YAML_MAPPING_START_EVENT) return;

	if(event->type == YAML_SCALAR_EVENT) { 
		m_cur_register_setting_name = ::std::string((const char *)(event->data.scalar.value)); 
		m_mode = REGISTER_SETTING_VALUE;
	}
}

void Interpreter::register_setting_value_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT &&
		 event->type != YAML_MAPPING_START_EVENT) return;

	if(event->type == YAML_SCALAR_EVENT) { 
		LOG_INFO_PRINT("		Register setting value '%s' -> '%s'", 
							m_cur_register_setting_name.c_str(), 
							event->data.scalar.value);
		m_cur_register.set_property(::std::string(m_cur_register_setting_name.c_str()), 
																::std::string((const char *)(event->data.scalar.value)));
		m_mode = REGISTER_SETTING; 
	}
}

void
Interpreter::widget_config_mode(yaml_event_t * event) { 
	
}	

Interpreter::interpreter_mode_t 
Interpreter::next_event(yaml_event_t * event) {

	switch(event->type) { 
		case YAML_NO_EVENT: 
			break; 
		case YAML_ALIAS_EVENT:
			LOG_DEBUG_EVT(YAML_ALIAS_EVENT);
			break; 
		case YAML_DOCUMENT_START_EVENT:
			LOG_DEBUG_EVT(YAML_DOCUMENT_START_EVENT);
			break; 
		case YAML_DOCUMENT_END_EVENT:
			LOG_DEBUG_EVT(YAML_DOCUMENT_END_EVENT);
			break; 
		case YAML_SEQUENCE_START_EVENT: 
			LOG_DEBUG_EVT(YAML_SEQUENCE_START_EVENT);
			break; 
		case YAML_SEQUENCE_END_EVENT:
			LOG_DEBUG_EVT(YAML_SEQUENCE_END_EVENT);
			break; 
		case YAML_SCALAR_EVENT: 
			LOG_DEBUG_EVT(YAML_SCALAR_EVENT);
			break; 
		case YAML_MAPPING_START_EVENT: 
			LOG_DEBUG_EVT(YAML_MAPPING_START_EVENT);
			break; 
		case YAML_MAPPING_END_EVENT: 
			LOG_DEBUG_EVT(YAML_MAPPING_END_EVENT);
			break; 
		case YAML_STREAM_START_EVENT: 
			LOG_DEBUG_EVT(YAML_STREAM_START_EVENT);
			break; 
		case YAML_STREAM_END_EVENT: 
			LOG_DEBUG_EVT(YAML_STREAM_END_EVENT);
			return DONE;
			break; 
		default: 
			LOG_DEBUG_EVT(INTERPRETER_ERROR);
			return ERROR; 
			break; 
	}

	switch(m_mode) { 
		case INIT: 
			// Expecting MAPPING_START
			init_mode(event);
			break; 
		case REGISTER_GROUP_LIST: 
			// Expecting SCALAR (name of register)
			register_group_list_mode(event); 
			break; 
		case REGISTER_GROUP_CONFIG: 
			// Expecting SCALAR or MAPPING
			register_group_config_mode(event); 
			break; 
		case REGISTER_GROUP_SETTING: 
			register_group_setting_mode(event); 
			break; 
		case REGISTER_GROUP_SETTING_VALUE: 
			register_group_setting_value_mode(event); 
			break; 
		case REGISTER_LIST: 
			// Expecting SCALAR (name of register)
			register_list_mode(event); 
			break; 
		case REGISTER_CONFIG: 
			// Expecting SCALAR or MAPPING
			register_config_mode(event); 
			break; 
		case REGISTER_SETTING: 
			register_setting_mode(event); 
			break; 
		case REGISTER_SETTING_VALUE: 
			register_setting_value_mode(event); 
			break; 
		case FLAG_LIST: 
			break; 
		case FLAG_CONFIG: 
			break; 
		case WIDGET_CONFIG: 
			break; 
		case ERROR: 
			break; 
		case CONTINUE: 
			break; 
		case DONE: 
			break;
	}

	return CONTINUE; 
}
