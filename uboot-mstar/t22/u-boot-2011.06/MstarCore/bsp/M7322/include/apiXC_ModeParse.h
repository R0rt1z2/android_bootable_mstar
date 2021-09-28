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
/// @brief MIU header file
///
////////////////////////////////////////////////////////////////////////////////

/*! \defgroup Video Video modules

 *  \defgroup XC_FE XC_FE modules
 *  \ingroup Video

	Mode Parse mechanism is a fast and easy way to look up input timing according to different timing characteristics.\n
	For more information,see \link MODE_Parse MODE_Parse interface (apiXC_ModeParse.h) \endlink

 *  \defgroup MODE_Parse MODE_Parse interface (apiXC_ModeParse.h)
 *  \ingroup XC_FE

    Mode Parse mechanism is a fast and easy way to look up input timing according to
    different timing characteristics listed here
    - input source type : Component / HDMI / DVI / VGA
    - input horizontal frequency
    - input vertical frequency
    - input vertical total
    - input sync detection : H/V polarity, scan type
    - HDMI/DVI only : Data Enable section detection result
    - Euro HDTV detection result

    <b>Features</b> \n
    - Customizable : User can customized their database by passing their own data base and the driver will help to do the matching tasks.
    - Supports multiple input source types : Component / HDMI / DVI / VGA

    <b> Operation Code Flow: </b> \n
    1. calls MApi_XC_ModeParse_Init for internal initilizations
    2. prepare MS_PCADC_MODETABLE_TYPE_EX structure type timing database.
    3. User switch input source to whatever the system is going to show. For example : Main or Sub window
    4. read sync detection info from scaler and stores it in XC_MODEPARSE_INPUT_INFO structure type
    5. calls MApi_XC_ModeParse_MatchModeEx with timing database and the information you just got from scaler
    6. wait for the result of matching timing.

 *  \defgroup MODE_Parse_INIT MODE_Parse init control
 *  \ingroup MODE_Parse

 *! \defgroup MODE_Parse_FEATURE MODE_Parse feature operation
 *  \ingroup MODE_Parse

 *! \defgroup MODE_Parse_INFO MODE_Parse Infomation pool
 *  \ingroup MODE_Parse

 *! \defgroup MODE_Parse_ToBeModified MODE_Parse api to be modified
 *  \ingroup MODE_Parse

 *! \defgroup MODE_Parse_ToBeRemove MODE_Parse api to be removed
 *  \ingroup MODE_Parse
 */


#ifndef _API_XC_MODEPARSE_H_
#define _API_XC_MODEPARSE_H_
#ifdef __cplusplus
extern "C"
{
#endif
#include "UFO.h"
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//------------------------------
// Mode Detect & Search
//------------------------------
#define MD_FLAG_NULL            0x00

#define MD_FLAG_POR_HPVP        0x01
#define MD_FLAG_POR_HPVN        0x02
#define MD_FLAG_POR_HNVP        0x04
#define MD_FLAG_POR_HNVN        0x08
#define MD_FLAG_INTERLACE       0x10

#define MD_FLAG_EURO_HDTV_BIT   0x20

#define MD_FLAG_YPBPR_BIT       0x40
#define MD_FLAG_HDTV_BIT        0x80


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// The type of result after mode searching.
typedef enum
{
    XC_MODEPARSE_NOT_PARSED = 0,        ///< timing not stable or stable no sync, not parse yet
    XC_MODEPARSE_OUT_OF_RANGE,          ///< out of spec.
    XC_MODEPARSE_UNSUPPORT_MODE,        ///< can't find in mode tables
    XC_MODEPARSE_SUPPORT_MODE,          ///< support mode
} XC_MODEPARSE_RESULT;

typedef struct DLL_PACKED
{
    // mode parse result
    MS_U8  u8ModeIndex;                    ///< OUT: mode parse will return the mode index by this variable
    MS_U16 u16ResWidth;                    ///< OUT: indicate the resolution -> width
    MS_U16 u16ResHeight;                   ///< OUT: indicate the resolution -> height

    // src type
    INPUT_SOURCE_TYPE_t eSrcType;       ///< IN: input source type

    // Analog
    MS_U16 u16HFreqx10;                    ///< IN: HFreq x 10, unit KHz after / 10
    MS_U16 u16VFreqx10;                    ///< IN: VFreq x 10, unit Hz after / 10
    MS_U16 u16Vtotal;                      ///< IN: Vertical total
    MS_U8  u8SyncStatus;                   ///< IN: from XC driver, including H/V sync polarity, interlaced or progressive, mode parse will or XC_MD_USER_MODE_BIT if new mode found

    // Digital
    MS_WINDOW_TYPE sDVI_HDMI_DE_Info;   ///< IN: DVI/HDMI input DE information

    // EuroHDTV detected or not
    MS_DETECT_EURO_HDTV_STATUS_TYPE eEuroHDTVStatus;    ///< IN: timing monitor detect Euro/Aus HDTV status
} XC_MODEPARSE_INPUT_INFO;  ///< the neccesary input timing information for mode search

typedef enum
{
    RES_640X350,     // 00
    RES_640X400,     // 01
    RES_720X400,     // 02
    RES_640X480,     // 03
    RES_800X600,     // 04
    RES_832X624,     // 05
    RES_1024X768,    // 06
    RES_1280X1024,   // 07
    RES_1600X1200,   // 08
    RES_1152X864,    // 09
    RES_1152X870,    // 10
    RES_1280X768,    // 11
    RES_1280X960,    // 12
    RES_720X480,     // 13
    RES_1920X1080,   // 14
    RES_1280X720,    // 15
    RES_720X576,     // 16
    RES_1920X1200,   // 17
    RES_1400X1050,   // 18
    RES_1440X900,    // 19
    RES_1680X1050,   // 20
    RES_1280X800,    // 21
    RES_1600X1024,   // 22
    RES_1600X900,    // 23
    RES_1360X768,    // 24
    RES_848X480,     // 25
    RES_1920X1080P,  // 26
    RES_1366X768,    // 27
    RES_864X648,     // 28
    RES_1280X1470,   // 29
    RES_1920X2205,   // 30
    RES_720X240,     // 31
    RES_720x288,     // 32
    RES_1152X900,    // 33
    RES_856x480,     // 34
    RES_1536X960,    // 35
    RES_1600X1000,   // 36
    RES_1704X960,    // 37
    RES_1728X1080,   // 38
    RES_1864X1050,   // 39
    RES_MAXIMUM
}EN_RESOLUTION_TYPE;

typedef struct DLL_PACKED
{
    EN_RESOLUTION_TYPE enResolutionIndex;   ///< resolution

    MS_U16 u16HorizontalFrequency;             ///< Horizontal frequency
    MS_U16 u16VerticalFrequency;               ///< Vertical frequency

    MS_U16 u16HorizontalStart;                 ///< Horizontal start
    MS_U16 u16VerticalStart;                   ///< Vertical start

    MS_U16 u16HorizontalTotal;                 ///< Horizontal Total
    MS_U16 u16VerticalTotal;                   ///< Vertical Total
    MS_U8 u8VTotalTolerance;                   ///< VTotal tolerance

    MS_U8 u8AdcPhase;                          ///< ADC phase

    MS_U8 u8StatusFlag; ///< flags
    // b0: VSync polarity(1/0 = positive/negative)
    // b1: HSync polarity(1/0 = positive/negative)
    // b2: Sync polarity care bit
    // b4: interlace mode
}MS_PCADC_MODETABLE_TYPE;

typedef struct DLL_PACKED
{
    EN_RESOLUTION_TYPE enResolutionIndex;   ///< resolution

    MS_U16 u16HorizontalFrequency;             ///< Horizontal frequency
    MS_U16 u16VerticalFrequency;               ///< Vertical frequency

    MS_U16 u16HorizontalStart;                 ///< Horizontal start
    MS_U16 u16VerticalStart;                   ///< Vertical start

    MS_U16 u16HorizontalTotal;                 ///< Horizontal Total
    MS_U16 u16VerticalTotal;                   ///< Vertical Total
    MS_U16 u16VTotalTolerance;                   ///< VTotal tolerance

    MS_U16 u16AdcPhase;                          ///< adc phase

    MS_U8 u8StatusFlag; ///< flags
    // b0: VSync polarity(1/0 = positive/negative)
    // b1: HSync polarity(1/0 = positive/negative)
    // b2: Sync polarity care bit
    // b4: interlace mode
}MS_PCADC_MODETABLE_TYPE_EX;

#if defined(UFO_PUBLIC_HEADER_300)
typedef struct
{
    MS_U16 u16DisplayWidth;  // width
    MS_U16 u16DisplayHeight; // height

}MS_MODE_RESOLUTION;
#endif

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// XC Mode Parse initilization
/// @ingroup MODE_Parse_INIT
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_ModeParse_Init(void);
// mode search

//-------------------------------------------------------------------------------------------------
/// XC Mode Parse Match Mode
/// @ingroup MODE_Parse_FEATURE
/// @param  ModeDB                             \b IN: the mode table
/// @param  u16NumberOfItems             \b IN: the item number
/// @param  psInputInfo                         \b IN: the Input source information
/// @return @ref XC_MODEPARSE_RESULT
//-------------------------------------------------------------------------------------------------
XC_MODEPARSE_RESULT MApi_XC_ModeParse_MatchMode(MS_PCADC_MODETABLE_TYPE* ModeDB, MS_U8 u16NumberOfItems , XC_MODEPARSE_INPUT_INFO *psInputInfo);

//-------------------------------------------------------------------------------------------------
/// XC Mode Parse Match Mode EX
/// @ingroup MODE_Parse_FEATURE
/// @param  ModeDB                             \b IN: the EX mode table
/// @param  u16NumberOfItems             \b IN: the item number
/// @param  psInputInfo                         \b IN: the Input source information
/// @return @ref XC_MODEPARSE_RESULT
//-------------------------------------------------------------------------------------------------
XC_MODEPARSE_RESULT MApi_XC_ModeParse_MatchModeEx(MS_PCADC_MODETABLE_TYPE_EX* ModeDB, MS_U8 u16NumberOfItems , XC_MODEPARSE_INPUT_INFO *psInputInfo);
#ifdef __cplusplus
}
#endif

#endif /* _API_XC_MODEPARSE_H_ */
