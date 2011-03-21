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
extern "C" {
#include "lg/types.h"
#include "lg/gpdef.h"
#include "lg/gprotc.h"
}

#include "governorclient.h"

extern "C" {
void gpc_output_trigger(void *data);
void gpc_register_changed(void *data, u8 addr);
void gpc_get_version(void *data);
}

GovernorClient::GovernorClient()
{
    int i;
    gpc_init(gpc_output_trigger, static_cast<void *>(this), gpc_register_changed, static_cast<void *>(this));
    gpc_set_get_version_callback(gpc_get_version, static_cast<void *>(this));
    for(i=0; i<32; i++){
        register_map[i] = 0;
        gpc_setup_reg(i, &register_map[i]);
    }
}

signed short GovernorClient::pickupByte()
{
    return gpc_pickup_byte();
}

unsigned short GovernorClient::getRegisterMapValue(unsigned char addr)
{
    return register_map[addr];
}

int GovernorClient::handleByte(unsigned char byte)
{
    return gpc_handle_byte(byte);
}

void GovernorClient::setRegister(unsigned char addr, unsigned short value)
{
    register_map[addr] = value;
}

int GovernorClient::registerTouched(unsigned char addr)
{
    return gpc_register_touched(addr);
}

void GovernorClient::sendString(QString &string)
{
    gpc_send_string(string.toAscii().data(), string.length());
}

void GovernorClient::outputTriggerCB()
{
    emit outputTriggered();
}

void GovernorClient::registerChangedCB(unsigned char addr)
{
    emit registerChanged(addr);
}

extern "C" {
void gpc_output_trigger(void *data)
{
    static_cast<GovernorClient *>(data)->outputTriggerCB();
}

void gpc_register_changed(void *data, u8 addr)
{
    static_cast<GovernorClient *>(data)->registerChangedCB(addr);
}

void gpc_get_version(void *data)
{
    char version[] = "qgovernor governor protocol simulator\n";

    gpc_send_string(version, strlen(version));

    data = data;
}
}
