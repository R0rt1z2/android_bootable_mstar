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
//  File name: apiDMX_v2.h
//  Description: Demux (dmx) api header file
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DMX_H_
#define _DMX_H_

#include "MsDevice.h"

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum {
    // General API                                                                //*** Args of API  ***

    MApi_CMD_DMX_Init = 0,                              // (DMX_TSPParam*)                                                              #0
    MApi_CMD_DMX_Exit = 1,                              // pointer to (U32_Result)
    MApi_CMD_DMX_ForceExit = 2,                         // pointer to (U32_Result)
    MApi_CMD_DMX_Suspend = 3,                           // pointer to (U32_Result)
    MApi_CMD_DMX_Resume = 4,                            // pointer to (U32_Result)
    MApi_CMD_DMX_ChkFwAlive = 5,                        // pointer to (U32_Result)
    MApi_CMD_DMX_Reset = 6,                             // pointer to (U32_Result)
    MApi_CMD_DMX_SetFwMiuDataAddr = 7,                  // pointer to (U32_PhyAddr_L, U32_PhyAddr_H, U32_Size, U32_Result)
    MApi_CMD_DMX_WProtectEnable = 8,                    // pointer to (PDMX_WP_PARAM)
    MApi_CMD_DMX_OrzWProtectEnable = 9,                 // pointer to (U32_bEnable, U32_StartAddr_L, U32_StartAddr_H, U32_EndAddr_L,  U32_EndAddr_H, U32_Result)

    MApi_CMD_DMX_ReadDropCount = 10,                    // pointer to (DMX_POWSTATE_PARAM)                                      #10
    MApi_CMD_DMX_SetPowerState = 11,
    MApi_CMD_DMX_SetOwner = 12,                         // pointer to (U32_Start_Flt_Id, U32_End_Flt_Id, U32_IsOwner, U32_Result)
    MApi_CMD_DMX_GetCap = 13,                           // pointer to (DMX_CAP_PARAM)

    MApi_CMD_DMX_SetBurstLen = 14,                      // pointer to (DMX_BURSTTYPE)

    //Flow Control
    MApi_CMD_DMX_Flow = 15,                             // pointer to (DMX_FLOW_PARAM)
    MApi_CMD_DMX_Flow_ParlInvert = 16,                  // pointer to (U32_DMX_FLOW, U32_bInvert, U32_Result)
    MApi_CMD_DMX_Flow_ENABLE = 17,                      // pointer to (U32_DMX_FLOW, U32_bEnable, U32_Result)

    //STC API
    MApi_CMD_DMX_GetPcr = 18,                            // pointer to (U32_Eng_Id, U32_PCR_H, U32_PCR_L, U32_Result)
    MApi_CMD_DMX_Stc_Get = 19,                           // pointer to (U32_Eng_Id, U32_STC_H, U32_STC_L, U32_Result)
    MApi_CMD_DMX_Stc_Set = 20,                           // pointer to (U32_Eng_Id, U32_STC_H, U32_STC_L, U32_Result)                     #20
    MApi_CMD_DMX_Stc_UpdateCtrl = 21,                    // pointer to (U32_Eng_Id, U32_eStcUpdateCtrlMode, U32_Result)
    MApi_CMD_DMX_Stc_SetOffset = 22,                    // pointer to (U32_Eng_Id, U32_STCOffset, U32_bAdd, U32_Result)
    MApi_CMD_DMX_Stc_ClkAdjust = 23,                     // pointer to (U32_Eng_Id, U32_Mutipletor64, U32_Result)

    //Filter API
    MApi_CMD_DMX_Open = 24,                              // pointer to (DMX_FLT_TYPE_PARAM)
    MApi_CMD_DMX_Close = 25,                             // pointer to (U32_DmxId, U32_Result)
    MApi_CMD_DMX_Start = 26,                             // pointer to (U32_DmxId, U32_Result)
    MApi_CMD_DMX_Stop = 27,                              // pointer to (U32_DmxId, U32_Result)
    MApi_CMD_DMX_Info = 28,                              // pointer to (DMX_FLT_INFO_PARAM)
    MApi_CMD_DMX_Pid = 29,                               // pointer to (U32_DmxId, U32_Pid, U32_bSet, U32_Result)
    MApi_CMD_DMX_IsStart = 30,                           // pointer to (U32_DmxId, U32_bEnable, U32_Result)                                           #30
    MApi_CMD_DMX_CopyData = 31,                          // pointer to (DMX_FLT_COPY_PARAM)
    MApi_CMD_DMX_Proc = 32,                              // pointer to (DMX_FLT_EVENT_PARAM)
    MApi_CMD_DMX_ChangeFltSrc = 33,                      // pointer to (DMX_FLT_TYPE_PARAM)
    MApi_CMD_DMX_GetOwner = 34,                          // pointer to (U32_DmxId, U32_bOwner, U32_Result)

    MApi_CMD_DMX_GetPESScmbSts = 35,                     // pointer to (U32_DmxId, U32_ScmbSts, U32_Result)
    MApi_CMD_DMX_GetTsScmbSts = 36,                      // pointer to (U32_DmxId, U32_ScmbSts, U32_Result)

    // Section API
    MApi_CMD_DMX_Sec_Reset = 37,                         // pointer to (U32_DmxId, U32_Result, U32_Result)
    MApi_CMD_DMX_Sec_ReadAddr = 38,                      // pointer to (U32_DmxId, U32_PhyReadAddr_L,  U32_PhyReadAddr_H, U32_bSet, U32_Result)
    MApi_CMD_DMX_Sec_GetWriteAddr = 39,                  // pointer to (U32_DmxId, U32_PhyWriteAddr_L, U32_PhyWriteAddr_H, U32_Result)
    MApi_CMD_DMX_Sec_GetStartAddr = 40,                  // pointer to (U32_DmxId, U32_PhyWriteAddr_L, U32_PhyWriteAddr_H, U32_Result)                             #40
    MApi_CMD_DMX_Sec_GetEndAddr = 41,                    // pointer to (U32_DmxId, U32_PhyEndAddr_L,  U32_PhyWriteAddr_H, U32_Result)
    MApi_CMD_DMX_Sec_SetPattern = 42,                    // pointer to (DMX_FLT_SEC_PAT_PARAM)

    //TTX API
    MApi_CMD_DMX_GetTTXWrite = 43,                       // pointer to (U32_DmxId, U32_PhyWriteAddr, U32_Result)
    MApi_CMD_DMX_GetAccess = 44,                         // pointer to (U32_Try, U32_Result)
    MApi_CMD_DMX_ReleaseAccess = 45,

    // AVFIFO control
    MApi_CMD_DMX_AVFIFO_Reset = 46,                      // pointer to (DMX_AVFIFO_PARAM), u32Data is for setting bFlush
    MApi_CMD_DMX_AVFIFO_Status = 47,                     // pointer to (DMX_AVFIFO_PARAM), u32Data is for getting FifoLevel
    MApi_CMD_DMX_RemoveDupAVFifoPkt = 48,                // pointer to (DMX_AVFIFO_DROP_PARAM)
    MApi_CMD_DMX_AUBD_Enable = 49,                       // pointer to (U32_bEnable, U32_Result)

    //PVR Playback API
    MApi_CMD_DMX_FI_PlayStamp = 50,                      // pointer to (U32_EngId, U32_Stamp, U32_bSet, U32_Result)                            #50
    MApi_CMD_DMX_FI_TimestampMode = 51,                  // pointer to (U32_EngId, U32_bEnable, U32_Result)
    MApi_CMD_DMX_FI_SetPlaybackStampClk = 52,            // pointer to (DMX_TIMESTAMP_CLK_PARAM)

    // PVR Engine API
    MApi_CMD_DMX_Pvr_Open = 53,                          // pointer to (DMX_PVR_INFO_PARAM)
    MApi_CMD_DMX_Pvr_Close = 54,                         // pointer to (U32_EngId, U32_Result)
    MApi_CMD_DMX_Pvr_PidOpen = 55,                       // pointer to (DMX_PVR_FLTTYPE_PARAM)
    MApi_CMD_DMX_Pvr_PidClose = 56,                      // pointer to (U32_EngId, U32_DmxId, U32_bDecryptRec, U32_Result)
    MApi_CMD_DMX_Pvr_Start = 57,                         // pointer to (U32_EngId, MS_U32_bRecAll, U32_bDecryptRec, U32_Result)
    MApi_CMD_DMX_Pvr_Stop = 58,                          // pointer to (U32_EngId, U32_bDecryptRec, U32_Result)
    MApi_CMD_DMX_Pvr_GetWriteAddr = 59,                  // pointer to (U32_EngId, U32_PhyWriteAddr_L,  U32_PhyWriteAddr_H, U32_Result)
    MApi_CMD_DMX_Pvr_SetPktMode = 60,                    // pointer to (U32_EngId, U32_bSet, U32_Result)                                           #60
    MApi_CMD_DMX_Pvr_SetRecordStamp = 61,                // pointer to (U32_EngId, U32_Stamp, U32_Result)
    MApi_CMD_DMX_Pvr_GetRecordStamp = 62,                // pointer to (U32_EngId, U32_Stamp, U32_Result)
    MApi_CMD_DMX_Pvr_SetMobfKey = 63,                    // pointer to (U32_EngId, U32_Key0, U32_Key1, U32_bEnable, U32_Result)
    MApi_CMD_DMX_Pvr_SetRecordStampClk = 64,             // pointer to (DMX_TIMESTAMP_CLK_PARAM)

    // File-in Engine API
    MApi_CMD_DMX_FI_Start = 65,                          // pointer to (DMX_FILE_START_PARAM)
    MApi_CMD_DMX_FI_Stop = 66,                           // pointer to (U32_EngId, U32_Result)
    MApi_CMD_DMX_FI_Info = 67,                           // poiinter to (DMX_FILE_INFO_PARAM)
    MApi_CMD_DMX_FI_Pause = 68,                          // pointer to (U32_EngId, U32_Result)
    MApi_CMD_DMX_FI_Resume = 69,                         // pointer to (U32_EngId, U32_Result)
    MApi_CMD_DMX_FI_IsIdle = 70,                         // pointer to (U32_EngId, U32_bIdle, U32_Result)                                          #70
    MApi_CMD_DMX_FI_IsBusy = 71,                         // pointer to (U32_EngId, U32_bBusy, U32_Result)
    MApi_CMD_DMX_FI_IsPause = 72,                        // pointer to (U32_EngId, U32_bPause, U32_Result)
    MApi_CMD_DMX_FI_CmdQReset = 73,                      // pointer to (U32_EngId, U32_Result)
    MApi_CMD_DMX_FI_CmdQGetEmptyNum = 74,                // pointer to (U32_EngId, U32_EmptyNum, U32_Result)
    MApi_CMD_DMX_FI_BypassFileTimestamp = 75,            // pointer to (U32_EngId, U32_bByPass, U32_Result)
    MApi_CMD_DMX_FI_CmdQGetFifoLevel = 76,               // pointer to (U32_EngId, U32_WLevel, U32_Result)
    MApi_CMD_DMX_FI_GetFileTimeStamp = 77,               // pointer to (U32_EngId, U32_TimeStamp, U32_Result)
    MApi_CMD_DMX_FI_GetReadAddr = 78,                    // pointer to (U32_EngId, U32_PhyReadAddr_L, U32_PhyReadAddr_H, U32_Result)
    MApi_CMD_DMX_FI_SetMobfKey = 79,                     // pointer to (U32_EngId, U32_Key, U32_bEnable, U32_Result)

    //MMFI  API
    MApi_CMD_DMX_MMFI_IsIdle = 80,                       // pointer to (U32_MMFIPath, U32_bIdle, U32_Result)                                     #80
    MApi_CMD_DMX_MMFI_IsBusy = 81,                       // pointer to (U32_MMFIPath, U32_bBusy, U32_Result)
    MApi_CMD_DMX_MMFI_CmdQReset = 82,                    // pointer to (U32_DMX_MMFI_PATH, U32_Result)
    MApi_CMD_DMX_MMFI_GetEmptyNum = 83,                  // pointer to (U32_MMFIPath, U32_EmptyNum, U32_Result)
    MApi_CMD_DMX_MMFI_Start = 84,                        // pointer to (DMX_FILE_START_PARAM)
    MApi_CMD_DMX_MMFI_GetFileTimeStamp = 85,             // pointer to (U32_EngId, U32_TimeStamp, U32_Result)

    MApi_CMD_DMX_MMFI_PidOpen = 86,                      // pointer to (DMX_MMFI_FLT_PARAM)
    MApi_CMD_DMX_MMFI_PidClose = 87,                     // pointer to (U32_EngId, U32_DmxId, U32_Result)
    MApi_CMD_DMX_MMFI_GetFifoLevel = 88,                 // pointer to (U32_EngId, U32_WLevel, U32_Result)
    MApi_CMD_DMX_MMFI_PlayStamp_Set = 89,                // pointer to (U32_EngId, U32_Stamp, U32_Result)
    MApi_CMD_DMX_MMFI_PlayStamp_Get = 90,                // pointer to (U32_EngId, U32_Stamp, U32_Result)                                         #90
    MApi_CMD_DMX_MMFI_SetTimeStampClk = 91,              // pointer to (U32_MMFIPath, U32_DMX_TimeStamp_Clk, U32_Result)
    MApi_CMD_DMX_MMFI_RemoveDupAVPkt = 92,               // pointer to (DMX_AVFIFO_DROP_PARAM)
    MApi_CMD_DMX_MMFI_SetMOBFKey = 93,                   // pointer to (U32_EngId, U32_Key, U32_bEnable, U32_Result)

    //Debug API
    MApi_CMD_DMX_SetDbgLevel = 94,                       // pointer to (U32_DMX_DBGMSG_LEVEL, U32_Result)
    MApi_CMD_DMX_GetFwVer = 95,                          // pointer to (U32_FwVer)

    MApi_CMD_DMX_CmdRun = 96,                            // pointer to (DMX_CMDRUN_PARAM)

    //Utopia 1.0 compatible API
    MApi_CMD_DMX_SetFw = 97,                              // pointer to (U32_FWAddr_L,  U32_FWAddr_H, U32_FWSize, U32_Result)
    MApi_CMD_DMX_SetHK = 98,                              // pointer to (U32_bHK, U32_Result)
    MApi_CMD_DMX_Init_1_0 = 99,                           // U32_Result
    MApi_CMD_DMX_TSPInit_1_0 = 100,                       // (DMX_TSPParam*)                                                              #100
    MApi_CMD_DMX_ReleseHWSemp = 101,                      // U32_Result
    MApi_CMD_DMX_GetLibVer = 102,                         // const MSIF_Version **
    MApi_CMD_DMX_STC64ModeEnable = 103,                   // pointer to (U32_bEnablem, U32_Result)

    //Merge stream API
    MApi_CMD_DMX_SetPktMode = 104,                        //// pointer to (DMX_PKT_MODE_PARAM)
    MApi_CMD_DMX_SetMergeStrSync = 105,                   ///  pointer to (U32_SrcID, U32_SyncByte, U32_Result)

    MApi_CMD_DMX_PVR_Flow = 106,                          /// pointer to (U32_PVR_ENG, DMX_PVRFLOW_PARAM)

    //TSO API
    MApi_CMD_DMX_TSO_FileInfo = 107,                      /// poiinter to (DMX_FILE_INFO_PARAM)
    MApi_CMD_DMX_TSO_FileIsIdle = 108,                    /// pointer to (U32_EngId, U32_bIdle, U32_Result)
    MApi_CMD_DMX_TSO_FileGetCmdQEmptyNum = 109,           /// pointer to (U32_EngId, U32EmptyNum, U32_Result)
    MApi_CMD_DMX_TSO_FileStart = 110,                     /// pointer to (U32_EngId, U32Address, U32Size, U32_Result)                          #110
    MApi_CMD_DMX_TSO_FileStop = 111,                      /// pointer to (U32_EngId, U32_Result)
    MApi_CMD_DMX_TSO_FilePlayStamp = 112,                 /// pointer to (U32_EngId, U32Stamp, U32_bEnable, U32_Result)
    MApi_CMD_DMX_TSO_FileGetTimeStamp = 113,              /// pointer to (U32_EngId, U32Stamp, U32_Result)
    MApi_CMD_DMX_TSO_FileBypassStamp = 114,               /// pointer to (U32_EngId, U32_bBypass, U32_Result)
    MApi_CMD_DMX_TSO_FileTimeStampEnable = 115,           /// pointer to (U32_EngId, U32_bEnable, U32_Result)

    // DBG INFO API
    MApi_CMD_DMX_DBG_GET_DISCONCNT = 116,                 /// poiinter to (DMX_PKT_DISCONCNT_PARAM)
    MApi_CMD_DMX_DBG_GET_DROPPKTCNT = 117,                /// poiinter to (DMX_PKT_DROPCNT_PARAM)
    MApi_CMD_DMX_DBG_GET_LOCKPKTCNT = 118,                /// poiinter to (DMX_PKT_LOCKCNT_PARAM)
    MApi_CMD_DMX_DBG_GET_AVPKTCNT = 119,                  /// poiinter to (DMX_PKT_AVCNT_PARAM)

    MApi_CMD_DMX_GET_SECTEI_PKTCNT = 120,                /// poiinter to (DMX_PKT_SECTEI_PARAM)                                   #120
    MApi_CMD_DMX_RESET_SECTEI_PKTCNT = 121,              /// poiinter to (DMX_FILTER_TYPE)
    MApi_CMD_DMX_GET_SECDISCON_PKTCNT = 122,             /// poiinter to (U32_DMXID, U32_PKTCNT, U32_Result)
    MApi_CMD_DMX_RESET_SECDISCON_PKTCNT = 123,           /// poiinter to (U32_DMXID, U32_Result)

    // BOX Series only
    MApi_CMD_DMX_Pvr_CBSize = 124,                       /// poiinter to (U32_ENGID, U32_CBSIZE, U32_bSet, U32_Result)
    MApi_CMD_DMX_Pvr_SetCaMode = 125,                    /// poiinter to (U32_ENGID, U32_CAMODE, U32_bSPSENABLE, U32_Result)
    MApi_CMD_DMX_Pvr_IsStart = 126,                      /// poiinter to (U32_ENGID, U32_bIsStart, U32_Result)

    //New
    MApi_CMD_DMX_TSO_OutClk = 127,                       /// pointer to (DMX_TSO_OUTCLK_PARAM)
    MApi_CMD_DMX_STCEng_Sel = 128,                       /// pointer to (U32_eFltSrc, U32_u32StcEng, U32_Result)
    MApi_CMD_DMX_TSO_LocStrId = 129,                     /// pointer to (DMX_TSO_LOCSTRID_PARAM)
    MApi_CMD_DMX_TSO_CmdQReset = 130,                    /// pointer to (U32_Eng, U32_Result)                                                                 #130
    MApi_CMD_DMX_GetFltScmbSts = 131,                    /// pointer to (U32_FltSrc, U32_FltGroupId, U32_PidFltId, U32_ScmbSts, U32_Result)

    //FQ
    MApi_CMD_DMX_FQ_SetFltRushPass = 132,
    MApi_CMD_DMX_FQ_Init = 133,
    MApi_CMD_DMX_FQ_Exit = 134,
    MApi_CMD_DMX_FQ_RushEnable = 135,
    MApi_CMD_DMX_FQ_SkipRushData = 136,
    MApi_CMD_DMX_FQ_Configure = 137,
    MApi_CMD_DMX_FQ_SetRushAddr = 138,
    MApi_CMD_DMX_FQ_ReadGet = 139,
    MApi_CMD_DMX_FQ_WriteGet = 140,

    //TSO2
    MApi_CMD_DMX_TSO_SvqBufSet = 150,                   /// pointer to (U32_Eng, U32_Addr_L, U32_Addr_H, U32Size, U32_Result)                                  #150
    MApi_CMD_DMX_TSO_InputCfg = 151,                    /// pointer to (DMX_TSO_Input_Cfg)
    MApi_CMD_DMX_TSO_OutputCfg = 152,                   /// pointer to (DMX_TSO_OutputCfg)
    MApi_CMD_DMX_TSO_OutputEn = 153,                    /// pointer to (U32_ENG, U32_Enable, U32_Result)
    MApi_CMD_DMX_TSO_PidOpen = 154,                     /// pointer to (U32_ENG, U32_eTSOInSrc, U32_Pid, U32_FilterID, U32_Result)
    MApi_CMD_DMX_TSO_PidClose = 155,                    /// pointer to (U32_ENG, U32_FilterID, U32_Result)
    MApi_CMD_DMX_TSO_FI_GetReadAddr = 156,              /// pointer to (U32_EngId, U32_PhyReadAddr, U32_Result)
    MApi_CMD_DMX_TSO_Configure = 157,                   /// pointer to (DMX_TSO_Cfg)

    //Merge Stream
    MApi_CMD_DMX_MStr_SyncBy = 170,                     /// pointer to (DMX_MSTR_SYNCBY_PARAM)                                  #170
    MApi_CMD_DMX_MergeSeream_SyncByte_Cfg = 171,

    //2K only
    MApi_CMD_DMX_WaitTspCbEvt = 190,                    /// pointer to (DMX_TSP_IOSIGNAL)                       #190

    //Misc
    MApi_CMD_DMX_GetIntCnt = 200,                       /// pointer to (U32_CNT, U32_Result)                                    #200
    MApi_CMD_DMX_DropEn = 201,                          /// pointer to (U32_ENABLE, U32_Result)
    MApi_CMD_DMX_TeiRmErrPkt = 202,                     /// pointer to (U32_DMX_TEI_RmPktType, U32_BOOL_bEnable, U32_Result)
    MApi_CMD_DMX_SetFwDbgParam = 203,                   /// pointer to (U32_ADDR_L,  U32_ADDR_H, U32_SIZE, U32_DBGWORD, U32_Result)
    MApi_CMD_DMX_PVR_MOBFEn = 204,                      /// pointer to (U32_ENABLE, U32_KEY0, U32_KEY1, U32_Result)
    MApi_CMD_DMX_Get_DbgPortInfo = 205,                 /// pointer to (U32_ENABLE, U32_KEY0, U32_KEY1, U32_Result)
    MApi_CMD_DMX_Open_MultiFlt = 206,                   /// pointer to (U32_DmxFltType, U32_DMXID,  U32_u8TargetDmxId, U32_Result)
    MApi_CMD_DMX_Pvr_Pause = 207,                       /// pointer to (U32_EngId, U32_bPAUSE, U32_Result)
    MApi_CMD_DMX_Pvr_Pid = 208,                         /// pointer to (U32_EngId, U32_u8DmxId, U32_Pid,  U32_bSet, U32_Result)
    MApi_CMD_DMX_OutPadCfg = 209,                       /// pointer to (DMX_OutputPad_Cfg)
    MApi_CMD_DMX_GetCapEx = 210,                        //  pointer to (DMX_CAP_EX_PARAM)                                               #210
    MApi_CMD_DMX_TsOutPhase = 211,                      /// pointer to (U32_U16VAL, U32_bEnable, U32_Reserved, , U32_Result)
    MApi_CMD_DMX_FlowDscmbEng = 212,                    /// pointer to (DMX_FLOW_DSCMBENG_PARAM)
    MApi_CMD_DMX_DropScmbPkt = 213,                     /// pointer to (DMX_DROP_SCMB_PARAM)
    MApi_CMD_DMX_LiveAB_Enable = 214,                   /// pointer to (U32_ENABLE, U32_Result)
    MApi_CMD_DMX_IsLiveAB_Enable = 215,                 /// pointer to (U32_bEnable, U32_Result)

    //MMFI part2
    MApi_CMD_DMX_MMFI_Info = 220,                       /// pointer to (DMX_FILE_INFO_PARAM)                        #220
    MApi_CMD_DMX_MMFI_BypassStamp = 221,                /// pointer to (U32_DbgSel, U32_DbgInfo, U32_Result)
    MApi_CMD_DMX_MMFI_Timestamp_En = 222,               /// pointer to (U32_ePath, U32_bEnable, U32_Result)

    //Filein PVR
    MApi_CMD_DMX_File_PVR_PidOpen = 230,                /// pointer to (U32_EngId,  U32_Pid, U32_DMXId, U32_ShareKeyType, U32_Result)       #230
    MApi_CMD_DMX_File_PVR_PidClose = 231,               /// pointer to (U32_EngId,  U32_DMXId, U32_Result)
    MApi_CMD_DMX_File_PVR_Start = 232,                  /// pointer to (U32_EngId,  U32_bPvrAll, U32_Result)
    MApi_CMD_DMX_File_PVR_Stop = 233,                   /// pointer to (U32_EngId, U32_Result)

    //UFO_DMX_RES_ALLOC_EN
    MApi_CMD_DMX_RES_ALLOC = 240,
    MApi_CMD_DMX_RES_FREE = 241,
    MApi_CMD_DMX_PCR_GET_MAP_STC = 242,


    /*****************New command******************/
    // General API
    MApi_CMD_DMX_TSPInit_GetConfig = 250,                /// (DMX_TSPParam*)
    MApi_CMD_DMX_AVFIFO_BypassTsHd = 251,                // pointer to (DMX_AVFIFO_PARAM), u32Data is for bEnable

    // File-in Engine API
    MApi_CMD_DMX_FI_Eng_Info = 267,                      /// poiinter to (DMX_FILE_INFO_PARAM)
    MApi_CMD_DMX_Get_PipeId = 268,                       /// poiinter to (U32_DMX_PIPEID_GROUP, U32_DMX_GENERAL_ENGID, U32_PipeID, U32_Result))

    //new Filter API
    MApi_CMD_DMX_Open_Ex = 269,
    MApi_CMD_DMX_Info_Ex = 270,
    MApi_CMD_DMX_Pvr_Eng_FlushData = 271,
    MApi_CMD_DMX_Pvr_Eng_IsFlushDone = 272,

    //new AVFIFO related
    MApi_CMD_DMX_Tracemark_Enable = 280,                /// pointer to　(DMX_TRACEMARK_PARAM)
    /***********************************************/
    //CILINK
    MApi_CMD_DMX_CILINK_CONFIG = 290,                   /// poiinter to (DMX_CILINK_CONFIG_PARAM)

    //TSIO
    MApi_CMD_DMX_TSIO_Service_SetDMAoutVC = 300,        /// pointer to (DMX_TSIO_Service_SetDMAoutVC_PARAM)
    MApi_CMD_DMX_TSIO_Service_SetDestination = 301,     /// pointer to (DMX_TSIO_Service_SetDestination_PARAM)
    MApi_CMD_DMX_TSIO_Service_SetDMAinVC = 302,         /// pointer to (DMX_TSIO_Service_SetDMAinVC_PARAM)
    MApi_CMD_DMX_TSIO_Service_SetLocdecKey = 303,       /// pointer to (DMX_TSIO_Service_SetLocdecKey_PARAM)
    MApi_CMD_DMX_TSIO_Service_PidOpen = 304,            /// pointer to (DMX_TSIO_Service_PidOpen_PARAM)
    MApi_CMD_DMX_TSIO_Service_PidClose = 305,           /// pointer to (DMX_TSIO_Service_PidClose_PARAM)
    MApi_CMD_DMX_TSIO_Service_Alloc = 306,              /// pointer to (DMX_TSIO_Service_Alloc_PARAM)
    MApi_CMD_DMX_TSIO_Service_Free = 307,               /// pointer to (DMX_TSIO_Service_Free_PARAM)
    MApi_CMD_DMX_TSIO_RegisterIntCb = 308,              /// pointer to (DMX_TSIO_RegisterIntCb_PARAM)
    MApi_CMD_DMX_TSIO_Init = 309,                       /// pointer to (DMX_TSIO_Init_PARAM)
    MApi_CMD_DMX_TSIO_Cmd = 310,                        /// pointer to (DMX_TSIO_Cmd_PARAM)
    MApi_CMD_DMX_TSIO_Exit = 311,                       /// pointer to (U32_Result)
    MApi_CMD_DMX_TSIO_CC = 312,                         /// pointer to (DMX_TSIO_CC_PARAM)
    MApi_CMD_DMX_TSIO_GetInfo = 313,                    /// pointer to (DMX_TSIO_GetInfo_PARAM)
    MApi_CMD_DMX_TSIO_Service_DMAinVC_Start = 314,      /// pointer to (DMX_TSIO_Service_DMAinVC_Start_PARAM)
    MApi_CMD_DMX_TSIO_Service_LocdecKeyEnable = 315,    /// pointer to (DMX_TSIO_Service_LocdecKeyEnable_PARAM)
    MApi_CMD_DMX_TSIO_Service_DMAout_WriteGet = 316,    /// pointer to (DMX_TSIO_Service_DMAout_WriteGet_PARAM)
    MApi_CMD_DMX_TSIO_Open = 317,                       /// pointer to (DMX_TSIO_Open_PARAM)
    MApi_CMD_DMX_TSIO_Close = 318,                      /// pointer to (U32_Result)
    MApi_CMD_DMX_TSIO_WaitCbEvt = 319,                  /// pointer to (DMX_TSIO_IOSIGNAL)
} eDmxIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

/// DMX Output destination
typedef enum
{
    DMX_FLOW_OUT_LIVE0             = 0,
    DMX_FLOW_OUT_LIVE1             = 1,
    DMX_FLOW_OUT_LIVE2             = 2,
    DMX_FLOW_OUT_LIVE3             = 3,

    DMX_FLOW_OUT_FILE0             = 4,
    DMX_FLOW_OUT_FILE1             = 5,
    DMX_FLOW_OUT_FILE2             = 6,
    DMX_FLOW_OUT_FILE3             = 7,

    DMX_FLOW_OUT_PVR0              = 8,
    DMX_FLOW_OUT_PVR1              = 9,
    DMX_FLOW_OUT_PVR2              = 10,
    DMX_FLOW_OUT_PVR3              = 11,
    DMX_FLOW_OUT_PVR4              = 12,
    DMX_FLOW_OUT_PVR5              = 13,
    DMX_FLOW_OUT_PVRCB             = 14,

    DMX_FLOW_OUT_MMFIAU            = 15,
    DMX_FLOW_OUT_MMFI0             = DMX_FLOW_OUT_MMFIAU,
    DMX_FLOW_OUT_MMFIV3D           = 16,
    DMX_FLOW_OUT_MMFI1             = DMX_FLOW_OUT_MMFIV3D,

} DMX_FLOW_OUTPUT;

typedef struct DLL_PACKED _DMX_CAP_PARAM
{
    MS_U32                  u32Res;
    DMX_QUERY_TYPE          Quetype;
    void*                   pdata;

}DMX_CAP_PARAM, *PDMX_CAP_PARAM;

typedef struct DLL_PACKED _DMX_CAP_EX_PARAM
{
    MS_U32                  u32Res;
    MS_S32                  InputStrLen;
    MS_S32                  OutputSize;
    char*                   StrQuetype;
    void*                   pdata;
}DMX_CAP_EX_PARAM, *PDMX_CAP_EX_PARAM;

typedef struct DLL_PACKED _DMX_FLOW_PARAM
{
    MS_U32             u32Res;
    MS_U32             u32CAEngSel; // 0xFFFFFFFF:None CA
    DMX_FLOW           DmxFlow;
    DMX_FLOW_INPUT     DmxFlowInput;
    MS_BOOL            bClkInv;
    MS_BOOL            bExtSync;
    MS_BOOL            bParallel;
    MS_BOOL            bSet;

}DMX_FLOW_PARAM, *PDMX_FLOW_PARAM;

typedef struct DLL_PACKED _DMX_PVRFLOW_PARAM
{
    MS_U32             u32Res;
    DMX_PVR_ENG        eEng;
    DMX_TSIF           eSrcTSIf;
    MS_BOOL            bSet;
    MS_BOOL            bDscmbRec;

}DMX_PVRFLOWPARAM, *PDMX_PVRFLOW_PARAM;

typedef struct DLL_PACKED _DMX_FLT_TYPE_PARAM
{
    MS_U32              u32Res;
    DMX_FILTER_TYPE     DmxFltType;
    MS_U32              u32DmxId;

}DMX_FLT_TYPE_PARAM, *PDMX_FLT_TYPE_PARAM;

typedef struct DLL_PACKED _DMX_FLT_INFO_PARAM
{
    MS_U32              u32Res;
    MS_U32              u32DmxId;
    DMX_Flt_info*       pDmxFltInfo;
    DMX_FILTER_TYPE*    pDmxFltType;
    MS_BOOL             bSet;

}DMX_FLT_INFO_PARAM, *PDMX_FLT_INFO_PARAM;

typedef struct DLL_PACKED _DMX_FLT_INFO_PARAM_Ex
{
    MS_U32              u32Res;
    MS_U32              u32DmxId;
    DMX_Flt_info_Ex*    pDmxFltInfo;
}DMX_FLT_INFO_PARAM_Ex, *PDMX_FLT_INFO_PARAM_Ex;

typedef struct DLL_PACKED _DMX_FLT_COPYDATA_PARAM
{
    MS_U32              u32Res;
    MS_U32              u32DmxId;
    MS_U32              u32BufSize;
    MS_U32              u32ActualSize;
    MS_U32              u32RmnSize;
    DMX_CheckCb         pfCheckCB;
    MS_U8*              pu8Buf;

}DMX_FLT_COPY_PARAM, *PDMX_FLT_COPY_PARAM;

typedef struct DLL_PACKED _DMX_FLT_EVENT_PARAM
{
    MS_U32              u32Res;
    MS_U32              u32DmxId;
    DMX_EVENT*          pEvent;

}DMX_FLT_EVENT_PARAM, *PDMX_FLT_EVENT_PARAM;

typedef struct DLL_PACKED _DMX_FLT_SEC_PAT_PARAM
{
    MS_U32              u32Res;
    MS_U32              u32DmxId;
    MS_U32              u32MatchSize;
    MS_U8*              pu8Pattern;
    MS_U8*              pu8Mask;
    MS_U8*              pu8NotMask;
}DMX_FLT_SEC_PAT_PARAM, *PDMX_FLT_SEC_PAT_PARAM;

typedef struct DLL_PACKED _DMX_AVFIFO_PARAM
{
    DMX_FILTER_TYPE     DmxFltType;
    MS_U32              u32Data;
    MS_U32              u32Res;
}DMX_AVFIFO_PARAM, *PDMX_AVFIFO_PARAM;

typedef struct DLL_PACKED _DMX_AVFIFO_DROP_PARAM
{
    MS_U32              u32Res;
    DMX_FILTER_TYPE     DmxFltType;
    MS_BOOL             bAllFifo;
    MS_BOOL             bEnable;

}DMX_AVFIFO_DROP_PARAM, *PDMX_AVFIFO_DROP_PARAM;


typedef struct DLL_PACKED _DMX_TIMESTAM_CLK_PARAM
{
    MS_U32              u32Res;
    MS_U32              u32EngId;
    DMX_TimeStamp_Clk   eClk;

}DMX_TIMESTAMP_CLK_PARAM, *PDMX_TIMESTAMP_CLK_PARAM;

typedef struct DLL_PACKED _DMX_PVR_INFO_PARAM
{
    MS_U32              u32Res;
    MS_U32              u32EngId;
    DMX_Pvr_info*       pPvrInfo;

}DMX_PVR_INFO_PARAM, *PDMX_PVR_INFO_PARAM;

typedef struct DLL_PACKED _DMX_PVR_FLTTYPE_PARAM
{
    MS_U32              u32Res;
    MS_U32              u32EngId;
    MS_U32              u32DmxId;
    MS_U32              u32Pid;
    DMX_FILTER_TYPE     FltSrc;
    MS_U32              u32CAEngSel;            // 0xFFFFFFFF:No Dscmb
    MS_U32              u32SelShareFlt;         //0: Not use share dscmb filter

}DMX_PVR_FLTTYPE_PARAM, *PDMX_PVR_FLTTYPE_PARAM;

typedef struct DLL_PACKED _DMX_FILE_START_PARAM
{
    MS_U32              u32Res;
    MS_U32              u32EngId;
    DMX_FILEIN_DST      Dst;
    MS_PHY              pBuf;
    MS_U32              u32BufSize;

}DMX_FILE_START_PARAM, *PDMX_FILE_START_PARAM;

typedef struct DLL_PACKED _DMX_FILE_INFO_PARAM
{
    MS_U32              u32Res;
    MS_U32              u32EngId;
    DMX_Filein_info*    pFileinInfo;

}DMX_FILE_INFO_PARAM, *PDMX_FILE_INFO_PARAM;

typedef struct DLL_PACKED _DMX_MMFI_FLT_PARAM
{
    MS_U32              u32Res;
    MS_U32              u32DmxId;
    MS_U32              u32Pid;
    DMX_MMFI_FLTTYPE    flttype;
    DMX_MMFI_PATH       MMFIPath;
}DMX_MMFI_FLT_PARAM, *PDMX_MMFI_FLT_PARAM;

typedef struct DLL_PACKED _DMX_CMDRUN_PARAM
{
    MS_U32              u32Res;
    MS_U32              u32Cmd;
    MS_U32              u32Config;
    MS_U32              u32DataNum;
    void*               pData;
}DMX_CMDRUN_PARAM, *PDMX_CMDRUN_PARAM;

typedef struct DLL_PACKED _DMX_POWSTATE_PARAM
{
    MS_U32              u32Res;
    EN_POWER_MODE       u16PowerState;
    MS_PHY            u32FWAddr;
    MS_U32              u32FWSize;
}DMX_POWSTATE_PARAM, *PDMX_POWSTATE_PARAM;

typedef struct DLL_PACKED _DMX_MMFI_START_PARAM
{
    MS_U32              u32Res;
    MS_U32              u32EngId;
    DMX_MMFI_DST        Dst;
    MS_PHY            pBuf;
    MS_U32              u32BufSize;

}DMX_MMFI_START_PARAM, *PDMX_MMFI_START_PARAM;

typedef struct DLL_PACKED _DMX_PKT_MODE_PARAM
{
    MS_U32              u32Res;
    DMX_FLOW            eFlow;
    DMX_PacketMode      ePktMode;
}DMX_PKT_MODE_PARAM, *PDMX_PKT_MODE_PARAM;

typedef struct DLL_PACKED _DMX_PKT_DISCONCNT_PARAM
{
    MS_U32                  u32Res;
    DMX_DisContiCnt_info*   pInfo;
    MS_U32                  u32Cnt;
}DMX_PKT_DISCONCNT_PARAM, *PDMX_PKT_DISCONCNT_PARAM;

typedef struct DLL_PACKED _DMX_PKT_DROPCNT_PARAM
{
    MS_U32                 u32Res;
    DMX_DropPktCnt_info*   pInfo;
    MS_U32                 u32Cnt;
}DMX_PKT_DROPCNT_PARAM, *PDMX_PKT_DROPCNT_PARAM;

typedef struct DLL_PACKED _DMX_PKT_LOCKCNT_PARAM
{
    MS_U32                 u32Res;
    DMX_LockPktCnt_info*   pInfo;
    MS_U32                 u32Cnt;
}DMX_PKT_LOCKCNT_PARAM, *PDMX_PKT_LOCKCNT_PARAM;

typedef struct DLL_PACKED _DMX_PKT_AVCNT_PARAM
{
    MS_U32                 u32Res;
    DMX_AVPktCnt_info*     pInfo;
    MS_U32                 u32Cnt;
}DMX_PKT_AVCNT_PARAM, *PDMX_PKT_AVCNT_PARAM;

typedef struct DLL_PACKED _DMX_PKT_SECTEI_PARAM
{
    MS_U32                 u32Res;
    DMX_FILTER_TYPE        eType;
    MS_U32                 u32Cnt;
}DMX_PKT_SECTEI_PARAM, *PDMX_PKT_SECTEI_PARAM;

typedef struct DLL_PACKED _DMX_WP_PARAM
{
    MS_BOOL                 bEnable;
    MS_PHY*                 pu32StartAddr;
    MS_PHY*                 pu32EndAddr;
}DMX_WP_PARAM, *PDMX_WP_PARAM;

typedef struct DLL_PACKED _DMX_TSO_OUTCLK_PARAM
{
    MS_U32                 u32Res;
    MS_U32                 u32Eng;
    DMX_TSO_OutClk         eTsoOutClk;
    DMX_TSO_OutClkSrc      eTsoOutClkSrc;
    MS_U16                 u16DivNum;
    MS_BOOL                bClkInv;
    MS_BOOL                bSet;
}DMX_TSO_OUTCLK_PARAM, *PDMX_TSO_OUTCLK_PARAM;

typedef struct DLL_PACKED _DMX_TSO_LOCSTRID_PARAM
{
    MS_U32                 u32Res;
    MS_U8                  u8Eng;
    DMX_TSIF               eIf;
    MS_U8*                 pu8StrId;
    MS_BOOL                bSet;
}DMX_TSO_LOCSTRID_PARAM, *PDMX_TSO_LOCSTRID_PARAM;

typedef struct DLL_PACKED _DMX_FQ_INIT_PARAM
{
    MS_U32                 u32Res;
    MS_U32                 u32Eng;
    DMX_FQ_Info*           pInfo;
}DMX_FQ_INIT_PARAM, *PDMX_FQ_INIT_PARAM;

typedef struct DLL_PACKED _DMX_FQ_SKIP_RUSH_DATA_PARAM
{
    MS_U32                 u32Res;
    MS_U32                 u32Eng;
    DMX_FQ_SkipPath        eSkipPath;
}DMX_FQ_SKIP_RUSH_DATA_PARAM, *PDMX_FQ_SKIP_RUSH_DATA_PARAM;

typedef struct DLL_PACKED _DMX_FQ_CONFIGURE_PARAM
{
    MS_U32                 u32Res;
    DMX_FQ_Cfg*            pCfg;
}DMX_FQ_CONFIGURE_PARAM, *PDMX_FQ_CONFIGURE_PARAM;

typedef struct DLL_PACKED _DMX_MSTR_SYNCBY_PARAM
{
    MS_U32                  u32Res;
    DMX_TSIF                eIf;
    MS_U8                   u8StrId;
    MS_U8*                  pu8SyncByte;
    MS_BOOL                 bSet;
}DMX_MSTR_SYNCBY_PARAM, *PDMX_MSTR_SYNCBY_PARAM;

typedef struct DLL_PACKED _DMX_SyncByte_CONFIGURE_PARAM
{
    MS_U32                  u32Res;
    DMX_TSIF                eIf;
    DMX_SyncByte_Cfg*       pstSyncByteCfg;
}DMX_SyncByte_CONFIGURE_PARAM, *PDMX_SyncByte_CONFIGURE_PARAM;


typedef struct DLL_PACKED _DMX_OutputPad_Cfg
{
    MS_U32              u32Res;
    DMX_FLOW_OUTPUT_PAD eOutPad;
    DMX_FLOW_INPUT      eInSrcPad;
    MS_BOOL             bInParallel;
    MS_U32              u32ResvNum;
    MS_U32*             pu32Resv;
}DMX_OutputPad_Cfg, *PDMX_OutputPad_Cfg;

typedef struct DLL_PACKED _DMX_FLOW_DSCMBENG_PARAM
{
    MS_U32              u32Res;
    MS_U32              u32DscmbEng;
    DMX_TSIF            eTsif;
    MS_BOOL             bSet;
}DMX_FLOW_DSCMBENG_PARAM, *PDMX_FLOW_DSCMBENG_PARAM;

typedef struct DLL_PACKED _DMX_DROP_SCMB_PKT_PARAM
{
    MS_U32              u32Res;
    DMX_FILTER_TYPE     eType;
    MS_BOOL             bEnable;
}DMX_DROP_SCMB_PKT_PARAM,*PDMX_DROP_SCMB_PKT_PARAM;

typedef struct DLL_PACKED _DMX_CILINK_CONFIG_PARAM
{
    MS_U32                 u32Res;
    DMX_CILINK_ENG         eEng;
    DMX_CILINK_Cfg*        pstCilinkCfg;
}DMX_CILINK_CONFIG_PARAM,*PDMX_CILINK_CONFIG_PARAM;

#ifdef __cplusplus
}
#endif

typedef struct DLL_PACKED _DMX_TRACEMARK_PARAM
{
    MS_U32                  u32Res;
    MS_BOOL                 bEnable;
    DMX_FILTER_TYPE         DmxFltType;
}DMX_TRACEMARK_PARAM, *PDMX_TRACEMARK_PARAM;

#endif //_DMX_H_


