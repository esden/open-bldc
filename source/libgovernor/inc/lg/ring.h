/*
 * governor - Open-BLDC configuration and debug protocol library
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

#ifndef RING_H
#define RING_H

typedef s32 ring_size_t;

struct ring {
	u8 *data;
	ring_size_t size;
	u32 begin;
	u32 end;
};

#define RING_SIZE(RING) ((RING)->size - 1)
#define RING_DATA(RING) (RING)->data

void ring_init(struct ring *ring, u8 *buf, ring_size_t size);
s32 ring_write_ch(struct ring *ring, u8 ch);
s32 ring_write(struct ring *ring, u8 *data, ring_size_t size);
s32 ring_read_ch(struct ring *ring, u8 *ch);
s32 ring_read(struct ring *ring, u8 *data, ring_size_t size);

#endif /* RING_H */
