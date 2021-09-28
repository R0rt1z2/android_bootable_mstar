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
#include <config.h>
#include <MsTypes.h>
#include "mdrv_voice.h"
#include <MsDebug.h>


//=============================================
// CM4 Halt out
//=============================================
void MDrv_voice_Halt(void)
{
    MHal_CM4_Halt();
}

//=============================================
// CM4 Run
//=============================================
void MDrv_voice_Run(void)
{
    MHal_CM4_Run();
}

//=============================================
//  paganini_ac_on_clk
//  paganini_dc_on_clk
//=============================================
void MDrv_voice_Init(void)
{
#if 0
  // AC_ON
  //  $display("PAGASIM_START");
  WRIU16 (0x032506, 0x0021); // PAGANIN PLL 396 MHz
  WRIU16 (0x032502, 0x0001); // [8]reg_dsppll_pd_dft PAGANINIPLL setup
  mdelay(1);//wait 1

  // AC ON
  WRIU16 (0x0325F0, 0x0001); // XTAIL ON

  // CM4 TSV & SYSTICK
  WRIU16 (0x0325F8, 0x1000); // [8]REG_CLK_CM4_TSV_EN; [10] REG_CLK_CM4_SYS_EN ; [12] REG_CLK_CM4_SYSTICK_GF_SEL

  // IMI CLK  XTAIL -> DFS OFF -> PLL -> DFS_ON
  WRIU16 (0x0325F4, 0x0380); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
  mdelay(1);//wait 1

  WRIU16 (0x0325F2, 0x023F); // [5:0] REG_CLK_IMI_DFS_DIV; [9] REG_CLK_IMI_DFS_NOBYPASS
  WRIU16 (0x0325F2, 0x033F); // [5:0] REG_CLK_IMI_DFS_DIV; [9] REG_CLK_IMI_DFS_NOBYPASS ; [8] REG_CLK_IMI_DFS_UPDATE
  WRIU16 (0x0325F2, 0x023F); // [5:0] REG_CLK_IMI_DFS_DIV; [9] REG_CLK_IMI_DFS_NOBYPASS ; [8] REG_CLK_IMI_DFS_UPDATE
  mdelay(1);//wait 1

  WRIU16 (0x0325F8, 0x1500);  // [8]REG_CLK_CM4_TSV_EN; [10] REG_CLK_CM4_SYS_EN ; [12] REG_CLK_CM4_SYSTICK_GF_SEL

  WRIU16 (0x0325F4, 0x0388); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
  mdelay(1);//wait 1

  WRIU16 (0x0325F4, 0x0398); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL

  WRIU16 (0x0325F2, 0x021F); // [5:0] REG_CLK_IMI_DFS_DIV; [9] REG_CLK_IMI_DFS_NOBYPASS
  WRIU16 (0x0325F2, 0x031F); // [5:0] REG_CLK_IMI_DFS_DIV; [9] REG_CLK_IMI_DFS_NOBYPASS ; [8] REG_CLK_IMI_DFS_UPDATE
  WRIU16 (0x0325F2, 0x021F); // [5:0] REG_CLK_IMI_DFS_DIV; [9] REG_CLK_IMI_DFS_NOBYPASS ; [8] REG_CLK_IMI_DFS_UPDATE

  WRIU16 (0x0325F8, 0x1580); // [3] REG_CLK_CM4_DFS_EN ; [4]REG_CLK_CM4_GF_SEL  ; [7]REG_CLK_CM4_EN
  mdelay(1);//wait 1

  WRIU16 (0x0325F6, 0x023F); // [5:0] REG_CLK_CM4_DFS_DIV; [9] REG_CLK_CM4_DFS_NOBYPASS
  WRIU16 (0x0325F6, 0x033F); // [5:0] REG_CLK_CM4_DFS_DIV; [9] REG_CLK_CM4_DFS_NOBYPASS ; [8] REG_CLK_CM4_DFS_UPDATE
  WRIU16 (0x0325F6, 0x023F); // [5:0] REG_CLK_CM4_DFS_DIV; [9] REG_CLK_CM4_DFS_NOBYPASS ; [8] REG_CLK_CM4_DFS_UPDATE

  WRIU16 (0x0325F8, 0x1590); // [3] REG_CLK_CM4_DFS_EN ; [4]REG_CLK_CM4_GF_SEL  ; [7]REG_CLK_CM4_EN
  mdelay(1);//wait 1

  WRIU16 (0x0325F8, 0x1598); // [3] REG_CLK_CM4_DFS_EN ; [4]REG_CLK_CM4_GF_SEL  ; [7]REG_CLK_CM4_EN

  WRIU16 (0x0325F6, 0x021F); // [5:0] REG_CLK_CM4_DFS_DIV; [9] REG_CLK_CM4_DFS_NOBYPASS
  WRIU16 (0x0325F6, 0x031F); // [5:0] REG_CLK_CM4_DFS_DIV; [9] REG_CLK_CM4_DFS_NOBYPASS ; [8] REG_CLK_CM4_DFS_UPDATE
  WRIU16 (0x0325F6, 0x021F); // [5:0] REG_CLK_CM4_DFS_DIV; [9] REG_CLK_CM4_DFS_NOBYPASS ; [8] REG_CLK_CM4_DFS_UPDATE

  WRIU16(0x032538, 0x0000);
  CLRREG16(GET_BASE_ADDR_BY_BANK(BASE_REG_RIU_PA, 0x32526), 0x8000);
  // DC_ON
  // INSREG16 (0x112ca4, 0x02, 0x02); // bdma XTAIL Clock
  INSREG16 (GET_REG16_ADDR(BASE_REG_RIU_PA, 0x112ca4), 0x02, 0x02); // bdma XTAIL Clock

  WRIU16 (0x112e52, 0x0f);
  WRIU16 (0x112e52, 0x1f);
  WRIU16 (0x112e52, 0x3f);
  WRIU16 (0x112e52, 0x1f);
  WRIU16 (0x112e52, 0x0f);
  WRIU16 (0x112e52, 0x00);

/*
  WRIU16(0x0325F4, 0x0198); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
  WRIU16(0x0325F4, 0x0188); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
  WRIU16(0x0325F4, 0x0189); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
  WRIU16(0x0325F4, 0x0199); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
  WRIU16(0x0325F4, 0x0399); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
*/
#else
    MDrv_VOC_MBX_Init(100);
    MHal_CM4_Init();
    MHal_BDMA_Init();
    MHal_CM4_DcOn();
#endif
}

void MDrv_voice_DcOn (void)
{
#if 0
  WRIU16(0x0325F4, 0x0198); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
  WRIU16(0x0325F4, 0x0188); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
  WRIU16(0x0325F4, 0x0189); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
  WRIU16(0x0325F4, 0x0199); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
  WRIU16(0x0325F4, 0x0399); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
#else
  MHal_CM4_DcOn();
  MHal_BDMA_Init();
  MHAL_VOC_MBX_Init(E_MBX_ROLE_HK);
#endif
}

void MDrv_voice_DcOff (void)
{
#if 0
  WRIU16(0x0325F4, 0x0199); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL  [9:8] 0/1 -> [4] 0 -> [1:0] 0/1 -> [4] 1 ->  [9:8] 3
  WRIU16(0x0325F4, 0x0189); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
  WRIU16(0x0325F4, 0x0188); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
  WRIU16(0x0325F4, 0x0198); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
  WRIU16(0x0325F4, 0x0398); // IMI CLK [9:8]REG_CLK_IMI_GF_PRE_SEL ; [7] REG_CLK_IMI_EN ; [4]REG_CLK_IMI_GF_SEL
#else
  MHal_CM4_DcOff();
#endif
}

//=============================================
//  VocEnableVp();
//  VocConfigVq();
//  VocEnableVq();
//=============================================
void MDrv_voice_VQ_ONOFF(void)
{
#if 0
  // OUTREG16(0x1f206700, 0x0001);     //Bank 103380,REG 0
  // OUTREG16(0x1f206704, 0x0105);     //Bank 103380,REG 1
  // OUTREG16(0x1f206708, 0x0001);     //Bank 103380,REG 2
  // OUTREG16(0x1f20670c, 0x0000);     //Bank 103380,REG 3
  // SETREG16(0x1f200A88, 0x0004);     //Bank 100540,REG 2
  // CLRREG16(0x1f200A88, 0x0004);     //Bank 100540,REG 2
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);     //Bank 103380,REG 0
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), 0x0105);     //Bank 103380,REG 1
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), 0x0001);     //Bank 103380,REG 2
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), 0x0000);     //Bank 103380,REG 3
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);     //Bank 100540,REG 2
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);     //Bank 100540,REG 2
  mdelay(10);
  // OUTREG16(0x1f206700, 0x0001);
  // OUTREG16(0x1f206704, 0x0104);
  // OUTREG16(0x1f206708, 0x0002);
  // OUTREG16(0x1f20670c, 0x0000);
  // SETREG16(0x1f200A88, 0x0004);
  // CLRREG16(0x1f200A88, 0x0004);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);     //Bank 103380,REG 0
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), 0x0104);     //Bank 103380,REG 1
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), 0x0002);     //Bank 103380,REG 2
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), 0x0000);     //Bank 103380,REG 3
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);     //Bank 100540,REG 2
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);     //Bank 100540,REG 2
  mdelay(10);
  // OUTREG16(0x1f206700, 0x0001);
  // OUTREG16(0x1f206704, 0x0103);
  // OUTREG16(0x1f206708, 0x0001);
  // OUTREG16(0x1f20670c, 0x0000);
  // SETREG16(0x1f200A88, 0x0004);
  // CLRREG16(0x1f200A88, 0x0004);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);     //Bank 103380,REG 0
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), 0x0103);     //Bank 103380,REG 1
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), 0x0001);     //Bank 103380,REG 2
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), 0x0000);     //Bank 103380,REG 3
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);     //Bank 100540,REG 2
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);     //Bank 100540,REG 2
  mdelay(10);
#else

#endif
}
//=============================================
//  VOC_REG_HW_AEC_ENABLE();
//=============================================
MS_S32 MDrv_voice_EnableHwAEC(int nEn)
{
#if 0
  MBX_MSG_e MSG_TYPE = E_MBX_MSG_AEC_ENABLE;
  MS_U8 para_cnt = 1;
  MS_U16 para1 = ((para_cnt & 0x00FF) << 8) | (MSG_TYPE & 0x00FF);
  MS_U16 para2 = ( nEn & 0x00FF) ;
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), para1);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), para2);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), 0x0000);
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  UBOOT_DEBUG("%s: para1: 0x%04x,para2: 0x%04x\n",__FUNCTION__,para1,para2);
  mdelay(10);
#else
    MBX_Result tResult;
    MBX_Msg tMsg;
    tMsg.eRoleID = E_MBX_ROLE_CP;
    tMsg.eMsgType = E_MBX_MSG_TYPE_NORMAL;
    tMsg.u8MsgClass = E_MBX_CLASS_VOC;
    tMsg.u8Index = E_MBX_MSG_AEC_ENABLE;

    tMsg.u8ParameterCount = 1;
    tMsg.u8Parameters[0] = nEn;

UBOOT_DEBUG("in %s, nEn: %d\n",__FUNCTION__,nEn);

    tResult=MDrv_VOC_MBX_SendMsg(&tMsg);
    if(tResult!=E_MBX_SUCCESS)
    {
        UBOOT_DEBUG("enable hw aec error : %d\n",tResult);
    }
    return tResult;
#endif
}

//=============================================
//  VOC_REG_I2S_ENABLE();
//=============================================
MS_S32 MDrv_voice_EnableI2S(int nEn)
{
#if 0
  MBX_MSG_e MSG_TYPE = E_MBX_MSG_I2S_ENABLE;
  MS_U8 para_cnt = 1;
  MS_U16 para1 = ((para_cnt & 0x00FF) << 8) | (MSG_TYPE & 0x00FF);
  MS_U16 para2 = ( nEn & 0x00FF) ;
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), para1);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), para2);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), 0x0000);
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  UBOOT_DEBUG("%s: para1: 0x%04x,para2: 0x%04x\n",__FUNCTION__,para1,para2);
  mdelay(10);
#else
    MBX_Result tResult;
    MBX_Msg tMsg;
    tMsg.eRoleID = E_MBX_ROLE_CP;
    tMsg.eMsgType = E_MBX_MSG_TYPE_NORMAL;
    tMsg.u8MsgClass = E_MBX_CLASS_VOC;
    tMsg.u8Index = E_MBX_MSG_I2S_ENABLE;

    tMsg.u8ParameterCount = 1;
    tMsg.u8Parameters[0] = nEn;
    UBOOT_DEBUG("in %s, nEn: %d\n",__FUNCTION__,nEn);

    tResult=MDrv_VOC_MBX_SendMsg(&tMsg);
    if(tResult!=E_MBX_SUCCESS)
    {
        UBOOT_DEBUG("enable i2s error : %d\n",tResult);
    }
    return tResult;
#endif
}
//=============================================
//  VOC_REG_CONFIG_MIC();
//=============================================
MS_S32 MDrv_voice_ConfigMIC(MIC_CONFIG_S sConfig)
{
#if 0
  MBX_MSG_e MSG_TYPE = E_MBX_MSG_MIC_CONFIG;
  MS_U8 para_cnt = 3;
  MS_U16 para1 = ((para_cnt & 0x00FF) << 8) | (MSG_TYPE & 0x00FF);
  MS_U16 para2 = ((E_MBX_AUD_BITWIDTH_MAX & 0x00FF) << 8) | (MicNum & 0x00FF);
  MS_U16 para3 = ( E_MBX_AUD_RATE_16K & 0x00FF) ;
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), para1);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), para2);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), para3);
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  UBOOT_DEBUG("%s: para1: 0x%x,para3: 0x%x,para3: 0x%x\n",__FUNCTION__,para1,para2,para3);
  mdelay(10);
  return 0;
#else
    MBX_Result tResult;
    MBX_Msg tMsg;
    tMsg.eRoleID = E_MBX_ROLE_CP;
    tMsg.eMsgType = E_MBX_MSG_TYPE_NORMAL;
    tMsg.u8MsgClass = E_MBX_CLASS_VOC;
    tMsg.u8Index = E_MBX_MSG_MIC_CONFIG;

    tMsg.u8ParameterCount = 3;
    tMsg.u8Parameters[0] = sConfig.mic_num;
    tMsg.u8Parameters[1] = E_MBX_AUD_RATE_16K;
    tMsg.u8Parameters[2] = sConfig.mic_bitwidth;

    UBOOT_DEBUG("in %s, MicNum: %d, mic_bitwidth: %d\n",__FUNCTION__,sConfig.mic_num,sConfig.mic_bitwidth);

    tResult=MDrv_VOC_MBX_SendMsg(&tMsg);
    if(tResult!=E_MBX_SUCCESS)
    {
        UBOOT_DEBUG("config dmic number : %d\n",tResult);
    }

    return tResult;
#endif
}
//=============================================
//  VOC_REG_CONFIG_VP();
//=============================================
MS_S32 MDrv_voice_ConfigVp(VP_CONFIG_S sConfig)
{
#if 0
  if(sConfig.nScale>16)
    sConfig.nScale = 16;
  if(sConfig.nScale<1)
    sConfig.nScale = 1;
  MBX_MSG_e MSG_TYPE = E_MBX_MSG_VP_CONFIG;
  MS_U8 para_cnt = 1;
  MS_U16 para1 = ((para_cnt & 0x00FF) << 8) | (MSG_TYPE & 0x00FF);
  MS_U16 para2 = ( sConfig.nScale & 0x00FF) ;
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), para1);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), para2);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), 0x0000);
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  UBOOT_DEBUG("%s: para1: 0x%04x,para2: 0x%04x\n",__FUNCTION__,para1,para2);
  mdelay(10);
#else
    MBX_Result tResult;
    MBX_Msg tMsg;
    tMsg.eRoleID = E_MBX_ROLE_CP;
    tMsg.eMsgType = E_MBX_MSG_TYPE_NORMAL;
    tMsg.u8MsgClass = E_MBX_CLASS_VOC;
    tMsg.u8Index = E_MBX_MSG_VP_CONFIG;


    if(sConfig.nScale>16)
        sConfig.nScale = 16;
    if(sConfig.nScale<1)
        sConfig.nScale = 1;
    tMsg.u8ParameterCount = 1;
    tMsg.u8Parameters[0] = sConfig.nScale;

    UBOOT_DEBUG("in %s, sConfig.nScale: %d\n",__FUNCTION__,sConfig.nScale);

    tResult=MDrv_VOC_MBX_SendMsg(&tMsg);
    if(tResult!=E_MBX_SUCCESS)
    {
        UBOOT_DEBUG("config vq error : %d\n",tResult);
    }
    return tResult;
#endif
}
//=============================================
//  VOC_REG_ENABLE_VP();
//=============================================
MS_S32 MDrv_voice_EnableVp(int nEn)
{
#if 0
  MBX_MSG_e MSG_TYPE = E_MBX_MSG_VP_ENABLE;
  MS_U8 para_cnt = 1;
  MS_U16 para1 = ((para_cnt & 0x00FF) << 8) | (MSG_TYPE & 0x00FF);
  MS_U16 para2 = ( nEn & 0x00FF) ;
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), para1);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), para2);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), 0x0000);
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  UBOOT_DEBUG("%s: para1: 0x%04x,para2: 0x%04x\n",__FUNCTION__,para1,para2);
  mdelay(10);
#else
    MBX_Result tResult;
    MBX_Msg tMsg;
    tMsg.eRoleID = E_MBX_ROLE_CP;
    tMsg.eMsgType = E_MBX_MSG_TYPE_NORMAL;
    tMsg.u8MsgClass = E_MBX_CLASS_VOC;
    tMsg.u8Index = E_MBX_MSG_VP_ENABLE;

    tMsg.u8ParameterCount = 1;
    tMsg.u8Parameters[0] = nEn;

    UBOOT_DEBUG("in %s, nEn: %d\n",__FUNCTION__,nEn);

    tResult=MDrv_VOC_MBX_SendMsg(&tMsg);
    if(tResult!=E_MBX_SUCCESS)
    {
        UBOOT_DEBUG("enable vp error : %d\n",tResult);
    }
    return tResult;
#endif
}

//=============================================
//  VOC_REG_ENABLE_UART();
//=============================================
void MDrv_voice_EnableUart(int nEn)
{
    MS_U16 u16Val;
    if (nEn)
    {
        UBOOT_DEBUG("in %s Uart enable \n",__FUNCTION__);
        u16Val = INREG16(REG_PM_UART_PAD);
        u16Val &= ~(7<<6);
        OUTREG16(REG_PM_UART_PAD, (u16Val |(0x1<<6)));
    }
    else
    {
        UBOOT_DEBUG("in %s Uart disable \n",__FUNCTION__);
        u16Val = INREG16(REG_PM_UART_PAD);
        u16Val &= ~(7<<6);
        OUTREG16(REG_PM_UART_PAD, (u16Val |(0x0<<6)));
    }
}
//=============================================
//  VOC_REG_HPF_CONFIG();
//=============================================
MS_S32 MDrv_voice_ConfigHpf(int nConfig)
{
#if 0
  MBX_MSG_e MSG_TYPE = E_MBX_MSG_HPF_CONFIG;
  MS_U8 para_cnt = 1;
  MS_U16 para1 = ((para_cnt & 0x00FF) << 8) | (MSG_TYPE & 0x00FF);
  MS_U16 para2 = ( nConfig & 0x00FF) ;
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), para1);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), para2);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), 0x0000);
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  UBOOT_DEBUG("%s: para1: 0x%04x,para2: 0x%04x\n",__FUNCTION__,para1,para2);
  mdelay(10);
#else
    MBX_Result tResult;
    MBX_Msg tMsg;
    tMsg.eRoleID = E_MBX_ROLE_CP;
    tMsg.eMsgType = E_MBX_MSG_TYPE_NORMAL;
    tMsg.u8MsgClass = E_MBX_CLASS_VOC;
    tMsg.u8Index = E_MBX_MSG_HPF_CONFIG;

    tMsg.u8ParameterCount = 1;
    tMsg.u8Parameters[0] = (MS_U8)nConfig;
    UBOOT_DEBUG("in %s, nConfig: %d\n",__FUNCTION__,nConfig);
    tResult=MDrv_VOC_MBX_SendMsg(&tMsg);
    if(tResult!=E_MBX_SUCCESS)
    {
        UBOOT_DEBUG("Config Hpf error : %d\n",tResult);
    }
    return tResult;
#endif
}
//=============================================
//  VOC_REG_HPF_ENABLE();
//=============================================
MS_S32 MDrv_voice_EnableHpf(int nStage)
{
#if 0
  MBX_MSG_e MSG_TYPE = E_MBX_MSG_HPF_ENABLE;
  MS_U8 para_cnt = 1;
  MS_U16 para1 = ((para_cnt & 0x00FF) << 8) | (MSG_TYPE & 0x00FF);
  MS_U16 para2 = ( nStage & 0x00FF) ;
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), para1);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), para2);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), 0x0000);
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  UBOOT_DEBUG("%s: para1: 0x%04x,para2: 0x%04x\n",__FUNCTION__,para1,para2);
  mdelay(10);
#else
    MBX_Result tResult;
    MBX_Msg tMsg;
    tMsg.eRoleID = E_MBX_ROLE_CP;
    tMsg.eMsgType = E_MBX_MSG_TYPE_NORMAL;
    tMsg.u8MsgClass = E_MBX_CLASS_VOC;
    tMsg.u8Index = E_MBX_MSG_HPF_ENABLE;

    tMsg.u8ParameterCount = 1;
    tMsg.u8Parameters[0] = (MS_U8)nStage;
    UBOOT_DEBUG("in %s, nStage: %d\n",__FUNCTION__,nStage);
    tResult=MDrv_VOC_MBX_SendMsg(&tMsg);
    if(tResult!=E_MBX_SUCCESS)
    {
        UBOOT_DEBUG("enable Hpf error : %d\n",tResult);
    }
    return tResult;
#endif
}

//=============================================
//  VOC_REG_DA_ENABLE();
//=============================================
MS_S32 MDrv_voice_EnableDa(int nEn)
{
#if 0
  MBX_MSG_e MSG_TYPE = E_MBX_MSG_CUS0_ENABLE;
  MS_U8 para_cnt = 1;
  MS_U16 para1 = ((para_cnt & 0x00FF) << 8) | (MSG_TYPE & 0x00FF);
  MS_U16 para2 = ( nEn & 0x00FF) ;
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), para1);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), para2);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), 0x0000);
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  UBOOT_DEBUG("%s: para1: 0x%04x,para2: 0x%04x\n",__FUNCTION__,para1,para2);
  mdelay(10);
#else
    MBX_Result tResult;
    MBX_Msg tMsg;
    tMsg.eRoleID = E_MBX_ROLE_CP;
    tMsg.eMsgType = E_MBX_MSG_TYPE_NORMAL;
    tMsg.u8MsgClass = E_MBX_CLASS_VOC;
    tMsg.u8Index = E_MBX_MSG_CUS0_ENABLE;

    tMsg.u8ParameterCount = 1;
    tMsg.u8Parameters[0] = nEn;
    UBOOT_DEBUG("in %s, nEn: %d\n",__FUNCTION__,nEn);
    tResult=MDrv_VOC_MBX_SendMsg(&tMsg);
    if(tResult!=E_MBX_SUCCESS)
    {
        UBOOT_DEBUG("enable da error : %d\n",tResult);
    }
    return tResult;
#endif
}
//=============================================
//  VOC_REG_VQ_CONFIG();
//=============================================
MS_S32 MDrv_voice_ConfigVq(VQ_CONFIG_S sConfig)
{
#if 0
  MBX_MSG_e MSG_TYPE = E_MBX_MSG_VQ_CONFIG;
  if(sConfig.nMode>=E_MBX_VQ_MODE_MAX)
    sConfig.nMode = 0;
  MS_U8 para_cnt = 1;
  MS_U16 para1 = ((para_cnt & 0x00FF) << 8) | (MSG_TYPE & 0x00FF);
  MS_U16 para2 = ( sConfig.nMode & 0x00FF) ;
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), para1);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), para2);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), 0x0000);
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  UBOOT_DEBUG("%s: para1: 0x%04x,para2: 0x%04x\n",__FUNCTION__,para1,para2);
  mdelay(10);
  return 0;
#else
    MBX_Result tResult;
    MBX_Msg tMsg;
    memset(&tMsg,0,sizeof(MBX_Msg));
    tMsg.eRoleID = E_MBX_ROLE_CP;
    tMsg.eMsgType = E_MBX_MSG_TYPE_NORMAL;
    tMsg.u8MsgClass = E_MBX_CLASS_VOC;
    tMsg.u8Index = E_MBX_MSG_VQ_CONFIG;


    if(sConfig.nMode>=E_MBX_VQ_MODE_MAX)
        sConfig.nMode = 0;
    tMsg.u8ParameterCount = 1;
    tMsg.u8Parameters[0] = sConfig.nMode;

    UBOOT_DEBUG("in %s, sConfig Mode: %d\n",__FUNCTION__,sConfig.nMode);

    tResult=MDrv_VOC_MBX_SendMsg(&tMsg);
    if(tResult!=E_MBX_SUCCESS)
    {
        UBOOT_DEBUG("config vq error : %d\n",tResult);
    }
    return tResult;
#endif
}
//=============================================
//  VOC_REG_VQ_ENABLE();
//=============================================
MS_S32 MDrv_voice_EnableVq(int nEn)
{
#if 0
  MBX_MSG_e MSG_TYPE = E_MBX_MSG_VQ_ENABLE;
  MS_U8 para_cnt = 1;
  MS_U16 para1 = ((para_cnt & 0x00FF) << 8) | (MSG_TYPE & 0x00FF);
  MS_U16 para2 = ( nEn & 0x00FF) ;
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), para1);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), para2);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), 0x0000);
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  UBOOT_DEBUG("%s: para1: 0x%04x,para2: 0x%04x\n",__FUNCTION__,para1,para2);
  mdelay(10);
#else
    MBX_Result tResult;
    MBX_Msg tMsg;
    tMsg.eRoleID = E_MBX_ROLE_CP;
    tMsg.eMsgType = E_MBX_MSG_TYPE_NORMAL;
    tMsg.u8MsgClass = E_MBX_CLASS_VOC;
    tMsg.u8Index = E_MBX_MSG_VQ_ENABLE;

    tMsg.u8ParameterCount = 1;
    tMsg.u8Parameters[0] = nEn;

    UBOOT_DEBUG("in %s, nEn: %d\n",__FUNCTION__,nEn);

    tResult=MDrv_VOC_MBX_SendMsg(&tMsg);
    if(tResult!=E_MBX_SUCCESS)
    {
        UBOOT_DEBUG("enable vq error : %d\n",tResult);
    }
    return tResult;
#endif
}
//=============================================
//  VOC_REG_SIGEN_ENABLE();
//=============================================
MS_S32 MDrv_voice_EnableSinegen(int bEn)
{
#if 0
  MBX_MSG_e MSG_TYPE = E_MBX_MSG_SIGEN_ENABLE;
  MS_U8 para_cnt = 1;
  MS_U16 para1 = ((para_cnt & 0x00FF) << 8) | (MSG_TYPE & 0x00FF);
  MS_U16 para2 = ( bEn & 0x00FF) ;
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), para1);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), para2);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), 0x0000);
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  UBOOT_DEBUG("%s: para1: 0x%04x,para2: 0x%04x\n",__FUNCTION__,para1,para2);
  mdelay(10);
#else
    MBX_Result tResult;
    MBX_Msg tMsg;
    tMsg.eRoleID = E_MBX_ROLE_CP;
    tMsg.eMsgType = E_MBX_MSG_TYPE_NORMAL;
    tMsg.u8MsgClass = E_MBX_CLASS_VOC;
    tMsg.u8Index = E_MBX_MSG_SIGEN_ENABLE;

    tMsg.u8ParameterCount = 1;
    tMsg.u8Parameters[0] = bEn;

    UBOOT_DEBUG("in %s, bEn: %d\n",__FUNCTION__,bEn);

    tResult=MDrv_VOC_MBX_SendMsg(&tMsg);
    if(tResult!=E_MBX_SUCCESS)
    {
        UBOOT_DEBUG("enable sinegen error : %d\n",tResult);
    }
    return tResult;
#endif
}
//=============================================
//  VOC_REG_DMIC_GAIN();
//=============================================
MS_S32 MDrv_voice_Gain(int nGain)
{
#if 0
  // OUTREG16(0x1f206700, 0x0001);
  // OUTREG16(0x1f206704, 0x010A);
  // OUTREG16(0x1f206708, 0x0005);
  // OUTREG16(0x1f20670c, 0x0000);
  // SETREG16(0x1f200A88, 0x0004);
  // CLRREG16(0x1f200A88, 0x0004);
  MBX_MSG_e MSG_TYPE = E_MBX_MSG_MIC_GAIN;
  MS_U8 para_cnt = 1;
  MS_U16 para1 = ((para_cnt & 0x00FF) << 8) | (MSG_TYPE & 0x00FF);
  MS_U16 para2 = ( nGain & 0x00FF);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x00), 0x0001);
  //OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), 0x010A);
  //OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), 0x0005);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x01), para1);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x02), para2);
  OUTREG16(GET_REG16_ADDR(REG_ADDR_CM4_MBX, 0x03), 0x0000);
  SETREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  CLRREG16(GET_REG16_ADDR(REG_ADDR_CM4_CPUINT, 0x02), 0x0004);
  UBOOT_DEBUG("%s: para1: 0x%04x,para2: 0x%04x\n",__FUNCTION__,para1,para2);
  mdelay(10);
#else
    MBX_Result tResult;
    MBX_Msg tMsg;
    tMsg.eRoleID = E_MBX_ROLE_CP;
    tMsg.eMsgType = E_MBX_MSG_TYPE_NORMAL;
    tMsg.u8MsgClass = E_MBX_CLASS_VOC;
    tMsg.u8Index = E_MBX_MSG_SW_GAIN;

    tMsg.u8ParameterCount = 1;
    tMsg.u8Parameters[0] = (MS_U8)nGain;

    UBOOT_DEBUG("in %s, nGain: %d\n",__FUNCTION__,nGain);

    tResult=MDrv_VOC_MBX_SendMsg(&tMsg);
    if(tResult!=E_MBX_SUCCESS)
    {
        UBOOT_DEBUG("config sw gain : %d\n",tResult);
    }
    return tResult;
#endif
}
#define CHECK_PATTERN           0x87878787
#define KEYWORD                 "[BR:"
#define KEYWORD_END             ']'
#define BDMA_RETRY_TIMES        5
#define PATTERN_RETRY_TIMES     100
int MDrv_voice_verifyHeader(char* pHeader)
{
    unsigned int nTagSize = *(unsigned int*)(pHeader+0);
    unsigned int nDataSize = *(unsigned int*)(pHeader+nTagSize);
    char Pattern_start[5]={0};
    char *branch = NULL;
    int strIndex = 0;
    int ret = 0;
    int i = 0;
    if((nDataSize == 0) || (nTagSize > 0x100))
    {
        return ret;
    }
    strncpy(Pattern_start,KEYWORD,sizeof(KEYWORD));
    for(i = 0;i < nTagSize; i++)
    {
        if((strIndex==0) && (strncmp(pHeader+i,Pattern_start,strlen(KEYWORD)) == 0))
        {
            UBOOT_DEBUG("[%x][%c%c%c]\n",i,*(pHeader+i),*(pHeader+i+1),*(pHeader+i+2));
            i+=strlen(KEYWORD);
            strIndex = i;
        }
        else if((strIndex!=0) && ((*(pHeader+i))==KEYWORD_END))
        {
            branch = malloc(sizeof(char)*(i-strIndex+1));
            memset(branch,'\0',sizeof(char)*(i-strIndex+1));
            memcpy(branch,pHeader+strIndex,i-strIndex);
            UBOOT_DEBUG(" verify header is pass, branch Name:[%s]\n",branch);
            ret = 1;
            free(branch);
            break;
        }
    }
    return ret;
}
int MDrv_voice_updateFw(char* pBufferAddr, unsigned int binsize)
{
    unsigned int nTagSize = *(unsigned int*)(pBufferAddr+0);
    unsigned int nDataSize = *(unsigned int*)(pBufferAddr+nTagSize);
    unsigned int nDataOffset = nTagSize+4;
    unsigned int nCRC  = *(unsigned int*)(pBufferAddr+nTagSize-4); //last 4 bytes will be CRC
    unsigned int nRetry = 0;
    unsigned int dramCRC,sramCRC;
    MS_PHYADDR msphyAddr=0;
    BOOL ret = FALSE;
    if(binsize > 0)
    {
        msphyAddr=MsOS_VA2PA((MS_PHYADDR)pBufferAddr);
        UBOOT_DEBUG("msphyAddr=0x%lx \n",(MS_U32)msphyAddr);
        ret = MDrv_BDMA_Copy(msphyAddr, 0x0,binsize, E_BDMA_HKtoCP);
        if(!ret)
        {
            UBOOT_DEBUG(" CM4 BDMA LOAD CODE FAIL CODE (%d)!!!\n",ret);
            return -1;
        }
    }else{
        UBOOT_DEBUG("nTagSize: %x nDataSize: %x,nDataOffset: %x, nCRC: %x\n", nTagSize, nDataSize, nDataOffset, nCRC);
        msphyAddr=MsOS_VA2PA((MS_PHYADDR)pBufferAddr);
        UBOOT_DEBUG("msphyAddr: %x\n", msphyAddr);
#if 0
        nRetry = 0;
        while(nRetry++<BDMA_RETRY_TIMES)
        {
            dramCRC = (unsigned int) MDrv_BDMA_Crc32(msphyAddr+nDataOffset, nDataSize,E_BDMA_DEV_MIU);
            UBOOT_DEBUG("dramCRC: %x nCRC: %x\n", dramCRC,nCRC);
            if(nCRC == dramCRC)
            {
                break;
            }
        }
        if(nRetry>BDMA_RETRY_TIMES)
        {
            UBOOT_DEBUG("in %s, DRAM CRC check failed!!\n",__FUNCTION__);
            return -1;
        }
        UBOOT_DEBUG("dramCRC: %x fw start( msphyAddr+ offset): %x\n", dramCRC, msphyAddr+nDataOffset);
#endif
        nRetry = 0;
        while(nRetry++<BDMA_RETRY_TIMES)
        {
            MDrv_BDMA_Copy(msphyAddr+nDataOffset, 0x0, nDataSize, E_BDMA_HKtoCP);
            sramCRC = (unsigned int) MDrv_BDMA_Crc32(0x0, nDataSize,E_BDMA_DEV_IMI);
            UBOOT_DEBUG("nCRC: %x sramCRC: %x\n", nCRC,sramCRC);
            if(sramCRC == nCRC)
            {
                break;
            }
        }
        if(nRetry>BDMA_RETRY_TIMES)
        {
            UBOOT_DEBUG("in %s, load code failed!!\n",__FUNCTION__);
            return -1;
        }
        MDrv_voice_Run();
        /* wait CM4 ready */
        nRetry = 0;
        while(nRetry++<PATTERN_RETRY_TIMES)
        {
            if(MDrv_BDMA_PatternSearch(0+nDataSize-4, 4, CHECK_PATTERN, E_BDMA_DEV_IMI))
            {
                UBOOT_DEBUG("chech pattern: pass\n");
                break;
            }
            mdelay(1);
        }

        if(nRetry > PATTERN_RETRY_TIMES)
        {
            UBOOT_DEBUG("out %s, CM4 not work!!\n",__FUNCTION__);
            return -1;
        }
    }
    return 0;
}
