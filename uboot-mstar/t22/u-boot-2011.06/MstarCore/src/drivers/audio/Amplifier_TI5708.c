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
 //wjq20100112>>>
#include <common.h>
#include <audio/Amplifier_TI5708.h>
#include <linux/types.h>
//#define code

//#if ENABLE_TI5708_DAP

//BOOL MSrv_dev5708Check(void)
//{
 //   if (SW_IIC_AccessStart(TAS570B_I2C_ADDR, 1))
   //     return TRUE;
   // return FALSE;
//}
//MAPI_U8  g_bTas5708bInstalled;//2008-07-24
//MAPI_U8  g_bTas5708Init;//2008-07-24

//MAPI_U8  g_ubCheckTas5708bFinishedCounter ;		// Seconed:Unit
//MAPI_U8  g_bNeedCheckTas5708b;		// Seconed:ms
#define TAS5708_ADDR     0x36
#define TAS5708_BUSID     (((U16) E_I2C_BUS_SYS << 8) | TAS5708_ADDR)

MAPI_U8  g_ucTas5708RegDate[20];
 MAPI_U8  g_ucTas5708RegDateread[20];

MAPI_U8  g_ucTas5708RegAddr;


MAPI_U8 Amp1[]={1,0x1B,0x00};			//X1B 00
MAPI_U8 Amp2[]={1,0x06,0x00};		//0x3F
MAPI_U8 Amp3[]={1,0x05,0x00};		//X05 00
MAPI_U8 Amp4[]={1,0x07,0x22};		//0x37


MAPI_U8 Amp5[]={4,0x50,0x00,0x00,0x00,0x00};
MAPI_U8 Amp6[]={20,0x29,0x00,  0x88,  0xB5,  0xAE,  0x0F,  0x4C,  0x87,  0x81,  0x00,  0x59, 0x82,  0x17,  0x00,  0xB3,  0x78,  0x7F,  0x0F,  0x9D,  0xC8,  0x3B,};		//0x00,0x7D,0xE2,0x80,0x0F,0x04,0x3B,0x00,0x00,0x7D,0xE2,0x80,0x00,0xFB,0xBC,0x0E,0x0F,0x84,0x32,0x0E
MAPI_U8 Amp7[]={20,0x2A,0x00,  0xA1,  0xE3,  0x80,  0x0F,  0xAC,  0xE4,  0xDB,  0x00,  0x1A,  0x02,  0xE5,  0x00,  0x53,  0x1B,  0x25,  0x0F,  0xC4,  0x19,  0x9A};		//0x00,0x7D,0xE2,0x80,0x0F,0x04,0x3B,0x00,0x00,0x7D,0xE2,0x80,0x00,0xFB,0xBC,0x0E,0x0F,0x84,0x32,0x0E
MAPI_U8 Amp8[]={20,0x2B,0x00,  0x7C,  0xD5,  0x0F,  0x0F,  0x22,  0x20,  0xA0,  0x00,  0x6D,  0x79,  0x96,  0x00,  0xDD,  0xDF,  0x60,  0x0F,  0x95,  0xB1,  0x5A };		//0x00,0x80,0x69,0x58,0x0F,0x02,0x10,0xC5,0x00,0x7D,0x97,0xEF,0x00,0xFD,0xEF,0x3B,0x0F,0x81,0xFE,0xB8
MAPI_U8 Amp9[]={20,0x2C, 0x00,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00};		//0x00,0x95,0x0D,0x58,0x0F,0x71,0x3C,0x0C,0x00,0x34,0xD9,0x44,0x00,0x8E,0xC3,0xF4,0x0F,0xB6,0x19,0x63
MAPI_U8 Amp10[]={20,0x2D, 0x00,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00};		//0x00,0x99,0x3F,0x11,0x0F,0xCE,0x88,0xA9,0x00,0x25,0xE0,0x51,0x00,0x31,0x77,0x57,0x0F,0xC0,0xE0,0x9D
MAPI_U8 Amp11[]={20,0x2E,  0x00,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00};		//0x00,0x7A,0xCA,0x47,0x0F,0x21,0x5E,0x9B,0x00,0x6D,0x66,0xEB,0x00,0xDE,0xA1,0x65,0x0F,0x97,0xCE,0xCD
MAPI_U8 Amp12[]={20,0x2F, 0x00,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00};		//0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

MAPI_U8 Amp13[]={20,0x30,0x00,  0x88,  0xB5,  0xAE,  0x0F,  0x4C,  0x87,  0x81,  0x00,  0x59, 0x82,  0x17,  0x00,  0xB3,  0x78,  0x7F,  0x0F,  0x9D,  0xC8,  0x3B,};		//0x00,0x7D,0xE2,0x80,0x0F,0x04,0x3B,0x00,0x00,0x7D,0xE2,0x80,0x00,0xFB,0xBC,0x0E,0x0F,0x84,0x32,0x0E
MAPI_U8 Amp14[]={20,0x31,0x00,  0xA1,  0xE3,  0x80,  0x0F,  0xAC,  0xE4,  0xDB,  0x00,  0x1A,  0x02,  0xE5,  0x00,  0x53,  0x1B,  0x25,  0x0F,  0xC4,  0x19,  0x9A};		//0x00,0x7D,0xE2,0x80,0x0F,0x04,0x3B,0x00,0x00,0x7D,0xE2,0x80,0x00,0xFB,0xBC,0x0E,0x0F,0x84,0x32,0x0E
MAPI_U8 Amp15[]={20,0x32,0x00,  0x7C,  0xD5,  0x0F,  0x0F,  0x22,  0x20,  0xA0,  0x00,  0x6D,  0x79,  0x96,  0x00,  0xDD,  0xDF,  0x60,  0x0F,  0x95,  0xB1,  0x5A };		//0x00,0x80,0x69,0x58,0x0F,0x02,0x10,0xC5,0x00,0x7D,0x97,0xEF,0x00,0xFD,0xEF,0x3B,0x0F,0x81,0xFE,0xB8
MAPI_U8 Amp16[]={20,0x33, 0x00,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00};		//0x00,0x95,0x0D,0x58,0x0F,0x71,0x3C,0x0C,0x00,0x34,0xD9,0x44,0x00,0x8E,0xC3,0xF4,0x0F,0xB6,0x19,0x63
MAPI_U8 Amp17[]={20,0x34, 0x00,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00};		//0x00,0x99,0x3F,0x11,0x0F,0xCE,0x88,0xA9,0x00,0x25,0xE0,0x51,0x00,0x31,0x77,0x57,0x0F,0xC0,0xE0,0x9D
MAPI_U8 Amp18[]={20,0x35,  0x00,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00};		//0x00,0x7A,0xCA,0x47,0x0F,0x21,0x5E,0x9B,0x00,0x6D,0x66,0xEB,0x00,0xDE,0xA1,0x65,0x0F,0x97,0xCE,0xCD
MAPI_U8 Amp19[]={20,0x36, 0x00,  0x80,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00};		//0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00



MAPI_U8 Amp20[]={8,0x3A, 0x00,  0x02,  0xA3,  0x9A,  0x00,  0x7D,  0x5C,  0x65};		//0x00,0x00,0x94,0x42,0x00,0x7F,0x6B,0xB9
MAPI_U8 Amp21[]={8,0x3B, 0x00,  0x02,  0xA3,  0x9A,  0x00,  0x7D,  0x5C,  0x65 };		//0x00,0x00,0x4A,0x36,0x00,0x7F,0xB5,0xC5
MAPI_U8 Amp22[]={8,0x3C,0x00,  0x00,  0x06,  0xd3,  0x00,  0x7f,  0xf9,  0x2c};		//0x00,0x00,0x4A,0x36,0x00,0x7F,0xB5,0xC5
MAPI_U8 Amp23[]={4,0x40,0xFD,  0x8C,  0xD1,  0xEA};		//0xFC,0xC2,0xD8,0xC5
MAPI_U8 Amp24[]={4,0x41, 0x0F,  0x90,  0x00,  0x00};		//0x0F,0xC0,0x00,0x00
MAPI_U8 Amp25[]={4,0x42, 0x00,  0x08,  0x42,  0x10};		//0x00,0x08,0x42,0x10
MAPI_U8 Amp26[]={4,0x46,0x00,  0x00,  0x00,  0x01};		//0x00,0x00,0x00,0x03

#define AmpInitRegListNum  26

MAPI_U8 * AmpInitRegList[AmpInitRegListNum]=
{
	Amp1,
	Amp2,
	Amp3,
	Amp4,
	Amp5,
	Amp6,
	Amp7,
	Amp8,
	Amp9,
	Amp10,
	Amp11,
	Amp12,
	Amp13,
	Amp14,
	Amp15,
	Amp16,
	Amp17,
	Amp18,
	Amp19,
	Amp20,
	Amp21,
	Amp22,
	Amp23,
	Amp24,
	Amp25,
	Amp26,
};



MAPI_BOOL Amplifier_TI5708_Init(void)
{
 	MAPI_U8 i=0;
	MAPI_U8 j=0;

	//if (!g_bTas5708bInstalled)
	//	return;
	//===========================================
    Audio_Amplifier_ON();//mdrv_gpio_set_low(12) ;   // must be set 1 for datesheet
//    	usleep(1*1000);
    msAPI_Timer_Delayms(2);
    Audio_Amplifier_RST_ON();//mdrv_gpio_set_high(8) ;
//    	usleep(1*10000);
    msAPI_Timer_Delayms(10);
    Audio_Amplifier_RST_OFF();//mdrv_gpio_set_low(8) ;
//    	usleep(1*1000);
    msAPI_Timer_Delayms(15);//must >10

	 for (i=0;i<AmpInitRegListNum;i++)
	 {
 		MAPI_U8 * Pstr;
		Pstr=AmpInitRegList[i];

		g_ucTas5708RegAddr = Pstr[1];
		for(j=0;j<Pstr[0];j++)
		{
			g_ucTas5708RegDate[j]=Pstr[j+2];
		}

        #if 0
		mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_4);
   		if(iptr->WriteBytes(1, &g_ucTas5708RegAddr, Pstr[0], g_ucTas5708RegDate) == MAPI_FALSE)
		 printf("5708write fail = %d\n",i);
        #else
		if (MApi_SWI2C_WriteBytes(TAS5708_BUSID,1,&g_ucTas5708RegAddr,Pstr[0],g_ucTas5708RegDate) == FALSE)
		    printf("5708write fail = %d\n",i);
		#endif
		if (i==0)
		{
		    	//usleep(50*1000);
		    	msAPI_Timer_Delayms(50);
		}
		else if (i<=4)
		{
		    	//usleep(1*1000);
		    	msAPI_Timer_Delayms(1);
		}

	 }

	//g_bTas5708Init=TRUE;
	return MAPI_TRUE;
}

MAPI_U8 AmpmasterVolume_ATV[]={1,0x07,0x37};
MAPI_U8 AmpmasterVolume_OTHER[]={1,0x07,0x06};		  //0x3a

MAPI_BOOL Amplifier_TI5708_VolumeForSource(void)
{
	MAPI_U8 j=0;
       MAPI_U8 * Pstr;
//	MAPI_U8    u8Data = 0;

	//if (!g_bTas5708bInstalled)
	//	return;

	//===========================================
	   //  if (IsATVInUse()  )
	    // 	{
		//	Pstr=AmpmasterVolume_ATV;
	     //	}
	   //  else
	    // 	{
		//	Pstr=AmpmasterVolume_OTHER;
	    // 	}

	Pstr=AmpmasterVolume_OTHER;
       g_ucTas5708RegAddr = Pstr[1];

	for(j=0;j<Pstr[0];j++)
	{
		g_ucTas5708RegDate[j]=Pstr[j+2];
	}

    #if 0
	 mapi_i2c *iptr = mapi_i2c::GetI2C_Dev(DEMOD_DYNAMIC_SLAVE_ID_4);
       if(iptr->WriteBytes(1, &g_ucTas5708RegAddr, 1, g_ucTas5708RegDate) == MAPI_FALSE)
        printf("570write fail\n");
    #else
    if (MApi_SWI2C_WriteBytes(TAS5708_BUSID,1,&g_ucTas5708RegAddr,1,g_ucTas5708RegDate) == FALSE)
        printf("570write fail 1 \n");
    #endif

	//usleep(1*1000);
    Audio_Amplifier_ON();
	msAPI_Timer_Delayms(10);


     //   if (iptr->ReadBytes(1, &g_ucTas5708RegAddr, 1, &u8Data)==MAPI_FALSE)
	// printf("5708read fail\n");
	// printf("wwwwwwwwwwwwwwwwwwwww  = %x\n",u8Data);

	return MAPI_TRUE;
}


//#endif
 //wjq20100112<<<


