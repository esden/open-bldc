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

#ifndef PROTOCOLMODEL_H
#define PROTOCOLMODEL_H

#include <QStandardItemModel>

class ProtocolModel : public QStandardItemModel
{
public:
    ProtocolModel();
    void addPacket(bool monitor, QChar r_w, unsigned char addr, unsigned short value);
    void addPacket(bool monitor, QChar r_w, unsigned char addr);
    void handleByte(unsigned char byte);
};

#endif // PROTOCOLMODEL_H
