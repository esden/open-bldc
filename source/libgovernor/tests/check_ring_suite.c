/*
 * governorlib - Open-BLDC configuration and debug protocol library
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

#include "lg/types.h"
#include "lg/ring.h"

#include "check_ring_suite.h"

struct ring test_ring;
u8 data[10];

int init_ring_suite(void);
int clean_ring_suite(void);
void test_ring_create(void);
void test_ring_write_one();
void test_ring_read_one();
void test_ring_write_max();
void test_ring_read_max();

int check_ring_suite_register()
{
	CU_pSuite pSuite = NULL;

	/* add suite to the registry */
	pSuite = CU_add_suite("Ring suite", init_ring_suite, clean_ring_suite);
	if(NULL == pSuite){
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the tests to the suite */
	if( (NULL == CU_add_test(pSuite, "test of ring create", test_ring_create)) ||
	    (NULL == CU_add_test(pSuite, "test of write one", test_ring_write_one)) ||
	    (NULL == CU_add_test(pSuite, "test of read one", test_ring_read_one)) ||
	    (NULL == CU_add_test(pSuite, "test of write max", test_ring_write_max)) ||
	    (NULL == CU_add_test(pSuite, "test of read max", test_ring_read_max))){
		CU_cleanup_registry();
		return CU_get_error();
	}

	return 0;
}

int init_ring_suite(void)
{
	ring_init(&test_ring, data, 10);

	return 0;
}

int clean_ring_suite(void)
{
	return 0;
}


void test_ring_create(void)
{
	CU_ASSERT(10 == RING_SIZE(&test_ring));
	CU_ASSERT(data == RING_DATA(&test_ring));
}

void test_ring_write_one()
{
	s32 ret;

	ret = (u8)ring_write_ch(&test_ring, (u8)'A');

	CU_ASSERT('A' == ret);
}

void test_ring_read_one()
{
	s32 ret;
	u8 ch;

	ret = (u8)ring_read_ch(&test_ring, &ch);

	CU_ASSERT('A' == ret);
	CU_ASSERT('A' == ch);
}

void test_ring_write_max()
{
	s32 ret;
	u8 i;

	for(i=0; i<9; i++){
		ret = (u8)ring_write_ch(&test_ring, i);

		CU_ASSERT(i == ret);
	}

	CU_ASSERT(-1 == ring_write_ch(&test_ring, 10));
}

void test_ring_read_max()
{
	s32 ret;
	u8 i, ch;

	for(i=0; i<9; i++){
		ret = (u8)ring_read_ch(&test_ring, &ch);

		CU_ASSERT(i == ret);
		CU_ASSERT(i == ch);
	}

	ch = 10;
	CU_ASSERT(-1 == ring_read_ch(&test_ring, &ch));
	CU_ASSERT(10 == ch);
}
