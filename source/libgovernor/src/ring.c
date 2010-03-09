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

#include "lg/types.h"

#include "lg/ring.h"

void ring_init(struct ring *ring, u8 *buf, ring_size_t size){
	ring->data = buf;
	ring->size = size;
	ring->begin = 0;
	ring->end = 0;
}

s32 ring_write_ch(struct ring *ring, u8 ch){
	if(((ring->end + 1) % ring->size) != ring->begin){
		ring->data[ring->end++] = ch;
		ring->end %= ring->size;
		return (u32)ch;
	}

	return -1;
}

s32 ring_write(struct ring *ring, u8 *data, ring_size_t size){
	s32 i;

	for(i = 0; i < size; i++){
		if(ring_write_ch(ring, data[i]) < 0){
			return -i;
		}
	}

	return i;
}

s32 ring_read_ch(struct ring *ring, u8 *ch){
	s32 ret = -1;

	if(ring->begin != ring->end){
		ret = ring->data[ring->begin++];
		ring->begin %= ring->size;
		if(ch) *ch = ret;
	}

	return ret;
}

s32 ring_read(struct ring *ring, u8 *data, ring_size_t size){
	s32 i;

	for(i = 0; i < size; i++){
		if(ring_read_ch(ring, data+i) < 0){
			return i;
		}
	}

	return -i;
}
