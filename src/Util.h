/* Util class.
 *
 * Copyright (c) 2012-2013 Fu Haiping <haipingf@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#ifndef _NEO4JCPP_UTIL_H_
#define _NEO4JCPP_UTIL_H_

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

namespace neo4jcpp {

class Util {
public:
    static bool safe_strtoull(const char *str, uint64_t * out);
    static bool safe_strtoll(const char *str, int64_t * out);
    static bool safe_strntoll(const char *str, size_t len, int64_t * out);
    static bool safe_strtoul(const char *str, uint32_t * out);
    static bool safe_strtol(const char *str, int32_t * out);
};

}

#endif // _NEO4JCPP_UTIL_H_

