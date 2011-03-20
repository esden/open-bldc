/*
 * libgovernor - Open-BLDC configuration and debug protocol library
 * Copyright (C) 2011 by Piotr Esden-Tempski <piotr@esden.net>
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

#include <stdio.h>
#include <regex.h>

#include "check_utils.h"

/**
 * Report if a string matches a supplied regular expression.
 *
 * @ret 0 for success 1 for failure.
 */
int regmatch(char *regex, char *string)
{
	regex_t r;
	int ret;

	if ((ret = regcomp(&r, regex, REG_EXTENDED | REG_NOSUB | REG_NEWLINE)) != 0 ) {
		char error[1024];

		regerror(ret, &r, error, 1024);

		printf("ERR: regex compilation error: %s\n", error);
		printf("ERR: regex used: '%s'\n", regex);

		return 1;
	}

	if ((ret = regexec(&r, string, 0, NULL, 0)) != 0) {
		char error[1024];

		regerror(ret, &r, error, 1024);

		printf("ERR: regex match error: %s\n", error);
		printf("ERR: regex used: '%s'\n", regex);
		printf("ERR: string used: '%s'\n", string);

		return 1;
	}

	regfree(&r);

	return 0;
}
