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
#include "lg/gprotc.h"

#include "check_suites.h"

u16 gpc_dummy_register_map[32];

void *gpc_dummy_trigger_output_data = 0;
int gpc_dummy_trigger_output_triggered = 0;
void *gpc_dummy_register_changed_data = 0;
int gpc_dummy_register_changed = 0;
int gpc_dummy_register_changed_addr = 0;

void gpc_dummy_trigger_output_hook(void* data)
{
	gpc_dummy_trigger_output_data = data;
	gpc_dummy_trigger_output_triggered = 1;
}

void gpc_dummy_register_changed_hook(void* data, u8 addr)
{
	gpc_dummy_register_changed_data = data;
	gpc_dummy_register_changed = 1;
	gpc_dummy_register_changed_addr = addr;
}

void init_gprotc_tc(void)
{
	int i;

	for(i=0; i<32; i++)
		gpc_dummy_register_map[i] = 0xAA55+i;

	gpc_init(gpc_dummy_trigger_output_hook, (void *)1, gpc_dummy_register_changed_hook, (void*)1);

	gpc_dummy_trigger_output_data = 0;
	gpc_dummy_trigger_output_triggered = 0;
	gpc_dummy_register_changed_data = 0;
	gpc_dummy_register_changed = 0;
	gpc_dummy_register_changed_addr = 0;
}

void clean_gprotc_tc(void)
{
}

START_TEST(test_gprotc_send_reg)
{
	int i;
	u8 addr;

	addr = 0;
	for(i = 0; i<256; i++){
		fail_unless(1 == gpc_send_reg(addr));
		fail_unless(-1 == gpc_pickup_byte());
		addr++;
	}

	fail_unless(0 == gpc_setup_reg(0, &gpc_dummy_register_map[0]));
	fail_unless(0 == gpc_send_reg(0));
	fail_unless(0 == gpc_pickup_byte());
	fail_unless(0x55 == gpc_pickup_byte());
	fail_unless(0xAA == gpc_pickup_byte());
	fail_unless(-1 == gpc_pickup_byte());
	fail_unless(1 == gpc_send_reg(1));

	for(addr=1; addr<32; addr++){
		fail_unless(0 == gpc_setup_reg(addr, &gpc_dummy_register_map[addr]));
		fail_unless(0 == gpc_send_reg(addr));
		fail_unless(addr == gpc_pickup_byte());
		fail_unless(0x55+addr == gpc_pickup_byte());
		fail_unless(0xAA == gpc_pickup_byte());
		fail_unless(-1 == gpc_pickup_byte());
	}


	for(addr=32; addr!=0; addr++){
		fail_unless(1 == gpc_send_reg(addr));
		fail_unless(-1 == gpc_pickup_byte());
	}
}
END_TEST

START_TEST(test_gprotc_handle_byte_read)
{
	u8 addr = 0;

	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpc_setup_reg(addr, &gpc_dummy_register_map[addr]));
	}

	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpc_handle_byte(addr | GP_MODE_READ | GP_MODE_PEEK));
		fail_unless(1 == gpc_dummy_trigger_output_triggered);
		fail_unless((void *)1 == gpc_dummy_trigger_output_data);
		fail_unless(addr == gpc_pickup_byte());
		fail_unless(0x55+addr == gpc_pickup_byte());
		fail_unless(0xAA == gpc_pickup_byte());
		fail_unless(-1 == gpc_pickup_byte());

		gpc_dummy_trigger_output_triggered = 0;
		gpc_dummy_trigger_output_data = 0;
	}

	for(addr=0; addr<32; addr++){
		fail_unless(1 == gpc_handle_byte(addr | GP_MODE_READ | GP_MODE_PEEK | GP_MODE_STRING));
		fail_unless(0 == gpc_dummy_trigger_output_triggered);
		fail_unless((void *)0 == gpc_dummy_trigger_output_data);
		fail_unless(-1 == gpc_pickup_byte());
	}
}
END_TEST

START_TEST(test_gprotc_handle_byte_write)
{
	u8 addr = 0;
	u16 data = 0xDADE;

	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpc_setup_reg(addr, &gpc_dummy_register_map[addr]));
	}

	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpc_handle_byte(addr | GP_MODE_WRITE));
		fail_unless(0 == gpc_handle_byte(data & 0xFF));
		fail_unless(0 == gpc_handle_byte(data >> 8));
		fail_unless(1 == gpc_dummy_register_changed);
		fail_unless(addr == gpc_dummy_register_changed_addr);
		fail_unless((void *)1 == gpc_dummy_register_changed_data);
		fail_unless(data == gpc_dummy_register_map[addr]);

		gpc_dummy_register_changed = 0;
		gpc_dummy_register_changed_addr = 0;
		gpc_dummy_register_changed_data = 0;
	}

	//for(addr=0; addr<32; addr++){
	//	fail_unless(1 == gpc_handle_byte(addr | GP_MODE_WRITE | GP_MODE_STRING));
	//	fail_unless(0 == gpc_dummy_register_changed);
	//	fail_unless(0 == gpc_dummy_register_changed_addr);
	//	fail_unless((void *)0 == gpc_dummy_register_changed_data);
	//	fail_unless(-1 == gpc_pickup_byte());
	//
	//	gpc_dummy_register_changed = 0;
	//	gpc_dummy_register_changed_addr = 0;
	//	gpc_dummy_register_changed_data = 0;
	//}
}
END_TEST

START_TEST(test_gprotc_read_cont)
{
	u16 addr = 0;

	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpc_setup_reg(addr, &gpc_dummy_register_map[addr]));
	}

	for(addr=0; addr<32; addr++){
		gpc_dummy_register_map[addr] = addr | ((addr+1) << 8);
	}

	for(addr=0; addr<256; addr++){
		fail_unless(1 == gpc_register_touched(addr));
		fail_unless(0 == gpc_dummy_register_changed);
		fail_unless(0 == gpc_dummy_register_changed_addr);
		fail_unless((void *)0 == gpc_dummy_register_changed_data);
	}

	for(addr=0; addr<31; addr++){
		fail_unless(0 == gpc_handle_byte(addr | GP_MODE_READ | GP_MODE_CONT));
		fail_unless(0 == gpc_register_touched(addr));
		fail_unless(addr == gpc_pickup_byte());
		fail_unless(addr == gpc_pickup_byte());
		fail_unless(addr+1 == gpc_pickup_byte());
		fail_unless(-1 == gpc_pickup_byte());
		fail_unless(0 == gpc_handle_byte(addr | GP_MODE_READ | GP_MODE_CONT));
		fail_unless(1 == gpc_register_touched(addr));
		fail_unless(0 == gpc_dummy_register_changed);
		fail_unless(0 == gpc_dummy_register_changed_addr);
		fail_unless((void *)0 == gpc_dummy_register_changed_data);
	}
}
END_TEST

START_TEST(test_gprotc_send_short_string)
{
	int i;
	char *string = "Hello World!";
	int string_size = strlen(string);

	fail_unless(string_size == gpc_send_string(string, string_size));

	fail_unless((GP_MODE_STRING | string_size) == gpc_pickup_byte());

	for (i=0; i<string_size; i++) {
		fail_unless(string[i] == gpc_pickup_byte());
	}

	fail_unless(-1 == gpc_pickup_byte());
}
END_TEST

START_TEST(test_gprotc_send_long_string)
{
	int i,j;
	int string_size = (('z' - 'a') + 1) * ((('9' - '0') + 1) + 1);
	char string[string_size];

	/* Generate string. */
	{
		int gi, gj, gk = 0;

		for(gi=0; gi<(('z' - 'a') + 1); gi++) {
			string[gk++] = 'a'+gi;
			for(gj=0; gj<(('9' - '0') + 1); gj++){
				string[gk++] = '0'+gj;
			}
		}

		string[gk] = '\0';

	}

	/* Run test. */
	fail_unless(string_size == gpc_send_string(string, string_size));

	for (j=0; j<(string_size / GP_STR_PAK_MAX_LEN); j++) {
		fail_unless((GP_MODE_STRING | GP_STR_PAK_MAX_LEN) == gpc_pickup_byte());

		for (i=0; i<GP_STR_PAK_MAX_LEN; i++) {
			fail_unless(string[i + (j * GP_STR_PAK_MAX_LEN)] == gpc_pickup_byte());
		}
	}

	fail_unless((GP_MODE_STRING | (string_size % GP_STR_PAK_MAX_LEN)) == gpc_pickup_byte());


	for (i=0; i<(string_size % GP_STR_PAK_MAX_LEN); i++) {
		fail_unless(string[i + (j * GP_STR_PAK_MAX_LEN)] == gpc_pickup_byte());
	}

	fail_unless(-1 == gpc_pickup_byte());
}
END_TEST

Suite *make_lg_gprotc_suite(void)
{
	Suite *s;
	TCase *tc;

	s= suite_create("Governor protocol for client");

	tc = tcase_create("Basic functions");
	suite_add_tcase(s, tc);
	tcase_add_checked_fixture(tc, init_gprotc_tc, clean_gprotc_tc);
	tcase_add_test(tc, test_gprotc_send_reg);
	tcase_add_test(tc, test_gprotc_handle_byte_read);
	tcase_add_test(tc, test_gprotc_handle_byte_write);
	tcase_add_test(tc, test_gprotc_read_cont);
	tcase_add_test(tc, test_gprotc_send_short_string);
	tcase_add_test(tc, test_gprotc_send_long_string);

	return s;
}
