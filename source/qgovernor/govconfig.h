#ifndef GOVCONFIG_H
#define GOVCONFIG_H

#include <olconf/module_config_builder.hpp>
#include <olconf/module_config.hpp>
#include <yamlgen/interpreter.hpp>
#include <yamlgen/postprocessor.hpp>

#include <vector>


class GovConfig
{
private:

    ::std::vector<YAMLGen::OBLDC::ModuleConfig> m_modules;
    QString m_target_name;
    QString m_filename;

public:

    GovConfig(QString const & filename)
        : m_filename(filename)
    {
        YAMLGen::Interpreter interpreter;
        interpreter.read(filename.toStdString().c_str());

        YAMLGen::Postprocessor postproc(interpreter.config());
        postproc.run();

        YAMLGen::OBLDC::ModuleConfigBuilder builder;
        builder.parse(postproc.config());

        m_modules     = builder.modules();
        m_target_name = builder.target_name();
    }

    QString const & target_name(void) const {
        return m_target_name;
    }

    ::std::vector<YAMLGen::OBLDC::ModuleConfig> const & modules(void) const {
        return m_modules;
    }

};

#endif // GOVCONFIG_H
