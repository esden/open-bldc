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

/* governor protocol implementation for the master */

/*
 * 0000 0000
 * ^^^^ ^^^^
 * |||'-''''- Register
 * ||'------- Reserved
 * |'-------- 0 ^= Peek 1 ^= Continous
 * '--------- 0 ^= Read 1 ^= Write
 */

#include "lg/types.h"
#include "lg/ring.h"
#include "lg/gpdef.h"

#include "lg/gprotm.h"

struct gpm_hooks {
	gp_simple_hook_t trigger_output;
	void *trigger_output_data;
	gp_with_addr_hook_t register_changed;
	void *register_changed_data;
} gpm_hooks;

u16 gpm_register_map[32];

struct ring gpm_output_ring;
u8 gpm_output_buffer[128];

enum gpm_states {
	GPMS_IDLE,
	GPMS_DATA_LSB,
	GPMS_DATA_MSB
};
enum gpm_states gpm_state = GPMS_IDLE;
u16 gpm_addr;
u16 gpm_data;

int gpm_init(gp_simple_hook_t trigger_output, void *trigger_output_data, gp_with_addr_hook_t register_changed, void *register_changed_data)
{
	int i;

	gpm_hooks.trigger_output = trigger_output;
	gpm_hooks.trigger_output_data = trigger_output_data;
	gpm_hooks.register_changed = register_changed;
	gpm_hooks.register_changed_data = register_changed_data;

	for(i=0; i<32; i++)
		gpm_register_map[i] = 0;

	ring_init(&gpm_output_ring, gpm_output_buffer, 128);

	return 0;
}

s32 gpm_get_register_map_val(u8 addr)
{
	if(addr > 31)
		return -1;

	return gpm_register_map[addr];
}

s32 gpm_pickup_byte(void)
{
	return ring_read_ch(&gpm_output_ring, 0);
}

int gpm_send_set(u8 addr, u16 val)
{
    u8 dat[3];

    if(addr > 31)
	    return 1;

    dat[0] = addr | GP_MODE_WRITE;
    dat[1] = val & 0xFF;
    dat[2] = val >> 8;

    if(0 <= ring_write(&gpm_output_ring, dat, 3)){
	    if(gpm_hooks.trigger_output) gpm_hooks.trigger_output(gpm_hooks.trigger_output_data);
	    gpm_register_map[addr] = val;
	    return 0;
    }

    return 1;
}

int gpm_send_get(u8 addr)
{
    u8 out = addr | GP_MODE_READ | GP_MODE_PEEK;

    if(addr > 31)
	    return 1;

    if(0 <= ring_write_ch(&gpm_output_ring, out)){
	    if(gpm_hooks.trigger_output) gpm_hooks.trigger_output(gpm_hooks.trigger_output_data);
	    return 0;
    }

    return 1;
}

int gpm_send_get_cont(u8 addr)
{
    u8 out = addr | GP_MODE_READ | GP_MODE_CONT;

    if(addr > 31)
	    return 1;

    if(0 <= ring_write_ch(&gpm_output_ring, out)){
	    if(gpm_hooks.trigger_output) gpm_hooks.trigger_output(gpm_hooks.trigger_output_data);
	    return 0;
    }

    return 1;
}

int gpm_handle_byte(u8 byte)
{
	switch(gpm_state){
	case GPMS_IDLE:
		if(byte > 31)
			return 1;

		gpm_addr = byte;
		gpm_state = GPMS_DATA_LSB;
		break;
	case GPMS_DATA_LSB:
		gpm_data = byte;
		gpm_state = GPMS_DATA_MSB;
		break;
	case GPMS_DATA_MSB:
		gpm_data |= byte << 8;
		gpm_register_map[gpm_addr] = gpm_data;
		if(gpm_hooks.register_changed)
			gpm_hooks.register_changed(gpm_hooks.register_changed_data, gpm_addr);
		gpm_state = GPMS_IDLE;
		break;
	}

	return 0;
}
