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

#ifndef GOVCONFIGWIDGET_H
#define GOVCONFIGWIDGET_H

#include <QWidget>
#include <olconf/register_config.hpp>
#include <yamlgen/config.hpp>


class GovConfigWidget
{
protected:

    QWidget * m_widget;
    QWidget * m_parent;
    YAMLGen::Config m_config;

public:

    GovConfigWidget(QWidget * parent, YAMLGen::OBLDC::RegisterConfig config)
    : m_parent(parent), m_config(config)
    {
       m_widget = new QWidget(parent);
    }

    virtual ~GovConfigWidget() {
        if(m_widget) { delete m_widget; }
    }

public:

    // TODO: Check if the getters can be constified like
    //   const QWidget * const widget(void) const

    QWidget * widget(void) {
       return m_widget;
    }
    YAMLGen::Config const & config(void) const {
       return m_config;
    }

signals:

    void valueChanged(int new_value);
};

#endif // GOVCONFIGWIDGET_H
