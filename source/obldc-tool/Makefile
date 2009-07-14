#
# Open-BLDC Tool - Open BruschLess DC Motor Controller PC Tool
# Copyright (C) 2009 by Piotr Esden-Tempski <piotr@esden.net>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

CC	= gcc
CFLAGS	+= -Wall -ansi -std=c99 -I.
LDFLAGS	+= -lftd2xx -lncurses

# TODO: Port to libftdi, ftd2xx is closed-source.

OBJECTS = main.o \
	  serial.o \
	  tools.o

all: obldc-tool

obldc-tool: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o obldc-tool

clean:
	rm -f $(OBJECTS) obldc-tool
