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
/// @brief  Serial Flash Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_SERFLASH SERFLASH interface
   *  \ingroup  G_PERIPHERAL

     \brief

     Flash Self Programming controlleris designed to for easy read/write external Serial Peripheral interface (SPI) flash.

     <b>Features</b>

     - Read ID Sequence
     - Read SPI Status Sequence
     - Write SPI Status Sequence
     - Chip Erase Sequence
     - Sector/Block Erase Sequence
     - Page Program Sequence
     - Read Data Sequence

    <b>SPI Diagram</b> \n
     \image html drvSERFLASH_pic.png

     *! \defgroup G_SERFLASH_INIT Initialization Task relative
     *  \ingroup  G_SERFLASH
     \defgroup G_SERFLASH_COMMON Common Task relative
     \ingroup  G_SERFLASH
     \defgroup G_SERFLASH_CONTROL Control relative
     \ingroup  G_SERFLASH
     \defgroup G_SERFLASH_FSP FSP relative
     \ingroup  G_SERFLASH
     *! \defgroup G_SERFLASH_OTHER  other relative
     *  \ingroup  G_SERFLASH
     *! \defgroup G_SERFLASH_ToBeModified SERFLASH api to be modified
     *  \ingroup  G_SERFLASH
     \defgroup G_SERFLASH_ToBeRemove SERFLASH api to be removed
     \ingroup  G_SERFLASH
*/

#ifndef _DRV_SERFLASH_H_
#define _DRV_SERFLASH_H_

#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define SERFLASH_UTOPIA20                  (0)
#define MSIF_SPIF_LIB_CODE                     {'S','P','I','F'}    //Lib code
#define MSIF_SPIF_LIBVER                       	{'0','6'}            //LIB version
#define MSIF_SPIF_BUILDNUM                      {'0','1'}            //Build Number
#define MSIF_SPIF_CHANGELIST                    {'0','0','4','5','6','1','5','8'} //P4 ChangeList Number

#define      SPIF_DRV_VERSION                  /* Character String for DRV/API version             */  \
        MSIF_TAG,                         /* 'MSIF'                                           */  \
        MSIF_CLASS,                       /* '00'                                             */  \
        MSIF_CUS,                         /* 0x0000                                           */  \
        MSIF_MOD,                         /* 0x0000                                           */  \
        MSIF_CHIP,                                                                              \
        MSIF_CPU,                                                                               \
        MSIF_SPIF_LIB_CODE,                    /* IP__                                             */  \
        MSIF_SPIF_LIBVER,                      /* 0.0 ~ Z.Z                                        */  \
        MSIF_SPIF_BUILDNUM,                    /* 00 ~ 99                                          */  \
        MSIF_SPIF_CHANGELIST,                  /* CL#                                              */  \
        MSIF_OS

/// Operation cfg
#define SPIDMA_CFG_ADDR_DIR_BIT   (0)
#define SPIDMA_CFG_REFLECT_BIT    (1)
#define SPIDMA_CFG_CRCCOPY_BIT    (2)
#define SPIDMA_CFG_NOWAITCOPY_BIT (3)

#define SPIDMA_OPCFG_DEF          (0)
#define SPIDMA_OPCFG_INV_COPY     _LShift(1, SPIDMA_CFG_ADDR_DIR_BIT)
#define SPIDMA_OPCFG_CRC_REFLECT  _LShift(1, SPIDMA_CFG_REFLECT_BIT)    //bit reflection of each input byte
#define SPIDMA_OPCFG_CRC_COPY     _LShift(1, SPIDMA_CFG_CRCCOPY_BIT)    //copy then crc check
#define SPIDMA_OPCFG_NOWAIT_COPY  _LShift(1, SPIDMA_CFG_NOWAITCOPY_BIT) //copy then quit
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// Serial Flash information structure
typedef struct
{
    MS_U32 u32AccessWidth;      //data access width in bytes
    MS_U32 u32TotalSize;        //total size in bytes
    MS_U32 u32SecNum;           //number of sectors
    MS_U32 u32SecSize;          //sector size in bytes
} SERFLASH_Info;

typedef struct
{
    MS_BOOL bIsBusy;
} SERFLASH_DrvStatus;

typedef enum
{
    E_SERFLASH_DBGLV_NONE,    //disable all the debug message
    E_SERFLASH_DBGLV_INFO,    //information
    E_SERFLASH_DBGLV_NOTICE,  //normal but significant condition
    E_SERFLASH_DBGLV_WARNING, //warning conditions
    E_SERFLASH_DBGLV_ERR,     //error conditions
    E_SERFLASH_DBGLV_CRIT,    //critical conditions
    E_SERFLASH_DBGLV_ALERT,   //action must be taken immediately
    E_SERFLASH_DBGLV_EMERG,   //system is unusable
    E_SERFLASH_DBGLV_DEBUG,   //debug-level messages
} SERFLASH_DbgLv;

typedef enum _SPIDMA_Dev
{
     E_SPIDMA_DEV_MIU0
    ,E_SPIDMA_DEV_MIU1
    ,E_SPIDMA_DEV_DMDMCU = 6
    ,E_SPIDMA_DEV_VDMCU
    ,E_SPIDMA_DEV_DSP
    ,E_SPIDMA_DEV_TSP
    ,E_SPIDMA_DEV_1KSRAM_HK51
    ,E_SPIDMA_DEV_MIU2
    ,E_SPIDMA_DEV_MIU3
    ,E_SPIDMA_DEV_NOT_SUPPORT
}SPIDMA_Dev;

typedef enum _SPI_DrvCKG
{
     E_SPI_XTALI = 0
    ,E_SPI_27M
    ,E_SPI_36M
    ,E_SPI_43M
    ,E_SPI_54M
    ,E_SPI_72M
    ,E_SPI_86M
    ,E_SPI_108M
    ,E_SPI_24M = 15 // T3 only
    ,E_SPI_HALCKG_NOT_SUPPORT
}SPI_DrvCKG;

typedef enum _SPI_DrvClkDiv
{
     E_SPI_DIV2
    ,E_SPI_DIV4
    ,E_SPI_DIV8
    ,E_SPI_DIV16
    ,E_SPI_DIV32
    ,E_SPI_DIV64
    ,E_SPI_DIV128
    ,E_SPI_ClkDiv_NOT_SUPPORT
}SPI_DrvClkDiv;

typedef enum _SERFLASH_RegionMapType
{
    E_SERFLASH_REGION_BLOCK_PROTECTION
   ,E_SERFLASH_REGION_INDIVIDUAL_BLOCK_LOCK
   ,E_SERFLASH_REGION_ONE_TIME_PROGRAM
   ,E_SERFLASH_REGION_SPECIAL_REGION
}SERFLASH_RegionMapType;

typedef enum
{
    E_FLASH_FSP
   ,E_FLASH_QSPI
} EN_FLASH_HOST;

typedef struct
{
   MS_U32  u32OtpLowerRecord;
   MS_U32  u32OtpUpperRecord;
   MS_U8    u8OtpBuffer[1024];
   void         *pstNext;
}ST_OTP_Record;


typedef struct
{
    MS_BOOL bProtectMark;
    MS_U32   u32Indicator;
    MS_U32   u32LowerBound;
    MS_U32   u32UpperBound;
    //ST_OTP_Record   StrOptRecord[3];
    void        *pstNext;
}SERFLASH_ProtecLayoutInfo;

typedef struct
{
   MS_U32 u32Start;
   MS_U32 u32Len;
   MS_U8 *u8data;
} SERFLASH_AccessInfo;

typedef struct
{
   MS_U32 u32StartAddr;
   MS_U32 u32EraseSize;
   MS_BOOL bWait;
} SERFLASH_EraseInfo;

typedef struct
{
   MS_U32 u32StartAddr;
   MS_U32 u32Endaddr;
} SERFLASH_SectorEraseInfo;

typedef struct
{
  MS_U32 u32DisableLowerBound;
  MS_U32 u32DisableSize;
} SERFLASH_DisRangeInfo;

typedef struct
{
    MS_U8 *pu8FlashID;
    MS_U32 u32IDSize;
} SERFLASH_ReadID;

typedef struct
{
    MS_U32 u32Addr;
    MS_U32 *u32BlockIndex;
} SERFLASH_Address_To_Block;

typedef struct
{
    MS_U32  u32StartBlock;
    MS_U32  u32EndBlock;
    MS_BOOL bWait;
} SERFLASH_BlockErase;

typedef enum _SPI_Attribute
{
     E_SPI_ATTRI_USER_DRV
    ,E_SPI_ATTRI_KERNEL_DRV
    ,E_SPI_ATTRI_UNKNOWN
}SPI_Attribute;

///SPI CS callback
typedef void (*ms_Mcu_ChipSelect_CB)(void);
typedef void (*ms_Flash_SetHWWP_CB)(MS_BOOL);
////////////////////////////////////////////////////////////////////////////////
// include utopia v2  header files here
////////////////////////////////////////////////////////////////////////////////
#include "drvSERFLASH_v2.h"

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Description : Get the information of Serial Flash
/// @ingroup G_SERFLASH_COMMON
/// @return the pointer to the driver information
//-------------------------------------------------------------------------------------------------
const SERFLASH_Info *MDrv_SERFLASH_GetInfo(void);

//------------------------------------------------------------------------------
/// Description : Show the SERFLASH driver version
/// @ingroup G_SERFLASH_COMMON
/// @param  ppVersion    \b OUT: output NORF driver version
/// @return TRUE : succeed
/// @return FALSE : failed
//------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_GetLibVer(const MSIF_Version **ppVersion);

//------------------------------------------------------------------------------
/// Description : Get Serial Flash driver status
/// @ingroup G_SERFLASH_COMMON
/// @param  pDrvStatus    \b OUT: poniter to store the returning driver status
/// @return TRUE : succeed
/// @return FALSE : failed to get the driver status
//------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_GetStatus(SERFLASH_DrvStatus* pDrvStatus);

//------------------------------------------------------------------------------
/// Description : Get detailed level of Parallel Flash driver debug message
/// @ingroup G_SERFLASH_COMMON
/// @param u8DbgLevel    \b OUT  debug level for Serial Flash driver
/// @return TRUE : succeed
/// @return FALSE : failed to get the debug level
//------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_GetDbgLevel(MS_U8 *u8DbgLevel);

//------------------------------------------------------------------------------
/// Description : Set detailed level of Parallel Flash driver debug message
/// @ingroup G_SERFLASH_COMMON
/// @param u8DbgLevel    \b IN  debug level for Serial Flash driver
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_SetDbgLevel(MS_U8 u8DbgLevel);
//------------------------------------------------------------------------------
/// Description : Set detailed level of Parallel Flash driver debug message
/// @ingroup G_SERFLASH_CONTROL
/// @param u8DbgLevel    \b IN  debug level for Serial Flash driver
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_SetWPInfo(MS_BOOL bWPInfo);

//------------------------------------------------------------------------------
/// Description : HK ask 8051 to select flash chip by call back function
/// @ingroup G_SERFLASH_COMMON
/// @param ms_Mcu_ChipSelect_CB    \b IN  call back function
/// @return TRUE : succeed
/// @return NULL :
//------------------------------------------------------------------------------
void MDrv_SERFLASH_SetMcuCSCallBack(ms_Mcu_ChipSelect_CB ChipSel_cb);
//------------------------------------------------------------------------------
/// Description : HK ask 8051 to select flash chip by call back function
/// @ingroup G_SERFLASH_COMMON
/// @param ms_Mcu_ChipSelect_CB    \b IN  call back function
/// @return TRUE : succeed
/// @return NULL :
//------------------------------------------------------------------------------
void MDrv_SERFLASH_SetFlashWPCallBack(ms_Flash_SetHWWP_CB FlashWP_cb);

//-------------------------------------------------------------------------------------------------
/// Description : Detect flash type by reading the MID and DID
/// @ingroup G_SERFLASH_CONTROL
/// @return TRUE : succeed
/// @return FALSE : unknown flash type
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_DetectType(void);

//-------------------------------------------------------------------------------------------------
/// Description : Detect flash Size
/// @ingroup G_SERFLASH_CONTROL
/// @return TRUE : succeed
/// @return FALSE : unknown flash size
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_DetectSize(MS_U32 *u32FlashSize);

//-------------------------------------------------------------------------------------------------
/// Description : Enable Flash 2XREAD mode, if support
/// @ingroup G_SERFLASH_CONTROL
/// @param  b2XMode    \b IN: ENABLE/DISABLE
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   Please ref. sprc. to confirm Flash support or not
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_Set2XRead(MS_BOOL b2XMode);

//-------------------------------------------------------------------------------------------------
/// Description : Set ckg_spi which flash supports (please ref. the spec. before using this function)
/// @ingroup G_SERFLASH_CONTROL
/// @param  SPI_DrvCKG    \b IN: enumerate the ckg_spi
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   Please ref. sprc. to confirm Flash support or not. It is safty to run at 43M (Default).
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_SetCKG(SPI_DrvCKG eCKGspi);

//-------------------------------------------------------------------------------------------------
/// Description : Set clock div such that spi clock = mcu clock /clock_div.
/// @ingroup G_SERFLASH_CONTROL
/// @param  SPI_DrvClkDiv    \b IN: enumerate the clock_div
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note
//-------------------------------------------------------------------------------------------------
void MDrv_SERFLASH_ClkDiv(SPI_DrvClkDiv eClkDivspi);

//-------------------------------------------------------------------------------------------------
/// Description : Set XIU/RIU mode (Default : XIU)
/// @ingroup G_SERFLASH_CONTROL
/// @param  bXiuRiu    \b IN: 1 for XIU, 0 for RIU
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   XIU mode is faster than RIU mode. It is stable to run by XIU (Default)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_SetMode(MS_BOOL bXiuRiu);

//-------------------------------------------------------------------------------------------------
/// Description : Get Unique ID
/// @ingroup G_SERFLASH_CONTROL
/// @param  None
/// @return the SPI Flash's Unique ID
//-------------------------------------------------------------------------------------------------
MS_U64 MDrv_SERFLASH_ReadUID(void);

//-------------------------------------------------------------------------------------------------
/// Description :  Set active flash among multi-spi flashes
/// @ingroup G_SERFLASH_CONTROL
/// @param  u8FlashIndex    \b IN: The Flash index, 0 for external #1 spi flash, 1 for external #2 spi flash
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   For Secure booting = 0, please check hw_strapping or e-fuse  (the board needs to jump)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_ChipSelect(MS_U8 u8FlashIndex);

//-------------------------------------------------------------------------------------------------
/// Initialize Serial Flash
/// @ingroup G_INIT
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_SERFLASH_Init(void);

//-------------------------------------------------------------------------------------------------
/// Description : Read ID from Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  pu8FlashID    \b OUT: Virtual data ptr to store the read ID
/// @param  u32IDSize    \b IN: size in Bytes
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_ReadID(MS_U8 *pu8FlashID, MS_U32 u32IDSize);

//-------------------------------------------------------------------------------------------------
/// Description : Read data from Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  u32FlashAddr    \b IN: Flash Address
/// @param  u32FlashSize    \b IN: Flash Size Data in Bytes
/// @param  user_buffer    \b OUT: Virtual Buffer Address ptr to store flash read data
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_Read(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer);

//-------------------------------------------------------------------------------------------------
/// Description : Erase all sectors in Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_EraseChip(void);

//-------------------------------------------------------------------------------------------------
/// Description : Get flash start block index of a flash address
/// @ingroup G_SERFLASH_CONTROL
/// @param  u32FlashAddr    \b IN: flash address
/// @param  pu32BlockIndex    \b IN: poniter to store the returning block index
/// @return TRUE : succeed
/// @return FALSE : illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_AddressToBlock(MS_U32 u32FlashAddr, MS_U32 *pu32BlockIndex);

//-------------------------------------------------------------------------------------------------
/// Description : Get flash start address of a block index
/// @ingroup G_SERFLASH_CONTROL
/// @param  u32BlockIndex    \b IN: block index
/// @param  pu32FlashAddr    \b IN: pointer to store the returning flash address
/// @return TRUE : succeed
/// @return FALSE : illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_BlockToAddress(MS_U32 u32BlockIndex, MS_U32 *pu32FlashAddr);

//-------------------------------------------------------------------------------------------------
/// Description : Erase certain sectors given starting address and size in Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  u32StartAddr    \b IN: start address at block boundry
/// @param  u32EraseSize    \b IN: size to erase
/// @param  bWait    \b IN: wait write done or not
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_AddressErase(MS_U32 u32StartAddr, MS_U32 u32EraseSize, MS_BOOL bWait);

//-------------------------------------------------------------------------------------------------
/// Description : Erase certain sectors in Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  u32StartBlock    \b IN: start block
/// @param  u32EndBlock    \b IN: end block
/// @param  bWait    \b IN: wait write done or not
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_BlockErase(MS_U32 u32StartBlock, MS_U32 u32EndBlock, MS_BOOL bWait);

//-------------------------------------------------------------------------------------------------
/// Description : Erase certain 4K sectors in Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  u32StartBlock    \b IN: start address
/// @param  u32EndBlock    \b IN: end address
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_SectorErase(MS_U32 u32StartAddr, MS_U32 u32EndAddr);

//-------------------------------------------------------------------------------------------------
/// Description : Check write done in Serial Flash
/// @ingroup G_SERFLASH_COMMON
/// @return TRUE : done
/// @return FALSE : not done
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_CheckWriteDone(void);

//-------------------------------------------------------------------------------------------------
/// Description : Write data to Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  u32FlashAddr    \b IN: start address (4-B aligned)
/// @param  u32FlashSize    \b IN: size in Bytes (4-B aligned)
/// @param  user_buffer    \b IN: Virtual Buffer Address ptr to flash write data
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_Write(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer);

//-------------------------------------------------------------------------------------------------
/// Description : Read data from Serial Flash to DRAM in DMA mode
/// @ingroup G_SERFLASH_CONTROL
/// @param  u32FlashStart    \b IN: src start address in flash (0 ~ flash size-1)
/// @param  u32DRAMStart    \b IN: dst start address in DRAM (16B-aligned) (0 ~ DRAM size-1)
/// @param  u32Size    \b IN: size in Bytes (8B-aligned) (>=8)
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// @note
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_DMA(MS_U32 u32FlashStart, MS_U32 u32DRAMStart, MS_U32 u32Size);

//-------------------------------------------------------------------------------------------------
/// Description : Protect blocks in Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  bEnable    \b IN: TRUE/FALSE: enable/disable protection
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteProtect(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Description : Enables all range of flash write protection
/// @ingroup G_SERFLASH_ToBeRemove
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteProtect_Enable_All_Range(void);

//-------------------------------------------------------------------------------------------------
/// Description : Disables all range of flash write protection
/// @ingroup G_SERFLASH_ToBeRemove
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteProtect_Disable_All_Range(void);

//-------------------------------------------------------------------------------------------------
/// Description : Set flash disable lower bound and size
/// @ingroup G_SERFLASH_ToBeRemove
/// @param  u32DisableLowerBound    \b IN: the lower bound to disable write protect
/// @param  u32DisableSize    \b IN: size to disable write protect
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteProtect_Disable_Range_Set(MS_U32 u32DisableLowerBound, MS_U32 u32DisableSize);

//-------------------------------------------------------------------------------------------------
/// Description : Set flash enable lower bound and size
/// @ingroup G_SERFLASH_ToBeRemove
/// @param  u32DisableLowerBound    \b IN: the lower bound to disable write protect
/// @param  u32DisableSize    \b IN: size to disable write protect
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteProtect_Enable_Range_Set(MS_U32 u32LowerBound, MS_U32 u32Size);

//-------------------------------------------------------------------------------------------------
/// Description : Protect blocks in Serial Flash
/// @ingroup G_SERFLASH_ToBeRemove
/// @param  bEnableAllArea    \b IN: enable or disable protection
/// @param  u8BlockProtectBits    \b IN: block protection bits which stand for the area to enable write protect
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteProtect_Area(MS_BOOL bEnableAllArea, MS_U8 u8BlockProtectBits);

//------- ------------------------------------------------------------------------------------------
/// Description : Read Status Register in Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  pu8StatusReg    \b OUT: ptr to Status Register value
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_ReadStatusRegister(MS_U8 *pu8StatusReg);

//------- ------------------------------------------------------------------------------------------
/// Description : Read Status Register2 in Serial Flash
/// @ingroup G_SERFLASH_ToBeRemove
/// @param  pu8StatusReg    \b OUT: ptr to Status Register value
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note   For Specific Flash IC with 16-bit status register (high-byte)
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_ReadStatusRegister2(MS_U8 *pu8StatusReg);

//------- ------------------------------------------------------------------------------------------
/// Description : Write Status Register in Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  u16StatusReg    \b IN: Status Register value
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note   For Specific Flash IC with 16-bit status register
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteStatusRegister(MS_U16 u16StatusReg);

//------- ------------------------------------------------------------------------------------------
/// Description : Handle for BDMA copy data from ONLY Flash src to other dst
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32FlashAddr \b IN: Physical Source address in spi flash
/// @param u32DramAddr \b IN: Physical Dst address
/// @param u32Len \b IN: data length
/// @param eDstDev \b IN: The Dst Device of Flash BDMA
/// @param u8OpCfg \b IN: u8OpCfg: default is SPIDMA_OPCFG_DEF
/// - Bit0: inverse mode --> SPIDMA_OPCFG_INV_COPY
/// - Bit2: Copy & CRC check in wait mode --> SPIDMA_OPCFG_CRC_COPY
/// - Bit3: Copy without waiting --> SPIDMA_OPCFG_NOWAIT_COPY
/// @return \b MS_BOOL
/// [NONOS_SUPPORT]
/// [fw : drvBDMA ]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_CopyHnd(MS_PHYADDR u32FlashAddr, MS_PHYADDR u32DstAddr, MS_U32 u32Len, SPIDMA_Dev eDstDev, MS_U8 u8OpCfg);

//------- ------------------------------------------------------------------------------------------
/// Description : Switch SPI as GPIO Input
/// @ingroup G_SERFLASH_CONTROL
/// @param  bSwitch    \b IN: 1 for GPIO, 0 for NORMAL
/// @note   Not allowed in interrupt context
/// @note   For project's power consumption
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
void MDrv_SERFLASH_SetGPIO(MS_BOOL bSwitch);

//------- ------------------------------------------------------------------------------------------
/// Description : Change Kernel or User Mode in Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  SpiAttri
/// @return TRUE : succeed
/// @return FALSE : fail not suppported enum
/// @note   No
/// @note   Use Kernel Enum to Kernel Space
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_Attribute(SPI_Attribute SpiAttri);

//-------------------------------------------------------------------------------------------------
/// Initialize Variety Of Serial Flash
/// @ingroup G_INIT
/// @param eFlashHost \b IN: FlashId 0:E_FLASH_FSP 1:E_FLASH_QSPI
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_SERFLASH_Init_Channel(EN_FLASH_HOST eFlashHost);

//-------------------------------------------------------------------------------------------------
/// Description : Get flash start block index of a flash address
/// @param  eFlashHost \b IN: FlashId 0:E_FLASH_FSP 1:E_FLASH_QSPI
/// @param  u32FlashAddr    \b IN: flash address
/// @param  pu32BlockIndex    \b IN: poniter to store the returning block index
/// @return TRUE : succeed
/// @return FALSE : illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_AddressToBlock_Channel(EN_FLASH_HOST eFlashHost,MS_U32 u32FlashAddr, MS_U32 *pu32BlockIndex);

//-------------------------------------------------------------------------------------------------
/// Description : Get flash start address of a block index
/// @param  eFlashHost \b IN: FlashId 0:E_FLASH_FSP 1:E_FLASH_QSPI
/// @param  u32BlockIndex    \b IN: block index
/// @param  pu32FlashAddr    \b IN: pointer to store the returning flash address
/// @return TRUE : succeed
/// @return FALSE : illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_BlockToAddress_Channel(EN_FLASH_HOST eFlashHost,MS_U32 u32BlockIndex, MS_U32 *pu32FlashAddr);

//-------------------------------------------------------------------------------------------------
/// Description : Erase certain sectors given starting address and size in Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  eFlashHost \b IN: FlashId 0:E_FLASH_FSP 1:E_FLASH_QSPI
/// @param  u32StartAddr    \b IN: start address at block boundry
/// @param  u32EraseSize    \b IN: size to erase
/// @param  bWait    \b IN: wait write done or not
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_AddressErase_Channel(EN_FLASH_HOST eFlashHost,MS_U32 u32StartAddr, MS_U32 u32EraseSize, MS_BOOL bWait);

//-------------------------------------------------------------------------------------------------
/// Description : Erase certain sectors in Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  eFlashHost \b IN: FlashId 0:E_FLASH_FSP 1:E_FLASH_QSPI
/// @param  u32StartBlock    \b IN: start block
/// @param  u32EndBlock    \b IN: end block
/// @param  bWait    \b IN: wait write done or not
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_BlockErase_Channel(EN_FLASH_HOST eFlashHost,MS_U32 u32StartBlock, MS_U32 u32EndBlock, MS_BOOL bWait);

//-------------------------------------------------------------------------------------------------
/// Description : Erase certain 4K sectors in Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  eFlashHost \b IN: FlashId 0:E_FLASH_FSP 1:E_FLASH_QSPI
/// @param  u32StartBlock    \b IN: start address
/// @param  u32EndBlock    \b IN: end address
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_SectorErase_Channel(EN_FLASH_HOST eFlashHost,MS_U32 u32StartAddr, MS_U32 u32EndAddr);

//-------------------------------------------------------------------------------------------------
/// Description : Write data to Serial Flash
/// @param  eFlashHost \b IN: FlashId 0:E_FLASH_FSP 1:E_FLASH_QSPI
/// @param  u32FlashAddr    \b IN: start address (4-B aligned)
/// @param  u32FlashSize    \b IN: size in Bytes (4-B aligned)
/// @param  user_buffer    \b IN: Virtual Buffer Address ptr to flash write data
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_Write_Channel(EN_FLASH_HOST eFlashHost,MS_U32 u32FlashAddr,MS_U32 u32FlashSize,MS_U8 * user_buffer);


//-------------------------------------------------------------------------------------------------
/// Description : Read data from Serial Flash
/// @param  eFlashHost \b IN: FlashId 0:E_FLASH_FSP 1:E_FLASH_QSPI
/// @param  u32FlashAddr    \b IN: Flash Address
/// @param  u32FlashSize    \b IN: Flash Size Data in Bytes
/// @param  user_buffer    \b OUT: Virtual Buffer Address ptr to store flash read data
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_Read_Channel(EN_FLASH_HOST eFlashHost,MS_U32 u32FlashAddr,MS_U32 u32FlashSize,MS_U8 * user_buffer);

//-------------------------------------------------------------------------------------------------
/// Description : Protect blocks in Serial Flash
/// @param  eFlashHost \b IN: FlashId 0:E_FLASH_FSP 1:E_FLASH_QSPI
/// @param  bEnable    \b IN: TRUE/FALSE: enable/disable protection
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteProtect_Channel(EN_FLASH_HOST eFlashHost,MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Description : Set ckg_spi which flash supports (please ref. the spec. before using this function)
/// @ingroup G_SERFLASH_CONTROL
/// @param  eFlashHost \b IN: FlashId 0:E_FLASH_FSP 1:E_FLASH_QSPI
/// @param  SPI_DrvCKG    \b IN: enumerate the ckg_spi
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   Please ref. sprc. to confirm Flash support or not. It is safty to run at 43M (Default).
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_SetCKG_Channel(EN_FLASH_HOST eFlashHost,SPI_DrvCKG eCKGspi);


//------- ------------------------------------------------------------------------------------------
/// Description : Read Status Register in Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  eFlashHost \b IN: FlashId 0:E_FLASH_FSP 1:E_FLASH_QSPI
/// @param  pu8StatusReg    \b OUT: ptr to Status Register value
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_ReadStatusRegister_Channel(EN_FLASH_HOST eFlashHost,MS_U8 *pu8StatusReg);


//------- ------------------------------------------------------------------------------------------
/// Description : Write Status Register in Serial Flash
/// @ingroup G_SERFLASH_CONTROL
/// @param  eFlashHost \b IN: FlashId 0:E_FLASH_FSP 1:E_FLASH_QSPI
/// @param  u16StatusReg    \b IN: Status Register value
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note   For Specific Flash IC with 16-bit status register
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteStatusRegister_Channel(EN_FLASH_HOST eFlashHost,MS_U16 u16StatusReg);

//-------------------------------------------------------------------------------------------------
/// Description :  Set active flash among multi-spi flashes
/// @param  eFlashHost \b IN: FlashId 0:E_FLASH_FSP 1:E_FLASH_QSPI
/// @param  u8FlashIndex    \b IN: The Flash index, 0 for external #1 spi flash, 1 for external #2 spi flash
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   For Secure booting = 0, please check hw_strapping or e-fuse (the board needs to jump)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_ChipSelect_Channel(EN_FLASH_HOST eFlashHost,MS_U8 u8FlashIndex);


//-------------------------------------------------------------------------------------------------
//  WRAPPER FOR CHAKRA
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteProtect_Area_Lookup(MS_U32 u32ProtectSpace, MS_U32 u32NonProtectSpace, MS_U8 *pu8BlockProtectBits);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_SERFLASH_WriteProtect_Area_Boundary(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_Write(MS_U32 u32FlashAddr,MS_U32 u32FlashSize,MS_U8 * user_buffer);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_Read(MS_U32 u32FlashAddr,MS_U32 u32FlashSize,MS_U8 * user_buffer);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_WriteProtect(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_WriteProtect_Enable_All_Range(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_WriteProtect_Disable_All_Range(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_WriteProtect_Disable_Range_Set(MS_U32 DisableLowerBound, MS_U32 DisableSize);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_WriteProtect_Area(MS_BOOL bEnableAllArea, MS_U8 BlockProtectBits);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_ReadStatusRegister(MS_U8 *pu8StatusReg);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_ReadStatusRegister2(MS_U8 *pu8StatusReg);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_WriteStatusRegister(MS_U16 u16StatusReg);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_DetectType(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_DetectSize(MS_U32 *u32FlashSize);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_AddressToBlock(MS_U32 u32FlashAddr, MS_U32 *pu32BlockIndex);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_BlockToAddress(MS_U32 u32BlockIndex, MS_U32 *pu32FlashAddr);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_AddressErase(MS_U32 u32StartAddr,MS_U32 u32EraseSize,MS_BOOL bWait);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_BlockErase(MS_U16 u16StartBlock,MS_U16 u16EndBlock,MS_BOOL bWait);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_CheckWriteDone(void);

#ifdef MXIC_IBPM
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_SERFLASH_EnterIBPM(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_SERFLASH_ReadBlockStatus(MS_PHYADDR u32FlashAddr);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_SingleBlockProtect(MS_PHYADDR u32FlashAddr,MS_BOOL bLock);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_SERFLASH_MultiBlockProtect(MS_U32 u32StartBlock, MS_U32 u32EndBlock, MS_BOOL bLock);
#endif

#ifdef MUNINN_ENABLE
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_OTP_ReadStatusReg(MS_U8 u8ByteAddr);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_OTP_WriteStatusReg(MS_U8 u8ByteAddr, MS_U8 u8ByteReg);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_PHYADDR MDrv_OTP_Read(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_PHYADDR MDrv_OTP_FastRead(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_PHYADDR MDrv_OTP_Write(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_OTP_WriteTestMode(MS_U32 u32FlashAddr, MS_U8 ByteData);
#endif

// FSP
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_FSP
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FSP_WriteData(MS_U32 u32Addr, MS_U32 u32DataSize, MS_U8 *pu8Data);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_FSP
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FSP_ReadData(MS_U32 u32Addr, MS_U32 u32DataSize, MS_U8 *pu8Data);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_FSP
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FSP_BlockErase(MS_U32 u32StartBlock, MS_U32 u32EndBlock, MS_BOOL bWait);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_FSP
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FSP_AddressErase(MS_U32 u32StartAddr, MS_U32 u32EraseSize, MS_BOOL bWait);

//power state
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SERFLASH_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_SERFLASH_SetPowerState(EN_POWER_MODE u16PowerState);

//-------------------------------------------------------------------------------------------------
//  Flash OTP USAGE
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Description : Get Protection Info
/// @param  Protection_Type,Protection Size ,Protection Info
/// @return TRUE : succeed
/// @return FALSE : failed
//------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteProtect_TableInfo(SERFLASH_RegionMapType type, void  *pstTable);

//------------------------------------------------------------------------------
/// Description : OTP Read
/// @param  OTP address, OTP Size , OTP read data buffer
/// @return TRUE : succeed
/// @return FALSE : failed
//------------------------------------------------------------------------------
MS_BOOL  MDrv_FLASH_OTP_Read(MS_U32 u32FlashAddr, MS_U16 u16FlashSize, MS_U8 *user_buffer);

//------------------------------------------------------------------------------
/// Description : OTP Write
/// @param  OTP address, OTP Size , OTP read data buffer
/// @return TRUE : succeed
/// @return FALSE : failed
//------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_OTP_Write(MS_U32 u32FlashAddr, MS_U16 u16FlashSize, MS_U8 *user_buffer);

//------------------------------------------------------------------------------
/// Description : OTP Erase
/// @param  OTP address
/// @return TRUE : succeed
/// @return FALSE : failed
//------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_OTP_ERASE(MS_U32  u32Addr);

extern ms_Mcu_ChipSelect_CB McuChipSelectCB;
extern ms_Flash_SetHWWP_CB FlashSetHWWPCB;
#ifdef __cplusplus
}
#endif

#endif // _DRV_SERFLASH_H_
