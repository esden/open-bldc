/*
 * libgovernor - Open-BLDC configuration and debug protocol library
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

#include <check.h>

#include <string.h>
#include <stdio.h>

#include "lg/types.h"
#include "lg/gpdef.h"
#include "lg/gprotm.h"
#include "lg/gprotc.h"

#include "check_suites.h"

u16 gp_register_map[32];

int gpm_register_changed = 0;
int gpm_register_changed_addr = 0;

void gpm_trigger_output_hook(void)
{
        s32 dat;

	while(-1 != (dat = gpm_pickup_byte())){
		if(gpc_handle_byte(dat))
			printf("gpc_handle_byte %02X failed.\n", dat);
	}
}

void gpm_register_changed_hook(u8 addr)
{
	gpm_register_changed_addr = addr;
	gpm_register_changed = 1;
}

void gpc_trigger_output_hook(void)
{
	s32 dat;

	while(-1 != (dat = gpc_pickup_byte())){
		if(gpm_handle_byte(dat))
			printf("gpm_handle_byte %02X failed.\n", dat);
	}
}

void init_gprot_tc(void)
{
	int i;

	gpm_init(gpm_trigger_output_hook, gpm_register_changed_hook);

	gpc_init(gpc_trigger_output_hook);

	for(i=0; i<32; i++){
		gp_register_map[i] = 0xAA55+i;
		gpc_setup_reg(i, &gp_register_map[i]);
	}
}

void clean_gprot_tc(void)
{
}

START_TEST(test_gprot_write)
{
	u8 addr = 0;

	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpm_send_set(addr, 0x0000));
		fail_unless(0x0000 == gp_register_map[addr]);
		fail_unless(0x0000 == gpm_get_register_map_val(addr));
	}

	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpm_send_set(addr, 0xAA55));
		fail_unless(0xAA55 == gp_register_map[addr]);
		fail_unless(0xAA55 == gpm_get_register_map_val(addr));
	}

	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpm_send_set(addr, 0xFFFF));
		fail_unless(0xFFFF == gp_register_map[addr]);
		fail_unless(0xFFFF == gpm_get_register_map_val(addr));
	}
}
END_TEST

START_TEST(test_gprot_read)
{
	u8 addr;

	for(addr=0; addr<32; addr++){
		gp_register_map[addr] = 0x0000;
	}

	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpm_send_get(addr));
		fail_unless(0x0000 == gpm_get_register_map_val(addr));
	}

	for(addr=0; addr<32; addr++){
		gp_register_map[addr] = 0xAA55;
	}

	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpm_send_get(addr));
		fail_unless(0xAA55 == gpm_get_register_map_val(addr));
	}

	for(addr=0; addr<32; addr++){
		gp_register_map[addr] = 0xFFFF;
	}

	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpm_send_get(addr));
		fail_unless(0xFFFF == gpm_get_register_map_val(addr));
	}
}
END_TEST

START_TEST(test_gprot_read_write)
{
	u8 addr;

	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpm_send_set(addr, 0x0000+addr));
		fail_unless(0 == gpm_send_get(addr));
		fail_unless(0x0000+addr == gpm_get_register_map_val(addr));
	}

}
END_TEST

Suite *make_lg_gprot_suite()
{
	Suite *s;
	TCase *tc;

	s= suite_create("Governor protocol master and client");

	tc = tcase_create("Basic functions");
	suite_add_tcase(s, tc);
	tcase_add_checked_fixture(tc, init_gprot_tc, clean_gprot_tc);
	tcase_add_test(tc, test_gprot_write);
	tcase_add_test(tc, test_gprot_read);
	tcase_add_test(tc, test_gprot_read_write);

	return s;
}
