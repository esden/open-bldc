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

#ifndef GOVCONFIGSPINBOX_H
#define GOVCONFIGSPINBOX_H

#include <yamlgen/config.hpp>

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>


class GovConfigSpinBox : public GovConfigWidget
{

public:

    GovConfigSpinBox(QWidget * parent, YAMLGen::OBLDC::RegisterConfig const & config)
    : GovConfigWidget(parent, config)
    {
        QLabel * label = new QLabel(QString::fromStdString(config.label()),
                                    m_widget);
        YAMLGen::OBLDC::WidgetConfig widget_conf = config.widget();

        QHBoxLayout * layout = new QHBoxLayout();
        layout->addWidget(label);

        QSpinBox * input;
        input = new QSpinBox(m_widget);
        input->setValue(widget_conf.default_value());

        QPushButton * commit_button;
        commit_button = new QPushButton("commit", m_widget);

        layout->addWidget(input);
        layout->addWidget(commit_button);

        m_widget->setLayout(layout);

        // tell when to
        // emit value_changed(int new_value)
    }

};

#endif // GOVCONFIGSPINBOX_H
