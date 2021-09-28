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

#define _MSKEYPAD_C_

#include <common.h>
#include <MsTypes.h>
#include <ShareType.h>
#include <MsIRQ.h>
#include <MsOS.h>

#include <MsUboot.h>
#include "msKeypad.h"
#include "drvSAR.h"

#include <bootlogo/MsPoolDB.h>
#include <stdlib.h>
#include <MsRawIO.h>
#include "keypad_type.h"

#include <MsDebug.h>

static SAR_KpdRegCfg_t m_KpdConfig[5][MAXKEYPADCH] = {
#include "keypad.h"
};

ulong key_layout = 0;

#if KEYPAD_USE_ISR
static MS_S32 u32Mutex_Keypad = 0;
static MS_BOOL bBlockKeypadIsr = FALSE;
static U8 g_u8KeypadKey = 0xFF;
static U8 g_u8KeypadRpt = 0;
static BOOLEAN g_u8KeypadRes=MSRET_ERROR;
static void MDrv_Keypad_Isr(MHAL_SavedRegisters *pHalReg, U32 u32Data);
#endif




static U8 Keypad_Channel[ADC_KEY_CHANNEL_NUM];

#ifndef ADC_KEY_LEVEL_TOLERANCE
#define ADC_KEY_LEVEL_TOLERANCE        0xFF
#endif




static BOOLEAN msKeypad_AllocChan(U8 u8KpdCh);
static U8 msKeypad_GetChanIndex(U8 u8KpdCh);
static BOOLEAN msKeypad_CH_GetKey(U8 Channel, U8 *pkey, U8* pflag);
#if KEYPAD_USE_ISR


static U8 tADCKeyLevel[MAXKEYPADCH][8];
static U8 tADCChannelLevels[MAXKEYPADCH];
static U8 tADCKeyFlag[MAXKEYPADCH][8];

static void msKeypad_Config_Struct(void);
static void msKeypad_Config_Struct(void)
{
   memset (tADCKeyFlag, 0x00, sizeof(char)*32);
   int i = 0;
   int j = 0;

   for (i = 0; i < MAXKEYPADCH; i++)
   {
       for (j = 0; j < 8; j++)
       {

           tADCKeyFlag[i][j] = m_KpdConfig[key_layout][i].u8KeyCode[j];
       }

       tADCChannelLevels[i] = m_KpdConfig[key_layout][i].u8KeyLevelNum;
   }

   memset (tADCKeyLevel, 0x00, sizeof(char)*32);
   for (i = 0; i < MAXKEYPADCH; i++)
   {
       for (j = 0; j < 8; j++)
       {

           tADCKeyLevel[i][j] = m_KpdConfig[key_layout][i].u8KeyThreshold[j];
       }
   }

}
#endif
static BOOLEAN msKeypad_AllocChan(U8 u8KpdCh)
{
    static U8 u8KpdChIdx=0;

    if(u8KpdChIdx>=ADC_KEY_CHANNEL_NUM)
    {
        printf("[msKeypad_Init]: Allocated channel number > %d !!!\n",ADC_KEY_CHANNEL_NUM);
        return MSRET_ERROR;
    }
    Keypad_Channel[u8KpdChIdx++]=u8KpdCh;
    return MSRET_OK;
}

static U8 msKeypad_GetChanIndex(U8 u8KpdCh)
{
    U8 k;

    for(k=0;k<ADC_KEY_CHANNEL_NUM;k++)
    {
        if(Keypad_Channel[k]==u8KpdCh)
            return Keypad_Channel[k];
    }
    return 0xFF;
}

#if KEYPAD_USE_ISR
/******************************************************************************/
///Keypad get ADC Channel value
///@param pkey \b IN ADC Channel
///@param pflag \b IN Pointer to the ADC Channel value
/******************************************************************************/
U8 msKeypad_Get_ADC_Channel(U8 Channel, U8 *pvalue)
{
    *pvalue=MDrv_SAR_Adc_GetValue(Channel);
     return 0;
}

static U8 msKeypad_GetChanLevels(U8 u8KpdCh);
static U8 msKeypad_GetChanLevels(U8 u8KpdCh)
{
    if(u8KpdCh>MAXKEYPADCH)
        return 0;
    return tADCChannelLevels[u8KpdCh];
}

extern void MApi_PNL_SetBackLight(MS_BOOL bEnable);
static BOOLEAN msKeypad_GetKeyCodeISR(U8 *pkey, U8 *pflag)
{
    U8 u8KeypadRes;
    U8 Channel;

    for (Channel=0; Channel<KEYPAD_CHANNEL_SUPPORT; Channel++)
    {
        u8KeypadRes = msKeypad_CH_GetKey(Channel, pkey, pflag);
        //printf("MDrv_Keypad_Isr>>> msKeypad_CH_GetKey[%d] = 0x%02X\n",Channel,*pkey);
        if (u8KeypadRes != MSRET_ERROR)
        {
            return u8KeypadRes;
        }
    }
    return MSRET_ERROR;
}

static void MDrv_Keypad_Isr(MHAL_SavedRegisters *pHalReg, U32 u32Data)
{
    MS_U8 u8KeypadKey=0xFF;
    MS_U8 u8KeypadRpt=0;
    BOOLEAN u8KeypadRes=MSRET_ERROR;

    UNUSED(pHalReg);
    UNUSED(u32Data);

    //printf("MDrv_Keypad_Isr>>> IN\n");
    //##########################
    //
    // Processing current interrupt
    // Get keypad key code + repeat status
    //
    if(bBlockKeypadIsr == FALSE)
    {
        u8KeypadRes = msKeypad_GetKeyCodeISR(&u8KeypadKey,&u8KeypadRpt);
        //msKeypad_Lock();    //No effect in non-os, replace by bBlockKeypadIsr flag
        //if(u8KeypadRes==MSRET_OK)
        if(u8KeypadRes!=MSRET_ERROR)
        {
            g_u8KeypadKey = (u8KeypadRes)? u8KeypadKey : 0xFF;
            g_u8KeypadRpt = (u8KeypadRes)? u8KeypadRpt : 0;
            g_u8KeypadRes = u8KeypadRes;
            if(g_u8KeypadKey==IRKEY_POWER)
            {
                printf("\n[Keypad] Key Code = 0x%02X\n",g_u8KeypadKey);
                if( !(strcmp(getenv("upgrade_mode"), "null")==0) )
                {
                    MApi_PNL_SetBackLight(1);
                }
                MDrv_PM_ActiveStandbyMode(FALSE);
                run_command ("setenv force_onetime_standby", 0);
                saveenv();
            }
        }
        else
        {
            g_u8KeypadRes = u8KeypadRes;
        }
        //msKeypad_UnLock();    //No effect in non-os, replace by bBlockKeypadIsr flag
    }
    //else
    //{
    //    printf("\n\rBlocked!");
    //}

    //##########################
    //
    // Processing next interrupt
    //
    if(u8KeypadRes!=MSRET_ERROR)
    {
        //enable interrupt after get key routine if  result<>MSRET_ERROR
        //Not to clear interrupt to let SAR generate continuous interrupts
        #if 0
        if(u8KeypadRes==MSRET_OK)
            printf("MDrv_Keypad_Isr>>> OK\n");
        else
            printf("MDrv_Keypad_Isr>>> OTHER\n");
        #endif
    }
    else
    {
        //printf("MDrv_Keypad_Isr>>> ERROR\n");
    }

    //enable interrupt
    if(g_u8KeypadKey!=IRKEY_POWER)
    {
       MsOS_EnableInterrupt(E_INT_IRQ_PMSLEEP);
    }
    //clear interrupt
    MDrv_WriteRegBit(REG_SAR_INT_CLR, ENABLE, SAR_INT_CLR_ALL);
    MDrv_WriteRegBit(REG_SAR_INT_CLR, DISABLE, SAR_INT_CLR_ALL);

}

static BOOLEAN msKeypad_CH_GetKey(U8 Channel, U8 *pkey, U8* pflag)
{
    U8 i, j, KEY_LV[MAXKEYPADLVLS], Key_Value, *Keymapping=NULL;
    U8 u8ChIdx=0;

    *pkey = 0xFF;
    *pflag = 0;

    udelay(100*1000); //IMPORTANT!!!

    for(i=0; i<MAXKEYPADLVLS; i++)
        KEY_LV[i] = 0;

    u8ChIdx = msKeypad_GetChanIndex(Channel);

    if(u8ChIdx == 0xFF)
        return MSRET_ERROR;

    Keymapping = (U8*)tADCKeyFlag[u8ChIdx];

    for ( i = 0; i < KEYPAD_STABLE_NUM; i++ )
    {
        msKeypad_Get_ADC_Channel(Channel,&Key_Value);
        for (j=0;j<msKeypad_GetChanLevels(u8ChIdx);j++)
        {
            if (Key_Value < tADCKeyLevel[u8ChIdx][j])
            {
                KEY_LV[j]++;
                break;
            }
        }
    }

    for(i=0; i<msKeypad_GetChanLevels(u8ChIdx); i++)
    {
        if(KEY_LV[i] >= KEYPAD_STABLE_NUM_MIN)
        {
            *pkey = *(Keymapping+i);
            return MSRET_OK;
        }
    }

    return MSRET_ERROR;

}

#else
static BOOLEAN msKeypad_CH_GetKey(U8 Channel, U8 *pkey, U8* pflag)
{
    U8 i, j, Key_Value;
    U8 u8ChIdx = msKeypad_GetChanIndex(Channel);

    if(u8ChIdx == 0xFF)
        return MSRET_ERROR;

    U8 u8KeyLevelNum = m_KpdConfig[key_layout][u8ChIdx].u8KeyLevelNum;
    U8 KEY_LV[u8KeyLevelNum];
    memset(KEY_LV, 0x0, u8KeyLevelNum);

    *pkey = 0xFF;
    *pflag = 0;

    for ( i = 0; i < KEYPAD_STABLE_NUM; i++ )
    {
        Key_Value = MDrv_SAR_Adc_GetValue(m_KpdConfig[key_layout][u8ChIdx].u8SARChID);
        for (j=0;j<u8KeyLevelNum;j++)
        {
            if (Key_Value < m_KpdConfig[key_layout][u8ChIdx].u8KeyThreshold[j])
            {
                if((m_KpdConfig[key_layout][u8ChIdx].u8KeyThreshold[j] - Key_Value) <= ADC_KEY_LEVEL_TOLERANCE)
        		{
	                KEY_LV[j]++;
	                break;
        		}
            }
        }
    }

    for(i=0; i<u8KeyLevelNum; i++)
    {
        if(KEY_LV[i] > KEYPAD_STABLE_NUM_MIN)
        {
            *pkey = m_KpdConfig[key_layout][u8ChIdx].u8KeyCode[i];
            return MSRET_OK;
        }
    }
    return MSRET_ERROR;

}
#endif

void msKeypad_Init(void)
{
    char *s = NULL;
    char *env = NULL;
	char *key_layout_str = NULL;
    U32 keypad_offset = 0;
    U32 keypad_size = 0;
    int i = 0, k = 0;
    s = getenv("keypad_select");
    if(s != NULL)
    {
        env = getenv("keypad_offset");
        if(env == NULL)
        {
            UBOOT_ERROR("get env 'keypad_offset' fail\n");
        }
        else
        {
            keypad_offset = simple_strtol(env, NULL, 16);
        }
        env = getenv("keypad_size");
        if(env == NULL)
        {
            UBOOT_ERROR("get env 'keypad_size' fail\n");
        }
        else
        {
            keypad_size = simple_strtol(env, NULL, 16);
        }
        UBOOT_DEBUG("keypad_offset: 0x%x, keypad_size: 0x%x\n", keypad_offset, keypad_size);
        if((keypad_size != 0) && (keypad_offset != 0))
        {
            if(-1 == raw_read(&m_KpdConfig, keypad_offset, keypad_size))
            {
                UBOOT_ERROR("ERROR: Read_FlashDataToAddr fail...>>>\n");
            }
        }
    }

	key_layout_str = getenv("key_layout");
	if (key_layout_str!=NULL)
		key_layout = simple_strtoul(key_layout_str, NULL, 10);
	else
		key_layout = 0;
    for(i=0; i<MAXKEYPADCH; i++)
    {
        UBOOT_DEBUG("\nKeypadInfoSet[%d] = {%s, 0x%02x, 0x%02x, 0x%02x, 0x%02x}\n", \
               i, (m_KpdConfig[key_layout][i].bEnable != TRUE) ? "Disable" : "Enable", \
               m_KpdConfig[key_layout][i].u8SARChID, \
               m_KpdConfig[key_layout][i].tSARChBnd.u8UpBnd, \
               m_KpdConfig[key_layout][i].tSARChBnd.u8LoBnd, \
               m_KpdConfig[key_layout][i].u8KeyLevelNum);
        for(k=0; k<m_KpdConfig[key_layout][i].u8KeyLevelNum; k++)
        {
            UBOOT_DEBUG("u8KeyThreshold[%d]: 0x%02x\n", k, m_KpdConfig[key_layout][i].u8KeyThreshold[k]);
        }
        for(k=0; k<m_KpdConfig[key_layout][i].u8KeyLevelNum; k++)
        {
            UBOOT_DEBUG("u8KeyCode[%d]: 0x%02x\n", k, m_KpdConfig[key_layout][i].u8KeyCode[k]);
        }
    }
#if KEYPAD_USE_ISR
    msKeypad_Config_Struct();
#endif
    MS_U8 u8ChEnb[MAXKEYPADCH]={m_KpdConfig[key_layout][KEYPADCH1].bEnable,
                                m_KpdConfig[key_layout][KEYPADCH2].bEnable,
                                m_KpdConfig[key_layout][KEYPADCH3].bEnable,
                                m_KpdConfig[key_layout][KEYPADCH4].bEnable};

    MS_U8 u8UpBnd[MAXKEYPADCH]={m_KpdConfig[key_layout][KEYPADCH1].tSARChBnd.u8UpBnd,
                                m_KpdConfig[key_layout][KEYPADCH2].tSARChBnd.u8UpBnd,
                                m_KpdConfig[key_layout][KEYPADCH3].tSARChBnd.u8UpBnd,
                                m_KpdConfig[key_layout][KEYPADCH4].tSARChBnd.u8UpBnd};

    MS_U8 u8LoBnd[MAXKEYPADCH]={m_KpdConfig[key_layout][KEYPADCH1].tSARChBnd.u8LoBnd,
                                m_KpdConfig[key_layout][KEYPADCH2].tSARChBnd.u8LoBnd,
                                m_KpdConfig[key_layout][KEYPADCH3].tSARChBnd.u8LoBnd,
                                m_KpdConfig[key_layout][KEYPADCH4].tSARChBnd.u8LoBnd};

    MS_U8 u8Idx;

	//for utopia driver for sar mmio base init
	MDrv_SAR_Kpd_MMIO_Init();

    memset(Keypad_Channel, 0xFF, ADC_KEY_CHANNEL_NUM);

    for(u8Idx=0; u8Idx < MAXKEYPADCH; u8Idx++)
    {
        if(u8ChEnb[u8Idx]==ENABLE)
        {
			MDrv_SAR_Kpd_CfgChannelBound(m_KpdConfig[key_layout][u8Idx].u8SARChID, u8UpBnd[u8Idx], u8LoBnd[u8Idx]);
            msKeypad_AllocChan(u8Idx);
        }
    }

    MDrv_WriteByte(REG_SAR_CTRL0, SAR_MODE_FREERUN);
    MDrv_WriteByte(REG_SAR_CTRL0, MDrv_ReadByte(REG_SAR_CTRL0)&(~SAR_DIGITAL_PWRDN));
    MDrv_WriteByte(REG_SAR_CTRL1, MDrv_ReadByte(REG_SAR_CTRL1)&(~SAR_ADC_PWRDN));
    MDrv_WriteByte(REG_SAR_CTRL1, MDrv_ReadByte(REG_SAR_CTRL1)|(SAR_ADC_FREERUN));
    MDrv_WriteByte(REG_SAR_SELCH, MDrv_ReadByte(REG_SAR_SELCH)&(~SAR_NCH_EN));
    MDrv_WriteByte(REG_SAR_CKSAMP_PRD, 0x05);   //sample period
#if defined(CONFIG_MSTAR_BD_MST176A_D01A_S_NIKE)
    MDrv_WriteByte(REG_SAR_TEST0, MDrv_ReadByte(REG_SAR_TEST0)|(BIT6));   //Full scale=3.3V
#elif defined(CONFIG_MSTAR_BD_MST049B_10AQV_NIKE) || defined(CONFIG_MSTAR_BD_MST049B_10AQV_NIKE_U)
    MDrv_WriteByte(REG_SAR_TEST0, MDrv_ReadByte(REG_SAR_TEST0)|(BIT6));   //Full scale=3.3V
#else
    //do nothing
#endif

    for(u8Idx=0; u8Idx < MAXKEYPADCH; u8Idx++)
    {
        if(u8ChEnb[u8Idx]==ENABLE)
        {
            switch (m_KpdConfig[key_layout][u8Idx].u8SARChID)
            {
                case 0:
                    //select pad as analog input
                    MDrv_WriteByte(REG_SAR_AISEL, MDrv_ReadByte(REG_SAR_AISEL)|(SAR_AISEL_CH0_MSK));
                    //select pad direction as input mode
                    MDrv_WriteByte(REG_SAR_GPIOOEN, MDrv_ReadByte(REG_SAR_GPIOOEN)|(SAR_GPIOOEN_CH0_MSK));
                    break;

                case 1:
                    //select pad as analog input
                    MDrv_WriteByte(REG_SAR_AISEL, MDrv_ReadByte(REG_SAR_AISEL)|(SAR_AISEL_CH1_MSK));
                    //select pad direction as input mode
                    MDrv_WriteByte(REG_SAR_GPIOOEN, MDrv_ReadByte(REG_SAR_GPIOOEN)|(SAR_GPIOOEN_CH1_MSK));
                    break;

                case 2:
                    //select pad as analog input
                    MDrv_WriteByte(REG_SAR_AISEL, MDrv_ReadByte(REG_SAR_AISEL)|(SAR_AISEL_CH2_MSK));
                    //select pad direction as input mode
                    MDrv_WriteByte(REG_SAR_GPIOOEN, MDrv_ReadByte(REG_SAR_GPIOOEN)|(SAR_GPIOOEN_CH2_MSK));
                    break;

                case 3:
                    //select pad as analog input
                    MDrv_WriteByte(REG_SAR_AISEL, MDrv_ReadByte(REG_SAR_AISEL)|(SAR_AISEL_CH3_MSK));
                    //select pad direction as input mode
                    MDrv_WriteByte(REG_SAR_GPIOOEN, MDrv_ReadByte(REG_SAR_GPIOOEN)|(SAR_GPIOOEN_CH3_MSK));
                    break;
            }
        }
    }

    #if KEYPAD_USE_ISR
    //irq : hook isr for pm_sleep
    MsOS_AttachInterrupt(E_INT_IRQ_PMSLEEP, (InterruptCb)MDrv_Keypad_Isr);
    //irq : enable interrupt
    MsOS_EnableInterrupt(E_INT_IRQ_PMSLEEP);
    //pm_sleep : un-mask sar wakeup event
    MDrv_WriteRegBit(REG_PM_WK_IRQ_MSK, DISABLE, PM_WK_IRQ_SAR);

    //sar : un-mask
    for(u8Idx=0; u8Idx < MAXKEYPADCH; u8Idx++)
    {
        if(u8ChEnb[u8Idx]==ENABLE)
        {
            switch (u8Idx)
            {
                case 0: MDrv_WriteRegBit(REG_SAR_INT_MASK, 0, BIT0); break;
                case 1: MDrv_WriteRegBit(REG_SAR_INT_MASK, 0, BIT1); break;
                case 2: MDrv_WriteRegBit(REG_SAR_INT_MASK, 0, BIT2); break;
                case 3: MDrv_WriteRegBit(REG_SAR_INT_MASK, 0, BIT3); break;
                default: break;
            }
        }
    }
    //sar : clear interrupt
    MDrv_WriteRegBit(REG_SAR_INT_CLR, ENABLE, SAR_INT_CLR_ALL);
    MDrv_WriteRegBit(REG_SAR_INT_CLR, DISABLE, SAR_INT_CLR_ALL);
    //sw pack for new irq structure //IMPORTANT!!!
    //MDrv_WriteRegBit(REG_SAR_SW_PACK, ENABLE, SAR_IRQ_CTRL);
    //create mutex
    if(u32Mutex_Keypad<=0)
    {
        //msKeypad_InitLock();
    }
    #else

    //disable sar mask
    MDrv_WriteByte(REG_SAR_INT_MASK, MDrv_ReadByte(REG_SAR_INT_MASK)|(SAR_INT_MASK));

    #endif
}

/******************************************************************************/
///Keypad get key value and repeat flag
///@param pkey \b IN return the key value(The same as Irda key value)
///@param pflag \b IN return the repeat flag(1:Repeat)
/******************************************************************************/
BOOLEAN msKeypad_GetKey(U8 *pkey, U8 *pflag)
{
    U8 Channel;

    for (Channel=0; Channel<MAXKEYPADCH; Channel++)
    {
        if (msKeypad_CH_GetKey(Channel, pkey, pflag))
        {
            return MSRET_OK;
        }
    }
    return MSRET_ERROR;
}

#undef _MSKEYPAD_C_

