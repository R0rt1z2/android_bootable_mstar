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

#include <config.h>
#include <malloc.h>
#include "drvFCIE_config.h"
#include "drvFCIE5.h"
#include "DrvSd.h"


static char *gStrR1CardStatus[] = {	"31_OUT_OF_RANGE",
									"30_ADDR_ERROR",
									"29_BLK_LEN_ERROR",
									"28_ERASE_SEQ_ERROR",
									"27_ERASE_PARAM",
									"26_WP_VIOLAION",
									"25_CARD_IS_LOCKED",
									"24_UNLOCK_LOCK_FAIL",
									"23_COM_CRC_ERROR",
									"22_ILLEGAL_COMMAND",
									"21_CARD_ECC_FAIL",
									"20_CARD_CTRLER_ERR",
									"19_UNKONW_ERROR",
									"18_RESERVED",
									"17_RESERVED",
									"16_CSD_OVERWRITE",
									"15_WP_ERASE_SKIP",
									"14_CARD_ECC_DISABLE",
									"13_ERASE_RESET  ",
									"12_CURRENT_STATE",
									"11_CURRENT_STATE",
									"10_CURRENT_STATE",
									"9_CURRENT_STATE",
									"8_READY_FOR_DATA",
									"7_",
									"6_",
									"5_APP_CMD",
									"4_RESERVED_SDIO",
									"3_AKE_SEQ_ERROR",
									"2_",
									"1_",
									"0_" };

static char *gStrR1CurrentState[] = {	"0(Idle)",
										"1(ready)",
										"2(ident)",
										"3(stby)",
										"4(tran)",
										"5(data)",
										"6(rcv)",
										"7(prg)",
										"8(dis)",
										"9(reserved)",
										"10(reserved)",
										"11(reserved)",
										"12(reserved)",
										"13(reserved)",
										"14(reserved)",
										"15(reserved)" };

void DrvSD_CheckR1(U32 u32CardStatusR1)
{
	S32 s32_i = 0;

	//printf("%s(%04Xh)\n", __FUNCTION__, u32CardStatusR1);

	printf("card state = %s, ", gStrR1CurrentState[(u32CardStatusR1&0x1E00)>>9]);

	for(s32_i=31; s32_i>=0; s32_i--)
	{
		//printf("u32_i: %d\n", s32_i);
		if( s32_i<=12 && s32_i>= 9 ) continue;
		if( u32CardStatusR1 & (1UL<<s32_i) )
		{
			//printf("%d error\n", s32_i);
			printf("%s ", gStrR1CardStatus[31-s32_i]);
		}
	}

	printf("\n");
}


U32 DrvSd_CMD17(U32 u32_BlkAddr, U8 *pu8_DataBuf)
{
	U32 u32_err = 0;
	U32 u32_arg;
	//U8  u8_retry = 0;
	//U32 u32PhyAddr = 0;
    CMD_RSP_INFO CmdRspInfo;
	U32 u32_dma_addr;

	memset(&CmdRspInfo, 0, sizeof(CMD_RSP_INFO));

	u32_arg =  u32_BlkAddr;

	u32_dma_addr = HalSdio_TranslateVirtAddr((U32)pu8_DataBuf, 0x200);

	CmdRspInfo.Command.Cmd.Index = 17;
	CmdRspInfo.Command.Cmd.Arg = U32BEND2LEND(u32_arg);
	//CmdRspInfo.ClockStop = CLK_KEEP;
    CmdRspInfo.CmdType = CMD_TYPE_ADTC;
    CmdRspInfo.RspType = RSP_TYPE_R1;
	CmdRspInfo.ReadWriteDir = DIR_R;
	CmdRspInfo.DataPath = PATH_DMA;
	CmdRspInfo.BusWidth = BUS_4_BITS;
	CmdRspInfo.MiuAddr = u32_dma_addr;
	CmdRspInfo.pBufferAddr = NULL; // DMA don't need this field
	CmdRspInfo.BlockSize = 512;
	CmdRspInfo.BlockCount = 1;

    u32_err = HalSdio_SendCommand(&CmdRspInfo);

	if(u32_err)
	{
		goto ErrorHandle;
	}

	return 0;

ErrorHandle:

	return u32_err;
}

U32 DrvSd_CMD18(U32 u32_BlkAddr, U16 u32BlkNum, U8 *pu8_DataBuf)
{
	U32 u32_err = 0;
	U32 u32_arg;
    CMD_RSP_INFO CmdRspInfo;
	U32 u32_dma_addr;

	memset(&CmdRspInfo, 0, sizeof(CMD_RSP_INFO));

	u32_arg =  u32_BlkAddr;

	u32_dma_addr = HalSdio_TranslateVirtAddr((U32)pu8_DataBuf, 0x200UL*u32BlkNum);

	CmdRspInfo.Command.Cmd.Index = 18;
	CmdRspInfo.Command.Cmd.Arg = U32BEND2LEND(u32_arg);
	//CmdRspInfo.ClockStop = CLK_KEEP;
    CmdRspInfo.CmdType = CMD_TYPE_ADTC;
    CmdRspInfo.RspType = RSP_TYPE_R1;
	CmdRspInfo.ReadWriteDir = DIR_R;
	CmdRspInfo.DataPath = PATH_DMA;
	CmdRspInfo.BusWidth = BUS_4_BITS;
	CmdRspInfo.MiuAddr = u32_dma_addr;
	CmdRspInfo.pBufferAddr = NULL; // DMA don't need this field
	CmdRspInfo.BlockSize = 512;
	CmdRspInfo.BlockCount = u32BlkNum;

    u32_err = HalSdio_SendCommand(&CmdRspInfo);

	if(u32_err)
	{
		goto ErrorHandle;
	}

	return 0;

ErrorHandle:

	return u32_err;
}

U32 DrvSd_CMD24(U32 u32_BlkAddr, U8 *pu8_DataBuf)
{
	U32 u32_err = 0;
	U32 u32_arg;
	//U8  u8_retry = 0;
	//U32 u32PhyAddr = 0;
    CMD_RSP_INFO CmdRspInfo;
	U32 u32_dma_addr;

	memset(&CmdRspInfo, 0, sizeof(CMD_RSP_INFO));

	u32_arg =  u32_BlkAddr;

	u32_dma_addr = HalSdio_TranslateVirtAddr((U32)pu8_DataBuf, 0x200);

	CmdRspInfo.Command.Cmd.Index = 24;
	CmdRspInfo.Command.Cmd.Arg = U32BEND2LEND(u32_arg);
	//CmdRspInfo.ClockStop = CLK_KEEP;
    CmdRspInfo.CmdType = CMD_TYPE_ADTC;
    CmdRspInfo.RspType = RSP_TYPE_R1;
	CmdRspInfo.ReadWriteDir = DIR_W;
	CmdRspInfo.DataPath = PATH_DMA;
	CmdRspInfo.BusWidth = BUS_4_BITS;
	CmdRspInfo.MiuAddr = u32_dma_addr;
	CmdRspInfo.pBufferAddr = NULL; // DMA don't need this field
	CmdRspInfo.BlockSize = 512;
	CmdRspInfo.BlockCount = 1;

    u32_err = HalSdio_SendCommand(&CmdRspInfo);

	if(u32_err)
	{
		goto ErrorHandle;
	}

	return 0;

ErrorHandle:

	return u32_err;
}

U32 DrvSd_CMD25(U32 u32_BlkAddr, U16 u32BlkNum, U8 *pu8_DataBuf)
{
	U32 u32_err = 0;
	U32 u32_arg;
    CMD_RSP_INFO CmdRspInfo;
	U32 u32_dma_addr;

	memset(&CmdRspInfo, 0, sizeof(CMD_RSP_INFO));

	u32_arg =  u32_BlkAddr;

	u32_dma_addr = HalSdio_TranslateVirtAddr((U32)pu8_DataBuf, 0x200UL*u32BlkNum);

	CmdRspInfo.Command.Cmd.Index = 25;
	CmdRspInfo.Command.Cmd.Arg = U32BEND2LEND(u32_arg);
	//CmdRspInfo.ClockStop = CLK_KEEP;
    CmdRspInfo.CmdType = CMD_TYPE_ADTC;
    CmdRspInfo.RspType = RSP_TYPE_R1;
	CmdRspInfo.ReadWriteDir = DIR_W;
	CmdRspInfo.DataPath = PATH_DMA;
	CmdRspInfo.BusWidth = BUS_4_BITS;
	CmdRspInfo.MiuAddr = u32_dma_addr;
	CmdRspInfo.pBufferAddr = NULL; // DMA don't need this field
	CmdRspInfo.BlockSize = 512;
	CmdRspInfo.BlockCount = u32BlkNum;

    u32_err = HalSdio_SendCommand(&CmdRspInfo);

	if(u32_err)
	{
		goto ErrorHandle;
	}

	return 0;

ErrorHandle:

	return u32_err;
}

#define STUFF_BITS			0x00000000

U32 DrvSd_CMD12(void)
{
	U32 u32_err = 0;
    CMD_RSP_INFO CmdRspInfo;

	memset(&CmdRspInfo, 0, sizeof(CMD_RSP_INFO));

	CmdRspInfo.Command.Cmd.Index = 12;
	CmdRspInfo.Command.Cmd.Arg = STUFF_BITS;
	//CmdRspInfo.ClockStop = CLK_KEEP;
    CmdRspInfo.CmdType = CMD_TYPE_AC;
    CmdRspInfo.RspType = RSP_TYPE_R1b;

    u32_err = HalSdio_SendCommand(&CmdRspInfo);

	if(u32_err)
	{
		goto ErrorHandle;
	}

	return 0;

ErrorHandle:

	return u32_err;
}

U32 DrvSd_CMD13(U16 u16RCA)
{
	U32 u32_err = 0;
    CMD_RSP_INFO CmdRspInfo;

	memset(&CmdRspInfo, 0, sizeof(CMD_RSP_INFO));

	CmdRspInfo.Command.Cmd.Index = 13;
	CmdRspInfo.Command.Cmd.Arg = U32BEND2LEND(((U32)u16RCA)<<16);
	//CmdRspInfo.ClockStop = CLK_KEEP;
    CmdRspInfo.CmdType = CMD_TYPE_AC;
    CmdRspInfo.RspType = RSP_TYPE_R1;

    u32_err = HalSdio_SendCommand(&CmdRspInfo);

	if(u32_err)
	{
		goto ErrorHandle;
	}

	DrvSD_CheckR1(U32BEND2LEND(CmdRspInfo.Response.Rsp.DeviceStatus));

	return 0;

ErrorHandle:

	return u32_err;
}

