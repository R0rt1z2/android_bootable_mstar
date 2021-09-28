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

/*! \defgroup G_LD Local dimming interface
    \ingroup  G_PERIPHERAL
   
    \brief 
    
    Local dimming is introduced to achieve power saving and higher image contrast for LED backlight. 


    <b>Features</b>

    - Less backlight, less energy
    - Compensation restores image luminance
    - Average 10~30% saving
    - Dark region becomes even darker
    - Light leakage problem

    <b> LD Block Diagram: </b> \n
    \image html apiLD_pic.png

    <b> Operation Code Flow: </b> \n
    -# mboot init, local dimming ready
    -# kernel run algorithm
    -# pm51 read led data from sharememory
    -# pm51 send luminance data to led via spi
    \image html apiLD_pic2.png

    \defgroup LD_Init LD Init API
    \ingroup LD
    \defgroup LD_Debug LD Debug API
    \ingroup LD
    \defgroup LD_to_be_removed LD_to_be_removed (to be removed)
    \ingroup LD
*/

#ifndef _API_XC_LD_H_
#define _API_XC_LD_H_

//#include "MsCommon.h"
//#include "MsDevice.h"
#include "MsTypes.h"

#ifdef __cplusplus
extern "C" {
#endif


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define LD_LOG_DBG     1

// Debug related
#if LD_LOG_DBG
#define LD_DBG(_fmt, _args...)      {printf("LD:"_fmt, ##_args); }
#define LD_ASSERT(_fmt, _args...)  {printf("LD ASSERT: %s %d:"_fmt, __FILE__, __LINE__, ##_args); }
#else
#define LD_DBG(_fmt, _args...)      { }
#define LD_ASSERT(_fmt, _args...)  {printf("LD ASSERT: %s %d:"_fmt, __FILE__, __LINE__, ##_args); } 
#endif


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// Define PNL local dimming return type
typedef enum
{
    E_APILD_STATUS_SUCCESS = 0,
    E_APILD_STATUS_FAIL = 1,
    E_APILD_STATUS_NOT_SUPPORTED,
    E_APILD_STATUS_PARAMETER_ERROR,
    E_APILD_OBTAIN_MUTEX_FAIL,            ///< obtain mutex timeout when calling this function
    E_APILD_OBTAIN_MMIO_FAIL,            ///< obtain mutex timeout when calling this function
} E_LD_RETURN;

typedef enum
{
    E_LD_LED_EDGE_TB_TYPE = 0, // deprecated
    E_LD_LED_EDGE_LR_TYPE = 1, // deprecated
    E_LD_LED_DIRECT_TYPE  = 2,
    E_LD_LED_LOCAL_TYPE   = 3,  // new mode supporting 32x32 LED
    E_LD_LED_TYPE_NUM,
    E_LD_LED_TYPE_MAX = E_LD_LED_TYPE_NUM,
} E_LD_TYPE;

typedef struct _stLD_Init_Parameters
{
    E_LD_TYPE ePnlLDType;
    MS_U8* pu8RawData;
    MS_U16 u16DataSize;
    MS_U16 u16PnlWidth;
    MS_U16 u16PnlHeight;
    MS_U16 u16MemSize;
    MS_PHYADDR u32MemAdrress;
    MS_U32 u32Reserved;
} stLD_Init_Parameters, *pstLD_Init_Parameters;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//--------------


//-------------------------------------------------------------------------------------------------
/// init local dimming, set base registers, load table etc
/// @return MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_LD_Init(pstLD_Init_Parameters pstLDArgs, MS_U16 u16DataLen);



//-------------------------------------------------------------------------------------------------
/// set debug level
/// @return MS_U16
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LD_SetDbgLevel(MS_U16 u16DbgSwitch);


#ifdef __cplusplus
}
#endif

#endif
