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

#include "check_utils.h"
#include "check_suites.h"

u16 gp_register_map[32];

int gpm_register_changed = 0;
int gpm_register_changed_addr = 0;

int gpm_string_received = 0;
char gpm_string_received_string[1024];
int gpm_string_received_len = 0;
int gpc_get_version = 0;

void gpm_trigger_output_hook(void *data)
{
        s32 dat;

	data = data;

	while(-1 != (dat = gpm_pickup_byte())){
		if(gpc_handle_byte(dat))
			printf("gpc_handle_byte %02X failed.\n", dat);
	}
}

void gpm_register_changed_hook(void *data, u8 addr)
{
	data = data;

	gpm_register_changed_addr = addr;
	gpm_register_changed = 1;
}

void gpc_trigger_output_hook(void* data)
{
	s32 dat;

	data = data;

	while(-1 != (dat = gpc_pickup_byte())){
		if(gpm_handle_byte(dat))
			printf("gpm_handle_byte %02X failed.\n", dat);
	}
}

void gpm_string_received_hook(void *data, char *string, int len)
{
	data = data;

	gpm_string_received = 1;
	memcpy(gpm_string_received_string+gpm_string_received_len, string, len);
	gpm_string_received_len += len;
}

void gpc_get_version_hook(void *data)
{
	data = data;

	gpc_get_version = 1;
}

void init_gprot_tc(void)
{
	int i;

	gpm_init(gpm_trigger_output_hook, NULL, gpm_register_changed_hook, NULL);
	gpm_set_string_received_callback(gpm_string_received_hook, NULL);

	gpc_init(gpc_trigger_output_hook, NULL, NULL, NULL);
	gpc_set_get_version_callback(gpc_get_version_hook, NULL);

	for(i=0; i<32; i++){
		gp_register_map[i] = 0xAA55+i;
		gpc_setup_reg(i, &gp_register_map[i]);
	}
}

void clean_gprot_tc(void)
{
	gpm_register_changed = 0;
	gpm_register_changed_addr = 0;
	gpm_string_received = 0;
	memset(gpm_string_received_string, 0, sizeof(gpm_string_received_string));
	gpm_string_received_len = 0;
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

START_TEST(test_gprot_send_short_string)
{
	int i;
	char *string = "Hello World!";
	int string_size = strlen(string);

	fail_unless(string_size == gpc_send_string(string, string_size));

	fail_unless(string_size == gpm_string_received_len);
	fail_unless(1 == gpm_string_received);

	for (i=0; i<string_size; i++) {
		fail_unless(string[i] == gpm_string_received_string[i]);
	}

	fail_unless(0 == gpm_string_received_string[i]);
}
END_TEST

START_TEST(test_gprot_send_long_string)
{
	int i;
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

	fail_unless(string_size == gpc_send_string(string, string_size));

	fail_unless(string_size == gpm_string_received_len);
	fail_unless(1 == gpm_string_received);

	for (i=0; i<string_size; i++) {
		fail_unless(string[i] == gpm_string_received_string[i]);
	}

	fail_unless(0 == gpm_string_received_string[i]);
}
END_TEST

START_TEST(test_gprot_send_arbitrary_string)
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

	for (j=0; j<=string_size; j++) {

		fail_unless(j == gpc_send_string(string, j));

		fail_unless(j == gpm_string_received_len);
		fail_unless(1 == gpm_string_received);

		for (i=0; i<j; i++) {
			fail_unless(string[i] == gpm_string_received_string[i]);
		}

		fail_unless(0 == gpm_string_received_string[i]);

		gpm_string_received_len = 0;
		gpm_string_received = 0;
		memset(gpm_string_received_string, 0, sizeof(gpm_string_received_string));
	}

}
END_TEST

START_TEST(test_gprot_get_client_version)
{
	fail_unless(0 == gpm_send_get_version());

	fail_unless(1 == gpc_get_version);

	fail_unless(0 == regmatch("^libgovernor [[:digit:]]+\\.[[:digit:]]+-[[:alnum:]]{8}(-dirty)?, build [[:digit:]]{8}$", gpm_string_received_string));
	fail_unless(0 == regmatch("^Copyright \\(C\\) 2010-20[[:digit:]]{2} Piotr Esden-Tempski <piotr@esden.net>$", gpm_string_received_string));
	fail_unless(0 == regmatch("^License GPLv3\\+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>$", gpm_string_received_string));

	memset(gpm_string_received_string, 0, sizeof(gpm_string_received_string));
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
	tcase_add_test(tc, test_gprot_send_short_string);
	tcase_add_test(tc, test_gprot_send_long_string);
	tcase_add_test(tc, test_gprot_send_arbitrary_string);
	tcase_add_test(tc, test_gprot_get_client_version);

	return s;
}
