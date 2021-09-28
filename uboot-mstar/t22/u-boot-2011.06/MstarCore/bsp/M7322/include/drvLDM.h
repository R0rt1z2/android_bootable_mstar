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
/// @brief  local dimming Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_LDM Local dimming interface
    \ingroup  G_PERIPHERAL

    \brief

    Local dimming is introduced to achieve power saving and higher image contrast for LED backlight.


    <b>Features</b>

    - Less backlight, less energy
    - Compensation restores image luminance
    - Average 10~30% saving
    - Dark region becomes even darker
    - Light leakage problem

    <b> LDM Block Diagram: </b> \n
    \image html apiLD_pic.png

    <b> Operation Code Flow: </b> \n
    -# AP init, local dimming ready
    -# kernel run algorithm
    -# pm51 read led data from sharememory
    -# pm51 send luminance data to led via spi
    \image html apiLD_pic2.png

    \defgroup LDM_Init LDM Init API
    \ingroup LDM
    \defgroup LDM_Debug LDM Debug API
    \ingroup LDM
    \defgroup G_LDM_COMMON Common Task relative
    \ingroup  G_LDM
    \defgroup G_LDM_CONTROL Control relative
    \ingroup  G_LDM

*/


#ifndef _DRV_LDM_H_
#define _DRV_LDM_H_


#ifdef __cplusplus
extern "C"
{
#endif


#ifdef MDRV_LDM_C
#define MDRV_INTERFACE
#else
#define MDRV_INTERFACE extern
#endif

// use 'L' as magic number
#define MDRV_LDM_IOC_MAGIC                  ('L')
#define MDRV_LDM_IOC_INIT                   _IO(MDRV_LDM_IOC_MAGIC, 0)
#define MDRV_LDM_IOC_DEINIT                 _IO(MDRV_LDM_IOC_MAGIC, 1)
#define MDRV_LDM_IOC_ENABLE                 _IOW(MDRV_LDM_IOC_MAGIC, 2, MS_U32)
#define MDRV_LDM_IOC_DISABLE                _IOW(MDRV_LDM_IOC_MAGIC, 3, MS_U32)
#define MDRV_LDM_IOC_SUSPEND                _IOW(MDRV_LDM_IOC_MAGIC, 4, MS_U32)
#define MDRV_LDM_IOC_RESUME                 _IOW(MDRV_LDM_IOC_MAGIC, 5, MS_U32)
#define MDRV_LDM_IOC_SET_STRENGTH           _IOW(MDRV_LDM_IOC_MAGIC, 6, MS_U32)
#define MDRV_LDM_IOC_GET_DATA               _IOR(MDRV_LDM_IOC_MAGIC, 7, MS_U32)
#define MDRV_LDM_IOC_DEBUG                  _IOW(MDRV_LDM_IOC_MAGIC, 8, MS_U32)
#define MDRV_LDM_IOC_CUS_PATH               _IOW(MDRV_LDM_IOC_MAGIC, 9, MS_U32)
#define MDRV_LDM_IOC_DEMO_PATTERN           _IOW(MDRV_LDM_IOC_MAGIC, 10, MS_U32)
#define MDRV_LDM_IOC_DUMP_DRAM              _IOW(MDRV_LDM_IOC_MAGIC, 11, MS_U32)
#define MDRV_LDM_IOC_SET_LOCALSTRENGTH      _IOW(MDRV_LDM_IOC_MAGIC, 12, MS_U32)
#define MDRV_LDM_IOC_UCD_ENABLE             _IOW(MDRV_LDM_IOC_MAGIC, 13, MS_U32)
#define MDRV_LDM_IOC_UCD_DISABLE            _IOW(MDRV_LDM_IOC_MAGIC, 14, MS_U32)
#define MDRV_LDM_IOC_UCD_CURVE              _IOW(MDRV_LDM_IOC_MAGIC, 15, ST_LDM_UCD_CURVE)

#define MDRV_LDM_IOC_MAX_NR                 (16)


typedef enum
{
    E_LDM_STATUS_INIT = 1,
    E_LDM_STATUS_ENABLE = 2,
    E_LDM_STATUS_DISNABLE,
    E_LDM_STATUS_SUSPEND,
    E_LDM_STATUS_RESUME,
    E_LDM_STATUS_DEINIT,
} EN_LDM_STATUS;

typedef enum
{
    E_LDM_RET_SUCCESS = 0,
    E_LDM_RET_FAIL = 1,
    E_LDM_RET_NOT_SUPPORTED,
    E_LDM_RET_PARAMETER_ERROR,
    E_LDM_RET_OBTAIN_MUTEX_FAIL,            ///< obtain mutex timeout when calling this function
} EN_LDM_RETURN;

typedef enum
{
    E_LDM_DEBUG_LEVEL_ERROR=0x01,
    E_LDM_DEBUG_LEVEL_WARNING=0x02,
    E_LDM_DEBUG_LEVEL_INFO=0x04,
    E_LDM_DEBUG_LEVEL_ALL=0x07,
    E_LDM_DEBUG_LEVEL_MAX
}EN_LDM_DEBUG_LEVEL;

typedef enum
{
    E_LDM_DATA_TYPE_LDF=0x01,
    E_LDM_DATA_TYPE_LDB=0x02,
    E_LDM_DATA_TYPE_SPI=0x03,
    E_LDM_DATA_TYPE_MAX
}EN_LDM_GET_DATA_TYPE;

typedef enum
{
    E_LDM_DEMO_PATTERN_SWITCH_SINGLE_LED=0x00,
    E_LDM_DEMO_PATTERN_LEFT_RIGHT_HALF=0x01,
    E_LDM_DEMO_PATTERN_MARQUEE=0x02,
    E_LDM_DEMO_PATTERN_LEFT_RIGHT_COLOR_SHELTER=0x03,
    E_LDM_DEMO_PATTERN_MAX
}EN_LDM_DEMO_PATTERN;


typedef struct __attribute__((packed))
{
    MS_U8 aCusPath[64];
    MS_U8 aCusPathU[64];
}ST_LDM_CONF_PATH;

typedef struct __attribute__((packed))
{
    EN_LDM_GET_DATA_TYPE enDataType;
    MS_PHY phyAddr;
}ST_LDM_GET_DATA;

typedef struct __attribute__((packed))
{
    MS_BOOL bOn;
    EN_LDM_DEMO_PATTERN enDemoPattern;
    MS_U16 u16LEDNum;
}ST_LDM_DEMO_PATTERN;

typedef struct __attribute__((packed))
{
    MS_U32 u32CommandIndex;
    void *pContent;
}ST_LDM_CMD_INFO;
typedef struct __attribute__((packed))
{
    MS_U64 u64pu8AnchorVal;
    MS_U16 u16AnchorCount;
    MS_U8 u8Index;
}ST_LDM_UCD_CURVE;

typedef struct
{
    //for LDF addr
    MS_U32 u32LDFAddr0_l;   //double buffer0
    MS_U32 u32LDFAddr0_r;
    MS_U32 u32LDFAddr1_l;   //double buffer1
    MS_U32 u32LDFAddr1_r;
    //for LDB addr
    MS_U32 u32LDBAddr0_l;   //double buffer0
    MS_U32 u32LDBAddr0_r;
    MS_U32 u32LDBAddr1_l;   //double buffer1
    MS_U32 u32LDBAddr1_r;
    //for edge2D addr
    MS_U32 u32Edge2DAddr;
    //for spi data mem base
    MS_U32 u32DataOffset;
}__attribute__((packed))ST_LDM_CFG_MMAP_ADDR;

//----------------------------------------------------------------
// MDrv_LDM_SetDbgLevel - Set debug level
/// @ingroup G_LDM_CONTROL
// @param: eLevel: debug level
// @return: E_LDM_STATUS_SUCCESS is setted
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_SetDbgLevel(EN_LDM_DEBUG_LEVEL eLevel);

//----------------------------------------------------------------
// MDrv_LDM_GetData - get local dimming status
/// @ingroup G_LDM_CONTROL
// @param: void
// @return: EN_LDM_STATUS
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_GetStatus(void);
//----------------------------------------------------------------
// MDrv_LDM_SetConfPath - Set config files path
/// @ingroup G_LDM_CONTROL
// @param: stPath: config files path in board or u-disk
// @return: E_LDM_STATUS_SUCCESS is setted
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_SetConfPath(ST_LDM_CONF_PATH stPath);

//----------------------------------------------------------------
// MDrv_LDM_Init - Set  mmap address to register base
/// @ingroup G_LDM_CONTROL
// @param: phyAddr: local dimming mmap address in mmap.ini
// @return: E_LDM_RET_SUCCESS is inital
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_Init(MS_PHY phyAddr);

//----------------------------------------------------------------
// MDrv_LDM_Enable - start local dimming algorithm
/// @ingroup G_LDM_CONTROL
// @param: na
// @return: E_LDM_RET_SUCCESS is enable
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_Enable(void);

//----------------------------------------------------------------
// MDrv_LDM_Disable - stop local dimming algorithm, send constant luminance  to led
/// @ingroup G_LDM_CONTROL
// @param: u8Lumma: constant luminance range from 00 to 255
// @return: E_LDM_RET_SUCCESS is disable
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_Disable(MS_U8 u8Lumma);

//----------------------------------------------------------------
// MDrv_LDM_GetData - get LDF/LDB/SPI data pre frame in buffer
/// @ingroup G_LDM_CONTROL
// @param: stData:  data type and mmap address filled with the requied type
// @return: E_LDM_RET_SUCCESS is getted
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_GetData(ST_LDM_GET_DATA *stData);

//----------------------------------------------------------------
// MDrv_LDM_SetStrength - Set back light percent
/// @ingroup G_LDM_CONTROL
// @param: u8Percent: the percent ranged from 0 to 100
// @return: E_LDM_RET_SUCCESS is setted
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_SetStrength(MS_U8 u8Percent);

//----------------------------------------------------------------
// MDrv_LDM_Suspend -go to sleep
/// @ingroup G_LDM_CONTROL
// @param: na
// @return: E_LDM_RET_SUCCESS is suspend
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_Suspend(void);

//----------------------------------------------------------------
// MDrv_LDM_Resume - wake up from sleeping
/// @ingroup G_LDM_CONTROL
// @param: na
// @return: E_LDM_RET_SUCCESS is resumed
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_Resume(void);

//----------------------------------------------------------------
// MDrv_LDM_DemoPattern - demo pattern from customer
/// @ingroup G_LDM_CONTROL
// @param: stPattern: demo type: turn on led, left-right half show
// @return: E_LDM_RET_SUCCESS is demonstrative
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_DemoPattern(ST_LDM_DEMO_PATTERN stPattern);

//----------------------------------------------------------------
// MDrv_LDM_DumpDRAM - Print LDF/LDB/SPI data
/// @ingroup G_LDM_CONTROL
// @param: eType: LDF/LDB/SPI
// @return: E_LDM_RET_SUCCESS is dumped
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_DumpDRAM(EN_LDM_GET_DATA_TYPE eType);

//----------------------------------------------------------------
// MDrv_LDM_Deinit - stop local dimmg, free resource owned
/// @ingroup G_LDM_CONTROL
// @param: na
// @return: E_LDM_RET_SUCCESS is deinit
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_Deinit(void);

//----------------------------------------------------------------
// MDrv_LDM_SendCommads - the api to reserve
/// @ingroup G_LDM_CONTROL
// @param: pstCMD: pointer to command
// @param: u8Number: number of command
// @return: E_LDM_RET_SUCCESS is sent
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_SendCommads(ST_LDM_CMD_INFO* pstCMD,MS_U8 u8Number);

//-------------------------------------------------------------------------------------------------
/// Description : set LDF base address
/// @param u8ClkHz \b IN:  back light period 60HZ or 120HZ
/// @return TRUE : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetIOMapBase(MS_U8 u8ClkHz);
//-------------------------------------------------------------------------------------------------
/// Description : set LDF base address
/// @param stLdmCfgAddr \b IN:  doublebuffer for  LDF index
/// @return TRUE : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_Config_MmapAddr(ST_LDM_CFG_MMAP_ADDR *stLdmCfgAddr);
//-------------------------------------------------------------------------------------------------
/// Description : Set MIU PackOffset & Pack length [DMA load mem offset data ,ps:always offset=0]
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8PackOffset \b IN: u8PackOffset
/// @param u8PackLength \b IN: DMA load mem data pack format size
/// @return TRUE : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetMIUPackFormat(MS_U8 u8HWNum,MS_U8 u8PackOffset,MS_U8 u8PackLength);
//-------------------------------------------------------------------------------------------------
/// Description : Set YoffEnd
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8YoffEnd \b IN: u8YoffEnd always =led height
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetYoffEnd(MS_U8 u8HWNum,MS_U8 u8YoffEnd);
//-------------------------------------------------------------------------------------------------
/// Description : Set Back light height number
/// @param u8Height \b IN: LDMA Height = led Height number
/// @param u8Height \b IN: LDMA Width = led Width number
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetBlLdmaSize(MS_U8 u8Height,MS_U8 u8Width);
//-------------------------------------------------------------------------------------------------
/// Description : Enable ldma for LD
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param bEnable \b IN: set DMA for local dimming enable or disable
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetDmaEnable(MS_U8 u8HWNum, MS_BOOL bEnable);
//----------------------------------------------------------------
// MDrv_LDM_SetUcdEnable
// @ingroup G_LDM_CONTROL
// @param: bEnable Enable/Disable Ultra Contrast Dimming
// @return: E_LDM_RET_SUCCESS is setted
//----------------------------------------------------------------
MS_U8 MDrv_LDM_SetUcdEnable(MS_BOOL bEnable);
//----------------------------------------------------------------
// MDrv_LDM_SetUcdCurve
// @ingroup G_LDM_CONTROL
// @param: u8CurveIndex Ultra Contrast Dimming Curve index
// @param: pu8AnchorVal pointer to Ultra Contrast Dimming Anchor value of Curve[index]
// @param: u16AnchorCount Ultra Contrast Dimming Anchor count of Curve[index]
// @return: E_LDM_RET_SUCCESS is setted
//----------------------------------------------------------------
MS_U8 MDrv_LDM_SetUcdCurve(MS_U8 u8CurveIndex,MS_U8 *pu8AnchorVal,MS_U16 u16AnchorCount);
#ifdef __cplusplus
}
#endif

#endif // _DRV_LDM_H_
