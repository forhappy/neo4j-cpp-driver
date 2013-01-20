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

bool Util::safe_strtoull(const char *str, uint64_t * out)
{
    assert(out != NULL);
    errno = 0;
    *out = 0;
    char *endptr;
    unsigned long long ull = strtoull(str, &endptr, 10);

    if ((errno == ERANGE) || (str == endptr)) {
        return false;
    }

    if (isspace(*endptr) || (*endptr == '\0' && endptr != str)) {
        if ((long long) ull < 0) {
            /* only check for negative signs in the uncommon case when
             * the unsigned number is so big that it's negative as a
             * signed number. 
             */
            if (strchr(str, '-') != NULL) {
                return false;
            }
        }
        *out = ull;
        return true;
    }
    return false;
}

bool Util::safe_strtoll(const char *str, int64_t * out)
{
    assert(out != NULL);
    errno = 0;
    *out = 0;
    char *endptr;
    long long ll = strtoll(str, &endptr, 10);

    if ((errno == ERANGE) || (str == endptr)) {
        return false;
    }

    if (isspace(*endptr) || (*endptr == '\0' && endptr != str)) {
        *out = ll;
        return true;
    }
    return false;
}

bool Util::safe_strntoll(const char *str, size_t len, int64_t * out)
{
    assert(out != NULL);
    char buf[64] = {0}; /* should be enough. */
    char *pstr = buf;
    errno = 0;
    *out = 0;
    char *endptr;
    snprintf(buf, len + 1, "%s", str);
    long long ll = strtoll(pstr, &endptr, 10);

    if ((errno == ERANGE) || (pstr == endptr)) {
        return false;
    }

    if (isspace(*endptr) || (*endptr == '\0' && endptr != pstr)) {
        *out = ll;
        return true;
    }
    return false;
}

bool Util::safe_strtoul(const char *str, uint32_t * out)
{
    char *endptr = NULL;
    unsigned long l = 0;

    assert(out);
    assert(str);
    *out = 0;
    errno = 0;

    l = strtoul(str, &endptr, 10);
    if ((errno == ERANGE) || (str == endptr)) {
        return false;
    }

    if (isspace(*endptr) || (*endptr == '\0' && endptr != str)) {
        if ((long) l < 0) {
            /* only check for negative signs in the uncommon case when
             * the unsigned number is so big that it's negative as a
             * signed number. 
             */
            if (strchr(str, '-') != NULL) {
                return false;
            }
        }
        *out = l;
        return true;
    }

    return false;
}

bool Util::safe_strtol(const char *str, int32_t * out)
{
    assert(out != NULL);
    errno = 0;
    *out = 0;
    char *endptr;
    long l = strtol(str, &endptr, 10);

    if ((errno == ERANGE) || (str == endptr)) {
        return false;
    }

    if (isspace(*endptr) || (*endptr == '\0' && endptr != str)) {
        *out = l;
        return true;
    }
    return false;
}

};

}

#endif // _NEO4JCPP_UTIL_H_

