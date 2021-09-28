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
//#include <math.h>
#include "MsCommon.h"
#include "drvIIC.h"
#include "Board.h"
#include "drvTuner_AV2012.h"
//#include "drvTuner.h"
//#include "drvTunerNull.h"


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define _GET_TUNER_ENTRY_NODE(var, Name) var##Name
#define GET_TUNER_ENTRY_NODE(Name) _GET_TUNER_ENTRY_NODE(tuner_entry_,Name)

/// Define extension function types
typedef enum
{
    TUNER_EXT_FUNC_STANDBY,
    TUNER_EXT_FUNC_WAKEUP,
    TUNER_EXT_FUNC_LOOP_THROUGH,
    TUNER_EXT_FUNC_POWER_ON_OFF,
    TUNER_EXT_FUNC_GET_POWER_LEVEL,
    TUNER_EXT_FUNC_SET_TUNER_MODE
} TUNER_EXT_FUNCTION_TYPE;

#if (FRONTEND_TUNER_TYPE==TUNER_AV2012 || FRONTEND_TUNER_TYPE==TUNER_AV2011)

#define  TUNER_AV2012_SLAVE_ID      0xC0//0xC6
#define  TUNER_CRYSTAL_FREQ         27
static MS_U8 _u8SlaveID = TUNER_AV2012_SLAVE_ID;

#if 1//(FRONTEND_TUNER_TYPE==TUNER_AV2011)

MS_U8 TunerInitialSetting[2][42]=
{//test for India
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29},
    {0x38, 0x00, 0x00, 0x50, 0x1F, 0xA3, 0xFD, 0x00, 0x46, 0x82, 0x88, 0xB4, 0x96, 0x40, 0x94, 0x4A, 0x66, 0x40, 0x80, 0x2B, 0x6A, 0x50, 0x91, 0x27, 0x8F, 0xCC, 0x21, 0x10, 0x80, 0x00, 0xF5, 0x7F, 0x4A, 0x9B, 0xE0, 0xE0, 0x36, 0xCC, 0xAB, 0x97, 0xC5, 0xA8}
};
/*
MS_U8 TunerInitialSetting[2][42]=
{
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,  0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29},
    {0x38, 0x00, 0x00, 0x50, 0x1F, 0xA3, 0xFD, 0x58, 0x0E, 0x82, 0x88, 0xB4, 0xD6, 0x40, 0x94,0x9A, 0x66, 0x40, 0x80, 0x2B, 0x6A, 0x50, 0x91, 0x27, 0x8F, 0xCC, 0x21, 0x10, 0x80,0x02, 0xF5, 0x7F, 0x4A, 0x9B, 0xE0, 0xE0, 0x36, 0x00, 0xAB, 0x97, 0xC5, 0xA8}
};
*/
#else
MS_U8 TunerInitialSetting[2][42]=
{
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,  0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29},
    {0x38, 0x00, 0x00, 0x50, 0x1F, 0xA3, 0xFD, 0x58, 0xC6, 0x82, 0x88, 0xB4, 0x96, 0x40, 0x94, 0x4A, 0x66, 0x40, 0x80, 0x2B, 0x6A, 0x50, 0x91, 0x27, 0x8F, 0xCC, 0x21, 0x10, 0x80, 0x02, 0xF5, 0x7F, 0x4A, 0x9B, 0xE0, 0xE0, 0x36, 0x02, 0xAB, 0x97, 0xC5, 0xA8}
};
#endif
MS_BOOL AV2012_WriteReg(MS_U8 u8SlaveID, MS_U8 u8Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8Value[2];
    u8Value[0]=u8Addr;
    u8Value[1]=u8Data;
    bRet&=MDrv_IIC_Write(u8SlaveID, 0, 0, u8Value, 2);
    return bRet;
}
MS_BOOL AV2012_ReadReg(MS_U8 u8SlaveID, MS_U8 u8Addr, MS_U8 *u8Data)
{
    MS_BOOL bRet=TRUE;
    bRet&=MDrv_IIC_Write(u8SlaveID, 0, 0, &u8Addr, 1);
    bRet&=MDrv_IIC_Read(u8SlaveID, 0, 0, u8Data, 1);
    return bRet;
}

void AV2012_SlaveID_Check(void)
{
     MS_U8 regValue;
     
      _u8SlaveID = 0xC0;
      do
      {
          regValue=(char) (0x38);
          if(AV2012_WriteReg(_u8SlaveID,0,regValue))
          {
               regValue = 0;
               if(AV2012_ReadReg(_u8SlaveID,0,&regValue))
               {
                     if(regValue == 0x38)
                     {
                         break;
                     }
               }
          }
          _u8SlaveID += 0x02;
      }while(_u8SlaveID <= 0xC6);
      if(_u8SlaveID > 0xC6)
      {
           _u8SlaveID = TUNER_AV2012_SLAVE_ID;
      }
}

MS_BOOL MDrv_Tuner_AV2012_PowerOnOff(MS_U8 u8TunerIndex,MS_BOOL bPowerOn)
{
    return TRUE;
}

MS_BOOL MDrv_Tuner_AV2012_Init(MS_U8 u8TunerIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U8 index;

    AV2012_SlaveID_Check();
    
    for (index=0; index < 12; index++)
    {
        bRet&=AV2012_WriteReg(_u8SlaveID, TunerInitialSetting[0][index], TunerInitialSetting[1][index]);
    }
    MsOS_DelayTask(1);
    for (index=13; index < 42; index++)
    {
        bRet&=AV2012_WriteReg(_u8SlaveID, TunerInitialSetting[0][index], TunerInitialSetting[1][index]);
    }
    MsOS_DelayTask(1);
    bRet&=AV2012_WriteReg(_u8SlaveID, TunerInitialSetting[0][12], TunerInitialSetting[1][12]);
    MsOS_DelayTask(100);
    for (index=0; index < 12; index++)
    {
        bRet&=AV2012_WriteReg(_u8SlaveID, TunerInitialSetting[0][index], TunerInitialSetting[1][index]);
    }
    MsOS_DelayTask(1);
    for (index=13; index < 42; index++)
    {
        bRet&=AV2012_WriteReg(_u8SlaveID, TunerInitialSetting[0][index], TunerInitialSetting[1][index]);
    }
    MsOS_DelayTask(1);
    bRet&=AV2012_WriteReg(_u8SlaveID, TunerInitialSetting[0][12], TunerInitialSetting[1][12]);
    MsOS_DelayTask(50);
    return bRet;
}

MS_BOOL MDrv_Tuner_AV2012_SetFreq(MS_U8 u8TunerIndex, MS_U32 u16CenterFreq, MS_U32 u32SymbolRate_Hz)
{
    MS_BOOL bRet=TRUE;
    MS_BOOL bAutoScan=FALSE;
    MS_U32 u32FracN;
    MS_U32 BW;
    MS_U32 BF;
    MS_U8 u8Reg[7];

    if (u32SymbolRate_Hz == 0 || u32SymbolRate_Hz == 45000) //auto-scan mode
    {
	    bAutoScan = TRUE;
    }
    //MsOS_DelayTask(50);
    u32FracN = (u16CenterFreq + TUNER_CRYSTAL_FREQ/2)/TUNER_CRYSTAL_FREQ;
    if(u32FracN > 0xff)
    {
   	   u32FracN = 0xff;
    }
    u8Reg[0]=(MS_U8) (u32FracN & 0xff);
    u32FracN = (u16CenterFreq<<17)/TUNER_CRYSTAL_FREQ;
    u32FracN = (u32FracN & 0x1ffff);
    u8Reg[1]=(MS_U8) ((u32FracN>>9)&0xff);
    u8Reg[2]=(MS_U8) ((u32FracN>>1)&0xff);
    u8Reg[3]=(MS_U8) (((u32FracN<<7)&0x80) | 0x50); // default is 0x50
    // Channel Filter Bandwidth Setting.
    if(bAutoScan==TRUE)//requested by BB
    {
        u8Reg[5] = 0xA3; //BW=27MHz
    }
    else
    {
    	if(u32SymbolRate_Hz>40000)//for VideoCon    	
    	BW = u32SymbolRate_Hz;
    	else
    	{
   	    // rolloff is 35%
   	    BW = u32SymbolRate_Hz*135/200;
#if 0 //test for PALAPA 3750H3125  	    
   	    // monsen 20080726, BB request low IF when sym < 6.5MHz
   	    // add 6M when Rs<6.5M,
   	    if(u32SymbolRate_Hz<6500)
   	    {
   	    	BW = BW + 6000;
   	    }
   	    	// add 2M for LNB frequency shifting
   	    BW = BW + 2000;
#endif
   	    	// add 8% margin since fc is not very accurate
   	    BW = BW*108/100;
   	  }
   	    	// Bandwidth can be tuned from 4M to 40M
   	    if( BW< 4000)
   	    {
   	    	BW = 4000;
   	    }
   	    if( BW> 40000)
   	    {
   	    	BW = 40000;
   	    }
   	    BF = (BW*127 + 21100/2) / (21100); // BW(MHz) * 1.27 / 211KHz
   	    u8Reg[5] = (MS_U8)BF;
    }
    // Sequence 4
    // Send Reg0 ->Reg4
    MsOS_DelayTask(5);
    bRet&=AV2012_WriteReg(_u8SlaveID, 0x00, u8Reg[0]);
    bRet&=AV2012_WriteReg(_u8SlaveID, 0x01, u8Reg[1]);
    bRet&=AV2012_WriteReg(_u8SlaveID, 0x02, u8Reg[2]);
    bRet&=AV2012_WriteReg(_u8SlaveID, 0x03, u8Reg[3]);
    MsOS_DelayTask(5);
    // Sequence 5
    // Send Reg5
    bRet&=AV2012_WriteReg(_u8SlaveID, 0x05, u8Reg[5]);
    MsOS_DelayTask(5);
    // Fine-tune Function Control
    //Tuner fine-tune gain function block. bit2.
    //not auto-scan case. enable block function. FT_block=1, FT_EN=1
    if (bAutoScan==FALSE)
    {
         u8Reg[6] = 0x06;
         bRet&=AV2012_WriteReg(_u8SlaveID, 0x25, u8Reg[6]);
         MsOS_DelayTask(5);
         //Disable RFLP at Lock Channel sequence after reg[37]
         //RFLP=OFF at Lock Channel sequence
         // RFLP can be Turned OFF, only at Receving mode.
		 u8Reg[7] = 0xD6;
         bRet&=AV2012_WriteReg(_u8SlaveID, 0x0C, u8Reg[7]);
    }
    return bRet;
}
MS_BOOL MDrv_Tuner_AV2012_GetLock(MS_U8 u8TunerIndex)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8Data;

    bRet&=AV2012_ReadReg(_u8SlaveID, 0x0B, &u8Data);
    if (bRet==FALSE)
    {
        return bRet;
    }
    else
    {
        if ((u8Data&0x03)!=0x03)
        {
            bRet=FALSE;
        }
    }
    return bRet;
}

MS_BOOL TUNER_AV2012_Extension_Function(MS_U8 u8TunerIndex, TUNER_EXT_FUNCTION_TYPE fuction_type, void *data)
{
    switch(fuction_type)
    {
        default:
            printf("Request extension function (%x) does not exist\n",fuction_type);
            return TRUE;
    }

}

/*
DRV_TUNER_TABLE_TYPE GET_TUNER_ENTRY_NODE(TUNER_AV2012) =//&*&*&*j1 DDI_DRV_TUNER_TABLE_ENTRY(tunertab) =
{
    .name               = "TUNER_AV2012",          // demod system name
    .data               = TUNER_AV2012,            // private data value
    .Init               = MDrv_Tuner_AV2012_Init,
    .SetFreq            = MDrv_Tuner_AV2012_SetFreq,
   // .SetFreqS2          = MDrv_Tuner_Null_SetFreq_S2,
    .GetLock            = MDrv_Tuner_AV2012_GetLock,
    .SetTuner           = MDrv_Tuner_Null_SetTuner,
    .CheckExist         = MDrv_Tuner_Null_CheckExist,
    .Extension_Function = TUNER_AV2012_Extension_Function
};
*/

#endif
