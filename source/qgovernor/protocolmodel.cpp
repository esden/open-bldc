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

#include "lg/types.h"
#include "lg/gpdef.h"

#include "protocolmodel.h"

ProtocolModel::ProtocolModel()
{
    setHorizontalHeaderLabels(
            QStringList() << tr("Mon")
                          << tr("R/W")
                          << tr("Addr")
                          << tr("Val Hex")
                          << tr("Val Dec")
                          << tr("Val Bin"));

    history_size = 100;
}

void ProtocolModel::addPacket(bool monitor, QChar r_w, unsigned char addr, unsigned short value)
{
    appendRow(
            QList<QStandardItem *>() << new QStandardItem(monitor ? "X" : "")
            << new QStandardItem(r_w)
            << new QStandardItem(QString::number(addr, 10).rightJustified(3, '0', false))
            << new QStandardItem(QString::number(value, 16).rightJustified(4, '0', false))
            << new QStandardItem(QString::number(value, 10).rightJustified(5, '0', false))
            << new QStandardItem(QString::number(value >> 8, 2).rightJustified(8, '0', false)
                                 .append(" ")
                                 .append(QString::number(value & 0xFF, 2).rightJustified(8, '0', false))));

    if(rowCount() > history_size){
        removeRows(0, rowCount() - history_size);
    }
}

void ProtocolModel::addPacket(bool monitor, QChar r_w, unsigned char addr)
{
    appendRow(
            QList<QStandardItem *>() << new QStandardItem(monitor ? "X" : "")
            << new QStandardItem(r_w)
            << new QStandardItem(QString::number(addr, 10).rightJustified(3, '0', false)));

    if(rowCount() > history_size){
        removeRows(0, rowCount() - history_size);
    }
}

void ProtocolModel::handleByte(unsigned char byte)
{
    static int state = 0;
    static unsigned char addr;
    static unsigned short value;

    switch(state){
    case 0:
        if((byte & GP_MODE_MASK) == GP_MODE_WRITE){
            addr = byte & GP_ADDR_MASK;
            state = 1;
        }else if((byte & GP_MODE_MASK) == (GP_MODE_READ | GP_MODE_PEEK)){
            addPacket(false, 'R', byte & GP_ADDR_MASK);
        }else if((byte & GP_MODE_MASK) == (GP_MODE_READ | GP_MODE_CONT)){
            addPacket(true, 'R', byte & GP_ADDR_MASK);
        }
        break;
    case 1:
        value = byte;
        state = 2;
        break;
    case 2:
        value |= byte << 8;
        addPacket(false, 'W', addr, value);
        state = 0;
        break;
    }
}

void ProtocolModel::setHistorySize(qint64 size)
{
    history_size = size;

    if(rowCount() > history_size){
        removeRows(0, rowCount() - history_size);
    }
}
