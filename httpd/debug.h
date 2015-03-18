/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: Debug.h
 * modify date: 2008-12-3
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */
#ifndef _Debug_H
#define _Debug_H

#define Debug 1

#include <stdio.h>
#include <stdarg.h>
static inline void debug_print(const char * format, ...)
				__attribute__ ((format(printf, 1, 2)));
#ifdef Debug

#define debug_where() \
	printf("\n###FILE: %s ###LINE: %d ###FUC: %s\n", \
					__FILE__, __LINE__, __FUNCTION__)
static inline void debug_print(const char * format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}
#else

#define debug_where() do { } while (0)

static inline void debug_print(const char * format, ...)
{
}
#endif /*end debug*/
#endif /*end _Debug_H*/

