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

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------
#include <common.h>
#include <drvCPU.h>
#include <MsTypes.h>
#include <MsDebug.h>
#include <MsSetupTee.h>
#include <MsSysUtility.h>
#include <uboot_mmap.h>

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if defined (CONFIG_R2_BRINGUP)
#define BIT2		0x04
#define BIT11       0x0800
#define BIT12       0x1000
#define BIT13       0x2000

#ifndef UART_DBG_SECR2
#if (CONFIG_TV_CHIP==0)

#define UART_DBG_SECR2          0x00
#else
#define UART_DBG_SECR2          0x0D
#endif
#endif

#ifndef REG_R2_BASE
#if (CONFIG_TV_CHIP==0)
#define REG_R2_BASE             0x103500
#else
#define REG_R2_BASE             0x122A00
#endif
#endif
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Extern Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Private Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Public Functions
//-------------------------------------------------------------------------------------------------
void MApi_Disable_R2(void)
{
    UBOOT_TRACE("IN\n");

    WriteByte(REG_R2_BASE+0x40*2, 0x00); //disable R2

    UBOOT_TRACE("OK\n");
}

void MApi_R2_StartFrDRAM(MS_PHYADDR addr)
{
    MS_PHYADDR AeonBufAdr=addr;
    MS_U16 value;
    char *pEnv=NULL;
    UBOOT_TRACE("IN\n");
    printf("R2 Boot from 0x%llx\n", (MS_PHYADDR)AeonBufAdr);

    // select UART source
    //WriteByte(0x101EA6, UART_DBG_SRC);

    pEnv=getenv("uart2R2");
    if(pEnv!=NULL)
    {
        if(strcmp(pEnv,"1")==0)
        {
            printf("switch uart message to uart\n");
            WriteByte(0x101EA6, UART_DBG_SECR2);
        }
    }

//switch UART to dig_mux (instead of MCU51)
    value = Read2Byte(0x0E12);
#if 1//HK message
    value &= (~BIT12); //0: dig_mux (reg_uart_sel0)
    value |= BIT11; //1: enable uart rx via  PAD_DDCA for dig_mux (reg_uart_sel0)
#else //PM message
    value |= (BIT12); //1: HK MCU51 uart0
    value |= BIT11; //1: enable uart rx via  PAD_DDCA for dig_mux (reg_uart_sel0)
#endif
    Write2Byte(0x0E12,value);

    #if defined (CONFIG_NUTTX_BRINGUP_TEST)

    MDrv_COPRO_GetBase();
    MDrv_COPRO_Init_Front();
    MDrv_COPRO_Init_End(0xa200000);
    #else
    MDrv_COPRO_GetBase();
    MDrv_COPRO_Init_Front();
    MDrv_COPRO_Init_End(addr);
    #endif

    UBOOT_TRACE("OK\n");
}
#endif //CONFIG_R2_BRINGUP
