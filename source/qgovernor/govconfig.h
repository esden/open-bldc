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
