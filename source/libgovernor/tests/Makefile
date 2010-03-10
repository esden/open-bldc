# governorlib - Open-BLDC configuration and debug protocol library
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

NAME=check
OBJ = check_main.o \
      check_ring_suite.o

CFLAGS += -I../inc -g -Wall -Wextra -Werror -Wshadow

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) -L../src -lgovernor -lcunit -lncurses -o $@ $(OBJ)

clean:
	rm -rf $(NAME)
	rm -rf $(OBJ)