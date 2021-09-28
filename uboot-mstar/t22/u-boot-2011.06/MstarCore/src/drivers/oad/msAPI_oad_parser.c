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
#ifndef _MSAPI_OAD_PARSER_C_
#define _MSAPI_OAD_PARSER_C_


//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define HIGHBYTE(u16)               ((MS_U8)((u16) >> 8))
#define LOWBYTE(u16)                ((MS_U8)(u16))

/*------------------------------------------------------------------------------
*   include files
*-----------------------------------------------------------------------------*/
#include "msAPI_oad_parser.h"
#include "msAPI_oad_process.h"
#include <linux/string.h>
#include <asm/string.h>
#include <MsTypes.h>

extern MS_U8 g_OAD_OUI[3];
static  void    msAPI_OAD_RestartSectionFilter(MS_U8  u08OadFID);

/******************************************************************************/
// The following functions is used by AP code.
/******************************************************************************/
MS_U8 *msAPI_OAD_PollingSectionFilter(MS_U8  u08OadFID,MS_U8  *sectionBuffer)
{
    MS_U32 u32ActualSize,u32RmnSize;
    MS_U8 *pu8SecBuf = NULL;
    DMX_FILTER_STATUS procRet ;
    DMX_EVENT eEvent = 0 ;//DMX_EVENT_DATA_READY;

    if( u08OadFID == MSAPI_DMX_INVALID_FLT_ID ) return pu8SecBuf;
    OAD_DEBUG(printf("u08OadFID = %d \n",(MS_U8)u08OadFID)) ;  
   
    procRet = MApi_DMX_Proc(u08OadFID,&eEvent) ;


    OAD_DEBUG(printf("eEvent = %d \n",(MS_U8)eEvent)) ;

    OAD_DEBUG(printf("procRet = %d \n",(MS_U8)procRet));

    if (procRet==DMX_FILTER_STATUS_OK)
    {
        if (eEvent == DMX_EVENT_DATA_READY)
        {
            OAD_DEBUG(memset(sectionBuffer, 0xBB, MSAPI_DMX_SECTION_4K));
            OAD_DEBUG(printf("section Addr: 0x%08LX\n", (sectionBuffer)));

            //Patch for section data error.
            //MsOS_DelayTask(5);
            if( MApi_DMX_CopyData(u08OadFID, sectionBuffer, MSAPI_DMX_SECTION_4K,(MS_U32*)&u32ActualSize,(MS_U32*)&u32RmnSize,NULL) == DMX_FILTER_STATUS_OK )
            {
                if(msAPI_DMX_CheckCRC32(sectionBuffer)== FALSE)
                {
                    OAD_ASSERT(printf("crc error %s %d \n",__FUNCTION__,__LINE__));
                    //msAPI_OAD_RestartSectionFilter(u08OadFID); 
                    pu8SecBuf = NULL ;
                }
                else
                {
                    pu8SecBuf = sectionBuffer;
                }
            }
            else
            {
                if ( 0 != u32RmnSize)
                {
                    OAD_ASSERT( printf("Error> msAPI_OAD_PollingSectionFilter : MApi_DMX_CopyData Error.\n") );
                    msAPI_OAD_RestartSectionFilter(u08OadFID);
                }
            }
        }
        else
        {
            if (DMX_EVENT_BUF_OVERFLOW == eEvent)
            {
                OAD_ASSERT( printf("Polling OVERFLOW = %u\n", (MS_U8)u08OadFID); );
                msAPI_OAD_RestartSectionFilter(u08OadFID);
            }
        }
    }
    return pu8SecBuf;
}

MS_BOOL msAPI_OAD_CreateSectionFilter( MS_U32 u32Type,MS_U8 u08TableId,MS_U16 u16MsgId, MS_U16 u16Pid,MS_U8  *u08OadFID)
{
    MS_DMX_SECTIONMONITOR_PARA   stSectionMonitor;
    MS_U32 u32GroupId;
    MS_U32 u32DownloadId;

    memset(stSectionMonitor.au8MaskByte,     0x00, MSAPI_DMX_MATCH_PATTERN_LENGTH);
    memset(stSectionMonitor.au8NotMatchByte, 0x00, MSAPI_DMX_MATCH_PATTERN_LENGTH);

    stSectionMonitor.u32SectionFltType   = u32Type;
    stSectionMonitor.au8MatchByte[0]    = u08TableId;              // table id
    stSectionMonitor.au8MaskByte[0]     = 0xFF;
    if (u08TableId!=OAD_UNT_SECTION_TYPE)
    {
       stSectionMonitor.au8MatchByte[8] = 0x11;//protocolDiscriminator
       stSectionMonitor.au8MaskByte[8] = 0xFF;
       stSectionMonitor.au8MatchByte[9] = 0x03;//dsmccType
       stSectionMonitor.au8MaskByte[9] = 0xFF;
       stSectionMonitor.au8MatchByte[10] = HIGHBYTE(u16MsgId);//HIGHBYTE(OAD_DSMCC_MSGID_DDB);//messageId[1]
       stSectionMonitor.au8MatchByte[11] = LOWBYTE(u16MsgId) ;//LOWBYTE(OAD_DSMCC_MSGID_DDB); //messageId[0]
       stSectionMonitor.au8MaskByte[10] = 0xFF;
       stSectionMonitor.au8MaskByte[11] = 0xFF;
       stSectionMonitor.pfNotify           = NULL;
       stSectionMonitor.u16TimeoutMilliSec = MSAPI_DMX_TIMEOUT_INFINITE;
       stSectionMonitor.SectMode           = DMX_SECT_MODE_CONTI|DMX_SECT_MODE_CRCCHK;

       if(u16MsgId == OAD_DSMCC_MSGID_DII)
       {
           u32GroupId = msAPI_OAD_GetGroupId();
           stSectionMonitor.au8MatchByte[12] = u32GroupId >> 24;
           stSectionMonitor.au8MaskByte[12] = 0xFF;
           stSectionMonitor.au8MatchByte[13] = (u32GroupId >> 16) & 0xFF;
           stSectionMonitor.au8MaskByte[13] = 0xFF;
           stSectionMonitor.au8MatchByte[14] = (u32GroupId >> 8) & 0xFF;
           stSectionMonitor.au8MaskByte[14] = 0xFF;
           stSectionMonitor.au8MatchByte[15] = (u32GroupId & 0xFF);
           stSectionMonitor.au8MaskByte[15] = 0xFF;

       }
       else if(u16MsgId == OAD_DSMCC_MSGID_DDB)
       {
           u32DownloadId = msAPI_OAD_GetDownloadId();
           stSectionMonitor.au8MatchByte[12] = u32DownloadId >> 24;
           stSectionMonitor.au8MaskByte[12] = 0xFF;
           stSectionMonitor.au8MatchByte[13] = (u32DownloadId >> 16) & 0xFF;
           stSectionMonitor.au8MaskByte[13] = 0xFF;
           stSectionMonitor.au8MatchByte[14] = (u32DownloadId >> 8) & 0xFF;
           stSectionMonitor.au8MaskByte[14] = 0xFF;
           stSectionMonitor.au8MatchByte[15] = (u32DownloadId & 0xFF);
           stSectionMonitor.au8MaskByte[15] = 0xFF;
       }
    }
    else
    {
       stSectionMonitor.au8MatchByte[8] = (GET_3BYTE(g_OAD_OUI)&0xff0000)>>16 ;
       stSectionMonitor.au8MaskByte[8] = 0xFF;
       stSectionMonitor.au8MatchByte[9] = (GET_3BYTE(g_OAD_OUI)&0x00ff00)>>8 ;
       stSectionMonitor.au8MaskByte[9] = 0xFF;
       stSectionMonitor.au8MatchByte[10] = (GET_3BYTE(g_OAD_OUI)&0x0000ff) ;
       stSectionMonitor.au8MaskByte[10] = 0xFF;
       stSectionMonitor.pfNotify           = NULL;
       stSectionMonitor.u16TimeoutMilliSec = MSAPI_DMX_TIMEOUT_INFINITE;
       stSectionMonitor.SectMode           = DMX_SECT_MODE_CONTI|DMX_SECT_MODE_CRCCHK;
    }
    stSectionMonitor.u16PID             = u16Pid;
    stSectionMonitor.pu8FID             = u08OadFID;

    if(msAPI_DMX_SectionMonitor(&stSectionMonitor, FALSE) == DMX_FILTER_STATUS_OK)
    {
        return TRUE ;
    }
    else
    {
        return FALSE ;
    }

}

void msAPI_OAD_DeleteSectionFilter(MS_U8  *u08OadFID)
{
    if( *u08OadFID == MSAPI_DMX_INVALID_FLT_ID ) return;

    msAPI_DMX_Stop(*u08OadFID);
    msAPI_DMX_Close(*u08OadFID);

    *u08OadFID = MSAPI_DMX_INVALID_FLT_ID;
}

static void msAPI_OAD_RestartSectionFilter(MS_U8  u08OadFID)
{
    if( u08OadFID == MSAPI_DMX_INVALID_FLT_ID ) return;

    msAPI_DMX_Stop(u08OadFID);
    msAPI_DMX_Reset(u08OadFID);
    msAPI_DMX_Start(u08OadFID);
}

#undef _MSAPI_OAD_PARSER_C_
#endif //_MSAPI_OAD_PARSER_C_

