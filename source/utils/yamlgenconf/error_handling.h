#ifndef ERROR_HANDLING_H__
#define ERROR_HANDLING_H__

#include <yaml.h>

void on_parser_error(yaml_parser_t * parser); 
void on_emitter_error(yaml_emitter_t * emitter); 

#endif
