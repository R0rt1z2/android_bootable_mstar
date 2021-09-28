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
#ifndef _DRVCLKM_H_
#define _DRVCLKM_H_

////////////////////////////////////////////////////////////////////////////////
/// @brief Byte CLK control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

// #define UART_TYPE_DEF
/// Version string.
#define CLKM_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'C','L','K','M'},                  /* IP__                                             */  \
    {'0','1'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','6'},                          /* 00 ~ 99                                          */  \
    {'0','0','3','1','0','0','5','0'},  /* CL#                                              */  \
    MSIF_OS



typedef enum _CLKM_DbgLvl
{
    E_CLKM_DBGLVL_NONE = 0,      /// no debug message
    E_CLKM_DBGLVL_WARNING,       /// show warning only
    E_CLKM_DBGLVL_ERROR,         /// show error only
    E_CLKM_DBGLVL_INFO,          /// show error & informaiton
    E_CLKM_DBGLVL_ALL,           /// show error, information & funciton name
}CLKM_DbgLvl;


typedef struct _CLKM_Info
{
    MS_BOOL     bInit;
    MS_S32      s32fd;
    MS_S32      s32Mutex;

}CLKM_Info;



typedef enum {
   
    E_MDRV_CMD_CLKM_Init,
    E_MDRV_CMD_CLKM_GET_HANDLE,
    E_MDRV_CMD_CLKM_CLK_GATE_DIABLE,
    E_MDRV_CMD_CLKM_SET_CLK_SOURCE,
    E_MDRV_CMD_CLKM_GET_SRAM_SD_INFO,


} eClkmIoctlOpt;

typedef enum _CLKM_Result
{
    E_CLKM_NOT_SUPPORT = -1
    ,E_CLKM_FAIL = 0
    ,E_CLKM_OK = 1
    ,E_CLKM_TIMEOUT
    ,E_CLKM_BUSY
}CLKM_Result;


typedef struct _CLKM_GetHandle_PARAM
{
    char *s8_Handle_Name;
    MS_S32 s32_Handle;
	
}CLKM_GetHandle_PARAM,*PCLKM_GetHandle_PARAM;


typedef struct _CLKM_CLK_GATE_DISABLE_PARAM
{

    MS_S32 s32_Handle;
	
}CLKM_CLK_GATE_DISABLE_PARAM,*PCLKM_CLK_GATE_DISABLE_PARAM;


typedef struct _CLKM_SET_CLK_SRC_PARAM
{
    MS_S32 s32_Handle;
    char *clk_src_name;
	
}CLKM_SET_CLK_SRC_PARAM,*PCLKM_SET_CLK_SRC_PARAM;


typedef struct _CLKM_GET_SRAM_SD_INFO_PARAM
{
    char *sram_sd_name;
    MS_U16 sram_sd_value;
	
}CLKM_GET_SRAM_SD_INFO_PARAM,*PCLKM_GET_SRAM_SD_INFO_PARAM;




MS_BOOL Drv_CLKM_Init(void);

void Drv_Clkm_Clk_Gate_Disable(MS_S32 s32Handle);

void Drv_Clkm_Set_Clk_Source(MS_S32 s32Handle,char *clk_src_name);

 MS_S32 Drv_Clkm_Get_Handle(char *name);

 MS_U16 Drv_Get_Sram_Sd_Info(char *name);

#ifdef __cplusplus
}
#endif
#endif // _DRVCLKM_H_
