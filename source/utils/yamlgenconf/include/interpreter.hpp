#ifndef INTERPRETER_H__
#define INTERPRETER_H__

#include <yaml.h>
#include <vector>

#include "flag_config.hpp"
#include "register_group_config.hpp"
#include "widget_config.hpp"
#include "interpreter_exception.hpp"

#define register_handler(mode, fun) \
	m_mode_handlers[mode] = &Interpreter::fun

class Interpreter
{

public: 

	typedef enum { 
		INIT=0, 
		REGISTER_LIST, 
		REGISTER_CONFIG, 
		REGISTER_SETTING, 
		REGISTER_SETTING_VALUE, 
		REGISTER_GROUP_LIST, 
		REGISTER_GROUP_CONFIG, 
		REGISTER_GROUP_SETTING, 
		REGISTER_GROUP_SETTING_VALUE, 
		FLAG_LIST, 
		FLAG_CONFIG, 
		WIDGET_CONFIG, 
		WIDGET_SETTING, 
		WIDGET_SETTING_VALUE, 
		COMPLETING, 
		ERROR, 
		CONTINUE, 
		DONE
	} interpreter_mode_t;

	typedef void (Interpreter::*mode_handler_fun)(yaml_event_t * event); 
	mode_handler_fun m_mode_handlers[DONE+1];

private:
	
	interpreter_mode_t m_mode; 
	::std::vector<RegisterGroupConfig> m_register_groups; 
	::std::vector<FlagConfig> m_flags; 

	RegisterGroupConfig m_cur_register_group; 
	RegisterConfig m_cur_register; 
	WidgetConfig m_cur_widget; 
	
	::std::string m_cur_register_group_setting_name; 
	::std::string m_cur_register_name; 
	::std::string m_cur_register_setting_name; 
	::std::string m_cur_widget_setting_name; 

public: 

	Interpreter()
	: m_mode(INIT)
	{ 
		register_handler(INIT, 												 init_mode); 
		register_handler(REGISTER_LIST,                register_list_mode); 
		register_handler(REGISTER_CONFIG,              register_config_mode); 
		register_handler(REGISTER_SETTING,             register_setting_mode); 
		register_handler(REGISTER_SETTING_VALUE,       register_setting_value_mode); 
		register_handler(REGISTER_GROUP_LIST,          register_group_list_mode); 
		register_handler(REGISTER_GROUP_CONFIG,        register_group_config_mode); 
		register_handler(REGISTER_GROUP_SETTING,       register_group_setting_mode); 
		register_handler(REGISTER_GROUP_SETTING_VALUE, register_group_setting_value_mode); 
		register_handler(WIDGET_CONFIG,                widget_config_mode); 
		register_handler(WIDGET_SETTING,               widget_setting_mode); 
		register_handler(WIDGET_SETTING_VALUE,         widget_setting_value_mode); 
		register_handler(COMPLETING,                   completing_mode); 
	}

	~Interpreter() { 
	}

public:

	inline void add_register_group(RegisterGroupConfig & config) { 
		m_register_groups.push_back(config);
	}
	inline ::std::vector<RegisterGroupConfig> & register_groups(void) { 
		return m_register_groups; 
	}

public: 

	interpreter_mode_t next_event(yaml_event_t * event) throw(InterpreterException);

private: 

	void init_mode(yaml_event_t * event); 

	void register_group_list_mode(yaml_event_t * event); 
	void register_group_config_mode(yaml_event_t * event); 
	void register_group_setting_mode(yaml_event_t * event); 
	void register_group_setting_value_mode(yaml_event_t * event); 

	void register_list_mode(yaml_event_t * event); 
	void register_config_mode(yaml_event_t * event); 
	void register_setting_mode(yaml_event_t * event); 
	void register_setting_value_mode(yaml_event_t * event); 
	
	void widget_config_mode(yaml_event_t * event); 
	void widget_setting_mode(yaml_event_t * event); 
	void widget_setting_value_mode(yaml_event_t * event); 

	void completing_mode(yaml_event_t * event); 

};

#endif
