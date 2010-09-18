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

#ifndef GOVCONFIGCHECKBOX_H
#define GOVCONFIGCHECKBOX_H

#include <yamlgen/config.hpp>

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>


class GovConfigCheckBox : public GovConfigWidget
{

public:

    GovConfigCheckBox(QWidget * parent, YAMLGen::OBLDC::RegisterConfig const & config)
    : GovConfigWidget(parent, config)
    {
        QLabel * label = new QLabel(QString::fromStdString(config.label()),
                                    m_widget);
        YAMLGen::OBLDC::WidgetConfig widget_conf = config.widget();

        QHBoxLayout * layout = new QHBoxLayout();
        layout->addWidget(label);
        QCheckBox * input;
        input = new QCheckBox(m_widget);

        layout->addWidget(input);
        m_widget->setLayout(layout);
    }

};

#endif // GOVCONFIGCHECKBOX_H
