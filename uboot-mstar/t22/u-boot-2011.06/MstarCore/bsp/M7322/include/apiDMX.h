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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Demux API
////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------
/*! \defgroup DMX DMX interface

  TSP (Transport Stream Processor) receives transport stream through serial or parallel interface.
  The input stream can be filtered by PID and masked pattern to get individual section data streams that are qualified with CRC and stored into the ring buffers located in external memory.
  Besides section filter, audio PES FIFO and video PES FIFO are used to pass the selected Audio/Video PES packet to audio/video decoder.
  PVR function is also supported.

  In order to parse section data in TS stream, TSP equipment a 32-bit RISC processor, besides, The PID filter, header compare, CRC, DMA and PVR are implemented with dedicated hardware.

  <b> Ownership of Filters </b> \n
  Not only one processor will use Demux driver at the same time, all the filters can be grouped into different processor by API MApi_DMX_SetOwner.
   \image html apiDMX_pic1.png

   <b> Filter Life Cycle </b> \n
   \image html apiDMX_pic2.png

  <b> API call flow for initiation: </b> \n
   \image html apiDMX_pic3.png

  <b> API call flow for exit: </b> \n
   \image html apiDMX_pic4.png

  <b> API call flow for general usage - Live in playback: </b> \n
   \image html apiDMX_pic5.png

  <b> API call flow for general usage - File in playback: </b> \n
   \image html apiDMX_pic6.png

  <b> API call flow for general usage - PVR/PVRCA: </b> \n
   \image html apiDMX_pic7.png

  <b> API call flow for general usage - MMFI Audio/MMFI Video3D: </b> \n
   \image html apiDMX_pic8.png

  *! \defgroup TSP TSP related functions
  *   \ingroup   DMX

  *! \defgroup TSP_General  TSP General functions
  *   \ingroup   TSP

  *! \defgroup TSP_Flow  TSP Flow control functions
  *   \ingroup   TSP

  *! \defgroup TSP_STC TSP STC/PCR functions
  *   \ingroup   TSP

  @details
        There are counters inside Demux hardware both operate on 27MHz frequency,
        STC, local PCR 1(LPCR1, i.e., PVR Record timestamp) and local PCR2(LPCR2, i.e., file in playback timestamp).
        The value of these counters increment 1 every 300 clock cycle, so user can transform to second by divided by 90K.

        LPCR1 used by PVR and LPCR2 used by file in playback for synchronization.
        In PVR, Demux will append 4 bytes LPCR1 to TS packet, it is useful to distinguish order and offset in different packets.
        In file in playback, Demux read one packet from memory when LPCR2 larger than timestamp (4bytes) of this packet.

  *! \defgroup TSP_Filter TSP Basic filter control functions
  *   \ingroup   TSP

  *! \defgroup TSP_SecFilter TSP Section filter control functions
  *   \ingroup   TSP

  *! \defgroup TSP_TTX TSP Teltext related functions
  *   \ingroup   TSP

  *! \defgroup TSP_FIFO TSP AVFIFO functions
  *   \ingroup   TSP

  *! \defgroup TSP_PVR TSP PVR (record) related functions
  *   \ingroup   TSP

  *! \defgroup TSP_PVR_General TSP PVR general  functions
  *   \ingroup   TSP_PVR

  *! \defgroup TSP_PVR_CA TSP PVR CA  functions (need decrypt)
  *   \ingroup   TSP_PVR

  *! \defgroup TSP_FILEIN_PVR TSP file-in record related functions
  *   \ingroup   TSP_PVR

  *! \defgroup TSP_PVR_STB TSP PVR related functions for STB
  *   \ingroup   TSP_PVR

  *! \defgroup TSP_Playback TSP playback (file-in) related functions
  *   \ingroup   TSP

  *! \defgroup TSP_SCMB TSP Scramble status check functions
  *   \ingroup   TSP

  *! \defgroup TSP_STB TSP functions which only used on STB chip
  *   \ingroup   TSP

  *! \defgroup TSP_Debug TSP Debug functions
  *   \ingroup   TSP

  *! \defgroup TSP_FIQ TSP FIQ related functions
  *   \ingroup   TSP

  *! \defgroup TSP_Merge TSP merge stream related functions for CI+ 1.4 used
  *   \ingroup   TSP

  *! \defgroup TSP_MM TSP MM (Multi-Media) related functions
  *   \ingroup   TSP

  *! \defgroup TSO TSO related functions
  *   \ingroup   DMX

  *! \defgroup TSO_General TSO general functions
  *   \ingroup   TSO

  *! \defgroup DMX_STR DMX STR functions
  *   \ingroup   DMX

  *! \defgroup DMX_ToBeModified DMX api to be modified
  *   \ingroup   DMX

  *! \defgroup DMX_ToBeModified_TSP TSP api to be modified
  *   \ingroup   DMX_ToBeModified

  *! \defgroup DMX_ToBeModified_MM MM api to be modified
  *   \ingroup   DMX_ToBeModified

  *! \defgroup DMX_ToBeModified_TSO TSO api to be modified
  *   \ingroup   DMX_ToBeModified

  *! \defgroup DMX_ToBeRemove DMX api to be removed
  *   \ingroup   DMX

  */
//-------------------------------------------------

#ifndef __API_DMX_H__
#define __API_DMX_H__

#include "MsCommon.h"
#include "UFO.h"

#ifdef __cplusplus
extern "C"
{
#endif

/// NULL PID definition
#define DMX_PID_NULL               (0x1FFFUL)

// Please don't change this value if you know exactly what you are doing
/// DMX Filter pattern depth
#define DMX_SECT_FLT_DEPTH         (16UL)

/// NULL DMX Id
#define DMX_DMXID_NULL             ((MS_U8)-1)
#define DMX_U8_DMXID_NULL          DMX_DMXID_NULL
#define DMX_U32_DMXID_NULL         ((MS_U32)-1)
#define MSIF_DMX_LIB_CODE                     {'D','M','X','_'}    //Lib code
#define MSIF_DMX_LIBVER                       {'1','5'}            //LIB version
#define MSIF_DMX_BUILDNUM                     {'0','3'}            //Build Number
#define MSIF_DMX_CHANGELIST                   {'0','0','6','1','7','4','7','0'} //P4 ChangeList Number

/// DMX Version
#define DMX_API_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_DMX_LIB_CODE,                  /* IP__                                             */  \
    MSIF_DMX_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_DMX_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_DMX_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

/// DMX status indicator
typedef enum
{
    DMX_FILTER_STATUS_OK,                   ///< DMX status OK
    DMX_FILTER_STATUS_ERROR,                ///< DMX status ERROR
    DMX_FILTER_STATUS_NOT_SUPPORT,           ///< DMX status NOT SUPPORT
    DMX_FILTER_STATUS_MULTIPRC_RUNNING      ///< DMX status multiple process still sunning
} DMX_FILTER_STATUS;

/// DMX debug levels
typedef enum
{
    DMX_DBG_NONE,   ///< None
    DMX_DBG_ERR,    ///< Errors
    DMX_DBG_WARN,   ///< Warnings
    DMX_DBG_INFO,   ///< Information
    DMX_DBG_PVR,    ///< PVR
    DMX_DBG_FUNC    ///< Function calls
} DMX_DBGMSG_LEVEL;

/// DMX internal startup parameter
#if defined(UFO_PUBLIC_HEADER_500_3)
typedef struct __attribute__((__packed__)) _DMX_FW_Param
#else
typedef struct DLL_PACKED _DMX_FW_Param
#endif
{
    MS_PHY                                  phyFWAddr;           ///< Firmware physical address
    MS_U32                                  u32FWSize;           ///< Firmware size

    MS_PHY                                  phyVQAddr;           ///< Virtual Queue physical address
    MS_U32                                  u32VQSize;           ///< Virtual Queue size

    MS_U32                                  u32IsHK;             ///< Indicates whether this is the "house-keeping" (= main) CPU
    MS_U32                                  u32Res;

    MS_U32                                  _zero_reserved[2];   ///< Reserved

} DMX_TSPParam;


#if 0
/// DMX fitler type and source type
typedef enum
{
    DMX_FILTER_TYPE_VIDEO,                                   ///< DMX video filter type
    DMX_FILTER_TYPE_AUDIO,                                   ///< DMX audio filter type
    DMX_FILTER_TYPE_AUDIO2,                                  ///< DMX audio2 filter type
    DMX_FILTER_TYPE_SECTION,                                 ///< DMX section filter type
    DMX_FILTER_TYPE_PES,                                     ///< DMX PES filter type
    DMX_FILTER_TYPE_PACKET,                                  ///< DMX TS packet filter type
    DMX_FILTER_TYPE_PCR,                                     ///< DMX PCR filter type
    DMX_FILTER_TYPE_TELETEXT,                                ///< DMX teletext filter type

    DMX_FILTER_SOURCE_TYPE_MASK  =  0xC0000000,              ///< DMX filter source mask
    DMX_FILTER_SOURCE_TYPE_LIVE  =  0x80000000,              ///< DMX live stream filter source
    DMX_FILTER_SOURCE_TYPE_FILE  =  0x40000000,              ///< DMX file in filter type
} DMX_FILTER_TYPE;
#endif

/// DMX fitler type and source type
#define DMX_FILTER_TYPE MS_U32  ///< Filter types

#define DMX_FILTER_TYPE_VIDEO                   0x00000000UL                  ///< DMX video filter type
#define DMX_FILTER_TYPE_AUDIO                   0x00000001UL                  ///< DMX audio filter type
#define DMX_FILTER_TYPE_AUDIO2                  0x00000002UL                  ///< DMX audio2 filter type
#define DMX_FILTER_TYPE_SECTION                 0x00000003UL                  ///< DMX section filter type
#define DMX_FILTER_TYPE_PES                     0x00000004UL                  ///< DMX PES filter type
#define DMX_FILTER_TYPE_PACKET                  0x00000005UL                  ///< DMX TS packet filter type
#define DMX_FILTER_TYPE_PCR                     0x00000006UL                  ///< DMX PCR filter type
#define DMX_FILTER_TYPE_TELETEXT                0x00000007UL                  ///< DMX teletext filter type
#define DMX_FILTER_TYPE_VIDEO3D                 0x00000008UL                  ///< DMX V3D filter type
#define DMX_FILTER_TYPE_VIDEO2                  DMX_FILTER_TYPE_VIDEO3D       ///< DMX video2 filter type
#define DMX_FILTER_TYPE_AUDIO3                  0x00000009UL                  ///< DMX audio3 filter type
#define DMX_FILTER_TYPE_AUDIO4                  0x0000000AUL                  ///< DMX audio4 filter type
#define DMX_FILTER_TYPE_REC                     0x0000000BUL                  ///< DMX record filter type no destination needs to be set
#define DMX_FILTER_TYPE_SCMBCHK                 0x0000000CUL                  ///< DMX filter type for checking TS scrambling status
#define DMX_FILTER_TYPE_SECTION_VER             0x0000000DUL                  ///< DMX section filter type with version and CRC check
#define DMX_FILTER_TYPE_SECTION_NO_PUSI         0x0000000EUL                  ///< DMX section filter type which receive section without PUSI
#define DMX_FILTER_TYPE_VIDEO3                  0x0000000FUL                  ///< DMX video3 filter type
#define DMX_FILTER_TYPE_VIDEO4                  0x00000010UL                  ///< DMX video4 filter type
#if defined(UFO_PUBLIC_HEADER_500_3)
#define DMX_FILTER_TYPE_PVR                     0x00000010UL                  ///< DMX record filter (merge stream) type no destination needs to be set
#define DMX_FILTER_TYPE_PVR1                    0x00000011UL                  ///< DMX record filter (merge stream) type no destination needs to be set
#define DMX_FILTER_TYPE_PVR2                    0x00000012UL                  ///< DMX record filter (merge stream) type no destination needs to be set
#define DMX_FILTER_TYPE_PVR3                    0x00000013UL                  ///< DMX record filter (merge stream) type no destination needs to be set
#else
#define DMX_FILTER_TYPE_PVR                     0x00000011UL                  ///< DMX record filter (merge stream) type no destination needs to be set
#define DMX_FILTER_TYPE_PVR1                    0x00000012UL                  ///< DMX record filter (merge stream) type no destination needs to be set
#define DMX_FILTER_TYPE_PVR2                    0x00000013UL                  ///< DMX record filter (merge stream) type no destination needs to be set
#define DMX_FILTER_TYPE_PVR3                    0x00000014UL                  ///< DMX record filter (merge stream) type no destination needs to be set
#endif
#define DMX_FILTER_TYPE_PVR4                    0x00000015UL                  ///< DMX record filter (merge stream) type no destination needs to be set
#define DMX_FILTER_TYPE_PVR5                    0x00000016UL                  ///< DMX record filter (merge stream) type no destination needs to be set
#define DMX_FILTER_TYPE_PVR6                    0x00000017UL                  ///< DMX record filter (merge stream) type no destination needs to be set
#define DMX_FILTER_TYPE_PVR7                    0x00000018UL                  ///< DMX record filter (merge stream) type no destination needs to be set
#define DMX_FILTER_TYPE_PVR8                    0x00000019UL                  ///< DMX record filter (merge stream) type no destination needs to be set
#define DMX_FILTER_TYPE_PVR9                    0x0000001AUL                  ///< DMX record filter (merge stream) type no destination needs to be set
#define DMX_FILTER_TYPE_AUDIO5                  0x0000001BUL                  ///< DMX audio5 filter type
#define DMX_FILTER_TYPE_AUDIO6                  0x0000001CUL                  ///< DMX audio6 filter type
#define DMX_FILTER_TYPE_VIDEO5                  0x0000001DUL                  ///< DMX video5 filter type
#define DMX_FILTER_TYPE_VIDEO6                  0x0000001EUL                  ///< DMX video6 filter type
#define DMX_FILTER_TYPE_VIDEO7                  0x0000001FUL                  ///< DMX video7 filter type
#define DMX_FILTER_TYPE_VIDEO8                  0x00000020UL                  ///< DMX video8 filter type
#define DMX_FILTER_TYPE_DSS_130                 0x00000021UL                  ///< DMX DSS_130 filter type. For CAP,DAP,etc.
#define DMX_FILTER_TYPE_DSS_130_AUX_PKT         0x00000022UL                  ///< DMX DSS_130_AUX_PKT filter type. For CWP.

#define DMX_FILTER_FLT_MASK                     0xF3FF0F00UL

#define DMX_FILTER_SOURCE_TYPE_MASK             0xF0FC0000UL                   ///< DMX filter source mask
#define DMX_FILTER_SOURCE_TYPE_LIVE             0x80000000UL                   ///< DMX live stream filter source
#define DMX_FILTER_SOURCE_TYPE_FILE             0x40000000UL                   ///< DMX file in filter type
#define DMX_FILTER_SOURCE_TYPE_LIVEB            DMX_FILTER_SOURCE_TYPE_FILE    ///< Input from TS1 to File PID filter
#define DMX_FILTER_SOURCE_TYPE_TS1              0x00800000UL                   ///< DMX live stream filter source 1
#define DMX_FILTER_SOURCE_TYPE_TS2              0x00400000UL                   ///< DMX live stream filter source 2
#define DMX_FILTER_SOURCE_TYPE_TS3              0x00200000UL                   ///< DMX live stream filter source 3
#define DMX_FILTER_SOURCE_TYPE_FILE1            0x00100000UL                   ///< DMX file-in stream, filter source from file1
#define DMX_FILTER_SOURCE_TYPE_FILE2            0x00080000UL                   ///< DMX file-in stream, filter source from file2
#define DMX_FILTER_SOURCE_TYPE_FILE3            0x00040000UL                   ///< DMX file-in stream, filter source from file3
#define DMX_FILTER_SOURCE_TYPE_TS4              0x00000000UL                   ///< DMX live stream filter source 4
#define DMX_FILTER_SOURCE_TYPE_TS5              0x000C0000UL                   ///< DMX live stream filter source 5
#define DMX_FILTER_SOURCE_TYPE_TS6              0x00140000UL                   ///< DMX live stream filter source 6
#define DMX_FILTER_SOURCE_TYPE_FILE4            0x00180000UL                   ///< DMX file-in stream, filter source from file4
#define DMX_FILTER_SOURCE_TYPE_FILE5            0x001C0000UL                   ///< DMX file-in stream, filter source from file5
#define DMX_FILTER_SOURCE_TYPE_FILE6            0x00240000UL                   ///< DMX file-in stream, filter source from file6


// Source ID define.  For merge stream
#define DMX_FILTER_SOURCEID_MASK                0x00010F00UL
#define DMX_FILTER_SOURCEID_0                   0x00000000UL
#define DMX_FILTER_SOURCEID_1                   0x00000100UL
#define DMX_FILTER_SOURCEID_2                   0x00000200UL
#define DMX_FILTER_SOURCEID_3                   0x00000300UL
#define DMX_FILTER_SOURCEID_4                   0x00000400UL
#define DMX_FILTER_SOURCEID_5                   0x00000500UL
#define DMX_FILTER_SOURCEID_6                   0x00000600UL
#define DMX_FILTER_SOURCEID_7                   0x00000700UL
#define DMX_FILTER_SOURCEID_8                   0x00000800UL
#define DMX_FILTER_SOURCEID_9                   0x00000900UL
#define DMX_FILTER_SOURCEID_10                  0x00000a00UL
#define DMX_FILTER_SOURCEID_11                  0x00000b00UL
#define DMX_FILTER_SOURCEID_12                  0x00000c00UL
#define DMX_FILTER_SOURCEID_13                  0x00000d00UL
#define DMX_FILTER_SOURCEID_14                  0x00000e00UL
#define DMX_FILTER_SOURCEID_15                  0x00000f00UL
#define DMX_FILTER_SOURCEID_16                  0x00010000UL
#define DMX_FILTER_SOURCEID_17                  0x00010100UL
#define DMX_FILTER_SOURCEID_18                  0x00010200UL
#define DMX_FILTER_SOURCEID_19                  0x00010300UL
#define DMX_FILTER_SOURCEID_20                  0x00010400UL
#define DMX_FILTER_SOURCEID_21                  0x00010500UL
#define DMX_FILTER_SOURCEID_22                  0x00010600UL
#define DMX_FILTER_SOURCEID_23                  0x00010700UL
#define DMX_FILTER_SOURCEID_24                  0x00010800UL
#define DMX_FILTER_SOURCEID_25                  0x00010900UL
#define DMX_FILTER_SOURCEID_26                  0x00010a00UL
#define DMX_FILTER_SOURCEID_27                  0x00010b00UL
#define DMX_FILTER_SOURCEID_28                  0x00010c00UL
#define DMX_FILTER_SOURCEID_29                  0x00010d00UL
#define DMX_FILTER_SOURCEID_30                  0x00010e00UL
#define DMX_FILTER_SOURCEID_31                  0x00010f00UL


// Only supported by TSP ver3.0 driver for section filter duplicate sw patch
#define DMX_FILTER_FLT_TYPE_DUPSEC              0x02000000UL

// Only supported by TSP ver2.0 driver
#define DMX_FILTER_SOURCE_TYPE_PLAYBACK         DMX_FILTER_SOURCE_TYPE_LIVE   ///< DMX playback filter source
#define DMX_FILTER_SOURCE_TYPE_PVR0             0x20000000UL                  ///< DMX PVR0 stream filter source
#define DMX_FILTER_SOURCE_TYPE_PVR1             0x10000000UL                  ///< DMX PVR1 stream filter source

#define DMX_FILTER_FLT_SCMB_MASK                0x30000000UL                  ///< DMX descramble filter mask
#define DMX_FILTER_FLT_TYPE_SCMB                0x20000000UL                  ///< DMX descramble filter type
#define DMX_FILTER_FLT_TYPE_SCMB_SHAREKEY       0x10000000UL                  ///< DMX descramble filter type with sharekey

// Support PVR I frame parser LUT.
// When open the filter type DMX_FILTER_TYPE_PVRx, video PID need to OR(|) this source type for PVR. EX: DMX_FILTER_TYPE_PVRx | DMX_FILTER_TYPE_EX_I_FRAME_PVR
#define DMX_FILTER_TYPE_EX_I_FRAME_PVR          0x01000000UL                  ///< DMX PVR I frame LUT PVR_TABLE enable
//--------------------------------------------------
// Debug table
/// DMX debug commands
typedef enum
{
    DMX_DEBUG_CMD_NONE,     ///< None
    DMX_DEBUG_CMD_CLEAR,    ///< Clear debug command
    DMX_DEBUG_CMD_ENABLE,   ///< Enable debug command
    DMX_DEBUG_CMD_DISABLE,  ///< Disable debug command
} DMX_DEBUG_CMD;

/// DMX debug source
typedef enum
{
    DMX_DEBUG_SRC_TS0       = 0,                     ///< Debug LIVE0 input
    DMX_DEBUG_SRC_TS1       = 1,                     ///< Debug LIVE1 input
    DMX_DEBUG_SRC_TS2       = 2,                     ///< Debug LIVE2 input
    DMX_DEBUG_SRC_FILE      = 3,                     ///< Debug FILE0 input
    DMX_DEBUG_SRC_TS3       = 4,                     ///< Debug LIVE3 input
    DMX_DEBUG_SRC_FILE0     = DMX_DEBUG_SRC_FILE,    ///< Debug FILE0 input
    DMX_DEBUG_SRC_FILE1     = 5,                     ///< Debug FILE1 input
    DMX_DEBUG_SRC_FILE2     = 6,                     ///< Debug FILE2 input
    DMX_DEBUG_SRC_FILE3     = 7,                     ///< Debug FILE3 input
    DMX_DEBUG_SRC_MMFI0     = 8,                     ///< Debug MMFI0 input
    DMX_DEBUG_SRC_MMFI1     = 9,                     ///< Debug MMFI1 input
    DMX_DEBUG_SRC_TS4       = 10,                    ///< Debug LIVE4 input
    DMX_DEBUG_SRC_TS5       = 11,                    ///< Debug LIVE5 input
    DMX_DEBUG_SRC_TS6       = 12,                    ///< Debug LIVE6 input
    DMX_DEBUG_SRC_FILE4     = 13,                    ///< Debug FILE4 input
    DMX_DEBUG_SRC_FILE5     = 14,                    ///< Debug FILE5 input
    DMX_DEBUG_SRC_FILE6     = 15,                    ///< Debug FILE6 input
} DMX_DEBUG_SRC;

/// DMX debug TSIF type
typedef enum
{
    DMX_DEBUG_TSIF_TS0,        ///< Debug LIVE0  input
    DMX_DEBUG_TSIF_TS1,        ///< Debug LIVE1  input
    DMX_DEBUG_TSIF_TSCB,       ///< Debug LIVECB input
    DMX_DEBUG_TSIF_TS2,        ///< Debug LIVE2  input
    DMX_DEBUG_TSIF_TS3,        ///< Debug LIVE3  input
    DMX_DEBUG_TSIF_TS4,        ///< Debug LIVE4  input
    DMX_DEBUG_TSIF_TS5,        ///< Debug LIVE5  input
    DMX_DEBUG_TSIF_TS6,        ///< Debug LIVE6  input

} DMX_DEBUG_TSIF;

/// DMX debug FIFO type
typedef enum
{
    DMX_DEBUG_FIFO_VIDEO,                                ///< Debug Video    FIFO
    DMX_DEBUG_FIFO_AUDIO,                                ///< Debug Audio    FIFO
    DMX_DEBUG_FIFO_VIDEO3D,                              ///< Debug Video 3D FIFO
    DMX_DEBUG_FIFO_VIDEO2 = DMX_DEBUG_FIFO_VIDEO3D,      ///< Debug Video 2  FIFO
    DMX_DEBUG_FIFO_AUDIOB,                               ///< Debug Audio B  FIFO
    DMX_DEBUG_FIFO_AUDIOC,                               ///< Debug Audio C  FIFO
    DMX_DEBUG_FIFO_AUDIOD,                               ///< Debug Audio D  FIFO
    DMX_DEBUG_FIFO_VIDEO3,                               ///< Debug Video 3  FIFO
    DMX_DEBUG_FIFO_VIDEO4,                               ///< Debug Video 4  FIFO
    DMX_DEBUG_FIFO_VIDEO5,                               ///< Debug Video 5  FIFO
    DMX_DEBUG_FIFO_VIDEO6,                               ///< Debug Video 6  FIFO
    DMX_DEBUG_FIFO_VIDEO7,                               ///< Debug Video 7  FIFO
    DMX_DEBUG_FIFO_VIDEO8,                               ///< Debug Video 8  FIFO
    DMX_DEBUG_FIFO_AUDIOE,                               ///< Debug Audio E  FIFO
    DMX_DEBUG_FIFO_AUDIOF                                ///< Debug Audio F  FIFO

} DMX_DEBUG_FIFO;

/// DMX debug FIFO Source
typedef enum
{
    DMX_DEBUG_PKT_DEMUX_0,      ///< Debug LIVE0  input
    DMX_DEBUG_PKT_DEMUX_0_FILE, ///< Debug FILE0  input
    DMX_DEBUG_PKT_DEMUX_1,      ///< Debug LIVE1  input
    DMX_DEBUG_PKT_DEMUX_2,      ///< Debug LIVE2  input
    DMX_DEBUG_MMFI0,            ///< Debug MMFI0  input
    DMX_DEBUG_MMFI1,            ///< Debug MMFI1  input
    DMX_DEBUG_PKT_DEMUX_3,      ///< Debug LIVE3  input
    DMX_DEBUG_PKT_DEMUX_1_FILE, ///< Debug FILE1  input
    DMX_DEBUG_PKT_DEMUX_2_FILE, ///< Debug FILE2  input
    DMX_DEBUG_PKT_DEMUX_3_FILE, ///< Debug FILE3  input
    DMX_DEBUG_PKT_DEMUX_4,      ///< Debug LIVE4  input
    DMX_DEBUG_PKT_DEMUX_5,      ///< Debug LIVE5  input
    DMX_DEBUG_PKT_DEMUX_6,      ///< Debug LIVE6  input
    DMX_DEBUG_PKT_DEMUX_4_FILE, ///< Debug FILE4  input
    DMX_DEBUG_PKT_DEMUX_5_FILE, ///< Debug FILE5  input
    DMX_DEBUG_PKT_DEMUX_6_FILE, ///< Debug FILE6  input

} DMX_DEBUG_FIFO_SRC;

// only one playback flow or PVR flow exist
// Playback flow and PVR flow can exist simultaneously

/// DMX flow type
typedef enum
{
    DMX_FLOW_PLAYBACK                   = 0,                       ///< DMX playback flow
    DMX_FLOW_PVR                        = 1,                       ///< DMX recording flow
    DMX_FLOW_PVR1                       = 2,                       ///< DMX recording flow 1 note: Not support for all chip

    DMX_FLOW_PVRCA                      = 3,                       ///< CI+ PVR

    DMX_FLOW_FILEIN_MM                  = 4,                       ///< DMX Filein flow with Main path Vido and MMFI path Audio
    DMX_FLOW_MMFI0                      = DMX_FLOW_FILEIN_MM,      ///< DMX Filein flow with MMFI 0 (MMFI version 2.0)
    DMX_FLOW_FILEIN_MM3D                = 5,                       ///< DMX Filein flow with Main path Video, MMFI path 3DVideo, and MMFI Audio
    DMX_FLOW_MMFI1                      = DMX_FLOW_FILEIN_MM3D,    ///< DMX Filein flow with MMFI 1 (MMFI version 2.0)

    DMX_FLOW_CIPHSS_PLAYBACK            = 6,                       ///< Playback flow for CI+ HSS 2 path input for CI+
    DMX_FLOW_CIPHSS_PVRCA               = 7,                       ///< PVR flow for CI+ HSS 2 path input for CI+

    DMX_FLOW_PVR2                       = 8,                       ///< DMX recording flow 2, note: Not support for all chip, PVR2
    DMX_FLOW_PVR3                       = 9,                       ///< DMX recording flow 3, note: Not support for all chip
    DMX_FLOW_PVR4                       = 10,                      ///< DMX recording flow 4, note: Not support for all chip
    DMX_FLOW_PVR5                       = 11,                      ///< DMX recording flow 5, note: Not support for all chip

    DMX_FLOW_TSO_PLAYBACK               = 12,                      ///< DMX TSO playback flow (live0 & file0)
    DMX_FLOW_PLAYBACK1                  = 13,                      ///< DMX playback flow from TS1 (version 3.0)
    DMX_FLOW_PVRCA1                     = 14,                      ///< CI+ PVR (version 3.0)

    DMX_FLOW_TSO_PLAYBACK1              = 15,                      ///< DMX TSO1 playback flow (live1 & file1)
    DMX_FLOW_PLAYBACK2                  = 16,                      ///< DMX playback flow from TS2 (version 3.0/4.0)
    DMX_FLOW_PLAYBACK3                  = 17,                      ///< DMX playback flow from TS3 (version 4.0)

    DMX_FLOW_PLAYBACK_NOCA              = 18,                      ///< DMX playback flow without pass through CA
    DMX_FLOW_PLAYBACK_SRC_TS1           = 19,                      ///< DMX playback, CA input from TS1 output to Live0

    DMX_FLOW_PLAYBACK_FI                = 20,                      ///< DMX playback flow from TSFI (version 3.0)

    DMX_FLOW_TSO_MMT                    = 21,                      ///< TSO MMT Flow

    DMX_FLOW_TSIO_PLAYBACK              = 22,                      ///< DMX TSIO playback flow

    DMX_FLOW_PLAYBACK4                  = 23,                      ///< DMX playback flow from TS4 (version 4.0)
    DMX_FLOW_PLAYBACK5                  = 24,                      ///< DMX playback flow from TS5 (version 4.0)
    DMX_FLOW_PLAYBACK6                  = 25,                      ///< DMX playback flow from TS6 (version 4.0)

    DMX_FLOW_ALP_TS0                    = 26,                      /// ALP path TSIF0
    DMX_FLOW_ALP_TSFI                   = 27,                      /// ALP path TSIF_FI
    DMX_FLOW_ALP_TS1                    = 28,                      /// ALP path TSIF1
    DMX_FLOW_ALP_TS2                    = 29,                      /// ALP path TSIF2

    DMX_FLOW_NUM,
} DMX_FLOW;

/// DMX input type
typedef enum
{
    DMX_FLOW_INPUT_DEMOD,               ///< DMX input from internal demodulator 0
    DMX_FLOW_INPUT_MEM,                 ///< DMX input from memory
    DMX_FLOW_INPUT_EXT_INPUT0,          ///< DMX input from external input 0
    DMX_FLOW_INPUT_EXT_INPUT1,          ///< DMX input from external input 1
    DMX_FLOW_INPUT_EXT_INPUT2,          ///< DMX input from external input 2. note: Not support for all chip
    DMX_FLOW_INPUT_EXT_INPUT3,          ///< DMX input from external input 3. note: Not support for all chip
    DMX_FLOW_INPUT_MEM_NOPASSCA,        ///< DMX input from memory, but not pass through CA
    DMX_FLOW_INPUT_DEMOD1,              ///< DMX input from internal demodulator 1
    DMX_FLOW_INPUT_TSO,                 ///< DMX input from TS Output 0
    DMX_FLOW_INPUT_TSO1,                ///< DMX input from TS Output 1
    DMX_FLOW_INPUT_MEM_PASSCA1,         ///< DMX input from memory, but pass through CA1, not CA0
    DMX_FLOW_INPUT_MEM1,                ///< DMX input from memory path 2
    DMX_FLOW_INPUT_EXT_INPUT4,          ///< DMX input from external input 4. note: Not support for all chip
    DMX_FLOW_INPUT_EXT_INPUT5,          ///< DMX input from external input 5. note: Not support for all chip

    DMX_FLOW_INPUT_EXT_INPUT0_3WIRE,    ///< DMX input from external 3-wire input 0. note: Not support for all chip
    DMX_FLOW_INPUT_EXT_INPUT1_3WIRE,    ///< DMX input from external 3-wire input 1. note: Not support for all chip
    DMX_FLOW_INPUT_EXT_INPUT2_3WIRE,    ///< DMX input from external 3-wire input 2. note: Not support for all chip
    DMX_FLOW_INPUT_EXT_INPUT3_3WIRE,    ///< DMX input from external 3-wire input 3. note: Not support for all chip
    DMX_FLOW_INPUT_EXT_INPUT4_3WIRE,    ///< DMX input from external 3-wire input 4. note: Not support for all chip
    DMX_FLOW_INPUT_EXT_INPUT5_3WIRE,    ///< DMX input from external 3-wire input 5. note: Not support for all chip
    DMX_FLOW_INPUT_EXT_INPUT6_3WIRE,    ///< DMX input from external 3-wire input 6. note: Not support for all chip

    DMX_FLOW_INPUT_TSIO,                ///< DMX input from TSIO output

    DMX_FLOW_INPUT_EXT_INPUT6,          ///< DMX input from external input 6. note: Not support for all chip
    DMX_FLOW_INPUT_EXT_INPUT7,          ///< DMX input from external input 7. note: Not support for all chip
    DMX_FLOW_INPUT_EXT_INPUT7_3WIRE,    ///< DMX input from external 3-wire input 7. note: Not support for all chip

    DMX_FLOW_INPUT_CILINK1,             ///< DMX input from CILINK1
    DMX_FLOW_INPUT_CILINK2,             ///< DMX input from CILINK2
    DMX_FLOW_INPUT_DISABLE,             ///< DMX input disable

} DMX_FLOW_INPUT;

/// DMX output pad
typedef enum
{
    DMX_FLOW_OUTPUT_NONE                = 0,
    DMX_FLOW_OUTPUT_EXT_PAD1            = 1,            ///< DMX output to external pad 1
    DMX_FLOW_OUTPUT_EXT_PAD3            = 3,            ///< DMX output to external pad 3
    DMX_FLOW_OUTPUT_CILINK1             = 4,            ///< DMX output to CILINK
    DMX_FLOW_OUTPUT_CILINK2             = 5,            ///< DMX output to CILINK1

}DMX_FLOW_OUTPUT_PAD;

/// DMX Transport Stream interface
typedef enum
{
    DMX_TSIF_LIVE0, ///< TS interface for live streams number 0
    DMX_TSIF_LIVE1, ///< TS interface for live streams number 1
    DMX_TSIF_LIVE2, ///< TS interface for live streams number 2
    DMX_TSIF_LIVE3, ///< TS interface for live streams number 3

    DMX_TSIF_FILE0, ///< TS interface for file input number 0
    DMX_TSIF_FILE1, ///< TS interface for file input number 1
    DMX_TSIF_FILE2, ///< TS interface for file input number 2
    DMX_TSIF_FILE3, ///< TS interface for file input number 3

    DMX_TSIF_MMT,

    DMX_TSIF_LIVE4, ///< TS interface for live streams number 4
    DMX_TSIF_LIVE5, ///< TS interface for live streams number 5
    DMX_TSIF_LIVE6, ///< TS interface for live streams number 6

    DMX_TSIF_FILE4, ///< TS interface for file input number 4
    DMX_TSIF_FILE5, ///< TS interface for file input number 5
    DMX_TSIF_FILE6, ///< TS interface for file input number 6

    DMX_TSIF_MAX,
} DMX_TSIF;

/// PVR recording engines - allow up to 6 simultaneous recordings
typedef enum
{
    DMX_PVR_EGN0, ///< Engine 0
    DMX_PVR_EGN1, ///< Engine 1
    DMX_PVR_EGN2, ///< Engine 2
    DMX_PVR_EGN3, ///< Engine 3
    DMX_PVR_EGN4, ///< Engine 4
    DMX_PVR_EGN5, ///< Engine 5

    DMX_PVR_TSO0,

    DMX_PVR_EGN6, ///< Engine 6
    DMX_PVR_EGN7, ///< Engine 7
    DMX_PVR_EGN8, ///< Engine 8
    DMX_PVR_EGN9, ///< Engine 9

    DMX_PVR_ENG_NUM,

    DMX_ALPPVR_ENG0 = 0x40,   //For TSIF0
    DMX_ALPPVR_ENG1 = 0x41,   //For TSIF_FI
    DMX_ALPPVR_ENG2 = 0x42,   //For TSIF1
    DMX_ALPPVR_ENG3 = 0x43,   //For TSIF2
} DMX_PVR_ENG;

#if 0
// make sure what you are doing before changing the value
/// DMX callback types
typedef enum
{
    DMX_EVENT_DATA_READY           = 0x00000001, ///< DMX event ready
    DMX_EVENT_BUF_OVERFLOW         = 0x00000002, ///< DMX event overflow
    DMX_EVENT_PVRBUF_FULL          = 0x00000010, ///< DMX event PVR buffer overflow

    /// TSP self task callback // optional --> default is CB by poll
    DMX_EVENT_CB_MASK              = 0x80000000, ///< DMX callback mask
    DMX_EVENT_CB_POLL              = 0x00000000, ///< DMX callback by polling // OBSOLETE
    DMX_EVENT_CB_SELF              = 0x80000000, ///< DMX callback by demux module
    DMX_EVENT_CB_SELF_TYPE2        = 0xC0000000, ///< DMX callback by demux module , type 2
} DMX_EVENT;
#endif

#define DMX_EVENT MS_U32                           ///< DMX event type
#define DMX_EVENT_DATA_READY          0x00000001UL ///< DMX event ready
#define DMX_EVENT_BUF_OVERFLOW        0x00000002UL ///< DMX event overflow
#define DMX_EVENT_SEC_CRCERROR        0x00000004UL ///< DMX event CRC error
#define DMX_EVENT_PVRBUF_FULL         0x00000010UL ///< DMX event PVR buffer overflow
#define DMX_EVENT_PVR1BUF_FULL        0x00000020UL ///< DMX event Channel Browser buffer overflow
#define DMX_EVENT_PVRBUF_SIZE_MET     0x00000040UL ///< DMX event PVR buffer callback size meet. note: Not support for all chips

/// TSP self task callback                         ///< optional --> default is CB by poll
#define DMX_EVENT_CB_MASK             0x80000000UL ///< DMX callback mask
#define DMX_EVENT_CB_POLL             0x00000000UL ///< DMX callback by polling // OBSOLETE
#define DMX_EVENT_CB_SELF             0x80000000UL ///< DMX callback by demux module
#define DMX_EVENT_CB_SELF_TYPE2       0xC0000000UL ///< DMX callback by demux module

/// DMX burst type
typedef enum
{
    DMX_BURST_1                    = 0x00,
    DMX_BURSTSINGLE                = DMX_BURST_1,
    DMX_BURST_2                    = 0x01,
    DMX_BURST2BYTE                 = DMX_BURST_2,
    DMX_BURST_4                    = 0x02,
    DMX_BURST4BYTE                 = DMX_BURST_4,
    DMX_BURST_8                    = 0x03,
    DMX_BURST8BYTE                 = DMX_BURST_8,
    DMX_BURST_16                   = 0x04
} DMX_BURSTTYPE;

typedef enum
{
    E_DMX_MODULE_SEC,   ///< engine ID should be 0
    E_DMX_MODULE_PVR,
    E_DMX_MODULE_MAX
} EN_DMX_MODULETYPE;

typedef struct
{
    MS_U32              u32DmxBurstInfoSize;    ///< total size of ST_DMX_BURST_INFO data structure
    EN_DMX_MODULETYPE   eModuleType;
    MS_U32              u32EngId;               ///< If eModuleType is E_DMX_MODULE_SEC, this field is no use.
    DMX_BURSTTYPE       BurstType;
} ST_DMX_BURST_INFO;

#if 0
// make sure what you are doing before changing the value
/// DMX section mode setting
typedef enum
{
    DMX_SECT_MODE_CONTI            = 0x00000000, ///< DMX receive section contiguously
    DMX_SECT_MODE_ONESHOT          = 0x00000001, ///< DMX only receive one section

    // optional
    DMX_SECT_MODE_CRCCHK           = 0x00000002, ///< DMX enables CRC check
    DMX_SECT_MODE_AUTO_ADDR        = 0x00000004, ///< DMX align read pointer to write pointer when get write pointer // OBSOLETE
} DMX_SECT_MODE;
#endif

#define DMX_SECT_MODE MS_U32                           ///< DMX section mode type
#define DMX_SECT_MODE_CONTI               0x00000000UL ///< DMX receive section contiguously
#define DMX_SECT_MODE_ONESHOT             0x00000001UL ///< DMX only receive one section
#define DMX_SECT_MODE_PESSCMCHK           0x00000003UL ///< DMX checking PES SCMB status

// optional
#define DMX_SECT_MODE_CRCCHK              0x00000002UL ///< DMX enables CRC check
#define DMX_SECT_MODE_AUTO_ADDR           0x00000004UL ///< DMX align read pointer to write pointer when get write pointer // OBSOLETE
#define DMX_SECT_MODE_AUTO_CRCCHK         0x00000008UL ///< DMX enables Auto CRC check, should not be set with DMX_SECT_MODE_CRCCHK together

/// for MApi_DMX_AVFifo_Status return value u32FifoStatus
#define DMX_FIFO_STATUS            MS_U32              ///< DMX fifo status type
#define DMX_FIFO_STATUS_LV_MASK           0x0000000FUL ///< use this mask to get fifo level and use below macro to get current fifo status
#define DMX_FIFO_STATUS_LV_USAGE_MASK     0x0000000CUL ///< use this mask to get fifo level usage
#define DMX_FIFO_STATUS_LV_USAGE_SHIFT             2UL
#define DMX_FIFO_STATUS_LV_EMPTY          0x00000001UL ///< if fifo level status equal to this macro means fifo is empty
#define DMX_FIFO_STATUS_RST_MASK          0x80000000UL ///< use this mask to get fifo reset status
#define DMX_FIFO_STATUS_RST_EN            0x80000000UL ///< if fifo reset status equal to this macro means fifo reset is enabled.

/// DMX filein Engine by path
typedef enum
{
    DMX_FILEIN_TSIF0,    ///< File input path 0
    DMX_FILEIN_TSIF1,    ///< File input path 1
    DMX_FILEIN_TSIF2,    ///< File input path 2
    DMX_FILEIN_TSIF3,    ///< File input path 3
    DMX_FILEIN_TSIF4,    ///< File input path 4
    DMX_FILEIN_TSIF5,    ///< File input path 5
    DMX_FILEIN_TSIF6,    ///< File input path 6
    DMX_FILEIN_INVALID,  ///< Invalid file input path
} DMX_FILEIN_PATH;


/// DMX filein destination types
typedef enum
{
    DMX_PES_AUDIO2_FIFO     = 0,                       ///< DMX destination is audio2 fifo
    DMX_PES_AUDIO_FIFO      = 1,                       ///< DMX destination is audio  fifo
    DMX_PES_VIDEO_FIFO      = 2,                       ///< DMX destination is video  fifo
    DMX_PES_NO_BYPASS_FIFO  = 3,                       ///< DMX destination with no bypass
    DMX_PES_VIDEO3D_FIFO    = 4,                       ///< DMX destination is video3D fifo
    DMX_PES_VIDEO2_FIFO     = DMX_PES_VIDEO3D_FIFO,    ///< DMX destination is video2  fifo
    DMX_PES_AUDIO3_FIFO     = 5,                       ///< DMX destination is audio3 fifo
    DMX_PES_AUDIO4_FIFO     = 6,                       ///< DMX destination is audio4 fifo
    DMX_PES_VIDEO3_FIFO     = 7,                       ///< DMX destination is video3 fifo
    DMX_PES_VIDEO4_FIFO     = 8,                       ///< DMX destination is video4 fifo
    DMX_PES_AUDIO5_FIFO     = 9,                       ///< DMX destination is audio5 fifo
    DMX_PES_AUDIO6_FIFO     = 10,                      ///< DMX destination is audio6 fifo
    DMX_PES_VIDEO5_FIFO     = 11,                      ///< DMX destination is video5 fifo
    DMX_PES_VIDEO6_FIFO     = 12,                      ///< DMX destination is video6 fifo
    DMX_PES_VIDEO7_FIFO     = 13,                      ///< DMX destination is video7 fifo
    DMX_PES_VIDEO8_FIFO     = 14,                      ///< DMX destination is video8 fifo

} DMX_FILEIN_DST; ///<  DMXPESDATAFIFO;

/// DMX callback prototype
typedef void (*DMX_NotifyCb)(MS_U8 u8DmxId, DMX_EVENT enEvent);                                ///< Callback type
/// DMX type 2 callback prototype
typedef void (*DMX_Type2NotifyCb)(MS_U8 u8DmxId, DMX_EVENT enEvent, MS_U32 NotifyParam1);      ///< Callback type 2
/// DMX type 2 callback prototype for filter number > 256
typedef void (*DMX_Type2NotifyCb_Ex)(MS_U32 u32DmxId, DMX_EVENT enEvent, void* NotifyParamEx); ///< Callback_Ex type 2
/// DMX callback used in copy data \ref MApi_DMX_CopyData
typedef MS_BOOL (*DMX_CheckCb)(MS_U8 u8DmxId, MS_U8* start, MS_U8* end, MS_U8* read, MS_U8* write);

// dummy data structure
typedef struct
{
    MS_U32                      streaminfo;
} DMX_Stream_info;

/// DMX open information structure for filter number > 256
typedef struct DLL_PACKED
{
    MS_U32                                  u32DmxInfoSize;  // total size of DMX_Info_EX data structure
    DMX_FILTER_TYPE                         DmxFltType;
} DMX_Info_EX;

/// DMX section filter information structure
#if defined(UFO_PUBLIC_HEADER_500_3)
typedef struct __attribute__((__packed__))
#else
typedef struct DLL_PACKED
#endif
{
    MS_PHY                      SectBufAddr;        ///< DMX section buffer in physical address
    MS_U32                      SectBufSize;        ///< DMX section buffer size
    DMX_SECT_MODE               SectMode;           ///< DMX section mode
    DMX_EVENT                   Event;              ///< DMX section event
    DMX_NotifyCb                pNotify;            ///< DMX section callback
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_700)
#if !defined (__aarch64__)
    MS_U32                      u32AlignDummy0;   //align size for MI init share mem size check fail
#endif
#endif
    DMX_Type2NotifyCb           pType2Notify;       ///< DMX section callback type 2
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_700)
#if !defined (__aarch64__)
    MS_U32                      u32AlignDummy1;   //align size for MI init share mem size check fail
#endif
#endif
    MS_U32                      Type2NotifyParam1;  ///< DMX section callback type 2 user's data
} DMX_Sect_info;

/// DMX section filter information structure for filter number > 256
typedef struct DLL_PACKED
{
    MS_U32                      u32SectInfoSize;    // sizeof(DMX_Sect_info_Ex)
    MS_PHY                      SectBufAddr;        ///< DMX section buffer in physical address
    MS_U32                      SectBufSize;        ///< DMX section buffer size
    DMX_SECT_MODE               SectMode;           ///< DMX section mode
    DMX_EVENT                   Event;              ///< DMX section event
    DMX_Type2NotifyCb_Ex        pType2Notify_Ex;    ///< DMX section callback type 2
    void*                       pType2NotifyParamEx;///< DMX section callback type 2 user's data
} DMX_Sect_info_Ex;


/// DMX filter information structure
#if defined(UFO_PUBLIC_HEADER_500_3)
typedef struct __attribute__((__packed__))
#else
typedef struct DLL_PACKED
#endif
{
    union {
        DMX_Stream_info    StreamInfo; ///< DMX stream information for future use
        DMX_Sect_info      SectInfo;   ///< DMX section information
    } Info;
} DMX_Flt_info;

/// DMX filter information structure for filter number > 256
typedef struct DLL_PACKED
{
    MS_U32                  u32FltInfoSize; // sizeof(DMX_Flt_info_Ex)
    MS_BOOL                 bSet;
    DMX_Sect_info_Ex        SectInfoEx;
    DMX_FILTER_TYPE         DmxFltType;
} DMX_Flt_info_Ex;

/// DMX PVR information structure
#if defined(UFO_PUBLIC_HEADER_500_3)
typedef struct __attribute__((__packed__))
#else
typedef struct DLL_PACKED
#endif
{
    MS_PHY                      pPvrBuf0;           ///< DMX PVR buffer 0 starting address
    MS_PHY                      pPvrBuf1;           ///< DMX PVR buffer 1 starting address
    MS_U32                      PvrBufSize0;        ///< DMX PVR buffer 0 size
    MS_U32                      PvrBufSize1;        ///< DMX PVR buffer 1 size
    DMX_EVENT                   Event;              ///< DMX PVR event
    DMX_NotifyCb                pNotify;            ///< DMX PVR callback

    MS_U16                      u16CaPvrModel;      ///< DMX PVR CA mode
    MS_BOOL                     bSpsEnable;         ///< DMX PVR CA mode

} DMX_Pvr_info;

/// DMX CAPVR mode
typedef enum
{
    DMX_CA_PVR_TSP                          = 0x00000000UL,
    DMX_CA_PVR_PROG0                        = 0x00000001UL,
    DMX_CA_PVR_PROG1                        = 0x00000002UL,
}DMX_CA_PVRMODE;

//----------------------------------
/// DMX debug table information structure
//----------------------------------
typedef struct DLL_PACKED
{
    DMX_DEBUG_CMD               TspCmd;
    DMX_DEBUG_SRC               TspSrc;
    DMX_DEBUG_FIFO              TspFifo;
} DMX_DisContiCnt_info, DMX_DropPktCnt_info;

typedef struct DLL_PACKED
{
    DMX_DEBUG_CMD               TspCmd;
    DMX_DEBUG_TSIF              TspTsif;
} DMX_LockPktCnt_info;

typedef struct DLL_PACKED
{
    DMX_DEBUG_CMD               TspCmd;
    DMX_DEBUG_FIFO              TspFifo;
    DMX_DEBUG_FIFO_SRC          TspFifoSrc;
} DMX_AVPktCnt_info;

/// DMX capability query type, all the capabilities' info are returned by MS_U32* except DMX_CAP_*_RANGE,
/// and return MS_U32* data0 & data1(array) for DMX_CAP_*_RANGE
typedef enum
{
    DMX_CAP_PID_FILTER_NUM          = 0,          ///< Get PID filter number
    DMX_CAP_SEC_FILTER_NUM          = 1,          ///< Get section filter number
    DMX_CAP_SEC_BUF_NUM             = 2,          ///< Get section buffer number

    DMX_CAP_PVR_ENG_NUM             = 3,          ///< Get pvr engine number
    DMX_CAP_PVR_FILTER_NUM          = 4,          ///< Get pvr filter number
    DMX_CAP_PVR1_FILTER_NUM         = 5,          ///< Get pvr1 filter number

    DMX_CAP_MMFI_AUDIO_FILTER_NUM   = 6,          ///< Get MM file in audio path filter number
    DMX_CAP_MMFI_V3D_FILTER_NUM     = 7,          ///< Get MM file in video 3D path filter number

    DMX_CAP_MMFI0_FILTER_NUM        = 6,          ///< Get MM file in 0 path filter number
    DMX_CAP_MMFI1_FILTER_NUM        = 7,          ///< Get MM file in 1 path filter number

    DMX_CAP_TSIF_NUM                = 8,          ///< Get ts interface number
    DMX_CAP_DEMOD_NUM               = 9,          ///< Get demod support number
    DMX_CAP_TS_PAD_NUM              = 10,         ///< Get TS pad support number
    DMX_CAP_VQ_NUM                  = 11,         ///< Get virtual queue support number

    DMX_CAP_CA_FLT_NUM              = 12,         ///< Get descramble filter number
    DMX_CAP_CA_KEY_NUM              = 13,         ///< Get total descramble key number

    DMX_CAP_FW_ALIGN                = 14,         ///< Get MIU alignment in bytes of DMX FW
    DMX_CAP_VQ_ALIGN                = 15,         ///< Get MIU alignment in bytes of Virtual queue
    DMX_CAP_VQ_PITCH                = 16,         ///< Get length in bytes in bytes of one VQ unit
    DMX_CAP_SEC_BUF_ALIGN           = 17,         ///< Get MIU alignment in bytes of section buffer
    DMX_CAP_PVR_ALIGN               = 18,         ///< Get miu alignment in bytes of record buffer

    DMX_CAP_PVRCA_PATH_NUM          = 19,         ///< Get the numbers of descramble record path

    DMX_CAP_SHAREKEY_FLT_RANGE      = 20,         ///< Get the range of sharekey descramble 0  filter id. data0 is start ID, data 1 is end ID
    DMX_CAP_PVRCA0_FLT_RANGE        = 21,         ///< Get the range of PVRCA0 filter id. data0 is start ID, data 1 is end ID
    DMX_CAP_PVRCA1_FLT_RANGE        = 22,         ///< Get the range of PVRCA1 filter id. data0 is start ID, data 1 is end ID
    DMX_CAP_PVRCA2_FLT_RANGE        = 23,         ///< Get the range of PVRCA2 filter id. data0 is start ID, data 1 is end ID
    DMX_CAP_SHAREKEY_FLT1_RANGE     = 24,         ///< Get the range of sharekey descramble 1 filter id. data0 is start ID, data 1 is end ID
    DMX_CAP_SHAREKEY_FLT2_RANGE     = 25,         ///< Get the range of sharekey descramble 2 filter id. data0 is start ID, data 1 is end ID

    DMX_CAP_HW_TYPE                 = 26,         ///< Get the HW architecture type. 0xXXX0: No PVR, 0x100X: TV series, 0x200X: STB series

    DMX_CAP_RESOURCE_SIZE           = 27,         ///< Get DMX Private Resource data struct size (Share resource)

    DMX_CAP_VFIFO_NUM               = 28,         ///< Get VFIFO support number
    DMX_CAP_AFIFO_NUM               = 29,         ///< Get AFIFO support number

    DMX_CAP_HWPCR_SUPPORT           = 30,         ///< Get HWPCR support status
    DMX_CAP_PCRFLT_START_IDX        = 31,         ///< Get PCR start index
    DMX_CAP_TSP_HWWP_SET_NUM        = 32,         ///< Get TSP write protect set numbers

    DMX_CAP_DSCMB_ENG_NUM           = 33,         ///< Get DSCMB engine numbers

    DMX_CAP_MAX_MERGESTR_NUM        = 34,         ///< Get Maxumum merge stream number

    DMX_CAP_MAX_SEC_FLT_DEPTH       = 35,
    DMX_CAP_FW_BUF_SIZE             = 36,
    DMX_CAP_FW_BUF_RANGE            = 37,
    DMX_CAP_VQ_BUF_RANGE            = 38,
    DMX_CAP_SEC_BUF_RANGE           = 39,
    DMX_CAP_FIQ_NUM                 = 40,

    DMX_CAP_TSO_FILTER_NUM          = 0x10000000, ///< Get TSO TSIF number
    DMX_CAP_TSO_ENGINE_NUM          = 0x10000002, ///< Get TSO Engine number
    DMX_CAP_TSO_TSIF_NUM            = 0x10000003, ///< Get TSO TSIF number

    DMX_CAP_NULL,
} DMX_QUERY_TYPE;

/// HW architecture type
typedef enum
{
    DMX_HW_TYPE_NONE             = 0UL,
    DMX_HW_TYPE_TV_SERIES        = 0x00001000UL,       ///< The HW architecture is TV series without PVR cunfction
    DMX_HW_TYPE_TV_TYPE1         = 0x00001001UL,       ///< The HW architecture is TV series with Seperate PVR filters
    DMX_HW_TYPE_TV_TYPE2         = 0x00001002UL,       ///< The HW architecture is TV series with general PVR filters
    DMX_HW_TYPE_TV_TYPE2_CA2     = 0x00001006UL,       ///< The HW architecture is TV series with general PVR filters, with general dscmb filters, and merge stream
    DMX_HW_TYPE_STB_SERIES       = 0x00002000UL,       ///< The HW architecture is BOX series without PVR cunfction
    DMX_HW_TYPE_STB_TYPE1        = 0x00002001UL,       ///< The HW architecture is BOX series 1
    DMX_HW_TYPE_STB_TYPE2        = 0x00002002UL,       ///< The HW architecture is BOX series 2
    DMX_HW_TYPE_STB_TYPE3        = 0x00002003UL,       ///< The HW architecture is BOX series 3
    DMX_HW_TYPE_STB_TYPE3_CASPS  = 0x80002003UL,       ///< The HW architecture is BOX series 3 with PVR CA SPS enable mode
} DMX_HW_TYPE;

/// Time-stamping clock type
typedef enum
{
    DMX_TIMESTAMP_CLK_90K       =  0x00000000UL,        ///< Original timestamp clock 90K
    DMX_TIMESTAMP_CLK_27M       =  0x00000001UL,        ///< Original timestamp clock 27M

    DMX_TIMESTAMP_CLK_STC0_90K  =  0x00000100UL,        ///< Select STC0 90K clock for timestamp clock source
    DMX_TIMESTAMP_CLK_STC0_27M  =  0x00000101UL,        ///< Select STC0 27M clock for timestamp clock source

    DMX_TIMESTAMP_CLK_STC1_90K  =  0x00000200UL,        ///< Select STC1 90K clock for timestamp clock source
    DMX_TIMESTAMP_CLK_STC1_27M  =  0x00000201UL,        ///< Select STC1 27M clock for timestamp clock source

    DMX_TIMESTAMP_CLK_STC2_90K  =  0x00000300UL,        ///< Select STC2 90K clock for timestamp clock source
    DMX_TIMESTAMP_CLK_STC2_27M  =  0x00000301UL,        ///< Select STC2 27M clock for timestamp clock source

    DMX_TIMESTAMP_CLK_STC3_90K  =  0x00000400UL,        ///< Select STC3 90K clock for timestamp clock source
    DMX_TIMESTAMP_CLK_STC3_27M  =  0x00000401UL,        ///< Select STC3 27M clock for timestamp clock source

    DMX_TIMESTAMP_CLK_STC4_90K  =  0x00000500UL,        ///< Select STC4 90K clock for timestamp clock source
    DMX_TIMESTAMP_CLK_STC4_27M  =  0x00000501UL,        ///< Select STC4 27M clock for timestamp clock source

    DMX_TIMESTAMP_CLK_STC5_90K  =  0x00000600UL,        ///< Select STC5 90K clock for timestamp clock source
    DMX_TIMESTAMP_CLK_STC5_27M  =  0x00000601UL,        ///< Select STC5 27M clock for timestamp clock source

    DMX_TIMESTAMP_CLK_STC6_90K  =  0x00000700UL,        ///< Select STC6 90K clock for timestamp clock source
    DMX_TIMESTAMP_CLK_STC6_27M  =  0x00000701UL,        ///< Select STC6 27M clock for timestamp clock source

    DMX_TIMESTAMP_CLK_STC7_90K  =  0x00000800UL,        ///< Select STC7 90K clock for timestamp clock source
    DMX_TIMESTAMP_CLK_STC7_27M  =  0x00000801UL         ///< Select STC7 27M clock for timestamp clock source

} DMX_TimeStamp_Clk;

/// TSP file in Packet mode
typedef enum //_DMX_Filein_Packmode
{
    DMX_PKTMODE_188         = 0x00000000UL,       ///< DMX live/file in packet size with 188 (Single stream)
    DMX_PKTMODE_192         = 0x00000001UL,       ///< DMX file in packet size with 192 (Single stream)
    DMX_PKTMODE_204         = 0x00000002UL,       ///< DMX file in packet size with 204 (Single stream)
    DMX_PKTMODE_130         = 0x00000003UL,       ///< DMX live/file in packet size with 130 (RVU/DSS)
    DMX_PKTMODE_134         = 0x00000004UL,       ///< DMX file in packet size with 134 (RVU with timestamp)
    DMX_PKTMODE_200         = 0x00000005UL,       ///< DMX live/file in packet size with 200 (Open cable Single stream)

    DMX_PKTMODE_MERG188     = 0x00000006UL,       ///< DMX live/file in packet size with 188 (Merge stream)
    DMX_PKTMODE_MERG192     = 0x00000007UL,       ///< DMX live/file in packet size with 192 (Merge stream)
    DMX_PKTMODE_MERG200     = 0x00000008UL,       ///< DMX live/file in packet size with 200 (Open cable Merge stream)

    DMX_PKTMODE_MXL192      = 0x00000009UL,       ///< DMX live/file in packet size with 192 (MxL 192 mode)
    DMX_PKTMODE_MXL196      = 0x0000000AUL,       ///< DMX live/file in packet size with 196 (MxL 196 mode)
    DMX_PKTMODE_MXL200      = 0x0000000BUL,       ///< DMX live/file in packet size with 196 (MxL 196 mode)

    DMX_PKTMODE_PES         = 0x00000100UL        ///< DMX pvr PES packet size
} DMX_Filein_PacketMode;

#define DMX_PacketMode DMX_Filein_PacketMode

/// TSP file in information structure
#if defined(UFO_PUBLIC_HEADER_500_3)
typedef struct __attribute__((__packed__))
#else
typedef struct DLL_PACKED
#endif
{
    MS_U32                 Rate;   ///< DMX file in rate
    DMX_Filein_PacketMode  PKT_Mode;    ///< DMX file in packet size mode
} DMX_Filein_info;

/// TSP PVR record Packet mode
#define DMX_Pvr_PacketMode DMX_Filein_PacketMode


/// TSP Flow Input information structure
/// For TSO_20 structure DMX_TSO_InputCfg
#if defined(UFO_PUBLIC_HEADER_500_3)
typedef struct __attribute__((__packed__))
#else
typedef struct DLL_PACKED
#endif
{
    DMX_FLOW_INPUT          Input;
    MS_BOOL                 bClkInv;
    MS_BOOL                 bExtSync;
    MS_BOOL                 bParallel;
}DMX_FlowInput_Info;

//MMFI Path
#define DMX_MMFI_PATH       MS_U32                ///< DMX multimedia file input path type.
#define DMX_MMFI_AUDIO      0UL                   ///< DMX destination with no bypass to MM Audio path
#define DMX_MMFI_VIDEO      1UL                   ///< DMX destination with no bypass to MM Video path
#define DMX_MMFI_PATH0      DMX_MMFI_AUDIO        ///< DMX MMFI path 0 to MM Audio path
#define DMX_MMFI_PATH1      DMX_MMFI_VIDEO        ///< DMX MMFI path 1 to MM Video path

/// Multimedia file input filter type
/// must alignment with TSP_FIFO_XXX in halTSP.h
typedef enum
{
    DMX_MMFI_FLTTYPE_AUD    = 0x00,                   ///< Pid filter whose output is sent to Audio    decoder FIFO
    DMX_MMFI_FLTTYPE_AUDB   = 0x01,                   ///< Pid filter whose output is sent to Audio-B  decoder FIFO
    DMX_MMFI_FLTTYPE_VD3D   = 0x02,                   ///< Pid filter whose output is sent to Video 3D decoder FIFO
    DMX_MMFI_FLTTYPE_VD2    = DMX_MMFI_FLTTYPE_VD3D,  ///< Pid filter whose output is sent to Video-2  decoder FIFO
    DMX_MMFI_FLTTYPE_VD     = 0x03,                   ///< Pid filter whose output is sent to Video    decoder FIFO
    DMX_MMFI_FLTTYPE_AUDC   = 0x04,                   ///< Pid filter whose output is sent to Audio-C  decoder FIFO
    DMX_MMFI_FLTTYPE_AUDD   = 0x05,                   ///< Pid filter whose output is sent to Audio-D  decoder FIFO
    DMX_MMFI_FLTTYPE_VD3    = 0x06,                   ///< Pid filter whose output is sent to Video-3  decoder FIFO
    DMX_MMFI_FLTTYPE_VD4    = 0x07,                   ///< Pid filter whose output is sent to Video-4  decoder FIFO
    DMX_MMFI_FLTTYPE_AUDE   = 0x08,                   ///< Pid filter whose output is sent to Audio-E  decoder FIFO
    DMX_MMFI_FLTTYPE_AUDF   = 0x09,                   ///< Pid filter whose output is sent to Audio-F  decoder FIFO
    DMX_MMFI_FLTTYPE_VD5    = 0x0A,                   ///< Pid filter whose output is sent to Video-5  decoder FIFO
    DMX_MMFI_FLTTYPE_VD6    = 0x0B,                   ///< Pid filter whose output is sent to Video-6  decoder FIFO
    DMX_MMFI_FLTTYPE_VD7    = 0x0C,                   ///< Pid filter whose output is sent to Video-7  decoder FIFO
    DMX_MMFI_FLTTYPE_VD8    = 0x0D,                   ///< Pid filter whose output is sent to Video-8  decoder FIFO

    DMX_MMFI1_FLTTYPE_MASK  = 0x80,
}DMX_MMFI_FLTTYPE;

/// Multimedia file input destination
typedef enum
{
    DMX_MMFI_PES_NO_BYPASS_AU       = 0x0000, ///< No audio bypass

    DMX_MMFI_PES_PS_AU              = 0x0001, ///< Directly bypass file-in data to audio fifo without any check or transformation. Since there is a PES parser between audio fifo and audio decoder, the data format should be PES formated such that the file can be decoded correctly
    DMX_MMFI_PES_PS_AUB             = 0x0002, ///< Directly bypass file-in data to audio-B fifo without any check or transformation. Since there is a PES parser between audio-B fifo and audio decoder, the data format should be PES formated such that the data can be decoded correctly

    DMX_MMFI_PES_APID_BYPASS        = 0x0004, ///< Audio PES data with bypass of audio PID to audio FIFO
    DMX_MMFI_PES_APIDB_BYPASS       = 0x0008, ///< Audio-B PES data with bypass audio-B PID to audio-2 FIFO

    DMX_MMFI_PES_NO_BYPASS_V3D      = 0x0010, ///< Extract PES data from input Transport Stream to video 3D FIFO
    DMX_MMFI_PES_NO_BYPASS_VD2      = DMX_MMFI_PES_NO_BYPASS_V3D,
    DMX_MMFI_PES_PS_V3D             = 0x0020, ///< Extract PES data from input Program Stream to video 3D FIFO
    DMX_MMFI_PES_PS_VD2             = DMX_MMFI_PES_PS_V3D,
    DMX_MMFI_PES_V3DPID_BYPASS      = 0x0040, ///< Video 3D PES data with bypass of video 3D PID to video 3D FIFO
    DMX_MMFI_PES_V2PID_BYPASS       = DMX_MMFI_PES_V3DPID_BYPASS,

    DMX_MMFI_PES_NO_BYPASS_VD       = 0x0080, ///< No video bypass
    DMX_MMFI_PES_VPID_BYPASS        = 0x0100, ///< Directly bypass file-in data to video fifo without any check or transformation. Since there is a PES parser between video fifo and video decoder, the data format should be PES formated such that the file can be decoded correctly

    DMX_MMFI_PES_NO_BYPASS_TS       = 0x0200, ///< The file-in data need to be TS format. Only pid-hit packets will be sent to audio/audioB fifo. User can use MApi_DMX_MMFI_Pid_Open() to set the pid filter to audio or audioB

    DMX_MMFI_PES_PS_VD              = 0x0400, ///< Program stream audio PES data bypass to video FIFO

    DMX_MMFI_PES_PS_AUC             = 0x0800, ///< Program stream audio PES data bypass to audio-C FIFO
    DMX_MMFI_PES_PS_AUD             = 0x1000, ///< Program stream audio PES data bypass to audio-D FIFO

    DMX_MMFI_PES_PS_VD3             = 0x2000, ///< Program stream audio PES data bypass to video-3 FIFO
    DMX_MMFI_PES_PS_VD4             = 0x4000, ///< Program stream audio PES data bypass to video-4 FIFO

    DMX_MMFI_PES_PS_AUE             = 0x0003, ///< Program stream audio PES data bypass to audio-E FIFO
    DMX_MMFI_PES_PS_AUF             = 0x0005, ///< Program stream audio PES data bypass to audio-F FIFO

    DMX_MMFI_PES_PS_VD5             = 0x0006, ///< Program stream audio PES data bypass to video-5 FIFO
    DMX_MMFI_PES_PS_VD6             = 0x0007, ///< Program stream audio PES data bypass to video-6 FIFO
    DMX_MMFI_PES_PS_VD7             = 0x0009, ///< Program stream audio PES data bypass to video-7 FIFO
    DMX_MMFI_PES_PS_VD8             = 0x000A, ///< Program stream audio PES data bypass to video-8 FIFO

    DMX_MMFI1_PES_TYPE_MASK         = 0x8000,   ///<  Must set, when using MMFI1 path
}DMX_MMFI_DST;

/// DMX TSO Clk
typedef enum
{
    E_DMX_TSO_OUTCLK_DIV2N,
    E_DMX_TSO_OUTCLK_DIVN,
    E_DMX_TSO_OUTCLK_62M,
    E_DMX_TSO_OUTCLK_54M,
    E_DMX_TSO_OUTCLK_PTSOOUT,
    E_DMX_TSO_OUTCLK_PTSOOUT_DIV8,
    E_DMX_TSO_OUTCLK_Dmd,
    E_DMX_TSO_OUTCLK_27M,
    E_DMX_TSO_OUTCLK_108M,
    E_DMX_TSO_OUTCLK_123M,
}DMX_TSO_OutClk;

/// DMX TSO Clk Source
typedef enum
{
    E_DMX_TSO_OUTCLKSRC_172M_2N,    ///< E_DMX_TSO_OUTCLK_DIV2N
    E_DMX_TSO_OUTCLKSRC_288M_2N,    ///< E_DMX_TSO_OUTCLK_DIV2N
    E_DMX_TSO_OUTCLKSRC_432M_2N,    ///< E_DMX_TSO_OUTCLK_DIV2N
    E_DMX_TSO_OUTCLKSRC_216M_N,     ///< E_DMX_TSO_OUTCLK_DIVN
    E_DMX_TSO_OUTCLKSRC_P_TS0IN,    ///< E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8
    E_DMX_TSO_OUTCLKSRC_P_TS1IN,    ///< E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8
    E_DMX_TSO_OUTCLKSRC_P_TS2IN,    ///< E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8
    E_DMX_TSO_OUTCLKSRC_P_TS3IN,    ///< E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8
    E_DMX_TSO_OUTCLKSRC_P_TS4IN,    ///< E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8
    E_DMX_TSO_OUTCLKSRC_P_TS5IN,    ///< E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8
    E_DMX_TSO_OUTCLKSRC_P_TS6IN,    ///< E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8
    E_DMX_TSO_OUTCLKSRC_P_CILINK1,  ///< E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8
    E_DMX_TSO_OUTCLKSRC_P_CILINK2,  ///< E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8

}DMX_TSO_OutClkSrc;

/// enum for cilink engine
typedef enum
{
    E_DMX_CILINK1,
    E_DMX_CILINK2
}DMX_CILINK_ENG;

/// enum for cilink configure cmd
typedef enum
{
    E_DMX_CILINK_COMPANION_TO_CARD_CLK_CFG,      ///< data type: DMX_CILINK_CPNCard_ClkCfg
    E_DMX_CILINK_COMPANION_FROM_CARD_CLK_CFG,    ///< data type: DMX_CILINK_CPNCard_ClkCfg
    E_DMX_CILINK_COMPANION_TO_SOC_CLK_CFG,       ///< data type: DMX_CILINK_CPNSoc_ClkCfg
    E_DMX_CILINK_COMPANION_FROM_SOC_CLK_CFG,     ///< data type: DMX_CILINK_CPNSoc_ClkCfg
}DMX_CILINK_CMD;

/// CILINK CPN TO/FROM CARD clk configuration
typedef struct DLL_PACKED
{
    MS_U32                  u32CfgDataSize;      ///< total size of DMX_CILINK_CPNCard_ClkCfg data structure
    MS_U8                   u8SampleCnt;         ///< phase adjustment counter
    MS_BOOL                 bSampleEnable;       ///< enable phase adjustment
    MS_BOOL                 bClkInv;             ///< clock invert
}DMX_CILINK_CPNCard_ClkCfg;

/// CILINK CPN TO/FROM SOC clk configuration
typedef struct DLL_PACKED
{
    MS_U32                  u32CfgDataSize;      ///< total size of DMX_CILINK_CPNSoc_ClkCfg data structure
    MS_BOOL                 bClkInv;             ///< clock invert
}DMX_CILINK_CPNSoc_ClkCfg;

// /CILINK configuration
typedef struct DLL_PACKED
{
    MS_U32              u32Res;                  ///< result field
    MS_U32              u32CfgDataSize;          ///< total size of DMX_CILINK_Cfg data structure
    DMX_CILINK_CMD      eCfgCmd;                 ///< command field
    MS_BOOL             bSet;                    ///< set (TRUE) or get (FALSE)
    void*               pCfgPara;                ///< pointer of config parameters
}DMX_CILINK_Cfg;

/// DMX TSO output packet size config. For TSO_20
typedef enum
{
    E_DMX_TSO_CFG_NONE         = 0x00000000UL,
    /// *pCfgPara data type:unused, u32CmdData0 is the packet size
    E_DMX_TSO_CFG_OPIF         = 0x00000001UL,
    /// *pCfgPara data type: DMX_TSO_SVQ_Cfg
    E_DMX_TSO_CFG_SVQ_BUF      = 0x00000002UL,
}DMX_TSO_CfgCmd;

/// DMX TSO Input Config. For TSO_20
#if defined(UFO_PUBLIC_HEADER_500_3)
typedef struct __attribute__((__packed__))
#else
typedef struct DLL_PACKED
#endif
{
    MS_U32                  u32Res;
    DMX_FLOW                eFlow;
    DMX_TSIF                eTSOInIf;
    DMX_FlowInput_Info      stInputInfo;
    MS_U8                   u8LocalStrId;
    MS_BOOL                 bBypassAll;
    MS_BOOL                 bEnable;
    MS_BOOL                 bSet;
}DMX_TSO_InputCfg;

/// DMX TSO Output Config. For TSO_20
#if defined(UFO_PUBLIC_HEADER_500_3)
typedef struct __attribute__((__packed__))
#else
typedef struct DLL_PACKED
#endif
{
    MS_U32                  u32Res;
    DMX_FLOW                eFlow;
    DMX_FLOW_OUTPUT_PAD     eOutPad;
    MS_U16                  u16OutPktSize;
    MS_BOOL                 bEnable;

    MS_BOOL                 bDefOutClk;
    MS_BOOL                 bOutClkInv;
    DMX_TSO_OutClk          eTsoOutClk;
    DMX_TSO_OutClkSrc       eTsoOutClkSrc;
    MS_U16                  u16DivNum;
    MS_BOOL                 bSet;
}DMX_TSO_OutputCfg;

/// For TSO_20, Set SVQ buffer use
typedef struct DLL_PACKED
{
    DMX_TSIF                eTSOTsif;   ///< TSO's TSIF
    MS_PHY                  phyAddr;    ///< TSO VQ buffer address
    MS_U32                  u32BufSize; ///< TSO VQ buffer size
}DMX_TSO_SVQ_Cfg;

/// DMX TSO Config. For TSO_20
typedef struct DLL_PACKED
{
    MS_U32                  u32Res;             ///< result field
    MS_U32                  u32TsoEng;          ///< tso engine numer
    MS_U32                  u32TsoCfgDataSize;  ///< total size of DMX_TSO_Cfg data structure
    MS_BOOL                 bSet;               ///< set (TRUE) or get (FALSE)

    DMX_TSO_CfgCmd          eCfgCmd;            ///< command field
    MS_U32                  u32CmdData0;
    void*                   pCfgPara;           ///< pointer of config parameters
}DMX_TSO_Cfg;

/// DMX FQ init infomation
#if defined(UFO_PUBLIC_HEADER_500_3)
typedef struct __attribute__((__packed__))
#else
typedef struct DLL_PACKED
#endif
{
    MS_U8    u8AddrMode;
    MS_PHY   u32BufStart;
    MS_U32   u32BufSize;
} DMX_FQ_Info;

/// File-in Queue data path skipping options
typedef enum
{
    E_DMX_FQ_SKIP_RUSH_DATA_PATH_NON        = 0x00000000UL, ///< None
    E_DMX_FQ_SKIP_APES_RUSH_DATA            = 0x00000008UL, ///< Audio-A data are filtered out
    E_DMX_FQ_SKIP_APES_B_RUSH_DATA          = 0x00000010UL, ///< Audio-B data are filtered out
    E_DMX_FQ_SKIP_VPES_RUSH_DATA            = 0x00000020UL, ///< Video data are filtered out
    E_DMX_FQ_SKIP_SEC_RUSH_DATA             = 0x00000040UL, ///< Sections data are filtered out
    E_DMX_FQ_SKIP_ADP_RUSH_DATA             = 0x00000080UL, ///< Reserved
    E_DMX_FQ_SKIP_PCR_RUSH_DATA             = 0x00000100UL, ///< PCR data is filtered out
    E_DMX_FQ_SKIP_RASP_RUSH_DATA            = 0x00000200UL, ///< RASP data are filtered out
    E_DMX_FQ_SKIP_PVR1_RUSH_DATA            = E_DMX_FQ_SKIP_RASP_RUSH_DATA,
    E_DMX_FQ_SKIP_PCR1_RUSH_DATA            = 0x00001000UL,
    E_DMX_FQ_SKIP_PVR3_RUSH_DATA            = E_DMX_FQ_SKIP_PCR1_RUSH_DATA,
    E_DMX_FQ_SKIP_CFG1_MASK                 = 0x80000000UL,
    E_DMX_FQ_SKIP_APES_C_RUSH_DATA          = 0x80000008UL, ///< Audio-C data are filtered out
    E_DMX_FQ_SKIP_APES_D_RUSH_DATA          = 0x80000010UL, ///< Audio-D data are filtered out
    E_DMX_FQ_SKIP_V3DPES_RUSH_DATA          = 0x80000020UL, ///< Video3D data are filtered out
    E_DMX_FQ_SKIP_V2PES_RUSH_DATA           = E_DMX_FQ_SKIP_V3DPES_RUSH_DATA,
    E_DMX_FQ_SKIP_PVR2_RUSH_DATA            = 0x80000200UL,
}DMX_FQ_SkipPath;

/// File-in Queue configuration options
typedef enum
{
    E_DMX_FQ_CFG_NONE                       = 0x00000000UL, ///< None
    E_DMX_FQ_CFG_SEL_SRCMUX                 = 0x000000001L, ///< File-in queue source selection
    E_DMX_FQ_CFG_FLT_NULL_PKT               = 0x000000002L, ///< File-in queue filter null packet or not
    /// *pCfgPara data type:MS_BOOL, *pCfgPara = TRUE => FQ Eng filter NULL packet; *pCfgPara = FALSE  =>FQ Eng doesn't filter NULL packet
    E_DMX_FQ_CFG_ADDR_MODE                  = 0x000000003L
    /// *pCfgPara data type:MS_BOOL, *pCfgPara = TRUE => Addr mode; *pCfgPara = FALSE => Normal mode
}DMX_FQ_CfgCmd;

/// File-in Queue configuration information
#if defined(UFO_PUBLIC_HEADER_500_3)
typedef struct __attribute__((__packed__))
#else
typedef struct DLL_PACKED
#endif
{
    MS_U32              u32Res;          ///< result field
    MS_U32              u32FQEng;        ///< File-in queue engine Id
    MS_U32              u32CfgDataSize;  ///< total size of DMX_FQ_Cfg data structure
    MS_BOOL             bSet;            ///< set (TRUE) or get (FALSE)
    DMX_FQ_CfgCmd       eCfgCmd;         ///< command field
    DMX_TSIF            eFqSrcSel;       ///< File-in queue source selection (transport stream input interface Id)
    void*               pCfgPara;        ///< pointer of config parameters
}DMX_FQ_Cfg;

/// DMX TEI  Remove Error Packet Infomation
typedef enum
{
    E_DMX_TEI_REMOVE_AUDIO_PKT,     ///< DMX TEI Remove Audio Packet
    E_DMX_TEI_REMOVE_VIDEO_PKT,     ///< DMX TEI Remove Video Packet

    E_DMX_TEI_REMOVE_PKT0_LIVE,
    E_DMX_TEI_REMOVE_PKT0_FILE,
    E_DMX_TEI_REMOVE_PKT1,
    E_DMX_TEI_REMOVE_PKT2,

    E_DMX_TEI_REMOVE_AUDIO2_PKT,    ///< DMX TEI Remove AudioB  Packet
    E_DMX_TEI_REMOVE_AUDIO3_PKT,    ///< DMX TEI Remove Audio3  Packet
    E_DMX_TEI_REMOVE_AUDIO4_PKT,    ///< DMX TEI Remove Audio4  Packet

    E_DMX_TEI_REMOVE_VIDEO2_PKT,    ///< DMX TEI Remove Video2  Packet
    E_DMX_TEI_REMOVE_VIDEO3_PKT ,   ///< DMX TEI Remove Video3  Packet
    E_DMX_TEI_REMOVE_VIDEO4_PKT ,   ///< DMX TEI Remove Video4  Packet

}DMX_TEI_RmPktType;

// DMX_RES_ALLOC_EN
typedef enum
{
    DMX_RES_NONE,
    DMX_RES_STC_ENG,                ///< STC engine: return data type (MS_U32)
    DMX_RES_FLOW_PB_LIVE,           ///< Live-in playback path: return data type (DMX_FLOW) , DMX_FLOW_PLAYBACK ~ DMX_FLOW_PLAYBACK3
    DMX_RES_FLOW_PB_FILE,           ///< File-in playback path: return data type (DMX_FLOW) , DMX_FLOW_PLAYBACK ~ DMX_FLOW_PLAYBACK3
    DMX_RES_FLOW_MMFI,               ///< MMFI path: return data type (DMX_FLOW) , DMX_FLOW_MMFI0 ~ DMX_FLOW_MMFI1
    DMX_RES_CILINK1,
    DMX_RES_CILINK2
}DMX_RES_TYPE;

//STC update Control Parameters define
#define eStcUpdateCtrlMode                  MS_U8 ///< STC update control options
#define DMX_STC_UPDATE_FW                   0x00  ///< Update STC by TSP FW (STC is updated by TSP FW running at AEON processor)
#define DMX_STC_UPDATE_HK                   0x01  ///< Update STC by HK (STC is updated by TSP utopia driver running at ARM/MIPS processor)
#define DMX_STC_UPDATE_UPDATEONCE           0x02  ///< Update STC Once when PCR reset

//S2P Clock Option
#define eS2PClkOpt                          MS_U32
#define S2P_CLK_OPT_NONE                    0x00000000
#define S2P_CLK_OPT_INVERT                  0x00000001
#define S2P_CLK_OPT_NON_INVERT              0x00000002

// Policy Manager
typedef enum
{
    DMX_PIPEID_GRP_NONE,
    DMX_PIPEID_GRP_PVR,
    DMX_PIPEID_GRP_FILE,
    DMX_PIPEID_GRP_MMFI,
    DMX_PIPEID_GRP_MAX_NUM,
}DMX_PIPEID_GROUP;

typedef union GENERAL_ENGID{
    DMX_PVR_ENG             PvrEng;         ///< PVR engine
    DMX_FILEIN_PATH         FileinPath;     ///< Filein Path
    DMX_MMFI_PATH           MmfiPath;
} DMX_GENERAL_ENGID;

/// DMX merge stream sync byte information structure
typedef struct DLL_PACKED
{
    MS_U8 u8SrcId;
    MS_U8 *pu8SyncByte;
} DMX_SrcID_SyncByte_Mapping;

/// Merge stream sync byte configuration options
typedef enum
{
    E_DMX_SYNC_BYTE_CFG_NONE                       = 0x00000000UL, ///< None
    E_DMX_SYNC_BYTE_CFG_SRCID                      = 0x00000001UL, ///< Merge stream source id and sync byte
    // pCfgPara -> DMX_SrcID_SyncByte_Mapping
    E_DMX_SYNC_BYTE_CFG_FORCE_SYNC                 = 0x00000002UL, ///< Force to replace all merged stream sync bytes to 0x47
    // pCfgPara -> MS_Bool bEnable
}DMX_SyncByte_CfgCmd;

/// DMX merge stream sync byte information structure
typedef struct DLL_PACKED
{
    MS_U32                                  u32Res;                 ///< result field
    MS_U32                                  u32DmxSyncByteInfo;     ///< total size of DMX_SyncByte_Cfg data structure
    MS_BOOL                                 bSet;                   ///< Set or get
    DMX_TSIF                                eIf;                    ///< Eunm value of DMX TSIF selection
    DMX_SyncByte_CfgCmd                     eCfgCmd;                ///< command field
    void*                                   pCfgPara;               ///< pointer of config parameters
} DMX_SyncByte_Cfg;


////////////////////////////////////////////////////////////////////////////////
// include utopia v2  header files here
////////////////////////////////////////////////////////////////////////////////
#ifdef DMX_UTOPIA_20
#include "apiDMX_v2.h"
#endif

//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @ingroup TSP_General
/// @param pu32ResMemAddr       \b IN: Pointer to store resource memory addresses
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_InitLibResource(void *pResMemAddr);
//-------------------------------------------------------------------------------------------------
/// Set TSP firmware address and size.
/// @ingroup TSP_General
/// @param pFwAddr       \b IN: TSP firmware address in DRAM physical address
/// @param u32FwSize     \b IN: TSP firmware size
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note It should be called before calling any other Demux API functions.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SetFW(MS_PHY pFwAddr, MS_U32 u32FwSize); // In DRAM physical address
//-------------------------------------------------------------------------------------------------
/// Set the MIU address for TSP aeon (firmware) used.
/// @ingroup TSP_General
/// @param phyDataAddr       \b IN: MIU phsyical address
/// @param u32size           \b IN: MIU buffer size
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SetFwDataAddr(MS_PHY phyDataAddr, MS_U32 u32size);
/// Set TSP is work on HK or AEON.
/// @ingroup TSP_General
/// @param bIsHK       \b IN: TRUE: HK, FALSE: AEON
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SetHK(MS_BOOL bIsHK); // OBSOLETE
//-------------------------------------------------------------------------------------------------
/// Initialize Demux module.
/// @ingroup TSP_General
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note It should be called before calling any other Demux API functions.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Init(void);
//-------------------------------------------------------------------------------------------------
/// Get init parameters of MApi_DMX_TSPInit
/// @ingroup TSP_General
/// @param pstParam       \b OUT: init parameters
/// @return UTOPIA_STATUS_ERR_NOT_AVAIL: DMX not system init
/// @return UTOPIA_STATUS_SUCCESS: DMX system init ready
//-------------------------------------------------------------------------------------------------
MS_U32 SYMBOL_WEAK MApi_DMX_TSPInit_GetConfig(DMX_TSPParam *pstParam);
//-------------------------------------------------------------------------------------------------
/// Initialize Demux API for Virtual Quere Enable
/// @ingroup TSP_General
/// @param param          \b IN: Init Virtual Quere Setting
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note if want to use virtual quere, please call this API after calling MApi_DMX_Init. (VQ does not support for all chip)
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_TSPInit(DMX_TSPParam *param);
//-------------------------------------------------------------------------------------------------
/// Exit Demux API
/// @ingroup TSP_General
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Exit(void);

//-------------------------------------------------------------------------------------------------
/// Force exit Demux API
/// @ingroup TSP_General
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note It is special case for MM
///-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_ForceExit(void);
//-------------------------------------------------------------------------------------------------
/// Save DMX driver states to DRAM.
/// @ingroup DMX_STR
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Suspend(void);
//-------------------------------------------------------------------------------------------------
/// Restore DMX driver states from DRAM. And TSP need input the firmware info. to reload TSP FW.
/// @ingroup DMX_STR
/// @param phyFWAddr     \b IN: TSP firmware load address
/// @param u32FWSize      \b IN: TSP firmware size
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Resume(MS_PHY phyFWAddr, MS_U32 u32FWSize);
//-------------------------------------------------------------------------------------------------
/// Check TSP is alive or not. (Firmware)
/// @ingroup TSP_General
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_ChkAlive(void);

//-------------------------------------------------------------------------------------------------
/// Reset TSP and MM. It will reset all filter and do HW reset.
/// @ingroup TSP_General
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Reset(void);

//-------------------------------------------------------------------------------------------------
/// Release TSP driver HW lock semaphone
/// @ingroup TSP_General
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_ReleaseSemaphone(void);

//-------------------------------------------------------------------------------------------------
/// Enable/Disable TSP MIU internal write protect. Include PVR record buffer and section buffer.
/// @ingroup TSP_General
/// @param bEnable          \b IN: Enable/Disable protect
/// @param pphyStartAddr    \b IN: Protect start address. Array[0-3]: [0]: PVR, [1-2]: Section, [3]: PVR1
/// @param pphyEndAddr      \b IN: Protect end address. Array[0-3]: [0]: PVR, [1-2]: Section, [3]: PVR1
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
void              MApi_DMX_WriteProtect_Enable(MS_BOOL bEnable, MS_PHY* pphyStartAddr, MS_PHY* pphyEndAddr);

//-------------------------------------------------------------------------------------------------
/// Enable/Disable Demux MIU internal Firmware write protect.
/// @ingroup TSP_General
/// @param bEnable          \b IN: Enable/Disable protect
/// @param phyStartAddr     \b IN: Protect start address.
/// @param phyEndAddr       \b IN: Protect end address.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
void              MApi_DMX_OrzWriteProtect_Enable(MS_BOOL bEnable, MS_PHY phyStartAddr, MS_PHY phyEndAddr);

//-----------------
// DMX_ToBeRemove
//-----------------
//-------------------------------------------------------------------------------------------------
/// Read drop packet count of AV due to duplicate. (Use new debug function to get this information)
/// @ingroup DMX_ToBeRemove
/// @param  eDmxPktType       \b IN: The packet type wihch want to remove
/// @param  bEnable           \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Read_DropPktCnt(MS_U16* pu16ADropCnt, MS_U16* pu16VDropCnt);

//-------------------------------------------------------------------------------------------------
/// Control DMX STR functions.
/// @ingroup DMX_STR
/// @param  u16PowerState  \b IN: STR power mode. (to control STR suspend & resume case.)
/// @param phyFWAddr       \b IN: TSP firmware load address
/// @param u32FWSize       \b IN: TSP firmware size
/// @return : Utopia status - UTOPIA_STATUS_SUCCESS (Succeed) or UTOPIA_STATUS_FAIL (Fail)
//-------------------------------------------------------------------------------------------------
MS_U32            MApi_DMX_SetPowerState(EN_POWER_MODE u16PowerState, MS_PHY phyFWAddr, MS_U32 u32FWSize);

//-------------------------------------------------------------------------------------------------
/// @brief Remove TEI error packet
/// @param eDmxPktType \b IN: DMX remove packet type
/// @param bEnable     \b IN: TRUE: enable the feature, FALSE: disable it
/// @return DMX_FILTER_STATUS_OK Success
/// @return DMX_FILTER_STATUS_ERROR Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_TEI_RemoveErrorPkt(DMX_TEI_RmPktType eDmxPktType, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set Demux Flow packet mode
/// @ingroup TSP_General
/// @param DmxFlow   \b IN: TSP flow path
/// @param ePktMode  \b IN: Packet mode of flow path.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SetPacketMode(DMX_FLOW DmxFlow, DMX_PacketMode ePktMode);

//-------------------------------------------------------------------------------------------------
/// Set enable/disable drop scramble packet of FIFO/PVR. If enable, scramble packet can't pass through FIFO/PVR.
/// @ingroup TSP_General
/// @param  pDmxFltType     \b IN: Type select: FIFO or PVR
/// @param  bEnable         \b IN: Enable/Disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_DropScmbPkt(DMX_FILTER_TYPE pDmxFltType, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Allocate DMX resource
/// @ingroup TSP_General
/// @param eResType      \b IN: Resource type to allocate
/// @param pRes          \b OUT: Pointer to store resource content
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
/// @note If you have already used this API to allocate resource,
/// @note please DO NOT call related APIs with un-allocated resource parameters.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_ResAllocate(DMX_RES_TYPE eResType, void *pRes);

//-------------------------------------------------------------------------------------------------
/// Free  DMX resource
/// @ingroup TSP_General
/// @param eResType         \b IN: Resource type to free
/// @param pRes             \b IN: Pointer to store resource content
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_ResFree(DMX_RES_TYPE eResType, void *pRes);

//-------------------------------------------------------------------------------------------------
/// Get STC engine which PCR PID index mapping to
/// @ingroup TSP_General
/// @param u32PcrFltId                \b IN: PCR PID index
/// @param pu32StcEng                 \b OUT: STC engine id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_Pcr_Get_MapSTC(MS_U32 u32PcrFltId, MS_U32 *pu32StcEng);

// owner ship set/checking
//--------------------------------------------------------------------------------------------------
/// Set demux filter owner.
/// @ingroup TSP_General
/// @param  u32DmxIdStart     \b IN: Demux filter start index
/// @param  u32DmxIdEnd       \b IN: Demux filter end index
/// @param  bOwner            \b IN: TRUE for owner, FALSE for not owner
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SetOwner(MS_U32 u32DmxIdStart, MS_U32 u32DmxIdEnd, MS_BOOL bOwner); //OBSOLETE

//--------------------------------------------------------------------------------------------------
/// Query DMX specific hardware capability.
/// @ingroup TSP_General
/// @param  DmxQueryType           \b IN: query type
/// @param  pOutput                \b OUT: answer of query type
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_GetCap(DMX_QUERY_TYPE DmxQueryType, void* pOutput);

//-----------------
// DMX_ToBeModified
//-----------------
//--------------------------------------------------------------------------------------------------
/// Query DMX specific hardware capability by string type.
/// @ingroup DMX_ToBeModified_TSP
/// @param  pstrQueryType          \b IN: String of capability item
/// @param  s32Strlen              \b IN: String size of capability item
/// @param  pOutput                \b OUT: answer of query type
/// @param  ps32OutSize            \b OUT: Output data length
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note:  Keep MApi_DMX_GetCap function and refine interface for parameter
//--------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_GetCap_Ex(char* pstrQueryType, MS_S32 s32Strlen, void* pOutput, MS_S32* ps32OutSize);

//-------------------------------------------------------------------------------------------------
/// Invert Demux parallel input bit order
/// @ingroup TSP_General
/// @param DmxFlow       \b IN: TSP flow path
/// @param bInvert       \b IN: Invert bit order of TS parallel in. (TRUE: Invert, FALSE: Not invert)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Parl_Invert(DMX_FLOW DmxFlow, MS_BOOL bInvert);

//-------------------------------------------------------------------------------------------------
/// Set Demux DMA burst length.
/// @ingroup TSP_General
/// @param BurstType    \b IN: Burst length
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SetBurstLen(DMX_BURSTTYPE BurstType);
DMX_FILTER_STATUS MApi_DMX_SetBurstLen_Ex(ST_DMX_BURST_INFO stDmxBurstInfo);

//-------------------------------------------------------------------------------------------------
/// Enable/Disable TSP VQ.
/// @ingroup TSP_General
/// @param bEnable        \b IN: Enable/Disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_VQ_Enable(MS_BOOL bEnable);

//-----------------
// TSP_Flow
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set Demux Flow config.
/// @ingroup TSP_Flow
/// @param DmxFlow          \b IN: TSP flow path
/// @param DmxFlowInput     \b IN: Input port. (ex: external pad0, internal demod...etc)
/// @param bClkInv          \b IN: TS-in options: clock phase inversion
/// @param bExtSync         \b IN: TS-in options: sync by external signal
/// @param bParallel        \b IN: TS-in is parallel interface or serial interface
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_FlowSet(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel);

//--------------------------------------------------------------------------------------------------
/// Setting PVR flow.
/// @ingroup TSP_Flow
/// @param  Eng              \b IN: PVR Engine ID
/// @param  ePvrSrcTSIf      \b IN: Select the record source
/// @param  bDscmbRec        \b IN: TRUE for recording dscmbled stream; FALSE for recording orignal stream
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PVR_FlowSet(DMX_PVR_ENG Eng, DMX_TSIF ePvrSrcTSIf, MS_BOOL bDscmbRec);

//-------------------------------------------------------------------------------------------------
/// Get Demux Flow setting.
/// @ingroup TSP_Flow
/// @param DmxFlow            \b IN: TSP flow path
/// @param pDmxFlowInput      \b OUT: pointer to store DMX input flow value.
/// @param pbClkInv           \b OUT: pointer to store clock phase inversion
/// @param pbExtSync          \b OUT: pointer to store sync by external signal
/// @param pbParallel         \b OUT: pointer to store parallel interface or serial interface
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_FlowInput_Status(DMX_FLOW DmxFlow, DMX_FLOW_INPUT *pDmxFlowInput, MS_BOOL *pbClkInv, MS_BOOL *pbExtSync, MS_BOOL *pbParallel);

//-------------------------------------------------------------------------------------------------
/// Enable Demux flow path.
/// @ingroup TSP_Flow
/// @param DmxFlow             \b IN: TSP flow path
/// @param bEnable             \b IN: TRUE: enable, FALSE: disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_FlowEnable(DMX_FLOW DmxFlow, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Enable liveAB config: TSIF1 path can use as live through TSIF0_FILE path. (Old arichtecture TSIF1 can't goto section)
/// Only support on TSP 1.0
/// @param bPF_EN              \b IN: Enable/Disable liveAB bit.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_LiveAB_En(MS_BOOL bPF_EN);

//-------------------------------------------------------------------------------------------------
/// Check liveAB config is enable or not
/// Only support on TSP 1.0
/// @return TRUE - LiveAB is enable
/// @return FALSE - LiveAB is disable
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL           MApi_DMX_IsLiveAB_Enable(void);

//-------------------------------------------------------------------------------------------------
/// Set/Get DScmbler engine id depend on TSIF path
/// @ingroup TSP_Flow
/// @param  eTsIf                 \b IN: TSP flow path (interface)
/// @param  pu32EngId             \b IN/OUT: pointer to store dscmb engine id
/// @param  bSet                  \b IN: TRUE:Set dscmb engine id ; FALSE: Get dscmb engine id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_Flow_DscmbEng(DMX_TSIF eTsIf, MS_U32* pu32EngId, MS_BOOL bSet);

//-------------------------------------------------------------------------------------------------
/// Configure TS output pad.
/// @ingroup TSP_Flow
/// @param  eOutPad           \b IN: Select output pad
/// @param  eInSrcPad         \b IN: Select input source pad
/// @param  bInParallel       \b IN: TRUE: input source is paralleled; FALSE: input source is serial in
/// @param  u32ResvNum        \b IN: Reserved value of the number of reserved parameters for future using
/// @param  pu32Resv          \b IN: Reserved pointer to store parameters for future using
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Only EXT_PAD1 and EXT_PAD3 has output mode
/// @note If output from S2P mode, input source should be external input pad only
/// @note If output from TSO mode, eInputPad should be E_DRVTSP_PAD_TSO
/// @note If input source is internal demod, output mode can be demod out or TSO out
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TsOutputPadCfg(DMX_FLOW_OUTPUT_PAD eOutPad, DMX_FLOW_INPUT eInSrcPad, MS_BOOL bInParallel, MS_U32 u32ResvNum, MS_U32 *pu32Resv);

//-------------------------------------------------------------------------------------------------
/// Configure output pad
/// Set S2P output clock phase and invert.
/// @ingroup TSP_Flow
/// @param  u16Val               \b IN: Value of phase
/// @param  bEnable              \b IN: Enable/Disable phase tuning
/// @param  u32Reserved          \b IN: Reserved for future using
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Configure for S2P mode
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TsS2POutputClkPhase(MS_U16 u16Val, MS_BOOL bEnable, MS_U32 u32S2pOpt);

//-----------------
// TSP_STC
//-----------------
//--------------------------------------------------------------------------------------------------
/// Enable/Disable STC64 bit mode.
/// @ingroup TSP_STC
/// @param  bEnable              \b IN: Enable or Disable STC64 mode
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note This is also set by TSP init function.
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_STC64_Mode_Enable(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Get STC counter.
/// @ingroup DMX_ToBeRemove
/// @param pu32Stc32       \b OUT: STC counter (bit 32)
/// @param pu32Stc         \b OUT: STC counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Stc_Get(MS_U32* pu32Stc32, MS_U32* pu32Stc);

//-------------------------------------------------------------------------------------------------
/// Set STC counter.
/// @ingroup DMX_ToBeRemove
/// @param u32Stc32       \b OUT: STC counter (bit 32)
/// @param u32Stc         \b OUT: STC counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Stc_Set(MS_U32 u32Stc32, MS_U32 u32Stc);

//-------------------------------------------------------------------------------------------------
/// Get PCR value.
/// @ingroup DMX_ToBeRemove
/// @param pu32Pcr32     \b OUT: PCR counter (bit 32)
/// @param pu32Pcr       \b OUT: PCR counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pcr_Get(MS_U32* pu32Pcr32, MS_U32* pu32Pcr);

//-------------------------------------------------------------------------------------------------
/// Select STC Engine.
/// @ingroup TSP_STC
/// @param eFltSrc         \b IN: Fliter Source type and Filter source ID
/// @param u32StcEng       \b IN: STC engine ID (0, 1)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Only support in driver version 3.0.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_Stc_Select(DMX_FILTER_TYPE eFltSrc, MS_U32 u32StcEng);

//-------------------------------------------------------------------------------------------------
/// Get STC counter.
/// @ingroup TSP_STC
/// @param u8Eng             \b IN: STC Engine ID
/// @param pu32Stc32         \b OUT: STC counter (bit 32)
/// @param pu32Stc           \b OUT: STC counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Stc_Eng_Get(MS_U8 u8Eng, MS_U32* pu32Stc32, MS_U32* pu32Stc);

//-------------------------------------------------------------------------------------------------
/// Set STC counter.
/// @ingroup TSP_STC
/// @param u8Eng          \b IN: STC Engine ID
/// @param u32Stc32       \b OUT: STC counter (bit 32)
/// @param u32Stc         \b OUT: STC counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Stc_Eng_Set(MS_U8 u8Eng, MS_U32 u32Stc32, MS_U32 u32Stc);

//-------------------------------------------------------------------------------------------------
/// Get PCR.
/// @ingroup TSP_STC
/// @param u8PcrEng      \b INPUT: PCR Eng
/// @param pu32Pcr32     \b OUT: PCR counter (bit 32)
/// @param pu32Pcr       \b OUT: PCR counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pcr_Eng_Get(MS_U8 u8PcrEng, MS_U32* pu32Pcr32, MS_U32* pu32Pcr);

//-------------------------------------------------------------------------------------------------
/// Set STC control.
/// @ingroup TSP_STC
/// @param u8Eng             \b IN: STC Engine ID
/// @param eMode             \b IN: STC update control options
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note eMode: 0x00 -> Update STC by TSP FW
/// @note             0x01 -> Update STC by HK
/// @note             0x02 -> Update STC Once when PCR reset
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_STC_UpdateCtrl(MS_U8 u8Eng, eStcUpdateCtrlMode eMode);

//-------------------------------------------------------------------------------------------------
/// Add or subtract STC Offset.
/// @ingroup TSP_STC
/// @param u32Eng              \b IN: STC Engine ID
/// @param u32Offset           \b IN: STC offset value
/// @param bAdd                \b IN: If TRUE, add the offset value; otherwise subtract the offset value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_Stc_Eng_SetOffset(MS_U32 u32Eng, MS_U32 u32Offset, MS_BOOL bAdd);

//-------------------------------------------------------------------------------------------------
/// Adjust STC Clock.
/// @ingroup TSP_STC
/// @param u32EngId            \b IN: STC Engine ID
/// @param bUpClk              \b IN: If TRUE, Clk will be faster, and if FALSE, Clk will be slower
/// @param u32Percentage       \b IN: The percentage of STC clock ratio
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Stc_Clk_Adjust(MS_U32 u32EngId, MS_BOOL bUpClk, MS_U32 u32Percentage);

//-----------------
// TSP_Filter
//-----------------
//-------------------------------------------------------------------------------------------------
/// Open a demux filter.
/// @ingroup TSP_Filter
/// @param DmxFltType    \b IN: The filter information to allocate
/// @param pu8DmxId      \b OUT: The available demux filer index
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Do NOT use MApi_DMX_Open & MApi_DMX_Open_Ex at the same time
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Open(DMX_FILTER_TYPE DmxFltType, MS_U8* pu8DmxId);

//-------------------------------------------------------------------------------------------------
/// Open a demux filter.
/// @ingroup TSP_Filter
/// @param pDmxInfo      \b IN: The filter information to allocate
/// @param pu32DmxId     \b OUT: The available demux filer index
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Open_Ex(DMX_Info_EX *pDmxInfo, MS_U32* pu32DmxId);

//-------------------------------------------------------------------------------------------------
/// Close a demux filter.
/// @ingroup TSP_Filter
/// @param u32DmxId        \b IN: the demux filer index to free
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Close(MS_U32 u32DmxId);

//-------------------------------------------------------------------------------------------------
/// Activate a demux filter.
/// @ingroup TSP_Filter
/// @param u32DmxId        \b IN: the demux filer index to activate
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Start(MS_U32 u32DmxId);

//-------------------------------------------------------------------------------------------------
/// Deactivate a demux filter.
/// @ingroup TSP_Filter
/// @param u32DmxId         \b IN: the demux filer index to deactivate
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Stop(MS_U32 u32DmxId);

//-------------------------------------------------------------------------------------------------
/// Get/set a demultiplexer filter information
/// @param  u32DmxId    \b IN: The ID of the demultiplexer filter
/// @param  pDmxFltInfo \b IN/OUT: The demultiplexer filter information
/// @param  pDmxFltType \b IN/OUT: The demultiplexer filter type
/// @param  bSet        \b IN: set (TRUE) or get (FALSE)
/// @return DMX_FILTER_STATUS_OK Success
/// @return DMX_FILTER_STATUS_ERROR Failure
/// @note If \b pDmxFltType is equal to \ref DMX_FILTER_TYPE_VIDEO, \ref DMX_FILTER_TYPE_AUDIO or \ref DMX_FILTER_TYPE_AUDIO2 :
/// - If \b bSet is equal to TRUE, pDmxFltInfo->StreamInfo shall be filled-in (for future usage)
/// - If \b bSet is equal to FALSE, then the previously set stream information will be returned in pDmxFltInfo->StreamInfo
/// @note If \b pDmxFltType is equal to \ref DMX_FILTER_TYPE_SECTION, \ref DMX_FILTER_TYPE_TELETEXT, \ref DMX_FILTER_TYPE_PES, \ref DMX_FILTER_TYPE_PACKET or \ref DMX_FILTER_TYPE_PCR :
/// - If \b bSet is equal to TRUE, pDmxFltInfo->SectInfo shall be filled-in
/// - If \b bSet is equal to FALSE, then the previously set information will be returned in pDmxFltInfo->SectInfo
/// - The members in pDmxFltInfo->SectInfo structure shall be set as follows: \n
///      SectBufAddr : the section buffer physical address \n
///      SectBufSize : the section buffer size \n
///      SectMode    : OR combination of the following: (DMX_SECT_MODE_CONTI and DMX_SECT_MODE_ONESHOT are exclusive) \n
///                    \ref DMX_SECT_MODE_CONTI : Receive section data continuously \n
///                    \ref DMX_SECT_MODE_ONESHOT : Receive one section only \n
///                    \ref DMX_SECT_MODE_CRCCHK : Ask DMX hardware to check CRC32 value of section data and to drop incorrect sections \n
///      Event       : OR combination of the following ( \ref DMX_EVENT_CB_POLL, \ref DMX_EVENT_CB_SELF and \ref DMX_EVENT_CB_SELF_TYPE2 are exclusive) \n
///                    \ref DMX_EVENT_CB_POLL : use polling mode \n
///                    \ref DMX_EVENT_CB_SELF : User set events will be notified to the user using the callback set in the \b pNotify field \n
///                    \ref DMX_EVENT_CB_SELF_TYPE2 : User set events will be notified to the user using the callback set in the \b pType2Notify field. The caller may specify specific user data using the \b Type2NotifyParam1 field \n
///                    \ref DMX_EVENT_DATA_READY : If set, an event will be raised (and notified through callback if \ref DMX_EVENT_CB_SELF or \ref DMX_EVENT_CB_SELF_TYPE2 are set) when one or more complete section data has been captured in the section buffer \n
///                    \ref DMX_EVENT_BUF_OVERFLOW : If set, an event will be raised (and notified through callback if \ref DMX_EVENT_CB_SELF or \ref DMX_EVENT_CB_SELF_TYPE2 are set) if the section buffer overflows \n
///                    \ref DMX_EVENT_DATA_READY : If set, an event will be raised (and notified through callback if \ref DMX_EVENT_CB_SELF or \ref DMX_EVENT_CB_SELF_TYPE2 are set) if CRC32 check fails (assuming \ref DMX_SECT_MODE_CRCCHK has been set in \b SectMode) \n
/// @note Do NOT use MApi_DMX_Info & MApi_DMX_Info_Ex at the same time
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Info(MS_U32 u32DmxId, DMX_Flt_info* pDmxFltInfo, DMX_FILTER_TYPE* pDmxFltType, MS_BOOL bSet);

//-------------------------------------------------------------------------------------------------
/// Get/set a demultiplexer filter information for filter number > 256
/// @param u32DmxId    \b IN: The ID of the demultiplexer filter
/// @param pDmxFltInfo \b IN/OUT: The demultiplexer filter information
/// @return DMX_FILTER_STATUS_OK Success
/// @return DMX_FILTER_STATUS_ERROR Failure
/// - The members in pDmxFltInfo->SectInfoEx structure shall be set as follows: \n
///      SectBufAddr : the section buffer physical address \n
///      SectBufSize : the section buffer size \n
///      SectMode    : OR combination of the following: (DMX_SECT_MODE_CONTI and DMX_SECT_MODE_ONESHOT are exclusive) \n
///                    \ref DMX_SECT_MODE_CONTI : Receive section data continuously \n
///                    \ref DMX_SECT_MODE_ONESHOT : Receive one section only \n
///                    \ref DMX_SECT_MODE_CRCCHK : Ask DMX hardware to check CRC32 value of section data and to drop incorrect sections \n
///      Event       : OR combination of the following ( \ref DMX_EVENT_CB_POLL, \ref DMX_EVENT_CB_SELF and \ref DMX_EVENT_CB_SELF_TYPE2 are exclusive) \n
///                    \ref DMX_EVENT_CB_POLL : use polling mode \n
///                    \ref DMX_EVENT_CB_SELF : User set events will be notified to the user using the callback set in the \b pNotify field \n
///                    \ref DMX_EVENT_CB_SELF_TYPE2 : User set events will be notified to the user using the callback set in the \b pType2Notify field. The caller may specify specific user data using the \b Type2NotifyParam1 field \n
///                    \ref DMX_EVENT_DATA_READY : If set, an event will be raised (and notified through callback if \ref DMX_EVENT_CB_SELF or \ref DMX_EVENT_CB_SELF_TYPE2 are set) when one or more complete section data has been captured in the section buffer \n
///                    \ref DMX_EVENT_BUF_OVERFLOW : If set, an event will be raised (and notified through callback if \ref DMX_EVENT_CB_SELF or \ref DMX_EVENT_CB_SELF_TYPE2 are set) if the section buffer overflows \n
///                    \ref DMX_EVENT_DATA_READY : If set, an event will be raised (and notified through callback if \ref DMX_EVENT_CB_SELF or \ref DMX_EVENT_CB_SELF_TYPE2 are set) if CRC32 check fails (assuming \ref DMX_SECT_MODE_CRCCHK has been set in \b SectMode) \n
/// @note Do NOT use MApi_DMX_Info & MApi_DMX_Info_Ex at the same time
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Info_Ex(MS_U32 u32DmxId, DMX_Flt_info_Ex* pDmxFltInfo);

//-------------------------------------------------------------------------------------------------
/// Set a demux filter PID.
/// @ingroup TSP_Filter
/// @param u32DmxId      \b IN: the demux filer index
/// @param pu16Pid        \b IN/OUT: PID
/// @param bSet             \b IN: set/get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pid(MS_U32 u32DmxId, MS_U16* pu16Pid, MS_BOOL bSet);

//-------------------------------------------------------------------------------------------------
/// Get a demux filter status.
/// @ingroup TSP_Filter
/// @param u32DmxId       \b IN: the demux filer index
/// @param pbEnable       \b OUT: the demux filer enable/disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_IsStart(MS_U32 u32DmxId, MS_BOOL* pbEnable);

//--------------------------------------------------------------------------------------------------
/// @brief Copy section data from section buffer to user-assigned buffer
/// @param   u32DmxId               \b IN:  demultiplexer filter ID
/// @param   pu8Buf                 \b OUT: address of the user-assigned destination buffer
/// @param   u32BufSize             \b IN:  size of the user-assigned destination buffer
/// @param   pu32ActualSize         \b OUT: actual copied data size
/// @param   pu32RmnSize            \b OUT: section buffer remainning data size
/// @param   pfCheckCB              \b IN:  callback function to check data correctness (NOTE: callback function must return to avoid blocking)
/// @pre The filter type of the filter corresponding to \b u32DmxId shall be one of \ref DMX_FILTER_TYPE_SECTION, \ref DMX_FILTER_TYPE_PES, \ref DMX_FILTER_TYPE_TELETEXT or \ref DMX_FILTER_TYPE_PACKET
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note If the filter type of the filter corresponding to \b u32DmxId is equal to \ref DMX_FILTER_TYPE_SECTION :
/// - If the data size in the section buffer is smaller than a complete section data length, then this function will return \ref DMX_FILTER_STATUS_ERROR. \n
/// - If \b u32BufSize is smaller than a complete section data length, then this function will print error message and remove a complete section data from the section buffer, without any copy operation.\n
/// - If the data size in the section buffer and \b u32BufSize are both larger than a complete section data length, then this function will copy a complete section data from the section buffer to \b pu8Buf. \n
/// @note If the filter type is equal to \ref DMX_FILTER_TYPE_SECTION, \ref DMX_FILTER_TYPE_PES, \ref DMX_FILTER_TYPE_TELETEXT or \ref DMX_FILTER_TYPE_PACKET :
/// - If \b u32BufSize is larger than all data in the section buffer, the function will copy all data in the section buffer to \b pu8Buf.\n
/// - If \b u32BufSize is smaller than all data in section buffer, the function will copy \b u32BufSize bytes to \b pu8Buf.
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_CopyData(MS_U32 u32DmxId, MS_U8* pu8Buf, MS_U32 u32BufSize, MS_U32* pu32ActualSize, MS_U32* pu32RmnSize, DMX_CheckCb pfCheckCB);

//--------------------------------------------------------------------------------------------------
/// Ask demuxer to process section/pvr data for Non-OS driver.
/// @ingroup TSP_Filter
/// @param  u32DmxId  \b IN: ID of the filter to be processed, or 0xFFFFFFFF for all filters
/// @param  pEvent    \b IN: Pointer to DMX event.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Calling this function in OS environment causes nothing
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Proc(MS_U32 u32DmxId, DMX_EVENT* pEvent);

//--------------------------------------------------------------------------------------------------
/// Change DMX filter source type.
/// @ingroup TSP_Filter
/// @param  u32DmxId             \b IN: DMX filter ID
/// @param  DmxFltSrcType        \b IN: DMX filter Source Type and filter source id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Change_FilterSource(MS_U32 u32DmxId, DMX_FILTER_TYPE DmxFltSrcType);

//--------------------------------------------------------------------------------------------------
///MApi_DMX_FilterFlowSwitch : Switch filter source from one channel to another.
/// @ingroup TSP_Filter
/// @param  TgtFlow        \b IN: chagne to this path setting
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note TSP 1.0:
/// @note DMX_FILTER_SOURCE_TYPE_LIVE
/// @note DMX_FILTER_SOURCE_TYPE_TS1
/// @note TSP 2.0:
/// @note DMX_FILTER_SOURCE_TYPE_PLAYBACK
/// @note DMX_FILTER_SOURCE_TYPE_PVR0
/// @note DMX_FILTER_SOURCE_TYPE_PVR1
/// @note TSP 4.0:
/// @note DMX_FILTER_SOURCE_TYPE_LIVE
/// @note DMX_FILTER_SOURCE_TYPE_FILE
/// @note DMX_FILTER_SOURCE_TYPE_TS1
/// @note DMX_FILTER_SOURCE_TYPE_TS2
/// @note DMX_FILTER_SOURCE_TYPE_TS3
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_LiveSrcSwitch(DMX_FILTER_TYPE TgtFlow);

//--------------------------------------------------------------------------------------------------
/// @brief Get demux filter owner
/// @param  u32DmxId            \b IN: demux ID
/// @param  pbOwner             \b IN: pointer to store return value of owner flag
/// @return DMX_FILTER_STATUS
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_GetOwner(MS_U32 u32DmxId, MS_BOOL* pbOwner); //OBSOLETE

//-----------------
// TSP_SCMB
//-----------------
//-------------------------------------------------------------------------------------------------
/// Get PID filter scramble status.
/// @ingroup TSP_SCMB
/// @param  FltSrc                 \b IN    : Filter source
/// @param  u32FltGroupId          \b IN    : Pid filter group id, every 32 filter for 1 group. 0: filter 0~31, 1: 32~63, 2: 64~95, 3: 96~127
/// @param  PidFltId               \b IN    : Pid filter id for getting. If 0xFFFFFFFF, means getting the status of all filters of the same group
/// @param  pu32ScmbSts            \b OUT : The scrmabling status of pid filter.
/// @return TSP_Result
/// @note pu32ScmbSts: If getting all group status, every bit means one status of every filter.
/// @note If only checking one filter,  1 means scrambled status and 0 means non-scrambled status.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_FltScmbSts(DMX_FILTER_TYPE FltSrc, MS_U32 u32FltGroupId, MS_U32 PidFltId, MS_U32 *pu32ScmbSts);

//-------------------------------------------------------------------------------------------------
/// Get the PES scrambling control status.
/// @ingroup TSP_SCMB
/// @param u32DmxId       \b IN: the demux filer Id
/// @param pu8scmb        \b OUT: the PES scrambling status. bit 0~ bit3, Every bit indicates one status of one packet. bit4 ~ bit7, packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Must open and start filter with DMX_FILTER_TYPE_PACKET type and DMX_SECT_MODE_PESSCMCHK mode
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_PesScmbSts(MS_U32 u32DmxId, MS_U8* pu8scmb);

//-------------------------------------------------------------------------------------------------
/// Get the TS scrambling control status.
/// @ingroup TSP_SCMB
/// @param u32DmxId        \b IN: the demux filer Id
/// @param pu8ScmSts       \b OUT: TS scrambling status. bit 0~ bit3, Every bit indicates one status of one packet. bit4 ~ bit7, packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Must open and start filter with DMX_FILTER_TYPE_PACKET type and DMX_SECT_MODE_PESSCMCHK mode
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_TsScmbSts(MS_U32 u32DmxId, MS_U8* pu8ScmSts);

//-----------------
// TSP_STB
//-----------------
// ---------------- [NOTE] API for TSP Ver2.0 HW architecture.-------------
//-------------------------------------------------------------------------------------------------
/// Open a demux filter and attach to a existed filter and buffer
/// @ingroup TSP_STB
/// @param DmxFltType         \b IN: the filter information to allocate
/// @param pu8DmxId           \b OUT: the available demux filer Id
/// @param u8TargetDmxId      \b IN: the target demux filer Id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Only use on TSP 2.0 (STB)
/// @note API for U4 TSP N filter to 1 Buffer HW architecture.
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_Open_MultiFlt( DMX_FILTER_TYPE DmxFltType , MS_U8* pu8DmxId, MS_U8 u8TargetDmxId);

//-------------------------------------------------------------------------------------------------
/// Enable drop section filter packet.
/// @ingroup TSP_STB
/// @param bSet          \b IN: TRUE: enable, FALSE: disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Only use on TSP 2.0 (STB)
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_Drop_Enable(MS_BOOL bSet);
//--------------------------------------------------------------------------------

//-----------------
// TSP_SecFilter
//-----------------
//-------------------------------------------------------------------------------------------------
/// Reset demux filer buffer.
/// @ingroup TSP_SecFilter
/// @param u32DmxId        \b IN: the target demux filer index
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SectReset(MS_U32 u32DmxId);

//-------------------------------------------------------------------------------------------------
/// Set the read pointer of demux filer buffer.
/// @ingroup TSP_SecFilter
/// @param u32DmxId          \b IN: the target demux filer index
/// @param Read              \b IN: the read pointer to be set
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SectReadSet(MS_U32 u32DmxId, MS_PHY Read);

//-------------------------------------------------------------------------------------------------
/// Get the read pointer of demux filer buffer.
/// @ingroup TSP_SecFilter
/// @param u32DmxId        \b IN: the target demux filer index
/// @param pRead           \b OUT: the pointer to store the obtained read pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SectReadGet(MS_U32 u32DmxId, MS_PHY* pRead);

//-------------------------------------------------------------------------------------------------
/// Get the write pointer of demux filer buffer.
/// @ingroup TSP_SecFilter
/// @param u32DmxId        \b IN: the target demux filer index
/// @param pWrite          \b OUT: the pointer to store the obtained write pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SectWriteGet(MS_U32 u32DmxId, MS_PHY* pWrite);
//-------------------------------------------------------------------------------------------------
/// Get the start pointer of demux filer buffer.
/// @ingroup TSP_SecFilter
/// @param u32DmxId        \b IN: the target demux filer index
/// @param pStart          \b OUT: the pointer to store the obtained start pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SectStartGet(MS_U32 u32DmxId, MS_PHY* pStart);
//-------------------------------------------------------------------------------------------------
/// Get the end pointer of demux filer buffer.
/// @ingroup TSP_SecFilter
/// @param u32DmxId        \b IN: the target demux filer index
/// @param pEnd            \b OUT: the pointer to store the obtained end pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SectEndGet(MS_U32 u32DmxId, MS_PHY* pEnd);
//-------------------------------------------------------------------------------------------------
/// Set demux filer match patterns.
/// @ingroup TSP_SecFilter
/// @param u32DmxId         \b IN: the target demux filer index
/// @param pPattern         \b IN: the match patterns
/// @param pMask            \b IN: the bit mask for match patterns
/// @param pu8NotMask       \b IN: the bit mask for negative match patterns
/// @param MatchSize        \b IN: the size in bytes for match patterns.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note The match pattern size is 16 bytes
/// @note       match mask      -- must set 1 to be compare (customer request)
/// @note       not match mask -- must set 1 to compare
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SectPatternSet(MS_U32 u32DmxId, MS_U8* pPattern, MS_U8* pMask, MS_U8 *pu8NotMask, MS_U32 MatchSize);

//-----------------
// TSP_TTX
//-----------------
//-------------------------------------------------------------------------------------------------
/// Get Teltext section buffer write address.
/// @ingroup TSP_TTX
/// @param u32DmxId          \b IN: the target demux filer index
/// @param pWrite            \b OUT: the pointer to store the obtained write pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Special case for TTX due to it can't use HW lock to do indirect read/write. (co-processer)
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_TTX_WriteGet(MS_U32 u32DmxId, MS_PHY* pWrite);

//-------------------------------------------------------------------------------------------------
/// Check can access TSP or not due to different CPU co-process. (HK and AEON)
/// @ingroup TSP_TTX
/// @param u32Try        \b IN: TRUE: check, FALSE: not check
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_GetAccess(MS_U32 u32Try); //OBSOLETE
//-------------------------------------------------------------------------------------------------
/// Unlock HW semaphore.
/// @ingroup TSP_TTX
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_ReleaseAccess(void); //OBSOLETE

//-----------------
// TSP_FIFO
//-----------------
//--------------------------------------------------------------------------------------------------
/// Reset AVFIFO.
/// @ingroup TSP_FIFO
/// @param  DmxFltType       \b IN: demux type
/// @param  bFlush           \b IN: section buffer size
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE DmxFltType, MS_BOOL bFlush);

//--------------------------------------------------------------------------------------------------
/// Get AVFIFO status (Level).
/// @ingroup TSP_FIFO
/// @param  DmxFltType       \b IN: demux type
/// @param  u32FifoLevel     \b OUT: fifo level
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE DmxFltType, DMX_FIFO_STATUS* u32FifoLevel);
//--------------------------------------------------------------------------------------------------
/// Enable remove duplicate A/V packets.
/// @ingroup DMX_ToBeRemove
/// @param  bEnable          \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note It will apply to all FIFO.
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_RemoveDupAVPkt(MS_BOOL bEnable);

//--------------------------------------------------------------------------------------------------
/// Enable remove duplicate A/V Fifo packets.
/// @ingroup TSP_FIFO
/// @param  DmxFltType    \b IN: demux type
/// @param  bEnable       \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_RemoveDupAVFifoPkt(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable);

//--------------------------------------------------------------------------------------------------
/// Enable/Disable Audio AB S
/// @ingroup TSP_FIFO
/// @param bEnable           \b IN: Enable/Disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_AU_BD_Mode_Enable(MS_BOOL bEnable);

//--------------------------------------------------------------------------------------------------
/// Enable/Disable AV FIFO watermark mechanism.
/// @ingroup TSP_FIFO
/// @param  bEnable           \b IN: TRUE: Enable watermark, FALSE: Disable watermark
/// @param  DmxFltType        \b IN: fifo type
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Not supprot for all chips
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_Tracemark_Enable(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable);

//--------------------------------------------------------------------------------------------------
/// Enable/Disable AV FIFO block mechanism.
/// @ingroup TSP_FIFO
/// @param  DmxFltType       \b IN: demux type
/// @param  bEnable            \b IN: TRUR: Enable block mechanism, FALSE: Disable block mechanism
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Not supprot for all chips
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_AVFifo_BlockEnable(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable);

//--------------------------------------------------------------------------------------------------
/// Bypass TS Header to AV Fifo
/// @param  DmxFltType       \b IN: demux type
/// @param  bEnable            \b IN: TRUR: Enable TS header bypass, FALSE: Disable TS header bypass
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Not supprot for all chips
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_AVFifo_BypassTsHD(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable);

// PVR Record API (Old)
//-------------------------------------------------------------------------------------------------
/// Set up the recording parameters.
/// @ingroup DMX_ToBeRemove
/// @param pPvrInfo      \b IN: the recording parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Open(DMX_Pvr_info* pPvrInfo);
//-------------------------------------------------------------------------------------------------
/// Clear the recording parameters.
/// @ingroup DMX_ToBeRemove
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Close(void);
//-------------------------------------------------------------------------------------------------
/// Pause/Resume PVR engine.
/// @ingroup DMX_ToBeRemove
/// @param bPause          \b IN: TRUE: pause, FALSE: resume
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Pause(MS_BOOL bPause);
//-------------------------------------------------------------------------------------------------
/// Set the PID to be recorded.
/// @ingroup DMX_ToBeRemove
/// @param Pid                 \b IN: The target PID to be recorded
/// @param pu8DmxId            \b OUT: The demux filter Id for this recorded PID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
/// @note Do NOT use MApi_DMX_Pvr_Pid_Open & MApi_DMX_Open_Ex at the same time
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Pid_Open(MS_U32 Pid, MS_U8* pu8DmxId);

//-------------------------------------------------------------------------------------------------
/// Free the PID to be recorded.
/// @ingroup DMX_ToBeRemove
/// @param u8DmxId        \b IN: The demux filter Id from MApi_DMX_Pvr_Pid_Open
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Pid_Close(MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Start to record.
/// @ingroup DMX_ToBeRemove
/// @param bPvrAll           \b IN: TRUE to record original bit stream, FALSE to recording the PIDs of interests
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Start(MS_BOOL bPvrAll);

//-------------------------------------------------------------------------------------------------
/// Stop recording.
/// @ingroup DMX_ToBeRemove
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Stop(void);

//-------------------------------------------------------------------------------------------------
/// Get write pointer of recording buffer.
/// @ingroup DMX_ToBeRemove
/// @param pphyWrite       \b OUT: the pointer to store the obtained write pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_WriteGet(MS_PHY* pphyWrite);

//-------------------------------------------------------------------------------------------------
/// Set recording packet mode.
/// @ingroup DMX_ToBeRemove
/// @param bSet               \b IN: TRUE: enable 192 mode, FALSE: disable 192 mode
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PVR_SetPacketMode(MS_BOOL bSet);

//-------------------------------------------------------------------------------------------------
/// Set recording starting time stamp.
/// @ingroup DMX_ToBeRemove
/// @param u32Stamp       \b IN: Record time-stamp (LPCR)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_SetRecordStamp(MS_U32 u32Stamp);

//-------------------------------------------------------------------------------------------------
/// Get current recording time stamp.
/// @ingroup DMX_ToBeRemove
/// @param pu32Stamp     \b OUT: the pointer to store the obtained record time-stamp (LPCR)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_GetRecordStamp(MS_U32* pu32Stamp);

//-------------------------------------------------------------------------------------------------
/// Enable MOBF encrypt.
/// @ingroup DMX_ToBeRemove
/// @param bEnable          \b IN: PVR MOBF Enable/Disable
/// @param u32key0          \b IN: MOBF encrypt key0
/// @param u32key1          \b IN: MOBF encrypt key1
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_MOBF_Enable(MS_BOOL bEnable, MS_U32 u32key0, MS_U32 u32key1);

//PVR Playback API (Old)
//-------------------------------------------------------------------------------------------------
/// Set playback time stamp.
/// @ingroup TSP_FILEIN_PVR
/// @param u32Stamp        \b IN: time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_SetPlaybackStamp(MS_U32 u32Stamp);
//-------------------------------------------------------------------------------------------------
/// Get playback time stamp
/// @ingroup TSP_FILEIN_PVR
/// @param pu32Stamp        \b OUT: time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_GetPlaybackStamp(MS_U32* pu32Stamp);
//-------------------------------------------------------------------------------------------------
/// Enable filein time stamp. (filein use 192 mode)
/// @ingroup DMX_ToBeModified_TSP
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note:  Merge with MApi_DMX_Pvr_TimeStampDisable
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_TimeStampEnable(void);
//-------------------------------------------------------------------------------------------------
/// Disable filein time stamp. (filein not use 192 mode)
/// @ingroup DMX_ToBeModified_TSP
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note:  Merge with MApi_DMX_Pvr_TimeStampEnable
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_TimeStampDisable(void);


//-----------------
// TSP_PVR_General
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set up the recording parameters.
/// @ingroup TSP_PVR_General
/// @param Eng               \b IN: PVR engine ID
/// @param pPvrInfo          \b IN: the recording parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Open(DMX_PVR_ENG Eng ,DMX_Pvr_info* pPvrInfo);

//-------------------------------------------------------------------------------------------------
/// Clear the recording parameters.
/// @ingroup TSP_PVR_General
/// @param Eng               \b IN: PVR engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Close(DMX_PVR_ENG Eng );

//-------------------------------------------------------------------------------------------------
/// Pause/Resume PVR engine.
/// @ingroup TSP_PVR_General
/// @param Eng               \b IN: PVR engine ID
/// @param bPause            \b IN: TRUE: pause, FALSE: resume
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Pause(DMX_PVR_ENG Eng ,MS_BOOL bPause);

//-------------------------------------------------------------------------------------------------
/// Set the PID to be recorded.
/// @ingroup TSP_PVR_General
/// @param Eng                 \b IN: PVR engine ID
/// @param Pid                 \b IN: The target PID to be recorded
/// @param pu8DmxId            \b OUT: The demux filter Id for this recorded PID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Do NOT use MApi_DMX_Pvr_Eng_Pid_Open & MApi_DMX_Open_Ex at the same time
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Pid_Open(DMX_PVR_ENG Eng ,MS_U32 Pid, MS_U8* pu8DmxId);

//-------------------------------------------------------------------------------------------------
/// Free the PID to be recorded.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param u8DmxId            \b IN: The demux filter Id from MApi_DMX_Pvr_Pid_Open
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Pid_Close(DMX_PVR_ENG Eng ,MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Start to record.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param bPvrAll            \b IN: TRUE to record original bit stream, FALSE to recording the PIDs of interests
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Start(DMX_PVR_ENG Eng ,MS_BOOL bPvrAll);

//-------------------------------------------------------------------------------------------------
/// Stop recording.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Stop(DMX_PVR_ENG Eng );

//-------------------------------------------------------------------------------------------------
/// Get write pointer of recording buffer.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param pphyWrite       \b OUT: the pointer to store the obtained write pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_WriteGet(DMX_PVR_ENG Eng ,MS_PHY* pphyWrite);

//-------------------------------------------------------------------------------------------------
/// Set recording packet mode.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param bSet               \b IN: TRUE: enable 192 mode, FALSE: disable 192 mode
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_SetPacketMode(DMX_PVR_ENG Eng ,MS_BOOL bSet);

//-------------------------------------------------------------------------------------------------
/// Set the recording packet mode (extension version).
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param ePvrPktMode        \b IN: DMX record packet type
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_Pvr_Eng_SetPacketMode_Ex(DMX_PVR_ENG Eng, DMX_Pvr_PacketMode ePvrPktMode);

//-------------------------------------------------------------------------------------------------
/// Set recording starting time stamp.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param u32Stamp           \b IN: Record time-stamp (LPCR)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_SetRecordStamp(DMX_PVR_ENG Eng ,MS_U32 u32Stamp);

//-------------------------------------------------------------------------------------------------
/// Get current recording time stamp.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param pu32Stamp          \b OUT: the pointer to store the obtained record time-stamp (LPCR)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_GetRecordStamp(DMX_PVR_ENG Eng ,MS_U32* pu32Stamp);

//-------------------------------------------------------------------------------------------------
/// Enable MOBF encrypt.
/// @ingroup TSP_PVR_General
/// @param u8Eng               \b IN: PVR engine ID
/// @param bEnable             \b IN: PVR MOBF Enable/Disable
/// @param u32key0             \b IN: MOBF encrypt key0
/// @param u32key1             \b IN: MOBF encrypt key1
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_MOBF_Enable(MS_U8 u8Eng, MS_BOOL bEnable, MS_U32 u32key0, MS_U32 u32key1);

//-------------------------------------------------------------------------------------------------
/// Check if PVR engine is started.
/// @ingroup TSP_PVR_General
/// @param Eng                 \b IN: PVR engine ID
/// @param pbIsStart           \b OUT: Pointer to store PVR engine starting status
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_IsStart(DMX_PVR_ENG Eng, MS_BOOL *pbIsStart);

//-----------------
// TSP_PVR_STB
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set/Get PVR PID filter PID.
/// @ingroup TSP_PVR_STB
/// @param Eng                 \b IN: PVR engine ID
/// @param u8DmxId             \b IN: PID filter index
/// @param Pid                 \b IN: PID value
/// @param bSet                \b IN: TRUE: set, FALSE: get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Only TSP 2.0/4.0 use this function now.
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Pid(DMX_PVR_ENG Eng ,MS_U8 u8DmxId , MS_U32 *Pid, MS_BOOL bSet);
//-------------------------------------------------------------------------------------------------
/// Set PVR time-stamp clock.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param eClk               \b IN: select 90K or 27M
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_SetRecordStampClk(DMX_PVR_ENG Eng, DMX_TimeStamp_Clk eClk);
//-------------------------------------------------------------------------------------------------
/// Set RASP callback engine size.
/// @ingroup TSP_PVR_STB
/// @param Eng                \b IN: PVR engine ID
/// @param pu32CallbackSize               \b IN: select 90K or 27M
/// @param bSet
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Only TSP 2.0 use this function now.
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_CallbackSize(DMX_PVR_ENG Eng, MS_U32* pu32CallbackSize, MS_BOOL bSet);
//-------------------------------------------------------------------------------------------------
///Set PVR CA SPS enable mode
/// @ingroup TSP_PVR_STB
/// @param Eng                  \b IN: PVR Engine ID
/// @param eCaMode              \b IN: CA PVR Selection, 0: from TSP, 1: from ca program0, 2: from ca program1
/// @param bspsEnable           \b IN: SPS mode enabled flag
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_SetCaMode(DMX_PVR_ENG Eng, DMX_CA_PVRMODE eCaMode, MS_BOOL bspsEnable);

//-------------------------------------------------------------------------------------------------
///Flush PVR remaining data
/// @ingroup TSP_PVR_STB
/// @param Eng                  \b IN: PVR Engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_FlushData(DMX_PVR_ENG Eng);

//-------------------------------------------------------------------------------------------------
///Check if PVR flush data operation is done.
/// @ingroup TSP_PVR_STB
/// @param Eng                  \b IN: PVR Engine ID
/// @param pbDone               \b OUT: TRUE-> if done, FALSE-> otherwise
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @return DMX_FILTER_STATUS_NOT_SUPPORT - Not support
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_IsFlushDone(DMX_PVR_ENG Eng, MS_BOOL *pbDone);

//-------------------------------------------------------------------------------------------------
/// Set playback time-stamp clock.
/// @ingroup TSP_FILEIN_PVR
/// @param u8Eng              \b IN: PVR engine ID
/// @param eClk               \b IN: select 90K or 27M
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_SetPlaybackStampClk(MS_U8 u8Eng, DMX_TimeStamp_Clk eClk);

//PVR CA API (Old)
//-------------------------------------------------------------------------------------------------
/// Open PVR PID filter for recording scramble stream depended on Engine ID.
/// @ingroup DMX_ToBeRemove
/// @param Pid                         \b IN: PID to record
/// @param pu8DmxId                    \b IN: Pointer to store PVR PID filter index
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
/// @note Do NOT use MApi_DMX_PvrCA_Pid_Open & MApi_DMX_Open_Ex at the same time
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Pid_Open(MS_U32 Pid, MS_U8* pu8DmxId);

//-------------------------------------------------------------------------------------------------
/// Close PVR PID filter depended on Engine ID.
/// @ingroup DMX_ToBeRemove
/// @param u8DmxId         \b IN: PID filter index to close
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Pid_Close(MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Start to record scramble stream depend on engine id.
/// @ingroup DMX_ToBeRemove
/// @param bPvrAll             \b IN: If true, record all stream data; if false, record data by PIDs
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Start(MS_BOOL bPvrAll);
//-------------------------------------------------------------------------------------------------
/// Stop to record scramble stream.
/// @ingroup DMX_ToBeRemove
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Stop(void);

//-----------------
// TSP_PVR_CA
//-----------------
//-------------------------------------------------------------------------------------------------
/// Open PVR PID filter for recording scramble stream depended on Engine ID.
/// @ingroup TSP_PVR_CA
/// @param Eng                         \b IN: PVR engine ID
/// @param Pid                         \b IN: PID to record
/// @param pu8DmxId                    \b IN: Pointer to store PVR PID filter index
/// @param u8ShareKeyType              \b IN: PID filter share key type. 0: Not use sharekey filters.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Do NOT use MApi_DMX_PvrCA_Eng_Pid_Open & MApi_DMX_Open_Ex at the same time
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Eng_Pid_Open(DMX_PVR_ENG Eng, MS_U32 Pid, MS_U8* pu8DmxId, MS_U8 u8ShareKeyType);
//-------------------------------------------------------------------------------------------------
/// Close PVR PID filter depended on Engine ID.
/// @ingroup TSP_PVR_CA
/// @param Eng                 \b IN: PVR engine ID
/// @param u8DmxId             \b IN: PID filter index to close
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Eng_Pid_Close(DMX_PVR_ENG Eng, MS_U8 u8DmxId);
//-------------------------------------------------------------------------------------------------
/// Start to record scramble stream depend on engine id.
/// @ingroup TSP_PVR_CA
/// @param Eng                  \b IN: PVR engine ID
/// @param bPvrAll              \b IN: If true, record all stream data; if false, record data by PIDs
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Eng_Start(DMX_PVR_ENG Eng, MS_BOOL bPvrAll);
//-------------------------------------------------------------------------------------------------
/// Stop to record scramble stream.
/// @ingroup TSP_PVR_CA
/// @param Eng                  \b IN: PVR engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Eng_Stop(DMX_PVR_ENG Eng);
//---------------------------------------------

//-----------------
// TSP_FILEIN_PVR
//-----------------
//-------------------------------------------------------------------------------------------------
/// Open & set pid filter for file-in PVR using.
/// @ingroup TSP_FILEIN_PVR
/// @param Eng                             \b IN   : PVR eigine id
/// @param Pid                             \b IN   : PVR PID to record
/// @param pu8DmxId                        \b OUT: Pointer to store PID filter index
/// @param u8ShareKeyType                  \b IN   : The flag of sharekey filter using. if 0, not use sharekey filters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Pvr_Eng_Pid_Open(DMX_PVR_ENG Eng, MS_U32 Pid, MS_U8* pu8DmxId, MS_U8 u8ShareKeyType);

//-------------------------------------------------------------------------------------------------
/// Close pid filter of file-in PVR.
/// @ingroup TSP_FILEIN_PVR
/// @param Eng                        \b IN   : PVR eigine id
/// @param u8DmxId                    \b IN   : PID filter id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_Filein_Pvr_Eng_Pid_Close(DMX_PVR_ENG Eng, MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Start file-in PVR recording.
/// @ingroup TSP_FILEIN_PVR
/// @param Eng                         \b IN   : PVR eigine id
/// @param bPvrAll                     \b IN   : FALSE is record PID, TRUE is record All ts data
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Pvr_Eng_Start(DMX_PVR_ENG Eng, MS_BOOL bPvrAll);

//-------------------------------------------------------------------------------------------------
/// Stop file-in PVR recording.
/// @ingroup TSP_FILEIN_PVR
/// @param Eng                         \b IN   : PVR eigine id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_Filein_Pvr_Eng_Stop(DMX_PVR_ENG Eng);
//---------------------------------------------


// File-in API (Old)
//-------------------------------------------------------------------------------------------------
/// Start to get bit stream from memory.
/// @ingroup DMX_ToBeRemove
/// @param Dst              \b IN: file in destination type
/// @param pBuf             \b IN: the memory containing the bit stream
/// @param u32BufSize       \b IN: the size the memory to get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Start(DMX_FILEIN_DST Dst, MS_PHY pBuf, MS_U32 u32BufSize);   // msAPI_DMX_PESPlayback/msAPI_DMX_TSPlayback will integrate into it
//-------------------------------------------------------------------------------------------------
/// Stop to get bit stream from memeory.
/// @ingroup DMX_ToBeRemove
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Stop(void);

//-------------------------------------------------------------------------------------------------
/// Set up parameters for input bit stream from memory.
/// @ingroup DMX_ToBeRemove
/// @param pFileinInfo      \b IN: the file in parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Info(DMX_Filein_info *pFileinInfo);
//-------------------------------------------------------------------------------------------------
/// Pause to get bit stream by memeory.
/// @ingroup DMX_ToBeRemove
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Pause(void);
//-------------------------------------------------------------------------------------------------
/// Pause to get bit stream by memeory.
/// @ingroup DMX_ToBeRemove
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Resume(void);

//-------------------------------------------------------------------------------------------------
/// Check if no memory transfer is under going.
/// @ingroup DMX_ToBeRemove
/// @return TRUE - if idle
/// @return FALSE - otherwise
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
MS_BOOL           MApi_DMX_Filein_IsIdle(void);

//-------------------------------------------------------------------------------------------------
/// Check if memory transfer is under going.
/// @ingroup DMX_ToBeRemove
/// @return TRUE - if idle
/// @return FALSE - otherwise
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
MS_BOOL           MApi_DMX_Filein_IsBusy(void);

//-------------------------------------------------------------------------------------------------
/// Check if memory transfer is paused.
/// @ingroup DMX_ToBeRemove
/// @return TRUE - if idle
/// @return FALSE - otherwise
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
MS_BOOL           MApi_DMX_Filein_IsPause(void);

//-------------------------------------------------------------------------------------------------
/// Reset CMDQ.
/// @ingroup DMX_ToBeRemove
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_CMDQ_Reset(void);

//-------------------------------------------------------------------------------------------------
/// Get the number of CMDQ empty slot.
/// @ingroup DMX_ToBeRemove
/// @param pu32EmptySlot      \b OUT: empty slot number
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
/// @note The user can use MApi_DMX_Filein_Start() as a multimedia file-in command to input data from memory to the demultiplexer. \n
/// The DMX engine allows to add multimedia file-in command without having to wait that the previous multimedia file-in command have been processed completely. \n
/// In this mode the waited multimedia file-in commands are stored to the so-called multimedia file-in command queue. \n
/// However, the size of the command queue is limited and each multimedia file-in command will consumes 5 slots in the multimedia file-in command queue. \n
/// Therefore, the caller has to check the number of empty slots in the multimedia file-in command queue with MApi_DMX_Filein_CMDQ_GetEmptyNum()
/// before adding a new multimedia file-in command.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_CMDQ_GetEmptyNum(MS_U32 *pu32EmptySlot);

//-------------------------------------------------------------------------------------------------
/// Disable 192 mode blovk scheme to bypass fill-in timestamp
/// @ingroup DMX_ToBeRemove
/// @param bbypass     \b IN: TRUE: bypass file-in timestamp, FALSE: not bypass
/// @return None
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
void MApi_DMX_BypassFileInTimeStamp(MS_BOOL bbypass);
//-------------------------------------------------------------------------------------------------
/// Get Command queue FIFO level.
/// @ingroup DMX_ToBeRemove
/// @param  pu8CmdQStatus   \b OUT: fifo level, 0~3
/// - 0: 0 to 25% full
/// - 1: 25 to 50% full
/// - 2: 50 to 75% full
/// - 3: 75 to 100% full
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_CMDQ_FIFOWriteLevel(MS_U8 *pu8CmdQStatus);

//-------------------------------------------------------------------------------------------------
/// Get filein time stamp. (Current time stamp from TS packet). It is used on 192 packet mode.
/// @ingroup DMX_ToBeRemove
/// @param pu32FileInTS     \b OUT: pointer to store time-stamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_GetFileInTimeStamp(MS_U32 *pu32FileInTS);

//-------------------------------------------------------------------------------------------------
/// Get playback read address.
/// @ingroup DMX_ToBeRemove
/// @param pphyRead        \b OUT: pointer to store read address
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_GetReadAddr(MS_PHY* pphyRead);

//-------------------------------------------------------------------------------------------------
/// Enable MOBF decrypt key.
/// @ingroup DMX_ToBeRemove
/// @param bEnable    \b IN: File-in MOBF Enable/Disable
/// @param u32key     \b IN: MOBF dyncrypt u32key
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_MOBF_Enable(MS_BOOL bEnable, MS_U32 u32key);

//-----------------
// TSP_Playback
//-----------------
//-------------------------------------------------------------------------------------------------
/// Start to get bit stream from memory.
/// @ingroup TSP_Playback
/// @param ePath            \b IN: file in path (TSIF)
/// @param Dst              \b IN: file in destination type
/// @param pBuf             \b IN: the memory containing the bit stream
/// @param u32BufSize       \b IN: the size the memory to get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_Start(DMX_FILEIN_PATH ePath, DMX_FILEIN_DST Dst, MS_PHY pBuf, MS_U32 u32BufSize);   // msAPI_DMX_PESPlayback/msAPI_DMX_TSPlayback will integrate into it
//-------------------------------------------------------------------------------------------------
/// Stop to get bit stream from memeory.
/// @ingroup TSP_Playback
/// @param ePath            \b IN: file in path (TSIF)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_Filein_Eng_Stop(DMX_FILEIN_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Set up parameters for input bit stream from memory.
/// @ingroup TSP_Playback
/// @param ePath             \b IN: file in path (TSIF)
/// @param pFileinInfo       \b IN: the file in parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_Info(DMX_FILEIN_PATH ePath, DMX_Filein_info *pFileinInfo);
//-------------------------------------------------------------------------------------------------
/// Pause to get bit stream by memeory.
/// @ingroup TSP_Playback
/// @param ePath             \b IN: file in path (TSIF)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_Pause(DMX_FILEIN_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Pause to get bit stream by memeory.
/// @ingroup TSP_Playback
/// @param ePath             \b IN: file in path (TSIF)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_Resume(DMX_FILEIN_PATH ePath);

//-------------------------------------------------------------------------------------------------
/// Check if no memory transfer is under going.
/// @ingroup TSP_Playback
/// @param ePath             \b IN: file in path (TSIF)
/// @return TRUE - if idle
/// @return FALSE - otherwise
//-------------------------------------------------------------------------------------------------
MS_BOOL           MApi_DMX_Filein_Eng_IsIdle(DMX_FILEIN_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Check if memory transfer is under going.
/// @ingroup TSP_Playback
/// @param ePath             \b IN: file in path (TSIF)
/// @return TRUE - if idle
/// @return FALSE - otherwise
//-------------------------------------------------------------------------------------------------
 MS_BOOL          MApi_DMX_Filein_Eng_IsBusy(DMX_FILEIN_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Check if memory transfer is paused.
/// @ingroup TSP_Playback
/// @param ePath             \b IN: file in path (TSIF)
/// @return TRUE - if idle
/// @return FALSE - otherwise
//-------------------------------------------------------------------------------------------------
 MS_BOOL          MApi_DMX_Filein_Eng_IsPause(DMX_FILEIN_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Reset CMDQ.
/// @ingroup TSP_Playback
/// @param ePath             \b IN: file in path (TSIF)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_CMDQ_Reset(DMX_FILEIN_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Get the number of CMDQ empty slot.
/// @ingroup TSP_Playback
/// @param ePath                   \b IN: file in path (TSIF)
/// @param pu32EmptySlot           \b OUT: empty slot number
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note The user can use MApi_DMX_Filein_Start() as a multimedia file-in command to input data from memory to the demultiplexer. \n
/// The DMX engine allows to add multimedia file-in command without having to wait that the previous multimedia file-in command have been processed completely. \n
/// In this mode the waited multimedia file-in commands are stored to the so-called multimedia file-in command queue. \n
/// However, the size of the command queue is limited and each multimedia file-in command will consumes 5 slots in the multimedia file-in command queue. \n
/// Therefore, the caller has to check the number of empty slots in the multimedia file-in command queue with MApi_DMX_Filein_CMDQ_GetEmptyNum()
/// before adding a new multimedia file-in command
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_CMDQ_GetEmptyNum(DMX_FILEIN_PATH ePath, MS_U32 *pu32EmptySlot);
//-------------------------------------------------------------------------------------------------
/// Disable 192 mode blovk scheme to bypass fill-in timestamp
/// @ingroup TSP_Playback
/// @param ePath         \b IN: file in path (TSIF)
/// @param bbypass       \b IN: TRUE: bypass file-in timestamp, FALSE: not bypass
/// @return None
//-------------------------------------------------------------------------------------------------
void              MApi_DMX_Filein_Eng_BypassFileInTimeStamp(DMX_FILEIN_PATH ePath, MS_BOOL bbypass);
//-------------------------------------------------------------------------------------------------
/// Get Command queue FIFO level.
/// @ingroup TSP_Playback
/// @param ePath                   \b IN: file in path (TSIF)
/// @param  pu8CmdQStatus          \b OUT: fifo level, 0~3
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_CMDQ_FIFOWriteLevel(DMX_FILEIN_PATH ePath, MS_U8 *pu8CmdQStatus);

//-------------------------------------------------------------------------------------------------
/// Get filein time stamp. (Current time stamp from TS packet). It is used on 192 packet mode.
/// @ingroup TSP_Playback
/// @param ePath                \b IN: file in path (TSIF)
/// @param pu32FileInTS         \b OUT: pointer to store time-stamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_GetFileInTimeStamp(DMX_FILEIN_PATH ePath, MS_U32 *pu32FileInTS);

//-------------------------------------------------------------------------------------------------
/// Get playback read address.
/// @ingroup TSP_Playback
/// @param ePath              \b IN: file in path (TSIF)
/// @param pphyRead           \b OUT: pointer to store read address
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_GetReadAddr(DMX_FILEIN_PATH ePath, MS_PHY* pphyRead);
//-------------------------------------------------------------------------------------------------
/// Enable MOBF decrypt key.
/// @ingroup TSP_Playback
/// @param ePath        \b IN: file in path (TSIF)
/// @param bEnable      \b IN: File-in MOBF Enable/Disable
/// @param u32key       \b IN: MOBF dyncrypt u32key
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_MOBF_Enable(DMX_FILEIN_PATH ePath, MS_BOOL bEnable, MS_U32 u32key);

//-------------------------------------------------------------------------------------------------
/// Enable FileEng Playback time stamp.
/// @ingroup TSP_Playback
/// @param ePath         \b IN: file in path (TSIF)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_PlaybackTimeStampEnable(DMX_FILEIN_PATH ePath);

//-------------------------------------------------------------------------------------------------
/// Disable fileEng Playback time stamp.
/// @ingroup TSP_Playback
/// @param ePath         \b IN: file in path (TSIF)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_PlaybackTimeStampDisable(DMX_FILEIN_PATH ePath);

//-------------------------------------------------------------------------------------------------
/// Set playback time stamp. (LPCR)
/// @ingroup TSP_Playback
/// @param ePath              \b IN: file in path (TSIF)
/// @param u32Stamp           \b IN: time-stamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_SetPlaybackStamp(DMX_FILEIN_PATH ePath,MS_U32 u32Stamp);
//-------------------------------------------------------------------------------------------------
/// Get playback time stamp. (LPCR)
/// @ingroup TSP_Playback
/// @param ePath              \b IN: file in path (TSIF)
/// @param pu32Stamp          \b OUT: pointer to store time-stamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_GetPlaybackStamp(DMX_FILEIN_PATH ePath,MS_U32* pu32Stamp);


//-------------------------------------------------------------
//MMFI & MINITSP API
//-----------------
// TSP_MM
//-----------------
//-------------------------------------------------------------------------------------------------
/// Check mmfi engine is ilde or not.
/// @ingroup TSP_MM
/// @param  ePath     \b IN: MM File-in Path
/// @return TRUE - Idle
/// @return FALSE - not idle
//-------------------------------------------------------------------------------------------------
MS_BOOL           MApi_DMX_MMFI_Filein_IsIdle(DMX_MMFI_PATH ePath);

//-------------------------------------------------------------------------------------------------
/// Check mmfi engine is busy or not.
/// @ingroup TSP_MM
/// @param  ePath      \b IN: MM File-in Path
/// @return TRUE - Busy
/// @return FALSE - not busy
//-------------------------------------------------------------------------------------------------
MS_BOOL          MApi_DMX_MMFI_Filein_IsBusy(DMX_MMFI_PATH ePath);

//-------------------------------------------------------------------------------------------------
/// Reset MMFilein Command queue
/// @ingroup TSP_MM
/// @param  ePath       \b IN: MM File-in Path
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_Filein_CMDQ_Reset(DMX_MMFI_PATH ePath);

//-------------------------------------------------------------------------------------------------
/// Get the MMFilein CMDQ empty slot number.
/// @ingroup TSP_MM
/// @param  ePath                  \b IN: MM File-in Path
/// @param  pu32EmptySlot          \b OUT: Empty slot
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note The user can use MApi_DMX_MMFI_Filein_Start() as a multimedia file-in command to input data from memory to the demultiplexer. \n
/// The DMX engine allows to add multimedia file-in command without having to wait that the previous multimedia file-in command have been processed completely. \n
/// In this mode the waited multimedia file-in commands are stored to the so-called multimedia file-in command queue. \n
/// However, the size of the command queue is limited and each multimedia file-in command will consumes 5 slots in the multimedia file-in command queue. \n
/// Therefore, the caller has to check the number of empty slots in the multimedia file-in command queue with MApi_DMX_MMFI_Filein_CMDQ_GetEmptyNum()
/// before adding a new multimedia file-in command.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_Filein_CMDQ_GetEmptyNum(DMX_MMFI_PATH ePath, MS_U32 *pu32EmptySlot);

//-------------------------------------------------------------------------------------------------
/// Start to get bit stream by memeory.
/// @ingroup TSP_MM
/// @param eDst             \b IN: file in destination path
/// @param pBuf             \b IN: the memory containing the bit stream
/// @param u32BufSize       \b IN: the size the memory to get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_Filein_Start(DMX_MMFI_DST eDst, MS_PHY pBuf, MS_U32 u32BufSize);

//-------------------------------------------------------------------------------------------------
/// Set up parameters for input bit stream from memory.
/// @ingroup TSP_MM
/// @param  ePath           \b IN: MM File-in Path
/// @param pFileinInfo      \b IN: the file in parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_Filein_Info(DMX_MMFI_PATH ePath, DMX_Filein_info *pFileinInfo);

//-------------------------------------------------------------------------------------------------
/// Disable 192 mode blovk scheme to bypass fill-in timestamp
/// @ingroup TSP_MM
/// @param  ePath       \b IN: MM File-in Path
/// @param bbypass      \b IN: If true, bypass file-in timestamp.
/// @return None
//-------------------------------------------------------------------------------------------------
void             MApi_DMX_MMFI_Filein_BypassTimeStamp(DMX_MMFI_PATH ePath, MS_BOOL bbypass);
//-------------------------------------------------------------------------------------------------
/// Free the PID of MM Filein.
/// @ingroup TSP_MM
/// @param  ePath                \b IN: MM File-in Path
/// @param pu32FileInTS          \b OUT: pointer for timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_GetFileInTimeStamp(DMX_MMFI_PATH ePath, MS_U32 *pu32FileInTS);

//MMFI  (MMFI Only) API
//-------------------------------------------------------------------------------------------------
/// Set the PID to be MM File-in
/// @ingroup TSP_MM
/// @param flttype         \b IN: MMFI filter type
/// @param u16Pid          \b IN: The target PID for MM Filein
/// @param pu8DmxId        \b OUT: The demux filter Id for this MMFilein PID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_Pid_Open(DMX_MMFI_FLTTYPE flttype, MS_U16 u16Pid, MS_U8* pu8DmxId);

//-------------------------------------------------------------------------------------------------
/// Free the PID of MM Filein.
/// @ingroup TSP_MM
/// @param u8DmxId      \b IN: The demux filter Id from MApi_DMX_Pvr_Pid_Open
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_Pid_Close(MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Get MMFilein Command queue fifo level.
/// @ingroup TSP_MM
/// @param  ePath                   \b IN: MM File-in Path
/// @param  pu8CmdQStatus           \b OUT: fifo level, 0~3
/// - 0: 0 to 25% full
/// - 1: 25 to 50% full
/// - 2: 50 to 75% full
/// - 3: 75 to 100% full
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_Filein_CMDQ_FIFOWriteLevel(DMX_MMFI_PATH ePath, MS_U8 *pu8CmdQStatus);

//-------------------------------------------------------------------------------------------------
/// Set MMFI playback timestamp.
/// @ingroup TSP_MM
/// @param  ePath          \b IN: MM File-in Path
/// @param u32pcr2         \b IN: LPCR2 value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_SetPlaybackTimeStamp(DMX_MMFI_PATH ePath, MS_U32 u32pcr2);

//-------------------------------------------------------------------------------------------------
/// Get MMFI Playback timestamp.
/// @ingroup TSP_MM
/// @param ePath               \b IN: MM File-in Path
/// @param pu32pcr2            \b OUT: pointer to store LCPR2 value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_GetPlaybackTimeStamp(DMX_MMFI_PATH ePath, MS_U32 *pu32pcr2);

//-------------------------------------------------------------------------------------------------
/// Enable remove duplicate A/V packets.
/// @ingroup TSP_MM
/// @param  bEnable           \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_RemoveDupAVPkt(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Enable MMFI timestamp mode.
/// @ingroup DMX_ToBeModified_MM
/// @param ePath               \b IN: MM File-in Path
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note:  Merge with MApi_DMX_MMFI_TimeStampDisable
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_TimeStampEnable(DMX_MMFI_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Disable MMFI timestamp mode.
/// @ingroup DMX_ToBeModified_MM
/// @param ePath               \b IN: MM File-in Path
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note:  Merge with MApi_DMX_MMFI_TimeStampEnable
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_TimeStampDisable(DMX_MMFI_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Enable/Disable MMFI MOBF function and set MOBF key index.
/// @ingroup TSP_MM
/// @param ePath              \b IN: MM File-in Path
/// @param  bEnable           \b IN: Enable or Disable
/// @param  u32key            \b IN: MOBF key index
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_MOBF_Enable(DMX_MMFI_PATH ePath, MS_BOOL bEnable, MS_U32 u32key);

//-------------------------------------------------------------------------------------------------
/// Set MMFI MOBF level.
/// @ingroup TSP_MM
/// @param epath              \b IN: MM File-in Path
/// @param  u8level           \b IN: the value of MOBF level
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note only used on TSP 1.0
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_MOBF_SetLevel(DMX_MMFI_PATH epath, MS_U8 u8level);
//-------------------------------------------------------------------------------------------------
/// Set MMFI TimeStamp Clk.
/// @ingroup TSP_MM
/// @param ePath              \b IN: MM File-in Path
/// @param eClk               \b IN: select 90K or 27M
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note only used on TSP 1.0
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_TimeStampClk(DMX_MMFI_PATH ePath, DMX_TimeStamp_Clk eClk);

//-------------------------------------------------------------


//-----------------
// TSO_General
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set up parameters for input bit stream from memory of TSO path.
/// @ingroup TSO_General
/// @param u8Eng          \b IN: TSO engine ID
/// @param pFileinInfo    \b IN: the file in parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Filein_Info(MS_U8 u8Eng, DMX_Filein_info *pFileinInfo);

//-------------------------------------------------------------------------------------------------
/// Check if no memory transfer is under going of TSO path
/// @ingroup TSO_General
/// @param u8Eng       \b IN: TSO engine ID
/// @return TRUE - if idle
/// @return FALSE - otherwise
//-------------------------------------------------------------------------------------------------
MS_BOOL SYMBOL_WEAK MApi_DMX_TSO_Filein_IsIdle(MS_U8 u8Eng);

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in CMDQ empty number.
/// @ingroup TSO_General
/// @param u8Eng             \b IN: TSO engine ID
/// @param pu32EmptySlot     \b OUT: pointer to store CMDQ empty slot number
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note The user can use MApi_DMX_TSO_Filein_Start() as a multimedia file-in command to input data from memory to the TSO. \n
/// The TSO engine allows to add multimedia file-in command without having to wait that the previous multimedia file-in command have been processed completely. \n
/// In this mode the waited multimedia file-in commands are stored to the so-called multimedia file-in command queue. \n
/// However, the size of the command queue is limited and each multimedia file-in command will consumes 5 slots in the multimedia file-in command queue. \n
/// Therefore, the caller has to check the number of empty slots in the multimedia file-in command queue with MApi_DMX_TSO_Filein_CMDQ_GetEmptyNum()
/// before adding a new multimedia file-in command.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Filein_CMDQ_GetEmptyNum(MS_U8 u8Eng, MS_U32 *pu32EmptySlot);

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in CMDQ reset.
/// @ingroup TSO_General
/// @param u8Eng           \b IN: TSO engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Filein_CMDQ_Reset(MS_U8 u8Eng);

//-------------------------------------------------------------------------------------------------
/// Start to get bit stream by memeory of TSO path.
/// @ingroup TSO_General
/// @param u8Eng             \b IN: TSO engine ID
/// @param pBuf              \b IN: the memory containing the bit stream
/// @param u32BufSize        \b IN: the size the memory to get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Filein_Start(MS_U8 u8Eng, MS_PHY pBuf, MS_U32 u32BufSize);

//-------------------------------------------------------------------------------------------------
/// Stop to get bit stream by memeory of TSO path.
/// @ingroup TSO_General
/// @param u8Eng            \b IN: TSO engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Filein_Stop(MS_U8 u8Eng);

//-------------------------------------------------------------------------------------------------
/// Set TSO playback time stamp. (LPCR)
/// @ingroup TSO_General
/// @param u8Eng             \b IN: TSO engine ID
/// @param u32Stamp          \b OUT: pointer to store timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_SetPlaybackTimeStamp(MS_U8 u8Eng, MS_U32 u32Stamp);

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in  time stamp. (LPCR)
/// @ingroup TSO_General
/// @param u8Eng              \b IN: TSO engine ID
/// @param pu32Stamp          \b OUT: pointer to store timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_GetPlaybackStamp(MS_U8 u8Eng, MS_U32* pu32Stamp);

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in  time stamp.
/// @ingroup TSO_General
/// @param u8Eng              \b IN: TSO engine ID
/// @param pu32Stamp          \b OUT: pointer to store timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_GetFileInTimeStamp(MS_U8 u8Eng, MS_U32 *pu32Stamp);

//-------------------------------------------------------------------------------------------------
/// Disable 192 mode block scheme to bypass fill-in timestamp.
/// @ingroup TSO_General
/// @param u8Eng           \b IN: TSO engine id.
/// @param bbypass         \b IN: If true, bypass file-in timestamp.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_BypassFileInTimeStamp(MS_U8 u8Eng, MS_BOOL bbypass);
//-------------------------------------------------------------------------------------------------
/// Enable TSO file in  time stamp.
/// @ingroup DMX_ToBeModified_MM_TSO
/// @param u8Eng           \b IN: TSO engine id.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_TimeStampEnable(MS_U8 u8Eng);

//-------------------------------------------------------------------------------------------------
/// Disable TSO file in time stamp
/// @ingroup DMX_ToBeModified_MM_TSO
/// @param u8Eng           \b IN: TSO engine id.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_TimeStampDisable(MS_U8 u8Eng);
//-------------------------------------------------------------------------------------------------
/// Set TSO Out Clock.
/// @ingroup TSO_General
/// @param  u8Eng               \b IN: TSO engine ID
/// @param  eTsOutClk           \b IN: TSO out clock select
/// @param  eTsOutClkSrc        \b IN: TSO out clock source select
/// @param  u16DivNum           \b IN: If select DMPLLDIV source, setting Divide number
/// @param  bClkInv             \b IN: If Out Clock invert
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note If eTsOutClk is E_DMX_TSO_OUTCLK_DIV2N, eTsOutClkSrc could be E_DMX_TSO_OUTCLKSRC_172M_2N/E_DMX_TSO_OUTCLKSRC_288M_2N/E_DMX_TSO_OUTCLKSRC_432M_2N,
/// @note and user should set u16DivNum to generate final output clock.
/// @note If eTsOutClk is E_DMX_TSO_OUTCLK_DIVN, eTsOutClkSrc could be E_DMX_TSO_OUTCLKSRC_216M_N, and user should set u16DivNum to generate final output clock.
/// @note If eTsOutClk is E_DMX_TSO_OUTCLK_PTSOOUT/E_DMX_TSO_OUTCLK_PTSOOUT_DIV8, eTsOutClkSrc could be E_DMX_TSO_OUTCLKSRC_P_TS0IN/E_DMX_TSO_OUTCLKSRC_P_TS1IN, etc,
/// @note and this colck is from external pad
/// @note E_DMX_TSO_OUTCLK_62M/E_DMX_TSO_OUTCLK_54M/E_DMX_TSO_OUTCLK_27M/E_DMX_TSO_OUTCLK_Dmd are const
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_SetOutClk(MS_U8 u8Eng, DMX_TSO_OutClk eTsOutClk, DMX_TSO_OutClkSrc eTsOutClkSrc, MS_U16 u16DivNum, MS_BOOL bClkInv);

//-------------------------------------------------------------------------------------------------
/// Enable or disable TSO output.
/// @ingroup TSO_General
/// @param  u8Eng                 \b IN: TSO engine ID
/// @param  bEnable               \b IN: TRUE is enable, FALSE is disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_OutputEnable(MS_U8 u8Eng, MS_BOOL bEnable);

//TSO2 API
//-------------------------------------------------------------------------------------------------
/// Set or get local stream ID of TSO Out.
/// @ingroup TSO_General
/// @param  u8Eng              \b IN: TSO engine ID
/// @param  eIf                \b IN: TSO input TSIF
/// @param  pu8StrId           \b IN or OUT: pointer to store local stream ID, default value is 0x47
/// @param  bSet               \b IN: If TRUE, set local stream id, otherwise get local stream id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_LocalStreamId(MS_U8 u8Eng, DMX_TSIF eIf, MS_U8* pu8StrId, MS_BOOL bSet);

//-------------------------------------------------------------------------------------------------
/// Set TSO virtual queue buffer.
/// @ingroup TSO_General
/// @param  u8Eng           \b IN: TSO engine ID
/// @param  phyAddr         \b IN: TSO VQ buffer address
/// @param  u32BufSize    \b IN: TSO VQ buffer size
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_SVQBuf_Set(MS_U8 u8Eng, MS_PHY phyAddr, MS_U32 u32BufSize);

//-------------------------------------------------------------------------------------------------
///TSO (TS output) Demux Flow input sources configure getting and setting.
/// @ingroup TSO_General
/// @param pstInputCfg                \b IN/OUT: Pointer of storing TSO input configure data
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// - The members in pstInputCfg structure
///     eFlow                         \b IN: DMX TSO playback flow,  DMX_FLOW_TSO_PLAYBACK/DMX_FLOW_TSO_PLAYBACK1
///     eTSOInIf                      \b IN: TSO input interface,  DMX_TSIF_LIVE0/DMX_TSIF_LIVE1/DMX_TSIF_LIVE2/DMX_TSIF_FILE0/DMX_TSIF_FILE1
///     stInputInfo.Input             \b IN/OUT: TSO input interface source,  enum item of DMX_FLOW_INPUT
///     stInputInfo.bClkInv           \b IN/OUT: If TSO input interface source is from demod, please set clock invert type
///     stInputInfo.bExtSync          \b IN/OUT: If TSO input interface source is from demod, please set external sync type
///     stInputInfo.bParallel         \b IN/OUT: If TSO input interface source is from demod, please set parallel or serial type
///     u8LocalStrId                  \b IN/OUT: The local stream ID (TS output sync byte) value, default is 0x47
///     bBypassAll                    \b IN/OUT: TRUE means bypass all ts data for TSO playback; FALSE means tso will output ts data by PIDs
///     bEnable                       \b IN/OUT: TRUE means enable TSO input now; FALSE means disable this TSO input fource

/// @note This API is for TSO 2.0 HW architecture.
/// @note Serval TSO input will be merge to be one TSO output. This API is for configuring one of TSO input.
/// @note If there are 3 TSO input source, you shold call this API 3 times for every TSO input path configure.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Flow_InputCfg(DMX_TSO_InputCfg* pstInputCfg);

//-------------------------------------------------------------------------------------------------
/// TSO (TS output) Demux Flow output path configure
/// @ingroup TSO_General
/// @param pstOutputCfg              \b IN/OUT: Pointer of storing TSO output configure data
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// - The members in pstOutputCfg structure
///     eFlow                        \b IN: DMX TSO playback flow,  DMX_FLOW_TSO_PLAYBACK/DMX_FLOW_TSO_PLAYBACK1
///     eOutPad                      \b IN: TSO output pad  select,  DMX_FLOW_OUTPUT_EXT_PAD1/DMX_FLOW_OUTPUT_EXT_PAD3///
///     u16OutPktSize                \b IN/OUT: TSO output packet size. default vale is 188 bytes
///     bEnable                      \b IN/OUT: TRUE means enable TSO output now; FALSE means disable TSO output
///     bDefOutClk                   \b IN: TURE means using default clock setting given by driver; FALSE means set output clock by user parameters
///     bOutClkInv                   \b IN/OUT: Set inver type of TSO output clock
///     eTsoOutClk                   \b IN/OUT: Select TSO output clock, enum item of DMX_TSO_OutClk
///     eTsoOutClkSrc                \b IN/OUT: Select TSO output clock source, enum item of DMX_TSO_OutClkSrc
///     u16DivNum                    \b IN/OUT: If TSO output clock source is E_DMX_TSO_OUTCLK_DIV2N/E_DMX_TSO_OUTCLK_DIVN, set this value for clock generatng.
///     bSet                         \b IN: TURE: set, FALSE: get
/// @note This API is for TSO 2.0 HW architecture.
/// @note Serval TSO input will be merge to be one TSO output. This API is for configuring TSO output path.
/// @note TSO output clock source selection, please also refer to MApi_DMX_TSO_SetOutClk API.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Flow_OutputCfg(DMX_TSO_OutputCfg* pstOutputCfg);

//-------------------------------------------------------------------------------------------------
///TSO configuration
/// @param pstTsoCfg                  \b IN/OUT: Pointer of storing TSO configure data
///
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
///
/// @note This API is for TSO2 HW architecture
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Configure(DMX_TSO_Cfg* pstTsoCfg);

//-------------------------------------------------------------------------------------------------
/// TSO (TS output) input pid filter open.
/// @ingroup TSO_General
/// @param u8Eng              \b IN: TSO engine ID
/// @param eTSOInSrc          \b IN: TSO PID filter source
/// @param u16Pid             \b IN: PID value of TSO PID filter
/// @param pu16DmxId          \b IN: Pointer to store PID filter ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Pid_Open(MS_U8 u8Eng, DMX_TSIF eTSOInSrc, MS_U16 u16Pid, MS_U16* pu16DmxId);

//-------------------------------------------------------------------------------------------------
/// TSO (TS output) input pid filter close.
/// @ingroup TSO_General
/// @param u8Eng             \b IN: TSO engine ID
/// @param u16DmxId          \b IN: TSO PID filter source
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Pid_Close(MS_U8 u8Eng, MS_U16 u16DmxId);
//-------------------------------------------------------------------------------------------------
/// Get TSO file-in  read address.
/// @ingroup TSO_General
/// @param u8Eng             \b IN: TSO engine ID
/// @param pphyRead          \b OUT: pointer to store read address
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Filein_GetReadAddr(MS_U8 u8Eng, MS_PHY* pphyRead);
//----------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/// Get TSP firmware version.
/// @ingroup TSP_General
/// @param u32FWVer      \b OUT: TSP firmware version infomation
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_TSP_Get_FW_VER(MS_U32* u32FWVer);
//-------------------------------------------------------------------------------------------------
/// Set TSP Debug Level.
/// @ingroup TSP_General
/// @param  level    \b IN: debug level.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SetDbgLevel(DMX_DBGMSG_LEVEL level);

//-----------------
// TSP_Debug
//-----------------
//-------------------------------------------------------------------------------------------------
/// Get interrupt count.
/// @ingroup TSP_Debug
/// @param pu32Count       \b OUT: interrupt count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_Get_Intr_Count(MS_U32* pu32Count);
//--------------------------------------------------------------------------------------------------
/// Get TSP driver library information
/// @ingroup TSP_General
/// @param ppVersion      \b OUT: TSP library version infomation
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_GetLibVer(const MSIF_Version **ppVersion);

//-------------------------------------------------------------------------------------------------
/// Get the register value of HW debug port .
/// @ingroup TSP_Debug
/// @param u32DbgSel       \b IN: Set which HW debug info you want to get. (ex: 0x4F, check TSIF0 data info)
/// @param u32DbgInfo      \b OUT: pointer to store the debug port value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_GetDbgPortInfo(MS_U32 u32DbgSel,MS_U32* u32DbgInfo);
//-------------------------------------------------------------------------------------------------
/// Set memory buffer information for TSP AEON to print debug message.
/// @ingroup TSP_Debug
/// @param phyAddr         \b IN: physical address of buffer
/// @param u32Size         \b IN: size of buffer
/// @param u32DbgWord      \b IN: control word to filter debug message
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SetFwDbgParam(MS_PHY phyAddr, MS_U32 u32Size, MS_U32 u32DbgWord);
//-------------------------------------------------------------------------------------------------
/// Get packet count, which is accumulated according to packets containing dis-continued CC field in TS header, by ts source and specific FIFO type.
/// @ingroup TSP_Debug
/// @param pDmxInfo          \b IN: set control parameter to get related packet count.
/// @param pu32Cnt           \b OUT: dis-continue packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_DisContiCnt(DMX_DisContiCnt_info* pDmxInfo, MS_U32* pu32Cnt);
//-------------------------------------------------------------------------------------------------
/// Get packet count, which is accumulated according to packets containing duplicated CC field in TS header, by ts source and specific FIFO type.
/// @ingroup TSP_Debug
/// @param pDmxInfo          \b IN: set control parameter to get related packet count.
/// @param pu32Cnt           \b OUT: drop packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_DropPktCnt(DMX_DropPktCnt_info* pDmxInfo, MS_U32* pu32Cnt);

//-------------------------------------------------------------------------------------------------
///Get packet count, which is accumulated according to assertion of SYNC signal, by ts source and specific FIFO type.
/// @ingroup TSP_Debug
/// @param pDmxInfo          \b IN: set control parameter to get related packet count.
/// @param pu32Cnt           \b OUT: Lock packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_LockPktCnt(DMX_LockPktCnt_info* pDmxInfo, MS_U32* pu32Cnt);
//-------------------------------------------------------------------------------------------------
///Get packet count, which is accumulated according to packets flowing into FIFO, by ts source and specific FIFO type.
/// @ingroup TSP_Debug
/// @param DmxInfo           \b IN: set control parameter to get related packet count.
/// @param pu32Cnt           \b OUT: AV packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_AVPktCnt(DMX_AVPktCnt_info* DmxInfo, MS_U32* pu32Cnt);
//-------------------------------------------------------------------------------------------------
/// Get section TEI packet count by ts source.
/// @ingroup TSP_Debug
/// @param FltSrc               \b IN: TS source
/// @param pu32PktCnt           \b OUT: TEI packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_SecTEI_PktCount(DMX_FILTER_TYPE FltSrc, MS_U32* pu32PktCnt);
//-------------------------------------------------------------------------------------------------
/// Reset section TEI packet count by DMX filter index.
/// @ingroup TSP_Debug
/// @param FltSrc               \b IN: TS source
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Reset_SecTEI_PktCount(DMX_FILTER_TYPE FltSrc);
//-------------------------------------------------------------------------------------------------
/// Get section dis-continue packet count.
/// @ingroup TSP_Debug
/// @param u32DmxID         \b IN: DMX filter index
/// @param pu32PktCnt       \b OUT: Dis-continue packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_SecDisCont_PktCount(MS_U32 u32DmxID, MS_U32* pu32PktCnt);
//-------------------------------------------------------------------------------------------------
/// Reset section dis-continue packet count.
/// @ingroup TSP_Debug
/// @param u32DmxID      \b IN: DMX filter Id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS MApi_DMX_Reset_SecDisCont_PktCount(MS_U32 u32DmxID);
//-----------------------------------------------------

//-----------------
// TSP_FIQ
//-----------------
//-------------------------------------------------------------------------------------------------
/// Enable/Disable section filter rush pass.
/// @ingroup TSP_FIQ
/// @param u32DmxId      \b IN: DMX filter Id
/// @param u8Enable      \b IN: TRUE: enable, FALSE: disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Now only TSP 2.0 & TSP 4.0 support it.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_FQ_SetFltRushPass(MS_U32 u32DmxId, MS_U8 u8Enable);
//-------------------------------------------------------------------------------------------------
/// Initialize Demux API for FIQ Enable.
///        (1) If the data rate of a single demod > 24MB/s,
///            we have to enable the FQ function for data correctness.
///        (2) If we have to enable FQ function,
///            the minimum size of FQ buffer is 6144(192*32) bytes
/// @ingroup TSP_FIQ
/// @param u32FQEng       \b IN: FQ engine id
/// @param pFQInfo        \b IN: Init FQ Setting
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note To use FQ, please call this API after MApi_DMX_Init. (Not all chips support FQ)
/// @note DO NOT init FQ function WHILE DTV playback, or data may lost and mosaic may ocurr.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_FQ_Init(MS_U32 u32FQEng, DMX_FQ_Info* pFQInfo);
//-------------------------------------------------------------------------------------------------
/// Finalize Demux API for FIQ.
/// @ingroup TSP_FIQ
/// @param u32FQEng       \b IN: FQ engine id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_FQ_Exit(MS_U32 u32FQEng);
//-------------------------------------------------------------------------------------------------
/// Enable FIQ rush function.
/// @ingroup TSP_FIQ
/// @param u32FQEng       \b IN: FQ engine id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_FQ_RushEnable(MS_U32 u32FQEng);

//-------------------------------------------------------------------------------------------------
/// Select path to skip rush data.
/// @ingroup TSP_FIQ
/// @param u32FQEng       \b IN: FQ engine id
/// @param eSkipPath      \b IN: Select path to skip rush data
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_FQ_SkipRushData(MS_U32 u32FQEng, DMX_FQ_SkipPath eSkipPath);
//-----------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Configure FIQ
/// @ingroup TSP_FIQ
/// @param pstFqCfg                         \b IN/OUT: Pointer of storing configure data
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
/// E_DMX_FQ_CFG_SEL_SRCMUX --> Set eFqSrcSel to select FQ source of TS path
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_FQ_Configure(DMX_FQ_Cfg* pstFqCfg);

//-------------------------------------------------------------------------------------------------
/// Set rush start address in the FIQ buffer.
/// @ingroup TSP_FIQ
/// @param u32FQEng        \b IN: FQ engine id
/// @param phyRushAddr     \b IN: Rush start address in the FIQ buffer
/// @return DMX_FILTER_STATUS_OK    - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_FQ_SetRushAddr(MS_U32 u32FQEng, MS_PHY phyRushAddr);

//-------------------------------------------------------------------------------------------------
/// Get FIQ File-in read pointer
/// @ingroup TSP_FIQ
/// @param u32FQEng        \b IN : FQ engine id
/// @param pphyRead        \b OUT: The pointer to store the obtained FIQ File-in read pointer
/// @return DMX_FILTER_STATUS_OK    - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_FQ_ReadGet(MS_U32 u32FQEng, MS_PHY* pphyRead);

//-------------------------------------------------------------------------------------------------
/// Get FIQ PVR write pointer
/// @ingroup TSP_FIQ
/// @param u32FQEng        \b IN : FQ engine id
/// @param pphyWrite       \b OUT: The pointer to store the obtained FIQ PVR write pointer
/// @return DMX_FILTER_STATUS_OK    - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_FQ_WriteGet(MS_U32 u32FQEng, MS_PHY* pphyWrite);
//-----------------
// CILINK FUNCTION
//-----------------
//-------------------------------------------------------------------------------------------------
/// Configure CILINK parameter
/// @param eEng                      \b IN: Engine id
/// @param pstCilinkCfg              \b IN/OUT: Pointer of storing configure data
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_CILINK_Config(DMX_CILINK_ENG eEng, DMX_CILINK_Cfg* pstCilinkCfg);
//-----------------
// TSP_Merge
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set or Get merge stream source id and sync byte
/// @ingroup TSP_Merge
/// @param eIf                    \b IN:  Eunm value of DMX TSIF selection
/// @param u8StrId                \b IN:  Stream index
/// @param pu8SyncByte            \b IN:  Pointer to sync bytearray of merege streams
/// @param bSet                   \b IN:  TRUE to setting data or FALSE to getting table
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note: Currently, maxmum number is 8, and don't call this API when stream processing is started
/// @deprecated Obsolete function. Use @ref MApi_DMX_MergeStream_SyncByte_Configure instead.
//-------------------------------------------------------------------------------------------------
 DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_MStr_SyncByte(DMX_TSIF eIf, MS_U8 u8StrId, MS_U8* pu8SyncByte, MS_BOOL bSet);
//-----------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Configure merge stream sync byte
/// @ingroup TSP_Merge
/// @param eIf                    \b IN:  Eunm value of DMX TSIF selection
/// @param DMX_SyncByte_Cfg       \b IN/OUT: Pointer of storing configure data
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS  MApi_DMX_MergeStream_SyncByte_Configure(DMX_TSIF eIf, DMX_SyncByte_Cfg *pstSyncByteCfg);

//-----------------
// Policy Manager
//-----------------
//-------------------------------------------------------------------------------------------------
/// Get pipe id
/// @param eGroup                           \b IN: Pipe Group
/// @param EngId                            \b IN: Engine id
/// @param pu32PipeId                       \b OUT: Pointer to store pipe id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_Get_PipeId(DMX_PIPEID_GROUP eGroup, DMX_GENERAL_ENGID EngId, MS_U32 *pu32PipeId);

//---  Common Interface for TSP config and Api commands ------------//
//-------------------------------------------------------------------------------------------------
/// General function for set cmd or get infomation.
/// @ingroup TSP_Debug
/// @param u32Cmd          \b IN: DMX filter Id
/// @param u32Config       \b IN: Config 0 (define by case)
/// @param u32DataNum   \b IN: Config 0 (define by case)
/// @param pData             \b OUT: Output data
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config, MS_U32 u32DataNum, void *pData);

//u32Cmd of MApi_DMX_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config, MS_U32 u32DataNum, void *pData);
#define DMX_CMD_RUN_DISABLE_SEC_CC_CHECK                0x00000001 //[u32Config]: 1-> Disable cc check on fw; 0-> Enable cc check on fw; [u32DataNum,*pData] do not use
#define DMX_CMD_RUN_SECT_OVERFLOW_STOP_FLT              0x00000002 //[u32Config]: 1-> Disable section filter out when overflow on fw; 0-> Not disable section filter out when overflow on fw; [u32DataNum,*pData] do not use
#define DMX_CMD_SET_PESCPY_CONT_MODE                    0x00000010 //[u32Config]: 0-> Disable; 1 -> Enable
    #define DMX_CMD_PESCPY_CONT_MODE_DISABLE            0
    #define DMX_CMD_PESCPY_CONT_MODE_ENABLE             1
#define DMX_CMD_ENABLE_INIT_REF_CNT                     0x00000100 //[u32Config]: 1-> Enable DMX init reference counter; 0-> Disable DMX init reference counter
#define DMX_CMD_STC_ADJUST_UNIT                         0x00000020 //[u32Config]: data
#define DMX_CMD_PVR_PES_MODE                            0x00000030 //u32Config: PVR EngineID, u32DataNum=1, *pData = 1 is enable, pData = 0 is disable
#define DMX_CMD_GET_SEC_CRCERR_CNT                      0x00000040 //[pData] : Section CRC error count

#if 0
DMX_FILTER_STATUS MApi_DMX_ResetFileinTimestamp(void)
#endif


#ifdef __cplusplus
}
#endif

#endif // #ifndef __API_DMX_H__
