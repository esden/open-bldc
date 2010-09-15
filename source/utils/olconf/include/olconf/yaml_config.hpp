#ifndef YAML_CONFIG_HPP__
#define YAML_CONFIG_HPP__

#include <yamlgen/logging.hpp>
#include <yamlgen/interpreter.hpp>
#include <yamlgen/exception/interpreter_exception.hpp>
#include <yamlgen/exception/parser_exception.hpp>
#include <yamlgen/exception/builder_exception.hpp>
#include <yamlgen/exception/config_exception.hpp>

#include <yaml.h>


namespace YAMLGen { 
namespace OBLDC { 

class YAMLConfig
{
private: 

	Interpreter m_interpreter; 

public: 

	YAMLConfig() { 
	}

public: 

	void read(char const * filename);

	inline void log(void) const { 
		m_interpreter.log(); 
	}

};

} /* namespace OBLDC */
} /* namespace YAMLGen */

#endif
