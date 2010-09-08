#ifndef YAML_CONFIG_HPP__
#define YAML_CONFIG_HPP__

#include <yaml.h>
#include "logging.hpp"
#include "error_handling.hpp"
#include "yaml_interpreter.hpp"
#include "yaml_interpreter_exception.hpp"
#include "parser_exception.hpp"


class YAMLConfig
{
private: 

	YAMLInterpreter m_yaml_interpreter; 

public: 

	YAMLConfig() { 
	}

private: 

	void on_parse_error(yaml_parser_t * parser); 

public: 

	void read(char const * filename) throw (ParserException, YAMLInterpreterException);

	inline void log(void) const { 
		m_yaml_interpreter.log(); 
	}

};

#endif
