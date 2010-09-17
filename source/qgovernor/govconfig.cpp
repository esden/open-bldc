
#include "govconfig.h"
#include <olconf/module_config_builder.hpp>
#include <yamlgen/interpreter.hpp>
#include <yamlgen/postprocessor.hpp>

#include <QMessageBox>

void
GovConfig::load(void) {
    try {
        YAMLGen::Interpreter interpreter;
        interpreter.read(m_filename.toStdString().c_str());

        YAMLGen::Postprocessor postproc(interpreter.config());
        postproc.run();

        YAMLGen::OBLDC::ModuleConfigBuilder builder;
        builder.parse(postproc.config());

        m_modules     = builder.modules();
        m_target_name = QString::fromStdString(builder.target_name());

        ::std::cerr << "Target loaded: " << m_target_name.toStdString() << ::std::endl;
        ::std::vector<YAMLGen::OBLDC::ModuleConfig>::const_iterator mod_it;
        ::std::vector<YAMLGen::OBLDC::ModuleConfig>::const_iterator mod_end;
        mod_end = m_modules.end();
        for(mod_it  = m_modules.begin(); mod_it != mod_end; ++mod_it) {
            ::std::cerr << "  Module: " << ::std::endl;
            (*mod_it).log();
        }
    } catch(YAMLGen::ParserException pe) {
        QMessageBox::critical( 0, "GovConf",
          QString("Syntax error in configuration:\n\n") + pe.what()
        );
    } catch(YAMLGen::InterpreterException ie) {
        QMessageBox::critical( 0, "GovConf",
          QString("Error in target configuration syntax:\n\n") + ie.what()
        );
    } catch(YAMLGen::ConfigException ce) {
        QMessageBox::critical( 0, "GovConf",
          QString("Error in target configuration:\n\n") + ce.what()
        );
    }
}
