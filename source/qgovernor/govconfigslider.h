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

#ifndef GOVCONFIGSLIDER_H
#define GOVCONFIGSLIDER_H

#include <yamlgen/config.hpp>

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>


class GovConfigSlider : public GovConfigWidget
{

public:

    GovConfigSlider(QWidget * parent, YAMLGen::OBLDC::RegisterConfig const & config)
    : GovConfigWidget(parent, config)
    {

        QLabel * label = new QLabel(QString::fromStdString(config.label()),
                                    m_widget);

        QWidget * input_container_widget = new QWidget(m_widget);
        YAMLGen::OBLDC::WidgetConfig widget_conf = config.widget();

        QVBoxLayout * vlayout = new QVBoxLayout();
        vlayout->addWidget(label);
        QHBoxLayout * hlayout = new QHBoxLayout();

        QSpinBox * spin_box;
        spin_box = new QSpinBox(input_container_widget);
        spin_box->setValue(widget_conf.default_value());

        QPushButton * commit_button;
        commit_button = new QPushButton("commit", input_container_widget);

        QSlider * input;
        input = new QSlider(Qt::Horizontal, input_container_widget);
        input->setSingleStep(widget_conf.step());
        input->setMinimum(widget_conf.min());
        input->setMaximum(widget_conf.max() || 100);
        input->setValue(widget_conf.default_value());
        vlayout->addWidget(input);

        hlayout->addWidget(spin_box);
        hlayout->addWidget(commit_button);

        input_container_widget->setLayout(hlayout);
        vlayout->addWidget(input_container_widget);
        m_widget->setLayout(vlayout);
    }

};

#endif // GOVCONFIGSLIDER_H
