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
#include "lg/gprotc.h"

#include "check_gprotc_suite.h"

u16 gpc_dummy_register_map[32];

int gpc_dummy_trigger_output_triggered = 0;
void gpc_dummy_trigger_output_hook(void);

int init_gprotc_suite(void);
int clean_gprotc_suite(void);
void test_gprotc_send_reg(void);
void test_gprotc_handle_byte_read(void);
void test_gprotc_handle_byte_write(void);

int check_gprotc_suite_register()
{
	CU_pSuite pSuite = NULL;

	/* add suite to the registry */
	pSuite = CU_add_suite("governor protocol for client suite", init_gprotc_suite, clean_gprotc_suite);
	if(NULL == pSuite){
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if((NULL == CU_add_test(pSuite, "gprotc send_reg", test_gprotc_send_reg)) ||
	   (NULL == CU_add_test(pSuite, "gprotc handle_byte_read", test_gprotc_handle_byte_read)) ||
	   (NULL == CU_add_test(pSuite, "gprotc handle_byte_write", test_gprotc_handle_byte_write))){
		CU_cleanup_registry();
		return CU_get_error();
	}

	return 0;
}

void gpc_dummy_trigger_output_hook(void)
{
	gpc_dummy_trigger_output_triggered = 1;
}

int init_gprotc_suite(void)
{
	int i;

	for(i=0; i<32; i++)
		gpc_dummy_register_map[i] = 0xAA55+i;

	gpc_init(gpc_dummy_trigger_output_hook);
	return 0;
}

int clean_gprotc_suite(void)
{
	return 0;
}

void test_gprotc_send_reg(void)
{
	int i;
	u8 addr;

	addr = 0;
	for(i = 0; i<256; i++){
		CU_ASSERT(1 == gpc_send_reg(addr));
		CU_ASSERT(-1 == gpc_pickup_byte());
		addr++;
	}

	CU_ASSERT(0 == gpc_setup_reg(0, &gpc_dummy_register_map[0]));
	CU_ASSERT(0 == gpc_send_reg(0));
	CU_ASSERT(0 == gpc_pickup_byte());
	CU_ASSERT(0x55 == gpc_pickup_byte());
	CU_ASSERT(0xAA == gpc_pickup_byte());
	CU_ASSERT(-1 == gpc_pickup_byte());
	CU_ASSERT(1 == gpc_send_reg(1));


	for(addr=1; addr<32; addr++){
		CU_ASSERT(0 == gpc_setup_reg(addr, &gpc_dummy_register_map[addr]));
		CU_ASSERT(0 == gpc_send_reg(addr));
		CU_ASSERT(addr == gpc_pickup_byte());
		CU_ASSERT(0x55+addr == gpc_pickup_byte());
		CU_ASSERT(0xAA == gpc_pickup_byte());
		CU_ASSERT(-1 == gpc_pickup_byte());
	}


	for(addr=32; addr!=0; addr++){
		CU_ASSERT(1 == gpc_send_reg(addr));
		CU_ASSERT(-1 == gpc_pickup_byte());
	}
}

void test_gprotc_handle_byte_read(void)
{
	u8 addr = 0;

	for(addr=0; addr<32; addr++){
		CU_ASSERT(0 == gpc_handle_byte(addr | GP_MODE_READ | GP_MODE_PEEK));
		CU_ASSERT(addr == gpc_pickup_byte());
		CU_ASSERT(0x55+addr == gpc_pickup_byte());
		CU_ASSERT(0xAA == gpc_pickup_byte());
		CU_ASSERT(-1 == gpc_pickup_byte());
	}

	for(addr=0; addr<32; addr++){
		CU_ASSERT(1 == gpc_handle_byte(addr | GP_MODE_READ | GP_MODE_PEEK | GP_MODE_RESERVED));
		CU_ASSERT(-1 == gpc_pickup_byte());
	}
}

void test_gprotc_handle_byte_write(void)
{
	u8 addr = 0;
	u16 data = 0xDADE;

	for(addr=0; addr<32; addr++){
		CU_ASSERT(0 == gpc_handle_byte(addr | GP_MODE_WRITE));
		CU_ASSERT(0 == gpc_handle_byte(data & 0xFF));
		CU_ASSERT(0 == gpc_handle_byte(data >> 8));
		CU_ASSERT(data == gpc_dummy_register_map[addr]);
	}

	for(addr=0; addr<32; addr++){
		CU_ASSERT(1 == gpc_handle_byte(addr | GP_MODE_WRITE | GP_MODE_RESERVED));
		CU_ASSERT(-1 == gpc_pickup_byte());
	}
}
