#ifndef GOVCONFIG_H
#define GOVCONFIG_H

#include <olconf/module_config.hpp>
#include <vector>
#include <QString>

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
        load();
    }

    QString const & target_name(void) const {
        return m_target_name;
    }

    ::std::vector<YAMLGen::OBLDC::ModuleConfig> const & modules(void) const {
        return m_modules;
    }

    virtual void load(void);

};

#endif // GOVCONFIG_H
