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
/// file    apiRfChannel.c
/// @brief  RF Channel Frequency High-Level API
////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
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
//#include "apiDigiTuner.h"
#include "apiRfChannel.h"


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------



#ifdef DVBT_STYLE
#define DVB_VHF_ENABLE           1
#else
#define DVB_VHF_ENABLE           0
#endif



//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
#if 1//AUSTRALIA_FREQTBL_NEW
FreqTblType FreqTbl[FREQ_TAL_SIZE]  =
{
        //must sort by channel no.  from small to large
        //if the channel is invalid,set the frequency 0
        //No. offset ----->for displaying 0:= No. ; +X:No.-X and add "A"  ; -X:No.-X
        {6,177500,0}, // 6~9
        {10,205500,1}, // 9A
        {11,212500,-1}, // 10~12
        {14,0,-1}, //13~20
        {21,480500,0}, // 21~27
        {28,527500,1}, // 27A
        {29,529500,-1}, // 28~75
};
#endif
//--------------------------------------------------------------------------------------------------
//  Global Variables
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Global Functions
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
/// Get RF Channel parameters
/// @param  u8RF_CH                 \b IN:  RF channel number
/// @param  u8Country               \b IN:  country
/// @param  u16BandWidth            \b IN:  channel bandwidth (kHz)
/// @param  u32Frequency            \b IN:  the frequency of the RF channel
/// @return TRUE: get the corresponding frequency
///         FALSE: failed to get the frequency.
//--------------------------------------------------------------------------------------------------
MS_BOOL MApi_RfChannel_GetTPSetting(MS_U8 u8RF_CH, MS_U8 u8Country, MS_U16 *u16BandWidth, MS_U32 *u32Frequency)
{
    //--------------------------------------------
    //wyf 20090324 to avoid  invalid channel number when manul scan
    MS_U8 u8MaxRFCh,u8MinRFCh;

    u8MaxRFCh = MApi_RfChannel_GetMaxRfChNum(u8Country);
    u8MinRFCh = MApi_RfChannel_GetMinRfChNum(u8Country);
    if(u8RF_CH<u8MinRFCh ||u8RF_CH>u8MaxRFCh)
    {
          *u32Frequency = 0;
	   return FALSE;
    }

    //---------------------------------------------
    switch(u8Country)
    {

        case COUNTRY_TYPE_CHILE:
            if ((u8RF_CH >= 14) && (u8RF_CH <= 79))   //473.000 ~ 803.000 Mhz
            {
                *u32Frequency = 473000L + (u8RF_CH - 14) * 6000L;
                *u16BandWidth = 6000;
            }
            else if((u8RF_CH >= 7) && (u8RF_CH <= 13))//177.000~473.000 Mhz
            {
                *u32Frequency = 177000L + (u8RF_CH - 7) * 6000L;
                *u16BandWidth = 6000;
            }
            else
            {
                *u32Frequency = 0;
            }
            break;

    //-------------------------------------//
        case COUNTRY_TYPE_PERU:
        case COUNTRY_TYPE_BRAZIL:
        if ((u8RF_CH >= 14) && (u8RF_CH <= 69))   //473.143 ~ 803.143 Mhz
        {
            *u32Frequency = 473143L + (u8RF_CH - 14) * 6000L;
            *u16BandWidth = 6000;
        }
        else if((u8RF_CH >= 7) && (u8RF_CH <= 13))//177.143~473.143 Mhz
        {
            *u32Frequency = 177143L + (u8RF_CH - 7) * 6000L;
            *u16BandWidth = 6000;
        }
        else
        {
            *u32Frequency = 0;
        }
        break;
	    case COUNTRY_TYPE_ARGENTINA:
	    {
	        if ((u8RF_CH ==14))
	        {
	             *u32Frequency = 527000;
	    	     *u16BandWidth = 6000;
	        }
 	        else
	        {
                 *u32Frequency = 647000;
	    	     *u16BandWidth = 6000;	        }
	        break;
	    }
        break;

    //-------------------------------------//
    case COUNTRY_TYPE_HONGKONG:
    case COUNTRY_TYPE_EUROPE:
    case COUNTRY_TYPE_CHINA:
        if ((u8RF_CH >= 5) && (u8RF_CH <= 12))   //177.5 ~ 226.5 Mhz
        {
            *u32Frequency = 177500L + (u8RF_CH - 5) * 7000L;
            *u16BandWidth = 7000;
        }
        else if ((u8RF_CH >= 21) && (u8RF_CH <= 70))   //474 ~ 858 Mhz
        {
            *u32Frequency = 474000L + (u8RF_CH - 21) * 8000L;
            *u16BandWidth = 8000;
        }
        else
        {
            *u32Frequency = 0;
        }
        break;
    //-------------------------------------//
    case COUNTRY_TYPE_GERMANY:
    case COUNTRY_TYPE_FRANCE:
        if ((u8RF_CH >= 5) && (u8RF_CH <= 12))   //177.5 ~ 226.5 Mhz
        {
            *u32Frequency = 177500L + (u8RF_CH - 5) * 7000L;
            *u16BandWidth = 7000;
        }
        else if ((u8RF_CH >= 21) && (u8RF_CH <= 70))   //474 ~ 858 Mhz
        {
            *u32Frequency = 474000L + (u8RF_CH - 21) * 8000L;
            *u16BandWidth = 8000;
        }
        else
        {
            *u32Frequency = 0;
        }
        break;
		
    case COUNTRY_TYPE_AUSTRALIA:
    {
    #if 0//AUSTRALIA_FREQTBL_NEW
        MS_S8 s8Index = FREQ_TAL_SIZE -1;

        for(s8Index = FREQ_TAL_SIZE -1;s8Index>=0;s8Index--)
        {
              if(u8RF_CH >= FreqTbl[s8Index].u8CHNo)
              {
                    break;
              }
        }
        if(s8Index>=0&&FreqTbl[s8Index].u32Freq>0)
        {
              *u32Frequency = FreqTbl[s8Index].u32Freq + (u8RF_CH -FreqTbl[s8Index].u8CHNo) * 7000L;
        }
        else
        {
              *u32Frequency = 0;
        }
    #else
	 if ((u8RF_CH >= 6) && (u8RF_CH <= 13))   //177.5 ~ 226.5 Mhz  6~9 9A~12
        {
            *u32Frequency = 177500L + (u8RF_CH - 6) * 7000L;
        }
        else if ((u8RF_CH >= 27) && (u8RF_CH <= 71))   //474 ~ 858 Mhz
        {
            *u32Frequency = 522500L + (u8RF_CH - 27) * 7000L;
        }
        else
        {
            *u32Frequency = 0;
        }
    #endif
	 *u16BandWidth = 7000;
        break;
    }
    case COUNTRY_TYPE_AUSTRALIA_NEW:
       {
   #if 1//AUSTRALIA_FREQTBL_NEW
           MS_S8 s8Index = FREQ_TAL_SIZE -1;
    
           for(s8Index = FREQ_TAL_SIZE -1;s8Index>=0;s8Index--)
           {
                 if(u8RF_CH >= FreqTbl[s8Index].u8CHNo)
                 {
                       break;
                 }
           }
           if(s8Index>=0&&FreqTbl[s8Index].u32Freq>0)
           {
                 *u32Frequency = FreqTbl[s8Index].u32Freq + (u8RF_CH -FreqTbl[s8Index].u8CHNo) * 7000L;
           }
           else
           {
                 *u32Frequency = 0;
           }
   #else
        if ((u8RF_CH >= 6) && (u8RF_CH <= 13))   //177.5 ~ 226.5 Mhz  6~9 9A~12
           {
               *u32Frequency = 177500L + (u8RF_CH - 6) * 7000L;
           }
           else if ((u8RF_CH >= 27) && (u8RF_CH <= 71))   //474 ~ 858 Mhz
           {
               *u32Frequency = 522500L + (u8RF_CH - 27) * 7000L;
           }
           else
           {
               *u32Frequency = 0;
           }
   #endif
        *u16BandWidth = 7000;
           break;
       }

    //-------------------------------------//
    case COUNTRY_TYPE_ITALY:
    {
	 MS_U32 u32FreqTbl[]={177500,186000,194500,203500,212500,219500,226500,233500}; // 5 ~ 12

        if ((u8RF_CH >= 5) && (u8RF_CH <= 12))   //177.5 ~ 226.5 Mhz  6~9 9A~12
        {
            *u32Frequency = u32FreqTbl[u8RF_CH - 5];
    	     *u16BandWidth = 7000;
        }
        else if ((u8RF_CH >= 21) && (u8RF_CH <= 70))   //474 ~ 858 Mhz
        {
            *u32Frequency = 474000L + (u8RF_CH - 21) * 8000L;
	     *u16BandWidth = 8000;
        }
        else
        {
            *u32Frequency = 0;
        }
        break;
    }
    //-------------------------------------//
    case COUNTRY_TYPE_TAIWAN:
        if ((u8RF_CH >= 24) && (u8RF_CH <= 36))   //533 ~ 605 Mhz
        {
            *u32Frequency = 533000L + (u8RF_CH - 24) * 6000L;
            *u16BandWidth = 6000;
        }
        else
        {
            *u32Frequency = 0;
        }
        break;

    //-------------------------------------//
    case COUNTRY_TYPE_OTHER:
        *u32Frequency  = 98000 + (u8RF_CH * 8000L);
        *u16BandWidth = 8000;
    //-------------------------------------//
        break;
    default:
        *u32Frequency = 0;
        break;
    }

    /* Frequency and symbol rate already verified in Scan Menu.
       0 = default value. */
    if (*u32Frequency != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


//--------------------------------------------------------------------------------------------------
/// Get RF channel number by frequency
/// @param  u8Country               \b IN:  country
/// @param  u32Frequency            \b IN:  the frequency of the RF channel
/// @return RF channel number
//--------------------------------------------------------------------------------------------------
MS_U8 MApi_RfChannel_Freq2ChannelNumber(MS_U8 u8Country, MS_U32 u32Frequency)
{
    MS_U8               u8RF_CH = 0;

    switch(u8Country)
    {
    default:
        u8RF_CH = 0;
        break;
    //-------------------------------------//
    case COUNTRY_TYPE_CHILE:
    {
        if (u32Frequency < 473000L)
        {           // 177.143 ~ 473.143
            u8RF_CH = (MS_U8)((u32Frequency - 177000L) / 6000L) + 7;
        }
        else
        {
            u8RF_CH = (MS_U8)((u32Frequency - 473000L) / 6000L) + 14;
        }
        break;
    }

   //-------------------------------------//
    case COUNTRY_TYPE_PERU:
    case COUNTRY_TYPE_BRAZIL:
    {
        if (u32Frequency < 473143L)
        {           // 177.143 ~ 473.143
            u8RF_CH = (MS_U8)((u32Frequency - 177143L) / 6000L) + 7;
        }
        else
        {
            u8RF_CH = (MS_U8)((u32Frequency - 473143L) / 6000L) + 14;
        }
        break;
    }
    //-------------------------------------//
	case COUNTRY_TYPE_ARGENTINA:
    {
        if (u32Frequency ==527000)
        {           // 177.143 ~ 473.143
            u8RF_CH = 14;
        }
		else
		{
			u8RF_CH = 15;
		}

        break;
    }
    //-------------------------------------//
    case COUNTRY_TYPE_HONGKONG:
    case COUNTRY_TYPE_EUROPE:
    case COUNTRY_TYPE_CHINA:
    {
        if (u32Frequency < 474000L)
        {           // 177.5 ~ 226.5
            u8RF_CH = (MS_U8)((u32Frequency - 177500L) / 7000L) + 5;
        }
        else
        {
            u8RF_CH = (MS_U8)((u32Frequency - 474000L) / 8000L) + 21;
        }
        break;
    }
    //-------------------------------------//
    case COUNTRY_TYPE_GERMANY:
    case COUNTRY_TYPE_FRANCE:
    {
        if (u32Frequency < 474000L)
        {           // 177.5 ~ 226.5
            u8RF_CH = (MS_U8)((u32Frequency - 177500L) / 7000L) + 5;
        }
        else
        {
            u8RF_CH = (MS_U8)((u32Frequency - 474000L) / 8000L) + 21;
        }
        break;
    }
    //-------------------------------------//
    case COUNTRY_TYPE_ITALY:
    {
	 MS_U32 u32FreqTbl[]={177500,186000,194500,203500,212500,219500,226500,233500}; // 5 ~ 12
	 MS_U16 i;
        if(u32Frequency < 474000L)
        {
            for(i=0;i<sizeof(u32FreqTbl)/sizeof(MS_U32);i++)
            {
                if(u32Frequency <= u32FreqTbl[i])
                {
                    u8RF_CH = 5 + i;
                    break;
                }
            }
        }
        else
        {
            u8RF_CH = (MS_U8)((u32Frequency - 474000L) / 8000L) + 21;
        }
        break;
    }
    //-------------------------------------//
    case COUNTRY_TYPE_AUSTRALIA:
    {

      #if 0//AUSTRALIA_FREQTBL_NEW
        MS_S8 s8Index = FREQ_TAL_SIZE -1;

        for(s8Index = FREQ_TAL_SIZE -1;s8Index>=0;s8Index--)
        {
              if(FreqTbl[s8Index].u32Freq>0&&u32Frequency >= FreqTbl[s8Index].u32Freq)
              {
                    break;
              }
        }
        if(s8Index >= 0)
        {
              u8RF_CH = (MS_U8)((u32Frequency -FreqTbl[s8Index].u32Freq) / 7000L)+FreqTbl[s8Index].u8CHNo;
        }
        else
        {
             u8RF_CH = FreqTbl[0].u8CHNo;
        }
    #else
        if (u32Frequency < 522500L)
        {           // 177.5 ~ 226.5
            u8RF_CH = (MS_U8)((u32Frequency - 177500L) / 7000L) + 6;
        }
        else
        {
            u8RF_CH = (MS_U8)((u32Frequency - 522500L) / 7000L) + 27;
        }
    #endif
        break;
    }
    case COUNTRY_TYPE_AUSTRALIA_NEW:
    {

      #if 1//AUSTRALIA_FREQTBL_NEW
        MS_S8 s8Index = FREQ_TAL_SIZE -1;

        for(s8Index = FREQ_TAL_SIZE -1;s8Index>=0;s8Index--)
        {
              if(FreqTbl[s8Index].u32Freq>0&&u32Frequency >= FreqTbl[s8Index].u32Freq)
              {
                    break;
              }
        }
        if(s8Index >= 0)
        {
              u8RF_CH = (MS_U8)((u32Frequency -FreqTbl[s8Index].u32Freq) / 7000L)+FreqTbl[s8Index].u8CHNo;
        }
        else
        {
             u8RF_CH = FreqTbl[0].u8CHNo;
        }
    #else
        if (u32Frequency < 522500L)
        {           // 177.5 ~ 226.5
            u8RF_CH = (MS_U8)((u32Frequency - 177500L) / 7000L) + 6;
        }
        else
        {
            u8RF_CH = (MS_U8)((u32Frequency - 522500L) / 7000L) + 27;
        }
    #endif
        break;
    }
    //-------------------------------------//
    case COUNTRY_TYPE_TAIWAN:
    {
        //if (u8RF_CH >= 24 && u8RF_CH <= 36)
        if (u32Frequency >= 533000L && u32Frequency <= 605000L)
        {            //533 ~ 605 Mhz
            u8RF_CH = (MS_U8)((u32Frequency - 533000L) / 6000L) + 24;
        }
        break;
    }
    //-------------------------------------//
    case COUNTRY_TYPE_OTHER:
        u8RF_CH = (MS_U8)((u32Frequency-98000 )/8000) ;
        break;
    //-------------------------------------//
    }

    return u8RF_CH;
}

//--------------------------------------------------------------------------------------------------
/// Get highest RF channel number by country
/// @param  u8Country               \b IN:  country
/// @return the highest RF channel number
//--------------------------------------------------------------------------------------------------
MS_U8 MApi_RfChannel_GetMaxRfChNum(MS_U8 u8Country)
{
    switch(u8Country)
    {
    case COUNTRY_TYPE_CHILE:
        return 79;
    case COUNTRY_TYPE_PERU:
    case COUNTRY_TYPE_BRAZIL:
        return 69;
	case COUNTRY_TYPE_ARGENTINA:
        return 15;
    case COUNTRY_TYPE_HONGKONG:
    case COUNTRY_TYPE_CHINA:
        return 69;
    case COUNTRY_TYPE_EUROPE:
    case COUNTRY_TYPE_GERMANY:
    case COUNTRY_TYPE_ITALY:
    case COUNTRY_TYPE_FRANCE:
        return 70;
    case COUNTRY_TYPE_AUSTRALIA:
        return 71;

    case COUNTRY_TYPE_AUSTRALIA_NEW:
        return 76;


    case COUNTRY_TYPE_TAIWAN:
        return 35;
    case COUNTRY_TYPE_OTHER:
        return 100 ;

    default:
        return 0;
    }
}

//--------------------------------------------------------------------------------------------------
/// Get lowest RF channel number by country
/// @param  u8Country               \b IN:  country
/// @return the lowest RF channel number
//--------------------------------------------------------------------------------------------------
MS_U8 MApi_RfChannel_GetMinRfChNum(MS_U8 u8Country)
{
    switch(u8Country)
    {
    case COUNTRY_TYPE_CHILE:
        return 7;
    //-------------------------//
    case COUNTRY_TYPE_PERU:
    case COUNTRY_TYPE_BRAZIL:
        return 7;
    //-------------------------//
    case COUNTRY_TYPE_ARGENTINA:
        return 14;
    //-------------------------//
#if 1//(MARKET == MARKET_HONGKONG || MARKET == MARKET_CHINA)
    case COUNTRY_TYPE_HONGKONG:
    case COUNTRY_TYPE_CHINA:
    {
        return 21;
    }
#endif
    //-------------------------//
    case COUNTRY_TYPE_EUROPE:
    case COUNTRY_TYPE_GERMANY:
    case COUNTRY_TYPE_ITALY:
    case COUNTRY_TYPE_FRANCE:
    {
#if (DVB_VHF_ENABLE == 1)
        return 5;
#else
        return 21;
#endif
    }
    //-------------------------//
    case COUNTRY_TYPE_AUSTRALIA:
    {
#if (DVB_VHF_ENABLE == 1)
        return 6;
#else
        return 21;
#endif
    }
    //-------------------------//
    case COUNTRY_TYPE_TAIWAN:
    {
        return 24;
    }
    //-------------------------//
    case COUNTRY_TYPE_OTHER:
        return 0;
    default:
        return 0;
    }
}

//--------------------------------------------------------------------------------------------------
/// Get Next RF channel number by country by current Rf Ch
/// @param  u8Country               \b IN:  country
/// @param  u8RF_CH               \b IN:  current RF channel
/// @return the lowest RF channel number
//--------------------------------------------------------------------------------------------------
MS_U8 MApi_RfChannel_GetNextRfChNum(MS_U8 u8Country, MS_U8 u8RF_CH)
{
    //--------------------------------------------
    //wyf 20090324 to avoid invalid channel number when manul scan
    MS_U8 u8MaxRFCh,u8MinRFCh;

    u8MaxRFCh = MApi_RfChannel_GetMaxRfChNum(u8Country);
    u8MinRFCh = MApi_RfChannel_GetMinRfChNum(u8Country);
    if(u8RF_CH == u8MaxRFCh)
    {
       u8RF_CH = u8MinRFCh;
	   return u8RF_CH;
    }
    //---------------------------------------------
    switch (u8Country)
    {
    case COUNTRY_TYPE_CHILE:
    case COUNTRY_TYPE_OTHER:
    case COUNTRY_TYPE_PERU:
    case COUNTRY_TYPE_BRAZIL:
	case COUNTRY_TYPE_ARGENTINA:
    {
        if (u8RF_CH < u8MaxRFCh)
        {
            u8RF_CH++;
        }
        else
        {
            u8RF_CH =  u8MinRFCh;
        }
        return u8RF_CH;
    }
    //------------------------------------------//
#if 1//(MARKET == MARKET_HONGKONG || MARKET == MARKET_CHINA)
    case COUNTRY_TYPE_HONGKONG:
    case COUNTRY_TYPE_CHINA:
    {
        if (u8RF_CH < u8MaxRFCh)
        {
            u8RF_CH++;
        }
        else
        {
            u8RF_CH =  21;
        }

        return u8RF_CH;
    }
#endif
    //------------------------------------------//
    case COUNTRY_TYPE_EUROPE:
    case COUNTRY_TYPE_GERMANY:
    case COUNTRY_TYPE_ITALY:
    case COUNTRY_TYPE_FRANCE:
    {
#if (DVB_VHF_ENABLE == 1)
        if (u8RF_CH >= u8MinRFCh && u8RF_CH < 12)
        {
            u8RF_CH++;
        }
        else if (u8RF_CH >= 12 && u8RF_CH < 21)
        {
            u8RF_CH =  21;
        }
        else if (u8RF_CH >= 21 && u8RF_CH < u8MaxRFCh)
        {
            u8RF_CH++;
        }
        else
        {
            u8RF_CH = u8MinRFCh;
        }

        return u8RF_CH;
#else
        if (u8RF_CH < u8MaxRFCh)
        {
            u8RF_CH++;
        }
        else
        {
            u8RF_CH =  21;
        }

        return u8RF_CH;
#endif
    }
    //------------------------------------------//
    case COUNTRY_TYPE_AUSTRALIA:
    {
 #if 0//AUSTRALIA_FREQTBL_NEW
        if (u8RF_CH < 13)
        {
            u8RF_CH++;
        }
        else if(u8RF_CH >= 13 && u8RF_CH<21)
        {
            u8RF_CH =  21;
        }
        else if (u8RF_CH >= 21 && u8RF_CH < u8MaxRFCh)
        {
            u8RF_CH++;
        }
        else
        {
            u8RF_CH =  u8MinRFCh;
        }
 #else
        if (u8RF_CH < 13)
        {
            u8RF_CH++;
        }
        else if(u8RF_CH >= 13 && u8RF_CH<27)
        {
            u8RF_CH =  27;
        }
        else if (u8RF_CH >= 27 && u8RF_CH < u8MaxRFCh)
        {
            u8RF_CH++;
        }
        else
        {
            u8RF_CH =  u8MinRFCh;
        }
   #endif

        return u8RF_CH;
    }
    case COUNTRY_TYPE_AUSTRALIA_NEW:
    {
 #if 1//AUSTRALIA_FREQTBL_NEW
        if (u8RF_CH < 13)
        {
            u8RF_CH++;
        }
        else if(u8RF_CH >= 13 && u8RF_CH<21)
        {
            u8RF_CH =  21;
        }
        else if (u8RF_CH >= 21 && u8RF_CH < u8MaxRFCh)
        {
            u8RF_CH++;
        }
        else
        {
            u8RF_CH =  u8MinRFCh;
        }
 #else
        if (u8RF_CH < 13)
        {
            u8RF_CH++;
        }
        else if(u8RF_CH >= 13 && u8RF_CH<27)
        {
            u8RF_CH =  27;
        }
        else if (u8RF_CH >= 27 && u8RF_CH < u8MaxRFCh)
        {
            u8RF_CH++;
        }
        else
        {
            u8RF_CH =  u8MinRFCh;
        }
   #endif

        return u8RF_CH;
    }
    //------------------------------------------//
    case COUNTRY_TYPE_TAIWAN:
    {
        if (u8RF_CH >= 24 && u8RF_CH < 35)
        {
            u8RF_CH++;
        }
        else
        {
            u8RF_CH = 24;
        }
        return u8RF_CH;
    }


    default:
        return 0;
    }
}

