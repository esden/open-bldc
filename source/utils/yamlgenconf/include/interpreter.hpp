#ifndef INTERPRETER_H__
#define INTERPRETER_H__

#include <yaml.h>

enum interpreter_mode { 
	INIT=0, 
	REGISTER_LIST, 
	REGISTER_CONFIG, 
	FLAG_LIST, 
	FLAG_CONFIG, 
	WIDGET_CONFIG, 
	ERROR, 
	CONTINUE, 
	DONE
};

typedef struct interpreter_state { 
	enum interpreter_mode mode; 
} interpreter_t;

enum interpreter_mode interpret_next_event(yaml_event_t * event); 

#endif
