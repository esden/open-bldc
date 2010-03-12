# libgovernor - Open-BLDC configuration and debug protocol library
# Copyright (C) 2010 by Piotr Esden-Tempski <piotr@esden.net>
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

INCLUDES = @EXTRACFLAGS@ -I$(top_srcdir)/inc -I.. -I.

AM_CFLAGS = $(MORE_WARNINGS_CFLAGS)

lib_LTLIBRARIES=libgovernor.la

libgovernor_la_SOURCES= ring.c \
			gprotm.c \
			gprotc.c
