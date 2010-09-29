/*
 * qgovernor - QT based Open-BLDC PC interface tool
 * Copyright (C) 2010 by Tobias Fuchs <twh.fuchs@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
