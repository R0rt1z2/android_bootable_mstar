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

#ifndef _FUNC_H_
#define _FUNC_H_

typedef int (*FP_MP_MOD_EXP)(DIGIT_T yout[], const DIGIT_T x[], const DIGIT_T e[], const DIGIT_T m[], int ndigits);

typedef int (*FP_MP_CONV_FROM_OCTETS)(DIGIT_T a[], int ndigits, const unsigned char *c, int nbytes);

typedef int (*FP_MP_EQUAL)(const DIGIT_T a[], const DIGIT_T b[], int ndigits);

typedef void (*FP_MP_SETZERO)(const DIGIT_T a[], int ndigits);

typedef void (*FP_NULL)(void);

//int mpSquare(DIGIT_T w[], const DIGIT_T x[], int ndigits)
typedef int (*FP_MP_SQUARE)(DIGIT_T w[], const DIGIT_T x[], int ndigits);

//int mpMultiply(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[], int ndigits)
typedef int (*FP_MP_MULTIPLY)(DIGIT_T w[], const DIGIT_T u[], const DIGIT_T v[], int ndigits);

//int mpDivide(DIGIT_T q[], DIGIT_T r[], const DIGIT_T u[], int udigits, DIGIT_T v[], int vdigits)
typedef int (*FP_MP_DIVIDE)(DIGIT_T q[], DIGIT_T r[], const DIGIT_T u[], int udigits, DIGIT_T v[], int vdigits);

//DIGIT_T mpShortDiv(DIGIT_T q[], const DIGIT_T u[], DIGIT_T v, int ndigits)
typedef DIGIT_T (*FP_MP_SHORT_DIV)(DIGIT_T q[], const DIGIT_T u[], DIGIT_T v, int ndigits);

//DIGIT_T mpShiftLeft(DIGIT_T a[], const DIGIT_T *b, int shift, int ndigits)
typedef DIGIT_T (*FP_MP_SHIFT_LEFT)(DIGIT_T a[], const DIGIT_T *b, int shift, int ndigits);

//DIGIT_T mpShiftRight(DIGIT_T a[], const DIGIT_T b[], int shift, int ndigits)
typedef DIGIT_T (*FP_MP_SHIFT_RIGHT)(DIGIT_T a[], const DIGIT_T b[], int shift, int ndigits);

//int spMultiply(DIGIT_T p[2], DIGIT_T x, DIGIT_T y)
typedef int (*FP_SP_MULTIPLY)(DIGIT_T p[2], DIGIT_T x, DIGIT_T y);

//DIGIT_T spDivide(DIGIT_T *q, DIGIT_T *r, const DIGIT_T u[2], DIGIT_T v)
typedef DIGIT_T (*FP_SP_DIVIDE)(DIGIT_T *q, DIGIT_T *r, const DIGIT_T u[2], DIGIT_T v);

typedef struct
{
    FP_MP_CONV_FROM_OCTETS ConvFromOctets;
    FP_MP_MOD_EXP       ModExp;
    FP_MP_EQUAL         Equal;

    FP_MP_SQUARE        Square;
    FP_MP_MULTIPLY      Multiply;
    FP_MP_DIVIDE        Divide;
    FP_MP_SHORT_DIV     ShortDiv;
    FP_MP_SHIFT_LEFT    ShiftLeft;
    FP_MP_SHIFT_RIGHT   ShiftRight;

    FP_SP_MULTIPLY      SpMultiply;
    FP_SP_DIVIDE        SpDivide;

    FP_NULL             reserve_1[13];
} MpFuncTbl;

extern MpFuncTbl        *mpFunc;
extern MpFuncTbl        _mpFunc;

#endif

