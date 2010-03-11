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
#include <stdio.h>

#include "lg/types.h"
#include "lg/gpdef.h"
#include "lg/gprotm.h"
#include "lg/gprotc.h"

#include "check_gprot_suite.h"

u16 gp_register_map[32];

int gpm_register_changed = 0;
int gpm_register_changed_addr = 0;
void gpm_trigger_output_hook(void);
void gpm_register_changed_hook(u8 addr);
void gpc_trigger_output_hook(void);

int init_gprot_suite(void);
int clean_gprot_suite(void);
void test_gprot_write(void);
void test_gprot_read(void);
void test_gprot_read_write(void);

int check_gprot_suite_register()
{
	CU_pSuite pSuite = NULL;

	/* add suite to the registry */
	pSuite = CU_add_suite("governor protocol suite", init_gprot_suite, clean_gprot_suite);
	if(NULL == pSuite){
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if((NULL == CU_add_test(pSuite, "gprot write", test_gprot_write)) ||
	   (NULL == CU_add_test(pSuite, "gprot read", test_gprot_read)) ||
	   (NULL == CU_add_test(pSuite, "gprot read write", test_gprot_read_write))){
		CU_cleanup_registry();
		return CU_get_error();
	}

	return 0;
}

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

int init_gprot_suite(void)
{
	int i;

	gpm_init(gpm_trigger_output_hook, gpm_register_changed_hook);

	gpc_init(gpc_trigger_output_hook);

	for(i=0; i<32; i++){
		gp_register_map[i] = 0xAA55+i;
		gpc_setup_reg(i, &gp_register_map[i]);
	}
	return 0;
}

int clean_gprot_suite(void)
{
	return 0;
}

void test_gprot_write(void)
{
	u8 addr = 0;

	for(addr=0; addr<32; addr++){
		CU_ASSERT(0 == gpm_send_set(addr, 0x0000));
		CU_ASSERT(0x0000 == gp_register_map[addr]);
		CU_ASSERT(0x0000 == gpm_get_register_map_val(addr));
	}

	for(addr=0; addr<32; addr++){
		CU_ASSERT(0 == gpm_send_set(addr, 0xAA55));
		CU_ASSERT(0xAA55 == gp_register_map[addr]);
		CU_ASSERT(0xAA55 == gpm_get_register_map_val(addr));
	}

	for(addr=0; addr<32; addr++){
		CU_ASSERT(0 == gpm_send_set(addr, 0xFFFF));
		CU_ASSERT(0xFFFF == gp_register_map[addr]);
		CU_ASSERT(0xFFFF == gpm_get_register_map_val(addr));
	}
}

void test_gprot_read(void)
{
	u8 addr;

	for(addr=0; addr<32; addr++){
		gp_register_map[addr] = 0x0000;
	}

	for(addr=0; addr<32; addr++){
		CU_ASSERT(0 == gpm_send_get(addr));
		CU_ASSERT(0x0000 == gpm_get_register_map_val(addr));
	}

	for(addr=0; addr<32; addr++){
		gp_register_map[addr] = 0xAA55;
	}

	for(addr=0; addr<32; addr++){
		CU_ASSERT(0 == gpm_send_get(addr));
		CU_ASSERT(0xAA55 == gpm_get_register_map_val(addr));
	}

	for(addr=0; addr<32; addr++){
		gp_register_map[addr] = 0xFFFF;
	}

	for(addr=0; addr<32; addr++){
		CU_ASSERT(0 == gpm_send_get(addr));
		CU_ASSERT(0xFFFF == gpm_get_register_map_val(addr));
	}
}

void test_gprot_read_write(void)
{
	u8 addr;

	for(addr=0; addr<32; addr++){
		CU_ASSERT(0 == gpm_send_set(addr, 0x0000+addr));
		CU_ASSERT(0 == gpm_send_get(addr));
		CU_ASSERT(0x0000+addr == gpm_get_register_map_val(addr));
	}

}
