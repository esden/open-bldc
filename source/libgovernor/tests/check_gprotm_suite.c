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

#include "check_suites.h"

int gpm_dummy_trigger_output_triggered = 0;
void *gpm_dummy_trigger_output_data = 0;

int gpm_dummy_register_changed = 0;
int gpm_dummy_register_changed_addr = 0;
void *gpm_dummy_register_changed_data = 0;

int gpm_dummy_string_received = 0;
char gpm_dummy_string_received_string[128];
void *gpm_dummy_string_received_data = 0;
int gpm_dummy_string_received_len = 0;

void gpm_dummy_trigger_output_hook(void *data)
{
	gpm_dummy_trigger_output_data = data;
	gpm_dummy_trigger_output_triggered = 1;
}

void gpm_dummy_register_changed_hook(void *data, u8 addr)
{
	gpm_dummy_register_changed_data = data;
	gpm_dummy_register_changed_addr = addr;
	gpm_dummy_register_changed = 1;
}

void gpm_dummy_string_received_hook(void *data, char *string, int len)
{
	gpm_dummy_string_received_data = data;
	memcpy(gpm_dummy_string_received_string, string, len);
	gpm_dummy_string_received = 1;
	gpm_dummy_string_received_len = len;
}

void init_gprotm_tc(void)
{
	gpm_init(gpm_dummy_trigger_output_hook, (void *)1, gpm_dummy_register_changed_hook, (void *)1);
	gpm_set_string_received_callback(gpm_dummy_string_received_hook, (void *) 1);
}

void clean_gprotm_tc(void)
{
	gpm_dummy_trigger_output_triggered = 0;
	gpm_dummy_register_changed = 0;
	gpm_dummy_register_changed_addr = 0;
	gpm_dummy_string_received = 0;
	memset(gpm_dummy_string_received_string, 0, sizeof(gpm_dummy_string_received_string));
	gpm_dummy_string_received = 0;
	gpm_dummy_string_received_len = 0;
}

START_TEST(test_gprotm_get_register_map_val)
{
	u8 addr;

	for(addr = 0; addr<32; addr++){
		fail_unless(0x0000 == gpm_get_register_map_val(addr));
	}

	for(addr = 32; addr!=0; addr++){
		fail_unless(-1 == gpm_get_register_map_val(addr));
	}
}
END_TEST

START_TEST(test_gprotm_send_set)
{
	u8 addr;

	/* test data zero */
	for(addr = 0; addr<32; addr++){
		fail_unless(0 == gpm_send_set(addr, 0x0000));

		fail_unless(1 == gpm_dummy_trigger_output_triggered);
		fail_unless((void *)1 == gpm_dummy_trigger_output_data);
		fail_unless((addr | GP_MODE_WRITE) == gpm_pickup_byte());
		fail_unless(0x00 == gpm_pickup_byte());
		fail_unless(0x00 == gpm_pickup_byte());
		fail_unless(-1 == gpm_pickup_byte());

		fail_unless(0x0000 == gpm_get_register_map_val(addr));

		gpm_dummy_trigger_output_triggered = 0;
		gpm_dummy_trigger_output_data = 0;
	}

	/* test data some value */
	for(addr = 0; addr<32; addr++){
		fail_unless(0 == gpm_send_set(addr, 0xAA55));

		fail_unless(1 == gpm_dummy_trigger_output_triggered);
		fail_unless((void *)1 == gpm_dummy_trigger_output_data);
		fail_unless((addr | GP_MODE_WRITE) == gpm_pickup_byte());
		fail_unless(0x55 == gpm_pickup_byte());
		fail_unless(0xAA == gpm_pickup_byte());
		fail_unless(-1 == gpm_pickup_byte());

		fail_unless(0xAA55 == gpm_get_register_map_val(addr));

		gpm_dummy_trigger_output_triggered = 0;
		gpm_dummy_trigger_output_data = 0;
	}

	/* test data max value */
	for(addr = 0; addr<32; addr++){
		fail_unless(0 == gpm_send_set(addr, 0xFFFF));

		fail_unless(1 == gpm_dummy_trigger_output_triggered);
		fail_unless((void *)1 == gpm_dummy_trigger_output_data);
		fail_unless((addr | GP_MODE_WRITE) == gpm_pickup_byte());
		fail_unless(0xFF == gpm_pickup_byte());
		fail_unless(0xFF == gpm_pickup_byte());
		fail_unless(-1 == gpm_pickup_byte());

		fail_unless(0xFFFF == gpm_get_register_map_val(addr));

		gpm_dummy_trigger_output_triggered = 0;
		gpm_dummy_trigger_output_data = 0;
	}

	/* test invalid addresses */
	for(addr = 32; addr!=0; addr++){
		fail_unless(1 == gpm_send_set(addr, 0x0000));

		fail_unless(0 == gpm_dummy_trigger_output_triggered);
		fail_unless((void *)0 == gpm_dummy_trigger_output_data);
		fail_unless(-1 == gpm_pickup_byte());

		gpm_dummy_trigger_output_triggered = 0;
		gpm_dummy_trigger_output_data = 0;
	}

}
END_TEST

START_TEST(test_gprotm_send_get)
{
	u8 addr;

	/* test all valid addresses */
	for(addr = 0; addr<32; addr++){
		fail_unless(0 == gpm_send_get(addr));

		fail_unless(1 == gpm_dummy_trigger_output_triggered);
		fail_unless((void *)1 == gpm_dummy_trigger_output_data);
		fail_unless((addr | GP_MODE_READ | GP_MODE_PEEK) == gpm_pickup_byte());
		fail_unless(-1 == gpm_pickup_byte());

		gpm_dummy_trigger_output_triggered = 0;
		gpm_dummy_trigger_output_data = 0;
	}

	/* test invalid addresses */
	for(addr = 32; addr!=0; addr++){
		fail_unless(1 == gpm_send_get(addr));

		fail_unless(0 == gpm_dummy_trigger_output_triggered);
		fail_unless(-1 == gpm_pickup_byte());

		gpm_dummy_trigger_output_triggered = 0;
		gpm_dummy_trigger_output_data = 0;
	}
}
END_TEST

START_TEST(test_gprotm_send_get_cont)
{
	u8 addr;

	/* test all valid addresses */
	for(addr = 0; addr<32; addr++){
		fail_unless(0 == gpm_send_get_cont(addr));

		fail_unless(1 == gpm_dummy_trigger_output_triggered);
		fail_unless((void *)1 == gpm_dummy_trigger_output_data);
		fail_unless((addr | GP_MODE_READ | GP_MODE_CONT) == gpm_pickup_byte());
		fail_unless(-1 == gpm_pickup_byte());

		gpm_dummy_trigger_output_triggered = 0;
		gpm_dummy_trigger_output_data = 0;
	}

	/* test invalid addresses */
	for(addr = 32; addr!=0; addr++){
		fail_unless(1 == gpm_send_get_cont(addr));

		fail_unless(0 == gpm_dummy_trigger_output_triggered);
		fail_unless((void *)0 == gpm_dummy_trigger_output_data);
		fail_unless(-1 == gpm_pickup_byte());

		gpm_dummy_trigger_output_triggered = 0;
		gpm_dummy_trigger_output_data = 0;
	}
}
END_TEST

START_TEST(test_gprotm_handle_byte_registers)
{
	u8 addr = 0;
	u16 data = 0;

	/* check all valid addresses with data zero */
	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpm_handle_byte(addr));
		fail_unless(0 == gpm_dummy_register_changed);
		fail_unless((void *)0 == gpm_dummy_register_changed_data);
		fail_unless(0 == gpm_handle_byte(data & 0xFF));
		fail_unless(0 == gpm_dummy_register_changed);
		fail_unless((void *)0 == gpm_dummy_register_changed_data);
		fail_unless(0 == gpm_handle_byte(data >> 8));
		fail_unless(1 == gpm_dummy_register_changed);
		fail_unless(addr == gpm_dummy_register_changed_addr);
		fail_unless((void *)1 == gpm_dummy_register_changed_data);
		fail_unless(data == gpm_get_register_map_val(gpm_dummy_register_changed_addr));
		gpm_dummy_register_changed = 0;
		gpm_dummy_register_changed_addr = 0;
		gpm_dummy_register_changed_data = 0;
	}

	/* check all valid addresses with data 0xAA55 */
	data = 0xAA55;
	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpm_handle_byte(addr));
		fail_unless(0 == gpm_dummy_register_changed);
		fail_unless((void *)0 == gpm_dummy_register_changed_data);
		fail_unless(0 == gpm_handle_byte(data & 0xFF));
		fail_unless(0 == gpm_dummy_register_changed);
		fail_unless((void *)0 == gpm_dummy_register_changed_data);
		fail_unless(0 == gpm_handle_byte(data >> 8));
		fail_unless(1 == gpm_dummy_register_changed);
		fail_unless(addr == gpm_dummy_register_changed_addr);
		fail_unless((void *)1 == gpm_dummy_register_changed_data);
		fail_unless(data == gpm_get_register_map_val(gpm_dummy_register_changed_addr));
		gpm_dummy_register_changed = 0;
		gpm_dummy_register_changed_addr = 0;
		gpm_dummy_register_changed_data = 0;
	}

	/* check all valid addresses with data 0xFFFF */
	data = 0xFFFF;
	for(addr=0; addr<32; addr++){
		fail_unless(0 == gpm_handle_byte(addr));
		fail_unless(0 == gpm_dummy_register_changed);
		fail_unless((void *)0 == gpm_dummy_register_changed_data);
		fail_unless(0 == gpm_handle_byte(data & 0xFF));
		fail_unless(0 == gpm_dummy_register_changed);
		fail_unless((void *)0 == gpm_dummy_register_changed_data);
		fail_unless(0 == gpm_handle_byte(data >> 8));
		fail_unless(1 == gpm_dummy_register_changed);
		fail_unless(addr == gpm_dummy_register_changed_addr);
		fail_unless((void *)1 == gpm_dummy_register_changed_data);
		fail_unless(data == gpm_get_register_map_val(gpm_dummy_register_changed_addr));
		gpm_dummy_register_changed = 0;
		gpm_dummy_register_changed_addr = 0;
		gpm_dummy_register_changed_data = 0;
	}

	/* check all invalid addresses */
	for(addr=32; addr<GP_MODE_STRING; addr++){
		fail_unless(1 == gpm_handle_byte(addr));
		fail_unless(0 == gpm_dummy_register_changed);
		fail_unless(0 == gpm_dummy_register_changed_addr);
		fail_unless((void *)0 == gpm_dummy_register_changed_data);
		gpm_dummy_register_changed = 0;
		gpm_dummy_register_changed_addr = 0;
		gpm_dummy_register_changed_data = 0;
	}
}
END_TEST

START_TEST(test_gprotm_handle_byte_short_string)
{
	int i;
	char *string = "Hello World!";
	int string_size = strlen(string);

	fail_unless(0 == gpm_handle_byte(GP_MODE_STRING | string_size));

	for (i=0; i<string_size; i++) {
		fail_unless(0 == gpm_handle_byte(string[i]));
	}

	fail_unless(1 == gpm_dummy_string_received);
	fail_unless((void *)1 == gpm_dummy_string_received_data);
	fail_unless(i == gpm_dummy_string_received_len);

	for (i=0; i<string_size; i++) {
		fail_unless(string[i] == gpm_dummy_string_received_string[i]);
	}

	fail_unless(1 == gpm_handle_byte(33));
}
END_TEST

START_TEST(test_gprotm_handle_byte_long_string)
{
	int i, j;
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
	for (i=0; i<(string_size / GP_STR_PAK_MAX_LEN); i++) {

		fail_unless(0 == gpm_handle_byte(GP_MODE_STRING | GP_STR_PAK_MAX_LEN));

		for (j=0; j<GP_STR_PAK_MAX_LEN; j++) {
			fail_unless(0 == gpm_handle_byte(string[j + (i * GP_STR_PAK_MAX_LEN)]));
		}

		fail_unless(1 == gpm_dummy_string_received);
		fail_unless((void *)1 == gpm_dummy_string_received_data);
		fail_unless(GP_STR_PAK_MAX_LEN == gpm_dummy_string_received_len);

		for (j=0; j<GP_STR_PAK_MAX_LEN; j++) {
			fail_unless(string[j + (i * GP_STR_PAK_MAX_LEN)] == gpm_dummy_string_received_string[j]);
		}

		gpm_dummy_string_received = 0;
		gpm_dummy_string_received_data = 0;
		memset(gpm_dummy_string_received_string, 0, sizeof(gpm_dummy_string_received_string));
	}

	fail_unless(0 == gpm_handle_byte(GP_MODE_STRING | (string_size % GP_STR_PAK_MAX_LEN)));

	for (j=0; j<(string_size % GP_STR_PAK_MAX_LEN); j++) {
		fail_unless(0 == gpm_handle_byte(string[j + (i * GP_STR_PAK_MAX_LEN)]));
	}

	fail_unless(1 == gpm_dummy_string_received);
	fail_unless((void *)1 == gpm_dummy_string_received_data);
	fail_unless(j == gpm_dummy_string_received_len);

	for (j=0; j<(string_size % GP_STR_PAK_MAX_LEN); j++) {
		fail_unless(string[j + (i * GP_STR_PAK_MAX_LEN)] == gpm_dummy_string_received_string[j]);
	}

	fail_unless(1 == gpm_handle_byte(33));
}
END_TEST

Suite *make_lg_gprotm_suite(void)
{
	Suite *s;
	TCase *tc;

	s= suite_create("Governor protocol for master");

	tc = tcase_create("Basic functions");
	suite_add_tcase(s, tc);
	tcase_add_checked_fixture(tc, init_gprotm_tc, clean_gprotm_tc);
	tcase_add_test(tc, test_gprotm_get_register_map_val);
	tcase_add_test(tc, test_gprotm_send_set);
	tcase_add_test(tc, test_gprotm_send_get);
	tcase_add_test(tc, test_gprotm_send_get_cont);
	tcase_add_test(tc, test_gprotm_handle_byte_registers);
	tcase_add_test(tc, test_gprotm_handle_byte_short_string);
	tcase_add_test(tc, test_gprotm_handle_byte_long_string);


	return s;
}
