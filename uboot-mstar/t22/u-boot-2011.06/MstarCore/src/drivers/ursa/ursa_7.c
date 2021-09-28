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

#ifndef __DRV_URSA_7_C__
#define __DRV_URSA_7_C__

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include "ursa_7.h"

//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------
#define URSA_7_CMD_DATA_SUM             (7)
#define URSA_7_I2C_BUS_NUM              (2)
#define URSA_7_DDC2BI_MODE_ADDR         (0x40)
#define URSA_7_I2C_SUB_ADDR             (0)

static SWI2C_BusCfg Ursa_7_I2cBusCfg[URSA_7_I2C_BUS_NUM]=
{
    // Bus-0
    {PAD_DDCR_CK, PAD_DDCR_DA, 100}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Bus-1
    {PAD_TGPIO2, PAD_TGPIO3, 100},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Others, add other bus here
};

//-------------------------------------------------------------------------------------------------
//  Function define
//-------------------------------------------------------------------------------------------------
void MDrv_Ursa_7_SWI2C_Init(void)
{
	MApi_SWI2C_Init(Ursa_7_I2cBusCfg, URSA_7_I2C_BUS_NUM);
}

MS_BOOL MDrv_Ursa_7_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    return MApi_SWI2C_WriteBytes(u16BusNumSlaveID, u8addrnum, pu8addr, u16size, pu8data);
}

MS_BOOL MDrv_Ursa_7_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    return MApi_SWI2C_ReadBytes(u16BusNumSlaveID, u8addrnum, pu8addr, u16size, pu8data);
}

MS_BOOL MDrv_Ursa_7_SendCmd(URSA_7_CMD_TYPE eCmdType)
{
    MS_U8 pCmdData[URSA_7_CMD_DATA_SUM] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    MS_BOOL bUnkown = FALSE;
    MS_BOOL bRet = FALSE;
    
    switch(eCmdType)
    {
         case UC_INIT_RX_ALL_1:
        {
            //0xB0, 0x01, 0x02, 0x03, 0x45, 0x00, 0x00
            pCmdData[0] = 0xB0;
            pCmdData[1] = 0x01;
            pCmdData[2] = 0x02;
            pCmdData[3] = 0x03;
            pCmdData[4] = 0x54;
            pCmdData[5] = 0x00;
            pCmdData[6] = 0x00;
        }
            break;
        
        case UC_INIT_RX_ALL_2:
            {
                //0x40, 0x02, 0x11, 0x00, 0x00, 0x01, 0x00}
                pCmdData[0] = 0x40;
                pCmdData[1] = 0x02;
                pCmdData[2] = 0x11;
                pCmdData[3] = 0x00;
                pCmdData[4] = 0x00;
                pCmdData[5] = 0x01;
                pCmdData[6] = 0x00;
                
            }
            break;
            
        case UC_SET_IN_VIDEO_SIZE:
          {
              //0x81, 0x05, 0x07, 0x80, 0x04, 0x38, 0x00 
              pCmdData[0] = 0x81;
              pCmdData[1] = 0x05;
              pCmdData[2] = 0x07;
              pCmdData[3] = 0x80;
              pCmdData[4] = 0x04;
              pCmdData[5] = 0x38;
              pCmdData[6] = 0x00;
              
          }
          break;

        case UC_SET_LVDS_RX_RECONFIG:
          {
              //0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00
              pCmdData[0] = 0x61;
              pCmdData[1] = 0x06;
              pCmdData[2] = 0x00;
              pCmdData[3] = 0x00;
              pCmdData[4] = 0x00;
              pCmdData[5] = 0x00;
              pCmdData[6] = 0x00;
              
          }
          break;
        case UC_SET_3D_MODE:
        {
            //0x81,0x05,0x07,0x80,0x04,0x38,0x00
            pCmdData[0] = 0x13;
            pCmdData[1] = 0x01;
            pCmdData[2] = 0x00;
            pCmdData[3] = 0x00;
            pCmdData[4] = 0x00;
            pCmdData[5] = 0x00;
            pCmdData[6] = 0x00;
            
        }
        break;
        case UC_WRITE_REGISTER_1:
          {
              //0x23,0x1B,0x10,0x27,0xC2,0xEE,0xD0,
              pCmdData[0] = 0x23;
              pCmdData[1] = 0x1B;
              pCmdData[2] = 0x10;
              pCmdData[3] = 0x27;
              pCmdData[4] = 0xC2;
              pCmdData[5] = 0xEE;
              pCmdData[6] = 0xD0;

          }
          break;
          case UC_WRITE_REGISTER_2:
            {
                //0x23,0x1B,0x10,0x27,0x40,0x40,0xD0
                pCmdData[0] = 0x23;
                pCmdData[1] = 0x1B;
                pCmdData[2] = 0x10;
                pCmdData[3] = 0x27;
                pCmdData[4] = 0x40;
                pCmdData[5] = 0x40;
                pCmdData[6] = 0xD0;

            }
            break;
           case UC_WRITE_REGISTER_3:
          {
              //0x33,0x1B,0x10,0x27,0xC3,0xEE,0xD0
              pCmdData[0] = 0x33;
              pCmdData[1] = 0x1B;
              pCmdData[2] = 0x10;
              pCmdData[3] = 0x27;
              pCmdData[4] = 0xC3;
              pCmdData[5] = 0xEE;
              pCmdData[6] = 0xD0;

          }
          break;
         case UC_WRITE_REGISTER_MASK_1:
          {
              //0x33,0x1C,0x13,0x00,0x04,0x40,0x40
              pCmdData[0] = 0x33;
              pCmdData[1] = 0x1C;
              pCmdData[2] = 0x13;
              pCmdData[3] = 0x00;
              pCmdData[4] = 0x04;
              pCmdData[5] = 0x40;
              pCmdData[6] = 0x40;

          }
          break;
          case UC_WRITE_REGISTER_MASK_2:
            {
                //0x33,0x1C,0x13,0x00,0x04,0x00,0x40
                pCmdData[0] = 0x33;
                pCmdData[1] = 0x1C;
                pCmdData[2] = 0x13;
                pCmdData[3] = 0x00;
                pCmdData[4] = 0x04;
                pCmdData[5] = 0x00;
                pCmdData[6] = 0x40;

            }
            break;

        case UC_URSA_MOD_POWER_ON:
            {
                //URSA MOD Power On command: 0x93, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00
                pCmdData[0] = 0x93;
                pCmdData[1] = 0x08;
                pCmdData[2] = 0x01;
                pCmdData[3] = 0x00;
                pCmdData[4] = 0x00;
                pCmdData[5] = 0x00;
                pCmdData[6] = 0x00;
        
            }
            break;

        default:
            bUnkown = TRUE;
            break;
    }

    if(!bUnkown)
    {
        bRet  = MDrv_Ursa_7_Write_Cmd(pCmdData);
        printf("[Ursa7]Send cmd:%d, return:%d\n", eCmdType, bRet);
    }
    else
    {
        bRet = FALSE;
        printf("[Ursa7]Can not find the command, please check!\n");
    }
    
    return bRet;
}

MS_BOOL MDrv_Ursa_7_Write_Cmd(MS_U8 *pu8CmdData)
{
    MS_U16 u16BusNumSlaveID = URSA_7_DDC2BI_MODE_ADDR;
    MS_U8 u8SubAddr = URSA_7_I2C_SUB_ADDR;
    
    MDrv_Ursa_7_SWI2C_Init();
    printf("[Ursa7]Write cmd:%x,%x,%x,%x,%x,%x,%x\n", pu8CmdData[0], pu8CmdData[1], pu8CmdData[2], pu8CmdData[3], pu8CmdData[4], pu8CmdData[5], pu8CmdData[6]);
    return MDrv_Ursa_7_SWI2C_WriteBytes(u16BusNumSlaveID, u8SubAddr, NULL, URSA_7_CMD_DATA_SUM, pu8CmdData);
}
//-------------------------------------------------------------------------------------------------
//  Member Function
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_Ursa_7_Set_2_lane_VB1(void)
{
        MS_BOOL bRet = FALSE;
        MS_U8 retryCount=0;
        MS_U8 MaxRetry=10;
        do
        {
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
        
            printf("delay Done......\n");
            bRet=MDrv_Ursa_7_SendCmd(UC_SET_IN_VIDEO_SIZE);
            retryCount=retryCount+1;

        }while ( (bRet==FALSE) && ( retryCount<MaxRetry));

            udelay(50);
            MDrv_Ursa_7_SendCmd(UC_SET_3D_MODE);
            udelay(50);
            MDrv_Ursa_7_SendCmd(UC_URSA_MOD_POWER_ON);
            udelay(50);
        return bRet;
}

MS_BOOL MDrv_Ursa_7_Set_2_lane_VB1_per_init(void)
{
        MS_BOOL bRet = FALSE;
        MS_U8 retryCount=0;
        MS_U8 MaxRetry=20;
        do
        {
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay ..........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay...........\n");
            udelay(50000);
            printf("delay Done......\n");
            bRet=MDrv_Ursa_7_SendCmd(UC_INIT_RX_ALL_1);
            retryCount=retryCount+1;

        }while ( (bRet==FALSE) && ( retryCount<MaxRetry));

        udelay(50);
        MDrv_Ursa_7_SendCmd(UC_INIT_RX_ALL_2);
        udelay(50);
        MDrv_Ursa_7_SendCmd(UC_SET_LVDS_RX_RECONFIG);
        udelay(50);
        return bRet;
}

#endif //__DRV_URSA_7_C__

