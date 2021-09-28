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

//--------------------------------------------------------------------------------------------------
// Include Files
//--------------------------------------------------------------------------------------------------
#include <common.h>
#include <MsCommon.h>
#include <MsOS.h>
#include <apiJPEG.h>
#include <uboot_mmap.h>
#include <MsMmap.h>
#include "MsDrvJpd.h"
#include <MsDebug.h>
#include <MsSystem.h>
#include <MsEnvironment.h>


unsigned int TARGET_LOGO_ADDR;
unsigned int TARGET_LOGO_SIZE;

int g_JpgPitch;
int g_JpgWidth;
int g_JpgHeight;

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//define jpd library
#define UTOPIA_JPD_OLD  1
#define UTOPIA_JPD_NEW  2
#define MS_JPD_API UTOPIA_JPD_NEW

//define slide show test
//#define SLIDE_SHOW_TEST

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------

#if CONFIG_ENABLE_4K2K_PANEL
#define JPEGBASEWIDTHMAX    3840
#define JPEGBASEHEIGHTMAX   2160
#define JPEGPROWIDTHMAX     3840
#define JPEGPROHEIGHTMAX    2160
#elif defined(__ARM__)
#define JPEGBASEWIDTHMAX    1920
#define JPEGBASEHEIGHTMAX   1080
#define JPEGPROWIDTHMAX     1920
#define JPEGPROHEIGHTMAX    1080
#else 
#define JPEGBASEWIDTHMAX    1600
#define JPEGBASEHEIGHTMAX   1200
#define JPEGPROWIDTHMAX     1024
#define JPEGPROHEIGHTMAX    768
#endif

#define MsDrv_DEBUG(x)     //x

#define printf_black(args...)         do{ printf("\033[1;30m"); printf(args); printf("\033[m"); }while(0)
#define printf_red(args...)           do{ printf("\033[1;31m"); printf(args); printf("\033[m"); }while(0)
#define printf_green(args...)         do{ printf("\033[1;32m"); printf(args); printf("\033[m"); }while(0)
#define printf_yellow(args...)        do{ printf("\033[1;33m"); printf(args); printf("\033[m"); }while(0)
#define printf_blue(args...)          do{ printf("\033[1;34m"); printf(args); printf("\033[m"); }while(0)
#define printf_purple(args...)        do{ printf("\033[1;35m"); printf(args); printf("\033[m"); }while(0)
#define printf_darkgreen(args...)     do{ printf("\033[1;36m"); printf(args); printf("\033[m"); }while(0)
#define printf_white(args...)         do{ printf("\033[1;37m"); printf(args); printf("\033[m"); }while(0)



#if defined(CHIP_T3) || defined(CHIP_T4) || defined(CHIP_T7) || defined(CHIP_JANUS) || defined(CHIP_U4)
#define VERJPD_EVENT_ALL    (E_JPEG_EVENT_DEC_DONE | E_JPEG_EVENT_DEC_ERROR_MASK | E_JPEG_EVENT_DEC_MRB_DONE | E_JPEG_EVENT_DEC_MWB_FULL)
#else
#define VERJPD_EVENT_ALL    (E_JPEG_EVENT_DEC_DONE | E_JPEG_EVENT_DEC_ERROR_MASK | E_JPEG_EVENT_DEC_MRB_DONE)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------------------------------------------
static EN_JPEG_DECODE_STATE g_state = E_JPEG_INIT;
static MS_U32 u32ReadBuffVirAddr = 0;
static MS_U32 u32IntBuffVirAddr = 0;
static MS_U32 u32OutBuffVirAddr = 0;
static MS_PHYADDR ReadBuffPA = 0;
static MS_PHYADDR WriteBuffPA = 0;
static MS_PHYADDR InterBuffPA = 0;
static U32 u32ReadBuffSize = 0;
static U32 u32WriteBuffSize = 0;
static U32 u32InterBuffSize = 0;

static MS_U16 u16ImageAlignPitch = 0, u16ImageAlignWidth = 0, u16ImageAlignHeight = 0;


static MS_U8 *u8InputDataAddr = NULL;
static MS_U32 u32InputDataLength = 0;
static MS_U32 u32ReadPosition = 0;

static MS_S32 s32InitByteRead = 0;
static MS_BOOL u8EOF_flag = FALSE;

static JPEG_Info stJPEG_Info;
static JPEG_Status stJPEG_Status;
static const MSIF_Version *pu8JPEG_Ver;

static JPEG_EXIF_DateTime _gEXIF_DateTime;
static JPEG_EXIF_Orientation _gEXIF_Orientation;

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------
static MS_S32 MsDrv_FILE_read(MS_U8 *Pbuf, MS_U32 max_bytes_to_read, MS_BOOL *Peof_flag);
static MS_S32 MsDrv_FillHdrFunc(MS_U32 BufAddr, MS_U32 BufLength);
static MS_BOOL MsDrv_PreLoadBuffer(JPEG_BuffLoadType u8MRBuffType);
static void MsDrv_SetStatus(void);
//static void MsDrv_Init(void);
//static void MsDrv_StartDecode(EN_JPEG_DECODE_TYPE decode_type);

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static MS_S32 MsDrv_FILE_read(MS_U8 *Pbuf, MS_U32 max_bytes_to_read, MS_BOOL *Peof_flag)
{
    MS_U8* u8RequestDataAddr = 0;
    MS_S32 bytes_read = 0;
    MS_S32 i = 0;
    UBOOT_TRACE("IN\n");

    u8RequestDataAddr = Pbuf;

   // UBOOT_DEBUG("%s,u8RequestDataAddr = 0x%lx, FilePosition = %lu, FileLength = %lu\n" , __FUNCTION__, (MS_U32)u8RequestDataAddr, u32ReadPosition, u32InputDataLength);

    if (u32ReadPosition <= u32InputDataLength)
    {
        bytes_read = max_bytes_to_read;
        if ((u32ReadPosition + max_bytes_to_read) >= u32InputDataLength)
        {
            bytes_read = u32InputDataLength - u32ReadPosition;
            *Peof_flag = TRUE;
        }

        for(i = 0; i < bytes_read; i++)
        {
            u8RequestDataAddr[i] = u8InputDataAddr[u32ReadPosition + i];
        }
        MsOS_FlushMemory();
    }
    else
    {
        *Peof_flag = TRUE;
    }
    u32ReadPosition += bytes_read;
    UBOOT_DEBUG("%s, bytes_read = %lu\n", __FUNCTION__, bytes_read);
    UBOOT_TRACE("OK\n");
    return bytes_read;
}

//--------------------------------------------------------------------------------------------------

static MS_S32 MsDrv_FillHdrFunc(MS_PHYADDR BufAddr, MS_U32 BufLength)
{
    MS_S32 bytes_read = 0;
    MS_U32 buf_left = 0;
    MS_U32 bufVirAddr = MS_PA2KSEG1((MS_U32)BufAddr);
    UBOOT_DEBUG("%s, BufAddr = %lu, bufVirAddr = %lu\n", __FUNCTION__, (MS_U32)BufAddr, (MS_U32)bufVirAddr);
    do
    {
        bytes_read = MsDrv_FILE_read((MS_U8 *)(bufVirAddr + buf_left), BufLength - buf_left, &u8EOF_flag);

        if (bytes_read < 0)
        {
            return bytes_read;
        }

        buf_left += bytes_read;
    } while(( buf_left < BufLength ) && ( !u8EOF_flag ));
    MApi_JPEG_UpdateReadInfo(buf_left, u8EOF_flag);
    return buf_left;
}


//--------------------------------------------------------------------------------------------------
static MS_BOOL MsDrv_PreLoadBuffer(JPEG_BuffLoadType u8MRBuffType)
{
    MS_S32 bytes_read = 0;
    MS_U32 buf_left = 0;
    MS_U32 u32RequestDataAddr, u32RequestDataSize;
    UBOOT_TRACE("IN\n");

    if(u8EOF_flag)
    {
        MApi_JPEG_ProcessEOF(u8MRBuffType);
        return TRUE;
    }

    u32RequestDataAddr = u32ReadBuffVirAddr;
    u32RequestDataSize = u32ReadBuffSize/2;

    if(u8MRBuffType == E_JPEG_BUFFER_HIGH)
    {
        u32RequestDataAddr += u32RequestDataSize;
    }

    do
    {
        bytes_read = MsDrv_FILE_read((MS_U8 *)(u32RequestDataAddr + buf_left), u32RequestDataSize - buf_left, &u8EOF_flag);

        if (bytes_read == -1)
        {
            MApi_JPEG_SetErrCode( E_JPEG_STREAM_READ );
            return FALSE;
        }

        buf_left += bytes_read;
    } while(( buf_left < u32RequestDataSize ) && ( !u8EOF_flag ));
    MApi_JPEG_UpdateReadInfo(buf_left, u8EOF_flag);
    if(buf_left > 0)
    {
        MApi_JPEG_SetMRBufferValid(u8MRBuffType);
    }
    UBOOT_TRACE("OK\n");
    return TRUE;
}

//--------------------------------------------------------------------------------------------------
static EN_JPEG_DECODE_STATUS MsDrv_WaitDone(void)
{
    EN_JPEG_DECODE_STATUS eDecodeStatus = E_DECODING;
    UBOOT_TRACE("IN\n");
    MsOS_DelayTask(1);
    switch(MApi_JPEG_WaitDone())
    {
        case E_JPEG_DEC_DONE:
            UBOOT_DEBUG("ApiWaitDone_state: E_JPEG_DEC_DONE\n");
            eDecodeStatus = E_DECODE_DONE;
            break;
        case E_JPEG_DEC_FAILED:
        case E_JPEG_DEC_RST_ERROR:
        case E_JPEG_DEC_BITSTREAM_ERROR:
            UBOOT_ERROR("ApiWaitDone_state: E_JPEG_DEC_FAILED or E_JPEG_DEC_RST_ERROR or E_JPEG_DEC_BITSTREAM_ERROR\n");
            UBOOT_ERROR("Baseline decode error\n");
            eDecodeStatus = E_DECODE_ERR;
            break;
        case E_JPEG_DEC_MRBL_DONE:
            UBOOT_DEBUG("ApiWaitDone_state: E_JPEG_DEC_MRBL_DONE\n");
            if(!MsDrv_PreLoadBuffer(E_JPEG_BUFFER_LOW))
            {
                if(MApi_JPEG_GetErrorCode() == E_JPEG_STREAM_READ)
                    eDecodeStatus = E_STREAM_READ_ERR;
                else
                    eDecodeStatus = E_DECODE_ERR;
            }
            else
            {
                eDecodeStatus = E_DECODING;
            }
            break;
        case E_JPEG_DEC_MRBH_DONE:
            UBOOT_DEBUG("ApiWaitDone_state: E_JPEG_DEC_MRBH_DONE\n");
            if(!MsDrv_PreLoadBuffer(E_JPEG_BUFFER_HIGH))
            {
                if(MApi_JPEG_GetErrorCode() == E_JPEG_STREAM_READ)
                    eDecodeStatus = E_STREAM_READ_ERR;
                else
                    eDecodeStatus = E_DECODE_ERR;
            }
            else
            {
                eDecodeStatus = E_DECODING;
            }
            break;
        case E_JPEG_DEC_DECODING:
        default:
            MsDrv_DEBUG(printf_yellow("ApiWaitDone_state: E_JPEG_DEC_DECODING\n"));
            eDecodeStatus = E_DECODING;
            break;
    }
    UBOOT_TRACE("OK\n");
    return eDecodeStatus;
}

//--------------------------------------------------------------------------------------------------
static void MsDrv_SetStatus(void)
{
    UBOOT_TRACE("IN\n");
    if( g_state == E_JPEG_DECODE_DONE )
    {
        UBOOT_DEBUG("MsDrv_SetStatus:E_JPEG_DECODE_DONE\n");
        //u16ImageWidth = MApi_JPEG_GetWidth();
        //u16ImageHeight = MApi_JPEG_GetHeight();
        //bIsProgressive = MApi_JPEG_IsProgressive();
        //u16ImageOriWidth = MApi_JPEG_GetOriginalWidth();
        //u16ImageOriHeight = MApi_JPEG_GetOriginalHeight();
        //u16ImageNonAlignWidth = MApi_JPEG_GetNonAlignmentWidth();
        //u16ImageNonAlignHeight = MApi_JPEG_GetNonAlignmentHeight();
        //u8ScaleDownFactor = MApi_JPEG_GetScaleDownFactor();
        //u8DebugLevel = MApi_JPEG_GetDbgLevel();
        MApi_JPEG_GetInfo(&stJPEG_Info);
        MApi_JPEG_GetStatus(&stJPEG_Status);
        MApi_JPEG_GetLibVer(&pu8JPEG_Ver);
        u16ImageAlignPitch = MApi_JPEG_GetAlignedPitch(),
        u16ImageAlignWidth = MApi_JPEG_GetAlignedWidth(),
        u16ImageAlignHeight = MApi_JPEG_GetAlignedHeight();
//        bThumbnailFound = MApi_JPEG_ThumbnailFound();

        UBOOT_DEBUG("MsDrv_SetStatus >>>>>>>>>>> w:%d,  h:%d,  p:%d\n",u16ImageAlignWidth,u16ImageAlignHeight,u16ImageAlignPitch);
        g_JpgPitch = MApi_JPEG_GetAlignedPitch();
        g_JpgHeight = MApi_JPEG_GetAlignedHeight();
        g_JpgWidth = MApi_JPEG_GetAlignedWidth();

        MApi_JPEG_GetEXIFDateTime(&_gEXIF_DateTime);
        MApi_JPEG_GetEXIFOrientation(&_gEXIF_Orientation);
    }
    else if( g_state == E_JPEG_DECODE_ERR )
    {
        UBOOT_ERROR("MsDrv_SetStatus:E_JPEG_DECODE_ERR\n");
        //u16ImageWidth = MApi_JPEG_GetWidth();
        //u16ImageHeight = MApi_JPEG_GetHeight();
        //bIsProgressive = MApi_JPEG_IsProgressive();
        //u16ImageOriWidth = MApi_JPEG_GetOriginalWidth();
        //u16ImageOriHeight = MApi_JPEG_GetOriginalHeight();
        //u16ImageNonAlignWidth = MApi_JPEG_GetNonAlignmentWidth();
        //u16ImageNonAlignHeight = MApi_JPEG_GetNonAlignmentHeight();
        //u8ScaleDownFactor = MApi_JPEG_GetScaleDownFactor();
        //u16ErrorCode = MApi_JPEG_GetErrorCode();
        //u8DebugLevel = MApi_JPEG_GetDbgLevel();
        MApi_JPEG_GetInfo(&stJPEG_Info);
        MApi_JPEG_GetStatus(&stJPEG_Status);
        MApi_JPEG_GetLibVer(&pu8JPEG_Ver);
        u16ImageAlignPitch = MApi_JPEG_GetAlignedPitch(),
        u16ImageAlignWidth = MApi_JPEG_GetAlignedWidth(),
        u16ImageAlignHeight = MApi_JPEG_GetAlignedHeight();
//        bThumbnailFound = MApi_JPEG_ThumbnailFound();
        MApi_JPEG_GetEXIFDateTime(&_gEXIF_DateTime);
        MApi_JPEG_GetEXIFOrientation(&_gEXIF_Orientation);
    }
    UBOOT_TRACE("OK\n");
}

//--------------------------------------------------------------------------------------------------
/// API for JPEG decoder initialization::
int MsDrv_JpdInit(void)
{
    UBOOT_TRACE("IN\n");

    //for test code
    g_state = E_JPEG_INIT;
    //u16ImageWidth = 0;
    //u16ImageHeight = 0;
    //u16ImageOriWidth = 0;
    //u16ImageOriHeight = 0;
    //u16ImageNonAlignWidth = 0;
    //u16ImageNonAlignHeight = 0;
    //u16ErrorCode = 0;
    //u8ScaleDownFactor = 0;
    //bIsProgressive = FALSE;

    //u8DebugLevel = 0x0;

    s32InitByteRead = 0;
    u8EOF_flag = FALSE;

    //E_MMAP_ID_JPD_READ 
    if(get_map_addr_from_env(E_JPD_READ, MMAP_E_MMAP_ID_JPD_READ_ADDR, &ReadBuffPA)!=0)
    {
        UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_JPD_READ_ADR);
        return -1;
    }

    if(get_map_size_from_env(E_JPD_READ, MMAP_E_MMAP_ID_JPD_READ_SIZE, &u32ReadBuffSize)!=0)
    {
        UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_JPD_READ_LEN);
        return -1;
    }

    //E_MMAP_ID_JPD_WRITE
    if(get_map_addr_from_env(E_JPD_WRITE, MMAP_E_MMAP_ID_JPD_WRITE_ADDR, &WriteBuffPA)!=0)
    {
        UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_JPD_WRITE_ADR);
        return -1;
    }


    if(get_map_size_from_env(E_JPD_WRITE, MMAP_E_MMAP_ID_JPD_WRITE_SIZE, &u32WriteBuffSize)!=0)
    {
        UBOOT_ERROR("get %s fail\n",E_MMAP_ID_JPD_WRITE_LEN);
        return -1;
    }

    //E_MMAP_ID_PHOTO_INTER
    if(get_map_addr_from_env(E_PHOTO_INTER, MMAP_E_MMAP_ID_PHOTO_INTER_ADDR, &InterBuffPA)!=0)
    {
        UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_PHOTO_INTER_ADR);
        return -1;
    }


    if(get_map_size_from_env(E_PHOTO_INTER, MMAP_E_MMAP_ID_PHOTO_INTER_SIZE, &u32InterBuffSize)!=0)
    {
        UBOOT_ERROR("get %s mmap fail\n",E_MMAP_ID_PHOTO_INTER_LEN);
        return -1;
    }

    if((ReadBuffPA == 0)   ||(WriteBuffPA == 0)   || (InterBuffPA == 0) ||
       (u32ReadBuffSize == 0) ||(u32WriteBuffSize == 0) || (u32InterBuffSize == 0))
    {
        UBOOT_ERROR("invalid bootlogo JPD buffer address\n");
    }

    u32ReadBuffVirAddr = MS_PA2KSEG1(ReadBuffPA);
    u32IntBuffVirAddr = MS_PA2KSEG1(InterBuffPA);
    u32OutBuffVirAddr = MS_PA2KSEG1(WriteBuffPA);

    UBOOT_DEBUG("u32ReadBuffVirAddr = %lx, u32IntBuffVirAddr = %lx, u32OutBuffVirAddr = %lx\n", u32ReadBuffVirAddr, u32IntBuffVirAddr, u32OutBuffVirAddr);


    //logo jpd decode using BaseLine
    u8InputDataAddr = (MS_U8 *)TARGET_LOGO_ADDR;
    u32InputDataLength = TARGET_LOGO_SIZE;

    u32ReadPosition = 0;
    _gEXIF_DateTime.bHasDataTime = FALSE;
    _gEXIF_DateTime.u32Year = 0;
    _gEXIF_DateTime.u32Month = 0;
    _gEXIF_DateTime.u32Day = 0;
    _gEXIF_DateTime.u32Hour = 0;
    _gEXIF_DateTime.u32Minute = 0;
    _gEXIF_DateTime.u32Second = 0;
    _gEXIF_Orientation = E_JPEG_EXIF_ORIENT_NOT_FOUND;

    //MsDrv_InitFileSystem();
    MApi_JPEG_SetDbgLevel(E_JPEG_DEBUG_NONE);

    //MApi_JPEG_SetDbgLevel(E_JPEG_DEBUG_ALL);
    UBOOT_TRACE("OK\n");
    return 0;
}

//--------------------------------------------------------------------------------------------------
/// API for starting JPEG data decoding
//--------------------------------------------------------------------------------------------------
void MsDrv_JpdStartDecode(EN_JPEG_DECODE_TYPE decode_type)
{
    UBOOT_TRACE("IN\n");
    if(g_state != E_JPEG_INIT)
    {
        UBOOT_ERROR("%s failed, state = %d\n", __FUNCTION__, g_state);
        return;
    }

    // init JPEG DECODE BUFFER
    JPEG_InitParam InitParam;

    UBOOT_DEBUG("%s: INIT\n", __FUNCTION__);

    MApi_JPEG_SetMaxDecodeResolution(JPEGBASEWIDTHMAX, JPEGBASEHEIGHTMAX);
    MApi_JPEG_SetProMaxDecodeResolution(JPEGPROWIDTHMAX, JPEGPROHEIGHTMAX);

    //InitParam.u32MRCBufAddr =  MS_VA2PA((((unsigned int)0x80A01000)|0x20000000) + 0x278000);  // (((unsigned int)0x80A01000)|0x20000000) + 0x278000; //MsDrv_READBUFF_ADR;
    //InitParam.u32MRCBufAddr =  MS_VA2PA((((unsigned int)0x80A01000)|0x20000000) + ResBIN_LogoOffset);


    InitParam.u32MRCBufAddr = ReadBuffPA;
    InitParam.u32MWCBufAddr = WriteBuffPA;
    InitParam.u32InternalBufAddr = InterBuffPA;
    InitParam.u32MRCBufSize = u32ReadBuffSize;
    InitParam.u32MWCBufSize = u32WriteBuffSize;
    InitParam.u32InternalBufSize = u32InterBuffSize;

    InitParam.u32DecByteRead =  (MS_U32)s32InitByteRead;//TARGET_LOGO_SIZE;// file size 
    InitParam.bEOF = u8EOF_flag; // TRUE, the number of read_act, End of File ?
    if(E_JPD_MAIN_DECODE == decode_type)
    {
        InitParam.u8DecodeType = E_JPEG_TYPE_MAIN;
    }
    else
    {
        InitParam.u8DecodeType = E_JPEG_TYPE_THUMBNAIL;
    }
    InitParam.bInitMem = TRUE;
    InitParam.pFillHdrFunc = (JPEG_FillHdrFunc)MsDrv_FillHdrFunc; // NULL

    MApi_JPEG_Init(&InitParam);

    if(MApi_JPEG_GetErrorCode() == E_JPEG_NO_ERROR)
    {
        g_state = E_JPEG_DECODE_HEADER;
    }
    else
    {
        g_state = E_JPEG_DECODE_ERR;
        UBOOT_ERROR("%s: Init failed\n", __FUNCTION__);
    }

    UBOOT_DEBUG("%s: INIT DONE : EventFlag = 0x%04X\n", __FUNCTION__, MApi_JPEG_GetJPDEventFlag());

    //parse JPEG Header
    if(g_state == E_JPEG_DECODE_HEADER)
    {
        UBOOT_DEBUG("%s: DECODE HEADER=======================================\n", __FUNCTION__);
        if(E_JPEG_FAILED == MApi_JPEG_DecodeHdr())
        {
            if(MApi_JPEG_GetErrorCode() == E_JPEG_STREAM_READ)
            {
                UBOOT_ERROR("%s: DecodeHdr read failed\n", __FUNCTION__);
                g_state = E_JPEG_IDLE;
            }
            else
            {
                UBOOT_ERROR("%s: DecodeHdr failed\n", __FUNCTION__);
                g_state = E_JPEG_DECODE_ERR;
            }
        }
        else
        {
            g_state = E_JPEG_DECODE_BEGIN;
        }
    }
    UBOOT_DEBUG("%s: DECODE HEADER DONE : EventFlag = 0x%04X\n", __FUNCTION__, MApi_JPEG_GetJPDEventFlag());

    if(g_state == E_JPEG_IDLE)
    {
        MsDrv_DEBUG(printf("%s: IDLE\n", __FUNCTION__));
        MApi_JPEG_Rst();
        MApi_JPEG_Exit();
        g_state = E_JPEG_INIT;
        return;
    }

    UBOOT_DEBUG("%s: DECODE BEGIN========================================\n", __FUNCTION__);
    //begin decode
    while(g_state == E_JPEG_DECODE_BEGIN)
    {
        switch(MApi_JPEG_Decode())
        {
            case E_JPEG_DONE:
                // This state is only for progressive JPEG file
                UBOOT_DEBUG("Decode_state: E_JPEG_DONE\n");
                g_state = E_JPEG_DECODE_BEGIN;
                break;
            case E_JPEG_OKAY:
                UBOOT_DEBUG("Decode_state: E_JPEG_OKAY\n");
                g_state = E_JPEG_WAITDONE;
                break;
            case E_JPEG_FAILED:
            default:
                UBOOT_ERROR("Decode_state: E_JPEG_FAILED\n");
                UBOOT_ERROR("MsDrv_StartDecode: DecodeBegin failed\n");
                g_state = E_JPEG_DECODE_ERR;
                break;
        }
    }
    UBOOT_DEBUG("%s: DECODE BEGIN DONE : EventFlag = 0x%04X\n", __FUNCTION__, MApi_JPEG_GetJPDEventFlag());

    //wait done
    UBOOT_DEBUG("%s: WAITDONE============================================\n", __FUNCTION__);
    while(g_state == E_JPEG_WAITDONE)
    {
        switch(MsDrv_WaitDone())
        {
            case E_DECODE_DONE:
                UBOOT_DEBUG("APPWaitDone_state: E_DECODE_DONE\n");
                printf("[AT][MB][JPD Decode][%lu]\n", MsSystemGetBootTime());
                g_state = E_JPEG_DECODE_DONE;
                break;

            case E_DECODING:
                UBOOT_DEBUG("APPWaitDone_state: E_DECODING\n");
                g_state = E_JPEG_WAITDONE;
                break;

            case E_STREAM_READ_ERR:
            case E_DECODE_ERR:
            default:
                UBOOT_ERROR("APPWaitDone_state: E_STREAM_READ_ERR or E_DECODE_ERR\n");
                UBOOT_ERROR("%s: WaitDone failed\n", __FUNCTION__);
                g_state = E_JPEG_DECODE_ERR;
                MApi_JPEG_Rst();
                break;
        }
    }

    MsDrv_SetStatus();
    MApi_JPEG_Exit();
    g_state = E_JPEG_INIT;
    UBOOT_TRACE("OK\n");
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
int JPG_GetAlignmentWidth(void)
{
    return g_JpgWidth;
}
int JPG_GetAlignmentHeight(void)
{
    return g_JpgHeight;
}
int JPG_GetAlignmentPitch(void)
{
    return g_JpgPitch;
}
MS_PHYADDR JPG_GetOutRawDataAddr(void)
{
    return WriteBuffPA;
}
//--------------------------------------------------------------------------------------------------
///Entry function for JPD testing.
//--------------------------------------------------------------------------------------------------
int MsDrv_JPD_Decode(MS_U32 u32SrcAddr, MS_U32 u32SrcSize)
{
    UBOOT_TRACE("IN\n");

    TARGET_LOGO_ADDR = u32SrcAddr;
    TARGET_LOGO_SIZE = u32SrcSize;

    if (MsDrv_JpdInit() != 0)
    {
        UBOOT_ERROR("MsDrv_JpdInit failed\n");
        return -1;
    }
    if(ReadBuffPA==0xFFFF || WriteBuffPA==0xFFFF ||InterBuffPA==0xFFFF)
    {
        UBOOT_ERROR("Get Mmap for JPD Fail Skip JPD Decode !!! \n");
        return -1;

    }

    //Preload data for init JPEG decoder data.
    s32InitByteRead = MsDrv_FILE_read((MS_U8 *)u32ReadBuffVirAddr, u32ReadBuffSize, &u8EOF_flag);

    if(s32InitByteRead < 0)
    {
        UBOOT_ERROR("Initial read file error!!\n");
        return -1;
    }

    MsDrv_JpdStartDecode(E_JPD_MAIN_DECODE);

    UBOOT_TRACE("OK\n");
    return 1;

}


