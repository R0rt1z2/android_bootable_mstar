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

#ifndef __PANEL_INX_NOVA_VB1_C__
#define __PANEL_INX_NOVA_VB1_C__

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include "panel_INX_NOVA_vb1.h"

//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------
#define PANEL_INX_NOVA_VB1_I2C_BUS_NUM          (3)
#define PANEL_INX_NOVA_VB1_I2C_BUS_SELECT		(E_I2C_BUS_SYS)
#define PANEL_INX_NOVA_VB1_I2C_ADDR		        ((PANEL_INX_NOVA_VB1_I2C_BUS_SELECT<<8)|0x34)
#define PANEL_INX_NOVA_VB1_I2C_SUB_ADDR         (0)

#define PANEL_INX_NOVA_VB1_CMD_LEN              (sizeof(PANEL_INX_NOVA_VB1_CMD_DATA)/sizeof(MS_U8))
#define PANEL_INX_NOVA_VB1_READ_DATA_LEN        (4)
#define PANEL_INX_NOVA_VB1_Delay(x)             MsOS_DelayTask(x)
#define PANEL_INX_NOVA_VB1_CMD_INTERVAL_TIME    (200)//(ms) the time is not exactly
#define PANEL_INX_NOVA_VB1_CMD_RETRY_TIMES      (5)

static MS_BOOL g_Panel_I2C_Init = FALSE;
static MS_BOOL g_AHB_Bridge_Unlock = FALSE;

static SWI2C_BusCfg Panel_INX_NOVA_VB1_I2cBusCfg[PANEL_INX_NOVA_VB1_I2C_BUS_NUM]=
{
    // Bus-0
    {PAD_DDCR_CK, PAD_DDCR_DA, 100}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Bus-1
    {PAD_TGPIO2, PAD_TGPIO3, 100},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Bus-2
    {PAD_GPIO36, PAD_GPIO37, 100}    //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
};

static PANEL_INX_NOVA_VB1_CMD_DATA UHDDivisonCmd[] =
{
    {0x00, 0x00, 0x01, 0x0A, 0x9F, 0x00, 0x00, 0x15, 0x04},
    {0x00, 0x00, 0x01, 0x0A, 0x9F, 0x01, 0x00, 0x50, 0x04},
};

static PANEL_INX_NOVA_VB1_CMD_DATA InputModeCmd[] =
{
    {0x00, 0x00, 0x01, 0x0A, 0x9F, 0x00, 0x00, 0x00, 0x0F}, //CMD_INX_NOVA_VB1_SET_INPUT_MODE
};

static PANEL_INX_NOVA_VB1_CMD_DATA Set3DModeCmd[] =
{
    {0x00, 0x00, 0x01, 0x0A, 0x9F, 0x00, 0x00, 0x01, 0x02}, //CMD_INX_NOVA_VB1_SET_3D_MODE
};

//-------------------------------------------------------------------------------------------------
//  Function define
//-------------------------------------------------------------------------------------------------
void MDrv_Panel_INX_NOVA_VB1_SWI2C_Init(void)
{
	MApi_SWI2C_Init(Panel_INX_NOVA_VB1_I2cBusCfg, PANEL_INX_NOVA_VB1_I2C_BUS_NUM);
    g_Panel_I2C_Init = 1;
}

MS_BOOL MDrv_Panel_INX_NOVA_VB1_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    return MApi_SWI2C_WriteBytes(u16BusNumSlaveID, u8addrnum, pu8addr, u16size, pu8data);
}

MS_BOOL MDrv_Panel_INX_NOVA_VB1_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    return MApi_SWI2C_ReadBytes(u16BusNumSlaveID, u8addrnum, pu8addr, u16size, pu8data);
}

MS_BOOL MDrv_Panel_INX_NOVA_VB1_SendCmd(PANEL_INX_NOVA_VB1_CMD_TYPE eCmdType, MS_U8 u8Data)
{
    PANEL_INX_NOVA_VB1_CMD_DATA *pCmdArray = NULL;
    MS_U8 u8ArraySize = 0;
    MS_BOOL bUnkown = FALSE;
    MS_BOOL bRet = FALSE;

    switch(eCmdType)
    {
        case CMD_INX_NOVA_VB1_UNLOCK_AHB:  //it is need set I2C cmd directly
            {
                MS_U8 u8Cmd1[] = {0xFE, 0x31};
                MS_U8 u8Cmd2[] = {0xFF, 0x72};
                MS_U8 u8Len = sizeof(u8Cmd1)/sizeof(MS_U8);
                bRet = MDrv_Panel_INX_NOVA_VB1_Write_Cmd(u8Cmd1, u8Len);
                bRet = MDrv_Panel_INX_NOVA_VB1_Write_Cmd(u8Cmd2, u8Len);
            }
            break;

        case CMD_INX_NOVA_VB1_SET_UHD_DIVISION_MODE:
            {
                pCmdArray = UHDDivisonCmd;
                u8ArraySize = sizeof(UHDDivisonCmd)/sizeof(PANEL_INX_NOVA_VB1_CMD_DATA);
                pCmdArray[0].Idx5 = u8Data; //set 0,1 ro 2 division
            }
            break;

        case CMD_INX_NOVA_VB1_SET_INPUT_MODE:
            {
                pCmdArray = InputModeCmd;
                u8ArraySize = sizeof(InputModeCmd)/sizeof(PANEL_INX_NOVA_VB1_CMD_DATA);
                pCmdArray[0].Idx5 = u8Data; //set INPUT_TIMMING_TYPE
            }
            break;

        case CMD_INX_NOVA_VB1_SET_3D_MODE:
            {
                pCmdArray = Set3DModeCmd;
                u8ArraySize = sizeof(Set3DModeCmd)/sizeof(PANEL_INX_NOVA_VB1_CMD_DATA);
                pCmdArray[0].Idx5 = u8Data; //set INPUT_3D_TYPE
            }
            break;

        default:
            bUnkown = TRUE;
            break;
    }

    if(bUnkown)
    {
        printf("[INX_NOVA_VB1]Can not find the command, please check!\n");
        return FALSE;
    }
    else if(pCmdArray != NULL) //it is only for array cmd
    {
        bRet = MDrv_Panel_INX_NOVA_VB1_Run_Cmd(pCmdArray, u8ArraySize);
    }
    printf("[INX_NOVA_VB1]send cmd:%d, return:%d\n", eCmdType, bRet);
    return bRet;
}

MS_BOOL MDrv_Panel_INX_NOVA_VB1_Run_Cmd(PANEL_INX_NOVA_VB1_CMD_DATA *pCmdArray, MS_U8 u8ArraySize)
{
    MS_BOOL bCmdPass = FALSE;
    MS_BOOL bStatusPass = FALSE;

    //1. make sure that unlock AHB bridge
    MDrv_Panel_INX_NOVA_VB1_Unlock_AHB();
    while(!bStatusPass)
    {
        //2.write command register
        MS_U8 u8CmdData[PANEL_INX_NOVA_VB1_CMD_LEN] = {0};
        MS_U8 u8Index = 0;
        MS_BOOL bRet = FALSE;
        for(u8Index = 0; u8Index < u8ArraySize; u8Index++)
        {
            memcpy(u8CmdData, &pCmdArray[u8Index], sizeof(PANEL_INX_NOVA_VB1_CMD_DATA));
            bRet = MDrv_Panel_INX_NOVA_VB1_Write_Cmd(u8CmdData, PANEL_INX_NOVA_VB1_CMD_LEN);
        }
        //3.read command register
        while(!bCmdPass)
        {
            bCmdPass = MDrv_Panel_INX_NOVA_VB1_Check_Cmd();
        }
        //4.read status register
        bStatusPass = MDrv_Panel_INX_NOVA_VB1_Check_Status();
    }
    return TRUE;
}

MS_BOOL MDrv_Panel_INX_NOVA_VB1_Check_Cmd(void)
{
    MS_U8 pCmd[5] = {0x08, 0x00, 0x01, 0x0A, 0x9F}; //cmd to read command register
    MS_U8 pReturn[PANEL_INX_NOVA_VB1_READ_DATA_LEN] = {0};
    MS_BOOL bRet = FALSE;
    MDrv_Panel_INX_NOVA_VB1_Write_Cmd(pCmd, sizeof(pCmd));
    MDrv_Panel_INX_NOVA_VB1_Read_Data(pReturn, PANEL_INX_NOVA_VB1_READ_DATA_LEN);
    if((pReturn[0]==0xFF)&&(pReturn[1]==0xFF)&&(pReturn[2]==0xFF)&&(pReturn[3]==0xFF))
    {
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }
    return bRet;
}

MS_BOOL MDrv_Panel_INX_NOVA_VB1_Check_Status(void)
{
    MS_U8 pCmd[5] = {0x08, 0x04, 0x01, 0x0A, 0x9F}; //cmd to read status register
    MS_U8 pReturn[PANEL_INX_NOVA_VB1_READ_DATA_LEN] = {0};
    MS_BOOL bRet = FALSE;

    memset(pReturn, 0xAB, PANEL_INX_NOVA_VB1_READ_DATA_LEN);
    MDrv_Panel_INX_NOVA_VB1_Write_Cmd(pCmd, sizeof(pCmd));
    MDrv_Panel_INX_NOVA_VB1_Read_Data(pReturn, PANEL_INX_NOVA_VB1_READ_DATA_LEN);
    if((pReturn[0]==0x00)&&(pReturn[1]==0x00)&&(pReturn[2]==0x00)&&(pReturn[3]==0x00))
    {
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }
    return bRet;
}

MS_BOOL MDrv_Panel_INX_NOVA_VB1_Write_Cmd(MS_U8 *pu8CmdData, MS_U8 u8CmdLen)
{
    MS_U16 u16BusNumSlaveID = PANEL_INX_NOVA_VB1_I2C_ADDR;
    MS_U8 u8SubAddr = PANEL_INX_NOVA_VB1_I2C_SUB_ADDR;
    MS_U8 uRetryCnt = PANEL_INX_NOVA_VB1_CMD_RETRY_TIMES;
    MS_BOOL bRet = FALSE;

    if(!g_Panel_I2C_Init)
    {
        MDrv_Panel_INX_NOVA_VB1_SWI2C_Init();
    }

    MDrv_Panel_INX_NOVA_VB1_Print_Cmd(TRUE, pu8CmdData, u8CmdLen);

    while(uRetryCnt--)
    {
        bRet = MDrv_Panel_INX_NOVA_VB1_SWI2C_WriteBytes(u16BusNumSlaveID, u8SubAddr, NULL, u8CmdLen, pu8CmdData);
        PANEL_INX_NOVA_VB1_Delay(PANEL_INX_NOVA_VB1_CMD_INTERVAL_TIME);

        if(bRet)
        {
            break;
        }
    }

    return bRet;
}

MS_BOOL MDrv_Panel_INX_NOVA_VB1_Read_Data(MS_U8 *pu8CmdData, MS_U8 u8DataLen)
{
    MS_U16 u16BusNumSlaveID = PANEL_INX_NOVA_VB1_I2C_ADDR;
    MS_U8 u8SubAddr[1] = {0x0C};
    MS_U8 u8SubAddrNum = sizeof(u8SubAddr);
    MS_BOOL bRet = FALSE;

    if(!g_Panel_I2C_Init)
    {
        MDrv_Panel_INX_NOVA_VB1_SWI2C_Init();
    }

    bRet = MDrv_Panel_INX_NOVA_VB1_SWI2C_ReadBytes(u16BusNumSlaveID, u8SubAddrNum, u8SubAddr, u8DataLen, pu8CmdData);
    MDrv_Panel_INX_NOVA_VB1_Print_Cmd(FALSE, pu8CmdData, u8DataLen);
    return bRet;
}

MS_BOOL MDrv_Panel_INX_NOVA_VB1_Print_Cmd(MS_BOOL bWrite, MS_U8 *pu8CmdData, MS_U8 u8CmdLen)
{
    MS_U8 u8Index = 0;

    if(bWrite)
    {
        printf("[INX_NOVA_VB1]Write cmd:");
    }
    else
    {
        printf("[INX_NOVA_VB1]Read cmd:");
    }
    while(u8Index < u8CmdLen)
    {
        printf("0x%02x, ", pu8CmdData[u8Index]);
        u8Index++;
    }
    printf("\r\n");

    return TRUE;
}
//-------------------------------------------------------------------------------------------------
//  Member Function
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Unlock_AHB(void)
{
    MS_BOOL bRet = FALSE;
    if(!g_AHB_Bridge_Unlock)
    {
        bRet = MDrv_Panel_INX_NOVA_VB1_SendCmd(CMD_INX_NOVA_VB1_UNLOCK_AHB, 0);
        g_AHB_Bridge_Unlock = bRet;
    }
    return bRet;
}

MS_BOOL MDrv_Panel_INX_NOVA_VB1_Set_UHD_DIVISION(UHD_DIVISION_TYPE eDivisionType)
{
    MDrv_Panel_INX_NOVA_VB1_Unlock_AHB();
    return MDrv_Panel_INX_NOVA_VB1_SendCmd(CMD_INX_NOVA_VB1_SET_UHD_DIVISION_MODE, (MS_U8)eDivisionType);
}

MS_BOOL MDrv_Panel_INX_NOVA_VB1_Set_INPUT_MODE(INPUT_TIMMING_TYPE eTimmingType)
{
    return MDrv_Panel_INX_NOVA_VB1_SendCmd(CMD_INX_NOVA_VB1_SET_INPUT_MODE, (MS_U8)eTimmingType);
}

MS_BOOL MDrv_Panel_INX_NOVA_VB1_Set_3D_MODE(INPUT_3D_TYPE e3DType)
{
    return MDrv_Panel_INX_NOVA_VB1_SendCmd(CMD_INX_NOVA_VB1_SET_3D_MODE, (MS_U8)e3DType);
}
#endif //__PANEL_INX_NOVA_VB1_C__

