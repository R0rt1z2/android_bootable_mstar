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

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <common.h>
#include <config.h>
#if (CONFIG_BOOTVIDEO == 1)

#include <malloc.h>
#include <MsDebug.h>
#include <MsMmap.h>
#include <demux/MsDrvDemux.h>
#include <video/MsDrvVideo.h>
#include <audio/MsDrvAudio.h>
#include <gop/MsDrvGop.h>
#include <miu/MsDrvMiu.h>
#include <bootvideo/MsBootVideo.h>
//#include <cofdm/COFDM_Demodulator.h>
#include <MsEnvironment.h>

#include <MsVfs.h>
#include <MsSystem.h>
#include "CusConfig.h"

#include <drvSYS.h>
#include <drvAUDIO_if.h>

#include <drvIPAUTH.h>
//#include <Customer_Info.h>

#define DEBUG_CUSTOMER_INFO_ENABLE      0

#define ENV_BOOT_VIDEO_AUTOSCAN         "BOOT_VIDEO_AUTOSCAN"
#define ENV_BOOT_VIDEO_VPID             "BOOT_VIDEO_VPID"
#define ENV_BOOT_VIDEO_VCODEC           "BOOT_VIDEO_VCODEC"
#define ENV_BOOT_VIDEO_APID             "BOOT_VIDEO_APID"
#define ENV_BOOT_VIDEO_ACODEC           "BOOT_VIDEO_ACODEC"
#define ENV_BOOT_VIDEO_PCR              "BOOT_VIDEO_PCR"
#define ENV_BOOT_VIDEO_VOLUME           "BOOT_VIDEO_VOLUME"
#define ENV_BOOT_VIDEO_WAIT_SYNC        "BOOT_VIDEO_WAIT_SYNC"
#define ENV_BOOT_VIDEO_PLAYTIME         "BOOT_VIDEO_PLAYTIME"

#define ARG_BOOT_VIDEO_PTS_ADDR         "BOOT_VIDEO_VDEC_PTS_ADDR"
#define ARG_BOOT_VIDEO_PTS_KUNIT        "BOOT_VIDEO_VDEC_PTS_KUNIT"    // KhZ Unit

#define ARG_BOOT_VIDEO_FRAME_CNT        "BV_V_FCNT"
#define ARG_BOOT_VIDEO_SKIP_FRAME_CNT   "BV_V_SFCNT"

#define ENV_BOOT_VIDEO_FILE_REPEAT      "BOOT_VIDEO_REPEAT"
#define ENV_BOOT_VIDEO_FILE_ADDR        "BV_F_Adr"
#define ENV_BOOT_VIDEO_FILE_SIZE        "BV_F_Sz"
#define ENV_BOOT_VIDEO_FILE_START_TIME  "BV_F_STime"

#define MAX_BOOT_VIDEO_TEXT_LENGTH      64
#define ARG_BOOT_VIDEO_FILE_NAME        "bootvideo.ts"
#define ENV_BOOT_VIDEO_ADVERISEMENT     CUSTOMER // "ADV"

#define SYNC_INVALID_PTS         (0xFFFFFFFF)
#define SYNC_STC_MASK            (0xFFFFFFFFLL)        // // 0x1FFFFFFFFLL
#define _VPTS_TIMEOUT            (45000)   // 500ms    // timeout to get Video First PTS
#define DMX_MAX_FILE_IN_SIZE     (0xEFFF48)// alignment 188 byte (0xF00000) //15MB  // EFFF48
#define BOOT_VIDEO_PACKET_SIZE   (188)

#define ENV_BOOT_VIDEO_AUTOSCAN_DEFAULT 1
#define ENV_BOOT_VIDEO_VPID_DEFAULT     0x1FFF
#define ENV_BOOT_VIDEO_VCODEC_DEFAULT   E_MSDRV_VIDEO_CODEC_TYPE_MPEG2
#define ENV_BOOT_VIDEO_APID_DEFAULT     0x1FFF
#define ENV_BOOT_VIDEO_ACODEC_DEFAULT   E_MSDRV_AUDIO_CODEC_TYPE_MPEG
#define ENV_BOOT_VIDEO_PCR_DEFAULT      0x1FFF
#define ENV_BOOT_VIDEO_VOLUME_DEFAULT   100
#define ENV_BOOT_VIDEO_PLAYTIME_DEFAULT (0xFFFF)
#define ENV_BOOT_VIDEO_FILE_REPEAT_DEFAULT 1
#define ENV_BOOT_VIDEO_WAIT_SYNC_DEFAULT 0


#define GET_BOOT_VIDEO_ENV_VALUE(str)  _GetBootVideoEnvValue(str, str##_DEFAULT)

static MS_U8 _u8VideoDmxId = DMX_DMXID_NULL;
static MS_U8 _u8AudioDmxId = DMX_DMXID_NULL;
static MS_U8 _u8PcrDmxId = DMX_DMXID_NULL;
static MS_U32 u32WaitSync;

/*static unsigned char Customer_info[49];

extern MS_BOOL do_tuner_checklock(void);

static void _MApi_Init_CustomerInfo(MS_U8 *pu8CustomerInfo)
{
    unsigned char * pu8Temp;
    MS_U8 i,u8Data;
    MS_U8 u8ASCII_Mapping[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    MS_U32 dolbycontrol=0, chiptype=0, reverse=0;

    if(pu8CustomerInfo == NULL)
        return;

#if (DEBUG_CUSTOMER_INFO_ENABLE)
    printf("****** Customer_info:Before ******");
    for (i=0;i<48;i++)
    {
        if (i%8 ==0)
            printf("\n");
        printf("0x%bx,",pu8CustomerInfo[i]);

    }

    printf("\n abcdef => 0x%bx,0x%bx,0x%bx,0x%bx,0x%bx,0x%bx,",'a','b','c','d','e','f');
    printf("\n ABCDEF => 0x%bx,0x%bx,0x%bx,0x%bx,0x%bx,0x%bx,",'A','B','C','D','E','F');
    printf("\n\n");
#endif

    pu8Temp = &CID_Buf[6];//(Byte6,7: Customer ID)(Byte8,9: Model ID)(Byte1011: Chip ID)

    //<1>.To Prepare the Customer-ID + Model-ID + Chip-ID
    for (i=0;i<3;i++)
    { //Get hight byte
        u8Data = ((*(pu8Temp+1))>> 4);
        pu8CustomerInfo[0+i*4] = u8ASCII_Mapping[u8Data];
        u8Data = (*(pu8Temp+1) & 0x0F);
        pu8CustomerInfo[1+i*4] =  u8ASCII_Mapping[u8Data];
        //Get Low byte
        u8Data = ((*(pu8Temp))>>4) ;
        pu8CustomerInfo[2+i*4] = u8ASCII_Mapping[u8Data];
        u8Data = (*(pu8Temp) & 0x0F);
        pu8CustomerInfo[3+i*4] = u8ASCII_Mapping[u8Data];
        pu8Temp +=2;
    }

    //support Hash Key v2.0
    dolbycontrol = MDrv_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_CONTROL_BIT);
    chiptype = MDrv_SYS_GetChipType();
    reverse = MDrv_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_REVERSE_BIT);
    if(dolbycontrol == 1 && chiptype == 1 && reverse == 0)
    {
        printf("\033[1;32m *** hash key v2.0, use Customer_Info_V2.h to replace Customer_Info.h *** \033[0m \r\n");
        //Get hight byte
        u8Data = ((*(pu8Temp + 1)) >> 4);
        pu8CustomerInfo[0 + 12] = u8ASCII_Mapping[u8Data];
        u8Data = (*(pu8Temp + 1) & 0x0F);
        pu8CustomerInfo[1 + 12] =  u8ASCII_Mapping[u8Data];
        //Get Low byte
        u8Data = ((*(pu8Temp)) >> 4) ;
        pu8CustomerInfo[2 + 12] = u8ASCII_Mapping[u8Data];
        u8Data = (*(pu8Temp) & 0x0F);
        pu8CustomerInfo[3 + 12] = u8ASCII_Mapping[u8Data];
    }
    else
    {
        printf("\033[1;32m *** hash key v1.0, use Customer_Info.h directly *** \033[0m \r\n");
        for (i=0;i<4;i++)
            pu8CustomerInfo[12+i] = 0x30;
    }

//<2>.To prepare the Customer IP
      pu8Temp = &IP_Cntrol_Mapping_1[0];
      memcpy(&pu8CustomerInfo[16], pu8Temp, 8);
      pu8Temp = &IP_Cntrol_Mapping_2[0];
      memcpy(&pu8CustomerInfo[16+8], pu8Temp, 8);
      pu8Temp = &IP_Cntrol_Mapping_3[0];
      memcpy(&pu8CustomerInfo[16+16], pu8Temp, 8);
      pu8Temp = &IP_Cntrol_Mapping_4[0];
      memcpy(&pu8CustomerInfo[16+24], pu8Temp, 8);

#if (DEBUG_CUSTOMER_INFO_ENABLE)
    printf("****** Customer_info ******");
    for (i=0;i<48;i++)
    {
        if (i%8 ==0)
            printf("\n");
        printf("0x%bx,",pu8CustomerInfo[i]);
    }
#endif

    pu8CustomerInfo[48] = 0;  //Important, string end symbol
}

static void _MApi_Set_SecurityAuthentication(void)
{
    memset(Customer_info, 0, sizeof(Customer_info));
    _MApi_Init_CustomerInfo(Customer_info);

    MApi_AUTH_Process(Customer_info, Customer_hash);
}*/

static MS_U32 _GetBootVideoEnvValue(char *pszEnvString, MS_U32 u32DefaultValue)
{
    if(pszEnvString == NULL)
        return u32DefaultValue;

    char *pEnv = getenv(pszEnvString);
    if(pEnv == NULL)
    {
        UBOOT_INFO("Can't find %s \n",pszEnvString);
        return u32DefaultValue;
    }

    return (MS_U32)simple_strtoul(pEnv, NULL, 10);
}

static void _StopBootVideo(void)
{
    // close video dmx filter
    if(_u8VideoDmxId != DMX_DMXID_NULL)
    {
        MsDrv_DmxCloseFilter(_u8VideoDmxId);
        _u8VideoDmxId = DMX_DMXID_NULL;
    }

    // close audio dmx filter
    if(_u8AudioDmxId != DMX_DMXID_NULL)
    {
        MsDrv_DmxCloseFilter(_u8AudioDmxId);
        _u8AudioDmxId = DMX_DMXID_NULL;
    }

    if(_u8PcrDmxId != DMX_DMXID_NULL)
    {
        MsDrv_DmxCloseFilter(_u8PcrDmxId);
        _u8PcrDmxId = DMX_DMXID_NULL;
    }

    // stop audio decoder
    MsDrv_AudioStop();

    // stop video decoder
    MsDrv_VideoStop();

}

static void _SaveBootVideoBootArg(MsDrv_VIDEO_CodecType_e eVdecCodecType)
{
    char szText[32];
    MS_BOOL bRet = FALSE;
    MS_PHYADDR u32PtsAddr = 0;
    MS_U32 u32PtsKhzUnit = 0;
    MS_PHYADDR u32FrameCntAddr = 0;
    MS_PHYADDR u32SkipFrameCntAddr = 0;


    u32PtsAddr = MsDrv_VideoGetPtsAddr(eVdecCodecType, &u32PtsKhzUnit);

    sprintf(szText, ARG_BOOT_VIDEO_PTS_ADDR"=0x%llx", u32PtsAddr);
    set_bootargs_cfg(ARG_BOOT_VIDEO_PTS_ADDR, szText, 1);   // don't need to save

    sprintf(szText, ARG_BOOT_VIDEO_PTS_KUNIT"=%ld", u32PtsKhzUnit);
    set_bootargs_cfg(ARG_BOOT_VIDEO_PTS_KUNIT, szText, 1); // don't need to save

    bRet = MsDrv_VideoGetFrameCountAddr(eVdecCodecType, &u32FrameCntAddr, &u32SkipFrameCntAddr);
    //UBOOT_INFO("[%s %d] (0x%lx, 0x%lx)\n",__FUNCTION__,__LINE__,u32FrameCntAddr, u32SkipFrameCntAddr);

    if (bRet == TRUE)
    {
        sprintf(szText, ARG_BOOT_VIDEO_FRAME_CNT"=%lld", u32FrameCntAddr);
        set_bootargs_cfg(ARG_BOOT_VIDEO_FRAME_CNT, szText, 1); // don't need to save

        sprintf(szText, ARG_BOOT_VIDEO_SKIP_FRAME_CNT"=%lld", u32SkipFrameCntAddr);
        set_bootargs_cfg(ARG_BOOT_VIDEO_SKIP_FRAME_CNT, szText, 1); // don't need to save
    }
}

static unsigned int Load_Video2Dram(char *SrcPath,MS_PHYADDR Dram_Addr)
{
    char PathBuf[128] = "\0";
    MS_U32 u32VideoSize = 0;
    MS_U32 u32DramAddr = 0;
    MS_U32 u32TmpVideoSize = 0;
    UBOOT_TRACE("IN\n");
    u32DramAddr=PA2NVA(Dram_Addr);
    UBOOT_DEBUG("PA2NVA(Dram_Addr)=0x%x\n",(unsigned int)Dram_Addr);

    if(SrcPath==NULL)
    {
        UBOOT_ERROR("Error: Source Path is NULL!\n");
        return 0;
    }
    else // from nand/emmc directly
    {
        vfs_mount(ENV_BOOT_VIDEO_ADVERISEMENT);
        snprintf(PathBuf, sizeof(PathBuf), "%s",SrcPath);
        UBOOT_DEBUG("PathBuf=%s\n",PathBuf);
        u32VideoSize = vfs_getsize(PathBuf);

        u32TmpVideoSize = (u32VideoSize + BOOT_VIDEO_PACKET_SIZE)/BOOT_VIDEO_PACKET_SIZE;
        u32VideoSize = u32TmpVideoSize * BOOT_VIDEO_PACKET_SIZE;

        if (u32VideoSize > DMX_MAX_FILE_IN_SIZE)
        {
            u32VideoSize = DMX_MAX_FILE_IN_SIZE;
        }
        UBOOT_DEBUG("0x%lx(%d)\n",u32DramAddr,(unsigned int)u32VideoSize);
        vfs_read((void *)u32DramAddr,PathBuf,0,u32VideoSize);
        MsOS_FlushMemory();
    }
    UBOOT_TRACE("OK\n");
    return u32VideoSize;
}


static int _SYNC_SetStc64(MS_U64 u64Stc)
{
    MS_U32 u32StcH, u32Stc;

    UBOOT_DEBUG("u64Stc = 0x%llx\n", u64Stc);

    u64Stc = u64Stc*90;
    u64Stc &= 0x1FFFFFFFFLL;

    //u64Stc &= SYNC_STC_MASK;

    u32StcH = (MS_U32)((u64Stc >> 32) & 0xFFFFFFFF);
    u32Stc = (MS_U32)(u64Stc & 0xFFFFFFFF);

    if(MApi_DMX_Stc_Set(u32StcH, u32Stc) != DMX_FILTER_STATUS_OK)
    {
        UBOOT_ERROR("Error: Failed to call MApi_DMX_Stc_Set(...)!\n");
        return -1;
    }

    return 0;
}


static MS_U32 _SYNC_MinStcDiff(MS_U32 u32Stc1, MS_U32 u32Stc2)
{
// for example, we wish the diff of "stc1 = 0xFFFFFFFF" and "stc2 = 0" is "1" not "0xFFFFFFFF"
    MS_S64 s64Stc1 = (MS_S64)u32Stc1;
    MS_S64 s64Stc2 = (MS_S64)u32Stc2;
    MS_U32 u32Diff1;
    MS_U32 u32Diff2;

    if(u32Stc1 == u32Stc2)
        return 0;

    u32Diff1 = (MS_U32)((s64Stc2 + 0x100000000LL - s64Stc1) & 0xFFFFFFFF);
    u32Diff2 = (MS_U32)((s64Stc1 + 0x100000000LL - s64Stc2) & 0xFFFFFFFF);
    if(u32Diff1 <= u32Diff2)
        return u32Diff1;

    return u32Diff2;
}


int _AVSync_AudioMaster(MS_U32 *pu32StartStc)
{
    static MS_U32 u32Afpts = SYNC_INVALID_PTS;
    MS_U32 u32Vfpts = SYNC_INVALID_PTS;
    MS_U32 u32CurAPts = SYNC_INVALID_PTS;
    MS_U32 u32InitStc = SYNC_INVALID_PTS;
    MS_U32 u32Diff = 0;
    // get video current PTS

    MsDrv_VideoRoutinePolling();
    MsOS_DelayTask(10);

    if(MsDrv_VideoGetDecFrmCnt() > 0)
    {
        //
        // VIDEO_INIT_AUDIO_MASTER sync mode
        //
        u32InitStc = u32Vfpts = MsDrv_VideoGetCurPts();
        if (u32Vfpts == SYNC_INVALID_PTS)
        {
            // get audio current PTS for time
            u32CurAPts = MsDrv_AudioGetCurPts();
            if (u32CurAPts != SYNC_INVALID_PTS)
            {
                if (u32Afpts == SYNC_INVALID_PTS)
                {
                    u32Afpts = u32CurAPts; // audio first PTS
                }
    
                u32Diff = _SYNC_MinStcDiff(u32CurAPts, u32Afpts);
                if(u32Diff > _VPTS_TIMEOUT) // 500ms
                {
                    // The audio is already playing, but timeout to wait video first PTS.
                    UBOOT_INFO("[0]Timeout for waiting video first PTS.\n");
                    u32InitStc = u32CurAPts;
                }
            }
        }
    }

    if(u32InitStc != SYNC_INVALID_PTS)
    {
        // init STC
        _SYNC_SetStc64(u32InitStc);
        *pu32StartStc = u32InitStc;
        UBOOT_INFO("\033[31mBootVideo Audio master Sync down)\033[m\n");
        return 0;
    }

    return -1;
}
int _AVSync_PCR(void)
{
    int nTimeout = 0;
    MsDrv_DmxRoutinePolling();
    MsDrv_VideoRoutinePolling();
    MsOS_DelayTask(10);

    if(MsDrv_VideoGetDecFrmCnt() > 0)
    {
        nTimeout = 100;
        while(nTimeout--)
        {
            if(MsDrv_VideoIsSyncOn() == FALSE)
            {
                UBOOT_INFO("\033[31mBootVideo Sync Off()\033[m\n");
                return 0;
            }

            if(MsDrv_VideoIsReachSync(u32WaitSync))
            {
                UBOOT_INFO("\033[33mBootVideo Reach Sync(%d ms)\033[m\n", ((100-nTimeout) * 20));
                return 0;
            }

            MsOS_DelayTask(20);
        }


    }
    return -1;
}

int do_bootvideo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    UBOOT_INFO("do_bootvideo()\n");

    MS_BOOL bLive = FALSE;
    MS_U32 u32AutoScan = 0;
    DMX_FLOW_INPUT inSrc = DMX_FLOW_INPUT_DEMOD;
    MS_U16 u16VideoPid = ENV_BOOT_VIDEO_VPID_DEFAULT;
    MS_U16 u16AudioPid = ENV_BOOT_VIDEO_APID_DEFAULT;
    MS_U16 u16PcrPid = ENV_BOOT_VIDEO_PCR_DEFAULT;
    MS_U32 u32PlayTime = ENV_BOOT_VIDEO_PLAYTIME_DEFAULT;
    MS_U32 u32Volume = ENV_BOOT_VIDEO_VOLUME_DEFAULT;
    MS_U32 u32Repeat = 0;
    MS_PHYADDR FileinAddr = 0;
    MS_U32 u32FileInSize = 0;
    char szText[32];
    int iRet = 0;
    MsDrv_VIDEO_StreamType_e eVdecStreamType = E_MSDRV_VIDEO_STREAM_LIVE;
    MsDrv_VIDEO_CodecType_e eVdecCodecType = ENV_BOOT_VIDEO_VCODEC_DEFAULT;

    MsDrv_AUDIO_StreamType_e eAdecStreamType = E_MSDRV_AUDIO_STREAM_LIVE;
    MsDrv_AUDIO_CodecType_e eAdecCodecType = ENV_BOOT_VIDEO_ACODEC_DEFAULT;

    if(argc!=2)
    {
        cmd_usage(cmdtp);
        return -1;
    }

    // Get arg parameters
    bLive = (MS_BOOL)simple_strtoul(argv[1], NULL, 10);
    if(bLive == FALSE)
    {
        inSrc           = DMX_FLOW_INPUT_MEM;
        eVdecStreamType = E_MSDRV_VIDEO_STREAM_TSFILE;
        eAdecStreamType = E_MSDRV_AUDIO_STREAM_TSFILE;
    }
    u32WaitSync = GET_BOOT_VIDEO_ENV_VALUE(ENV_BOOT_VIDEO_WAIT_SYNC);
    u32AutoScan = GET_BOOT_VIDEO_ENV_VALUE(ENV_BOOT_VIDEO_AUTOSCAN);

    // MIU init
    MsDrv_MiuInit();

    // AUTH check
    // _MApi_Set_SecurityAuthentication();

    // DMX init
    if(MsDrv_DmxInit(inSrc) != 0)
    {
        UBOOT_ERROR("failed to MsDrv_DmxInit()\n");
        return -1;
    }

    if (DMX_FLOW_INPUT_MEM == inSrc)
    {
        char PathBuf[128] = "\0";
        if (get_addr_from_mmap("E_MMAP_ID_VIDEO_FILE_IN", (MS_PHYADDR *) &FileinAddr) !=0)
        {
            UBOOT_ERROR("get E_MMAP_ID_VIDEO_FILE_IN_ADR mmap fail\n");
            return -1;
        }
        snprintf(PathBuf, sizeof(PathBuf), "%s/%s",CUSTOMER_PATH, ARG_BOOT_VIDEO_FILE_NAME);
        u32FileInSize = Load_Video2Dram(PathBuf, FileinAddr);
        u32AutoScan = 0;
    }

    if(u32AutoScan)
    {
        MsDrv_DmxProg_t stDmxProg;
        memset(&stDmxProg, 0, sizeof(MsDrv_DmxProg_t));
        if(MsDrv_DmxScan(bLive, &stDmxProg) != 0)
        {
            UBOOT_ERROR("failed to MsDrv_DmxScan(bLive = %d)\n", bLive);
            return -1;
        }

        u16VideoPid = stDmxProg.u16VideoPid;
        eVdecCodecType = stDmxProg.eVdecCodecType;
        u16AudioPid = stDmxProg.u16AudioPid;
        eAdecCodecType = stDmxProg.eAdecCodecType;
        u16PcrPid = stDmxProg.u16PcrPid;
    }
    else
    {
        // Get env parameters
        u16VideoPid    = (MS_U16)GET_BOOT_VIDEO_ENV_VALUE(ENV_BOOT_VIDEO_VPID);
        eVdecCodecType = GET_BOOT_VIDEO_ENV_VALUE(ENV_BOOT_VIDEO_VCODEC);
        u16AudioPid    = (MS_U16)GET_BOOT_VIDEO_ENV_VALUE(ENV_BOOT_VIDEO_APID);
        eAdecCodecType = GET_BOOT_VIDEO_ENV_VALUE(ENV_BOOT_VIDEO_ACODEC);
        u16PcrPid      = GET_BOOT_VIDEO_ENV_VALUE(ENV_BOOT_VIDEO_PCR);
        u32PlayTime    = GET_BOOT_VIDEO_ENV_VALUE(ENV_BOOT_VIDEO_PLAYTIME);
        if (u32PlayTime < 5000) // 5 sec
        {
            UBOOT_ERROR("PlayTime is less than 5000ms (u32PlayTime = %lu ms)\n", u32PlayTime);
            return -1;
        }
    }
    u32Volume = GET_BOOT_VIDEO_ENV_VALUE(ENV_BOOT_VIDEO_VOLUME);

    UBOOT_INFO("Live? %d AutoScan? %ld V(pid:%d codec:%d), A(pid:%d, codec:%d), PCR:%d, Volume = %ld\n", bLive, u32AutoScan, u16VideoPid, eVdecCodecType, u16AudioPid, eAdecCodecType, u16PcrPid, u32Volume);

    // Video init and start
    if(MsDrv_VideoStart(eVdecStreamType, eVdecCodecType) != 0)
    {
        UBOOT_ERROR("failed to MsDrv_VideoStart()\n");
        return -1;
    }

    // Audio init and start
    if(MsDrv_AudioStart(eAdecStreamType, eAdecCodecType) != 0)
    {
        UBOOT_ERROR("failed to MsDrv_AudioStart()\n");
        goto BOOT_VIDEO_FAILED;
    }
    // Open and start video dmx filter
    if(MsDrv_DmxOpenVideoFilter(u16VideoPid, &_u8VideoDmxId) != 0)
    {
        UBOOT_ERROR("failed to MsDrv_DmxOpenVideoFilter(pid = 0x%x)\n", u16VideoPid);
        goto BOOT_VIDEO_FAILED;
    }

    // Open and start audio dmx filter
    if(MsDrv_DmxOpenAudioFilter(u16AudioPid, &_u8AudioDmxId) != 0)
    {
        UBOOT_ERROR("failed to MsDrv_DmxOpenAudioFilter(pid = 0x%x)\n", u16AudioPid);
        goto BOOT_VIDEO_FAILED;
    }

    // Open and start pcr dmx filter
    if (1 == bLive)
    {
        if (MsDrv_DmxOpenPcrFilter(u16PcrPid, &_u8PcrDmxId) != 0)
        {
            UBOOT_ERROR("failed to MsDrv_DmxOpenPcrFilter(pid = 0x%x)\n", u16AudioPid);
            goto BOOT_VIDEO_FAILED;
        }
        /*if(do_tuner_checklock() != TRUE)
        {
            UBOOT_ERROR("failed to checklock\n");
            goto BOOT_VIDEO_FAILED;
        }*/
        MsDrv_DmxEnableAvFifo();
    }
    else
    {
        MsDrv_DmxEnableAvFifo();
        u32Repeat = (MS_U16)GET_BOOT_VIDEO_ENV_VALUE(ENV_BOOT_VIDEO_FILE_REPEAT);
        if (MsDrv_DmxFileInInit(FileinAddr, u32FileInSize, u32Repeat) != 0)
        {
            UBOOT_ERROR("failed to MsDrv_DmxFileInInit() \n");
            goto BOOT_VIDEO_FAILED;
        }
    }

    // debuggint: check decode status
    int nTimeout = 500;
    int nPollingTimes = 0;
    MS_U32 u32StartStc = 0;

    while(/*1 || */(nTimeout--))
    {
        if (1 == bLive)
        {
            if (0 == _AVSync_PCR())
            {
                break;
            }
        }
        else
        {
            if (0 == _AVSync_AudioMaster(&u32StartStc))
            {
                break;
            }
        }

        if(nTimeout <= 0)
        {
            UBOOT_ERROR("\033[31mBootVideo timeout to reach sync\033[m\n");
        }

        MsDrv_VideoShowDecInfo();
        MsDrv_AudioShowDecInfo();

    }

    if(MsDrv_VideoGetDecFrmCnt() == 0)
    {
        UBOOT_ERROR("\033[31m\nVideo Decode count=0. Change to Normal bootlogo display\033[m\n");
        goto BOOT_VIDEO_FAILED;
    }
    else
    {
        UBOOT_INFO("\033[31m\nVideo Decode count > 0. Normal BootVideo display\033[m\n");
    }

    MsDrv_VideoResume();
    MsDrv_VideoUnMute();
    // hide bootlogo only.
    MsDrvGOP_Hide();
    //MsOS_DelayTask(10);

    // set command to let APTS sync STC
    MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_syncSTC_in_mmTs, 1, 0);
    while(MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_syncSTC_in_mmTs) == 0)
    {
        MsDrv_VideoShowDecInfo();
    }
    MApi_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_syncSTC_in_mmTs, 0, 0);
    //


    MsDrv_AudioUnMute(u32Volume);
    _SaveBootVideoBootArg(eVdecCodecType);
    while(nPollingTimes < 100)
    {
        nPollingTimes++;
        MsDrv_VideoShowDecInfo();
        MsDrv_AudioShowDecInfo();
    }

    memset(szText, 0x00, sizeof(szText));
    snprintf(szText, ENV_BOOT_VIDEO_FILE_ADDR, "%s=0x%llx",ENV_BOOT_VIDEO_FILE_ADDR, FileinAddr);
    iRet = set_bootargs_cfg(ENV_BOOT_VIDEO_FILE_ADDR, szText, 1); // don't need to save
    //printf("[%s %d] %s (%d)\n",__FUNCTION__,__LINE__,szText,iRet);

    memset(szText, 0x00, sizeof(szText));
    snprintf(szText, MAX_BOOT_VIDEO_TEXT_LENGTH, "%s=0x%lx",ENV_BOOT_VIDEO_FILE_SIZE,u32FileInSize);
    iRet = set_bootargs_cfg(ENV_BOOT_VIDEO_FILE_SIZE, szText, 1); // don't need to save
    //printf("[%s %d] %s (%d)\n",__FUNCTION__,__LINE__,szText,iRet);

    memset(szText, 0x00, sizeof(szText));
    snprintf(szText, MAX_BOOT_VIDEO_TEXT_LENGTH, "%s=0x%lx",ENV_BOOT_VIDEO_FILE_START_TIME, u32StartStc);
    iRet = set_bootargs_cfg(ENV_BOOT_VIDEO_FILE_START_TIME, szText, 1); // don't need to save
    //printf("[%s %d] %s (%d)\n",__FUNCTION__,__LINE__,szText,iRet);


    return 0;

BOOT_VIDEO_FAILED:

    _StopBootVideo();

    return -1;
}
#endif  // #if (CONFIG_BOOTVIDEO == 1)


