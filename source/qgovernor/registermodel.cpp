/*
 * qgovernor - QT based Open-BLDC PC interface tool
 * Copyright (C) 2010 by Piotr Esden-Tempski <piotr@esden.net>
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

#include "registermodel.h"

RegisterModel::RegisterModel()
{
    setHorizontalHeaderLabels(
            QStringList() << tr("Dec")
                          << tr("Hex")
                          << tr("Bin")
                          << tr("Mon")
                          << tr("Dis"));

    for(int i=0; i<32; i++){
        setVerticalHeaderItem(i, new QStandardItem(QString::number(i, 10).rightJustified(3, '0', false)));
        setItem(i, 0, new QStandardItem(QString::number(0, 10).rightJustified(5, '0', false)));
        setItem(i, 1, new QStandardItem(QString::number(0, 16).rightJustified(4, '0', false)));
        setItem(i, 2, new QStandardItem(QString::number(0 >> 8, 2).rightJustified(8, '0', false)
                                        .append(" ")
                                        .append(QString::number(0 & 0xFF, 2).rightJustified(8, '0', false))));
        setItem(i, 3, new QStandardItem());
        item(i, 3)->setCheckable(true);
        setItem(i, 4, new QStandardItem());
        item(i, 4)->setCheckable(true);
    }
}

void RegisterModel::setRegisterValue(uint8_t addr, uint16_t value)
{
    item(addr, 0)->setData(QString::number(value, 10).rightJustified(5, '0', false), Qt::DisplayRole);
    item(addr, 1)->setData(QString::number(value, 16).rightJustified(4, '0', false), Qt::DisplayRole);
    item(addr, 2)->setData(QString::number(value >> 8, 2).rightJustified(8, '0', false)
                           .append(" ")
                           .append(QString::number(value & 0xFF, 2)
                                   .rightJustified(8, '0', false)),
                           Qt::DisplayRole);
}
