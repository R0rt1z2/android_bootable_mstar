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
/// @brief  Master SPI Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_MSPI MSPI interface
   *  \ingroup  G_PERIPHERAL

    \brief

    MSPI is a synchronous serial interface and can connect to a variety of external device.

    <b>Features</b>

     - Generic SPI protocol with half duplex
     - Supports Motorola SPI compatible timing(CPHA/CPOL)
     - 8-Byte write buffer and 8-Byte read buffer
     - Configurable Bit width from one bit to 8 bits in a byte transfer
     - Supports up to 8 slave device select signals
     - Supports 3-wire mode
     - Supports an internal RIU (Register Interface Unit) interface. RIU is an in-house protocol of MSTAR

    <b>MSPI Block Diagram</b> \n
    \image html drvMSPI_pic.png

     \defgroup G_MSPI_INIT Initialization Task relative
     \ingroup  G_MSPI
     \defgroup G_MSPI_COMMON Common Task relative
     \ingroup  G_MSPI
     *! \defgroup G_MSPI_CONTROL Control relative
     *  \ingroup  G_MSPI
     *! \defgroup G_MSPI_OTHER  other relative
     *  \ingroup  G_MSPI
     \defgroup G_MSPI_ToBeModified MSPI api to be modified
     \ingroup  G_MSPI
     \defgroup G_MSPI_ToBeRemove MSPI api to be removed
     \ingroup  G_MSPI
*/



#ifndef _DRV_MSPI_H_
#define _DRV_MSPI_H_
#include "MsCommon.h"


#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSPI_UTOPIA20                  (0)
#define MSPI_READ_OPERATION    0
#define MSPI_WRITE_OPERATION   1
#define MSPI_CMD_TYPE          3

// config bit map
#define MSPI_DC_CONFIG        1
#define MSPI_CLK_CONFIG       2
#define MSPI_FRAME_CONFIG     4

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8 u8TrStart;      //time from trigger to first SPI clock
    MS_U8 u8TrEnd;        //time from last SPI clock to transferred done
    MS_U8 u8TB;           //time between byte to byte transfer
    MS_U8 u8TRW;          //time between last write and first read
} MSPI_DCConfig;

typedef struct
{
    MS_U8 u8WBitConfig[8];      //bits will be transferred in write buffer
    MS_U8 u8RBitConfig[8];      //bits Will be transferred in read buffer
} MSPI_FrameConfig;


typedef struct
{
	MS_U8 U8Clock;
	MS_BOOL BClkPolarity;
	MS_BOOL BClkPhase;
    MS_U32 u32MAXClk;
} MSPI_CLKConfig;

typedef struct
{
	MSPI_DCConfig  tMSPI_DCConfig[MSPI_CMD_TYPE];
	MSPI_FrameConfig  tMSPI_FrameConfig[MSPI_CMD_TYPE];
	MSPI_CLKConfig tMSPI_ClockConfig[MSPI_CMD_TYPE];
	MS_U8 U8ChipSel;
	MS_BOOL BLsbFirst;
	void (*MSPIIntHandler)(void); // interrupt handler
	MS_BOOL BIntEnable;   // interrupt mode enable or polling mode
	MS_U8 U8BitMapofConfig[MSPI_CMD_TYPE];
    MS_U32 u32DevId;
} MSPI_config;

typedef struct
{
    MS_U16 PadCs;
}stMSPI_csPdCfg;
typedef enum _MSPI_ERRORNOn
{
     E_MSPI_OK = 0
    ,E_MSPI_INIT_FLOW_ERROR =1
    ,E_MSPI_DCCONFIG_ERROR =2
    ,E_MSPI_CLKCONFIG_ERROR =4
    ,E_MSPI_FRAMECONFIG_ERROR =8
    ,E_MSPI_OPERATION_ERROR = 0x10
    ,E_MSPI_PARAM_OVERFLOW = 0x20
    ,E_MSPI_MMIO_ERROR = 0x40
    ,E_MSPI_HW_NOT_SUPPORT = 0x80
    ,E_MSPI_NULL
}MSPI_ErrorNo;

typedef enum
{
    E_MSPI_DBGLV_NONE,    //disable all the debug message
    E_MSPI_DBGLV_INFO,    //information
    E_MSPI_DBGLV_NOTICE,  //normal but significant condition
    E_MSPI_DBGLV_WARNING, //warning conditions
    E_MSPI_DBGLV_ERR,     //error conditions
    E_MSPI_DBGLV_CRIT,    //critical conditions
    E_MSPI_DBGLV_ALERT,   //action must be taken immediately
    E_MSPI_DBGLV_EMERG,   //system is unusable
    E_MSPI_DBGLV_DEBUG,   //debug-level messages
} MSPI_DbgLv;

////////////////////////////////////////////////////////////////////////////////
// include utopia v2  header files here
////////////////////////////////////////////////////////////////////////////////
#include "drvMSPI_v2.h"

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @ingroup G_MSPI_INIT
/// Description : MSPI initial EXt
/// @param u8HWNum \b IN : Index number of hardware
/// @return E_MSPI_OK : Succeed
/// @return > 0 : Failed to initial, please refer to MSPI_ErrorNo for error code
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_Init_Ext(MS_U8 u8HWNum); // ToBeRemove

//------------------------------------------------------------------------------
/// @ingroup G_MSPI_INIT
/// Description : MSPI initial
/// @param tMSPIConfig \b IN : struct pointer of MSPI config
/// @param u8HWNum \b IN : Index number of hardware
/// @return E_MSPI_OK : Succeed
/// @return > 0 : Failed to initial, please refer to MSPI_ErrorNo for error code
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_Init(MSPI_config *tMSPIConfig, MS_U8 u8HWNum);

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_COMMON
/// Description : Read data from MSPI
/// @param pData \b OUT : Pointer to receive data from MSPI read buffer
/// @param u16Size \b IN : Read data size
/// @return E_MSPI_OK : Succeed
/// @return > 0 : Failed to read, please refer to MSPI_ErrorNo for error code
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_Read(MS_U8 *pData, MS_U16 u16Size);

//------------------------------------------------------------------------------
/// @ingroup G_MSPI_COMMON
/// Description : Write data to MSPI
/// @param pData \b IN : Pointer to write data to MSPI write buffer
/// @param u16Size \b IN : Write data size
/// @return E_MSPI_OK : Succeed
/// @return > 0 : Failed to write, please refer to MSPI_ErrorNo for error code
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_Write(MS_U8 *pData, MS_U16 u16Size);

//------------------------------------------------------------------------------
/// @ingroup G_MSPI_ToBeRemove
/// Description : Config spi transfer timing
/// @param ptDCConfig \b IN Struct pointer of transfer timing config
/// @return E_MSPI_OK : Succeed
/// @return E_MSPI_DCCONFIG_ERROR : Failed to config transfer timing
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_DCConfig(MSPI_DCConfig *ptDCConfig); // ToBeRemove

//------------------------------------------------------------------------------
/// @ingroup G_MSPI_ToBeRemove
/// Description : Config spi clock setting
/// @param ptCLKConfig \b IN Struct pointer of clock config
/// @return E_MSPI_OK : Succeed
/// @return E_MSPI_CLKCONFIG_ERROR : Failed to config spi clock
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_CLKConfig(MSPI_CLKConfig *ptCLKConfig); // ToBeRemove

//------------------------------------------------------------------------------
/// @ingroup G_MSPI_ToBeRemove
/// Description : config spi transfer bit per frame
/// @param ptFrameConfig \b IN struct pointer of bits of buffer tranferred to slave config
/// @return E_MSPI_OK : Succeed
/// @return E_MSPI_FRAMECONFIG_ERROR : Failed to config transfered bit per buffer
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_FRAMEConfig(MSPI_FrameConfig *ptFrameConfig); // ToBeRemove

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_COMMON
/// Description : Enable or Disable Slave device
/// @param Enable \b IN : value to eable or disble slave device
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_MSPI_SlaveEnable(MS_BOOL Enable);

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_COMMON
/// Description : Chip support MSPI
/// @param None
/// @return TRUE : Chip Support MSPI
/// @return FALSE : Chip Not Support MSPI
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MSPI_HW_Support(void);

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_COMMON
/// Description : Set MSPI Power State
/// @param enPowerState \b IN : Value to set MSPI Power State
/// @return 0 : Succeed
/// @return > 1 : Failed to set power state
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_MSPI_SetPowerState(EN_POWER_MODE enPowerState);

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_COMMON
/// Description : Set debug message level for MSPI
/// @param u8DbgLevel \b IN : Value of debug message level
/// @return TRUE : Succeed
/// @return FALSE : Failed to set the debug level
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MSPI_SetDbgLevel(MS_U8 u8DbgLevel);

//==================================================================
//         Master SPI API with slave device id to support multiple slave device
//==================================================================
//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_INIT
/// Description : Master SPI initial
/// @param tMSPIConfig \b IN : struct pointer of MSPI config
/// @param u8HWNum \b OTU : Index number of hardware
/// @return E_MSPI_OK : Succeed
/// @return > 0 : Failed to initial, please refer to MSPI_ErrorNo for error code
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_Init(MSPI_config *tMSPIConfig, MS_U8 u8HWNum);

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_COMMON
/// Description : Read data from MSPI
/// @param u32DevID \b IN : Device id of slave device
/// @param pData \b OUT : Pointer to receive data from MSPI read buffer
/// @param u16Size \b IN : Read data size
/// @return E_MSPI_OK : Succeed
/// @return > 0 : Failed to read, please refer to MSPI_ErrorNo for error code
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_Read(MS_U32 u32DevID, MS_U8 *pData, MS_U16 u16Size);

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_COMMON
/// Description : Write data to MSPI
/// @param u32DevID \b IN : Device id of slave device
/// @param pData \b IN : Pointer to write data to MSPI write buffer
/// @param u16Size \b IN : Write data size
/// @return E_MSPI_OK : Succeed
/// @return > 0 : Failed to write, please refer to MSPI_ErrorNo for error code
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_Write(MS_U32 u32DevID, MS_U8 *pData, MS_U16 u16Size);

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_ToBeRemove
/// Description : Config spi transfer timing
/// @param u32DevID \b IN : Device id of slave device
/// @param ptDCConfig \b IN Struct pointer of transfer timing config
/// @return E_MSPI_OK : Succeed
/// @return E_MSPI_DCCONFIG_ERROR : Failed to config transfer timing
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_DCConfig(MS_U32 u32DevID, MSPI_DCConfig *ptDCConfig); // ToBeRemove

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_ToBeRemove
/// Description : Config spi clock setting
/// @param u32DevID \b IN : Device id of slave device
/// @param ptCLKConfig \b IN Struct pointer of clock config
/// @return E_MSPI_OK : Succeed
/// @return E_MSPI_CLKCONFIG_ERROR : Failed to config spi clock
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_CLKConfig(MS_U32 u32DevID, MSPI_CLKConfig *ptCLKConfig); // ToBeRemove

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_ToBeRemove
/// Description : Config spi transfer bit per frame
/// @param u32DevID \b IN : Device id of slave device
/// @param ptFrameConfig \b IN Struct pointer of bits of buffer tranferred to slave config
/// @return E_MSPI_OK : Succeed
/// @return E_MSPI_FRAMECONFIG_ERROR : Failed to config transfered bit per buffer
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_FRAMEConfig(MS_U32 u32DevID, MSPI_FrameConfig *ptFrameConfig); // ToBeRemove

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_COMMON
/// Description : Enable or Disable Slave device
/// @param u32DevID \b IN : Device id of slave device
/// @param Enable \b IN : Value to eable or disble slave device
/// @return E_MSPI_OK : Succeed
/// @return > 0 : Failed to enable or disable slave device, please refer to MSPI_ErrorNo for error code
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_SlaveEnable(MS_U32 u32DevID, MS_BOOL Enable);

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_ToBeRemove
/// Description : CS Pad Config Setting
/// @param u32DevID \b IN : Device id of slave device
/// @param u32CsPad \b IN : Pad number
/// @return TRUE : Succeed
/// @return FALSE : Setting CS Pad Failed
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MasterSPI_CsPadConfig(MS_U32 u32DevID, MS_U32 u32CsPad); // ToBeRemove

//-------------------------------------------------------------------------------------------------
/// Description : Maximum Clock Config Setting
/// @param u32DevID \b IN : Device id of slave device
/// @param u32MaxClk \b IN : Value of Maximum Clock
/// @return TRUE : Succeed
/// @return FALSE : Setting Maximum Clock Config Failed
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MasterSPI_MaxClkConfig(MS_U32 u32DevID, MS_U32 u32MaxClk); // ToBeRemove

//------------------------------------------------------------------------------
/// Description : config mspi info for local dimming
/// @param stMspi_Info \b IN : mspi info struct
/// @return E_MSPI_OK : Succeed
/// @return > 0 : Failed to config mspi, please refer to MSPI_ErrorNo for error code
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_Info_Config(ST_DRV_MSPI_INFO *stMspi_Info);

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_ToBeRemove
/// @param Direct \b IN : Read or write flag, 0 : read, 1 : write
/// @param u8Bytes \b IN: Set read/write trigger buffer byte size
/// @return E_MSPI_OK : Succeed
/// @return > 0 : Failed to read/write bytes, please refer to MSPI_ErrorNo for error code
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_RWBytes(MS_BOOL Direct, MS_U8 u8Bytes);


//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_COMMON
/// Description : Enable or Disable slave device on specified channel
/// @param Enable \b IN :Channel, value to eable or disble slave device
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_MSPI_SlaveEnable_Channel(MS_U8 u8ch,MS_BOOL Enable);

//------------------------------------------------------------------------------
/// @ingroup G_MSPI_COMMON
/// Description : Write data to specified MSPI channel.
/// @param pData \b IN : Channel, Pointer to write data to MSPI write buffer
/// @param u16Size \b IN : Write data size
/// @return E_MSPI_OK : Succeed
/// @return > 0 : Failed to write, please refer to MSPI_ErrorNo for error code
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_Write_Channel(MS_U8 u8ch,MS_U8 *pData, MS_U16 u16Size);

//-------------------------------------------------------------------------------------------------
/// @ingroup G_MSPI_COMMON
/// Description : Read data from specified MSPI channel.
/// @param pData \b OUT : Channel,Pointer to receive data from MSPI read buffer
/// @param u16Size \b IN : Read data size
/// @return E_MSPI_OK : Succeed
/// @return > 0 : Failed to read, please refer to MSPI_ErrorNo for error code
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_Read_Channel(MS_U8 u8ch,MS_U8 *pData, MS_U16 u16Size);

#ifdef __cplusplus
}
#endif

#endif // _DRV_MSPI_H_

