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
//#include <config.h>
//#include <common.h>
//#include <command.h>
//#include <mmc.h>
//#include <part.h>
//#include <malloc.h>
//#include <asm/errno.h>

#include "../../inc/common/eMMC.h"


#if defined(UNIFIED_eMMC_DRIVER) && UNIFIED_eMMC_DRIVER

#define eMMC_NOT_READY_MARK    ~(('e'<<24)|('M'<<16)|('M'<<8)|'C')
static U32 sgu32_IfReadyGuard = eMMC_NOT_READY_MARK;

#if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
eMMC_ALIGN0 U8 g_eMMCDrv_array[512] eMMC_ALIGN1 = {0};
#else
eMMC_ALIGN0 U8 g_eMMCDrv_array[32] eMMC_ALIGN1 = {1,0};
#endif

#if defined(ENABLE_DMA_TO_IMI) && ENABLE_DMA_TO_IMI
char gu8_eMMC_str[]  = {'e','M','M','C',' ','E','r','r',':','\r'};


static void LDR_PUTX(char val)
{
    if (val > 0x9)
        putk_emmc('A'+(val-10));
    else
        putk_emmc('0'+val);
}


static void UART_PUTW(U16 val)
{
    U8 value;

    value = (val>>12)&0xF;
    LDR_PUTX(value);
    value = (val>>8)&0xF;
    LDR_PUTX(value);
    value = (val>>4)&0xF;
    LDR_PUTX(value);
    value = (val)&0xF;
    LDR_PUTX(value);
}

void UART_PUTS(char *str , U8 u8_CmdIdx, U16 u16_err)
{
     //eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

     while(*str != '\r') {
         putk_emmc(*str);
         *(str++);
     }
     putk_emmc('C');
     putk_emmc('M');
     putk_emmc('D');
     UART_PUTW(u8_CmdIdx);
     putk_emmc(' ');
     UART_PUTW(u16_err);
     #if 0
     putk_emmc(' ');
     putk_emmc('S');
     putk_emmc('T');
     putk_emmc(':');
     UART_PUTW(REG_FCIE(FCIE_SD_STATUS));
     if(u8_CmdIdx != 0 && u8_CmdIdx != 1 && u8_CmdIdx != 2)
     {
         putk_emmc(' ');
         putk_emmc('R');
         putk_emmc('1');
         putk_emmc(':');
         UART_PUTW(g_eMMCDrv->au8_Rsp[1]);
         putk_emmc(' ');
         UART_PUTW(g_eMMCDrv->au8_Rsp[2]);
         putk_emmc(' ');
         UART_PUTW(g_eMMCDrv->au8_Rsp[3]);
         putk_emmc(' ');
         UART_PUTW(g_eMMCDrv->au8_Rsp[4]);
     }
     #endif
     putk_emmc('\n');
}

#endif
//========================================================
U32 eMMC_LoadImages(U32 u32_Addr, U32 u32_ByteCnt, U32 u32_BlkAddr)
{
	U32 u32_err;


	//eMMC Identify
	u32_err = eMMC_CheckIfReady();

	if (u32_err != eMMC_ST_SUCCESS)
		goto LABEL_LOAD_IMAGE_END;

	u32_err = eMMC_ReadBootPart((U8*)u32_Addr, u32_ByteCnt, u32_BlkAddr);
	if (u32_err != eMMC_ST_SUCCESS)
		goto LABEL_LOAD_IMAGE_END;

	// --------------------------------
	return eMMC_ST_SUCCESS;

LABEL_LOAD_IMAGE_END:

	while(1);

	return u32_err;

}

//========================================================

U32 eMMC_Init_Device_Ex(void)
{
	U32 u32_err;
	U8  u8_retry=0;

	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	// ---------------------------------
	// init platform & FCIE
	g_eMMCDrv->u16_RCA = 1;
	eMMC_PlatformInit();

LABEL_INIT_START:

	eMMC_RST_L();
	eMMC_hw_timer_delay(HW_TIMER_DELAY_1ms);
	g_eMMCDrv->u8_BUS_WIDTH = BIT_SD_DATA_WIDTH_1;
	g_eMMCDrv->u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;
	eMMC_RST_H();
	eMMC_hw_timer_delay(HW_TIMER_DELAY_1ms);

	u32_err = eMMC_FCIE_Init();
	if (u32_err)
		goto  LABEL_INIT_END;

	// ---------------------------------
	// init eMMC device
	u32_err = eMMC_Identify();
	if (eMMC_ST_SUCCESS != u32_err) {
		if (u8_retry < 10) {
			u8_retry++;
			goto LABEL_INIT_START;
		}

		goto  LABEL_INIT_END;
	}

	//REG_FCIE_CLRBIT(FCIE_BOOT_CONFIG, BIT_NAND_BOOT_MODE_EN);
	eMMC_clock_setting(FCIE_SLOW_CLK);

	// setup eMMC device
	// CMD7
	u32_err = eMMC_CMD3_CMD7(g_eMMCDrv->u16_RCA, 7);
	if (eMMC_ST_SUCCESS != u32_err)
		goto  LABEL_INIT_END;

#if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
	u32_err = eMMC_ExtCSD_Config();
	if(eMMC_ST_SUCCESS != u32_err)
		return u32_err;
#endif

	u32_err = eMMC_SetBusWidth(8, 0);
	if (eMMC_ST_SUCCESS != u32_err)
		goto  LABEL_INIT_END;

    #ifdef IP_FCIE_VERSION_5
	u32_err = eMMC_SetBusSpeed(eMMC_SPEED_HIGH);
	if (eMMC_ST_SUCCESS != u32_err)
		goto  LABEL_INIT_END;
	eMMC_clock_setting(BIT_FCIE_CLK_48M);
    #endif

	sgu32_IfReadyGuard = ~eMMC_NOT_READY_MARK;

LABEL_INIT_END:

	return u32_err;
}

static U32 eMMC_Init_Ex(void)
{
	U32 u32_err = eMMC_ST_SUCCESS;

#ifndef IP_FCIE_VERSION_5
	// boot end
	REG_FCIE_SETBIT(FCIE_BOOT_CONFIG, BIT_BOOT_END_EN);
	while (1) {
		if (REG_FCIE(FCIE_MIE_EVENT) & BIT_CARD_BOOT_DONE) {
			REG_FCIE_CLRBIT(FCIE_BOOT_CONFIG, BIT_EMMC_BOOT_MODE_EN);
			break;
		}

		eMMC_hw_timer_delay(HW_TIMER_DELAY_1ms);
	}

	REG_FCIE_SETBIT(FCIE_MIE_EVENT, BIT_CARD_BOOT_DONE);
#endif

	u32_err = eMMC_Init_Device_Ex();
	if (eMMC_ST_SUCCESS != u32_err)
		goto  LABEL_INIT_END;

#if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
	u32_err = eMMC_FCIE_ChooseSpeedMode();
	if ((eMMC_ST_SUCCESS != u32_err) &&
		(eMMC_ST_ERR_DDRT_CHKSUM != u32_err) &&
		(eMMC_ST_ERR_DDRT_NONA != u32_err))
		goto  LABEL_INIT_END;

	REG_FCIE_W(FCIE_DBG0, 0x3697);
#endif

	return eMMC_ST_SUCCESS;

LABEL_INIT_END:

#if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
	REG_FCIE_W(FCIE_DBG0, 0x2454);

	u32_err = eMMC_Init_Device_Ex();
	if (eMMC_ST_SUCCESS != u32_err)
	{
		REG_FCIE_W(FCIE_DBG0, 0x0014); //eMMC_ST_ERR_NOT_INIT
	}
#endif

	return u32_err;
}

U32 eMMC_CheckIfReady(void)
{
	U32 u32_err = 0;

	if (eMMC_NOT_READY_MARK != sgu32_IfReadyGuard)
		return eMMC_ST_SUCCESS;

	REG_FCIE_CLRBIT(FCIE_BOOT_CONFIG, BIT_NAND_BOOT_MODE_EN);

	u32_err = eMMC_Init_Ex();

	return u32_err;
}

U32 eMMC_ReadBootPart(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr)
{
	U32 u32_err=0;
	U16 u16_SecCnt;

	// set Access Boot Partition 1
	#ifdef IP_FCIE_VERSION_5
    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT0);
	#else
	u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT6|BIT3|BIT0);
	#endif
	if(eMMC_ST_SUCCESS != u32_err)
	{
		return u32_err;
	}

	// read Boot Code
	u16_SecCnt = (u32_DataByteCnt>>eMMC_SECTOR_512BYTE_BITS) + ((u32_DataByteCnt&0x1FF)?1:0);
    #if defined(ENABLE_DMA_TO_IMI) && ENABLE_DMA_TO_IMI
	u32_err = eMMC_ReadData_MIU(pu8_DataBuf, u16_SecCnt<<eMMC_SECTOR_512BYTE_BITS, u32_BlkAddr);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		return u32_err;
	}
	#else
	u32_err = eMMC_ReadData_CIFD(pu8_DataBuf, u16_SecCnt<<eMMC_SECTOR_512BYTE_BITS, u32_BlkAddr);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		return u32_err;
	}
    #endif

	// clear Access Boot Partition
	#ifdef IP_FCIE_VERSION_5
    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);
	#else
	u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT6|BIT3);
	#endif

	return u32_err;
}

#if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400

void eMMC_FCIE_Apply_Reg(void)
{
	int i;
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	for (i = 0; i < g_eMMCDrv->TimingTable_G_t.u8_RegisterCnt; i ++) {
		if (g_eMMCDrv->TimingTable_G_t.RegSet[i].u16_OpCode == REG_OP_W)
			REG_FCIE_W(RIU_BASE + g_eMMCDrv->TimingTable_G_t.RegSet[i].u32_RegAddress, g_eMMCDrv->TimingTable_G_t.RegSet[i].u16_RegValue);
		else if (g_eMMCDrv->TimingTable_G_t.RegSet[i].u16_OpCode == REG_OP_CLRBIT)
			REG_FCIE_CLRBIT(RIU_BASE + g_eMMCDrv->TimingTable_G_t.RegSet[i].u32_RegAddress, g_eMMCDrv->TimingTable_G_t.RegSet[i].u16_RegValue);
		else if (g_eMMCDrv->TimingTable_G_t.RegSet[i].u16_OpCode == REG_OP_SETBIT) {
			REG_FCIE_CLRBIT(RIU_BASE + g_eMMCDrv->TimingTable_G_t.RegSet[i].u32_RegAddress, g_eMMCDrv->TimingTable_G_t.RegSet[i].u16_RegMask);
			REG_FCIE_SETBIT(RIU_BASE + g_eMMCDrv->TimingTable_G_t.RegSet[i].u32_RegAddress, g_eMMCDrv->TimingTable_G_t.RegSet[i].u16_RegValue);
		}
	}
}

void eMMC_FCIE_SetATopTimingReg(void)
{
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	if (g_eMMCDrv->TimingTable_t.Set[0].u8_Reg2Ch)
		REG_FCIE_SETBIT(REG_ANL_SKEW4_INV, BIT_ANL_SKEW4_INV);
	else
		REG_FCIE_CLRBIT(REG_ANL_SKEW4_INV, BIT_ANL_SKEW4_INV);

	REG_FCIE_CLRBIT(reg_emmcpll_0x03, BIT_SKEW4_MASK);
	REG_FCIE_SETBIT(reg_emmcpll_0x03, g_eMMCDrv->TimingTable_t.Set[0].u8_Skew4<<12);
}

#endif

// =======================================================
// u32_DataByteCnt: has to be 512B-boundary !
// =======================================================
U32 eMMC_ReadData_CIFD(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr)
{
	U32 u32_err;
	U16 u16_BlkCnt;

	u16_BlkCnt = 1;
    #ifdef IP_FCIE_VERSION_5
    eMMC_pads_switch_default();
    #endif
 	while(u32_DataByteCnt)
	{
	    u32_err = eMMC_CMD17_CIFD(u32_BlkAddr, pu8_DataBuf);

		if(eMMC_ST_SUCCESS != u32_err)
		{
			break;
		}

		u32_BlkAddr += u16_BlkCnt;
		pu8_DataBuf += u16_BlkCnt << eMMC_SECTOR_512BYTE_BITS;
		u32_DataByteCnt -= u16_BlkCnt << eMMC_SECTOR_512BYTE_BITS;
	}

	#if !(defined(ENABLE_DMA_TO_IMI) && ENABLE_DMA_TO_IMI) && defined(IP_FCIE_VERSION_5)
	eMMC_pads_switch();
    #endif
	return u32_err;
}

// =======================================================
// u32_DataByteCnt: has to be 512B-boundary !
// =======================================================

#if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400

U32 eMMC_LoadTimingTable(U8 u8_PadType)
{
	U32 u32_err = eMMC_ST_SUCCESS;
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;
	U32 u32_ChkSum = 0;
    U16 u16_temp = 0;

	u32_err = eMMC_CMD18_IMI(eMMC_HS400EXTTABLE_BLK_0, gau8_eMMC_SectorBuf, 1);
	if (u32_err)
		goto LABEL_END;

    u16_temp = sizeof(g_eMMCDrv->TimingTable_G_t) - sizeof(g_eMMCDrv->TimingTable_G_t.RegSet) + 5*sizeof(eMMC_FCIE_REG_SET_t);
    memcpy((U8*)&g_eMMCDrv->TimingTable_G_t, gau8_eMMC_SectorBuf, u16_temp);

	if (FCIE_eMMC_HS400 == u8_PadType) {
        u32_ChkSum = eMMC_ChkSum((U8*)(gau8_eMMC_SectorBuf+sizeof(U32)/*u32_VerNo*/),
                                     (sizeof(g_eMMCDrv->TimingTable_G_t) - sizeof(U32)/*checksum*/));

		if (u32_ChkSum != g_eMMCDrv->TimingTable_G_t.u32_ChkSum) {
			u32_err = eMMC_ST_ERR_DDRT_CHKSUM;
			goto LABEL_END;
		}

		if (u32_ChkSum == 0) {
			u32_err = eMMC_ST_ERR_DDRT_CHKSUM;
			goto LABEL_END;
		}


		if (eMMC_memcmp(g_eMMCDrv->TimingTable_G_t.au8_CID, g_eMMCDrv->au8_CID, eMMC_MAX_RSP_BYTE_CNT - 1))
			u32_err = eMMC_ST_ERR_DDRT_NONA;

	} else if (FCIE_eMMC_HS200 == u8_PadType) {
		u32_err = eMMC_CMD18_IMI(eMMC_HS200TABLE_BLK_0, gau8_eMMC_SectorBuf, 1);

		if (u32_err)
			goto LABEL_END;

		eMMC_memcpy((U8*)&g_eMMCDrv->TimingTable_t, gau8_eMMC_SectorBuf,
			sizeof(g_eMMCDrv->TimingTable_t));

		u32_ChkSum = eMMC_ChkSum((U8*)&g_eMMCDrv->TimingTable_t,
			sizeof(g_eMMCDrv->TimingTable_t)-eMMC_TIMING_TABLE_CHKSUM_OFFSET);

		if (u32_ChkSum != g_eMMCDrv->TimingTable_t.u32_ChkSum) {
			u32_err = eMMC_ST_ERR_DDRT_CHKSUM;
			goto LABEL_END;
		}

		if (u32_ChkSum ==0) {
			u32_err = eMMC_ST_ERR_DDRT_CHKSUM;
			goto LABEL_END;
		}

		if (eMMC_memcmp(g_eMMCDrv->TimingTable_G_t.au8_CID, g_eMMCDrv->au8_CID, eMMC_MAX_RSP_BYTE_CNT - 1))
			u32_err = eMMC_ST_ERR_DDRT_NONA;
	} else
		u32_err = eMMC_ST_ERR_DDRT_NONA;

 LABEL_END:

	return u32_err;

}

U32 eMMC_FCIE_EnableFastMode_Ex(U8 u8_PadType)
{
	U32 u32_err = eMMC_ST_SUCCESS;
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	switch (u8_PadType) {

	case FCIE_eMMC_HS200:

		u32_err = eMMC_SetBusWidth(8, 0);
		if (u32_err)
			return u32_err;

		u32_err = eMMC_SetBusSpeed(eMMC_SPEED_HS200);
		if (u32_err)
			return u32_err;

		eMMC_clock_setting(g_eMMCDrv->TimingTable_t.Set[0].u8_Clk);

		break;

	case FCIE_eMMC_HS400:

        #if defined(ENABLE_HS400_ENHANCED_STORBE) && ENABLE_HS400_ENHANCED_STORBE
        if (g_eMMCDrv->u8_ECSD184_Stroe_Support)
        u32_err = eMMC_SetBusWidth(8, 2);
        else
            u32_err = eMMC_SetBusWidth(8, 1);// enable DDR
        #else
		u32_err = eMMC_SetBusWidth(8, 1);// enable DDR
		#endif
		if (u32_err)
			return u32_err;

		u32_err = eMMC_SetBusSpeed(eMMC_SPEED_HS400);
		if (u32_err)
			return u32_err;

		eMMC_clock_setting(g_eMMCDrv->TimingTable_G_t.u32_Clk);

		break;
	}

	eMMC_pads_switch(u8_PadType);

	if (u8_PadType == FCIE_eMMC_HS400)
		eMMC_FCIE_Apply_Reg();

	if(u8_PadType == FCIE_eMMC_HS200)
		eMMC_FCIE_SetATopTimingReg();

	return eMMC_ST_SUCCESS;
}

U32 eMMC_FCIE_EnableFastMode(U8 u8_PadType)
{
	U32 u32_err = eMMC_ST_SUCCESS;

	u32_err = eMMC_LoadTimingTable(u8_PadType);
	if (eMMC_ST_SUCCESS != u32_err)
		return u32_err;

	u32_err = eMMC_FCIE_EnableFastMode_Ex(u8_PadType);

	return u32_err;
}

U32 eMMC_FCIE_ChooseSpeedMode(void)
{
	U32 u32_err = eMMC_ST_SUCCESS;
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	if (g_eMMCDrv->u8_ECSD196_DevType & eMMC_DEVTYPE_HS400_1_8V) {
		u32_err = eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS400);
		if (u32_err)
			goto LABEL_END;
	} else if (g_eMMCDrv->u8_ECSD196_DevType & eMMC_DEVTYPE_HS200_1_8V) {
		u32_err = eMMC_FCIE_EnableFastMode(FCIE_eMMC_HS200);
		if (u32_err)
			goto LABEL_END;
	} else {
		u32_err = eMMC_ST_SUCCESS;
	}

LABEL_END:

	return u32_err;
}

#endif
U32 eMMC_ReadData_MIU(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr)
{
	U32 u32_err;
	U16 u16_BlkCnt;

	// read data
	// first 512 bytes, special handle if not cache line aligned
        u16_BlkCnt = (u32_DataByteCnt>>eMMC_SECTOR_512BYTE_BITS) + ((u32_DataByteCnt&0x1FF)?1:0);
	#if defined(ENABLE_DMA_TO_IMI) && ENABLE_DMA_TO_IMI
	if ((((U32) pu8_DataBuf) >> 16) == ((CONFIG_SRAM_BASE_ADDRESS) >> 16))
		REG_FCIE_SETBIT(FCIE_BOOT, BIT_IMI_SEL);
	#endif
	u32_err = eMMC_CMD18_MIU(u32_BlkAddr, pu8_DataBuf, u16_BlkCnt);
	#if defined(ENABLE_DMA_TO_IMI) && ENABLE_DMA_TO_IMI
	if ((((U32) pu8_DataBuf) >> 16) == ((CONFIG_SRAM_BASE_ADDRESS) >> 16))
		REG_FCIE_CLRBIT(FCIE_BOOT, BIT_IMI_SEL);
	#endif
	if(eMMC_ST_SUCCESS != u32_err)
	{
        while(1);
	}

	return eMMC_ST_SUCCESS;
}

U32 __ATTR_DRAM_CODE__ eMMC_WriteData_MIU(U8* pu8_DataBuf, U32 u32_DataByteCnt, U32 u32_BlkAddr)
{
	U32 u32_err;
	U16 u16_BlkCnt;

	// write data
	// first 512 bytes, special handle if not cache line aligned
    u16_BlkCnt = (u32_DataByteCnt>>eMMC_SECTOR_512BYTE_BITS) + ((u32_DataByteCnt&0x1FF)?1:0);
	u32_err = eMMC_CMD25_MIU(u32_BlkAddr, pu8_DataBuf, u16_BlkCnt);

	if(eMMC_ST_SUCCESS != u32_err)
	{
        while(1);
	}

	return eMMC_ST_SUCCESS;
}


void __ATTR_DRAM_CODE__ eMMC_GetCID(U8 *pu8_CID)
{
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;
    U8 u8_i;

    for(u8_i=0;u8_i<15;u8_i++)
        pu8_CID[u8_i]= g_eMMCDrv->au8_CID[u8_i+1];
}

#if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400
U32 eMMC_ChkSum(U8 *pu8_Data, U32 u32_ByteCnt)
{
    volatile U16 u16_Tmp;
    volatile U32 u32_Sum=0;
    for (u16_Tmp=0; u16_Tmp < u32_ByteCnt; u16_Tmp++)
       u32_Sum += pu8_Data[u16_Tmp];
    return (u32_Sum);
}
#endif

#if defined(CONFIG_DUAL_SYSTEM)
#define BOOT_MODE_TMP_BUF_ADDR  (CONFIG_UBOOT_LOADADDR)

U32 eMMC_CheckBootFlag(void)
{
    U32 u32_err;
    U8 bootflag = 0;

    u32_err = eMMC_CMD18_MIU(0x7880, (U32 *)BOOT_MODE_TMP_BUF_ADDR, 1);  //bootfalg partition addr = 0xF10000
    if(eMMC_ST_SUCCESS != u32_err)
    {
        while(1);
    }

    bootflag = *(U8 *)(BOOT_MODE_TMP_BUF_ADDR);
    if (0 == bootflag)  //bring up MBootBAK
    {
        return 0;
    }
    else  //bring up MBoot
    {
        return 1;
    }
}
#endif

#endif

