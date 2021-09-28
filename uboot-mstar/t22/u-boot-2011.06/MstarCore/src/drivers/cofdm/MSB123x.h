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

#ifndef _MSB123x_H_
#define _MSB123x_H_
#include <msAPI_Tuner.h>

#define EXTSEL extern

#define     DEMOD_SLAVE_ID          0xd2
#define     DEMOD_SLAVE_ID_2          0xA2


/*@ <Definitions> @*/
    // ------------------------------------------------------------
    // Other Define
    // -----------------------------------------------------------



    #define REG_CMD_CTRL                0x809C
    #define REG_DTA_CTRL                0x809D
    #define REG_CMD_ADDR                0x809E
    #define REG_CMD_DATA                0x809F
    #define _REG_START                  REG_CMD_CTRL
    #define _REG_END                    REG_CMD_CTRL
    #define _REG_DRQ                    REG_DTA_CTRL
    #define _REG_FSM                    REG_CMD_CTRL
    #define _REG_ERR                    REG_DTA_CTRL
    #define _BIT_START                  0x02
    #define _BIT_END                    0x01
    #define _BIT_DRQ                    0x01
    #define _BIT_FSM                    0x08
    #define _BIT_ERR                    0x80
    #define MSVIF_0702_ENABLE           1


/*@ </Definitions> @*/

typedef enum
{
	COFDM_FEC_LOCK,
	COFDM_PSYNC_LOCK,
	COFDM_TPS_LOCK,
	COFDM_DCR_LOCK,
	COFDM_AGC_LOCK,
	COFDM_MODE_DET,

} COFDM_LOCK_STATUS;

typedef struct
{
	U8		cmd_code;
	U8		param[64];
} S_CMDPKTREG;

typedef enum
{
    // fw version, check sum
    T2_CHECK_SUM_L      = 0x00,
    T2_CHECK_SUM_H,
    T2_FW_VER_0,
    T2_FW_VER_1,
    T2_FW_VER_2,

    // operation mode
    T2_ZIF_EN           = 0x20,
    T2_RF_AGC_EN,
    T2_HUM_DET_EN,
    T2_DCR_EN,
    T2_IQB_EN,
    T2_IIS_EN,
    T2_CCI_EN,
    T2_LOW_PWR_DET_EN,
    T2_ACI_DET_EN,
    T2_TD_MOTION_EN,
    T2_FD_MOTION_EN,

    // channel tuning param
    T2_BW               = 0x40,
    T2_FC_L             = 0x41,
    T2_FC_H             = 0x42,
    T2_FS_L,
    T2_FS_H,
    T2_ZIF,
    T2_GI,
    T2_ACI_DET_TYPE,
    T2_AGC_REF,			//0x48
    T2_RSSI_REF,
    T2_SNR_TIME_L,
    T2_SNR_TIME_H,
    T2_BER_CMP_TIME_L,
    T2_BER_CMP_TIME_H,
    T2_SFO_CFO_NUM,
    T2_CCI,
    T2_ACI_DET_TH_L,	//0x50
	T2_ACI_DET_TH_H,
	T2_TS_SERIAL = 0x52,
	T2_TS_CLK_RATE = 0x53,
	T2_TS_OUT_INV = 0x54,
	T2_TS_DATA_SWAP = 0x55,
	T2_TDP_CCI_KP,
    T2_CCI_FSWEEP,		//0x57
    T2_TS_ERR_POL,		//0x58


    // dvbt2 lock history
    T2_DVBT2_LOCK_HIS   = 0xF0,

    // splp, mplp releted
    T2_PLP_ID_ARR       = 0x100,
    T2_L1_FLAG          = 0x120,
    T2_PLP_ID,
    T2_GROUP_ID,

    DVBT2_PARAM_LEN

} E_DVBT2_PARAM;


//--------------------------------------------------------------------
EXTSEL BOOLEAN MSB123x_WriteReg(U16 u16Addr, U8 u8Data);
EXTSEL BOOLEAN MSB123x_ReadReg(U16 u16Addr, U8 *pu8Data);
EXTSEL BOOLEAN MSB123x_Reset ( void );
EXTSEL BOOLEAN MSB123x_Active(BOOLEAN bEnable);
EXTSEL BOOLEAN MSB123x_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, U8 param_cnt);
EXTSEL BOOLEAN MSB123x_Power_On_Initialization (BOOLEAN bIsDVBT);
EXTSEL void MSB123x_AFC_Routine (U32 dwFrequency, RF_CHANNEL_BANDWIDTH eBandWidth);
EXTSEL BOOLEAN DTV_GetPlpBitMap(U8* u8PlpBitMap);
EXTSEL BOOLEAN DTV_SetPlpGroupID(U8 u8PlpID, U8 u8GroupID);
EXTSEL BOOLEAN DTV_GetPlpGroupID(U8 u8PlpID, U8* u8GroupID);
EXTSEL U8 MDrv_Demod_GetPlpIDList(void);
EXTSEL U8 MDrv_Demod_GetNextPlpID(U8 u8Index);
EXTSEL BOOLEAN MDrv_Demod_SetPlpID(U8 u8PlpID);
EXTSEL BOOLEAN DTV_Set_Demod_TYPE(EN_DEVICE_DEMOD_TYPE enDemodType);
EXTSEL EN_DEVICE_DEMOD_TYPE DTV_Get_Demod_TYPE(void);
//--------------------------------------------------------------------

#undef EXTSEL
#endif

