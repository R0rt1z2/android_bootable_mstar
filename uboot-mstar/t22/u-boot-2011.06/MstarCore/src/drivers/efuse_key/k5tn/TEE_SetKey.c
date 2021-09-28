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

// [Notice]: This file configuration should customerized per CA project 
//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <MsTypes.h>
#include <drvAESDMA.h>
#include <drvDSCMB.h>
//#include <drvSEM.h>

#define YY_CAVID 0x0F

#define STB_CHIP

// 3-Level KeyLadder input
static MS_U8 u8KL_AESDMA_Input[48] = {
    //Input Key 2
    0x48, 0xd0, 0xcf, 0xaa, 0x00, 0x23, 0x43, 0x6b, 0x09, 0xd9, 0x4b, 0xe2, 0xe3, 0x6e, 0x07, 0x2f,
    //Input Key 1
    0x95, 0x73, 0x93, 0x5f, 0x96, 0x8d, 0xec, 0xb6, 0x51, 0x85, 0xc3, 0xcd, 0xaa, 0x5b, 0x64, 0x18,
    //Input Key 0 (Encrypted Control Word)
    0xda, 0x1d, 0xe9, 0xb9, 0xff, 0xf1, 0x2b, 0x8a, 0xff, 0xa2, 0x14, 0x0d, 0xfa, 0x99, 0xfc, 0x41};

static MS_U8 u8KL_RootKey[16] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

void TEE_SetKey(void)
{
    DSCMB_KL_Status KL_Status = 0;
    MS_U8 ACPU_Out[16] = {0x00};

    MS_BOOL ret = FALSE;

    extern MS_BOOL MDrv_SEM_Init(void);
    extern DLL_PUBLIC MS_BOOL MDrv_DSCMB2_Init(void);
    extern DLL_PUBLIC MS_BOOL MDrv_DSCMB2_KLadder_AtomicExec(DSCMB_KLCfg_All* KLCfg , MS_U8 *ACPU_Out, DSCMB_KL_Status* u32Status );
    extern DLL_PUBLIC MS_BOOL MDrv_DSCMB2_SetDefaultCAVid(MS_U32 u32EngId, MS_U32 u32CAVid);

    DSCMB_KLCfg_All KLConfigAll = {
        .eAlgo = E_DSCMB_KL_AES,
        .eSrc = E_DSCMB_KL_SRC_ACPU,    // By CA project
        .eDst = E_DSCMB_KL_DST_DMA_SK0,     // Do not modify, only use SK0
        .eOutsize = E_DSCMB_KL_128_BITS,    // Always set 128 Bit
        .eKeyType = 0,                      // Don't care when dst is Crypto DMA
        .u32Level = 3,                      // By CA project
        .u32EngID = 0,                      // Don't care when dst is Crypto DMA
        .u32DscID = 0,                      // Don't care when dst is Crypto DMA
        .u8KeyACPU = u8KL_RootKey,          // If eSrc is not from ACPU, this field should be NULL or zero
        .pu8KeyKLIn = u8KL_AESDMA_Input,    // KL input encrypted data
        .bDecrypt = TRUE,                   // bDecrypt
        .bInverse = FALSE,                  // Don't care
        .u32CAVid = YY_CAVID,               // CAVid for output key, the same of DMA
    };
    MDrv_SEM_Init();
    MDrv_DSCMB2_Init();
	  // Run Key Ladder
    ret = MDrv_DSCMB2_KLadder_AtomicExec(&KLConfigAll , ACPU_Out, &KL_Status );
    if(ret == FALSE)
    {
        printf("Key Ladder: Fail!!! 0x%x\n",(unsigned int)KL_Status);
        return;
    }

    MDrv_AESDMA_SetDefaultCAVid(YY_CAVID); //By project, AESDMA set cavid,
    MDrv_AESDMA_SetKey_Ex(NULL, 0); //Select key from KL
}

