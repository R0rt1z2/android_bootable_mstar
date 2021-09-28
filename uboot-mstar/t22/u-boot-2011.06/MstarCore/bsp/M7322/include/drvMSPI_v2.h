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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  MSPI Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MSPI_V2_H_
#define _DRV_MSPI_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure


typedef enum {
    MDrv_CMD_MSPI_Init_Ext,
    MDrv_CMD_MSPI_Init,
    MDrv_CMD_MSPI_Read,
    MDrv_CMD_MSPI_Write,
    MDrv_CMD_MSPI_DCConfig,
    MDrv_CMD_MSPI_CLKConfig,
    MDrv_CMD_MSPI_FRAMEConfig,
    MDrv_CMD_MSPI_SlaveEnable,
    MDrv_CMD_MSPI_SetPowerState,
    MDrv_CMD_MSPI_DbgLEvel,
    MDrv_CMD_MasterSPI_Init_Ext,
    MDrv_CMD_MasterSPI_Init,
    MDrv_CMD_MasterSPI_Read,
    MDrv_CMD_MasterSPI_Write,
    MDrv_CMD_MasterSPI_DCConfig,
    MDrv_CMD_MasterSPI_CLKConfig,
    MDrv_CMD_MasterSPI_FRAMEConfig,
    MDrv_CMD_MasterSPI_SlaveEnable,
    MDrv_CMD_MasterSPI_CSPadConfig,
    MDrv_CMD_MasterSPI_MaxClkConfig,
} eMSPIIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

typedef struct _MPSI_INIT_EXT
{
    MS_U8 u8HWNum;
}MSPI_INIT_EXT, *PMSPI_INIT_EXT;

typedef struct _MSPI_INIT
{
    MSPI_config *ptMSPIConfig;
    MS_U8 u8HWNum;
}MSPI_INIT,     *PMSPI_INIT;

typedef struct _MSPI_READ
{
    MS_U8 *pu8Data;
    MS_U16 u16Size;
}MSPI_READ,    *PMSPI_READ;

typedef struct _MSPI_WRITE
{
    MS_U8 *pu8Data;
    MS_U16 u16Size;
}MSPI_WRITE,   *PMSPI_WRITE;

typedef struct _MSPI_DCCONFIG
{
    MSPI_DCConfig *ptDCConfig;
}MSPI_DCCONFIG, *PMSPI_DCCONFIG;

typedef struct _MSPI_CLKCONFIG
{
    MSPI_CLKConfig *ptCLKConfig;
}MSPI_CLKCONFIG, *PMSPI_CLKCONFIG;

typedef struct _MSPI_FRAMECONFIG
{
    MSPI_FrameConfig *ptFrameConfig;
}MSPI_FRAMECONFIG, *PMSPI_FRAMECONFIG;

typedef struct _MSPI_SLAVE_ENABLE
{
    MS_BOOL bEnable;
}MSPI_SLAVE_ENABLE, *PMSPI_SLAVE_ENABLE;

typedef struct _MSPI_SETPOWERSTATE
{
    EN_POWER_MODE enPowerState;
}MSPISETPOWERSTATE, *PMSPISETPOWERSTATE; 

typedef struct _MASTERSPI_READ
{
    MS_U32 u32DevID;
    MS_U8 *pu8Data;
    MS_U16 u16Size;
}MASTERSPI_READ,    *PMASTERSPI_READ;

typedef struct _MASTERSPI_WRITE
{
    MS_U32 u32DevID;
    MS_U8 *pu8Data;
    MS_U16 u16Size;
}MASTERSPI_WRITE,   *PMASTERSPI_WRITE;

typedef struct _MASTERSPI_DCCONFIG
{
    MS_U32 u32DevID;
    MSPI_DCConfig *ptDCConfig;
}MASTERSPI_DCCONFIG, *PMASTERSPI_DCCONFIG;

typedef struct _MASTERSPI_CLKCONFIG
{
    MS_U32 u32DevID;
    MSPI_CLKConfig *ptCLKConfig;
}MASTERSPI_CLKCONFIG, *PMASTERSPI_CLKCONFIG;

typedef struct _MASTERSPI_FRAMECONFIG
{
    MS_U32 u32DevID;
    MSPI_FrameConfig *ptFrameConfig;
}MASTERSPI_FRAMECONFIG, *PMASTERSPI_FRAMECONFIG;

typedef struct _MASTERSPI_SLAVE_ENABLE
{
    MS_U32 u32DevID;
    MS_BOOL bEnable;
}MASTERSPI_SLAVE_ENABLE, *PMASTERSPI_SLAVE_ENABLE;

typedef struct _MASTERSPI_CSPADCONFIG
{
    MS_U32 u32DevID;
    MS_U32 u32CsPad;
}MASTERSPI_CS_PADCONFIG, *PMASTERSPI_CSPADCONFIG;

typedef struct _MASTERSPI_MAXCLKCONFIG
{
    MS_U32 u32DevID;
    MS_U32 u32MaxClk;
}MASTERSPI_MAXCLKCONFIG, *PMASTERSPI_MAXCLKCONFIG;

typedef struct _MSPI_SETDBGLEVEL
{
    MS_U8 u8DbgLevel;
}MSPI_SETDBGLEVEL,   *PMSPI_SETDBGLEVEL;


//MSPI DRV For Local Dimming  API Structure Declaration

//ST_DRV_MSPI_INFO describe mspi spec
typedef struct
{
    MS_U8 u8MspiChanel;               //mspi chanel
    MS_U8 u8MspiMode;                  //mspi triggle mode
    MS_U32 u32MspiClk;                //spi clk
    MS_U8 u8WBitConfig[8];            //write mode :The bits for per byte
    MS_U8 u8RBitConfig[8];            //read mode :The bits for per byte
    MS_U8 u8TrStart;                  //start delay time  The time from "reg_MSPI_trigger" to first SPI clock
    MS_U8 u8TrEnd;                    //end delay time  The time from last SPI clock to "reg_MSPI_done_flag"
    MS_U8 u8TB;                       //The time between byte to byte transfer
    MS_U8 u8TRW;                      //The time between last write and first read
}ST_DRV_MSPI_INFO;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _DRV_MSPI_V2_H_

