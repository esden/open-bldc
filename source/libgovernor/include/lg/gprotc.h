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

#ifndef LG_GPROTC_H
#define LG_GPROTC_H

int gpc_init(gp_simple_hook_t trigger_output, void *trigger_output_data,
	     gp_with_addr_hook_t register_changed, void *register_changed_data);
int gpc_set_get_version_callback(gp_simple_hook_t get_version, void *get_version_data);
int gpc_setup_reg(u8 addr, volatile u16 * reg);
s32 gpc_pickup_byte(void);
int gpc_send_reg(u8 addr);
int gpc_handle_byte(u8 ch);
int gpc_register_touched(u8 addr);
int gpc_send_string(char *string, int len);

#endif /* LG_GPROTC_H */
