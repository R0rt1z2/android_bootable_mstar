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

#ifndef __DRV_URSA_6M40_C__
#define __DRV_URSA_6M40_C__

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include "ursa_6m40.h"

//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------
#define URSA_6M40_CMD_DATA_SUM          (7)
#define URSA_6M40_I2C_BUS_NUM           (2)
#define URSA_6M40_DDC2BI_MODE_ADDR		(0x40)
#define URSA_6M40_I2C_SUB_ADDR          (0)

#define URSA_ISP_I2C_BUS_SEL 0x0
#define SPI_WRITE           0x10
#define SPI_READ            0x11
#define SPI_STOP            0x12

#define SLAVEADDR        0x98//  0x94//0x98
#define SERIALDEBUGADDR   0xB4// 0xB8//0xB4
#define cCmdDataWrite 0x10

#define  SW_IIC_READ             0
#define  SW_IIC_WRITE            1

#define URSA_6M40_CMD_RETRY_TIMES   (3)
#define URSA_6M40_CMD_INTERVAL_TIME     (50)
#define URSA_6M40_CMD_Delay(x)          udelay(x*1000)

URSA_CMD_GEN_SETTINGS g_UrsaCMDGenSetting;

static SWI2C_BusCfg Ursa_6m40_I2cBusCfg[URSA_6M40_I2C_BUS_NUM]=
{
    // Bus-0
    {PAD_DDCR_CK, PAD_DDCR_DA, 100}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Bus-1
    {PAD_TGPIO2, PAD_TGPIO3, 100},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Others, add other bus here
};

MS_BOOL g_bUrsaInit = FALSE;
//-------------------------------------------------------------------------------------------------
//  Function define
//-------------------------------------------------------------------------------------------------
void MDrv_Ursa_6M40_SWI2C_Init(void)
{
    if(g_bUrsaInit == FALSE)
    {
    	MApi_SWI2C_Init(Ursa_6m40_I2cBusCfg, URSA_6M40_I2C_BUS_NUM);
        g_bUrsaInit = TRUE;
    }
}

MS_BOOL MDrv_Ursa_6M40_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    return MApi_SWI2C_WriteBytes(u16BusNumSlaveID, u8addrnum, pu8addr, u16size, pu8data);
}

MS_BOOL MDrv_Ursa_6M40_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    return MApi_SWI2C_ReadBytes(u16BusNumSlaveID, u8addrnum, pu8addr, u16size, pu8data);
}

void MDrv_Ursa_6M40_Cmd_CheckSum(MS_U8 *pCmd, MS_U8 u8CmdLen)
{
    MS_U8 u8CheckSum =0;

    while( u8CmdLen > 1 )
    {
        -- u8CmdLen;
        u8CheckSum += pCmd[u8CmdLen] & 0x0F;
    }
    u8CheckSum &= 0x0F;
    pCmd[0] &= 0x0F;
    pCmd[0] |= ((u8CheckSum )<<4);
}

MS_BOOL MDrv_Ursa_6M40_SendCmd(URSA_6M40_CMD_TYPE eCmdType)
{
    MS_U8 pCmdData[URSA_6M40_CMD_DATA_SUM] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    MS_BOOL bUnkown = FALSE;
    MS_BOOL bRet = FALSE;

    switch(eCmdType)
    {
        case CMD_6M40_OSD_MUTE:
            {
            //0x43, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00
                pCmdData[0] = 0x43;
                pCmdData[1] = 0x03;
                pCmdData[2] = 0x01;
            }
            break;

        case CMD_6M40_OSD_ON:
            {
            //0x51,  0x04,  0x01,  0x00,  0x00,  0x00,  0x00
                pCmdData[0] = 0x51;
                pCmdData[1] = 0x04;
                pCmdData[2] = 0x01;
            }
            break;

        case CMD_6M40_OSD_OFF:
            {
            //0x41,  0x04,  0x00,  0x00,  0x00,  0x00,  0x00
                pCmdData[0] = 0x41;
                pCmdData[1] = 0x04;
            }
            break;

        case CMD_6M40_OSD_UNMUTE:
            {
            //0x33, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00
                pCmdData[0] = 0x33;
                pCmdData[1] = 0x03;
            }
            break;

        case CMD_6M40_LVDS_ON:
            {
            //0x93, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00
                pCmdData[0] = 0x93;
                pCmdData[1] = 0x08;
                pCmdData[2] = 0x01;
            }
            break;

        case CMD_6M40_LVDS_OFF:
            {
            //0x83, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00
                pCmdData[0] = 0x83;
                pCmdData[1] = 0x08;
            }
            break;
		case CMD_6M40_2K_MODE_ON:
			{
				//0x81,0x05,0x07,0x80,0x04,0x38,0x00
				pCmdData[0] = 0x81;
                pCmdData[1] = 0x05;
				pCmdData[2] = 0x07;
				pCmdData[3] = 0x80;
				pCmdData[4] = 0x04;
				pCmdData[5] = 0x38;

			}
			break;
	    case CMD_6M40_OSD_PROTECT_OFF:
			{
			//  95 09 00 00 00 00 00 
                pCmdData[0] = 0x95;
                pCmdData[1] = 0x09;							
			}
			break;
		case CMD_6M40_OSD_PROTECT_ON:
			{
			//  A5 09 01 00 00 00 00 
                pCmdData[0] = 0xA5;
                pCmdData[1] = 0x09;		
		pCmdData[2] = 0x01;						
			}
			break;
		case CMD_6M40_OSD_PROTECT_ON_EMMC:
			{
			//  B5 09 02 00 00 00 00 
                pCmdData[0] = 0xB5;
                pCmdData[1] = 0x09;		
		pCmdData[2] = 0x02;						
			}
			break;
        default:
            bUnkown = TRUE;
            break;
    }

    if(!bUnkown)
    {
        bRet  = MDrv_Ursa_6M40_Write_Cmd(pCmdData);
        printf("[6m40]Send cmd:%d, return:%d\n", eCmdType, bRet);
    }
    else
    {
        bRet = FALSE;
        printf("[6m40]Can not find the command, please check!\n");
    }

    return bRet;
}

MS_BOOL MDrv_Ursa_6M40_Write_Cmd(MS_U8 *pu8CmdData)
{
    MS_U16 u16BusNumSlaveID = URSA_6M40_DDC2BI_MODE_ADDR;
    MS_U8 u8SubAddr = URSA_6M40_I2C_SUB_ADDR;
    MS_U8 uRetryCnt = URSA_6M40_CMD_RETRY_TIMES;
    MS_BOOL bRet = FALSE;

    MDrv_Ursa_6M40_SWI2C_Init();
    printf("[6m40]Write cmd:%x,%x,%x,%x,%x,%x,%x\n", pu8CmdData[0], pu8CmdData[1], pu8CmdData[2], pu8CmdData[3], pu8CmdData[4], pu8CmdData[5], pu8CmdData[6]);
    while(uRetryCnt--)
    {
        bRet = MDrv_Ursa_6M40_SWI2C_WriteBytes(u16BusNumSlaveID, u8SubAddr, NULL, URSA_6M40_CMD_DATA_SUM, pu8CmdData);
        URSA_6M40_CMD_Delay(URSA_6M40_CMD_INTERVAL_TIME);
        if(bRet)
        {
            break;
        }
    }
    return bRet;
}

MS_BOOL MDrv_Ursa_6M40_Write_Cmd_Array(URSA_6M40_CMD_DATA *pCmdArray, MS_U8 u8ArraySize)
{
    MS_U8 cmd[URSA_6M40_CMD_DATA_SUM] = {0};
    MS_U8 u8Index = 0;
    MS_BOOL bRet = FALSE;

    for(u8Index = 0; u8Index < u8ArraySize; u8Index++)
    {
        memcpy(cmd, &pCmdArray[u8Index], sizeof(URSA_6M40_CMD_DATA));
        bRet = MDrv_Ursa_6M40_Write_Cmd(cmd);
        printf("[6m40]send cmd array[%d], return:%d\n", u8Index, bRet);
    }

    return bRet;
}

//-------------------------------------------------------------------------------------------------
//  Member Function
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Ursa_6M40_Set_Osd_On(void)
{
    return MDrv_Ursa_6M40_SendCmd(CMD_6M40_OSD_ON);
}

MS_BOOL MDrv_Ursa_6M40_Set_Osd_Off(void)
{
    return MDrv_Ursa_6M40_SendCmd(CMD_6M40_OSD_OFF);
}

MS_BOOL MDrv_Ursa_6M40_Set_Osd_Mute(void)
{
    return MDrv_Ursa_6M40_SendCmd(CMD_6M40_OSD_MUTE);
}

MS_BOOL MDrv_Ursa_6M40_Set_Osd_Unmute(void)
{
    return MDrv_Ursa_6M40_SendCmd(CMD_6M40_OSD_UNMUTE);
}

MS_BOOL MDrv_Ursa_6M40_Set_Lvds_On(void)
{
    return MDrv_Ursa_6M40_SendCmd(CMD_6M40_LVDS_ON);
}

MS_BOOL MDrv_Ursa_6M40_Set_Lvds_Off(void)
{
    return MDrv_Ursa_6M40_SendCmd(CMD_6M40_LVDS_OFF);
}

MS_BOOL MDrv_Ursa_6M40_Set_2K_Mode_On(void)
{
    return MDrv_Ursa_6M40_SendCmd(CMD_6M40_2K_MODE_ON);
}

MS_BOOL MDrv_Ursa_6M40_Set_VB1_Init(APIPNL_LINK_EXT_TYPE eLinkExtType)
{
    URSA_6M40_CMD_DATA Vb1_InitCmd[] =
    {
        {0x00, 0x01, 0x02, 0x80, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, /*this cmd need change by eLinkExtType*/
        {0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00},
    };
    MS_U8 u8ArraySize = sizeof(Vb1_InitCmd)/sizeof(URSA_6M40_CMD_DATA);
    MS_BOOL bRet = FALSE;

    printf("[6m40]APIPNL_LINK_EXT_TYPE:%u\n", eLinkExtType);

    if(LINK_VBY1_10BIT_2LANE == eLinkExtType || LINK_VBY1_10BIT_4LANE == eLinkExtType || LINK_VBY1_10BIT_8LANE == eLinkExtType)
    {
        switch(eLinkExtType)
        {
            case LINK_VBY1_10BIT_2LANE:
                {
                    MS_U8 fixCmd[URSA_6M40_CMD_DATA_SUM] = {0x00, 0x02, 0x11, 0x00, 0x00, 0x04, 0x00};
                    memcpy(&Vb1_InitCmd[1], fixCmd, sizeof(fixCmd));
                }
                break;

            case LINK_VBY1_10BIT_4LANE:
                {
                    MS_U8 fixCmd[URSA_6M40_CMD_DATA_SUM] = {0x00, 0x02, 0x55, 0x00, 0x04, 0x01, 0x05};
                    memcpy(&Vb1_InitCmd[1], fixCmd, sizeof(fixCmd));
                }
                break;

            case LINK_VBY1_10BIT_8LANE:
                {
                    MS_U8 fixCmd[URSA_6M40_CMD_DATA_SUM] = {0x00, 0x02, 0xff, 0x20, 0x31, 0x64, 0x75};
                    memcpy(&Vb1_InitCmd[1], fixCmd, sizeof(fixCmd));
                }
                break;
            default:
                break;
        }

        bRet = MDrv_Ursa_6M40_Write_Cmd_Array(Vb1_InitCmd, u8ArraySize);
    }

    return bRet;
}

void MDrv_Ursa_6M40_SWI2C_ReadDatas(MS_U8  * paddr, MS_U8 addrSize, MS_U8 * pdatabuf, MS_U8 dataSize)
{
    MS_U8 k;
    MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);

    if(MApi_SWI2C_AccessStart(SLAVEADDR, SW_IIC_WRITE)==FALSE)
    {
	printf("\n6M30->I2cReadDatas->IIC_AccessStart Error");
    }

    for(k=0;k<addrSize;k++)
    {
       if(MApi_SWI2C_SendByte(paddr[k])==FALSE)
	{
    		printf("\n6M30->I2cReadDatas paddr Error");
       }
   }

    if(MApi_SWI2C_AccessStart(SLAVEADDR, SW_IIC_READ)==FALSE)
   {
	printf("\n6M30->I2cReadDatas->IIC_AccessReStart Error");
   }

    for(k=0;k<dataSize-1;k++)
    {
       pdatabuf[k]=MApi_SWI2C_GetByte(1);
    }
    //last byte
    pdatabuf[k]=MApi_SWI2C_GetByte(0);

    MApi_SWI2C_Stop();
}

void MDrv_Ursa_6M40_SWI2C_WriteDatas(MS_U8 * paddr, MS_U8 addrSize, MS_U8  * pdatabuf, MS_U8 dataSize)
{
	 MS_U8 k;
	 MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);

	 if(MApi_SWI2C_AccessStart(SLAVEADDR, SW_IIC_WRITE)==FALSE)
	{
	 printf("\n6M30->I2cWriteDatas->IIC_AccessStart Error");
	}

	 for(k=0;k<addrSize;k++)
	 {
		 if(MApi_SWI2C_SendByte(paddr[k])==FALSE)
	 {
			 printf("\n6M30->paddr Error");
		 }

	}

	 for(k=0;k<dataSize;k++)
	 {
		 if(MApi_SWI2C_SendByte(pdatabuf[k])==FALSE)
		  {
			 printf("\n6M30->pdatabuf Error");
		  }
	 }

	 MApi_SWI2C_Stop();


   }

void MDrv_Ursa_6M40_SWI2C_WriteDatas2(MS_U8 * paddr, MS_U8 addrSize, MS_U8 * pdatabuf, MS_U8 dataSize)
{
    MS_U8 k;
    MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);
    MApi_SWI2C_AccessStart(SERIALDEBUGADDR, SW_IIC_WRITE);

    for(k=0;k<addrSize;k++)
        MApi_SWI2C_SendByte(paddr[k]);

    for(k=0;k<dataSize;k++)
    {
        MApi_SWI2C_SendByte(pdatabuf[k]);
    }

    MApi_SWI2C_Stop();
}
void MDrv_Ursa_6M40_SerialFlash_WriteReg(MS_U8 bBank, MS_U8 bAddr, MS_U8 bData)
{
    MS_U8 bWriteData[4];

    bWriteData[0] = cCmdDataWrite;
    bWriteData[1] = bBank;
    bWriteData[2] = bAddr;
    bWriteData[3] = bData;

    MDrv_Ursa_6M40_SWI2C_WriteDatas2(bWriteData, sizeof(bWriteData),NULL,0);
}

void MDrv_Ursa_6M40_SWI2C_WriteStop(void)
{
    MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);

    if(MApi_SWI2C_AccessStart(SLAVEADDR, SW_IIC_WRITE)==FALSE)
    {
	 printf("\n6M30->I2cWriteStop->IIC_AccessStart Error");
    }
    if(MApi_SWI2C_SendByte(SPI_STOP)==FALSE)
    {
	 printf("\n6M30->I2cWriteStop-> Error");
    }
    MApi_SWI2C_Stop();
}
void MDrv_Ursa_6M40_SWI2C_CommandRead(void)
{
    MApi_SWI2C_UseBus(URSA_ISP_I2C_BUS_SEL);

    if(MApi_SWI2C_AccessStart(SLAVEADDR, SW_IIC_WRITE)==FALSE)
    {
	 printf("\n6M30->I2cCommondRead Error");
    }
    if(MApi_SWI2C_SendByte(SPI_READ)==FALSE)
   {
	printf("\n6M30->I2cCommondRead-> SPI_READ");
   }
}


MS_U8 MDrv_Ursa_6M40_ReadChipID(void)
{
    MS_U8 ReadIDCommand[]={SPI_WRITE,0xAB,0x00,0x00,0x00};
    MS_U8 buf;

    MDrv_Ursa_6M40_SWI2C_WriteDatas(ReadIDCommand,sizeof(ReadIDCommand),NULL,0);

    buf=SPI_READ; //Manufacture ID
    MDrv_Ursa_6M40_SWI2C_ReadDatas(&buf,1,&buf,1);

    buf=SPI_READ; //Device ID1
    MDrv_Ursa_6M40_SWI2C_ReadDatas(&buf,1,&buf,1);

    buf=SPI_READ; //Device ID2
    MDrv_Ursa_6M40_SWI2C_ReadDatas(&buf,1,&buf,1);

    MDrv_Ursa_6M40_SWI2C_WriteStop();
    return buf;
}

MS_U8 MDrv_Ursa_6M40__ReadChipJEDEC_ID(MS_U8 *ReadIDBuf)
{
    MS_U8 ReadIDCommand[]={SPI_WRITE,0x9f};

    MS_U8 buf;

    MDrv_Ursa_6M40_SWI2C_WriteDatas(ReadIDCommand,sizeof(ReadIDCommand),NULL,0);

    buf=SPI_READ; //Manufacture ID
    MDrv_Ursa_6M40_SWI2C_ReadDatas(&buf,1,&ReadIDBuf[0],1);
    buf=SPI_READ; //Device ID1
    MDrv_Ursa_6M40_SWI2C_ReadDatas(&buf,1,&ReadIDBuf[1],1);

    buf=SPI_READ; //Device ID2
    MDrv_Ursa_6M40_SWI2C_ReadDatas(&buf,1,&ReadIDBuf[2],1);

    MDrv_Ursa_6M40_SWI2C_WriteStop();
    return buf;
}

MS_U8 MDrv_Ursa_6M40_ReadChipIDBuffer(MS_U8 *ReadIDBuf)
{
    MS_U8 ReadIDCommand[]={SPI_WRITE,0xAB,0x00,0x00,0x00};
    MS_U8 buf;

    MDrv_Ursa_6M40_SWI2C_WriteDatas(ReadIDCommand,sizeof(ReadIDCommand),NULL,0);
    buf=SPI_READ; //Manufacture ID
    MDrv_Ursa_6M40_SWI2C_ReadDatas(&buf,1,&ReadIDBuf[0],1);

    buf=SPI_READ; //Device ID1
    MDrv_Ursa_6M40_SWI2C_ReadDatas(&buf,1,&ReadIDBuf[1],1);

    buf=SPI_READ; //Device ID2
    MDrv_Ursa_6M40_SWI2C_ReadDatas(&buf,1,&ReadIDBuf[2],1);

    MDrv_Ursa_6M40_SWI2C_WriteStop();
    return buf;
}
MS_BOOL MDrv_Ursa_6M40_EnterIspMode(void)
{
    // 0x4D,0x53,0x54,0x41,0x52
    MS_U8 enterISPdata[]="MSTAR";
    MS_U8 addr=0x20;

    // enter isp mode

    MDrv_Ursa_6M40_SWI2C_WriteDatas(enterISPdata,sizeof(enterISPdata)-1,NULL,0);
    MDrv_Ursa_6M40_SWI2C_ReadDatas(&addr,1,&addr,1);

    if(addr==0xC0)
    {
        printf("\nEnterIspMode->OK\n");
        return TRUE; // enter ok
    }
    else
    {
        printf("\nEnterIspMode->fail\n");
        return FALSE; // enter fail
    }
}
MS_BOOL MDrv_Ursa_6M40_ExitIspMode(void)
{
    MS_U8 tdata=0x24;
	    MS_U8 addr=0x20;
    // exit isp mode
    printf("\n1.ExitIspMode");
    MDrv_Ursa_6M40_SWI2C_WriteDatas(&tdata,1,NULL,0);
	MDrv_Ursa_6M40_SWI2C_ReadDatas(&addr,1,&addr,1);

    if(addr==0xC0)
    {
        printf("\nExitIspMode->fail\n");
        return TRUE; // enter ok
    }
    else
    {
        printf("\nExitIspMode->OK\n");
        return FALSE; // enter fail
    }
}

void MDrv_Ursa_6M40_EnterSerialDebug(void)
{
    MS_U8 EnterSerialDebug[]={0x53, 0x45, 0x52, 0x44, 0x42};

    MDrv_Ursa_6M40_SWI2C_WriteDatas2(EnterSerialDebug,sizeof(EnterSerialDebug),NULL,0);
}

void MDrv_Ursa_6M40_EnterSingleStep(void)
{
    MS_U8 EnterSingleStep[]={cCmdDataWrite, 0xC0, 0xC1, 0x53};

    MDrv_Ursa_6M40_SWI2C_WriteDatas2(EnterSingleStep,sizeof(EnterSingleStep),NULL,0);
}

void MDrv_Ursa_6M40_ExitSingleStep(void)
{
    MS_U8 ExitSingleStep[]={cCmdDataWrite, 0xC0, 0xC1, 0xFF};

    MDrv_Ursa_6M40_SWI2C_WriteDatas2(ExitSingleStep,sizeof(ExitSingleStep),NULL,0);
}

void MDrv_Ursa_6M40_ExitSerialDebug(void)
{
    MS_U8 ExitSerialDebug[]={0x45};

    MDrv_Ursa_6M40_SWI2C_WriteDatas2(ExitSerialDebug,sizeof(ExitSerialDebug),NULL,0);
}

void MDrv_Ursa_6M40_WP_PullHigh(void)
{
	 MDrv_Ursa_6M40_EnterSerialDebug();
    MDrv_Ursa_6M40_EnterSingleStep();

    // Set chip to power up for register setting
    MDrv_Ursa_6M40_SerialFlash_WriteReg(0x00, 0, 0);
    MDrv_Ursa_6M40_SerialFlash_WriteReg(0x00, 0xF0, 0x02);

    // Only for GIPO P2.4
    MDrv_Ursa_6M40_SerialFlash_WriteReg(0x00, 0x00, 0x01);    // BK1_50[7] = 1
    MDrv_Ursa_6M40_SerialFlash_WriteReg(0x00, 0x50, 0x80);    // Enable register clock for gaultier

    MDrv_Ursa_6M40_SerialFlash_WriteReg(0x00, 0x00, 0x00);
    MDrv_Ursa_6M40_SerialFlash_WriteReg(0x00, 0x63, 0x3D);    // Adjust P2.4 and P2.7
                                               // TSUM chip p2.4 at 0x63[4], p2.7 at 0x63[3]
                                               // TSUMO chip p2.4 at 0x63[5],p2.7 at 0x63[2]
    MDrv_Ursa_6M40_SerialFlash_WriteReg(0x00, 0xc5, 0xFF);    // PWM 1 set to high (P2.7)
    MDrv_Ursa_6M40_SerialFlash_WriteReg(0x00, 0xc7, 0xFF);    // PWM 2 set to high (P2.4)

    // Paulo series chip
    // MCU config pin PWM0~3 default resistor setting = 1001
    MDrv_Ursa_6M40_SerialFlash_WriteReg(0x00, 0x00, 0x0A);
    MDrv_Ursa_6M40_SerialFlash_WriteReg(0x00, 0xED, 0x3F);    // PWM1 pin set to PWM BK0A_ED[7:6]=00
                                               // the other pin set to input mode
    // Lopez series chip
    // MCU config pin PWM2
    MDrv_Ursa_6M40_SerialFlash_WriteReg(0x1E, 0xE5, 0xC8);
    MDrv_Ursa_6M40_SerialFlash_WriteReg(0x1E, 0x8A, 0xC0);

    MDrv_Ursa_6M40_ExitSingleStep();
    MDrv_Ursa_6M40_ExitSerialDebug();
}


#endif //__DRV_URSA_6M40_C__

