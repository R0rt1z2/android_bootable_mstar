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
#include <configs/uboot_board_config.h>
#include <MsTypes.h>
#include <MsIRQ.h>
#include <MsOS.h>
#include <apiGOP.h>
#include <apiGFX.h>
#include <drvSEM.h>
#include <drvGPIO.h>
#include <drvIIC.h>
#include <apiDMX.h>
#include <configs/uboot_module_config.h>
#include "../oad/msAPI_demux.h"
#include "../oad/msAPI_oad_process.h"
#include "../oad/msAPI_oad_parser.h"
#include "../oad/msAPI_Memory.h"
#include "msAPI_Tuner.h"
#include <linux/string.h>
#include <asm/string.h>
#include "uboot_mmap.h"
#include "apiRfChannel.h"
//#include <apiDigiTuner.h>
#include "../oad/cus_oad/msAPI_oad_scan.h"
//#include "frontend_config.h"
//#include "../customer/cus_cfg_info.h"
//#include "../customer/frontend_config.h"

extern MS_U32 msAPI_Timer_GetTime0(void);
extern MS_U32 msAPI_Timer_DiffTimeFromNow(MS_U32 u32TaskTimer);
extern MS_BOOL MApi_LDR_CheckANTStatus(void);

#define TUNER_LOCK_TIME_OUT     5000//ms
#define  MAX_PIDS_CNT 50
#define appDemoDmx_print printf
static MS_U16 gCurrProgPids[MAX_PIDS_CNT];
static MS_U16 gCurrProgNo[MAX_PIDS_CNT];
static MS_U8  gCurrProgCnt = 0;
//unsigned int gCountryType = COUNTRY_TYPE_EUROPE;
#define OADSEC_BUFFER_ADR   0x0001B00000  //Alignment 0x00010

typedef enum
{
	BW_MODE_6MHZ = 0,                                                   ///< 6 MHz
	BW_MODE_7MHZ,                                                       ///< 7 MHz
	BW_MODE_8MHZ                                                        ///< 8 MHz
} EN_TER_BW_MODE;


BOOLEAN msAPI_OAD_SCAN_CreateSectionFilter(U8 u08TableId,U16 u16MsgId, U16 u16Pid, U8  *u08OadFID)
{
    MS_U8                                  pattern[16];
    MS_U8                                  mask[16];
    MS_U8                                  nmask[16];
    DMX_Flt_info                           FltInfo;
    MS_U8                                 *pu8BufAddr;
    DMX_FILTER_TYPE                        FilterType;
    DMX_FLOW_INPUT                         inSrc;
    MS_U8                                  DmxIdSect;
#if (ENABLE_DVB_S2_SYS||ENABLE_DVB_S_SYS||ENABLE_DVB_T2_SYS)
    inSrc = DMX_FLOW_INPUT_EXT_INPUT1;
#if(FRONTEND_DEMOD_TYPE == MSTAR_MSB1236C_DEMOD)
inSrc = DMX_FLOW_INPUT_EXT_INPUT0;
#endif
#else
#if (DMX_INPUT == DMX_INPUT_DEMOD)
    inSrc = DMX_FLOW_INPUT_DEMOD;
#elif (DMX_INPUT == DMX_INPUT_EXT_INPUT0)
    inSrc = DMX_FLOW_INPUT_EXT_INPUT0;
#elif (DMX_INPUT == DMX_INPUT_EXT_INPUT1)
    inSrc = DMX_FLOW_INPUT_EXT_INPUT1;
#else
    inSrc = DMX_FLOW_INPUT_DEMOD;
#endif
#endif
    printf("inSrc:%d, \n", inSrc);

    FilterType = DMX_FILTER_TYPE_SECTION | DMX_FILTER_SOURCE_TYPE_LIVE;


    // Allocate a Filter and set Filter Basic Type
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(FilterType, &DmxIdSect))
    {
        appDemoDmx_print("[%s] Allocaet filter fail \n",__FUNCTION__);
    }

    // Here we use pre-defined physical address of reserved section buffer.
     // [Note] The section buffe MUST be continus in physical address space.(for DMA usage)
     pu8BufAddr = (MS_U8*)OADSEC_BUFFER_ADR;

     //MsOS_PA2KSEG1 : transfer physical address to virtual address.
     memset((MS_U8*)MsOS_PA2KSEG1((MS_U32)pu8BufAddr), 0 , MSAPI_DMX_SECTION_4K);


     // Transfer Virtual Address to Phisical Hardware Address
     // Section buffer is structed as ring buffer, keep 4 pointer of start,end ,read and write.
     FltInfo.Info.SectInfo.SectBufAddr = (MS_U32)pu8BufAddr ;
     appDemoDmx_print("Physical Address = %08lx\n" ,FltInfo.Info.SectInfo.SectBufAddr);
     // Set buffer size
     FltInfo.Info.SectInfo.SectBufSize =     MSAPI_DMX_SECTION_4K;
     appDemoDmx_print("Size = %08lx\n" ,FltInfo.Info.SectInfo.SectBufSize);


     // We only need one section of PAT so we choose "DMX_SECT_MODE_ONESHOT" instead of "DMX_SECT_MODE_CONTI"
     FltInfo.Info.SectInfo.SectMode    =     (DMX_SECT_MODE_ONESHOT| DMX_SECT_MODE_CRCCHK);

     // <DMX_EVENT_DATA_READY/DMX_EVENT_BUF_OVERFLOW>
     // Event trigger condition for driver, Driver will call ap's callback  < CallBack Mode >
     // Cause We set SectMode as "DMX_SECT_MODE_ONESHOT" , we don't need to worry about Overflow event.
     // The Default event setting is Polling mode, but for performance consideration, Callback mode is recommended.
     FltInfo.Info.SectInfo.Event       =     DMX_EVENT_DATA_READY | DMX_EVENT_CB_SELF; // DMX_EVENT_BUF_OVERFLOW

     // Set the pointer of the event CB function into Demux driver
     FltInfo.Info.SectInfo.pNotify     =   NULL;//DmxDataReadyNotify; //_appDemo_PatCb;

     // Set Advanced Filter infomation
     if (DMX_FILTER_STATUS_OK != MApi_DMX_Info( DmxIdSect, &FltInfo, &FilterType, TRUE))
     {
         appDemoDmx_print("[%s] MApi_DMX_Info fail \n",__FUNCTION__);
         goto DMX_PAT_FAIL;
     }


     if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(DmxIdSect , &u16Pid, TRUE))
     {
         appDemoDmx_print("[%s] MApi_DMX_Pid fail \n",__FUNCTION__);
         goto DMX_PAT_FAIL;
     }

     // Set Section Match pattern
     // The Match pattern is used for Getting specific section
     // Pattern[16] =  Pattern for match
     // Mask[16]    =  Match Mask : set 1 for match / 0 for ignore
     memset(pattern, 0x0, sizeof(pattern));
     memset(mask,    0x0, sizeof(mask));
     memset(nmask,   0x0, sizeof(nmask));
     pattern[0]=     u08TableId ;
     mask[0]=        0xFF ;
     if (u16MsgId != 0x00)
     {
        OAD_DEBUG(printf("++++++++++++++++++++++>(program_number=%d)<+++++++++++++++++++++@\n", u16MsgId));
        pattern[3] = (MS_U8)(u16MsgId>>8);
        pattern[4] = (MS_U8)u16MsgId;
        mask[3]=        0xFF ;
        mask[4]=        0xFF ;
     }

     MApi_DMX_SectPatternSet(DmxIdSect, pattern, mask, nmask, 16);

     // Reset Section filter and section Buffer status;
     MApi_DMX_SectReset(DmxIdSect);

     // Start Filter and record section into Section Buffer.
     if (DMX_FILTER_STATUS_OK!= MApi_DMX_Start(DmxIdSect))
     {
         appDemoDmx_print("enable section filter fail\n");
         goto DMX_PAT_FAIL;
     }
     *u08OadFID = DmxIdSect;

     return TRUE;

 DMX_PAT_FAIL:
     MApi_DMX_Close(DmxIdSect);
     return FALSE;

}

extern U8 au8MonitorSection[4096];

static void OAD_SCAN_ParsePAT(void)
{
    MS_U16 u16ProgramNumber, u16SectionLength;
    MS_U8  uSectionNumber, uLastSectionNumber;  // todo: multiple section
    MS_U8  *pu8SecIter;
    MS_U8  i;
    MS_U32 u32NumOfItem;
    MS_U8  u8PatVerNum;
    MS_U16 u16ActualTSid;
    MS_U8* pu8Section= au8MonitorSection;
    MS_U8  u8Tid;
    MS_U16 u16Pid;

    u8Tid=                      pu8Section[0];
    u16SectionLength=           (pu8Section[1] & 0x0f ) << 8;
    u16SectionLength|=          pu8Section[2];
    u32NumOfItem=               (u16SectionLength - 9)/ 4;
    u16ActualTSid=              (pu8Section[3]<< 8)| pu8Section[4];
    u8PatVerNum=                (pu8Section[5]>> 1) & 0x1F;
    uSectionNumber=             pu8Section[6];
    uLastSectionNumber=         pu8Section[7];

    gCurrProgCnt = 0;
    pu8SecIter = pu8Section + 8;
    for ( i = 0; i < u32NumOfItem; i++ )
    {
        u16ProgramNumber =       (pu8SecIter[0]<<8)| pu8SecIter[1];
        u16Pid =                 ((pu8SecIter[2] & 0x1F)<< 8)| pu8SecIter[3];
        OAD_DEBUG(printf("(program number, PID)= (0x%04x, 0x%04x)\n", u16ProgramNumber, u16Pid));
        pu8SecIter += 4;
        if (i < MAX_PIDS_CNT && (u16ProgramNumber != 0x00))
        {
            gCurrProgNo[gCurrProgCnt] = u16ProgramNumber;
            gCurrProgPids[gCurrProgCnt] = u16Pid;
            gCurrProgCnt += 1;
        }
    }
}
#define GPIO_ANT_OVERCURRENT 42
#define GPIO_ANT_PWR_CTRL    20

MS_BOOL MApi_LDR_CheckANTStatus(void)
{
    MS_U8 isANTOK;

    isANTOK = mdrv_gpio_get_level(GPIO_ANT_OVERCURRENT);


    if (isANTOK)
    {
        return TRUE;
    }
    else
    {
        mdrv_gpio_set_low(GPIO_ANT_PWR_CTRL);
        return FALSE;
    }
}


MS_BOOL OAD_SCAN_GetSectionData(MS_U8 DmxIdSect)
{
    MS_U8                  *pu8SecBuf;
    MS_U32                 u32ActualSize,u32RmnSize;
    DMX_FILTER_STATUS      procRet ;
    DMX_EVENT              eEvent = DMX_EVENT_DATA_READY;
    MS_U16                 delayCnt = 6000;

    do
    {
        procRet = MApi_DMX_Proc(DmxIdSect,&eEvent) ;
        if (procRet==DMX_FILTER_STATUS_OK)
        {
            if (eEvent == DMX_EVENT_DATA_READY)
            {
                OAD_DEBUG(memset(au8MonitorSection, 0xBB, MSAPI_DMX_SECTION_4K));
                OAD_DEBUG(printf("section Addr: 0x%08LX\n", (au8MonitorSection)));

                if( MApi_DMX_CopyData(DmxIdSect, au8MonitorSection, MSAPI_DMX_SECTION_4K,(MS_U32*)&u32ActualSize,(MS_U32*)&u32RmnSize,NULL) == DMX_FILTER_STATUS_OK )
                {
                    if(msAPI_DMX_CheckCRC32(au8MonitorSection)== FALSE)
                    {
                        OAD_ASSERT(printf("crc error %s %d \n",__FUNCTION__,__LINE__));
                        msAPI_DMX_Stop(DmxIdSect);
                        msAPI_DMX_Reset(DmxIdSect);
                        msAPI_DMX_Start(DmxIdSect);
                        pu8SecBuf = NULL ;
                    }
                    else
                    {
                        MApi_DMX_Stop(DmxIdSect);
                        MApi_DMX_Close(DmxIdSect);
                        return TRUE;
                    }
                }
                else
                {
                    OAD_ASSERT( printf("Error> msAPI_OAD_PollingSectionFilter : MApi_DMX_CopyData Error.\n") );
                    msAPI_DMX_Stop(DmxIdSect);
                    msAPI_DMX_Reset(DmxIdSect);
                    msAPI_DMX_Start(DmxIdSect);
                }
            }
            else
            {
                OAD_ASSERT( printf("Polling OVERFLOW = %u\n", (U8)DmxIdSect); );
                msAPI_DMX_Stop(DmxIdSect);
                msAPI_DMX_Reset(DmxIdSect);
                msAPI_DMX_Start(DmxIdSect);
            }

        }
        MsOS_DelayTask(10);
        MApi_LDR_CheckANTStatus();
    }while(delayCnt-->0);

    MApi_DMX_Stop(DmxIdSect);
    MApi_DMX_Close(DmxIdSect);

    OAD_DEBUG(printf("\n+++++++++++++++++++++++++++++++++++> TIMEOUT <++++++++++++++++++++++\n"));

    return FALSE;
}


MS_BOOL OAD_SCAN_GetChanPATInfo(void)
{
    MS_U8  DmxIdSect;

    if (!msAPI_OAD_SCAN_CreateSectionFilter(0, 0, 0, &DmxIdSect))
    {
        return FALSE;
    }

    if(!OAD_SCAN_GetSectionData(DmxIdSect))
    {
        return FALSE;
    }

    OAD_SCAN_ParsePAT();

    return TRUE;
}

MS_BOOL OAD_SCAN_ParsePMT(MS_U16 *pOADPid)
{
    MS_U16 u16ProgramNumber, u16SectionLength;
    MS_U8  uSectionNumber, uLastSectionNumber;
    MS_U16 i;
    MS_U8  u8PatVerNum;
    MS_U8* pu8Section= au8MonitorSection;
    MS_U8  u8Tid;
    MS_U16 u16PCRPid;
    MS_U16 u16ProgInfoLen;
    MS_U16 curPos;
    MS_U16 streamType;
    MS_U16 esPid;
    MS_U16 esInfoLen;
    char *pTemp;
	MS_U32 CustomerOUI = 0x14B9;
    u8Tid=                      pu8Section[0];
    u16SectionLength=           (pu8Section[1] & 0x0f ) << 8;
    u16SectionLength|=          pu8Section[2];
    u16ProgramNumber =          (pu8Section[3]<< 8)| pu8Section[4];
    u8PatVerNum=                (pu8Section[5]>> 1) & 0x1F;
    uSectionNumber=             pu8Section[6];
    uLastSectionNumber=         pu8Section[7];
    u16PCRPid         =         ((pu8Section[8]&0x1F)<<8)|pu8Section[9];
    u16ProgInfoLen    =         ((pu8Section[10]&0x0F)<<8)|pu8Section[11];

    curPos = 12;
    curPos += u16ProgInfoLen;
    pTemp = getenv("CUSTOMER_OUI");
    if (pTemp)
    {
        CustomerOUI = simple_strtoul(pTemp, NULL, 16);
    }
    for (; curPos < u16SectionLength-4; )
    {
        streamType = pu8Section[curPos];
        esPid =      ((pu8Section[curPos+1]&0x1F)<<8)|pu8Section[curPos+2];
        curPos += 3;
        esInfoLen =  ((((pu8Section[curPos]&0x0F)<<8)|pu8Section[curPos+1]));
        curPos += 2;
        if (streamType == 0x0B)
        {
            MS_U8 esDescTag;
            MS_U8 descLen;
            for (i = curPos; i < esInfoLen+curPos; )
            {
                esDescTag = pu8Section[i];
                i++;
                descLen = pu8Section[i];
                i++;
                if (esDescTag == 0x66 && descLen == 18)
                {
                    OAD_DEBUG(printf("descLen=%d\n", descLen));
                    OAD_DEBUG(printf("%c\n", pu8Section[i+9]));
                    OAD_DEBUG(printf("%c\n", pu8Section[i+10]));
                    OAD_DEBUG(printf("%c\n", pu8Section[i+11]));
                    OAD_DEBUG(printf("%c\n", pu8Section[i+12]));
                    OAD_DEBUG(printf("%c\n", pu8Section[i+13]));

                    if (pu8Section[i] == 0x00 && pu8Section[i+1] == 0x0a)
                    {
                        if(pu8Section[i+3] == (MS_U8)(CustomerOUI>>16)
                           &&pu8Section[i+4] == (MS_U8)(CustomerOUI>>8)
                           &&pu8Section[i+5] == (MS_U8)CustomerOUI && !memcmp(&pu8Section[i+9], "MSTAR OAD", 9))
                        {
                            *pOADPid = esPid;
                            OAD_DEBUG(printf("esPId=%d\n", esPid));
                            return TRUE;
                         }
                    }
                }
                OAD_DEBUG(printf (">>>esDescTag=%x\n", esDescTag));
                i += descLen;
            }
        }
        curPos += esInfoLen;
    }
    return FALSE;
}

MS_BOOL OAD_SCAN_GetPMTTable(MS_U16 *pOADId)
{
    MS_U8             DmxIdSect;
    MS_U8             i;

    for (i = 0; i < gCurrProgCnt; i++)
    {
        if(!msAPI_OAD_SCAN_CreateSectionFilter(0x02, gCurrProgNo[i], gCurrProgPids[i], &DmxIdSect))
        {
            OAD_DEBUG(printf("\n@)++++++++++++>(msAPI_OAD_SCAN_CreateSectionFilter,Error)<+++++++++++++++(@\n"));
            continue;
        }

        if(!OAD_SCAN_GetSectionData(DmxIdSect))
        {
            printf("\n=======OAD_SCAN_GetSectionData=======\n");
            continue;
        }

        if(!OAD_SCAN_ParsePMT(pOADId))
        {
            printf("\n=======OAD_SCAN_ParsePMT======\n");
            continue;
        }
        else
        	{
        	printf("\n=======break for=======\n");
            break;
        	}
    }

    if (i >= gCurrProgCnt)
    	{
    	 MApi_DMX_Stop(DmxIdSect);
         MApi_DMX_Close(DmxIdSect);
		 printf("\n========return false and close dmx!!!\n");
         return FALSE;
    	}
   OAD_DEBUG( printf("\n@)++++++++++++++++++++>Get OAD PID-->(%d)<++++++++++++++++++++++(@\n", *pOADId);)
    return TRUE;
}
#if 0//DVBS_STYLE
int OAD_SCAN_ChanTryLock(CHANINFO_S*pChanInfo_S)
{
    //front end init
    char* buffer=NULL;
    int ret=0;
    U32 u32DsmccTimer=0;
    MS_BOOL Check_FE_Result;

    buffer=(char *)malloc(256);
    if(buffer == NULL)
        return -1;

    memset(buffer, 0 , 256);
    sprintf(buffer, "tuner_init");
    ret=run_command(buffer, 0);
    if(ret==-1)
    {
        free(buffer);
        return -1;
    }
	//free(buffer);
	memset(buffer, 0 , sizeof(CHANINFO_S));
	sprintf(buffer, "tuner_tune %d %d %d %d %d %d %d %d %d %d %d %d %d", pChanInfo_S->m_SatNo,pChanInfo_S->m_Fre,       \
                                pChanInfo_S->m_Sym,pChanInfo_S->m_Pol,pChanInfo_S->m_u16LoLOF,pChanInfo_S->m_u16HiLOF,	\
								pChanInfo_S->m_eToneburstType,pChanInfo_S->m_eSwt10Port,pChanInfo_S->m_eSwt11Port,pChanInfo_S->m_e22KOnOff,	\
								pChanInfo_S->m_eLNBPwrOnOff,pChanInfo_S->m_e0V12VOnOff,pChanInfo_S->m_u8Position);
    ret=run_command(buffer, 0);
	if(ret==-1)
	{
	   free(buffer);
	   return -1;
	}
	//memset(buffer, 0 , 256);

	memset(buffer, 0 , 256);
	sprintf(buffer, "dmx_init 3");
	ret=run_command(buffer, 0);
	if(ret==-1)
	{
		free(buffer);
		ret =  -1;
	}
    free(buffer);

		u32DsmccTimer = msAPI_Timer_GetTime0();
		while(1)
		{
			MApi_LDR_CheckANTStatus();
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
			if (msAPI_Timer_DiffTimeFromNow(u32DsmccTimer) > TUNER_LOCK_TIME_OUT)
			{
				OAD_DEBUG( printf("Error> Exit DSI\n") );
				return -1; //return to do USB download
			}
			MsOS_DelayTask(500);
		}
		return ret;
	}

#else
int OAD_SCAN_ChanTryLock(CHANINFO_T*pChanInfo)
{
    //front end init
    char* buffer=NULL;
    int ret=0;
    U32 u32DsmccTimer=0;
    MS_BOOL Check_FE_Result;

    buffer=(char *)malloc(256);
    if(buffer == NULL)
        return -1;

    memset(buffer, 0 , 256);
    sprintf(buffer, "tuner_init");
    ret=run_command(buffer, 0);
    if(ret==-1)
    {
        free(buffer);
        return -1;
    }


#if 1//((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD )||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT51_DEMOD )||(FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC51_DEMOD ))//!= MSTAR_MSB123x_DEMOD)||(FRONTEND_DEMOD_TYPE != MSTAR_MSB122x_DEMOD)
   //printf("Internal \n");
   memset(buffer, 0 , 256);
   sprintf(buffer, "tuner_tune %lu %x", pChanInfo->m_frequency, pChanInfo->m_bandwith);
   ret=run_command(buffer, 0);
   if(ret==-1)
   {
       free(buffer);
       return -1;
   }
   memset(buffer, 0 , 256);

static MS_U8 dmxHadInited = 0;
if(dmxHadInited)
{
    MApi_DMX_Exit();
    dmxHadInited = 0;
}
   sprintf(buffer, "dmx_init 0");
   ret=run_command(buffer, 0);
   if(ret==-1)
   {
       free(buffer);
       return -1;
   }
   dmxHadInited = 1;

#endif

    free(buffer);


    u32DsmccTimer = msAPI_Timer_GetTime0();
    while(1)
    {
    	MApi_LDR_CheckANTStatus();
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
        if (msAPI_Timer_DiffTimeFromNow(u32DsmccTimer) > TUNER_LOCK_TIME_OUT)
        {
            OAD_DEBUG( printf("Error> Exit DSI\n") );
            return -1; //return to do USB download
        }
        MsOS_DelayTask(500);
    }
    return ret;
}
#endif
int OAD_SCAN_GetNextChanInfo(CHANINFO_T *pChanInfo)
{
    unsigned int ret = 0;
    MS_U8 currChNum;

    currChNum = pChanInfo->m_chanNum;
    pChanInfo->m_chanNum=MApi_RfChannel_GetNextRfChNum(pChanInfo->m_countryType, pChanInfo->m_chanNum);
    if (currChNum >= pChanInfo->m_chanNum)
        return -1;

    ret = MApi_RfChannel_GetTPSetting(pChanInfo->m_chanNum,pChanInfo->m_countryType,
                                      &(pChanInfo->m_bandwith), &(pChanInfo->m_frequency));

    if (!ret)
        return -1;

    if (pChanInfo->m_bandwith == 8000)
    {
        pChanInfo->m_bandwith = BW_MODE_8MHZ;
    }
    else if (pChanInfo->m_bandwith == 7000)
    {
        pChanInfo->m_bandwith = BW_MODE_7MHZ;
    }
    else
        pChanInfo->m_bandwith = BW_MODE_6MHZ;

    pChanInfo->m_bandwith +=1;
    printf ("chNum=%d, bandwith=%d, freqency=%ld\n", pChanInfo->m_chanNum, pChanInfo->m_bandwith, pChanInfo->m_frequency);
    return 0;
}

int OAD_SCAN_ShowPrompt(void)
{
    run_command("draw_rect  100 100 540 400 0x0040ff", 0);
    run_command("draw_string 100 150 0x3fffffff 1 OAD UPGRADE", 0);
    run_command("draw_string 100 220 0x3fffffff 1 Searching OAD Channel...", 0);
	run_command("draw_string 100 280 0x3fffffff 1 REMOTE -EXIT- EXIT", 1);
    return 0;
}

char buffer[100];

int OAD_SCAN_ShowProgress(MS_U8 precent)
{
    run_command("draw_rect 120 340 510 30 0x0040ff", 0);
    memset(buffer, 0x00, 30);
    sprintf(buffer, "draw_progress 120 345 0xffffff, %d", precent);
    return run_command(buffer, 0);
}

int MApp_BL_OADGetRFChanInfo(CHANINFO_T *pRFChan)
{
    MS_U8               minChNum;
    MS_U8               maxChNum;
    int                 ret;
	char *pTemp;
	MS_U32 CountryType=COUNTRY_TYPE_EUROPE;
    
    if (!pRFChan)
        return 0;
    pTemp = getenv("COUNTRY_TYPE");
    if (pTemp)
	{
        CountryType = simple_strtoul(pTemp, NULL, 16);
    }
    minChNum = MApi_RfChannel_GetMinRfChNum(CountryType);
    maxChNum = MApi_RfChannel_GetMaxRfChNum(CountryType);
    pRFChan->m_countryType = CountryType;
    pRFChan->m_chanNum = minChNum;
    ret = MApi_RfChannel_GetTPSetting(pRFChan->m_chanNum, pRFChan->m_countryType,&pRFChan->m_bandwith, &pRFChan->m_frequency);
    if (!ret)
        return 0;

    if (pRFChan->m_bandwith == 8000)
    {
        pRFChan->m_bandwith = BW_MODE_8MHZ;
    }
    else if (pRFChan->m_bandwith == 7000)
    {
        pRFChan->m_bandwith = BW_MODE_7MHZ;
    }
    else
    {
        pRFChan->m_bandwith = BW_MODE_6MHZ;
    }
    pRFChan->m_bandwith += 1;

    OAD_DEBUG(printf ("chNum=%d, bandwith=%d, freqency=%d\n", minChNum, pRFChan->m_bandwith, pRFChan->m_frequency);)
    return (maxChNum-minChNum+1);

}


int MApp_BL_FindOADChan(MS_U16 *pOADPid)
{
    int ret;

    ret = OAD_SCAN_GetChanPATInfo();
    if (!ret)
        return -1;
    ret = OAD_SCAN_GetPMTTable(pOADPid);

    if(ret)
    	{
    	 printf("\n%s------success\n",__func__);
    	 return 0;
    	}
    else {
		 printf("\n%s------fail\n",__func__);
    	return -1;
    	}
}


