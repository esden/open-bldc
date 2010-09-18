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

#include "targetwidgetfactory.h"

#include "govconfigwidget.h"
#include "govconfigspinbox.h"
#include "govconfigslider.h"
#include "govconfigcheckbox.h"

#include <olconf/module_config.hpp>
#include <olconf/register_group_config.hpp>
#include <olconf/register_config.hpp>
#include <olconf/flag_config.hpp>

#include <QLabel>
#include <QString>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QGroupBox>
#include <QPushButton>
#include <QSlider>

#include <vector>


QWidget *
TargetWidgetFactory::createFrom(GovConfig const & config)
{
    QVBoxLayout * layout = new QVBoxLayout();
    QWidget * container_widget = new QWidget(m_parent);

    ::std::vector<YAMLGen::OBLDC::ModuleConfig>::const_iterator mod_it;
    ::std::vector<YAMLGen::OBLDC::ModuleConfig>::const_iterator mod_end;
    mod_end = config.modules().end();
    for(mod_it = config.modules().begin(); mod_it != mod_end; ++mod_it) {
        QWidget * widget = createWidgetGroups(container_widget, *mod_it);
        layout->addWidget(widget);
    }

    container_widget->setLayout(layout);
    return container_widget;
}

QWidget *
TargetWidgetFactory::createWidgetGroups(QWidget * parent, YAMLGen::OBLDC::ModuleConfig const & config)
{
    ::std::vector<YAMLGen::OBLDC::RegisterGroupConfig>::const_iterator rg_it;
    ::std::vector<YAMLGen::OBLDC::RegisterGroupConfig>::const_iterator rg_end;
    rg_end = config.registers().end();
    QVBoxLayout * layout       = new QVBoxLayout();
    QWidget * container_widget = new QWidget(parent);

    for(rg_it = config.registers().begin(); rg_it != rg_end; ++rg_it) {
        QWidget * widget = createWidgetGroup(container_widget, *rg_it);
        layout->addWidget(widget);
    }
    container_widget->setLayout(layout);
    return container_widget;
}

QWidget *
TargetWidgetFactory::createWidgetGroup(QWidget * parent, YAMLGen::OBLDC::RegisterGroupConfig const & config)
{
    QGroupBox * group_box = new QGroupBox(parent);
    QVBoxLayout * layout  = new QVBoxLayout();

    group_box->setTitle(QString::fromStdString(config.description()));

    ::std::vector<YAMLGen::OBLDC::RegisterConfig>::const_iterator reg_it;
    ::std::vector<YAMLGen::OBLDC::RegisterConfig>::const_iterator reg_end;
    reg_end = config.registers().end();
    for(reg_it = config.registers().begin(); reg_it != reg_end; ++reg_it) {
        GovConfigWidget * govwidget = createWidget(group_box, *reg_it);
        layout->addWidget(govwidget->widget());
    }
    group_box->setLayout(layout);
    return group_box;
}

GovConfigWidget *
TargetWidgetFactory::createWidget(QWidget * parent, YAMLGen::OBLDC::RegisterConfig const & config)
{
    QWidget * container_widget = new QWidget(parent);

    YAMLGen::OBLDC::WidgetConfig widget_conf = config.widget();
    ::std::string widget_class = widget_conf.classname();

    GovConfigWidget * gov_widget;

    if(widget_class == "SpinBox") {
        gov_widget = new GovConfigSpinBox(parent, config);
    }
    else if(widget_class == "Checkbox") {
        gov_widget = new GovConfigCheckBox(parent, config);
    }
    else if(widget_class == "Slider") {
        gov_widget = new GovConfigSlider(parent, config);
    }

    return gov_widget;
}

GovConfigWidget *
TargetWidgetFactory::createWidget(QWidget * parent, YAMLGen::OBLDC::FlagConfig const & config) {
    return 0;
}
