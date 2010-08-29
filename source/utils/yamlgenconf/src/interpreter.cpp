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

#include "interpreter.hpp"
#include "interpreter_exception.hpp"
#include "logging.hpp"
#include <yaml.h>


void 
Interpreter::init_mode(yaml_event_t * event) { 
	if(event->type != YAML_MAPPING_START_EVENT) {
		throw InterpreterException(event, "Init");
	}

	LOG_DEBUG_PRINT("%i  BEGIN register group list", m_mode);
	m_mode = REGISTER_GROUP_LIST; 
}

void 
Interpreter::register_group_list_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT) {
		throw InterpreterException(event, "Register group list");
	}

	LOG_INFO_PRINT("Register group '%s'", event->data.scalar.value);
	m_cur_register_group = RegisterGroupConfig(event->data.scalar.value);
	m_mode = REGISTER_GROUP_CONFIG; 
}

void 
Interpreter::register_group_config_mode(yaml_event_t * event) { 
	if(event->type != YAML_MAPPING_START_EVENT) {
		throw InterpreterException(event, "Register group config");
	}

	LOG_DEBUG_PRINT("%i  BEGIN register group config", m_mode);
	m_mode = REGISTER_GROUP_SETTING;
}

void 
Interpreter::register_group_setting_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT && 
	   event->type != YAML_MAPPING_END_EVENT) {
		throw InterpreterException(event, "Register group setting");
	}

	if(event->type == YAML_SCALAR_EVENT) { 
		m_cur_register_group_setting_name = ::std::string((const char *)(event->data.scalar.value));

		LOG_DEBUG_PRINT("%i  Register group setting name '%s'", m_mode, 
							event->data.scalar.value);
		m_mode = REGISTER_GROUP_SETTING_VALUE;
	}
	else if(event->type == YAML_MAPPING_END_EVENT) { 
		LOG_INFO_PRINT("%i  END register group config", m_mode);
		m_register_groups.push_back(m_cur_register_group); 
		m_mode = REGISTER_GROUP_LIST;
	}
}

void 
Interpreter::register_group_setting_value_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT &&
		 event->type != YAML_MAPPING_START_EVENT) {
		throw InterpreterException(event, "Register group setting");
	}

	if(event->type == YAML_SCALAR_EVENT) { 
		LOG_DEBUG_PRINT("%i    Register group setting value '%s' -> '%s'", m_mode, 
							m_cur_register_group_setting_name.c_str(), 
							event->data.scalar.value);
		if(strcmp(m_cur_register_group_setting_name.c_str(), "description") == 0) { 
			m_cur_register_group.set_description(event->data.scalar.value); 
			LOG_INFO_PRINT("     Register group description is '%s'" , event->data.scalar.value);
			m_mode = REGISTER_GROUP_SETTING; // Register group description saved, 
																			 // look for further settings
		}
	}
	else if(event->type == YAML_MAPPING_START_EVENT) { 
		if(strcmp(m_cur_register_group_setting_name.c_str(), "registers") == 0) { 
			m_mode = REGISTER_LIST; // We expect a mapping of register settings now
		}
		else { 
			LOG_DEBUG_PRINT("%i    ERROR: Register group setting mapping, but not registers", m_mode);
		}
	}
}

void Interpreter::register_list_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT && 
		 event->type != YAML_MAPPING_END_EVENT) {
		throw InterpreterException(event, "Register list");
	}

	if(event->type == YAML_SCALAR_EVENT) { 
		LOG_DEBUG_PRINT("%i  Register name '%s'", m_mode, 
							event->data.scalar.value);
		m_cur_register = RegisterConfig(event->data.scalar.value); 
		m_mode = REGISTER_CONFIG;
	}
	else if(event->type == YAML_MAPPING_END_EVENT) { 
		LOG_DEBUG_PRINT("%i  Leaving register list mode", m_mode);
		m_mode = REGISTER_GROUP_LIST; 
	}
}

void Interpreter::register_config_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT &&
		 event->type != YAML_MAPPING_START_EVENT) {
		throw InterpreterException(event, "Register config");
	}
	m_mode = REGISTER_SETTING; 
}

void Interpreter::register_setting_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT &&
		 event->type != YAML_MAPPING_START_EVENT &&
		 event->type != YAML_MAPPING_END_EVENT) {
		throw InterpreterException(event, "Register setting");
	}

	if(event->type == YAML_SCALAR_EVENT) { 
		if(strcmp((const char *)(event->data.scalar.value), "widget") == 0) { 
			LOG_DEBUG_PRINT("%i   Switch to widget setting mode", m_mode);
			m_cur_widget = WidgetConfig(); 
			m_mode = WIDGET_SETTING; 
		} 
		else { 
			m_cur_register_setting_name = ::std::string((const char *)(event->data.scalar.value)); 
			m_mode = REGISTER_SETTING_VALUE;
		}
	}
	else if(event->type == YAML_MAPPING_END_EVENT) { 
		LOG_DEBUG_PRINT("%i    Leaving register setting mode", m_mode); 
		m_mode = REGISTER_LIST;
		m_cur_register.log(); 
		m_cur_widget.log(); 

		m_cur_register.set_widget(m_cur_widget);
		m_cur_register_group.add_register(m_cur_register); 
	}
}

void Interpreter::register_setting_value_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT &&
		 event->type != YAML_MAPPING_START_EVENT) { 
		throw InterpreterException(event, "Register setting value");
	}

	if(event->type == YAML_SCALAR_EVENT) { 
		LOG_DEBUG_PRINT("		Register setting value '%s' -> '%s'", 
							m_cur_register_setting_name.c_str(), 
							event->data.scalar.value);
		m_cur_register.set_property(m_cur_register_setting_name, 
																::std::string((const char *)(event->data.scalar.value)));
		m_mode = REGISTER_SETTING; 
	}
}

void
Interpreter::widget_setting_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT &&
	   event->type != YAML_MAPPING_END_EVENT) { 
		throw InterpreterException(event, "Widget setting");
	}

	if(event->type == YAML_SCALAR_EVENT) { 
		m_cur_widget_setting_name = ::std::string((const char *)(event->data.scalar.value)); 
		m_mode = WIDGET_SETTING_VALUE;
	}
	else if(event->type == YAML_MAPPING_END_EVENT) { 
		LOG_DEBUG_PRINT("%i    Leaving widget mode", m_mode); 
		m_mode = REGISTER_SETTING;
	}
}	

void
Interpreter::widget_setting_value_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT) {
		throw InterpreterException(event, "Widget setting value");
	}

	m_cur_widget.set_property(m_cur_widget_setting_name, 
														::std::string((const char *)(event->data.scalar.value)));
	m_mode = WIDGET_SETTING; 
}	

Interpreter::interpreter_mode_t 
Interpreter::next_event(yaml_event_t * event) throw (InterpreterException)
{

#ifdef LOG
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
			LOG_DEBUG_PRINT("     Scalar: '%s' mode: %i", event->data.scalar.value, m_mode);
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
#endif

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
		case WIDGET_SETTING: 
			widget_setting_mode(event); 
			break; 
		case WIDGET_SETTING_VALUE: 
			widget_setting_value_mode(event); 
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
