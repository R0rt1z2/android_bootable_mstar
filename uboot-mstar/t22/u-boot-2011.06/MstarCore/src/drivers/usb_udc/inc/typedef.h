/* SPDX-License-Identifier: GPL-2.0-only OR BSD-3-Clause */
/******************************************************************************
 *
 * This file is provided under a dual license.  When you use or
 * distribute this software, you may choose to be licensed under
 * version 2 of the GNU General Public License ("GPLv2 License")
 * or BSD License.
 *
 * GPLv2 License
 *
 * Copyright(C) 2019 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 *
 * BSD LICENSE
 *
 * Copyright(C) 2019 MediaTek Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#include <hal_usb_platform.h>

#define true		1
#define false		0

#define TRUE		true
#define FALSE		false

#ifndef NULL
#define NULL		0
#endif

#define DISABLE		0
#define ENABLE		1

#define FAIL		0
#define PASS		1

#define NO		0
#define YES		1

typedef char	C8;

#if !defined(__BOOL_defined)
#define __BOOL_defined
typedef unsigned char BOOL;
typedef unsigned char bool;
#endif

#define ABS(X)    ((X>=0) ? (X) : (-X))
#define COUNTOF( array )    (sizeof(array) / sizeof((array)[0]))

#if defined( INCLUDE_LINUX_TYPE )
/* say some thing XD */
#else
typedef unsigned char		u_int8_t;
typedef unsigned short int	u_int16_t;
typedef unsigned long int	u_int32_t;
typedef unsigned long int	u_int;
typedef unsigned long long int	u_int64_t;
typedef char c8;
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
#endif

typedef u_int8_t		byte;	/* 8-bit byte (octet) */
typedef u_int16_t		u16b;	/* 16-bit unsigned word */
typedef u_int32_t		u32b;	/* 32-bit unsigned word */

#define __packed

#ifndef BIT0
#define BIT0		0x00000001
#endif
#ifndef BIT1
#define BIT1		0x00000002
#endif
#ifndef BIT2
#define BIT2		0x00000004
#endif
#ifndef BIT3
#define BIT3		0x00000008
#endif
#ifndef BIT4
#define BIT4		0x00000010
#endif
#ifndef BIT5
#define BIT5		0x00000020
#endif
#ifndef BIT6
#define BIT6		0x00000040
#endif
#ifndef BIT7
#define BIT7		0x00000080
#endif
#ifndef BIT8
#define BIT8		0x00000100
#endif
#ifndef BIT9
#define BIT9		0x00000200
#endif
#ifndef BIT10
#define BIT10		0x00000400
#endif
#ifndef BIT11
#define BIT11		0x00000800
#endif
#ifndef BIT12
#define BIT12		0x00001000
#endif
#ifndef BIT13
#define BIT13		0x00002000
#endif
#ifndef BIT14
#define BIT14		0x00004000
#endif
#ifndef BIT15
#define BIT15		0x00008000
#endif
#ifndef BIT16
#define BIT16		0x00010000
#endif
#ifndef BIT17
#define BIT17		0x00020000
#endif
#ifndef BIT18
#define BIT18		0x00040000
#endif
#ifndef BIT19
#define BIT19		0x00080000
#endif
#ifndef BIT20
#define BIT20		0x00100000
#endif
#ifndef BIT21
#define BIT21		0x00200000
#endif
#ifndef BIT22
#define BIT22		0x00400000
#endif
#ifndef BIT23
#define BIT23		0x00800000
#endif
#ifndef BIT24
#define BIT24		0x01000000
#endif
#ifndef BIT25
#define BIT25		0x02000000
#endif
#ifndef BIT26
#define BIT26		0x04000000
#endif
#ifndef BIT27
#define BIT27		0x08000000
#endif
#ifndef BIT28
#define BIT28		0x10000000
#endif
#ifndef BIT29
#define BIT29		0x20000000
#endif
#ifndef BIT30
#define BIT30		0x40000000
#endif
#ifndef BIT31
#define BIT31		0x80000000
#endif

#define U8	u8
#define U16	u16
#define U32	u32

#define S32	s32
#define S16	s16
#define S8	s8
#endif
