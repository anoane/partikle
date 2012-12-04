/*
 * $FILE: limits.h
 *
 * Implementation-defined constant
 *
 * $VERSION: 1.0
 *
 * Author: Miguel Masmano <mimastel@doctor.upv.es>
 *
  * $LICENSE:
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef _ULIBC_LIMITS_H_
#define _ULIBC_LIMITS_H_

#ifndef __SCHAR_MAX__
#define __SCHAR_MAX__   127
#endif
#ifndef __SHRT_MAX__
#define __SHRT_MAX__    32767
#endif
#ifndef __USHRT_MAX__
#define __USHRT_MAX__    65535
#endif
#ifndef __INT_MAX__
#define __INT_MAX__     2147483647
#endif

#ifndef __LONG_MAX__
#if __WORDSIZE == 64
#define __LONG_MAX__    9223372036854775807L
#else
#define __LONG_MAX__    2147483647L
#endif
#endif

#define USHRT_MAX (__USHRT_MAX__)
#define SHRT_MAX (__SHRT_MAX__)

#define INT_MIN (-1 - INT_MAX)
#define INT_MAX (__INT_MAX__)
#define UINT_MAX (INT_MAX * 2U + 1U)

#define LONG_MIN (-1L - LONG_MAX)
#define LONG_MAX ((__LONG_MAX__) + 0L)
#define ULONG_MAX (LONG_MAX * 2UL + 1UL)

#define LLONG_MAX 9223372036854775807LL
#define LLONG_MIN (-LLONG_MAX - 1LL)

/* Maximum value an `unsigned long long int' can hold.  (Minimum is 0.)  */
#define ULLONG_MAX 18446744073709551615ULL

#endif
