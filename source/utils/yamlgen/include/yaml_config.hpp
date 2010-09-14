#ifndef YAML_CONFIG_HPP__
#define YAML_CONFIG_HPP__

#include <yaml.h>
#include "logging.hpp"
#include "error_handling.hpp"
#include "interpreter.hpp"
#include "exception/interpreter_exception.hpp"
#include "exception/parser_exception.hpp"
#include "exception/builder_exception.hpp"
#include "exception/config_exception.hpp"


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
