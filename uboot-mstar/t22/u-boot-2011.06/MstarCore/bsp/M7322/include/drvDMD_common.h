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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  DVBC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DMD_COMMON_H_
#define _DRV_DMD_COMMON_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define WRAP(p, ...) MDrv_DMD_MS_FLOAT_ST_WRAPPER(p, __VA_ARGS__)

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define USE_UTOPIA2P0

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    _QPSK        = 0x0,
    _16QAM        = 0x1,
    _64QAM        = 0x2,
    _UNKNOW_QAM   = 0xff,
}DMD_CONSTEL;

typedef enum
{
    _CR1Y2        = 0x0,
    _CR2Y3        = 0x1,
    _CR3Y4        = 0x2,
    _CR5Y6        = 0x3,
    _CR7Y8        = 0x4,
    _UNKNOW_CR   = 0xff,
}DMD_CODERATE;

typedef enum
{
    UNKNOW_SIGNAL    = 0x00,
    T_SIGNAL         = 0x55,
    T2_SIGNAL        = 0xaa,
}DMD_SIGNALTYPE;

typedef enum
{
    CODE_LOADED      = 0,
    CHANNEL_SWITCH   = 1,
    T_T2_SWITCH      = 2,
}DMD_CMDTYPE;

//bryan temp mark
#if(1)
typedef struct
{
    float   power_db;
    MS_U8   sar3_val;
}DMD_RFAGC_SSI;

typedef struct
{
    float   power_db;
    MS_U8   agc_val;
}DMD_IFAGC_SSI;

typedef struct
{
    float   attn_db;
    MS_U8   agc_err;
}DMD_IFAGC_ERR;

typedef struct
{
    MS_S16   power_db;
    MS_U8   sar3_val;
}DMD_RFAGC_SSI_KERNEL;

typedef struct
{
    MS_S16   power_db;
    MS_U8   agc_val;
}DMD_IFAGC_SSI_KERNEL;

typedef struct
{
    MS_S16   attn_db;
    MS_U8   agc_err;
}DMD_IFAGC_ERR_KERNEL;

typedef struct
{
    DMD_CONSTEL         constel;
    DMD_CODERATE        code_rate;
    float               p_ref;
}DMD_SSI_DBM_NORDIGP1;

typedef struct
{
    DMD_CONSTEL   constel;
    DMD_CODERATE   code_rate;
    float   cn_ref;
}DMD_SQI_CN_NORDIGP1;

typedef struct
{
    DMD_CONSTEL         constel;
    DMD_CODERATE        code_rate;
    MS_S16               p_ref;
}DMD_SSI_DBM_NORDIGP1_KERNEL;

typedef struct
{
    DMD_CONSTEL   constel;
    DMD_CODERATE   code_rate;
    MS_S16   cn_ref;
}DMD_SQI_CN_NORDIGP1_KERNEL;

typedef struct
{
    DMD_RFAGC_SSI *pRfagcSsi;
    MS_U16 u16RfagcSsi_Size;
    DMD_IFAGC_SSI *pIfagcSsi_LoRef;
    MS_U16 u16IfagcSsi_LoRef_Size;
    DMD_IFAGC_SSI *pIfagcSsi_HiRef;
    MS_U16 u16IfagcSsi_HiRef_Size;
    DMD_IFAGC_ERR *pIfagcErr_LoRef;
    MS_U16 u16IfagcErr_LoRef_Size;
    DMD_IFAGC_ERR *pIfagcErr_HiRef;
    MS_U16 u16IfagcErr_HiRef_Size;
}DMD_SSI_TABLE;
#endif
typedef struct _s_I2C_Interface_func
{
    MS_BOOL (*I2C_WriteBytes)(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
    MS_BOOL (*I2C_ReadBytes)(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);
}s_I2C_Interface_func;

typedef enum
{
  add = 0,
  minus,
  multiply,
  divide
}OP_type;

typedef struct
{
  MS_S32 DATA; // 2^31-1 ~ -2^31
  MS_S8 EXP; // -128~127
}MS_FLOAT_ST;
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_DMD_PreInit(void);
DLL_PUBLIC void MDrv_DMD_RFAGC_Tristate(MS_BOOL bEnable);
DLL_PUBLIC void MDrv_DMD_IFAGC_Tristate(MS_BOOL bEnable);

/************************************************************************************************
  Subject:    MDrv_DMD_TSO_Clk_Control
  Function:   ts output clock frequency and phase configure
  Parmeter:   u8cmd_array, clock div,           0x01, div (0x00~0x1f),
                           clock phase inv,     0x02, inv_en (0,1),
                           clock phase tuning,  0x03, phase_tuning_en (0,1), tuning_num (0x00~0x1f)
  Return:     MS_BOOL
  Remark:
*************************************************************************************************/
DLL_PUBLIC MS_BOOL MDrv_DMD_TSO_Clk_Control(MS_U8 *u8cmd_array);
DLL_PUBLIC MS_BOOL MDrv_DMD_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value);
DLL_PUBLIC MS_BOOL MDrv_DMD_WriteReg(MS_U32 u32Reg, MS_U8 u8Value);
DLL_PUBLIC MS_BOOL MDrv_DMD_WriteRegs(MS_U32 u32Reg, MS_U8 *u8Value, MS_U8 u8Length);
DLL_PUBLIC MS_BOOL MDrv_DMD_I2C_Channel_Change(MS_U8 ch_num);
DLL_PUBLIC MS_BOOL MDrv_DMD_I2C_Channel_Set(MS_U8  ch_num);

DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_Init(MS_U8  u8DeviceNum);
DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_MIU_Writes(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size);
DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_MIU_Reads(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size);
DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_RIU_Write8(MS_U16 u16Addr, MS_U8 data);
DLL_PUBLIC MS_BOOL MDrv_DMD_SSPI_RIU_Read8(MS_U16 u16Addr, MS_U8 *pdata);
DLL_PUBLIC MS_FLOAT_ST MDrv_DMD_MS_FLOAT_OP(MS_FLOAT_ST stRn,MS_FLOAT_ST stRd, OP_type eOpcode);
DLL_PUBLIC MS_FLOAT_ST MDrv_DMD_MS_FLOAT_ST_WRAPPER(const char text[], ...);
DLL_PUBLIC MS_BOOL MDrv_DMD_MS_FLOAT_Greater_Compare(MS_FLOAT_ST input1, MS_FLOAT_ST input2);
DLL_PUBLIC MS_S64 MDrv_DMD_MS_FLOAT_To_MS_S64 (MS_FLOAT_ST msf_input);
/************************************************************************************************
  Subject:    MDrv_DMD_SetSpeedUpIndex
  Function:   Set parameter to speedup T/T2 channel scan or channel switch.
  Parmeter:   Cmd, 1: CHANNEL_SWITCH   ==> Scan channel or switch channel.
                   2: T_T2_SWITCH      ==> If switch between T and T2.
  Parmeter:   Value, when Cmd==1       ==> 0: Scan channel.  1: Swtich to T.  2: Switch to T2.
                     when Cmd==2       ==> 0: Switch to other sources.  1: Switch between T and T2.
  Return:     MS_BOOL
  Remark:
*************************************************************************************************/
DLL_PUBLIC MS_BOOL MDrv_DMD_SetSpeedUpIndex(MS_U8 Cmd, MS_U8 Value);

#ifdef __cplusplus
}
#endif


#endif // _DRV_DVBC_H_

