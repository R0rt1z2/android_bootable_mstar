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

#ifndef _HAL_AESDMA_H_
#include "../../drvAESDMA.h"
#include "halAESDMA.h"
#endif

void HAL_RSA_LoadKeyE_v2(U32 *ptr_E)
{
    U32 i;

    RIU[(RSA_BASE_ADDR+(0x22<<1))]= RSA_E_BASE_ADDR; //RSA E addr
    RIU[(RSA_BASE_ADDR+(0x20<<1))]= ((RIU[(RSA_BASE_ADDR+(0x20<<1))])|(RSA_IND32_START)); //RSA start

    //RIU[(POR_STATUS_BASE_ADDR+(0xA<<1))]=(U16)((0x0000ffff)&(U32)(ptr_E)); //write ptr_E addr to por_status(0x10050A)

    for( i = 0; i < 64; i++ )
    {
        RIU[(RSA_BASE_ADDR+(0x23<<1))]= (U16)(((*(ptr_E+i))>>8)&0xFF00)|(((*(ptr_E+i))>>24)&0xFF);
        RIU[(RSA_BASE_ADDR+(0x24<<1))]= (U16)(((*(ptr_E+i))>>8)&0xFF)|(((*(ptr_E+i))<<8)&0xFF00);
        break;
    }

    for( i = 1; i < 64; i++ )
    {
        RIU[(RSA_BASE_ADDR+(0x23<<1))]= (U16)0;
        RIU[(RSA_BASE_ADDR+(0x24<<1))]= (U16)0;
    }
}

void HAL_RSA_LoadKeyN_v2(U32 *ptr_N)
{
    U32 i;
    ptr_N += 63;

    RIU[(RSA_BASE_ADDR+(0x22<<1))]= RSA_N_BASE_ADDR; //RSA N addr
    RIU[(RSA_BASE_ADDR+(0x20<<1))]= ((RIU[(RSA_BASE_ADDR+(0x20<<1))])|(RSA_IND32_START)); //RSA start

    //RIU[(POR_STATUS_BASE_ADDR+(0xB<<1))]=(U16)((0x0000ffff)&(U32)(ptr_N)); //write ptr_N addr to por_status(0x10050B)

    for( i = 0; i < 64; i++ )
    {
        RIU[(RSA_BASE_ADDR+(0x23<<1))]= (U16)(((*(ptr_N-i))>>8)&0xFF00)|(((*(ptr_N-i))>>24)&0xFF);
        RIU[(RSA_BASE_ADDR+(0x24<<1))]= (U16)(((*(ptr_N-i))>>8)&0xFF)|(((*(ptr_N-i))<<8)&0xFF00);
    }
}



void HAL_SHA_SetByPassTable(U8 bEnable)
{
    if (bEnable == TRUE)
    {
        //Enable SHA ByPass Table, 0x8[11] = 1
        RIU[(AESDMA_BASE_ADDR+(0x08<<1))] |= 0x800;
    }
    else
    {
        //Enable SHA ByPass Table, 0x8[11] = 1
        RIU[(AESDMA_BASE_ADDR+(0x08<<1))] &= ~(0x800);
    }
}

void HAL_SHA_SetAddress(U32 u32Addr)
{
    RIU[(AESDMA_BASE_ADDR+(0x0A<<1))] = (U16)((0x0000ffff)&(u32Addr));
    RIU[(AESDMA_BASE_ADDR+(0x0B<<1))] = (U16)((((0xffff0000)&(u32Addr)) - CONFIG_MIU0_BUSADDR)>>16);
}

void HAL_SHA_Start(void)
{
    //Enable SHA ByPass Table, 0x8[0] = 1
    RIU[(AESDMA_BASE_ADDR+(0x08<<1))] &= ~(0x0001);
    RIU[(AESDMA_BASE_ADDR+(0x08<<1))] |= 0x0001;
}

void HAL_AESDMA_Start_FileOutEn(U8 u8AESDMAStart)
{
    // AESDMA file start
    if(u8AESDMAStart==1)
    {
        RIU[(AESDMA_BASE_ADDR+(0x50<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x50<<1))])&(~(AESDMA_CTRL_FILE_ST|AESDMA_CTRL_FOUT_EN)));
        RIU[(AESDMA_BASE_ADDR+(0x50<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x50<<1))])|(AESDMA_CTRL_FILE_ST)|(AESDMA_CTRL_FOUT_EN));
    }
    else
    {
        RIU[(AESDMA_BASE_ADDR+(0x50<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x50<<1))])&(~(AESDMA_CTRL_FILE_ST|AESDMA_CTRL_FOUT_EN)));
    }
}

void HAL_SHA_Clear_Reset(void)
{
    //SHA_Reset
    RIU[SHARNG_BASE_ADDR+(0x08<<1)]= 0x0;
    RIU[SHARNG_BASE_ADDR+(0x08<<1)]= (RIU[SHARNG_BASE_ADDR+(0x08<<1)]|(SHARNG_CTRL_SHA_RST));
    RIU[SHARNG_BASE_ADDR+(0x08<<1)]= (RIU[SHARNG_BASE_ADDR+(0x08<<1)]&(~SHARNG_CTRL_SHA_RST));
}

void HAL_AESDMA_DisableHwKey(void)
{
    // disable normal bank to use efuse key
    RIU[(DMA_SECURE_BASE_ADDR+(0x01<<1))]= ((RIU[(DMA_SECURE_BASE_ADDR+(0x01<<1))])|(REG_USE_SECRET_KEY));
}

#if defined(CONFIG_MSTAR_NS_UBOOT) || defined(CONFIG_TEE_LOADER)
void HAL_AESDMA_Reset(void)
{
    RIU[(AESDMA_BASE_ADDR+(0x5F<<1))] = 0;
    RIU[(AESDMA_BASE_ADDR+(0x50<<1))] = 0;
}

void HAL_AESDMA_SetEngine(U32 engine)
{
    RIU[(AESDMA_BASE_ADDR+(0x51<<1))] = engine;
}

void HAL_AESDMA_SetDecrypt(U32 decrypt)
{
    if (decrypt)
        RIU[(AESDMA_BASE_ADDR+(0x51<<1))] |= 0x0200;
}

void HAL_AESDMA_SetKey(U32* key)
{
    int i;
    if (key)
    {
        // AESDMA using SW key
        RIU[(AESDMA_BASE_ADDR+(0x5e<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5e<<1))])& (~AESDMA_CTRL_USE_SECRET_KEY));
        for (i=0; i<8; i+=2)
        {
            RIU[(AESDMA_BASE_ADDR+((0x60+i)<<1))] = key[i>>1] & 0xFFFF;
            RIU[(AESDMA_BASE_ADDR+((0x60+i+1)<<1))] = (key[i>>1]>>16) & 0xFFFF;
        }
    }
    else
    {
        // AESDMA using HW key
        RIU[(AESDMA_BASE_ADDR+(0x5e<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5e<<1))])|AESDMA_CTRL_USE_SECRET_KEY);
    }
}

void HAL_AESDMA_SetKeyIndex(E_AESDMA_KEYTYPE keytype, U32* key)
{
    // Reset all key select
    RIU[(TZPC_PM_BASE_ADDR+(0x1c<<1))] = ((RIU[(TZPC_PM_BASE_ADDR+(0x1c<<1))])& (~REG_AESDMA_KEY_SEL));
    RIU[(TZPC_PM_BASE_ADDR+(0x31<<1))] = ((RIU[(TZPC_PM_BASE_ADDR+(0x31<<1))])& (~REG_TZPC_PM_MSTAR_KEY_SEL));
    RIU[(AESDMA_BASE_ADDR+(0x5e<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5e<<1))])& (~(AESDMA_CTRL_USE_SECRET_KEY | AESDMA_CTRL_RESERVE)));

    if( keytype == E_AESDMA_EFUSEKEY0)
    {
        // AESDMA using HW key
        RIU[(AESDMA_BASE_ADDR+(0x5e<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5e<<1))])|AESDMA_CTRL_USE_SECRET_KEY);
    }
    else if( keytype == E_AESDMA_EFUSEKEY1)
    {
        RIU[(AESDMA_BASE_ADDR+(0x5e<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5e<<1))])|AESDMA_CTRL_USE_SECRET_KEY|AESDMA_CTRL_RESERVE);
    }
    else if( keytype == E_AESDMA_UNIFORMKEY0)
    {
        // Assigned Uniform Key to AESDMA
        RIU[(TZPC_PM_BASE_ADDR+(0x1c<<1))] = ((RIU[(TZPC_PM_BASE_ADDR+(0x1c<<1))])| REG_AESDMA_KEY_SEL);
        // Use Uniform Key 0
        RIU[(TZPC_PM_BASE_ADDR+(0x31<<1))] = ((RIU[(TZPC_PM_BASE_ADDR+(0x31<<1))])& (~REG_TZPC_PM_MSTAR_KEY_SEL));
        // AESDMA using HW key
        RIU[(AESDMA_BASE_ADDR+(0x5e<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5e<<1))])|AESDMA_CTRL_USE_SECRET_KEY);
    }
    else if( keytype == E_AESDMA_UNIFORMKEY1)
    {
        // Assigned Uniform Key to AESDMA
        RIU[(TZPC_PM_BASE_ADDR+(0x1c<<1))] = ((RIU[(TZPC_PM_BASE_ADDR+(0x1c<<1))])| REG_AESDMA_KEY_SEL);
        // Use Uniform Key 1
        RIU[(TZPC_PM_BASE_ADDR+(0x31<<1))] = ((RIU[(TZPC_PM_BASE_ADDR+(0x31<<1))])| REG_TZPC_PM_MSTAR_KEY_SEL);
        // AESDMA using HW key
        RIU[(AESDMA_BASE_ADDR+(0x5e<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5e<<1))])|AESDMA_CTRL_USE_SECRET_KEY);
    }
    else
    {
        int i;
        for (i=0; i<8; i+=2)
        {
            RIU[(AESDMA_BASE_ADDR+((0x60+i)<<1))] = key[i>>1] & 0xFFFF;
            RIU[(AESDMA_BASE_ADDR+((0x60+i+1)<<1))] = (key[i>>1]>>16) & 0xFFFF;
        }
    }
}

unsigned int HAL_SYS_Query(E_SYS_QUERY query, unsigned int* result)
{
    switch(query)
    {
        case E_SYS_QUERY_MOBF_KEY_SAVED:
        {
            *result = RIU[(TZPC_PM_BASE_ADDR+(0x02<<1))] & 0x0FU;
            return TRUE;
        }
        case E_SYS_QUERY_MOBF_ENABLED:
        {
            *result = RIU[(MIU_BASE_ADDR+(0x15<<1))] & (0x1U << 15);
            return TRUE;
        }
        case E_SYS_QUERY_TRNG_KEY_SAVED:
        {
            *result = RIU[(STR_PM_BASE_ADDR+(0x22<<1))] & (0x1U << 0);
            return TRUE;
        }
        default:
        {
            return FALSE;
        }
    }
    return FALSE;
}

// use a random key which is different in each boot
void HAL_AESDMA_EnableRNGKey(void)
{
    // STR_PM, reg_hmac_auth = 1
    // mapping efuse key to RNG key
    RIU[(STR_PM_BASE_ADDR+(0x20<<1))]= ((RIU[(STR_PM_BASE_ADDR+(0x20<<1))])| 0x1U);

    // AESDMA using HW key
    RIU[(AESDMA_BASE_ADDR+(0x5e<<1))]= ((RIU[(AESDMA_BASE_ADDR+(0x5e<<1))])|AESDMA_CTRL_USE_SECRET_KEY);
}

void HAL_AESDMA_DisableRNGKey(void)
{
    // STR_PM, reg_hmac_auth = 0
    // disable mapping efuse key to RNG key
    RIU[(STR_PM_BASE_ADDR+(0x20<<1))]= ((RIU[(STR_PM_BASE_ADDR+(0x20<<1))]) & (~0x1U));
}

void HAL_AESDMA_ClearRNGKey(void)
{
    // STR_PM, reg_trng_saved_key_clean = 1
    // clear RNG key to ensure next time use different key
    RIU[(STR_PM_BASE_ADDR+(0x22<<1))]= ((RIU[(STR_PM_BASE_ADDR+(0x22<<1))]) | (0x1U << 4));
}

void HAL_AESDMA_SetIV(U32* key)
{
    int i;
    if (key)
    {
        // SW IV
        for (i=0; i<8; i+=2)
        {
            RIU[(AESDMA_BASE_ADDR+((0x68+i)<<1))] = key[i>>1] & 0xFFFF;
            RIU[(AESDMA_BASE_ADDR+((0x68+i+1)<<1))] = (key[i>>1]>>16) & 0xFFFF;
        }
    }
    else
    {
        // Zero IV
        for (i=0; i<8; i+=2)
        {
            RIU[(AESDMA_BASE_ADDR+((0x68+i)<<1))] = 0x0000;
            RIU[(AESDMA_BASE_ADDR+((0x68+i+1)<<1))] = 0x0000;
        }
    }
}


void HAL_AESDMA_SetFileInAddr(U32 addr)
{
    RIU[(AESDMA_BASE_ADDR+(0x52<<1))] = addr & 0xFFFF;
    RIU[(AESDMA_BASE_ADDR+(0x53<<1))] = (addr>>16) & 0xFFFF;
}

void HAL_AESDMA_SetFileOutAddr(U32 addr)
{
    RIU[(AESDMA_BASE_ADDR+(0x56<<1))] = addr & 0xFFFF;
    RIU[(AESDMA_BASE_ADDR+(0x57<<1))] = (addr>>16) & 0xFFFF;
}

void HAL_AESDMA_SetFileSize(U32 len)
{
    RIU[(AESDMA_BASE_ADDR+(0x54<<1))] = len & 0xFFFF;
    RIU[(AESDMA_BASE_ADDR+(0x55<<1))] = (len>>16) & 0xFFFF;
}

U32 HAL_AESDMA_IsFinished(void)
{
//    RIU[(0x1012+(0x74<<1))] = 0x1234;
    return RIU[(AESDMA_BASE_ADDR+(0x7F<<1))] & 0x0001;
}
#endif
