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


void Interpreter::init_mode(yaml_event_t * event) { 
	if(event->type != YAML_MAPPING_START_EVENT &&
		 event->type != YAML_STREAM_START_EVENT && 
		 event->type != YAML_DOCUMENT_START_EVENT) { 
		throw InterpreterException(event, "Init");
	}

	// Wait for first mapping: 
	if(event->type == YAML_MAPPING_START_EVENT) { 
		m_mode = REGISTER_GROUP_LIST; 
	}
}

void Interpreter::register_group_list_mode(yaml_event_t * event) { 
	if(event->type == YAML_SCALAR_EVENT) { 
		m_cur_register_group = RegisterGroupConfig(event->data.scalar.value);
		m_mode = REGISTER_GROUP_CONFIG; 
	}
	else if(event->type == YAML_MAPPING_END_EVENT) { 
		m_mode = COMPLETING; 
	}
	else { 
		throw InterpreterException(event, "Register group list");
	}
}

void Interpreter::register_group_config_mode(yaml_event_t * event) { 
	if(event->type != YAML_MAPPING_START_EVENT) {
		throw InterpreterException(event, "Register group config");
	}
	m_mode = REGISTER_GROUP_SETTING;
}

void Interpreter::register_group_setting_mode(yaml_event_t * event) { 
	if(event->type == YAML_SCALAR_EVENT) { 
		m_cur_register_group_setting_name = ::std::string((const char *)(event->data.scalar.value));

		m_mode = REGISTER_GROUP_SETTING_VALUE;
	}
	else if(event->type == YAML_MAPPING_END_EVENT) { 
		m_cur_register_group.log(); 
		m_register_groups.push_back(m_cur_register_group); 
		m_mode = REGISTER_GROUP_LIST;
	}
	else { 
		throw InterpreterException(event, "Register group setting");
	}
}

void Interpreter::register_group_setting_value_mode(yaml_event_t * event) { 
	if(event->type != YAML_SCALAR_EVENT &&
		 event->type != YAML_MAPPING_START_EVENT) {
		throw InterpreterException(event, "Register group setting");
	}

	if(event->type == YAML_SCALAR_EVENT) { 
		if(strcmp(m_cur_register_group_setting_name.c_str(), "description") == 0) { 
			m_cur_register_group.set_description(event->data.scalar.value); 
			m_mode = REGISTER_GROUP_SETTING; // Register group description saved, 
																			 // look for further settings
		}
	}
	else if(event->type == YAML_MAPPING_START_EVENT) { 
		if(strcmp(m_cur_register_group_setting_name.c_str(), "registers") == 0) { 
			m_mode = REGISTER_LIST; // We expect a mapping of register settings now
		}
		else { 
			throw InterpreterException(event, "Only attribute 'registers' may "
																				"be a mapping in register group config"); 
		}
	}
}

void Interpreter::register_list_mode(yaml_event_t * event) { 
	if(event->type == YAML_SCALAR_EVENT) { 
		m_cur_register = RegisterConfig(event->data.scalar.value); 
		m_mode = REGISTER_CONFIG;
	}
	else if(event->type == YAML_MAPPING_END_EVENT) { 
		m_mode = REGISTER_GROUP_SETTING; 
	}
	else { 
		throw InterpreterException(event, "Register list");
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
	if(event->type == YAML_SCALAR_EVENT) { 
		if(strcmp((const char *)(event->data.scalar.value), "widget") == 0) { 
			m_cur_widget = WidgetConfig(); 
			m_mode = WIDGET_CONFIG; 
		} 
		else { 
			m_cur_register_setting_name = ::std::string((const char *)(event->data.scalar.value)); 
			m_mode = REGISTER_SETTING_VALUE;
		}
	}
	else if(event->type == YAML_MAPPING_END_EVENT) { 
		m_mode = REGISTER_LIST;
		m_cur_register.set_widget(m_cur_widget);
		m_cur_register_group.add_register(m_cur_register); 
	}
	else { 
		throw InterpreterException(event, "Register setting");
	}
}

void Interpreter::register_setting_value_mode(yaml_event_t * event) { 
	if(event->type == YAML_MAPPING_START_EVENT) { 
		// continue, wait for first SCALAR
	}
	else if(event->type == YAML_SCALAR_EVENT) { 
		m_cur_register.set_property(m_cur_register_setting_name, 
																::std::string((const char *)(event->data.scalar.value)));
		m_mode = REGISTER_SETTING; 
	}
	else { 
		throw InterpreterException(event, "Register setting value");
	}
}

void Interpreter::widget_config_mode(yaml_event_t * event) { 
	if(event->type != YAML_MAPPING_START_EVENT) { 
		throw InterpreterException(event, "Widget config");
	}
	m_mode = WIDGET_SETTING; 
}

void
Interpreter::widget_setting_mode(yaml_event_t * event) { 
	if(event->type == YAML_SCALAR_EVENT) { 
		m_cur_widget_setting_name = ::std::string((const char *)(event->data.scalar.value)); 
		m_mode = WIDGET_SETTING_VALUE;
	}
	else if(event->type == YAML_MAPPING_END_EVENT) { 
		m_mode = REGISTER_SETTING;
	}
	else { 
		throw InterpreterException(event, "Widget setting");
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

void 
Interpreter::completing_mode(yaml_event_t * event) { 
	if(event->type != YAML_DOCUMENT_END_EVENT) {
		throw InterpreterException(event, "At end of document");
	}
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
