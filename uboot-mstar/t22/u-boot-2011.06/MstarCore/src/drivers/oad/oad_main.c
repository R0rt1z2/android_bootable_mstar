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
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <config.h>
#include <MsTypes.h>
#include <MsIRQ.h>
#include <MsOS.h>
#include <apiGOP.h>
#include <apiGFX.h>
#include <drvSEM.h>
#include <drvGPIO.h>
#include <apiDMX.h>
#include "msAPI_demux.h"
#include "msAPI_oad_process.h"
#include "msAPI_oad_parser.h"
#include "msAPI_Memory.h"
#include "msAPI_Tuner.h"
#include <linux/string.h>
#include <asm/string.h>
#include "uboot_mmap.h"
#include "mstar_string_emnu.h"

#if (CONFIG_OAD_IN_MBOOT)
#define ENABLE_OAD 1
#define ENABLE_BL_OAD_SCAN 1
#if (CONFIG_DISPLAY_OSD)
#define ENABLE_BL_UI 1
#define GWIN_WIDTH          720
#define GWIN_HEIGHT         576
#define GRAPHIC_WIDTH       600
#define GRAPHIC_HEIGHT      400
#define GRAPHIC_X           60
#define GRAPHIC_Y           88
#define LINE_HEIGHT         50
#define RECT_LEFT_INTERVAL  50
#else
#define ENABLE_BL_UI 0
#endif
#define DBG_PRINT(x)    //x
#define TUNER_LOCK_TIME_OUT     5000//ms
#define MONITOR_DSI_TIME_OUT    60000//ms
#define MONITOR_DII_TIME_OUT    60000//ms
#define MONITOR_DDB_TIME_OUT    60000*1//ms
#define FE_NOT_LOCK     0
#define FE_LOCK         1
#define FE_AGC_NOT_LOCK 2
#define BUF_SIZE 4096

// OAD info
#define CUSTOMER_OUI            0x0014B9    // MStar OUI
#define DVB_OUI                 0x00015A
#define HW_MODEL                0x01        // HW Model
#define HW_VERSION              0x01        // HW Version
#define BL_SW_MODEL             0x01        // SW Model of Bootloader
#define BL_SW_VERSION           0x01        // SW Version of of Bootloader
#define AP_SW_MODEL             0x0001        // SW Model of Application
#define AP_SW_VERSION           0x0001        // SW Version of of Application

#define VERSION_CHECK 1
#define DATA_BC_ID_SSU          0x000A // DVB-SSU
#define DATA_BC_ID_UK_EC        0x0111 // UK Engineering Channel
#define MAX_DDB_MSGSIZE 4066
#define HW_MODEL_VER_CHECK      1

//buffer address
#define DOWNLOAD_BUFFER_LEN_OAD     0x0003200000
#define DOWNLOAD_BUFFER_ADR_OAD     UPGRADE_BUFFER_ADDR
#define OADSEC_BUFFER_LEN           0x0000014000

#define DMX_BUF_SIZE_MONITOR    0x4000
#define DMX_BUF_SIZE_DOWNLOAD   0x4000*4

#define DVBT_FIRST_FREQ 474
#define DVBT_LAST_FREQ  866
#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif
#ifdef HB_CUS
typedef enum _EN_COUNTRY_TYPE
{
    COUNTRY_TYPE_EUROPE,
    COUNTRY_TYPE_GERMANY,
    COUNTRY_TYPE_TAIWAN,
    COUNTRY_TYPE_ITALY,
    COUNTRY_TYPE_FRANCE,
    COUNTRY_TYPE_CHINA,
    COUNTRY_TYPE_AUSTRALIA,
    COUNTRY_TYPE_BRAZIL,
    COUNTRY_TYPE_CHILE,
    COUNTRY_TYPE_HONGKONG,
    COUNTRY_TYPE_ARGENTINA,
    COUNTRY_TYPE_PERU,
    COUNTRY_TYPE_UK,
    COUNTRY_TYPE_OTHER,
    COUNTRY_TYPE_AUSTRALIA_NEW,
    COUNTRY_TYPE_NUM,

} EN_COUNTRY_TYPE;

typedef struct _chaninfo
{
    MS_U32   m_frequency;
    MS_U16   m_chanNum;
    MS_U16   m_bandwith;
    MS_U8    m_countryType;
    MS_U16   m_u16OADPid;
}CHANINFO_T;

#define DEFAULT_OAD_PID 0
#endif
static MS_U16 wOad_PID = 0;
static MS_U32 u32Frequency = 0;
static MS_U8  u8BandWidth = 0;
static MS_U16 wOad_PLP = 0xffff;
MS_U32  u32QAM = 0;
MS_U16  u16SymbolRate = 0;
static MS_U8  u8IsSDTT_OAD = 0;
static MS_U32 u32GroupId = 0;
static MS_U32 tsVersion=0;
static MS_U32 MS_U32DOWNLOADSIZE=0;
static MS_U8   _u8OADMonitorFID=MSAPI_DMX_INVALID_FLT_ID;
static MS_U8   _u8OADDownloadFID=MSAPI_DMX_INVALID_FLT_ID;
MS_U8  au8MonitorSection[4096];
MS_U8  au8DownloadSection[4096];
static MS_U32 u32DownloadSize = 0x0;
static MS_U8  versionNum = 0xff ;
static MS_U32 u32ModuleAddr=0;
MS_U32 u32OadbufMonitorAddr = 0;
MS_U32 u32OadbufMonitorSize = 0;
MS_U32 u32OadbufDownloadAddr = 0;
MS_U32 u32OadbufDownloadSize = 0;
#if ENABLE_STB_ECOS_BOOT
MS_U32 u32OADBuffer = 0;
#endif

extern MS_BOOL msAPI_Tuner_CheckLock(MS_BOOL *CheckLockResult,MS_BOOL fScan);

#if (ENABLE_BL_UI)
void MApp_BL_DisplaySystem(MS_U8  u8Percent);
void MApp_BL_DisplaySystem_clear(void);
void MApp_BL_DisplaySystem_setStatus(MS_U32 status);

void MApp_BL_DisplaySystem(MS_U8  u8Percent)
{
    char* buffer=NULL;
    int ret=0;

    buffer=(char *)malloc(BUF_SIZE);
    if(buffer != NULL)
    {
        memset(buffer, 0 , 256);
        sprintf(buffer, "draw_rect %d %d %d %d 000000ff", GRAPHIC_X, GRAPHIC_Y+300, GRAPHIC_WIDTH, GRAPHIC_HEIGHT-300);
        ret=run_command(buffer, 0);

        memset(buffer, 0 , 256);
        sprintf(buffer, "draw_progress %d %d ffffffff %u", GRAPHIC_X+60, GRAPHIC_Y+300, u8Percent);
        ret=run_command(buffer, 0);
        sprintf(buffer, "osd_flush");
        run_command(buffer, 0);
        free(buffer);
    }
    else
    {
        printf("%s: malloc() failed, at %d\n", __func__, __LINE__);
    }
}

void MApp_BL_DisplaySystem_clear(void)
{
    char* buffer=NULL;
    int ret=0;

    buffer=(char *)malloc(BUF_SIZE);
    if(buffer != NULL)
    {
        memset(buffer, 0 , 256);
        sprintf(buffer, "draw_rect %d %d %d %d 000000ff", GRAPHIC_X, GRAPHIC_Y, GRAPHIC_WIDTH, GRAPHIC_HEIGHT);
        ret=run_command(buffer, 0);

        memset(buffer, 0 , 256);
        sprintf(buffer, "draw_string %d %d ffffffff 1 %x", GRAPHIC_X, GRAPHIC_Y+100, en_not_turn_off);
        ret=run_command(buffer, 0);
        sprintf(buffer, "osd_flush");
        run_command(buffer, 0);
        free(buffer);
    }
    else
    {
        printf("%s: malloc() failed, at %d\n", __func__, __LINE__);
    }
}

void MApp_BL_DisplaySystem_setStatus(MS_U32 status)
{
    char* buffer=NULL;
    int ret=0;

    buffer=(char *)malloc(BUF_SIZE);
    if(buffer != NULL)
    {
        memset(buffer, 0 , 256);
        sprintf(buffer, "draw_string %d %d ffffffff 1 %x", GRAPHIC_X, GRAPHIC_Y+200, status);
        ret=run_command(buffer, 0);
        sprintf(buffer, "osd_flush");
        run_command(buffer, 0);
        free(buffer);
    }
    else
    {
        printf("%s: malloc() failed, at %d\n", __func__, __LINE__);
    }
}
#else //(ENABLE_BL_UI)
void MApp_BL_DisplaySystem(MS_U8  u8Percent){UNUSED(u8Percent);}
void MApp_BL_DisplaySystem_clear(void){}
void MApp_BL_DisplaySystem_setStatus(MS_U32 status){UNUSED(status);}
#endif //(ENABLE_BL_UI)

int oad_check(void);
int oad_check()
{
    MS_U8  *pu8Buf = NULL;
    OAD_STATUS_REPORT_TYPE ddbStatus ;
    MS_U16 blockID=0,msgLen=0,msgType=0 ;
    MS_U32 msgAddr = 0 ;
    MS_BOOL Check_FE_Result;
    MS_U32 u32DsmccTimer=0;
    MS_U32 u32BlockSize=0;
    MS_U32 u32DownloadOadBuffADR=0;
    DBG_PRINT( printf("%s: at %d\n", __func__, __LINE__) );
#if ENABLE_STB_ECOS_BOOT
    u32DownloadOadBuffADR = DOWNLOAD_BUFFER_ADR_OAD;
#else
    if(get_map_addr_from_env(E_OAD_DOWNLOAD, DOWNLOAD_BUFFER_ADR_OAD,&u32DownloadOadBuffADR)!=0)
    {
        printf("get %s mmap fail\n",E_MMAP_ID_DMX_SECBUF_ADR);
        return -1;
    }
#endif
    if(wOad_PID == 0)
    {
        return 1;
    }
    u32DsmccTimer = MsOS_GetSystemTime();
    while(1)
    {
        if( msAPI_Tuner_CheckLock( &Check_FE_Result,FALSE) == FALSE )
        {
            printf("FE Fail!\n");
        }
        else
        {
            if(Check_FE_Result==FE_LOCK)
            {
                printf("FE Locked.\n");
                break;
            }
            else
            {
                printf("FE Not Locked. Check_FE_Result = %d\n", Check_FE_Result);
            }
        }
        if (MsOS_Timer_DiffTimeFromNow(u32DsmccTimer) > TUNER_LOCK_TIME_OUT)
        {
            OAD_DEBUG( printf("Error> Exit DSI\n") );
            return 1; //return to do USB download
        }
		DBG_PRINT( printf("%s: at %d\n", __func__, __LINE__) );
        MsOS_DelayTask(500);
    }

    if((!u8IsSDTT_OAD) && (!u32GroupId))
    {
        if(msAPI_OAD_CreateSectionFilter( MSAPI_DMX_FILTER_TYPE_OAD_MONITOR, OAD_CONTROL_SECTION_TYPE, OAD_DSMCC_MSGID_DSI,
                wOad_PID, &_u8OADMonitorFID ) == TRUE)
        {
            DBG_PRINT( printf("Create filter: OK, pid: 0x%x.\n", wOad_PID) );
        }

        DBG_PRINT( printf("Line: %d\n", __LINE__) );

        u32DsmccTimer = MsOS_GetSystemTime();
        while(1)
        {
            pu8Buf = msAPI_OAD_PollingSectionFilter(_u8OADMonitorFID, au8MonitorSection);

            if(pu8Buf)
            {
                DBG_PRINT( printf("DSI Poll filter: OK.\n") );

                if (OAD_SUCCEED==msAPI_OAD_ProcessDSI(pu8Buf))
                {
                    u32DsmccTimer = MsOS_GetSystemTime();
                    versionNum = msAPI_OAD_GetDsiVersionNum() ;

                    if (versionNum!=msAPI_OAD_GetDsiVersionNum())
                    {
                        printf("versionNum error !!!\n");
                    }
                    msAPI_OAD_DeleteSectionFilter(&_u8OADMonitorFID);
                    break;
                }
                else
                {
                    printf("process DSI error !!!\n");
                }
            }
            else
            {
                printf("No DSI Data!\n");
            }

            if (MsOS_Timer_DiffTimeFromNow(u32DsmccTimer) > MONITOR_DSI_TIME_OUT)
            {
                OAD_DEBUG( printf("Error> Exit DSI\n") );
                return 1;
            }
            MsOS_DelayTask(100);
        }
    }
    else
    {
        printf("===== [%s][%d] =====\n",__FUNCTION__,__LINE__);
        DBG_PRINT( printf("%s: at %d\n", __func__, __LINE__) );
        msAPI_OAD_SetGroupId(u32GroupId);
        msAPI_OAD_SetOADMode(OAD_MODE_DATACAR);
    }

    if(msAPI_OAD_CreateSectionFilter( MSAPI_DMX_FILTER_TYPE_OAD_DOWNLOAD,OAD_CONTROL_SECTION_TYPE,OAD_DSMCC_MSGID_DII,wOad_PID, &_u8OADDownloadFID) == TRUE)
    {
        DBG_PRINT( printf("\n") );
        DBG_PRINT( printf("DII Create filter: OK.\n") );
        DBG_PRINT( printf("DII _u8OADDownloadFID = %d\n", _u8OADDownloadFID) );
        u32DsmccTimer = MsOS_GetSystemTime();
    }
    else
    {
        printf("Error> msAPI_OAD_CreateSectionFilter(OAD_CONTROL_SECTION_TYPE) : Invalid FID=%x\n",_u8OADDownloadFID);
        return 1;
    }

    while(1)
    {
        pu8Buf = msAPI_OAD_PollingSectionFilter(_u8OADDownloadFID, au8DownloadSection);

        if(pu8Buf)
        {
            if(OAD_SUCCEED==msAPI_OAD_ProcessDII(pu8Buf))
            {
                msAPI_OAD_DeleteSectionFilter(&_u8OADDownloadFID) ;
                DBG_PRINT( printf("process DII ok\n") );
                break;
            }

            if (MsOS_Timer_DiffTimeFromNow(u32DsmccTimer) > MONITOR_DII_TIME_OUT)
            {
                printf("Error> Timeout to receive DII\n");
                return 1;
            }
        }
    }

    if(msAPI_OAD_CreateSectionFilter( MSAPI_DMX_FILTER_TYPE_OAD_DOWNLOAD,OAD_DATA_SECTION_TYPE ,OAD_DSMCC_MSGID_DDB,
                wOad_PID,&_u8OADDownloadFID) == TRUE)
    {
        DBG_PRINT( printf("\n") );
        DBG_PRINT( printf("DDB Create filter: OK.\n") );
        u32DsmccTimer = MsOS_GetSystemTime();
    }
    else
    {
        printf("Error> msAPI_OAD_CreateSectionFilter(OAD_DATA_SECTION_TYPE) : Invalid FID\n");
    }
    MApp_BL_DisplaySystem_clear(); //clear screen
    MApp_BL_DisplaySystem_setStatus(en_status_download);
    MApp_BL_DisplaySystem(0);
    u32BlockSize = msAPI_OAD_GetBlockSize();

    while(1)
    {
//        msAPI_Timer_ResetWDT();
        pu8Buf = msAPI_OAD_PollingSectionFilter(_u8OADDownloadFID, au8DownloadSection);

        if(pu8Buf)
        {
            ddbStatus = msAPI_OAD_ProcessDDB(pu8Buf, &blockID, &msgLen, &msgAddr, &msgType) ;

            if (ddbStatus != OAD_FAIL)
            {
                u32DsmccTimer = MsOS_GetSystemTime();
                if(msgLen>0)
                {
                    if (msgType==OAD_MODE_OBJCAR)
                    {
                        if (blockID==0)
                        {
                        #if defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KENYA)
                            memcpy((MS_U8 *)(u32DownloadOadBuffADR),(MS_U8 *)msgAddr,(MS_U32)msgLen) ;
                        #else
                            memcpy((MS_U8 *)MS_PA2KSEG1(MS_VA2PA(u32DownloadOadBuffADR)),(MS_U8 *)msgAddr,(MS_U32)msgLen) ;
                        #endif

                        }
                        else
                        {
                        #if defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KENYA)
                            memcpy((MS_U8 *)(u32DownloadOadBuffADR+(blockID*MAX_DDB_MSGSIZE)-BIOP_HEADER_LEN),(MS_U8 *)msgAddr,(MS_U32)msgLen) ;
                        #else
                            memcpy((MS_U8 *)MS_PA2KSEG1(MS_VA2PA(u32DownloadOadBuffADR+(blockID*MAX_DDB_MSGSIZE)-BIOP_HEADER_LEN)),(MS_U8 *)msgAddr,(MS_U32)msgLen) ;
                        #endif
                        }
                    }
                    else if (msgType==OAD_MODE_DATACAR)
                    {
                    #if defined(CONFIG_URANUS4) || defined(CONFIG_KRONUS) || defined(CONFIG_KAISERIN) || defined(CONFIG_KENYA)
					    DBG_PRINT( printf("%s: at %d\n", __func__, __LINE__) );
                        memcpy((MS_U8 *)(u32DownloadOadBuffADR+(blockID*u32BlockSize)),(MS_U8 *)msgAddr,(MS_U32)msgLen) ;
                    #else
                        memcpy((MS_U8 *)MS_PA2KSEG1(MS_VA2PA(u32DownloadOadBuffADR+(blockID*u32BlockSize))),(MS_U8 *)msgAddr,(MS_U32)msgLen) ;
                    #endif

                    }
                    //u32DownloadSize += msgLen ;
                }
            }
            if (ddbStatus == OAD_COMPLETE)
            {
                //MS_U32 CRC_result=0;
                //MS_U32 CRC_value=0;
                DBG_PRINT( printf("DDB complete.\n") );
                //Check CRC32
                //CRC_value=*((MS_U32*)(DOWNLOAD_BUFFER_ADR_OAD+u32DownloadSize-4));
                //CRC_result=crc32( 0x00, (MS_U8 *)DOWNLOAD_BUFFER_ADR_OAD, u32DownloadSize-4 );
                //DBG_PRINT( printf("CRC_result 0x%x, CRC_value 0x%x\n", CRC_result, CRC_value) );
                msAPI_OAD_DeleteSectionFilter(&_u8OADDownloadFID) ;
                break;
            }
            static MS_U8  u8PrevPersentage=0;
            MS_U8  u8Percentage = msAPI_OAD_GetProgress();
            u8Percentage = u8Percentage > 100 ? 100 : u8Percentage;
            if(u8Percentage > (u8PrevPersentage + 1))
            {
                u8PrevPersentage = u8Percentage;
                MApp_BL_DisplaySystem(u8Percentage);
                DBG_PRINT( printf("u8Percentage: %u, at %d\n", u8Percentage, __LINE__) );
            }
        }
        if (MsOS_Timer_DiffTimeFromNow(u32DsmccTimer) > MONITOR_DDB_TIME_OUT)
        {
            msAPI_OAD_ClearData();
            OAD_DEBUG( printf("Error> Exit DDB\n") );
            printf("DDB timeout!\n");
            return 1;
        }
    }

    MApp_BL_DisplaySystem(100);
    DBG_PRINT( printf("\nOAD Finished\n") );

    msAPI_OAD_GetModuleInfo( &u32ModuleAddr, &u32DownloadSize );
    printf("u32ModuleAddr 0x%x, u32DownloadSize 0x%x\n",(unsigned int)u32ModuleAddr,(unsigned int) u32DownloadSize);
    printf("u32DownloadOadBuffADR 0x%X, DOWNLOAD_BUFFER_LEN_OAD 0x%X\n",(unsigned int) u32DownloadOadBuffADR,(unsigned int) DOWNLOAD_BUFFER_LEN_OAD);
    MS_U32DOWNLOADSIZE = u32DownloadSize;
#ifdef HB_CUS
#else
    MApp_BL_DisplaySystem_clear();
    MApp_BL_DisplaySystem_setStatus(en_status_OAD_Update);
    MApp_BL_DisplaySystem(0);
#endif
    return 0;
}

int chakra2_main_loop(void)
{
    DBG_PRINT( printf("%s: at %d\n", __func__, __LINE__) );
    if(!oad_check())
    {
        return 0;
    }
    return 1;
}

MS_BOOL oad_VersonCheck(MS_U16 type,MS_U8  *pPrivateData)
{
    MS_U16  compatibilityLength,descriptorCount,model,version=0x0,i,j;
    MS_U8   descriptorType, descriptorLength=0, specifierType,subDescriptorCount=0,subDescriptorType=0, subDescriptorLength=0;
    MS_U32 specifierData;
    MS_BOOL swPass=FALSE;
#if HW_MODEL_VER_CHECK
    MS_BOOL hwPass=FALSE;
#endif
    // get OAD Info from env or not
    char *pTemp = NULL;
    MS_U32 u32CUSTOMER_OUI = 0, u32DVB_OUI = 0;
    MS_U16 u16HW_MODEL = 0, u16HW_VERSION = 0, u16AP_SW_MODEL = 0;

    pTemp = getenv("CUSTOMER_OUI");
    u32CUSTOMER_OUI = (pTemp) ? simple_strtoul(pTemp, NULL, 16) : CUSTOMER_OUI;
    pTemp = getenv("DVB_OUI");
    u32DVB_OUI = (pTemp) ? simple_strtoul(pTemp, NULL, 16) : DVB_OUI;
    pTemp = getenv("HW_MODEL");
    u16HW_MODEL = (pTemp) ? simple_strtoul(pTemp, NULL, 16) : HW_MODEL;
    pTemp = getenv("HW_VERSION");
    u16HW_VERSION = (pTemp) ? simple_strtoul(pTemp, NULL, 16) : HW_VERSION;
    pTemp = getenv("AP_SW_MODEL");
    u16AP_SW_MODEL = (pTemp) ? simple_strtoul(pTemp, NULL, 16) : AP_SW_MODEL;

    OAD_DEBUG( printf("CUSTOMER_OUI = 0x%x\nDVB_OUI = 0x%x\nHW_MODEL = 0x%x\nHW_VERSION = 0x%x\nAP_SW_MODEL = 0x%x\n", u32CUSTOMER_OUI, u32DVB_OUI, u16HW_MODEL, u16HW_VERSION, u16AP_SW_MODEL) );

    if (!VERSION_CHECK) return TRUE ;

    if (type==DATA_BC_ID_UK_EC) // MIS
    {
        MS_U16 model_hw, version_hw;
        model_hw = GET_2BYTE(&pPrivateData[0]);
        version_hw = GET_2BYTE(&pPrivateData[2]);
        model = GET_2BYTE(&pPrivateData[4]);
        version = GET_2BYTE(&pPrivateData[6]);
#if HW_MODEL_VER_CHECK
        if ((model_hw == u16HW_MODEL)&&(version_hw == u16HW_VERSION))
#endif //HW_MODEL_VER_CHECK
        {
            if (model == u16AP_SW_MODEL)
            {
                tsVersion = (MS_U32)version;
                return TRUE ;
            }
        }
        return FALSE ;
    }
    else if (type==DATA_BC_ID_SSU) // GroupCompatibility()
    {
        compatibilityLength = GET_2BYTE(pPrivateData);
        pPrivateData += 2;

        if ( compatibilityLength < 2 )
        {
            OAD_ASSERT( printf("Error> msAPI_OAD_ProcessDSI : compatibilityLength = 0x%x\n", compatibilityLength) );
            return FALSE ;
        }

        descriptorCount = GET_2BYTE(pPrivateData); pPrivateData += 2;
        OAD_DEBUG( printf("descriptorCount = %u\n", descriptorCount) );

        for ( i = 0; i < descriptorCount; i ++ )
        {
            descriptorType  = *pPrivateData++;
            descriptorLength= *pPrivateData++;

            switch ( descriptorType )
            {
                case 0x02:
                {
                    specifierType = *pPrivateData++;
                    specifierData = GET_3BYTE( pPrivateData ); pPrivateData += 3;
                    model = GET_2BYTE(pPrivateData); pPrivateData += 2;
                    version = GET_2BYTE(pPrivateData); pPrivateData += 2;
                    subDescriptorCount = *pPrivateData++;
                    tsVersion = (MS_U32)version;
                    OAD_DEBUG( printf("TS=%x\n",tsVersion));
                    if ((specifierType==0x01)&&(model == u16AP_SW_MODEL)&&(specifierData==u32CUSTOMER_OUI)) //Do NOT check version
                    {
                        swPass = TRUE;
                    }
                    else
                    {
                        return FALSE ;
                    }
                    //pPrivateData += (descriptorLength-9) ;
                    //OAD_DEBUG( printf("[OTA] specifierType = 0x%02bx, specifierData = 0x%08lx\n", specifierType, specifierData) );
                    break;
                }
                case 0x01:
#if HW_MODEL_VER_CHECK
                    specifierType = *pPrivateData++;
                    specifierData = GET_3BYTE( pPrivateData ); pPrivateData += 3;
                    model = GET_2BYTE(pPrivateData); pPrivateData += 2;
                    version = GET_2BYTE(pPrivateData); pPrivateData += 2;
                    subDescriptorCount = *pPrivateData++;
                    if(1)
                    {
                        tsVersion = (MS_U32)version;
                    }
                    OAD_DEBUG( printf("TS=%x\n",tsVersion));
                    if ((specifierType==0x01)&&(model == u16HW_MODEL)&&(version == u16HW_VERSION)&&(specifierData==u32CUSTOMER_OUI))
                    {
                        hwPass = TRUE;
                    }
                    else if ((subDescriptorCount > 0)&&(specifierType==0x01)&&(model == 0xFFFF)&&(version == 0xFFFF)&&(specifierData==u32DVB_OUI))
                    {
                        for(j = 0; j < subDescriptorCount; j++)
                        {
                            subDescriptorType = *pPrivateData;
                            pPrivateData++;
                            subDescriptorLength = *pPrivateData;
                            pPrivateData++;

                            if(subDescriptorType == 0x01)
                            {
                                specifierType = *pPrivateData;
                                pPrivateData++;
                                specifierData = GET_3BYTE(pPrivateData);
                                pPrivateData += 3;
                                model = GET_2BYTE(pPrivateData);
                                pPrivateData += 2;
                                version = GET_2BYTE(pPrivateData);
                                pPrivateData += 2;
                                pPrivateData++;
                                if((specifierType == 0x01) && (model == u16HW_MODEL) && (version == u16HW_VERSION) && (specifierData == u16HW_VERSION))
                                {
                                    hwPass = TRUE;
                                }
                                pPrivateData += (subDescriptorLength-9) ;
                            }
                            else
                            {
                                pPrivateData += subDescriptorLength;
                            }
                        }
                    }
                    else
                    {
                        return FALSE ;
                    }
                    //pPrivateData += (descriptorLength-9) ;
                    //OAD_DEBUG( printf("[OTA] specifierType = 0x%02bx, specifierData = 0x%08lx\n", specifierType, specifierData) );
                    break;
#endif
                default:
                {
                    pPrivateData += descriptorLength;
                    OAD_DEBUG( printf("[OTA] descriptorType = 0x%02bx\n", descriptorType) );
                    break;
                }
            }
       }// End of for
    }
#if HW_MODEL_VER_CHECK
    if((swPass == TRUE) && (hwPass == TRUE))
#else //HW_MODEL_VER_CHECK
    if(swPass == TRUE)
#endif //HW_MODEL_VER_CHECK
    {
        return TRUE ;
    }
    return FALSE ;
}
MS_U8 dmx_init = 0;

int oad_prepare(void)
{
    DBG_PRINT( printf("%s: at %d\n", __func__, __LINE__) );
    _u8OADMonitorFID = MSAPI_DMX_INVALID_FLT_ID;
    _u8OADDownloadFID = MSAPI_DMX_INVALID_FLT_ID;
    MS_U32DOWNLOADSIZE = 0;
    u32DownloadSize = 0;
    versionNum = 0;
    u32ModuleAddr = 0;
    tsVersion=0;
    memset(au8MonitorSection, 0, sizeof(au8MonitorSection));
    memset(au8DownloadSection, 0, sizeof(au8DownloadSection));

    char *pTemp = getenv("CUSTOMER_OUI");
    MS_U32 u32CUSTOMER_OUI = (pTemp) ? simple_strtoul(pTemp, NULL, 16) : CUSTOMER_OUI;

    msAPI_OAD_ClearData();
    msAPI_OAD_SetOui((MS_U8 )(u32CUSTOMER_OUI>>16), (MS_U8 )(u32CUSTOMER_OUI>>8), (MS_U8 )(u32CUSTOMER_OUI));
    msAPI_OAD_SetVersionCheck(oad_VersonCheck) ;

    {
        //front end init
        char* buffer=NULL;
        int ret=0;

        buffer=(char *)malloc(BUF_SIZE);
        if(buffer != NULL)
        {
#if (ENABLE_DVB_C_SYS || ENABLE_DVB_S_SYS || ENABLE_DVB_S2_SYS)
            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_demodtype 1"); // dvbt/dvbs/dvbc
            ret=run_command(buffer, 0);
            if(ret==-1)
            {
                free(buffer);
                return -1;
            }

            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_init");
            ret=run_command(buffer, 0);
            if(ret==-1)
            {
                free(buffer);
                return -1;
            }
            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_tune %lu %x", u32Frequency, u8BandWidth);
            ret = run_command(buffer, 0);
            if(ret==-1)
            {
                return -1;
            }
#elif (ENABLE_DVB_T_SYS || ENABLE_DVB_T2_SYS)
            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_demodtype 1"); //dvbt
            ret=run_command(buffer, 0);
            if(ret==-1)
            {
                free(buffer);
                return -1;
            }
            memset(buffer, 0 , 256);
            sprintf(buffer, "tuner_tune %lu %x", u32Frequency, u8BandWidth);
            ret=run_command(buffer, 0);
            if(ret==-1)
            {
                //try T2
                memset(buffer, 0 , 256);
                sprintf(buffer, "tuner_demodtype 2"); //dvbt2
                ret=run_command(buffer, 0);
                if(ret==-1)
                {
                    free(buffer);
                    return -1;
                }
                memset(buffer, 0 , 256);
                sprintf(buffer, "tuner_tune %lu %x", u32Frequency, u8BandWidth);
                ret=run_command(buffer, 0);
                if(ret==-1)
                {
                    free(buffer);
                    return -1;
                }
            }
#endif
            memset(buffer, 0 , 256);

#if((MS_BOARD_TYPE_SEL == BD_MST098E_D01B) || (MS_BOARD_TYPE_SEL == BD_MST138B_D01A) \
    ||(MS_BOARD_TYPE_SEL == BD_MST050B_10ART_13225) || (MS_BOARD_TYPE_SEL == BD_MST065B_10ART_13225))
            sprintf(buffer, "dmx_init 3");  // DMX_FLOW_INPUT_EXT_INPUT1
#elif((defined(CONFIG_KRATOS)|| defined(CONFIG_KRIS)|| defined(CONFIG_KAYLA)) && (ENABLE_DVB_S_SYS || ENABLE_DVB_S2_SYS)) // kris and kratos are internal demod
            sprintf(buffer, "dmx_init 0");
#elif defined(CONFIG_KENYA) || ((defined(CONFIG_KERES) || defined(CONFIG_KIRIN)||defined(CONFIG_KRATOS)||defined(CONFIG_K1C)) && ENABLE_DVB_C_SYS)
            sprintf(buffer, "dmx_init 0");
#elif ((defined(CONFIG_K5TN)||defined(CONFIG_K5AP)) && ENABLE_DVB_T2_SYS)
            sprintf(buffer, "dmx_init 0");
#elif defined(CONFIG_KENYA) || ((defined(CONFIG_KERES) || defined(CONFIG_KIRIN)) && (ENABLE_DVB_T_SYS || ENABLE_DVB_T2_SYS || ENABLE_DVB_S_SYS || ENABLE_DVB_S2_SYS))
            sprintf(buffer, "dmx_init 3");
            printf("dmx flow input ext 3\n");
#else
            sprintf(buffer, "dmx_init 2");  // DMX_FLOW_INPUT_EXT_INPUT0
#endif

            if(dmx_init == 0)//DMX justneed to init one time
            {
                ret=run_command(buffer, 0);
                dmx_init = 1;
            }

            free(buffer);

            if(ret==-1)
            {
                return -1;
            }
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

void MApp_BL_Init(void)
{
#if (ENABLE_BL_UI)
    {
        char* buffer=NULL;
        int ret=0;

        buffer=(char *)malloc(BUF_SIZE);
        if(buffer != NULL)
        {
            memset(buffer, 0 , 256);
            sprintf(buffer, "osd_create %d %d", GWIN_WIDTH, GWIN_HEIGHT);
            ret=run_command(buffer, 0);
            free(buffer);
            if(ret==-1)
                return;
        }
        else
        {
            printf("%s: malloc() failed, at %d\n", __func__, __LINE__);
        }
    }
#endif //(ENABLE_BL_UI)

    /* initialize 8K bytes memory pool */
    msAPI_Memory_Init();
}

#ifdef HB_CUS
extern int MApp_BL_FindOADChan(MS_U16 *pOADPid);
MS_BOOL oad_get_pid(void)
{
    MS_U16 u16OADPid;
    int ret = 0;
#if ENABLE_DVB_T2_SYS
    MS_U16 PlpIDList = 0;
    MS_U16 u8Index = 0;
    MS_U16 PlpID = 0;
#endif
    if (wOad_PID != 0xFF && wOad_PID !=0)
    {
        return TRUE;
    }

#if ENABLE_DVB_T2_SYS
    PlpIDList = msAPI_GetPlpIDList();
    printf("PlpIDList=%d\n",PlpIDList);
    do
    {
        PlpID = msAPI_GetNextPlpID(u8Index);
        printf("PlpID=%d\n",PlpID);
        msAPI_SetPlpID(PlpID);
        ret = MApp_BL_FindOADChan(&u16OADPid);
        if (ret == 0)
        {
            wOad_PID = u16OADPid;
            return TRUE;
        }
    }while(u8Index++ < PlpIDList);
    return FALSE;
#else
    ret = MApp_BL_FindOADChan(&u16OADPid);//if  true return :0 if false return:-1
    if (ret)//MApp_BL_FindOADChan return wrong value
    {
        printf("\nhererere\n");
        u16OADPid = DEFAULT_OAD_PID;//if we couldn't get oad_pid,use the default one 1260
        return FALSE;
	}

    wOad_PID = u16OADPid;
    return TRUE;
#endif
}

void scan_chaninfo_init(CHANINFO_T * chan_info)
{
    char* p_str = NULL;
    p_str = getenv("oad_upgrade_freq");
    if(p_str == NULL)
    {
        printf("Get oad_upgrade_freq fail...\n");
        return;
    }

    wOad_PID = 0;
    u32Frequency = simple_strtoul (p_str, NULL, 10);
    u8BandWidth = 3;

    chan_info->m_bandwith = u8BandWidth;
    chan_info->m_chanNum = 21;
    chan_info->m_countryType = COUNTRY_TYPE_EUROPE;
    chan_info->m_frequency = u32Frequency;
    chan_info->m_u16OADPid = wOad_PID;
}

void scan_chaninfo_update(CHANINFO_T * chan_info)
{
    u32Frequency = chan_info->m_frequency;
    u8BandWidth =  chan_info->m_bandwith;
}
#endif // end ENABLE_HUMMINGBIRD_CUSTOMER

int chakra2_main(void)
{
#ifdef HB_CUS
    CHANINFO_T  RfChanInfo;
    int ret = 0;
    char* buffer = NULL;
    MS_U32 progress_value = 2;
    MS_U32 frequency;
#endif
    DBG_PRINT( printf("%s: at %d\n", __func__, __LINE__) );
    MApp_BL_Init();

    DBG_PRINT( printf("prepare USB/OAD upgrade !!\n") );
#ifdef HB_CUS
#if (ENABLE_DVB_C_SYS || ENABLE_DVB_S_SYS || ENABLE_DVB_S2_SYS)
    if(oad_prepare())//if true return 0 else return -1
    {
        return -1;
    }
    if(!oad_get_pid())// if true return ture else return false
    {
        return -1;
    }
    //MDrv_Sys_ClearWatchDog();
    //usb_prepare();
#elif (ENABLE_DVB_T_SYS || ENABLE_DVB_T2_SYS)
    buffer = (char *)malloc(BUF_SIZE);
    if(!buffer)
    {
        return -1;
    }
    scan_chaninfo_init(&RfChanInfo);

    memset(buffer, 0 , BUF_SIZE);
    sprintf(buffer, "tuner_init");
    ret=run_command(buffer, 0);
    if(ret == -1)
    {
        free(buffer);
        //Tuner init fail, return
        return -1;
    }

    do{
        memset(buffer, 0 , BUF_SIZE);
        MApp_BL_DisplaySystem_clear();
        scan_chaninfo_update(&RfChanInfo);
        frequency = (RfChanInfo.m_frequency / 1000);
        progress_value = ((frequency - DVBT_FIRST_FREQ) * 100) / (DVBT_LAST_FREQ - DVBT_FIRST_FREQ);
        MApp_BL_DisplaySystem_setStatus(en_freq_scan);
        sprintf(buffer, "draw_string %d %d ffffffff 1 %dMHz", GRAPHIC_X + LINE_HEIGHT, GRAPHIC_Y + LINE_HEIGHT * 5, frequency);
        run_command(buffer, 0);
        MApp_BL_DisplaySystem(progress_value);

        if(oad_prepare())//if true return 0 else return -1
        {
            continue;
        }

        if(!oad_get_pid())// if true return ture else return false
        {
            continue;//get oad_pid false
        }
        else
        {
            ret = 1;
            MApp_BL_DisplaySystem_clear();
	        MApp_BL_DisplaySystem_setStatus(en_freq_scan_success);
            sprintf(buffer, "draw_string %d %d 0x3fffffff 1 %x", GRAPHIC_X + LINE_HEIGHT, GRAPHIC_Y + LINE_HEIGHT * 5,en_wait);
            run_command(buffer, 0);
            MApp_BL_DisplaySystem(100);
            MsOS_DelayTask(3000);
            break;//get oad_pid true
        }
    }while(!OAD_SCAN_GetNextChanInfo(&RfChanInfo));//if ture,return 0 else return -1

    free(buffer);
    if (!ret) //if scan for oad_pid failed then return
        return -1;
#endif
#else
    if(oad_prepare())
        return -1;
#endif

    MApp_BL_DisplaySystem_clear(); //clear screen
    MApp_BL_DisplaySystem(0);

    if(!chakra2_main_loop())
    {
        return 0;
    }
    return -1;
}

int do_oad_get_size (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_U32* size;
    if (argc !=2) {
        printf("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }
    size=(MS_U32*)simple_strtoul(argv[1], NULL, 16);
    *size = MS_U32DOWNLOADSIZE;
    return 0;
}

static int _AllocateSectionBuffer(void)
{
    MS_U32 u32DmxSecBuffADR = 0;
    MS_U32 u32DmxSecBuffLen = 0;
#if ENABLE_STB_ECOS_BOOT
    MS_U8 u8Alignment;
    MApi_DMX_GetCap(DMX_CAP_SEC_BUF_ALIGN, &u8Alignment);
    u32OADBuffer = malloc(OADSEC_BUFFER_LEN + u8Alignment - 1);
    if(!u32OADBuffer)
    {
        printf("section buffer allocate fail");
        return -1;
    }
    u32DmxSecBuffADR = (MS_U32)MS_VA2PA(((u32OADBuffer + u8Alignment - 1)/u8Alignment)*u8Alignment);
#else
    if(get_map_addr_from_env(E_DMX_SECBUF, DMX_SEC_BUFFER_ADR,&u32DmxSecBuffADR)!=0)
    {
        printf("get %s mmap fail\n",E_MMAP_ID_DMX_SECBUF_ADR);
        return -1;
    }
    if(get_map_size_from_env(E_DMX_SECBUF, NO_DEFAULT_MMAP_VALUE,&u32DmxSecBuffLen)!=0)
    {
        printf("get %s mmap fail\n",E_MMAP_ID_DMX_SECBUF_LEN);
        return -1;
    }
#endif
    u32OadbufMonitorAddr = u32DmxSecBuffADR;
    u32OadbufDownloadAddr = (MS_U32)(u32OadbufMonitorAddr + 0x4000);
    u32OadbufMonitorSize = DMX_BUF_SIZE_MONITOR;
    u32OadbufDownloadSize = DMX_BUF_SIZE_DOWNLOAD;

#if !ENABLE_STB_ECOS_BOOT
    if((u32OadbufMonitorSize + u32OadbufDownloadSize) > u32DmxSecBuffLen)
    {
        printf("E_MMAP_ID_DMX_SECBUF size is not enough\n");
        return -1;
    }
#endif

    return 0;
}

static void _FreeSectionBuffer(void)
{
#if ENABLE_STB_ECOS_BOOT
    if(u32OADBuffer)
    {
        free(u32OADBuffer);
    }
#endif
}

int do_oad (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (argc != 7) {
        printf("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }
    u32Frequency = (MS_U32)simple_strtoul(argv[1], NULL, 10);
    u8BandWidth  = (MS_U8 )simple_strtoul(argv[2], NULL, 16);
    wOad_PID     = (MS_U16)simple_strtoul(argv[3], NULL, 16);
    u32GroupId   = (MS_U32)simple_strtoul(argv[4], NULL, 16);
    u8IsSDTT_OAD = (MS_U8 )simple_strtoul(argv[5], NULL, 10);
    wOad_PLP     = (MS_U16)simple_strtoul(argv[6], NULL, 10);

    if(_AllocateSectionBuffer())
    {
        return -1;
    }
    int ret = chakra2_main();
    _FreeSectionBuffer();

    return ret;
}

int do_oad_dvbc (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (argc != 6) {
        printf("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }
    u32Frequency = (MS_U32)simple_strtoul(argv[1], NULL, 10);
    u32QAM = (MS_U32)simple_strtoul(argv[2], NULL, 10);
    u16SymbolRate = (MS_U16)simple_strtoul(argv[3], NULL, 10);
    wOad_PID     = (MS_U16)simple_strtoul(argv[4], NULL, 16);
    u32GroupId   = (MS_U32)simple_strtoul(argv[5], NULL, 16);

    if(_AllocateSectionBuffer())
    {
        return -1;
    }
    int ret = chakra2_main();
    _FreeSectionBuffer();

    return ret;
}

int do_oad_dvbs(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    if (argc != 5) {
        printf("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }
    u32Frequency = (MS_U32)simple_strtoul(argv[1], NULL, 10);
    u16SymbolRate = (MS_U16)simple_strtoul(argv[2], NULL, 10);
    wOad_PID     = (MS_U16)simple_strtoul(argv[3], NULL, 16);
    u32GroupId   = (MS_U32)simple_strtoul(argv[4], NULL, 16);

    if(_AllocateSectionBuffer())
    {
        return -1;
    }
    int ret = chakra2_main();
    _FreeSectionBuffer();

    return ret;
}

U_BOOT_CMD(
    oad_get_size,    2,   1,  do_oad_get_size,
    "oad_get_size - Get the file size from OAD download \n",
    "Get download size\n"
    "    - Address: 32 bits (Hex) address to save download size\n"
);

U_BOOT_CMD(
    oad,    7,   1,  do_oad,
    "oad - Software Upgrade(OAD) \n",
    "Over the Air Download\n"
    "    - Frequency: 32 bits (Decimal). e.g. 474000\n"
    "    - Bandwidth: 1:6MHz, 2:7MHz, 3: 8MHz\n"
    "    - Pid: 16 bits (Hex) pid, e.g. 0x150\n"
);

U_BOOT_CMD(
    oaddvbc,    6,   1,  do_oad_dvbc,
    "oad - Software Upgrade(OAD) \n",
    "Over the Air Download\n"
    "    - Frequency: 32 bits (Decimal). e.g. 474000\n"
    "    - Bandwidth: 1:6MHz, 2:7MHz, 3: 8MHz\n"
    "    - Pid: 16 bits (Hex) pid, e.g. 0x150\n"
);

U_BOOT_CMD(
    oaddvbs,    5,   1,  do_oad_dvbs,
    "oad - Software Upgrade(OAD) \n",
    "Over the Air Download\n"
    "    - Frequency: 32 bits (Decimal). e.g. 474000\n"
    "    - u16SymbolRate: e.g. 27500\n"
    "    - Pid: 16 bits (Hex) pid, e.g. 0x150\n"
);

#endif

