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

#ifndef GOVERNORCLIENT_H
#define GOVERNORCLIENT_H

#include <QObject>

class GovernorClient : public QObject
{
    Q_OBJECT
public:
    GovernorClient();
    signed short pickupByte();
    unsigned short getRegisterMapValue(unsigned char addr);
    int handleByte(unsigned char byte);
    void setRegister(unsigned char addr, unsigned short value);
    int registerTouched(unsigned char addr);
    void sendString(QString &string);
    void outputTriggerCB();
    void registerChangedCB(unsigned char addr);

private:
    unsigned short register_map[32];

signals:
    void outputTriggered();
    void registerChanged(unsigned char addr);
};

#endif // GOVERNORCLIENT_H
