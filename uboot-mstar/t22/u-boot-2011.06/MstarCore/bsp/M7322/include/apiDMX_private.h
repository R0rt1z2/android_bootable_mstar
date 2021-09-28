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
//
//  File name: apiDMX_private.h
//  Description: Demux  (DMX) API private header file
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DMX_PRIV_H_
#define _DMX_PRIV_H_

#include "apiDMX.h"
#include "MsTypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

// internal structure
#define DMX_MAX_SIGFLAG_NUM             16UL
#define DMX_MAX_PVRSIGFLAG_NUM          10UL
typedef struct DLL_PACKED
{
    MS_U32                      u32Eng;
    MS_U32                      u32EvtWaitOpt;
    MS_U32                      u32DmxEvt;
    MS_U32                      u32TspEvt;

    MS_U32                      u32PvrBufFullFlag[DMX_MAX_PVRSIGFLAG_NUM];

    MS_U32                      u32SecRdyFlag[DMX_MAX_SIGFLAG_NUM];
    MS_U32                      u32SecOvfFlag[DMX_MAX_SIGFLAG_NUM];
    MS_U32                      u32SecCrcErrFlag[DMX_MAX_SIGFLAG_NUM];

    MS_S32                      s32KerModeTspEvtId; //For kernel mode, every user callback process must has itself eventid in multiple process support
    MS_U32                      u32TblIndex;
}DMX_TSP_IOSIGNAL;

typedef struct DLL_PACKED
{
    MS_U32                      u32Event;
    MS_U64                      u64DmainInt;
    MS_U64                      u64DmaoutInt[4];
}DMX_TSIO_IOSIGNAL;

typedef struct DLL_PACKED
{
    MS_U32                  u32InfoSetSize; // sizeof(DMX_InfoSet)
    MS_BOOL                 bEx;
    union {
        DMX_Sect_info_Ex        SectInfoEx;
        DMX_Sect_info           SectInfo;
    } DmxInfo;

}DMX_InfoSet;

// General API
typedef DMX_FILTER_STATUS (*IOCTL_DMX_INIT_LIBRES)(void*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_FW)(MS_PHY, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_FW_DATAADDR)(MS_PHY, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_HK)(MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_INIT)(void);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSPINIT)(DMX_TSPParam*);
typedef MS_U32            (*IOCTL_DMX_TSPINIT_GETCONFIG)(DMX_TSPParam*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_EXIT)(void);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FORCE_EXIT)(void);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SUSPEND)(void);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_RESUME)(MS_PHY, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_CHK_ALIVE)(void);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_RESET)(void);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_RELEASE_HWSEMP)(void);
typedef void              (*IOCTL_DMX_WPROTECT_EN)(MS_BOOL, MS_PHY*, MS_PHY*);
typedef void              (*IOCTL_DMX_ORZWPROTECT_EN)(MS_BOOL, MS_PHY, MS_PHY);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_READ_DROP_COUNT)(MS_U16*, MS_U16*);
typedef MS_U32            (*IOCTL_DMX_SET_POWER_STATE)(EN_POWER_MODE, MS_PHY, MS_U32);

typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_OWNER)(MS_U32, MS_U32, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_CAP)(DMX_QUERY_TYPE, void*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_CAP_EX)(char*, MS_S32, void*, MS_S32*);

typedef DMX_FILTER_STATUS (*IOCTL_DMX_PARL_INVERT)(DMX_FLOW, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_BURSTLEN)(ST_DMX_BURST_INFO);

//Flow Control
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_FLOW)(DMX_FLOW, DMX_FLOW_INPUT, MS_BOOL, MS_BOOL, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_PVRFLOW)(DMX_PVR_ENG, DMX_TSIF, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_FLOWINPUT_STS)(DMX_FLOW, DMX_FLOW_INPUT*, MS_BOOL*, MS_BOOL*, MS_BOOL*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FLOW_ENABLE)(DMX_FLOW, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSOUTPAD_CFG)(DMX_FLOW_OUTPUT_PAD, DMX_FLOW_INPUT, MS_BOOL, MS_U32, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSS2P_OUTPHASE)(MS_U16, MS_BOOL, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FLOW_DSCMBENG)(DMX_TSIF, MS_U32*, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_DROP_SCMB_PKT)(DMX_FILTER_TYPE, MS_BOOL);

//STC API
typedef DMX_FILTER_STATUS (*IOCTL_DMX_STC64_MODE_EN)(MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_PCR_ENG)(MS_U8, MS_U32*, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_STC_ENG)(MS_U8, MS_U32*, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_STC_ENG)(MS_U8, MS_U32, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_STC_UPDATECTL)(MS_U8, eStcUpdateCtrlMode);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_STC_OFFSET)(MS_U32, MS_U32, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_STC_CLK_ADJUST)(MS_U32, MS_BOOL, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SEL_STC)(DMX_FILTER_TYPE, MS_U32);


//Filter API
typedef DMX_FILTER_STATUS (*IOCTL_DMX_OPEN)(DMX_FILTER_TYPE, MS_U32*);
//new API
typedef DMX_FILTER_STATUS (*IOCTL_DMX_OPEN_Ex)(DMX_FILTER_TYPE , MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_CLOSE)(MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_START)(MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_STOP)(MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_INFO)(MS_U32, DMX_Flt_info*, DMX_FILTER_TYPE*, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_INFO_Ex)(MS_U32, DMX_Flt_info_Ex*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PID)(MS_U32, MS_U16*, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_IS_START)(MS_U32, MS_BOOL*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_COPY_DATA)(MS_U32, MS_U8*, MS_U32, MS_U32*, MS_U32*, DMX_CheckCb);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PROC)(MS_U32, DMX_EVENT*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_CHANGE_FLTSRC)(MS_U32, DMX_FILTER_TYPE);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SWITCH_LIVE_FLTSRC)(DMX_FILTER_TYPE);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_OWNER)(MS_U32, MS_BOOL*);

typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_FLTSCMBSTS)(DMX_FILTER_TYPE, MS_U32, MS_U32, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_PESSCMBSTS)(MS_U32, MS_U8*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_TSSCMBSTS)(MS_U32, MS_U8*);

// Section API
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SEC_RESET)(MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SEC_SET_READADDR)(MS_U32, MS_PHY);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SEC_GET_READADDR)(MS_U32, MS_PHY*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SEC_GET_WRITEADDR)(MS_U32, MS_PHY*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SEC_GET_STARTADDR)(MS_U32, MS_PHY*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SEC_GET_ENDADDR)(MS_U32, MS_PHY*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SEC_SET_PATTERN)(MS_U32, MS_U8*, MS_U8*, MS_U8*, MS_U32);

//TTX API
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_TTX_WRITE)(MS_U32, MS_PHY*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_ACCESS)(MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_RELEASE_ACCESS)(void);

// AVFIFO control
typedef DMX_FILTER_STATUS (*IOCTL_DMX_AVFIFO_RESET)(DMX_FILTER_TYPE, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_AVFIFO_STATUS)(DMX_FILTER_TYPE, DMX_FIFO_STATUS*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_REMOVE_DUPAVFPKT)(MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_REMOVE_DUPAVFIFOPKT)(DMX_FILTER_TYPE, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_AUBD_MODE_EN)(MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TRACEMARK_EN)(DMX_FILTER_TYPE, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_PKTMODE)(DMX_FLOW, DMX_PacketMode);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_MERSTR_SYNC)(MS_U32, MS_U8);

//PVR Playback API (Old)
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_SET_PLAYSTAMP)(DMX_FILEIN_PATH, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_GET_PLAYSTAMP)(DMX_FILEIN_PATH, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_TIMESTAMP_ENABLE)(DMX_FILEIN_PATH);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_TIMESTAMP_DISABLE)(DMX_FILEIN_PATH);

// PVR Engine API  (New)
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_OPEN)(DMX_PVR_ENG, DMX_Pvr_info*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_CLOSE)(DMX_PVR_ENG);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_PAUSE)(DMX_PVR_ENG, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_PID_OPEN)(DMX_PVR_ENG, MS_U32, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_PID_CLOSE)(DMX_PVR_ENG, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_START)(DMX_PVR_ENG, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_STOP)(DMX_PVR_ENG);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_GET_WRITEADDR)(DMX_PVR_ENG, MS_PHY*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_SET_PKTMODE)(DMX_PVR_ENG, DMX_Pvr_PacketMode);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_SET_RECORDSTAMP)(DMX_PVR_ENG, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_GET_RECORDSTAMP)(DMX_PVR_ENG, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_MOBF_EN)(MS_U8 , MS_BOOL, MS_U32, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_SET_RECORDSTAMP_CLK)(DMX_PVR_ENG, DMX_TimeStamp_Clk);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_CBSIZE)(DMX_PVR_ENG, MS_U32*, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_SET_CAMODE)(DMX_PVR_ENG, DMX_CA_PVRMODE, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_IS_START)(DMX_PVR_ENG, MS_BOOL*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_FLUSH_DATA)(DMX_PVR_ENG);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_ENG_IS_FLUSH_Done)(DMX_PVR_ENG, MS_BOOL*);

//PVR Playback API (New)
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_ENG_SET_PLAYSTAMP_CLK)(MS_U8, DMX_TimeStamp_Clk);

//PVR CA Engine API (New)
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVRCA_ENG_PID_OPEN)(DMX_PVR_ENG, MS_U32, MS_U32*, MS_U8);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVRCA_ENG_PID_CLOSE)(DMX_PVR_ENG, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVRCA_ENG_START)(DMX_PVR_ENG, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVRCA_ENG_STOP)(DMX_PVR_ENG);

// File-in API (Old)
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_START)(DMX_FILEIN_PATH, DMX_FILEIN_DST, MS_PHY, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_STOP)(DMX_FILEIN_PATH);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_INFO)(DMX_Filein_info*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_ENG_INFO)(DMX_FILEIN_PATH, DMX_Filein_info*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_PAUSE)(DMX_FILEIN_PATH);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_RESUME)(DMX_FILEIN_PATH);
typedef MS_BOOL           (*IOCTL_DMX_FILE_IS_IDLE)(DMX_FILEIN_PATH);
typedef MS_BOOL           (*IOCTL_DMX_FILE_IS_BUSY)(DMX_FILEIN_PATH);
typedef MS_BOOL           (*IOCTL_DMX_FILE_IS_PAUSE)(DMX_FILEIN_PATH);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_CMDQ_RESET)(DMX_FILEIN_PATH);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_CMDQ_GET_EMPTY_NUM)(DMX_FILEIN_PATH, MS_U32*);
typedef void              (*IOCTL_DMX_FILE_BYPASS_FILE_TIMESTAMP)(DMX_FILEIN_PATH, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_CMDQ_GET_FIFO_WLEVEL)(DMX_FILEIN_PATH, MS_U8*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_GET_FILE_TIMESTAMP)(DMX_FILEIN_PATH, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_GET_READADDR)(DMX_FILEIN_PATH, MS_PHY*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_MOBF_EN)(DMX_FILEIN_PATH, MS_BOOL, MS_U32);

//MMFI & MINITSP API
typedef MS_BOOL           (*IOCTL_DMX_MMFI_IS_IDLE)(DMX_MMFI_PATH);
typedef MS_BOOL           (*IOCTL_DMX_MMFI_IS_BUSY)(DMX_MMFI_PATH);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_CMDQ_RESET)(DMX_MMFI_PATH);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_CMDQ_Get_EMPTY_NUM)(DMX_MMFI_PATH, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_START)(DMX_MMFI_DST, MS_PHY, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_GET_FILE_TIMESTAMP)(DMX_MMFI_PATH, MS_U32*);

//MMFI  (MMFI Only) API
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_PID_OPEN)(DMX_MMFI_FLTTYPE, MS_U16, MS_U8*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_PID_CLOSE)(MS_U8);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_CMDQ_GET_FIFOLEVEL)(DMX_MMFI_PATH, MS_U8*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_SET_PLAYTIMESTAMP)(DMX_MMFI_PATH, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_GET_PLAYTIMESTAMP)(DMX_MMFI_PATH, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_SET_TIMESTAMPCLK)(DMX_MMFI_PATH, DMX_TimeStamp_Clk);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_INFO)(DMX_MMFI_PATH, DMX_Filein_info*);
typedef void              (*IOCTL_DMX_MMFI_BYPASSS_STAMP)(DMX_MMFI_PATH, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_TIMESTAMP_EN)(DMX_MMFI_PATH);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_TIMESTAMP_DISABLE)(DMX_MMFI_PATH);

typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_REMOVE_DUPAVPKT)(MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MMFI_MOBF_EN)(DMX_MMFI_PATH, MS_BOOL, MS_U32);

//TSO API
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_FI_INFO)(MS_U8, DMX_Filein_info*);
typedef MS_BOOL           (*IOCTL_DMX_TSO_FI_ISIDLE)(MS_U8);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_FI_CMDQ_GET_EMPTYNUM)(MS_U8, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_FI_CMDQ_RESET)(MS_U8);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_FI_START)(MS_U8, MS_PHY, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_FI_STOP)(MS_U8);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_FI_SET_PLAYTIMESTAMP)(MS_U8, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_FI_GET_PLAYSTAMP)(MS_U8, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_FI_GETSTAMP)(MS_U8, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_FI_BYPASS_STAMP)(MS_U8, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_FI_STAMP_EN)(MS_U8);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_FI_STAMP_DISABLE)(MS_U8);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_SET_OUTCLK)(MS_U8, DMX_TSO_OutClk, DMX_TSO_OutClkSrc, MS_U16, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_OUTPUT_EN)(MS_U8, MS_BOOL);

//TSO2 API
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_LOCSTRID)(MS_U8, DMX_TSIF, MS_U8*, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_SVQBUF_SET)(MS_U8, MS_PHY, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_FLOW_INPUTCFG)(DMX_TSO_InputCfg*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_FLOW_OUTPUTCFG)(DMX_TSO_OutputCfg*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_CONFIGURE)(DMX_TSO_Cfg*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_PID_OPEN)(MS_U8, DMX_TSIF, MS_U16, MS_U16*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_PID_CLOSE)(MS_U8, MS_U16);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSO_GET_FILEIN_READADDR)(MS_U8, MS_PHY*);

//Debug API
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_DBGLEVEL)(DMX_DBGMSG_LEVEL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_FWVER)(MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_LIBVER)(const MSIF_Version **);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_DISCONCNT)(DMX_DisContiCnt_info*, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_DROPPKTCNT)(DMX_DropPktCnt_info*, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_LOCKPKTCNT)(DMX_LockPktCnt_info*, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_AVPKTCNT)(DMX_AVPktCnt_info*, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_SECTEI_PKTCNT)(DMX_FILTER_TYPE, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_RESET_SECTEI_PKTCNT)(DMX_FILTER_TYPE);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_SECDISCON_PKTCNT)(MS_U32, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_RESET_SECDISCONT_PKTCNT)(MS_U32);

typedef DMX_FILTER_STATUS (*IOCTL_DMX_CMD_RUN)(MS_U32, MS_U32, MS_U32, void*);

//FQ
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FQ_SET_FLT_RUSH_PASS)(MS_U32, MS_U8);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FQ_INIT)(MS_U32, DMX_FQ_Info*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FQ_EXIT)(MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FQ_RUSH_EN)(MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FQ_SKIP_RUSH_DATA)(MS_U32,DMX_FQ_SkipPath);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FQ_CONFIGURE)(DMX_FQ_Cfg*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FQ_SET_RUSH_ADDR)(MS_U32, MS_PHY);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FQ_READ_GET)(MS_U32, MS_PHY*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FQ_WRITE_GET)(MS_U32, MS_PHY*);


//Merge Stream API
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MSTR_SYNCBY)(DMX_TSIF, MS_U8, MS_U8*, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_PIPEID)(DMX_PIPEID_GROUP , DMX_GENERAL_ENGID , MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_MERGESTREAM_SNYBY_CFG)(DMX_TSIF, DMX_SyncByte_Cfg*);

//utopia 2K only
typedef DMX_FILTER_STATUS (*IOCTL_DMX_WAIT_TSPSIG)(DMX_TSP_IOSIGNAL*);

typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_INTCNT)(MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_DROP_EN)(MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TEI_RM_ERRPKT)(DMX_TEI_RmPktType, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_SET_FWDBGPARAM)(MS_PHY, MS_U32, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_MOBF_EN)(MS_BOOL, MS_U32, MS_U32);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_GET_DBGPORT_INFO)(MS_U32, MS_U32*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_OPEN_MULTI_FLT)(DMX_FILTER_TYPE, MS_U8*, MS_U8);

typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_PVR_PID_OPEN)(DMX_PVR_ENG, MS_U32, MS_U8*, MS_U8);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_PVR_PID_CLOSE)(DMX_PVR_ENG, MS_U8);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_PVR_START)(DMX_PVR_ENG, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_FILE_PVR_STOP)(DMX_PVR_ENG);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_PAUSE)(DMX_PVR_ENG, MS_BOOL);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PVR_PID)(DMX_PVR_ENG, MS_U32 ,MS_U32*, MS_BOOL);

// DMX_RES_ALLOC_EN
typedef DMX_FILTER_STATUS (*IOCTL_DMX_RES_ALLOCATE)(DMX_RES_TYPE, void*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_RES_FREE)(DMX_RES_TYPE, void*);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_PCR_GET_MAP_STC)(MS_U32, MS_U32*);

//CILINK
typedef DMX_FILTER_STATUS (*IOCTL_DMX_CILINK_CONFIG)(DMX_CILINK_ENG, DMX_CILINK_Cfg*);

// TSIO API
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Service_SetDMAoutVC)(MS_U16 u16Handle, TSIO_DMAOUT_SET *eSet);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Service_DMAout_WriteGet)(MS_U16 u16Handle, MS_PHY* pphyWrite);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Service_SetDestination)(MS_U16 u16Handle, TSIO_SERVICEDEST eDest);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Service_SetDMAinVC)(MS_U16 u16Handle, TSIO_DMAIN_SET *eSet);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Service_SetDMAinVC_Start)(MS_U16 u16Handle, MS_U32 u32Pacing);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Service_SetLocdecKey)(MS_U16 u16Handle, TSIO_LOCKEY_SET *eSet);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Service_LocdecKeyEnable)(MS_U16 u16Handle, MS_BOOL bEnable);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Service_PidOpen)(MS_U16 u16Handle, TSIO_FLT_CFG *pstFltCfg);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Service_PidClose)(MS_U16 u16Handle, MS_U32 u32FltId);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Service_Alloc)(MS_U8 u8Sid, MS_U16 *u16Handle);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Service_Free)(MS_U16 u16Handle);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_RegisterIntCb)(TSIOIntTypeCB efn);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Init)(TSIO_Init_Set *eParm);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Open)(TSIO_CLK eClk);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Cmd)(TSIO_CMD eCmd, MS_U32 u32Value);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Exit)(void);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_Close)(void);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_CC)(TSIO_CC_Set *Parm, TSIO_CC_RESULT* pResult);
typedef DMX_FILTER_STATUS (*IOCTL_DMX_TSIO_GetInfo)(TSIO_INFO eInfo, MS_U32 u32Arg, MS_U32 *u32Retinfo);

typedef struct _DMX_INSTANT_PRIVATE
{
    IOCTL_DMX_INIT_LIBRES                   fpDMXInitLibRes;
    IOCTL_DMX_SET_FW                        fpDMXSetFWBuf;
    IOCTL_DMX_SET_FW_DATAADDR               fpDMXSetFWMiuDataAddr;
    IOCTL_DMX_SET_HK                        fpDMXSetHK;
    IOCTL_DMX_INIT                          fpDMXInit;
    IOCTL_DMX_TSPINIT                       fpDMXTSPInit;
    IOCTL_DMX_TSPINIT_GETCONFIG             fpDMXTSPInit_GetConfig;
    IOCTL_DMX_EXIT                          fpDMXExit;
    IOCTL_DMX_FORCE_EXIT                    fpDMXForceExit;
    IOCTL_DMX_SUSPEND                       fpDMXSuspend;
    IOCTL_DMX_RESUME                        fpDMXResume;
    IOCTL_DMX_CHK_ALIVE                     fpDMXChkFwAlive;
    IOCTL_DMX_RESET                         fpDMXReset;
    IOCTL_DMX_RELEASE_HWSEMP                fpDMXReleaseHwSemp;
    IOCTL_DMX_WPROTECT_EN                   fpDMXWProtectEnable;
    IOCTL_DMX_ORZWPROTECT_EN                fpDMXOrzWProtectEnable;
    IOCTL_DMX_READ_DROP_COUNT               fpDMXReadDropCount;
    IOCTL_DMX_SET_POWER_STATE               fpDMXSetPowerState;

    IOCTL_DMX_SET_OWNER                     fpDMXSetOwner;
    IOCTL_DMX_GET_CAP                       fpDMXGetCap;
    IOCTL_DMX_GET_CAP_EX                    fpDMXGetCapEx;
    IOCTL_DMX_PARL_INVERT                   fpDMXParlInvert;
    IOCTL_DMX_SET_BURSTLEN                  fpDMXSetBurstLen;

    IOCTL_DMX_SET_FLOW                      fpDMXFlowSet;
    IOCTL_DMX_SET_PVRFLOW                   fpDMXPvrFlowSet;
    IOCTL_DMX_GET_FLOWINPUT_STS             fpDMXGetFlowInputSts;
    IOCTL_DMX_FLOW_ENABLE                   fpDMXFlowEnable;
    IOCTL_DMX_TSOUTPAD_CFG                  fpDMXTsOutpadCfg;
    IOCTL_DMX_TSS2P_OUTPHASE                fpDMXTsS2POutPhase;
    IOCTL_DMX_FLOW_DSCMBENG                 fpDMXFlowDscmbEng;
    IOCTL_DMX_DROP_SCMB_PKT                 fpDMXDropScmbPkt;

    IOCTL_DMX_STC64_MODE_EN                 fpDMX64bitModeEn;
    IOCTL_DMX_GET_PCR_ENG                   fpDMXGetPcrEng;
    IOCTL_DMX_GET_STC_ENG                   fpDMXGetStcEng;
    IOCTL_DMX_SET_STC_ENG                   fpDMXSetStcEng;
    IOCTL_DMX_SET_STC_UPDATECTL             fpDMXStcUpdateCtrl;
    IOCTL_DMX_SET_STC_OFFSET                fpDMXSetStcOffset;
    IOCTL_DMX_SET_STC_CLK_ADJUST            fpDMXStcClkAdjust;
    IOCTL_DMX_SEL_STC                       fpDMXStcSel;

    IOCTL_DMX_OPEN                          fpDMXOpen;
    //new API
    IOCTL_DMX_OPEN_Ex                       fpDMXOpenEx;
    IOCTL_DMX_CLOSE                         fpDMXClose;
    IOCTL_DMX_START                         fpDMXStart;
    IOCTL_DMX_STOP                          fpDMXStop;
    IOCTL_DMX_INFO                          fpDMXInfo;
    IOCTL_DMX_INFO_Ex                       fpDMXInfoEx;
    IOCTL_DMX_PID                           fpDMXPid;
    IOCTL_DMX_IS_START                      fpDMXIsStart;
    IOCTL_DMX_COPY_DATA                     fpDMXCopyData;
    IOCTL_DMX_PROC                          fpDMXProc;
    IOCTL_DMX_CHANGE_FLTSRC                 fpDMXChangeFltSrc;
    IOCTL_DMX_SWITCH_LIVE_FLTSRC            fpDMXSwitchLiveFltSrc;
    IOCTL_DMX_GET_OWNER                     fpDMXGetOnwer;

    IOCTL_DMX_GET_FLTSCMBSTS                fpDMXGetFltScmbSts;
    IOCTL_DMX_GET_PESSCMBSTS                fpDMXGetPesScmbSts;
    IOCTL_DMX_GET_TSSCMBSTS                 fpDMXGetTsScmbSts;

    IOCTL_DMX_SEC_RESET                     fpDMXSecReset;
    IOCTL_DMX_SEC_SET_READADDR              fpDMXSecSetReadAddr;
    IOCTL_DMX_SEC_GET_READADDR              fpDMXSecGetReadAddr;
    IOCTL_DMX_SEC_GET_WRITEADDR             fpDMXSecGetWriteAddr;
    IOCTL_DMX_SEC_GET_STARTADDR             fpDMXSecGetStartAddr;
    IOCTL_DMX_SEC_GET_ENDADDR               fpDMXSecGetEndAddr;
    IOCTL_DMX_SEC_SET_PATTERN               fpDMXSecSetPattern;

    IOCTL_DMX_GET_TTX_WRITE                 fpDMXGetTTXWrite;
    IOCTL_DMX_GET_ACCESS                    fpDMXGetAccess;
    IOCTL_DMX_RELEASE_ACCESS                fpDMXReleaseAccess;

    IOCTL_DMX_AVFIFO_RESET                  fpDMXAVFifoReset;
    IOCTL_DMX_AVFIFO_STATUS                 fpDMXAVFifoStatus;
    IOCTL_DMX_REMOVE_DUPAVFPKT              fpDMXRemoveDupAVPkt;
    IOCTL_DMX_REMOVE_DUPAVFIFOPKT           fpDMXRemoveDupAVFifoPkt;
    IOCTL_DMX_AUBD_MODE_EN                  fpDMXAUBDModeEn;
    IOCTL_DMX_TRACEMARK_EN                  fpDMXTracemarkEn;
    IOCTL_DMX_SET_PKTMODE                   fpDMXSetPktMode;
    IOCTL_DMX_SET_MERSTR_SYNC               fpDMXSetMerStrSyc;
    IOCTL_DMX_GET_PIPEID                    fpDMXGetPipeId;

    IOCTL_DMX_FILE_SET_PLAYSTAMP            fpDMXFileSetPlayStamp;
    IOCTL_DMX_FILE_GET_PLAYSTAMP            fpDMXFileGetPlayStamp;
    IOCTL_DMX_FILE_TIMESTAMP_ENABLE         fpDMXFileTimestampEnable;
    IOCTL_DMX_FILE_TIMESTAMP_DISABLE        fpDMXFileTimestampDisable;

    IOCTL_DMX_PVR_ENG_OPEN                  fpDMXPvrEngOpen;
    IOCTL_DMX_PVR_ENG_CLOSE                 fpDMXPvrEngClose;
    IOCTL_DMX_PVR_ENG_PAUSE                 fpDMXPvrEngPause;
    IOCTL_DMX_PVR_ENG_PID_OPEN              fpDMXPvrEngPidOpen;
    IOCTL_DMX_PVR_ENG_PID_CLOSE             fpDMXPvrEngPidClose;
    IOCTL_DMX_PVR_ENG_START                 fpDMXPvrEngStart;
    IOCTL_DMX_PVR_ENG_STOP                  fpDMXPvrEngStop;
    IOCTL_DMX_PVR_ENG_GET_WRITEADDR         fpDMXPvrEngGetWriteAddr;
    IOCTL_DMX_PVR_ENG_SET_PKTMODE           fpDMXPvrEngSetPktMode;
    IOCTL_DMX_PVR_ENG_SET_RECORDSTAMP       fpDMXPvrEngSetRecordStamp;
    IOCTL_DMX_PVR_ENG_GET_RECORDSTAMP       fpDMXPvrEngGetRecordStamp;
    IOCTL_DMX_PVR_ENG_MOBF_EN               fpDMXPvrEngMobfEn;
    IOCTL_DMX_PVR_ENG_SET_RECORDSTAMP_CLK   fpDMXPvrEngSetRecordStampClk;
    IOCTL_DMX_PVR_ENG_CBSIZE                fpDMXPvrEngCBSize;
    IOCTL_DMX_PVR_ENG_SET_CAMODE            fpDMXPvrEngSetCaMode;
    IOCTL_DMX_PVR_ENG_IS_START              fpDMXPvrIsStart;
    IOCTL_DMX_PVR_ENG_FLUSH_DATA            fpDMXPvrEngFlushData;
    IOCTL_DMX_PVR_ENG_IS_FLUSH_Done         fpDMXPvrEngIsFlushDone;

    IOCTL_DMX_FILE_ENG_SET_PLAYSTAMP_CLK    fpDMXFileEngSetPlayStampClk;

    IOCTL_DMX_PVRCA_ENG_PID_OPEN            fpDMXPvrcaEngPidOpen;
    IOCTL_DMX_PVRCA_ENG_PID_CLOSE           fpDMXPvrcaEngPidClose;
    IOCTL_DMX_PVRCA_ENG_START               fpDMXPvrcaEngStart;
    IOCTL_DMX_PVRCA_ENG_STOP                fpDMXPvrcaEngStop;

    IOCTL_DMX_FILE_START                    fpDMXFileStart;
    IOCTL_DMX_FILE_STOP                     fpDMXFileStop;
    IOCTL_DMX_FILE_INFO                     fpDMXFileInfo;
    IOCTL_DMX_FILE_ENG_INFO                 fpDMXFileEngInfo;
    IOCTL_DMX_FILE_PAUSE                    fpDMXFilePause;
    IOCTL_DMX_FILE_RESUME                   fpDMXFileResume;
    IOCTL_DMX_FILE_IS_IDLE                  fpDMXFileIsIdle;
    IOCTL_DMX_FILE_IS_BUSY                  fpDMXFileIsBusy;
    IOCTL_DMX_FILE_IS_PAUSE                 fpDMXFileIsPause;
    IOCTL_DMX_FILE_CMDQ_RESET               fpDMXFileCmdQReset;
    IOCTL_DMX_FILE_CMDQ_GET_EMPTY_NUM       fpDMXFileCmdQGetEmptyNum;
    IOCTL_DMX_FILE_BYPASS_FILE_TIMESTAMP    fpDMXFileBypassFileTimeStamp;
    IOCTL_DMX_FILE_CMDQ_GET_FIFO_WLEVEL     fpDMXFileCmdQGetFifoWLevel;
    IOCTL_DMX_FILE_GET_FILE_TIMESTAMP       fpDMXFileGetFileTimestamp;
    IOCTL_DMX_FILE_GET_READADDR             fpDMXFileGetReadAddr;
    IOCTL_DMX_FILE_MOBF_EN                  fpDMXFileMobfEn;

    IOCTL_DMX_MMFI_IS_IDLE                  fpDMXMMFIIsIdle;
    IOCTL_DMX_MMFI_IS_BUSY                  fpDMXMMFIIsBusy;
    IOCTL_DMX_MMFI_CMDQ_RESET               fpDMXMMFICmdQReset;
    IOCTL_DMX_MMFI_CMDQ_Get_EMPTY_NUM       fpDMXMMFIGetEmptyNum;
    IOCTL_DMX_MMFI_START                    fpDMXMMFIStart;
    IOCTL_DMX_MMFI_GET_FILE_TIMESTAMP       fpDMXMMFIGetFileTimestamp;

    IOCTL_DMX_MMFI_PID_OPEN                 fpDMXMMFIPidOpen;
    IOCTL_DMX_MMFI_PID_CLOSE                fpDMXMMFIPidClose;
    IOCTL_DMX_MMFI_CMDQ_GET_FIFOLEVEL       fpDMXMMFIGetFifoLevel;
    IOCTL_DMX_MMFI_SET_PLAYTIMESTAMP        fpDMXMMFISetPlayTimestamp;
    IOCTL_DMX_MMFI_GET_PLAYTIMESTAMP        fpDMXMMFIGetPlayTimestamp;
    IOCTL_DMX_MMFI_SET_TIMESTAMPCLK         fpDMXMMFISetTimestampClk;
    IOCTL_DMX_MMFI_INFO                     fpDMXMMFIInfo;
    IOCTL_DMX_MMFI_BYPASSS_STAMP            fpDMXMMFIBypassStamp;
    IOCTL_DMX_MMFI_REMOVE_DUPAVPKT          fpDMXMMFIRemoveDupAVPkt;
    IOCTL_DMX_MMFI_MOBF_EN                  fpDMXMMFIMobfEn;
    IOCTL_DMX_MMFI_TIMESTAMP_EN             fpDMXMMFITimestampEn;
    IOCTL_DMX_MMFI_TIMESTAMP_DISABLE        fpDMXMMFITimestampDisable;

    IOCTL_DMX_TSO_FI_INFO                   fpDMXTsoFileInfo;
    IOCTL_DMX_TSO_FI_ISIDLE                 fpDMXTsoFileIsIdle;
    IOCTL_DMX_TSO_FI_CMDQ_GET_EMPTYNUM      fpDMXTsoFileGetCmdQEmptyNum;
    IOCTL_DMX_TSO_FI_CMDQ_RESET             fpDMXTsoFileGetCmdQReset;
    IOCTL_DMX_TSO_FI_START                  fpDMXTsoFileStart;
    IOCTL_DMX_TSO_FI_STOP                   fpDMXTsoFileStop;
    IOCTL_DMX_TSO_FI_SET_PLAYTIMESTAMP      fpDMXTsoFileSetPlayStamp;
    IOCTL_DMX_TSO_FI_GET_PLAYSTAMP          fpDMXTsoFileGetPlayStamp;
    IOCTL_DMX_TSO_FI_GETSTAMP               fpDMXTsoFileGetTimeStamp;
    IOCTL_DMX_TSO_FI_BYPASS_STAMP           fpDMXTsoFileBypassStamp;
    IOCTL_DMX_TSO_FI_STAMP_EN               fpDMXTsoFileTimeStampEnable;
    IOCTL_DMX_TSO_FI_STAMP_DISABLE          fpDMXTsoFileTimeStampDisable;
    IOCTL_DMX_TSO_SET_OUTCLK                fpDMXTsoSetOutClk;
    IOCTL_DMX_TSO_OUTPUT_EN                 fpDMXTsoOutputEn;
    IOCTL_DMX_TSO_LOCSTRID                  fpDMXTsoLocStrId;
    IOCTL_DMX_TSO_SVQBUF_SET                fpDMXTsoSvqBufSet;
    IOCTL_DMX_TSO_FLOW_INPUTCFG             fpDMXTsoFlowInputCfg;
    IOCTL_DMX_TSO_FLOW_OUTPUTCFG            fpDMXTsoFlowOutputCfg;
    IOCTL_DMX_TSO_CONFIGURE                 fpDMXTsoConfigure;
    IOCTL_DMX_TSO_PID_OPEN                  fpDMXTsoPidOpen;
    IOCTL_DMX_TSO_PID_CLOSE                 fpDMXTsoPidClose;
    IOCTL_DMX_TSO_GET_FILEIN_READADDR       fpDMXTsoGetFileinAddr;

    IOCTL_DMX_SET_DBGLEVEL                  fpDMXSetDbgLevel;
    IOCTL_DMX_GET_FWVER                     fpDMXGetFwVer;
    IOCTL_DMX_GET_LIBVER                    fpDMXGetLibVer;
    IOCTL_DMX_GET_DISCONCNT                 fpDMXGetDisConCnt;
    IOCTL_DMX_GET_DROPPKTCNT                fpDMXGetDropPktCnt;
    IOCTL_DMX_GET_LOCKPKTCNT                fpDMXGetLockPktCnt;
    IOCTL_DMX_GET_AVPKTCNT                  fpDMXGetAVPktCnt;
    IOCTL_DMX_GET_SECTEI_PKTCNT             fpDMXGetSecTeiPktCnt;
    IOCTL_DMX_RESET_SECTEI_PKTCNT           fpDMXResetSecTeiPktCnt;
    IOCTL_DMX_GET_SECDISCON_PKTCNT          fpDMXGetSecDisConPktCnt;
    IOCTL_DMX_RESET_SECDISCONT_PKTCNT       fpDMXResetSecDisContPktCnt;

    IOCTL_DMX_CMD_RUN                       fpDMXCmdRun;

    IOCTL_DMX_FQ_SET_FLT_RUSH_PASS          fpDMXFQSetFltRushPass;
    IOCTL_DMX_FQ_INIT                       fpDMXFQInit;
    IOCTL_DMX_FQ_EXIT                       fpDMXFQExit;
    IOCTL_DMX_FQ_RUSH_EN                    fpDMXFQRushEn;
    IOCTL_DMX_FQ_SKIP_RUSH_DATA             fpDMXFQSkipRushData;
    IOCTL_DMX_FQ_CONFIGURE                  fpDMXFQConfigure;
    IOCTL_DMX_FQ_SET_RUSH_ADDR              fpDMXFQSetRushAddr;
    IOCTL_DMX_FQ_READ_GET                   fpDMXFQReadGet;
    IOCTL_DMX_FQ_WRITE_GET                  fpDMXFQWriteGet;

    IOCTL_DMX_MSTR_SYNCBY                   fpDMXMStrSyncBy;
    IOCTL_DMX_WAIT_TSPSIG                   fpDMXWaitTspSig;
    IOCTL_DMX_MERGESTREAM_SNYBY_CFG         fpDMXSyncByteConfigure;

    IOCTL_DMX_GET_INTCNT                    fpDMXGetIntCnt;
    IOCTL_DMX_DROP_EN                       fpDMXDropEn;
    IOCTL_DMX_TEI_RM_ERRPKT                 fpDMXTeiRmErrPkt;
    IOCTL_DMX_SET_FWDBGPARAM                fpDMXSetFwDbgParam;
    IOCTL_DMX_PVR_MOBF_EN                   fpDMXPvrMobfEn;
    IOCTL_DMX_GET_DBGPORT_INFO              fpDMXGetDbgportInfo;
    IOCTL_DMX_OPEN_MULTI_FLT                fpDMXOpenMultiFlt;

    IOCTL_DMX_FILE_PVR_PID_OPEN             fpDMXFilePvrPidOpen;
    IOCTL_DMX_FILE_PVR_PID_CLOSE            fpDMXFilePvrPidClose;
    IOCTL_DMX_FILE_PVR_START                fpDMXFilePvrStart;
    IOCTL_DMX_FILE_PVR_STOP                 fpDMXFilePvrStop;
    IOCTL_DMX_PVR_PAUSE                     fpDMXPvrPause;
    IOCTL_DMX_PVR_PID                       fpDMXPvrPid;

    IOCTL_DMX_RES_ALLOCATE                  fpDMXResAllocate;
    IOCTL_DMX_RES_FREE                      fpDMXResFree;
    IOCTL_DMX_PCR_GET_MAP_STC               fpDMXPcrGetMapStc;

    IOCTL_DMX_CILINK_CONFIG                 fpDMXCILINKConfig;

    IOCTL_DMX_TSIO_Service_SetDMAoutVC      fpDMXTSIOServiceSetDMAoutVC;
    IOCTL_DMX_TSIO_Service_DMAout_WriteGet  fpDMXTSIOServiceDMAoutWriteGet;
    IOCTL_DMX_TSIO_Service_SetDestination   fpDMXTSIOServiceSetDestination;
    IOCTL_DMX_TSIO_Service_SetDMAinVC       fpDMXTSIOServiceSetDMAinVC;
    IOCTL_DMX_TSIO_Service_SetDMAinVC_Start fpDMXTSIOServiceDMAinVCStart;
    IOCTL_DMX_TSIO_Service_SetLocdecKey     fpDMXTSIOServiceSetLocdecKey;
    IOCTL_DMX_TSIO_Service_LocdecKeyEnable  fpDMXTSIOServiceLocdecKeyEnable;
    IOCTL_DMX_TSIO_Service_PidOpen          fpDMXTSIOServicePidOpen;
    IOCTL_DMX_TSIO_Service_PidClose         fpDMXTSIOServicePidClose;
    IOCTL_DMX_TSIO_Service_Alloc            fpDMXTSIOServiceAlloc;
    IOCTL_DMX_TSIO_Service_Free             fpDMXTSIOServiceFree;
    IOCTL_DMX_TSIO_RegisterIntCb            fpDMXTSIORegisterIntCb;
    IOCTL_DMX_TSIO_Init                     fpDMXTSIOInit;
    IOCTL_DMX_TSIO_Open                     fpDMXTSIOOpen;
    IOCTL_DMX_TSIO_Cmd                      fpDMXTSIOCmd;
    IOCTL_DMX_TSIO_Exit                     fpDMXTSIOExit;
    IOCTL_DMX_TSIO_Close                    fpDMXTSIOClose;
    IOCTL_DMX_TSIO_CC                       fpDMXTSIOCC;
    IOCTL_DMX_TSIO_GetInfo                  fpDMXTSIOGetInfo;
}DMX_INSTANT_PRIVATE;


void    DMXRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32  DMXOpen(void** pInstance, void* pAttribute);
MS_U32  DMXClose(void* pInstance);
MS_U32  DMXIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
MS_U32  DMXMdbIoctl(MS_U32 u32Cmd, const void* const pArgs);
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_U32  DMXStr(MS_U32 u32PowerState, void* pModule);
#endif //MSOS_TYPE_LINUX_KERNEL


//Commend define for MApi_DMX_CMD_Run API
#define DMX_CMD_SET_LIB_MODE           0x80000003UL   //u32Config0: 0 ->  user mode, 1 ->  Kernel mode with user cb


//----------------------
// Utopia 1.0 body API
//----------------------
DMX_FILTER_STATUS _MApi_DMX_SetFW(MS_PHY pFwAddr, MS_U32 u32FwSize);
DMX_FILTER_STATUS _MApi_DMX_Init(void);
DMX_FILTER_STATUS _MApi_DMX_TSPInit(DMX_TSPParam *param);
MS_U32            _MApi_DMX_TSPInit_GetConfig(DMX_TSPParam *param);
DMX_FILTER_STATUS _MApi_DMX_Exit(void);
DMX_FILTER_STATUS _MApi_DMX_Reset(void);
DMX_FILTER_STATUS _MApi_DMX_ForceExit(void);
DMX_FILTER_STATUS _MApi_DMX_Resume(MS_PHY phyFWAddr, MS_U32 u32FWSize);
DMX_FILTER_STATUS _MApi_DMX_Suspend(void);
DMX_FILTER_STATUS _MApi_DMX_SetHK(MS_BOOL bIsHK);
DMX_FILTER_STATUS _MApi_DMX_ReleaseSemaphone(void);
DMX_FILTER_STATUS _MApi_DMX_Get_FlowInput_Status(DMX_FLOW DmxFlow, DMX_FLOW_INPUT *pDmxFlowInput, MS_BOOL *pbClkInv, MS_BOOL *pbExtSync, MS_BOOL *pbParallel);
DMX_FILTER_STATUS _MApi_DMX_FlowSet(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel);
DMX_FILTER_STATUS _MApi_DMX_PVR_FlowSet(DMX_PVR_ENG Eng, DMX_TSIF ePvrSrcTSIf, MS_BOOL bDscmbRec);
DMX_FILTER_STATUS _MApi_DMX_Open(DMX_FILTER_TYPE DmxFltType, MS_U32* pu32DmxId);
DMX_FILTER_STATUS _MApi_DMX_Open_Ex(DMX_FILTER_TYPE DmxFltType, MS_U32* pu32DmxId);
DMX_FILTER_STATUS _MApi_DMX_Open_Common(DMX_FILTER_TYPE DmxFltType, MS_U32* pu32DmxId );

DMX_FILTER_STATUS _MApi_DMX_Close(MS_U32 u32DmxId);
DMX_FILTER_STATUS _MApi_DMX_Start(MS_U32 u32DmxId);
DMX_FILTER_STATUS _MApi_DMX_Stop(MS_U32 u32DmxId);
DMX_FILTER_STATUS _MApi_DMX_IsStart(MS_U32 u32DmxId, MS_BOOL* pbEnable);
DMX_FILTER_STATUS _MApi_DMX_Get_FltScmbSts(DMX_FILTER_TYPE FltSrc, MS_U32 u32FltGroupId, MS_U32 PidFltId, MS_U32 *pu32ScmbSts);
DMX_FILTER_STATUS _MApi_DMX_Get_PesScmbSts(MS_U32 u32DmxId, MS_U8* pu8scmb);
DMX_FILTER_STATUS _MApi_DMX_Get_TsScmbSts(MS_U32 u32DmxId, MS_U8* pu8ScmSts);
DMX_FILTER_STATUS _MApi_DMX_ChkAlive(void);
DMX_FILTER_STATUS _MApi_DMX_Info(MS_U32 u32DmxId, DMX_Flt_info* pDmxFltInfo, DMX_FILTER_TYPE* pDmxFltType, MS_BOOL bSet);
DMX_FILTER_STATUS _MApi_DMX_Info_Ex(MS_U32 u32DmxId, DMX_Flt_info_Ex* pDmxFltInfo);
DMX_FILTER_STATUS _MApi_DMX_Pid(MS_U32 u32DmxId, MS_U16* pu16Pid, MS_BOOL bSet);
DMX_FILTER_STATUS _MApi_DMX_SectPatternSet(MS_U32 u32DmxId, MS_U8* pPattern, MS_U8* pMask, MS_U8 *pu8NotMask, MS_U32 MatchSize);
DMX_FILTER_STATUS _MApi_DMX_SectReset(MS_U32 u32DmxId);
DMX_FILTER_STATUS _MApi_DMX_SectReadSet(MS_U32 u32DmxId, MS_PHY Read);
DMX_FILTER_STATUS _MApi_DMX_SectReadGet(MS_U32 u32DmxId, MS_PHY* pRead);
DMX_FILTER_STATUS _MApi_DMX_SectWriteGet(MS_U32 u32DmxId, MS_PHY* pWrite);
DMX_FILTER_STATUS _MApi_DMX_SectStartGet(MS_U32 u32DmxId, MS_PHY* pStart);
DMX_FILTER_STATUS _MApi_DMX_SectEndGet(MS_U32 u32DmxId, MS_PHY* pEnd);
DMX_FILTER_STATUS _MApi_DMX_SetFwDataAddr(MS_PHY phyDataAddr, MS_U32 u32size);
DMX_FILTER_STATUS _MApi_DMX_TTX_WriteGet(MS_U32 u32DmxId, MS_PHY* pWrite);
DMX_FILTER_STATUS _MApi_DMX_Pvr_SetPlaybackStamp(MS_U32 u32Stamp);
DMX_FILTER_STATUS _MApi_DMX_Pvr_GetPlaybackStamp(MS_U32* pu32Stamp);
DMX_FILTER_STATUS _MApi_DMX_Pvr_TimeStampEnable(void);
DMX_FILTER_STATUS _MApi_DMX_Pvr_TimeStampDisable(void);
DMX_FILTER_STATUS _MApi_DMX_PvrCA_Eng_Pid_Open(DMX_PVR_ENG Eng, MS_U32 Pid, MS_U32* pu32DmxId, MS_U8 u8ShareKeyType);
DMX_FILTER_STATUS _MApi_DMX_PvrCA_Eng_Pid_Close(DMX_PVR_ENG Eng, MS_U32 u32DmxId);
DMX_FILTER_STATUS _MApi_DMX_PvrCA_Eng_Start(DMX_PVR_ENG Eng, MS_BOOL bPvrAll);
DMX_FILTER_STATUS _MApi_DMX_PvrCA_Eng_Stop(DMX_PVR_ENG Eng);
DMX_FILTER_STATUS _MApi_DMX_Filein_Info(DMX_Filein_info *pFileinInfo);
DMX_FILTER_STATUS _MApi_DMX_Filein_Start(DMX_FILEIN_DST Dst, MS_PHY pBuf, MS_U32 u32BufSize);
DMX_FILTER_STATUS _MApi_DMX_Filein_Stop(void);
DMX_FILTER_STATUS _MApi_DMX_Filein_Pause(void);
DMX_FILTER_STATUS _MApi_DMX_Filein_Resume(void);
DMX_FILTER_STATUS _MApi_DMX_Filein_CMDQ_Reset(void);
DMX_FILTER_STATUS _MApi_DMX_Filein_CMDQ_GetEmptyNum(MS_U32 *pu32EmptySlot);
DMX_FILTER_STATUS _MApi_DMX_Filein_CMDQ_FIFOWriteLevel(MS_U8 *pu8CmdQStatus);
MS_BOOL           _MApi_DMX_Filein_IsIdle(void);
MS_BOOL           _MApi_DMX_Filein_IsBusy(void);
MS_BOOL           _MApi_DMX_Filein_IsPause(void);
DMX_FILTER_STATUS _MApi_DMX_Stc_Eng_Get(MS_U8 u8Eng, MS_U32* pu32Stc32, MS_U32* pu32Stc);
DMX_FILTER_STATUS _MApi_DMX_Stc_Eng_Set(MS_U8 u8Eng, MS_U32 u32Stc32, MS_U32 u32Stc);
DMX_FILTER_STATUS _MApi_DMX_STC_UpdateCtrl(MS_U8 u8Eng, eStcUpdateCtrlMode eMode);
DMX_FILTER_STATUS _MApi_DMX_Stc_Eng_SetOffset(MS_U32 u32Eng, MS_U32 u32Offset, MS_BOOL bAdd);
DMX_FILTER_STATUS _MApi_DMX_Stc_Clk_Adjust(MS_U32 u32EngId, MS_BOOL bUpClk, MS_U32 u32Percentage);
DMX_FILTER_STATUS _MApi_DMX_Stc_Select(DMX_FILTER_TYPE eFltSrc, MS_U32 u32StcEng);
DMX_FILTER_STATUS _MApi_DMX_Pcr_Get(MS_U32* pu32Pcr32, MS_U32* pu32Pcr);
DMX_FILTER_STATUS _MApi_DMX_Pcr_Eng_Get(MS_U8 u8Eng, MS_U32* pu32Pcr32, MS_U32* pu32Pcr);
DMX_FILTER_STATUS _MApi_DMX_Proc(MS_U32 u32DmxId, DMX_EVENT* pEvent);
DMX_FILTER_STATUS _MApi_DMX_Parl_Invert(DMX_FLOW DmxFlow, MS_BOOL bInvert);
DMX_FILTER_STATUS _MApi_DMX_CopyData(MS_U32 u32DmxId, MS_U8* pu8Buf, MS_U32 u32BufSize, MS_U32* pu32ActualSize, MS_U32* pu32RmnSize, DMX_CheckCb pfCheckCB);
void              _MApi_DMX_WriteProtect_Enable(MS_BOOL bEnable, MS_PHY* pphyStartAddr, MS_PHY* pphyEndAddr);
void              _MApi_DMX_OrzWriteProtect_Enable(MS_BOOL bEnable, MS_PHY phyStartAddr, MS_PHY phyEndAddr);
DMX_FILTER_STATUS _MApi_DMX_FlowEnable(DMX_FLOW DmxFlow, MS_BOOL bEnable);
DMX_FILTER_STATUS _MApi_DMX_Read_DropPktCnt(MS_U16* pu16ADropCnt, MS_U16* pu16VDropCnt);
DMX_FILTER_STATUS _MApi_DMX_SetOwner(MS_U32 u32DmxIdStart, MS_U32 u32DmxIdEnd, MS_BOOL bOwner);
DMX_FILTER_STATUS _MApi_DMX_GetOwner(MS_U32 u32DmxId, MS_BOOL* pbOwner);
DMX_FILTER_STATUS _MApi_DMX_GetAccess(MS_U32 u32Try);
DMX_FILTER_STATUS _MApi_DMX_ReleaseAccess(void);
DMX_FILTER_STATUS _MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE DmxFltType, MS_BOOL bReset);
DMX_FILTER_STATUS _MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE DmxFltType, DMX_FIFO_STATUS* u32FifoLevel);
DMX_FILTER_STATUS _MApi_DMX_GetLibVer(const MSIF_Version **ppVersion);
DMX_FILTER_STATUS _MApi_DMX_GetCap(DMX_QUERY_TYPE DmxQueryType, void* pOutput);
DMX_FILTER_STATUS _MApi_DMX_GetCap_Ex(char* pstrQueryType, MS_S32 s32Strlen, void* pOutput, MS_S32* ps32OutSize);
DMX_FILTER_STATUS _MApi_DMX_SetBurstLen_Ex(ST_DMX_BURST_INFO stDmxBurstInfo);
DMX_FILTER_STATUS _MApi_DMX_GetFileInTimeStamp(MS_U32 *pu32FileInTS);
DMX_FILTER_STATUS _MApi_DMX_Filein_GetReadAddr(MS_PHY* pphyRead);
void              _MApi_DMX_BypassFileInTimeStamp(MS_BOOL bbypass);
DMX_FILTER_STATUS _MApi_TSP_Get_FW_VER(MS_U32* u32FWVer);
DMX_FILTER_STATUS _MApi_DMX_RemoveDupAVPkt(MS_BOOL bEnable);
DMX_FILTER_STATUS _MApi_DMX_RemoveDupAVFifoPkt(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable);
DMX_FILTER_STATUS _MApi_DMX_SetPacketMode(DMX_FLOW DmxFlow, DMX_PacketMode ePktMode);
DMX_FILTER_STATUS _MApi_DMX_SetMergeStrSyncByte(MS_U32 u32SrcID, MS_U8 u8SyncByte);
DMX_FILTER_STATUS _MApi_DMX_Change_FilterSource(MS_U32 u32DmxId, DMX_FILTER_TYPE DmxFltSrcType);
DMX_FILTER_STATUS _MApi_DMX_SetDbgLevel(DMX_DBGMSG_LEVEL level);
DMX_FILTER_STATUS _MApi_DMX_STC64_Mode_Enable(MS_BOOL bEnable);
MS_BOOL           _MApi_DMX_MMFI_Filein_IsIdle(DMX_MMFI_PATH ePath);
MS_BOOL           _MApi_DMX_MMFI_Filein_IsBusy(DMX_MMFI_PATH ePath);
DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_CMDQ_Reset(DMX_MMFI_PATH ePath);
DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_CMDQ_GetEmptyNum(DMX_MMFI_PATH ePath, MS_U32 *pu32EmptySlot);
DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_Start(DMX_MMFI_DST eDst, MS_PHY pBuf, MS_U32 u32BufSize);
DMX_FILTER_STATUS _MApi_DMX_MMFI_GetFileInTimeStamp(DMX_MMFI_PATH ePath, MS_U32 *pu32FileInTS);
DMX_FILTER_STATUS _MApi_DMX_MMFI_Pid_Open(DMX_MMFI_FLTTYPE flttype, MS_U16 u16Pid, MS_U8* pu8DmxId);
DMX_FILTER_STATUS _MApi_DMX_MMFI_Pid_Close(MS_U8 u8DmxId);
DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_CMDQ_FIFOWriteLevel(DMX_MMFI_PATH ePath, MS_U8 *pu8CmdQStatus);
DMX_FILTER_STATUS _MApi_DMX_MMFI_SetPlaybackTimeStamp(DMX_MMFI_PATH ePath, MS_U32 u32pcr2);
DMX_FILTER_STATUS _MApi_DMX_MMFI_GetPlaybackTimeStamp(DMX_MMFI_PATH ePath, MS_U32 *pu32pcr2);
DMX_FILTER_STATUS _MApi_DMX_MMFI_TimeStampClk(DMX_MMFI_PATH ePath, DMX_TimeStamp_Clk eClk);
DMX_FILTER_STATUS _MApi_DMX_MMFI_RemoveDupAVPkt(MS_BOOL bEnable);
DMX_FILTER_STATUS _MApi_DMX_MMFI_MOBF_Enable(DMX_MMFI_PATH ePath, MS_BOOL bEnable, MS_U32 u32key);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_MOBF_Enable(MS_U8 u8Eng, MS_BOOL bEnable, MS_U32 u32key0, MS_U32 u32key1);
DMX_FILTER_STATUS _MApi_DMX_Filein_MOBF_Enable(MS_BOOL bEnable, MS_U32 u32key);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Open(DMX_PVR_ENG Eng ,DMX_Pvr_info* pPvrInfo);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Close(DMX_PVR_ENG Eng );
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Pid_Open(DMX_PVR_ENG Eng ,MS_U32 Pid, MS_U32* pu32DmxId);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Pid_Close(DMX_PVR_ENG Eng ,MS_U32 u32DmxId);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Start(DMX_PVR_ENG Eng, MS_BOOL bPvrAll);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Stop(DMX_PVR_ENG Eng);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_WriteGet(DMX_PVR_ENG Eng ,MS_PHY* pphyWrite);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetPacketMode(DMX_PVR_ENG Eng, DMX_Pvr_PacketMode ePvrPktMode);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetRecordStamp(DMX_PVR_ENG Eng ,MS_U32 u32Stamp);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_GetRecordStamp(DMX_PVR_ENG Eng ,MS_U32* pu32Stamp);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetPlaybackStampClk(MS_U8 u8Eng, DMX_TimeStamp_Clk eClkSrc);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetRecordStampClk(DMX_PVR_ENG Eng, DMX_TimeStamp_Clk eClkSrc);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_CallbackSize(DMX_PVR_ENG Eng, MS_U32* pu32CallbackSize, MS_BOOL bSet);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetCaMode(DMX_PVR_ENG Eng, DMX_CA_PVRMODE eCaMode, MS_BOOL bspsEnable);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_IsStart(DMX_PVR_ENG Eng, MS_BOOL *pbIsStart);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_IsFlushDone(DMX_PVR_ENG Eng, MS_BOOL *pbDone);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_FlushData(DMX_PVR_ENG Eng);
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_Info(MS_U8 u8Eng, DMX_Filein_info *pFileinInfo);
MS_BOOL           _MApi_DMX_TSO_Filein_IsIdle(MS_U8 u8Eng);
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_CMDQ_GetEmptyNum(MS_U8 u8Eng, MS_U32 *pu32EmptySlot);
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_CMDQ_Reset(MS_U8 u8Eng);
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_Start(MS_U8 u8Eng, MS_PHY pBuf, MS_U32 u32BufSize);
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_Stop(MS_U8 u8Eng);
DMX_FILTER_STATUS _MApi_DMX_TSO_SetPlaybackTimeStamp(MS_U8 u8Eng, MS_U32 u32Stamp);
DMX_FILTER_STATUS _MApi_DMX_TSO_GetPlaybackStamp(MS_U8 u8Eng, MS_U32* pu32Stamp);
DMX_FILTER_STATUS _MApi_DMX_TSO_GetFileInTimeStamp(MS_U8 u8Eng, MS_U32 *pu32Stamp);
DMX_FILTER_STATUS _MApi_DMX_TSO_BypassFileInTimeStamp(MS_U8 u8Eng, MS_BOOL bbypass);
DMX_FILTER_STATUS _MApi_DMX_TSO_TimeStampEnable(MS_U8 u8Eng);
DMX_FILTER_STATUS _MApi_DMX_TSO_TimeStampDisable(MS_U8 u8Eng);
DMX_FILTER_STATUS _MApi_DMX_TSO_SetOutClk(MS_U8 u8Eng, DMX_TSO_OutClk eTsOutClk, DMX_TSO_OutClkSrc eTsOutClkSrc, MS_U16 u16DivNum, MS_BOOL bClkInv);
DMX_FILTER_STATUS _MApi_DMX_TSO_OutputEnable(MS_U8 u8Eng, MS_BOOL bEnable);
DMX_FILTER_STATUS _MApi_DMX_TSO_LocalStreamId(MS_U8 u8Eng, DMX_TSIF eIf, MS_U8* pu8StrId, MS_BOOL bSet);
DMX_FILTER_STATUS _MApi_DMX_TSO_SVQBuf_Set(MS_U8 u8Eng, MS_PHY phyAddr, MS_U32 u32BufSize);
DMX_FILTER_STATUS _MApi_DMX_TSO_Flow_InputCfg(DMX_TSO_InputCfg* pstInputCfg);
DMX_FILTER_STATUS _MApi_DMX_TSO_Flow_OutputCfg(DMX_TSO_OutputCfg* pstOutputCfg);
DMX_FILTER_STATUS _MApi_DMX_TSO_Configure(DMX_TSO_Cfg* pstTsoCfg);
DMX_FILTER_STATUS _MApi_DMX_TSO_Pid_Open(MS_U8 u8Eng, DMX_TSIF eTSOInSrc, MS_U16 u16Pid, MS_U16* pu16DmxId);
DMX_FILTER_STATUS _MApi_DMX_TSO_Pid_Close(MS_U8 u8Eng, MS_U16 u16DmxId);
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_GetReadAddr(MS_U8 u8Eng, MS_PHY* pphyRead);

DMX_FILTER_STATUS _MApi_DMX_LiveSrcSwitch(DMX_FILTER_TYPE TgtFlow);
DMX_FILTER_STATUS _MApi_DMX_AU_BD_Mode_Enable(MS_BOOL bEnable);
DMX_FILTER_STATUS _MApi_DMX_Tracemark_Enable(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable);
DMX_FILTER_STATUS _MApi_DMX_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config, MS_U32 u32DataNum, void *pData);
MS_U32            _MApi_DMX_SetPowerState(EN_POWER_MODE ePowerState, MS_PHY phyFWAddr, MS_U32 u32FWSize);
DMX_FILTER_STATUS _MApi_DMX_Get_DisContiCnt(DMX_DisContiCnt_info* pDmxInfo, MS_U32* pu32Cnt);
DMX_FILTER_STATUS _MApi_DMX_Get_DropPktCnt(DMX_DropPktCnt_info* pDmxInfo, MS_U32* pu32Cnt);
DMX_FILTER_STATUS _MApi_DMX_Get_LockPktCnt(DMX_LockPktCnt_info* pDmxInfo, MS_U32* pu32Cnt);
DMX_FILTER_STATUS _MApi_DMX_Get_AVPktCnt(DMX_AVPktCnt_info* DmxInfo, MS_U32* pu32Cnt);
DMX_FILTER_STATUS _MApi_DMX_Get_SecTEI_PktCount(DMX_FILTER_TYPE FltSrc, MS_U32* pu32PktCnt);
DMX_FILTER_STATUS _MApi_DMX_Reset_SecTEI_PktCount(DMX_FILTER_TYPE FltSrc);
DMX_FILTER_STATUS _MApi_DMX_Get_SecDisCont_PktCount(MS_U32 u32DmxID, MS_U32* pu32PktCnt);
DMX_FILTER_STATUS _MApi_DMX_Reset_SecDisCont_PktCount(MS_U32 u32DmxID);

DMX_FILTER_STATUS _MApi_DMX_FQ_SetFltRushPass(MS_U32 u32DmxId, MS_U8 u8Enable);
DMX_FILTER_STATUS _MApi_DMX_FQ_Init(MS_U32 u32FQEng, DMX_FQ_Info* pFQInfo);
DMX_FILTER_STATUS _MApi_DMX_FQ_Exit(MS_U32 u32FQEng);
DMX_FILTER_STATUS _MApi_DMX_FQ_RushEnable(MS_U32 u32FQEng);
DMX_FILTER_STATUS _MApi_DMX_FQ_SkipRushData(MS_U32 u32FQEng, DMX_FQ_SkipPath eSkipPath);
DMX_FILTER_STATUS _MApi_DMX_FQ_Configure(DMX_FQ_Cfg* pstFqCfg);
DMX_FILTER_STATUS _MApi_DMX_FQ_SetRushAddr(MS_U32 u32FQEng, MS_PHY phyStart);
DMX_FILTER_STATUS _MApi_DMX_FQ_ReadGet(MS_U32 u32FQEng, MS_PHY* pphyRead);
DMX_FILTER_STATUS _MApi_DMX_FQ_WriteGet(MS_U32 u32FQEng, MS_PHY* pphyWrite);


DMX_FILTER_STATUS _MApi_DMX_MStr_SyncByte(DMX_TSIF eIf, MS_U8 u8ScrId, MS_U8* pu8SyncByte, MS_BOOL bSet);
DMX_FILTER_STATUS _MApi_DMX_MergeStream_SyncByte_Configure(DMX_TSIF eIf, DMX_SyncByte_Cfg *pstSyncByteCfg);
DMX_FILTER_STATUS _MApi_DMX_Get_PipeId(DMX_PIPEID_GROUP eGroup, DMX_GENERAL_ENGID u32EngId, MS_U32 *pu32PipeId);
DMX_FILTER_STATUS _MApi_DMX_Wait_TspIoSignal(DMX_TSP_IOSIGNAL *pDmxTspIoSig);

DMX_FILTER_STATUS _MApi_DMX_Get_Intr_Count(MS_U32* pu32Count);
DMX_FILTER_STATUS _MApi_DMX_Drop_Enable(MS_BOOL bSet);
DMX_FILTER_STATUS _MApi_DMX_VQ_Enable(MS_BOOL bEnable);
DMX_FILTER_STATUS _MApi_DMX_TEI_RemoveErrorPkt( DMX_TEI_RmPktType eDmxPktType, MS_BOOL bEnable );
DMX_FILTER_STATUS _MApi_DMX_SetFwDbgParam(MS_PHY phyAddr, MS_U32 u32Size, MS_U32 u32DbgWord);
DMX_FILTER_STATUS _MApi_DMX_Pvr_MOBF_Enable(MS_BOOL bEnable, MS_U32 u32key0, MS_U32 u32key1);

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Start(DMX_FILEIN_PATH ePath, DMX_FILEIN_DST Dst, MS_PHY pBuf, MS_U32 u32BufSize);
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Stop(DMX_FILEIN_PATH ePath);
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Info(DMX_FILEIN_PATH ePath, DMX_Filein_info *pFileinInfo);
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Pause(DMX_FILEIN_PATH ePath);
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Resume(DMX_FILEIN_PATH ePath);
MS_BOOL           _MApi_DMX_Filein_Eng_IsIdle(DMX_FILEIN_PATH ePath);
MS_BOOL           _MApi_DMX_Filein_Eng_IsBusy(DMX_FILEIN_PATH ePath);
MS_BOOL           _MApi_DMX_Filein_Eng_IsPause(DMX_FILEIN_PATH ePath);
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_CMDQ_Reset(DMX_FILEIN_PATH ePath);
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_CMDQ_GetEmptyNum(DMX_FILEIN_PATH ePath, MS_U32 *pu32EmptySlot);
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_CMDQ_FIFOWriteLevel(DMX_FILEIN_PATH ePath, MS_U8 *pu8CmdQStatus);
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_GetFileInTimeStamp(DMX_FILEIN_PATH ePath, MS_U32 *pu32FileInTS);
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_GetPlaybackStamp(DMX_FILEIN_PATH ePath,MS_U32* pu32Stamp);
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_SetPlaybackStamp(DMX_FILEIN_PATH ePath,MS_U32 u32Stamp);
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_GetReadAddr(DMX_FILEIN_PATH ePath, MS_PHY* pphyRead);
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_MOBF_Enable(DMX_FILEIN_PATH ePath, MS_BOOL bEnable, MS_U32 u32key);
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_PlaybackTimeStampDisable(DMX_FILEIN_PATH ePath);
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_PlaybackTimeStampEnable(DMX_FILEIN_PATH ePath);
void              _MApi_DMX_Filein_Eng_BypassFileInTimeStamp(DMX_FILEIN_PATH ePath, MS_BOOL bbypass);

DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_Info(DMX_MMFI_PATH ePath, DMX_Filein_info *pFileinInfo);
void              _MApi_DMX_MMFI_Filein_BypassTimeStamp(DMX_MMFI_PATH ePath, MS_BOOL bbypass);
DMX_FILTER_STATUS _MApi_DMX_MMFI_TimeStampEnable(DMX_MMFI_PATH ePath);
DMX_FILTER_STATUS _MApi_DMX_MMFI_TimeStampDisable(DMX_MMFI_PATH ePath);

DMX_FILTER_STATUS _MApi_DMX_GetDbgPortInfo(MS_U32 u32DbgSel,MS_U32* u32DbgInfo);
DMX_FILTER_STATUS _MApi_DMX_Open_MultiFlt( DMX_FILTER_TYPE DmxFltType , MS_U8* pu8DmxId, MS_U8 u8TargetDmxId);
DMX_FILTER_STATUS _MApi_DMX_Filein_Pvr_Eng_Pid_Open(DMX_PVR_ENG Eng, MS_U32 Pid, MS_U8* pu8DmxId, MS_U8 u8ShareKeyType);
DMX_FILTER_STATUS _MApi_DMX_Filein_Pvr_Eng_Pid_Close(DMX_PVR_ENG Eng, MS_U8 u8DmxId);
DMX_FILTER_STATUS _MApi_DMX_Filein_Pvr_Eng_Start(DMX_PVR_ENG Eng, MS_BOOL bPvrAll);
DMX_FILTER_STATUS _MApi_DMX_Filein_Pvr_Eng_Stop(DMX_PVR_ENG Eng);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Pause(DMX_PVR_ENG Eng ,MS_BOOL bPause);
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Pid(DMX_PVR_ENG Eng ,MS_U32 u32DmxId , MS_U32 *Pid, MS_BOOL bSet);
DMX_FILTER_STATUS _MApi_DMX_Stc_Select(DMX_FILTER_TYPE eFltSrc, MS_U32 u32StcEng);
DMX_FILTER_STATUS _MApi_DMX_TsOutputPadCfg(DMX_FLOW_OUTPUT_PAD eOutPad, DMX_FLOW_INPUT eInSrcPad, MS_BOOL bInParallel, MS_U32 u32ResvNum, MS_U32 *pu32Resv);
DMX_FILTER_STATUS _MApi_DMX_TsS2POutputClkPhase(MS_U16 u16Val, MS_BOOL bEnable, MS_U32 u32Reserved);
DMX_FILTER_STATUS _MApi_DMX_Flow_DscmbEng(DMX_TSIF eTsIf, MS_U32* pu32EngId, MS_BOOL bSet);
DMX_FILTER_STATUS _MApi_DMX_DropScmbPkt(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable);

DMX_FILTER_STATUS _MApi_DMX_ResAllocate(DMX_RES_TYPE eResType, void *pRes);
DMX_FILTER_STATUS _MApi_DMX_ResFree(DMX_RES_TYPE eResType, void *pRes);
DMX_FILTER_STATUS _MApi_DMX_Pcr_Get_MapSTC(MS_U32 u32PcrFltId, MS_U32 *pu32StcEng);

//CILINK
DMX_FILTER_STATUS _MApi_DMX_CILINK_Config(DMX_CILINK_ENG eEng, DMX_CILINK_Cfg* pstCilinkCfg);

//TSIO
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_SetDMAoutVC(MS_U16 u16Handle, TSIO_DMAOUT_SET *eSet);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_DMAout_WriteGet(MS_U16 u16Handle, MS_PHY* pphyWrite);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_SetDestination(MS_U16 u16Handle, TSIO_SERVICEDEST eDest);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_SetDMAinVC(MS_U16 u16Handle, TSIO_DMAIN_SET *eSet);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_DMAinVC_Start(MS_U16 u16Handle, MS_U32 u32Pacing);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_SetLocdecKey(MS_U16 u16Handle, TSIO_LOCKEY_SET *eSet);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_LocdecKeyEnable(MS_U16 u16Handle, MS_BOOL bEnable);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_PidOpen(MS_U16 u16Handle, TSIO_FLT_CFG *pstFltCfg);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_PidClose(MS_U16 u16Handle, MS_U32 u32FltId);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_Alloc(MS_U8 u8Sid, MS_U16 *u16Handle);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_Free(MS_U16 u16Handle);
DMX_FILTER_STATUS _MApi_DMX_TSIO_RegisterIntCb(TSIOIntTypeCB efn);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Init(TSIO_Init_Set *eParm);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Open(TSIO_CLK eClk);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Cmd(TSIO_CMD eCmd, MS_U32 u32Value);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Exit(void);
DMX_FILTER_STATUS _MApi_DMX_TSIO_Close(void);
DMX_FILTER_STATUS _MApi_DMX_TSIO_CC(TSIO_CC_Set *Parm, TSIO_CC_RESULT* pResult);
DMX_FILTER_STATUS _MApi_DMX_TSIO_GetInfo(TSIO_INFO eInfo , MS_U32 u32Arg, MS_U32 *u32Retinfo);
DMX_FILTER_STATUS _MApi_DMX_TSIO_WaitIoSignal(DMX_TSIO_IOSIGNAL *pDmxTsioIoSig);

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
DMX_FILTER_STATUS _MApi_DMX_DBG_GetModuleInfo(MS_U64* pu64ReqHdl);
DMX_FILTER_STATUS _MApi_DMX_DBG_EchoCmd(MS_U64* pu64ReqHdl, MS_U32 u32CmdSize, char* pcmd);
#endif

#ifdef __cplusplus
}
#endif

#endif //_DMX_PRIV_H_

