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


#ifndef _DRV_AESDMA_H_
#define _DRV_AESDMA_H_

#ifdef CONFIG_MSTAR_EDISON
#define AESDMA_HAL_EDISON       1
#endif

#ifdef CONFIG_MSTAR_EIFFEL
#define AESDMA_HAL_EIFFEL       1
#endif

#ifdef CONFIG_MSTAR_NIKE
#define AESDMA_HAL_NIKE         1
#endif

#ifdef CONFIG_MSTAR_MADISON
#define AESDMA_HAL_MADISON      1
#endif

#ifdef CONFIG_MSTAR_CLIPPERS
#define AESDMA_HAL_CLIPPERS     1
#endif

#ifdef CONFIG_MSTAR_MIAMI
#define AESDMA_HAL_MIAMI        1
#endif

#ifdef CONFIG_MSTAR_MONACO
#define AESDMA_HAL_MONACO       1
#endif

#ifdef CONFIG_MSTAR_MUJI
#define AESDMA_HAL_MUJI       1
#endif

#ifdef CONFIG_MSTAR_MANHATTAN
#define AESDMA_HAL_MANHATTAN     1
#endif

#ifdef CONFIG_MSTAR_MASERATI
#define AESDMA_HAL_MASERATI      1
#endif

#ifdef CONFIG_MSTAR_CELTICS
#define AESDMA_HAL_CELTICS       1
#endif

#ifdef CONFIG_MSTAR_MACAN
#define AESDMA_HAL_MACAN        1
#endif

#ifdef CONFIG_MSTAR_MAZDA
#define AESDMA_HAL_MAZDA        1
#endif

#ifdef CONFIG_MSTAR_MUSTANG
#define AESDMA_HAL_MUSTANG        1
#endif

#ifdef CONFIG_MSTAR_MAXIM
#define AESDMA_HAL_MAXIM        1
#endif

#ifdef CONFIG_MSTAR_NADAL
#define AESDMA_HAL_NADAL        1
#endif

#ifdef CONFIG_MSTAR_NUGGET
#define AESDMA_HAL_NUGGET       1
#endif

#ifdef CONFIG_MSTAR_EINSTEIN
#define AESDMA_HAL_EINSTEIN     1
#endif

#ifdef CONFIG_MSTAR_EINSTEIN3
#define AESDMA_HAL_EINSTEIN3    1
#endif

#ifdef CONFIG_MSTAR_NAPOLI
#define AESDMA_HAL_NAPOLI       1
#endif

#ifdef CONFIG_MSTAR_MONET
#define AESDMA_HAL_MONET        1
#endif

#ifdef CONFIG_MSTAR_MOONEY
#define AESDMA_HAL_MOONEY        1
#endif

#ifdef CONFIG_MSTAR_MESSI
#define AESDMA_HAL_MESSI        1
#endif

#ifdef CONFIG_MSTAR_MUNICH
#define AESDMA_HAL_MUNICH       1
#endif

#ifdef CONFIG_MSTAR_MAXIM
#define AESDMA_HAL_MAXIM       1
#endif

#ifdef CONFIG_MSTAR_MAINZ
#define AESDMA_HAL_MAINZ        1
#endif

#ifdef CONFIG_MSTAR_MATISSE
#define AESDMA_HAL_MATISSE      1
#endif

#ifdef CONFIG_MSTAR_MARLON
#define AESDMA_HAL_MARLON       1
#endif

#ifdef CONFIG_MSTAR_M5621
#define AESDMA_HAL_M5621        1
#endif

#ifdef CONFIG_MSTAR_M7621
#define AESDMA_HAL_M7621        1
#endif

#ifdef CONFIG_MSTAR_M7221
#define AESDMA_HAL_M7221        1
#endif

#ifdef CONFIG_MSTAR_M7821
#define AESDMA_HAL_M7821        1
#endif

#ifdef CONFIG_MSTAR_M7322
#define AESDMA_HAL_M7322        1
#endif

#ifdef CONFIG_MSTAR_M7622
#define AESDMA_HAL_M7622        1
#endif

#ifdef CONFIG_MSTAR_M3822
#define AESDMA_HAL_M3822        1
#endif

#if defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_TEE_LOADER)
typedef enum
{
    E_AESDMA_SWKEY = 0x0,
    E_AESDMA_EFUSEKEY0 = 0x10,
    E_AESDMA_EFUSEKEY1,
    E_AESDMA_UNIFORMKEY0 = 0x100,
    E_AESDMA_UNIFORMKEY1
}E_AESDMA_KEYTYPE;
#endif

//=====================================================
// do NOT edit the following content.
//=====================================================
#if defined(AESDMA_HAL_EDISON) && AESDMA_HAL_EDISON
  #include "hal/edison/halAESDMA.h"
#elif defined(AESDMA_HAL_EIFFEL) && AESDMA_HAL_EIFFEL
  #include "hal/eiffel/halAESDMA.h"
#elif defined(AESDMA_HAL_NIKE) && AESDMA_HAL_NIKE
  #include "hal/nike/halAESDMA.h"
#elif defined(AESDMA_HAL_MADISON) && AESDMA_HAL_MADISON
  #include "hal/madison/halAESDMA.h"
#elif defined(AESDMA_HAL_CLIPPERS) && AESDMA_HAL_CLIPPERS
  #include "hal/clippers/halAESDMA.h"
#elif defined(AESDMA_HAL_MIAMI) && AESDMA_HAL_MIAMI
  #include "hal/miami/halAESDMA.h"
#elif defined(AESDMA_HAL_MONACO) && AESDMA_HAL_MONACO
  #include "hal/monaco/halAESDMA.h"
#elif defined(AESDMA_HAL_MUJI) && AESDMA_HAL_MUJI
  #include "hal/muji/halAESDMA.h"
#elif defined(AESDMA_HAL_CELTICS) && AESDMA_HAL_CELTICS
  #include "hal/celtics/halAESDMA.h"
#elif defined(AESDMA_HAL_NADAL) && AESDMA_HAL_NADAL
    #include "hal/nadal/halAESDMA.h"
#elif defined(AESDMA_HAL_NUGGET) && AESDMA_HAL_NUGGET
  #include "hal/nugget/halAESDMA.h"
#elif defined(AESDMA_HAL_EINSTEIN) && AESDMA_HAL_EINSTEIN
  #include "hal/einstein/halAESDMA.h"
#elif defined(AESDMA_HAL_EINSTEIN3) && AESDMA_HAL_EINSTEIN3
  #include "hal/einstein3/halAESDMA.h"
#elif defined(AESDMA_HAL_NAPOLI) && AESDMA_HAL_NAPOLI
  #include "hal/napoli/halAESDMA.h"
#elif defined(AESDMA_HAL_MONET) && AESDMA_HAL_MONET
  #include "hal/monet/halAESDMA.h"
#elif defined(AESDMA_HAL_MOONEY) && AESDMA_HAL_MOONEY
  #include "hal/mooney/halAESDMA.h"
#elif defined(AESDMA_HAL_MESSI) && AESDMA_HAL_MESSI
  #include "hal/messi/halAESDMA.h"
#elif defined(AESDMA_HAL_MUNICH) && AESDMA_HAL_MUNICH
  #include "hal/munich/halAESDMA.h"
#elif defined(AESDMA_HAL_MANHATTAN) && AESDMA_HAL_MANHATTAN
  #include "hal/manhattan/halAESDMA.h"
#elif defined(AESDMA_HAL_MASERATI) && AESDMA_HAL_MASERATI
  #include "hal/maserati/halAESDMA.h"
#elif defined(AESDMA_HAL_MACAN) && AESDMA_HAL_MACAN
  #include "hal/macan/halAESDMA.h"
#elif defined(AESDMA_HAL_MAZDA) && AESDMA_HAL_MAZDA
  #include "hal/mazda/halAESDMA.h"
#elif defined(AESDMA_HAL_MUSTANG) && AESDMA_HAL_MUSTANG
  #include "hal/mustang/halAESDMA.h"
#elif defined(AESDMA_HAL_MAXIM) && AESDMA_HAL_MAXIM
  #include "hal/maxim/halAESDMA.h"
#elif defined(AESDMA_HAL_MAINZ) && AESDMA_HAL_MAINZ
  #include "hal/mainz/halAESDMA.h"
#elif defined(AESDMA_HAL_MATISSE) && AESDMA_HAL_MATISSE
  #include "hal/matisse/halAESDMA.h"
#elif defined(AESDMA_HAL_MARLON) && AESDMA_HAL_MARLON
  #include "hal/marlon/halAESDMA.h"
#elif defined(AESDMA_HAL_M5621) && AESDMA_HAL_M5621
  #include "hal/M5621/halAESDMA.h"
#elif defined(AESDMA_HAL_M7621) && AESDMA_HAL_M7621
  #include "hal/M7621/halAESDMA.h"
#elif defined(AESDMA_HAL_M7221) && AESDMA_HAL_M7221
  #include "hal/M7221/halAESDMA.h"
#elif defined(AESDMA_HAL_M7821) && AESDMA_HAL_M7821
  #include "hal/M7821/halAESDMA.h"
#elif defined(AESDMA_HAL_M7322) && AESDMA_HAL_M7322
  #include "hal/M7322/halAESDMA.h"
#elif defined(AESDMA_HAL_M7622) && AESDMA_HAL_M7622
  #include "hal/M7622/halAESDMA.h"
#elif defined(AESDMA_HAL_M3822) && AESDMA_HAL_M3822
  #include "hal/M3822/halAESDMA.h"
#else
  #error "Error! no platform selected."
#endif

void MDrv_AESDMA_SecureAuthen(U32 u32SHAInBuf, U32 u32Size, U32 u32RSAInBuf, U32 u32SHAOutBuf, U32 u32RSAOutBuf, U32 u32SramSel);
U32 MDrv_AESDMA_SecureMain(U32 u32PlaintextAddr, U32 u32Size, U32 u32SignatureAddr, U32 u32SramSel);

#if defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_TEE_LOADER)
U32 MDrv_AESDMA_Decrypt(U32 addr, U32 len, U8* key, U32 engine, E_AESDMA_KEYTYPE keytype);
U32 MDrv_AESDMA_Encrypt(U32 addr, U32 len, U8* key, U32 engine, E_AESDMA_KEYTYPE keytype);
U32 MDrv_AESDMA_STR_CBCMAC(U32 addr, U32 len, U8* result);
#endif

//-------------------------------------------------------------------------------------------------
/// Get HMAC with SHA256
/// @ingroup G_AES_EnDecrypt
/// @param text     \b IN: pointer to data stream
/// @param text_len \b IN: length of data stream
/// @param key      \b IN: pointer to authentication key
/// @param key_len  \b IN: length of authentication key
/// @param digest   \b OUT: caller digest to be filled in
/// @return n/a
//-------------------------------------------------------------------------------------------------
void MDrv_AESDMA_Hmac_sha256(const U8 *text, U32 text_len, const U8 *key, U32 key_len, void *digest);

//-------------------------------------------------------------------------------------------------
/// Get HMAC Key
/// @ingroup G_AES_EnDecrypt
/// @param u32KeyBufPA \b OUT: Physcial address of the key buffer.
/// @param u32key_len  \b IN: length of key (In the case, >=16 bytes is required)
/// @param key         \b IN: buffer for AESKey, NULL for EFuse key
/// @return 0 : Success
//-------------------------------------------------------------------------------------------------
U32 GetHmacKey(U32 u32KeyBufPA, U32 u32key_len, U8* key);

//-------------------------------------------------------------------------------------------------
/// Get Hash by SHA1/SHA256
/// @ingroup G_AES_EnDecrypt
/// @param u32SrcPA     \b IN: Physcial address of data.
/// @param u32Size      \b IN: length of data
/// @param eMode        \b IN: 1: SHA256, 0:SHA1
/// @param u32ShaOutPA  \b IN: Physcial address of hash
/// @return 0 : Success
//-------------------------------------------------------------------------------------------------
U32 MDrv_AESDMA_GetSHA (U32 u32SrcPA, U32 u32Size, U8 eMode, U32 u32ShaOutPA);

void MDrv_AESDMA_DisableHwKey(void);
#endif
