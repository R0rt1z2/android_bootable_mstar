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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  PKA Driver Interface
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVPKA_H_
#define _DRVPKA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"

#define PKA_MAX_BIGVALUE_ARRAY_SIZE     (130)
#define PKA_MAX_POINTCURVE_ARRAY_SIZE   (24)

/** PKA related functions */
typedef struct DLL_PACKED
{
    MS_U32 au32Value[PKA_MAX_BIGVALUE_ARRAY_SIZE]; // little endian
    MS_U32 u32Size;        // number of elements used in value
}DrvPKA_PKAValue;

typedef struct DLL_PACKED
{
    MS_U32 au32X[PKA_MAX_POINTCURVE_ARRAY_SIZE];      // little endian
    MS_U32 au32Y[PKA_MAX_POINTCURVE_ARRAY_SIZE];      // little endian
    MS_U32 u32Size;        // number of elements used in x/y, must not exceed prime modulus p's size
}DrvPKA_ECCPoint;

/* an elliptic curve is defined by p,a,b */
typedef struct DLL_PACKED
{
    MS_U32 au32P[PKA_MAX_POINTCURVE_ARRAY_SIZE];            // little endian, prime modulus p which > 2^63, the highest word indicated by u32Size may not be zero
    MS_U32 au32A[PKA_MAX_POINTCURVE_ARRAY_SIZE];            // little endian, a < p
    MS_U32 au32B[PKA_MAX_POINTCURVE_ARRAY_SIZE];            // little endian, b < p
    MS_U32 au32Order[PKA_MAX_POINTCURVE_ARRAY_SIZE];        // order
    MS_U32 au32Gx[PKA_MAX_POINTCURVE_ARRAY_SIZE];           // base point x
    MS_U32 au32Gy[PKA_MAX_POINTCURVE_ARRAY_SIZE];           // base point y
    MS_U32 u32Size;              // number of elements used in p
}DrvPKA_ECCCurve;

typedef struct DLL_PACKED
{
    MS_U32 au32R[PKA_MAX_BIGVALUE_ARRAY_SIZE];     // little endian
    MS_U32 au32S[PKA_MAX_BIGVALUE_ARRAY_SIZE];     // little endian
    MS_U32 u32Size;        // number of elements used in R/S
}DrvPKA_ECCSignature;

MS_BOOL MDrv_PKA_Init(void);

// Operation: Elliptic curve affine point addition in prime domain
// P1_xy + P2_xy -> P0_xy, prime curve: y^2 = x^3 + ax + b (mod p)
// Neither P1_xy nor P2_xy can be the "point-at-infinity"
// P1_xy and P2_xy must be on the curve
MS_BOOL MDrv_PKA_ECCPointAdd(DrvPKA_ECCCurve* pstCurve, DrvPKA_ECCPoint* pstPoint1, DrvPKA_ECCPoint* pstPoint2, DrvPKA_ECCPoint* pstResult);

// Operation: Elliptic curve affine point multiplication in prime domain
// k * P1_xy -> P0_xy, optional randomization factor Rz, prime curve: y^2 = x^3 + ax + b (mod p)
// 1 <= Rz < p
// 1 < k <= n (curve's order)
// P1_xy cannot be the "point-at-infinity"
// P1_xy must be on the curve
// p, k < 2^768
MS_BOOL MDrv_PKA_ECCPointMul(DrvPKA_ECCCurve* pstCurve, DrvPKA_ECCPoint* pstPoint, DrvPKA_PKAValue* pstScalar, DrvPKA_PKAValue* pstRandFact, DrvPKA_ECCPoint* pstResult);


// Operation: modular exponentiation
MS_BOOL MDrv_PKA_ModExp(DrvPKA_PKAValue* pstBase, DrvPKA_PKAValue* pstExponent, DrvPKA_PKAValue* pstModulus, MS_U32 pstShift, DrvPKA_PKAValue* pstResult);

// Operation: modular exponentiation with Chinese Remainders Theorem
MS_BOOL MDrv_PKA_ModExpCRT(DrvPKA_PKAValue* pstExpP, DrvPKA_PKAValue* pstExpQ, DrvPKA_PKAValue* pstModP, DrvPKA_PKAValue* pstModQ, DrvPKA_PKAValue* pstQInv, DrvPKA_PKAValue* pstInput, MS_U32 pstShift, DrvPKA_PKAValue* pstResult);

// Operation: modular inversion
// A: input, N: modulus, R: result
// 1 / A (mod N) -> R
// N must be odd
// N != 1
// The highest word of the modulus may not be zero
// input, N < 2^4160
MS_BOOL MDrv_PKA_ModInv(DrvPKA_PKAValue* pstInput, DrvPKA_PKAValue* pstModulus, DrvPKA_PKAValue* pstResult);

// Operation: Modulo
// A: dividend, B: modulus, R: result
// A mod B -> R
// size of A must larger than size of B
// The highest word of the modulus may not be zero
// A < 2^4160
MS_BOOL MDrv_PKA_Mod(DrvPKA_PKAValue* pstDividend, DrvPKA_PKAValue* pstModulus, DrvPKA_PKAValue* pstResult);

// Operation: Big number addition
// A + B - C -> D
// A, B: addend , C: subtrahend, D: result
// A, B, C < 2^4160, A + B must >= C
MS_BOOL MDrv_PKA_AddSub(DrvPKA_PKAValue* pstOpnd1, DrvPKA_PKAValue* pstOpnd2, DrvPKA_PKAValue* pstOpnd3, DrvPKA_PKAValue* pstResult);

// Operation: Big number addition
// A + B -> C
// A, B: addend , C: result
// A, B < 2^4160
MS_BOOL MDrv_PKA_Add(DrvPKA_PKAValue* pstOpnd1, DrvPKA_PKAValue* pstOpnd2, DrvPKA_PKAValue* pstResult);

// Operation: Big number subtraction
// A - B -> C
// A: minuend, B: subtrahend, C: result
// A, B < 2^4160
// A must >= B
MS_BOOL MDrv_PKA_Sub(DrvPKA_PKAValue* pstOpnd1, DrvPKA_PKAValue* pstOpnd2, DrvPKA_PKAValue* pstResult);

// Operation: Big number multiplication
// A * B -> C
// A: multiplicand, B: multiplier, C: result
// C < 2^4160
MS_BOOL MDrv_PKA_Mul(DrvPKA_PKAValue* pstOpnd1, DrvPKA_PKAValue* pstOpnd2, DrvPKA_PKAValue* pstResult);

// Operation: Big number multiplication
// A / B -> Q, R
// A: Dividend, B: Divisor, Q: Quotient, R: Remainder
// size of A must larger than size of B
// The highest word of the divisor may not be zero
// A < 2^4160
MS_BOOL MDrv_PKA_Div(DrvPKA_PKAValue* pstOpnd1, DrvPKA_PKAValue* pstOpnd2, DrvPKA_PKAValue* pstQuotient, DrvPKA_PKAValue* pstReminder);

// Operation: Elliptic Curve Digital Signature generation
// (curve, G, n, Rb, priKey, HASH(msg), Rz) -> (r, s)
// curve: defined by p, a, b
// G: base point
// n: integer order of G, means that n * G = O
MS_BOOL MDrv_PKA_ECDSASign(DrvPKA_ECCCurve* pstCurve, DrvPKA_PKAValue* pstBaseRandomizer, DrvPKA_PKAValue* pstPrivateKey, DrvPKA_PKAValue* pstMsgHash, DrvPKA_PKAValue* pstRandomFactor, DrvPKA_ECCSignature* pstSignature);

// Operation: Elliptic Curve Digital Signature verification
// (curve, G, n, Rb, pubKey, Rpub, HASH(msg), (r, s)) -> (r', s')
// curve: defined by p, a, b
// G: base point
// n: integer order of G, means that n * G = O
// Output: *bValid = TRUE if signature is valid, elase FALSE
MS_BOOL MDrv_PKA_ECDSAVerify(DrvPKA_ECCCurve* pstCurve, DrvPKA_PKAValue* pstBaseRandomizer, DrvPKA_ECCPoint* pstPublicKey, DrvPKA_PKAValue* pstKeyRandomizer, DrvPKA_PKAValue* pstMsgHash, DrvPKA_ECCSignature* pstSignature, MS_BOOL* bValid);

// Operation: Digital Signature generation
MS_BOOL MDrv_PKA_DSASign(DrvPKA_PKAValue* pstBase, DrvPKA_PKAValue* pstModP, DrvPKA_PKAValue* pstModN, DrvPKA_PKAValue* pstPrivateKey, DrvPKA_PKAValue* pstMsgHash, DrvPKA_PKAValue* pstRandomFactor, MS_U32 pstShift, DrvPKA_ECCSignature* pstSignature);

// Operation: Digital Signature verification
MS_BOOL MDrv_PKA_DSAVerify(DrvPKA_PKAValue* pstBase, DrvPKA_PKAValue* pstModP, DrvPKA_PKAValue* pstModN, DrvPKA_PKAValue* pstPublicKey, DrvPKA_PKAValue* pstMsgHash, MS_U32 pstShift, DrvPKA_ECCSignature* pstSignature, MS_BOOL* bValid);

#ifdef __cplusplus
}
#endif


#endif // _DRVPKA_H_
