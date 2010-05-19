/*
 * libgovernor - Open-BLDC configuration and debug protocol library
 * Copyright (C) 2009-2010 by Piotr Esden-Tempski <piotr@esden.net>
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

//#define GPC_DEBUG

#ifdef GPC_DEBUG
#include <stdio.h>
#define DEBUG(STR, ARGS...) printf(STR , ## ARGS)
#else
#define DEBUG(STR, ARGS...)
#endif

#include "lg/types.h"
#include "lg/ring.h"
#include "lg/gpdef.h"

#include "lg/gprotc.h"

struct gpc_hooks {
	gp_simple_hook_t trigger_output;
	void *trigger_output_data;
	gp_with_addr_hook_t register_changed;
	void *register_changed_data;
} gpc_hooks;

volatile u16 *gpc_register_map[32];

struct ring gpc_output_ring;
u8 gpc_output_buffer[128];

enum gpc_states {
	GPCS_IDLE,
	GPCS_DATA_LSB,
	GPCS_DATA_MSB
};
enum gpc_states gpc_state = GPCS_IDLE;
u16 gpc_addr;
u16 gpc_data;

u32 gpc_monitor_map;

int gpc_init(gp_simple_hook_t trigger_output, void *trigger_output_data,
	     gp_with_addr_hook_t register_changed, void *register_changed_data)
{
	int i;

	gpc_state = GPCS_IDLE;

	gpc_hooks.trigger_output = trigger_output;
	gpc_hooks.trigger_output_data = trigger_output_data;
	gpc_hooks.register_changed = register_changed;
	gpc_hooks.register_changed_data = register_changed_data;

	for (i = 0; i < 32; i++)
		gpc_register_map[i] = 0;

	gpc_monitor_map = 0;

	ring_init(&gpc_output_ring, gpc_output_buffer, 128);

	return 0;
}

int gpc_setup_reg(u8 addr, volatile u16 * reg)
{
	if (addr > 31)
		return 1;

	DEBUG("Setting up register %02X with %p\n", addr, reg);

	gpc_register_map[addr] = reg;

	return 0;
}

s32 gpc_pickup_byte(void)
{
	return ring_read_ch(&gpc_output_ring, 0);
}

int gpc_send_reg(u8 addr)
{
	u8 dat[3];

	if ((addr > 31) | !gpc_register_map[addr])
		return 1;

	dat[0] = addr;
	dat[1] = (*gpc_register_map[addr]) & 0xFF;
	dat[2] = (*gpc_register_map[addr]) >> 8;

	DEBUG("sending reg %02X with content %04X\n", addr,
	      *gpc_register_map[addr]);

	if (0 <= ring_write(&gpc_output_ring, dat, 3)) {
		if (gpc_hooks.trigger_output)
			gpc_hooks.trigger_output(gpc_hooks.trigger_output_data);
		return 0;
	}

	return 1;
}

int gpc_handle_byte(u8 byte)
{
	DEBUG("got byte %04X ", byte);

	switch (gpc_state) {
	case GPCS_IDLE:
		if (byte & GP_MODE_RESERVED) {
			DEBUG("reserved\n");
			return 1;
		}

		if ((byte & GP_MODE_MASK) == (GP_MODE_WRITE)) {
			DEBUG("write ");
			gpc_addr = byte & GP_ADDR_MASK;
			gpc_state = GPCS_DATA_LSB;
		} else if ((byte & GP_MODE_MASK) ==
			   (GP_MODE_READ | GP_MODE_PEEK)) {
			DEBUG("read ");
			gpc_send_reg(byte & GP_ADDR_MASK);
		} else if ((byte & GP_MODE_MASK) ==
			   (GP_MODE_READ | GP_MODE_CONT)) {
			DEBUG("read cont ");
			gpc_monitor_map ^= 1 << (byte & GP_ADDR_MASK);
		} else {
			DEBUG("unimplemented\n");
			return 1;
		}
		break;
	case GPCS_DATA_LSB:
		DEBUG("lsb ");
		gpc_data = byte;
		gpc_state = GPCS_DATA_MSB;
		break;
	case GPCS_DATA_MSB:
		DEBUG("msb ");
		gpc_data |= byte << 8;
		gpc_state = GPCS_IDLE;

		if (!gpc_register_map[gpc_addr]) {
			DEBUG("addr %02X with pointer %p not set up\n",
			      gpc_addr, gpc_register_map[gpc_addr]);
			return 1;
		}

		*gpc_register_map[gpc_addr] = gpc_data;
		if (gpc_hooks.register_changed)
			gpc_hooks.register_changed(gpc_hooks.
						   register_changed_data,
						   gpc_addr);

		break;
	default:
		return 1;
	}

	DEBUG("\n");
	return 0;
}

int gpc_register_touched(u8 addr)
{
	if (addr > 31)
		return 1;

	DEBUG("touched_register %02X, search mask %08X and register map %08X ",
	      addr, (1 << addr), gpc_monitor_map);
	if (gpc_monitor_map & (1 << addr)) {
		DEBUG("sending\n");
		gpc_send_reg(addr);
	} else {
		DEBUG("not sending\n");
		return 1;
	}

	return 0;
}
