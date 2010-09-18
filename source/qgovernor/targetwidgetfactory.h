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

#ifndef TARGETWIDGETFACTORY_H
#define TARGETWIDGETFACTORY_H

#include "govconfig.h"
#include "govconfigwidget.h"

#include <olconf/module_config.hpp>
#include <olconf/register_group_config.hpp>
#include <olconf/register_config.hpp>
#include <olconf/flag_config.hpp>

#include <QWidget>


class TargetWidgetFactory
{
private:

    QWidget * m_parent;

public:

    TargetWidgetFactory(QWidget * parent)
    : m_parent(parent)
    { }

public:

    /** Creates widget for a given target configuration.
     *  The widget returned can be added to the QTablist in the
     *  main window.
     */
    QWidget * createFrom(GovConfig const & config);

private:

    /** Creates widget group from module configuration.
     *  A module configuration may contain:
     *  - A list of register groups
     *  - Flag settings
     */
    QWidget * createWidgetGroups(QWidget * parent, YAMLGen::OBLDC::ModuleConfig const & config);

    /** Creates widget for a given register group.
     *
     */
    QWidget * createWidgetGroup(QWidget * parent, YAMLGen::OBLDC::RegisterGroupConfig const & config);

    /** Creates widget for a given register.
     *
     */
    GovConfigWidget * createWidget(QWidget * parent, YAMLGen::OBLDC::RegisterConfig const & config);

    /** Creates widget for flag settings.
     *
     */
    GovConfigWidget * createWidget(QWidget * parent, YAMLGen::OBLDC::FlagConfig const & config);
};

#endif // TARGETWIDGETFACTORY_H
