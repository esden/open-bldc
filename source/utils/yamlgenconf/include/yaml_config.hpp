#ifndef YAML_CONFIG_HPP__
#define YAML_CONFIG_HPP__

#include <yaml.h>
#include "logging.hpp"
#include "error_handling.hpp"
#include "interpreter.hpp"
#include "exception/interpreter_exception.hpp"
#include "exception/parser_exception.hpp"


class YAMLConfig
{
private: 

	Interpreter m_interpreter; 

public: 

	YAMLConfig() { 
	}

public: 

	void read(char const * filename) throw (ParserException, InterpreterException);

	inline void log(void) const { 
		m_interpreter.log(); 
	}

};

#endif
