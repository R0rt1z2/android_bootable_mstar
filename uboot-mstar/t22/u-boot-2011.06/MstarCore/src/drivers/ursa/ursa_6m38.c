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

#ifndef __DRV_URSA_6M38_C__
#define __DRV_URSA_6M38_C__

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include "ursa_6m38.h"
#include <panel/panel.h>
#include <MsSystem.h>
//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------
#define URSA_6M38_CMD_DATA_SUM              (7)
#define URSA_6M38_I2C_BUS_NUM               (1)
#define URSA_6M38_DDC2BI_MODE_ADDR          (0x38)
#define URSA_6M38_I2C_SUB_ADDR              (0)
#define URSA_6M38_PANEL_INIT_AC_DELAY          (750)
#define URSA_6M38_PANEL_INIT_DC_DELAY         (1200)

#if defined(CONFIG_MSTAR_MASERATI)
static SWI2C_BusCfg Ursa_6M38_I2cBusCfg[URSA_6M38_I2C_BUS_NUM]=
{
    // Bus-0
    {PAD_GPIO28, PAD_GPIO29, 100},  // FRC I2C
};
#else
static SWI2C_BusCfg Ursa_6M38_I2cBusCfg[URSA_6M38_I2C_BUS_NUM]=
{
    // Bus-0
    {PAD_DDCR_CK, PAD_DDCR_DA, 100}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
};
#endif

//-------------------------------------------------------------------------------------------------
//  Function define
//-------------------------------------------------------------------------------------------------
// for URSA6 panel
void MDrv_Ursa_6M38_SWI2C_Init(void)
{
    static MS_U8 is_first_init = FALSE;
    unsigned long lpanelVCCOnTimer = 0;
    unsigned long ltotalTime = 0;
    unsigned long lursaDelayTime = 0;
    UBOOT_TRACE("IN\n");
    if(is_first_init == FALSE)
    {
        UBOOT_DEBUG("[6M38] Initialize SW I2C\n");
        // according to spec, before sending i2c command to U6, we must delay about 1200ms
        lpanelVCCOnTimer = MApi_PNL_GetPanelVccOnTime();
        ltotalTime = MsSystemGetBootTime();
        if(ltotalTime > lpanelVCCOnTimer)
        {
            lursaDelayTime = ltotalTime - lpanelVCCOnTimer;
            if(lursaDelayTime < URSA_6M38_PANEL_INIT_DC_DELAY)
            {
                lursaDelayTime = URSA_6M38_PANEL_INIT_DC_DELAY - lursaDelayTime;
                printf("[%s][%d]ursa6 delay time: %ld\n", __FUNCTION__, __LINE__, lursaDelayTime);
                mdelay(lursaDelayTime);
            }
        }
        MApi_SWI2C_Init(Ursa_6M38_I2cBusCfg, URSA_6M38_I2C_BUS_NUM);
        is_first_init = TRUE;
    }
    else
    {
        UBOOT_ERROR("[6M38] SW I2C is already initialized\n");
        return;
    }
    UBOOT_TRACE("OK\n");
}

MS_BOOL MDrv_Ursa_6M38_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    return MApi_SWI2C_WriteBytes(u16BusNumSlaveID, u8addrnum, pu8addr, u16size, pu8data);
}

MS_BOOL MDrv_Ursa_6M38_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    return MApi_SWI2C_ReadBytes(u16BusNumSlaveID, u8addrnum, pu8addr, u16size, pu8data);
}

MS_BOOL MDrv_Ursa_6M38_SendCmd(URSA_6M38_CMD_TYPE eCmdType)
{
    MS_U8 pCmdData[URSA_6M38_CMD_DATA_SUM] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    MS_BOOL bUnkown = FALSE;
    MS_BOOL bRet = FALSE;

    UBOOT_TRACE("IN\n");
    switch(eCmdType)
    {
         case UC_INIT_RX_ALL_1:
            {
                //0x81,0x05,0x07,0x80,0x04,0x38,0x00
                pCmdData[0] = 0x40;
                pCmdData[1] = 0x01;
                pCmdData[2] = 0x02;
                pCmdData[3] = 0x00;
                pCmdData[4] = 0x00;
                pCmdData[5] = 0x11;
                pCmdData[6] = 0x00;

            }
            break;

        case UC_INIT_RX_ALL_2:
            {
                //0x81,0x05,0x07,0x80,0x04,0x38,0x00
                pCmdData[0] = 0x30;
                pCmdData[1] = 0x02;
                pCmdData[2] = 0x00;
                pCmdData[3] = 0x00;
                pCmdData[4] = 0x00;
                pCmdData[5] = 0x01;
                pCmdData[6] = 0x00;

            }
            break;

        case UC_SET_IN_VIDEO_SIZE:
          {
              //0x81,0x05,0x07,0x80,0x04,0x38,0x00
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
              //0x81,0x05,0x07,0x80,0x04,0x38,0x00
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
         case UC_DISABLE_AUTO_MUTE:
            {
                //0xFD, 0x0D, 0x02, 0x00, 0x00, 0x00, 0x00
                pCmdData[0] = 0xFD;
                pCmdData[1] = 0x0D;
                pCmdData[2] = 0x02;
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
        bRet = MDrv_Ursa_6M38_Write_Cmd(pCmdData);
        if(bRet == FALSE)
            UBOOT_ERROR("[6M38]Send cmd:%d FAILED !!!\n", eCmdType);
        else
            UBOOT_DEBUG("[6M38]Send cmd:%d SUCCESSFUL !!!\n", eCmdType);
    }
    else
    {
        bRet = FALSE;
        UBOOT_ERROR("[6M38]Can not find the command, please check!\n");
    }

    UBOOT_TRACE("OK\n");
    return bRet;
}

MS_BOOL MDrv_Ursa_6M38_Write_Cmd(MS_U8 *pu8CmdData)
{
    UBOOT_TRACE("IN\n");
    MS_U16 u16BusNumSlaveID = URSA_6M38_DDC2BI_MODE_ADDR;
    MS_U8 u8SubAddr = URSA_6M38_I2C_SUB_ADDR;
    MS_U8 bRet;

    UBOOT_DEBUG("[6M38]Write cmd:%x,%x,%x,%x,%x,%x,%x\n", pu8CmdData[0], pu8CmdData[1], pu8CmdData[2], pu8CmdData[3], pu8CmdData[4], pu8CmdData[5], pu8CmdData[6]);

    bRet = MDrv_Ursa_6M38_SWI2C_WriteBytes(u16BusNumSlaveID, u8SubAddr, NULL, URSA_6M38_CMD_DATA_SUM, pu8CmdData);
    if (!bRet)
        UBOOT_TRACE("CMD FAIL\n");
    else
        UBOOT_TRACE("CMD OK\n");
    return bRet;
}


//-------------------------------------------------------------------------------------------------
//  Member Function
//-------------------------------------------------------------------------------------------------

void MDrv_Ursa_6M38_Set_2_lane_VB1(void)
{
    UBOOT_TRACE("IN\n");
    MDrv_Ursa_6M38_SendCmd(UC_SET_IN_VIDEO_SIZE);
    MDrv_Ursa_6M38_SendCmd(UC_SET_3D_MODE);
    MDrv_Ursa_6M38_SendCmd(UC_WRITE_REGISTER_MASK_1);
    MDrv_Ursa_6M38_SendCmd(UC_WRITE_REGISTER_MASK_2);
    UBOOT_TRACE("OK\n");
}

void MDrv_Ursa_6M38_Set_2_lane_VB1_per_init(void)
{
    char cmd[32] = { 0x00 };

    UBOOT_TRACE("IN\n");
    if(is_str_resume())
    {
        UBOOT_DEBUG("[6M38] DELAY %d\n", URSA_6M38_PANEL_INIT_DC_DELAY);
        sprintf(cmd, "delay %d", URSA_6M38_PANEL_INIT_DC_DELAY);
    }
    else
    {
        UBOOT_DEBUG("[6M38] DELAY %d\n", URSA_6M38_PANEL_INIT_AC_DELAY);
        sprintf(cmd, "delay %d", URSA_6M38_PANEL_INIT_AC_DELAY);
    }

    run_command(cmd, 0);                   // according to spec, from panel powers up and receives I2C command, we must delay at lease 1200ms

    MDrv_Ursa_6M38_SendCmd(UC_INIT_RX_ALL_1);
    MDrv_Ursa_6M38_SendCmd(UC_INIT_RX_ALL_2);
    MDrv_Ursa_6M38_SendCmd(UC_SET_LVDS_RX_RECONFIG);
    MDrv_Ursa_6M38_SendCmd(UC_WRITE_REGISTER_1);
    MDrv_Ursa_6M38_SendCmd(UC_WRITE_REGISTER_2);
    MDrv_Ursa_6M38_SendCmd(UC_WRITE_REGISTER_3);
    MDrv_Ursa_6M38_SendCmd(UC_DISABLE_AUTO_MUTE);

    UBOOT_TRACE("OK\n");
}

void MDrv_Ursa_6M38_4K2K_init(void)
{
    MS_U8 u8_4k2k_cmd_init[9][7] =
    {
        {0x80, 0x01, 0x02, 0x00, 0x00, 0x55, 0x00},
        {0x80, 0x02, 0x00, 0x00, 0x02, 0x01, 0x03},
        {0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x23, 0x1B, 0x10, 0x27, 0xC2, 0xEE, 0xD0},
        {0x33, 0x1B, 0x10, 0x27, 0xC3, 0xEE, 0xD0},
        {0xC1, 0x05, 0x0F, 0x00, 0x08, 0x70, 0x00},
        {0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x33, 0x1C, 0x13, 0x00, 0x04, 0x40, 0x40},
        {0x33, 0x1C, 0x13, 0x00, 0x04, 0x00, 0x40},
    };
    MS_U8 cmd_index;

    UBOOT_TRACE("IN\n");

    for(cmd_index = 0; cmd_index < 9; cmd_index++)
    {
        UBOOT_DEBUG("[6M38] send 4K2K init cmd_%d--->\n", cmd_index);
        if(!MDrv_Ursa_6M38_Write_Cmd(u8_4k2k_cmd_init[cmd_index]))
        {
            UBOOT_ERROR("[6M38]Send cmd: FAILED !!!\n");
        }
    }

    UBOOT_TRACE("OK\n");
}
void MDrv_Ursa_6M38_4K2K_DisableAutoMute(void)
{
    MS_U8 u8_4k2k_cmd_init[1][7] =
    {
        {0xFD, 0x0D, 0x02, 0x00, 0x00, 0x00, 0x00},
    };
    MS_U8 cmd_index;

    UBOOT_TRACE("IN\n");
    for(cmd_index = 0; cmd_index < 1; cmd_index++)
    {
        UBOOT_DEBUG("[6M38] send 4K2K init cmd_%d--->\n", cmd_index);
        if(!MDrv_Ursa_6M38_Write_Cmd(u8_4k2k_cmd_init[cmd_index]))
        {
            UBOOT_ERROR("[6M38]Send cmd: FAILED !!!\n");
        }
    }

    UBOOT_TRACE("OK\n");
}

void MDrv_Ursa_6M38_4K2K60_init(void)
{
    MS_U8 u8_4k2k_cmd_init[10][7] =
    {
        {0x20, 0x01, 0x02, 0x00, 0x40, 0xFF, 0x00},
        {0x80, 0x02, 0x00, 0x40, 0x62, 0x51, 0x73},
        {0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x23, 0x1B, 0x10, 0x27, 0xC2, 0xEE, 0xD0},
        {0x23, 0x1B, 0x10, 0x27, 0x40, 0x40, 0xD0},
        {0x33, 0x1B, 0x10, 0x27, 0xC3, 0xEE, 0xD0},
        {0xC1, 0x05, 0x0F, 0x00, 0x08, 0x70, 0x00},
        {0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x33, 0x1C, 0x13, 0x00, 0x04, 0x40, 0x40},
        {0x33, 0x1C, 0x13, 0x00, 0x04, 0x00, 0x40},
    };


    MS_U8 u8_reset_cmd[2][7] =
    {
        {0x43, 0x1B, 0x10, 0x27, 0x02, 0x00, 0x00},
        {0x33, 0x1B, 0x10, 0x27, 0x02, 0xFF, 0x00},
    };

    MS_U8 cmd_index;

    UBOOT_TRACE("IN\n");

    for(cmd_index = 0; cmd_index < 10; cmd_index++)
    {
        UBOOT_DEBUG("[6M38] send 4K2K init cmd_%d--->\n", cmd_index);
        if(!MDrv_Ursa_6M38_Write_Cmd(u8_4k2k_cmd_init[cmd_index]))
        {
            UBOOT_ERROR("[6M38]Send cmd: FAILED !!!\n");
        }
        mdelay(20);
    }
    
    //add reset cmd after 6m38 init to prevent RX lock 
    for(cmd_index = 0; cmd_index < 2; cmd_index++)
    {
        UBOOT_DEBUG("[6M38] send 4K2K reset cmd_%d--->\n", cmd_index);
        if(!MDrv_Ursa_6M38_Write_Cmd(u8_reset_cmd[cmd_index]))
        {
            UBOOT_ERROR("[6M38]Send cmd: FAILED !!!\n");
        }
        mdelay(20);
    }

    UBOOT_TRACE("OK\n");
}

void MDrv_Ursa_6M38_Mute(MS_BOOL bMute)
{
    UBOOT_TRACE("IN\n");
    MS_U8 u8_mute_cmd[2][7] =
    {
        {0x53, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00},
        {0x33, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00}
    };

    if (bMute == TRUE)
    {
        UBOOT_DEBUG("[6M38] send mute cmd\n");
        
        if(!MDrv_Ursa_6M38_Write_Cmd(u8_mute_cmd[0]))
        {
            UBOOT_ERROR("[6M38]Send cmd: FAILED !!!\n");
        }

    }
    else
    {
        UBOOT_DEBUG("[6M38] send unmute cmd\n");
        if(!MDrv_Ursa_6M38_Write_Cmd(u8_mute_cmd[1]))
        {
            UBOOT_ERROR("[6M38]Send cmd: FAILED !!!\n");
        }
    }
    UBOOT_TRACE("OK\n");
}

// for KS2 panel
void MDrv_KS2_Panel_Unlock(void)
{
    MS_U8    pCmdData_unlock1[2] = { 0xfe, 0x31 };
    MS_U8    pCmdData_unlock2[2] = { 0xff, 0x72 };
    MS_BOOL  bRet = FALSE;

    run_command("delay 1200",0);
    printf("delay 1.2 second for INX request \n");

    MApi_SWI2C_Init(Ursa_6M38_I2cBusCfg, URSA_6M38_I2C_BUS_NUM);
    udelay(5000);
    udelay(5000);
    printf("[KS2]Write cmd:%x,%x\n", pCmdData_unlock1[0], pCmdData_unlock1[1]);
    bRet = MDrv_Ursa_6M38_SWI2C_WriteBytes(0x34, 0x00, NULL, 2, pCmdData_unlock1);
    printf("[KS2] send unlock cmd1 return %d\n", bRet);
    udelay(5000);
    udelay(5000);
    printf("[KS2]Write cmd:%x,%x\n", pCmdData_unlock2[0], pCmdData_unlock2[1]);
    bRet = MDrv_Ursa_6M38_SWI2C_WriteBytes(0x34, 0x00, NULL, 2, pCmdData_unlock2);
    printf("[KS2] send unlock cmd2 return %d\n", bRet);
    udelay(5000);
    udelay(5000);
}

void MDrv_KS2_Panel_Bootlogo(MS_U8 is_4k2k_mode)
{
    MS_U8 pCmdData_bootlogo[4];
    if (is_4k2k_mode == 2)
    {
        // 4k2k@60
        pCmdData_bootlogo[0] = 0x01;
        pCmdData_bootlogo[1] = 0x00;
        pCmdData_bootlogo[2] = 0x50;
        pCmdData_bootlogo[3] = 0x04;
        printf("[KS2] send 4K2K@60 bootlog cmd --->\n");
    }
    else if (is_4k2k_mode == 1)
    {
        // 4k2k@30
        pCmdData_bootlogo[0] = 0x03;
        pCmdData_bootlogo[1] = 0x00;
        pCmdData_bootlogo[2] = 0x00;
        pCmdData_bootlogo[3] = 0x0F;
        printf("[KS2] send 4K2K@30 bootlog cmd --->\n");
    }
    else
    {
        // FHD@60
        pCmdData_bootlogo[0] = 0x01;
        pCmdData_bootlogo[1] = 0x00;
        pCmdData_bootlogo[2] = 0x00;
        pCmdData_bootlogo[3] = 0x0F;
        printf("[KS2] send FHD bootlog cmd --->\n");
    }

    printf("[KS2]Write cmd:%x,%x,%x,%x\n", pCmdData_bootlogo[0], pCmdData_bootlogo[1], pCmdData_bootlogo[2], pCmdData_bootlogo[3]);
    MDrv_KS2_Panel_DeviceCmdWrite(pCmdData_bootlogo);
}


void MDrv_KS2_Panel_AutoMuteMode(void)
{
    MS_U8 pCmdData_AutoMuteMode[4] = {0x00, 0x00, 0x57, 0x0F};
    printf("[KS2]Write cmd:%x,%x,%x,%x\n", pCmdData_AutoMuteMode[0], pCmdData_AutoMuteMode[1], pCmdData_AutoMuteMode[2], pCmdData_AutoMuteMode[3]);
    MDrv_KS2_Panel_DeviceCmdWrite(pCmdData_AutoMuteMode);
}

void MDrv_KS2_Panel_DeviceCmdWrite(MS_U8 *reg)
{
    MS_U8  opFrcRegister[9] = {0x00, 0x00, 0x01, 0x0A, 0x9F};
    MS_U8  u8RetBuf[4] = {0x00};
    MS_U8  i, j, k, bRet, done = 0;
    MS_U8  u8SubAddr[1] = {0x0C};
    MS_U32 u32Status;

    memcpy(opFrcRegister + 5, reg, sizeof(MS_U8) * 4);

    MApi_SWI2C_Init(Ursa_6M38_I2cBusCfg, URSA_6M38_I2C_BUS_NUM);
    for(i = 0; !done && (i < 3); i++)
    {
        bRet = MDrv_Ursa_6M38_SWI2C_WriteBytes(0x34, 0x00, NULL, 9, opFrcRegister);
        if(!bRet) {
            printf("I2C Send Performed-Cmd fail!\n");
            continue;
        }
        for(j = 0; !done && (j < 3); j++)
        {
            udelay(15000);
            udelay(15000);
            opFrcRegister[0] = 0x08;
            bRet = MDrv_Ursa_6M38_SWI2C_WriteBytes(0x34, 0x00, NULL, 5, opFrcRegister);
            if(!bRet) {
                printf("I2C Send Performed-Cmd fail!\n");
                continue;
            }
            udelay(1000);
            bRet = MDrv_Ursa_6M38_SWI2C_ReadBytes(0x34, sizeof(u8SubAddr), u8SubAddr, 4, u8RetBuf);
            u32Status = (u8RetBuf[0]<<24) + (u8RetBuf[1]<<16) + (u8RetBuf[2]<<8) + u8RetBuf[3];
            if(u32Status != 0xffffffff) {
                printf("IN8922 not performed!\n");
                continue;
            }

            for(k = 0; !done && (k < 3); k++)
            {
                udelay(15000);
                udelay(15000);
                opFrcRegister[1] = 0x04;
                bRet = MDrv_Ursa_6M38_SWI2C_WriteBytes(0x34, 0x00, NULL, 5, opFrcRegister);
                if(!bRet) {
                    printf("I2C Send Performed-Cmd fail!\n");
                    continue;
                }
                udelay(1000);
                bRet = MDrv_Ursa_6M38_SWI2C_ReadBytes(0x34, sizeof(u8SubAddr), u8SubAddr, 4, u8RetBuf);
                u32Status = (u8RetBuf[0]<<24) + (u8RetBuf[1]<<16) + (u8RetBuf[2]<<8) + u8RetBuf[3];
                if(u32Status != 0x00000000){
                    printf("IN8922 not executed!\n");
                    continue;
                }
                done = 1;
            }
        }
    }

    if(done == TRUE)
    {
        printf("Send IN8922 Command Success \n");
    }

}
#endif //__DRV_URSA_6M38_C__
