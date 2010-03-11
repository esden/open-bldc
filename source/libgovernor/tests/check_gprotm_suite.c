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

#include <CUnit/Basic.h>

#include <string.h>

#include "lg/types.h"
#include "lg/gpdef.h"
#include "lg/gprotm.h"

#include "check_gprotm_suite.h"

int gpm_dummy_trigger_output_triggered = 0;
int gpm_dummy_register_changed = 0;
int gpm_dummy_register_changed_addr = 0;
void gpm_dummy_trigger_output_hook(void);
void gpm_dummy_register_changed_hook(u8 addr);

int init_gprotm_suite(void);
int clean_gprotm_suite(void);
void test_gprotm_get_register_map_val(void);
void test_gprotm_send_set(void);
void test_gprotm_send_get(void);
void test_gprotm_send_get_cont(void);
void test_gprotm_handle_byte(void);

int check_gprotm_suite_register()
{
	CU_pSuite pSuite = NULL;

	/* add suite to the registry */
	pSuite = CU_add_suite("governor protocol for master suite", init_gprotm_suite, clean_gprotm_suite);
	if(NULL == pSuite){
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if((NULL == CU_add_test(pSuite, "gprotm get_register_map_val", test_gprotm_get_register_map_val)) ||
	   (NULL == CU_add_test(pSuite, "gprotm send_set", test_gprotm_send_set)) ||
	   (NULL == CU_add_test(pSuite, "gprotm send_get", test_gprotm_send_get)) ||
	   (NULL == CU_add_test(pSuite, "gprotm send_get_cont", test_gprotm_send_get_cont)) ||
	   (NULL == CU_add_test(pSuite, "gprotm send_handle_byte", test_gprotm_handle_byte))){
		CU_cleanup_registry();
		return CU_get_error();
	}

	return 0;
}

void gpm_dummy_trigger_output_hook(void)
{
	gpm_dummy_trigger_output_triggered = 1;
}

void gpm_dummy_register_changed_hook(u8 addr)
{
	gpm_dummy_register_changed_addr = addr;
	gpm_dummy_register_changed = 1;
}

int init_gprotm_suite(void)
{
	gpm_init(gpm_dummy_trigger_output_hook, gpm_dummy_register_changed_hook);
	return 0;
}

int clean_gprotm_suite(void)
{
	return 0;
}

void test_gprotm_get_register_map_val(void)
{
	u8 addr;

	for(addr = 0; addr<32; addr++){
		CU_ASSERT(0x0000 == gpm_get_register_map_val(addr));
	}

	for(addr = 32; addr!=0; addr++){
		CU_ASSERT(-1 == gpm_get_register_map_val(addr));
	}
}

void test_gprotm_send_set(void)
{
	u8 addr;

	/* test data zero */
	for(addr = 0; addr<32; addr++){
		CU_ASSERT(0 == gpm_send_set(addr, 0x0000));

		CU_ASSERT(1 == gpm_dummy_trigger_output_triggered);
		CU_ASSERT((addr | GP_MODE_WRITE) == gpm_pickup_byte());
		CU_ASSERT(0x00 == gpm_pickup_byte());
		CU_ASSERT(0x00 == gpm_pickup_byte());
		CU_ASSERT(-1 == gpm_pickup_byte());

		CU_ASSERT(0x0000 == gpm_get_register_map_val(addr));

		gpm_dummy_trigger_output_triggered = 0;
	}

	/* test data some value */
	for(addr = 0; addr<32; addr++){
		CU_ASSERT(0 == gpm_send_set(addr, 0xAA55));

		CU_ASSERT(1 == gpm_dummy_trigger_output_triggered);
		CU_ASSERT((addr | GP_MODE_WRITE) == gpm_pickup_byte());
		CU_ASSERT(0x55 == gpm_pickup_byte());
		CU_ASSERT(0xAA == gpm_pickup_byte());
		CU_ASSERT(-1 == gpm_pickup_byte());

		CU_ASSERT(0xAA55 == gpm_get_register_map_val(addr));

		gpm_dummy_trigger_output_triggered = 0;
	}

	/* test data max value */
	for(addr = 0; addr<32; addr++){
		CU_ASSERT(0 == gpm_send_set(addr, 0xFFFF));

		CU_ASSERT(1 == gpm_dummy_trigger_output_triggered);
		CU_ASSERT((addr | GP_MODE_WRITE) == gpm_pickup_byte());
		CU_ASSERT(0xFF == gpm_pickup_byte());
		CU_ASSERT(0xFF == gpm_pickup_byte());
		CU_ASSERT(-1 == gpm_pickup_byte());

		CU_ASSERT(0xFFFF == gpm_get_register_map_val(addr));

		gpm_dummy_trigger_output_triggered = 0;
	}

	/* test invalid addresses */
	for(addr = 32; addr!=0; addr++){
		CU_ASSERT(1 == gpm_send_set(addr, 0x0000));

		CU_ASSERT(0 == gpm_dummy_trigger_output_triggered);
		CU_ASSERT(-1 == gpm_pickup_byte());

		gpm_dummy_trigger_output_triggered = 0;
	}

}

void test_gprotm_send_get(void)
{
	u8 addr;

	/* test all valid addresses */
	for(addr = 0; addr<32; addr++){
		CU_ASSERT(0 == gpm_send_get(addr));

		CU_ASSERT(1 == gpm_dummy_trigger_output_triggered);
		CU_ASSERT((addr | GP_MODE_READ | GP_MODE_PEEK) == gpm_pickup_byte());
		CU_ASSERT(-1 == gpm_pickup_byte());

		gpm_dummy_trigger_output_triggered = 0;
	}

	/* test invalid addresses */
	for(addr = 32; addr!=0; addr++){
		CU_ASSERT(1 == gpm_send_get(addr));

		CU_ASSERT(0 == gpm_dummy_trigger_output_triggered);
		CU_ASSERT(-1 == gpm_pickup_byte());

		gpm_dummy_trigger_output_triggered = 0;
	}
}

void test_gprotm_send_get_cont(void)
{
	u8 addr;

	/* test all valid addresses */
	for(addr = 0; addr<32; addr++){
		CU_ASSERT(0 == gpm_send_get_cont(addr));

		CU_ASSERT(1 == gpm_dummy_trigger_output_triggered);
		CU_ASSERT((addr | GP_MODE_READ | GP_MODE_CONT) == gpm_pickup_byte());
		CU_ASSERT(-1 == gpm_pickup_byte());

		gpm_dummy_trigger_output_triggered = 0;
	}

	/* test invalid addresses */
	for(addr = 32; addr!=0; addr++){
		CU_ASSERT(1 == gpm_send_get_cont(addr));

		CU_ASSERT(0 == gpm_dummy_trigger_output_triggered);
		CU_ASSERT(-1 == gpm_pickup_byte());

		gpm_dummy_trigger_output_triggered = 0;
	}
}

void test_gprotm_handle_byte(void)
{
	u8 addr = 0;
	u16 data = 0;

	/* check all valid addresses with data zero */
	for(addr=0; addr<32; addr++){
		CU_ASSERT(0 == gpm_handle_byte(addr));
		CU_ASSERT(0 == gpm_dummy_register_changed);
		CU_ASSERT(0 == gpm_handle_byte(data & 0xFF));
		CU_ASSERT(0 == gpm_dummy_register_changed);
		CU_ASSERT(0 == gpm_handle_byte(data >> 8));
		CU_ASSERT(1 == gpm_dummy_register_changed);
		CU_ASSERT(addr == gpm_dummy_register_changed_addr);
		CU_ASSERT(data == gpm_get_register_map_val(gpm_dummy_register_changed_addr));
		gpm_dummy_register_changed = 0;
		gpm_dummy_register_changed_addr = 0;
	}

	/* check all valid addresses with data 0xAA55 */
	data = 0xAA55;
	for(addr=0; addr<32; addr++){
		CU_ASSERT(0 == gpm_handle_byte(addr));
		CU_ASSERT(0 == gpm_dummy_register_changed);
		CU_ASSERT(0 == gpm_handle_byte(data & 0xFF));
		CU_ASSERT(0 == gpm_dummy_register_changed);
		CU_ASSERT(0 == gpm_handle_byte(data >> 8));
		CU_ASSERT(1 == gpm_dummy_register_changed);
		CU_ASSERT(addr == gpm_dummy_register_changed_addr);
		CU_ASSERT(data == gpm_get_register_map_val(gpm_dummy_register_changed_addr));
		gpm_dummy_register_changed = 0;
		gpm_dummy_register_changed_addr = 0;
	}

	/* check all valid addresses with data 0xFFFF */
	data = 0xFFFF;
	for(addr=0; addr<32; addr++){
		CU_ASSERT(0 == gpm_handle_byte(addr));
		CU_ASSERT(0 == gpm_dummy_register_changed);
		CU_ASSERT(0 == gpm_handle_byte(data & 0xFF));
		CU_ASSERT(0 == gpm_dummy_register_changed);
		CU_ASSERT(0 == gpm_handle_byte(data >> 8));
		CU_ASSERT(1 == gpm_dummy_register_changed);
		CU_ASSERT(addr == gpm_dummy_register_changed_addr);
		CU_ASSERT(data == gpm_get_register_map_val(gpm_dummy_register_changed_addr));
		gpm_dummy_register_changed = 0;
		gpm_dummy_register_changed_addr = 0;
	}

	/* check all invalid addresses */
	for(addr=32; addr!=0; addr++){
		CU_ASSERT(1 == gpm_handle_byte(addr));
		CU_ASSERT(0 == gpm_dummy_register_changed);
		CU_ASSERT(0 == gpm_dummy_register_changed_addr);
		gpm_dummy_register_changed = 0;
		gpm_dummy_register_changed_addr = 0;
	}
}
