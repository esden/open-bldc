/*
 * yamlgen - YAML to everything generator framework
 * Copyright (C) 2010 by Tobias Fuchs <twh.fuchs@gmail.com>
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

#ifndef LOGGING_H__
#define LOGGING_H__

#ifdef LOG

#define DEBUG 1
#define INFO 2

#if LOG == DEBUG
#define LOG_DEBUG_EVT(x) fprintf(stderr, "\nD- Parser event: %s\n", x)
#define LOG_DEBUG_PRINT(s, args...) fprintf(stderr, "D- " s "\n", ##args)
#else
#define LOG_DEBUG_EVT(x) ((void)0)
#define LOG_DEBUG_PRINT(s, args...) ((void)0)
#endif

#if LOG == DEBUG || LOG == INFO
#define LOG_INFO_PRINT(s, args...) fprintf(stderr, "I- " s "\n", ##args)
#else 
#define LOG_INFO_PRINT(s, args...) ((void)0)
#endif

#else // ifdef LOG

#define LOG_DEBUG_EVT(c) ((void)0)
#define LOG_DEBUG_PRINT(s, args...) ((void)0)
#define LOG_INFO_PRINT(s, args...) ((void)0)

#endif // ifdef LOG

#endif // ifndef LOGGING_H__
