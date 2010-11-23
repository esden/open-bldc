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
#include <lg/types.h>
#include <lg/gpdef.h>
#include <lg/gprotm.h>
}

#include "governormaster.h"

extern "C" {
void gpm_output_trigger(void *data);
void gpm_log_callback(void *data);
void gpm_register_changed(void *data, u8 addr);
}

GovernorMaster::~GovernorMaster()
{
  delete reglog;
}

GovernorMaster::GovernorMaster()
{
    gpm_init(gpm_output_trigger, static_cast<void *>(this), gpm_register_changed, static_cast<void *>(this));
}

signed short GovernorMaster::pickupByte()
{
    return gpm_pickup_byte();
}

int GovernorMaster::sendSet(unsigned char addr, unsigned short data)
{
    return gpm_send_set(addr, data);
}

int GovernorMaster::sendGet(unsigned char addr)
{
    return gpm_send_get(addr);
}

int GovernorMaster::sendGetCont(unsigned char addr)
{
    return gpm_send_get_cont(addr);
}

unsigned short GovernorMaster::getRegisterMapValue(unsigned char addr)
{
    return gpm_get_register_map_val(addr);
}

int GovernorMaster::handleByte(unsigned char byte)
{
    return gpm_handle_byte(byte);
}

void GovernorMaster::outputTriggerCB()
{
    emit outputTriggered();
}

void GovernorMaster::registerChangedCB(unsigned char addr)
{
    emit registerChanged(addr);
}

void GovernorMaster::newLog(const QString &name)
{
  reglog = new QGLogger(name);
  gpm_set_log(&gpm_log_callback, static_cast<void *>(this));
}

// The libgovernor callbacks use a void pointer to keep the address of
// the GovernorMaster instance; these callbacks then re-cast it to
// call the methods on the right instance.  

extern "C" {
void gpm_output_trigger(void *data)
{
    static_cast<GovernorMaster *>(data)->outputTriggerCB();
}

void gpm_register_changed(void *data, u8 addr)
{
    static_cast<GovernorMaster *>(data)->registerChangedCB(addr);
}

void gpm_log_callback(void *data)
{
  static_cast<GovernorMaster *>(data)->reglog->
    writeRegisters(&gpm_get_register_map_val);
}

}
