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
#include "../../inc/common/eMMC.h"
#if defined(UNIFIED_eMMC_DRIVER) && UNIFIED_eMMC_DRIVER


//========================================================
// HAL pre-processors
//========================================================
#if IF_FCIE_SHARE_PINS
	#define eMMC_PAD_SWITCH(enable)    eMMC_pads_switch(enable);
#else
    // NULL to save CPU a JMP/RET time
	#define eMMC_PAD_SWITCH(enable)
#endif

#if IF_FCIE_SHARE_CLK
	#define eMMC_CLK_SETTING(ClkParam) eMMC_clock_setting(ClkParam);
#else
    // NULL to save CPU a JMP/RET time
	#define eMMC_CLK_SETTING(ClkParam)
#endif

#if IF_FCIE_SHARE_IP
    // re-config FCIE3 for NFIE mode
	#define eMMC_RECONFIG()       eMMC_ReConfig();
#else
    // NULL to save CPU a JMP/RET time
    #define eMMC_RECONFIG()
#endif

//#define eMMC_FCIE_CLK_DIS()    REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN)

#define eMMC_CMD1_RETRY_CNT    0x1000
#define eMMC_CMD3_RETRY_CNT    0x5

//========================================================
// HAL APIs
//========================================================
U32 eMMC_FCIE_WaitEvents(U32 u32_RegAddr, U16 u16_Events, U32 u32_MicroSec)
{
    return eMMC_FCIE_PollingEvents(u32_RegAddr, u16_Events, u32_MicroSec);
}

U32 eMMC_FCIE_PollingEvents(U32 u32_RegAddr, U16 u16_Events, U32 u32_MicroSec)
{
	 U32 u32_i;
	 U16 u16_val;

	for(u32_i=0; u32_i<u32_MicroSec; u32_i++)
	{
		eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
		REG_FCIE_R(u32_RegAddr, u16_val);
		if(u16_Events == (u16_val & u16_Events))
			break;
	}

	if(u32_i == u32_MicroSec)
	{
		return eMMC_ST_ERR_TIMEOUT_WAIT_REG0;
	}

	return eMMC_ST_SUCCESS;
}

void eMMC_FCIE_ClearEvents(void)
{
	U16 u16_reg;

	while(1){
		REG_FCIE_W(FCIE_MIE_EVENT, BIT_ALL_CARD_INT_EVENTS);
		REG_FCIE_R(FCIE_MIE_EVENT, u16_reg);
		if(0==(u16_reg&BIT_ALL_CARD_INT_EVENTS))
			break;
	}

	REG_FCIE_SETBIT(FCIE_SD_STATUS, BIT_SD_FCIE_ERR_FLAGS);
}

//========================================================
// Send CMD HAL APIs
//========================================================
U32 eMMC_Identify(void)
{
	U32 u32_err;
	U16 u16_i;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	// CMD0
	u32_err = eMMC_CMD0(0); // reset to idle state

	if(eMMC_ST_SUCCESS != u32_err)
		return u32_err;

	// CMD1
	for(u16_i=0; u16_i<eMMC_CMD1_RETRY_CNT; u16_i++)
	{
		u32_err = eMMC_CMD1();
		if(eMMC_ST_SUCCESS == u32_err)
			break;
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1ms);

		if(eMMC_ST_ERR_CMD1_DEV_NOT_RDY != u32_err)
            return u32_err;
	}

	// CMD2
    u32_err = eMMC_CMD2();
	if(eMMC_ST_SUCCESS != u32_err)
		return u32_err;

	// CMD3
	u32_err = eMMC_CMD3_CMD7(g_eMMCDrv->u16_RCA, 3);
	if(eMMC_ST_SUCCESS != u32_err)
	{
	    g_eMMCDrv->u16_RCA++;
	    return u32_err;
	}


	return eMMC_ST_SUCCESS;
}

U32 eMMC_SetBusWidth(U8 u8_BusWidth, U8 u8_IfDDR)
{
	U8  u8_value;
	U32 u32_err;

    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;
	switch(u8_BusWidth)
	{
		case 8:
			u8_value=2;
			break;
		default:
			return eMMC_ST_ERR_PARAMETER;
			break;
	}
	if(u8_IfDDR)
	{
		u8_value |= BIT2;
		g_eMMCDrv->u32_Flag |= FCIE_FLAG_DDR_MODE;
	}
	else
		g_eMMCDrv->u32_Flag &= ~FCIE_FLAG_DDR_MODE;

    #if defined(ENABLE_HS400_ENHANCED_STORBE) && ENABLE_HS400_ENHANCED_STORBE
    if(u8_IfDDR == 2 && g_eMMCDrv->u8_ECSD184_Stroe_Support )
    {
        u8_value |= BIT7; // Enhanced Storbe
    }
    #endif

	// -------------------------------
	u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 183, u8_value);
	if(eMMC_ST_SUCCESS != u32_err)
		return u32_err;

	// -------------------------------
	g_eMMCDrv->u16_Reg10_Mode &= ~BIT_SD_DATA_WIDTH_MASK;


	switch(u8_BusWidth)
	{
		case 8:
			g_eMMCDrv->u8_BUS_WIDTH = BIT_SD_DATA_WIDTH_8;
			g_eMMCDrv->u16_Reg10_Mode |= BIT_SD_DATA_WIDTH_8;
			break;
		default:
			return eMMC_ST_ERR_PARAMETER;
			break;
	}

	// -------------------------------


	return u32_err;
}

U32 eMMC_ModifyExtCSD(U8 u8_AccessMode, U8 u8_ByteIdx, U8 u8_Value)
{
	U32 u32_arg, u32_err;

	u32_arg = ((u8_AccessMode&3)<<24) | (u8_ByteIdx<<16) |
		      (u8_Value<<8);

	u32_err = eMMC_CMD6(u32_arg);


    return u32_err;
}



U32 eMMC_CheckR1Error(void)
{
	U32 u32_err = eMMC_ST_SUCCESS;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;
	eMMC_FCIE_GetCIFC(0, 3, (U16*)g_eMMCDrv->au8_Rsp);
	if(g_eMMCDrv->au8_Rsp[1] & (eMMC_ERR_R1_31_24>>24))
	{	
		u32_err = eMMC_ST_ERR_R1_31_24;
	}

	if(g_eMMCDrv->au8_Rsp[2] & (eMMC_ERR_R1_23_16>>16))
	{
		u32_err = eMMC_ST_ERR_R1_23_16;
	}

	if(g_eMMCDrv->au8_Rsp[3] & (eMMC_ERR_R1_15_8>>8))
	{
		u32_err = eMMC_ST_ERR_R1_15_8;	
	}

	if(g_eMMCDrv->au8_Rsp[4] & eMMC_ERR_R1_7_0)
	{
		u32_err = eMMC_ST_ERR_R1_7_0;		
	}

	return u32_err;
}


#ifdef IP_FCIE_VERSION_5

#if defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400

eMMC_ALIGN0 U8 gau8_eMMC_SectorBuf[eMMC_SECTOR_512BYTE] eMMC_ALIGN1 = {0};

U32 eMMC_CMD8_IMI(U8 *pu8_DataBuf)
{
    U32 u32_err, u32_arg;
    U16 u16_mode, u16_ctrl, u16_reg;
    U32 u32_dma_addr;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    // -------------------------------
    REG_FCIE_CLRBIT(reg_ckg_miu_fcie_sel, BIT_CKG_MIU_FCIE_SEL); // select clock source for IMI
    REG_FCIE_SETBIT(FCIE_BOOT, BIT_IMI_SEL);

    // send cmd
    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DAT_EN;
    u32_arg =  0;
    u16_mode = g_eMMCDrv->u16_Reg10_Mode | g_eMMCDrv->u8_BUS_WIDTH;

    eMMC_FCIE_ClearEvents();

    REG_FCIE_W(FCIE_JOB_BL_CNT, 1);
    u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE);

    REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);

    REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, eMMC_SECTOR_512BYTE);
    REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16, 0);

    u32_err = eMMC_FCIE_SendCmd(u16_mode, u16_ctrl, u32_arg, 8, eMMC_R1_BYTE_CNT);
    if (eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    // -------------------------------
    // check FCIE
    u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_DMA_END, TIME_WAIT_1_BLK_END);
    if (eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
    if(u16_reg & BIT_SD_FCIE_ERR_FLAGS)
    {
        u32_err = eMMC_ST_ERR_STATUS;
        goto LABEL_END;
    }

    // -------------------------------
    // check device
    u32_err = eMMC_CheckR1Error();


LABEL_END:

    REG_FCIE_CLRBIT(FCIE_BOOT, BIT_IMI_SEL);
    REG_FCIE_SETBIT(reg_ckg_miu_fcie_sel, BIT_CKG_MIU_FCIE_SEL);

    if(u32_err)
        eMMC_PUTS(gu8_eMMC_str, 8, (U16)u32_err);

    return u32_err;
}

U32 eMMC_ExtCSD_Config(void)
{
    U32 u32_err;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    u32_err = eMMC_CMD8_IMI(gau8_eMMC_SectorBuf);
    if (eMMC_ST_SUCCESS != u32_err)
        return u32_err;
    #if defined(ENABLE_HS400_ENHANCED_STORBE) && ENABLE_HS400_ENHANCED_STORBE
    g_eMMCDrv->u8_ECSD184_Stroe_Support = gau8_eMMC_SectorBuf[184];
    #endif
    g_eMMCDrv->u8_ECSD196_DevType = gau8_eMMC_SectorBuf[196];

    return eMMC_ST_SUCCESS;
}

U32 eMMC_CMD18_IMI(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
    U32 u32_err, u32_arg;
    U16 u16_mode, u16_ctrl, u16_reg;
    U32 u32_dma_addr;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    u32_err = eMMC_CMD23(u16_BlkCnt);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    // -------------------------------
    REG_FCIE_CLRBIT(reg_ckg_miu_fcie_sel, BIT_CKG_MIU_FCIE_SEL); // select clock source for IMI
    REG_FCIE_SETBIT(FCIE_BOOT, BIT_IMI_SEL);

    // send cmd
    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DAT_EN;
    u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv->u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
    u16_mode = g_eMMCDrv->u16_Reg10_Mode | g_eMMCDrv->u8_BUS_WIDTH;

    eMMC_FCIE_ClearEvents();

    REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);
    u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);

    REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);

    REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, (eMMC_SECTOR_512BYTE*u16_BlkCnt) & 0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16,( eMMC_SECTOR_512BYTE*u16_BlkCnt) >> 16);

    u32_err = eMMC_FCIE_SendCmd(u16_mode, u16_ctrl, u32_arg, 18, eMMC_R1_BYTE_CNT);
    if (eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    // -------------------------------
    // check FCIE
    u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>9)));
    if (eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
    if (u16_reg & BIT_SD_FCIE_ERR_FLAGS)
    {
        u32_err = eMMC_ST_ERR_STATUS;
        goto LABEL_END;
    }

    // -------------------------------
    // check device
    u32_err = eMMC_CheckR1Error();


LABEL_END:

    REG_FCIE_CLRBIT(FCIE_BOOT, BIT_IMI_SEL);
    REG_FCIE_SETBIT(reg_ckg_miu_fcie_sel, BIT_CKG_MIU_FCIE_SEL);

    if(u32_err)
        eMMC_PUTS(gu8_eMMC_str, 18, (U16)u32_err);

    return u32_err;

}



#endif



U32 eMMC_FCIE_Reset(void)
{
	REG_FCIE_CLRBIT(FCIE_RST, BIT_FCIE_SOFT_RST_n);
	eMMC_hw_timer_delay(HW_TIMER_DELAY_100us);
	REG_FCIE_SETBIT(FCIE_RST, BIT_FCIE_SOFT_RST_n);

	return eMMC_ST_SUCCESS;
}


U32 eMMC_FCIE_Init(void)
{
	U32 u32_err;
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;
	eMMC_PlatformResetPre();

    REG_FCIE_W(FCIE_SD_CTRL, 0);
    REG_FCIE_W(FCIE_SD_MODE, g_eMMCDrv->u16_Reg10_Mode);
	// ------------------------------------------
	u32_err = eMMC_FCIE_Reset();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		return u32_err;
	}


	REG_FCIE_W(FCIE_MIE_INT_EN, 0);
	REG_FCIE_W(FCIE_MIE_FUNC_CTL, BIT_EMMC_EN);
	REG_FCIE_W(FCIE_BLK_SIZE, 512);

	REG_FCIE_W(FCIE_RSP_SHIFT_CNT, 0);
	REG_FCIE_W(FCIE_RX_SHIFT_CNT, 0);

	REG_FCIE_W(FCIE_WR_SBIT_TIMER, 0);
	REG_FCIE_W(FCIE_RD_SBIT_TIMER, 0);


	eMMC_FCIE_ClearEvents();
    eMMC_PlatformResetPost();

	return eMMC_ST_SUCCESS;	// ok

}

U32 eMMC_FCIE_WaitD0High(U32 u32_Timeout)
{
	U32 u32_cnt;
	U16 u16_read;

	for(u32_cnt=0; u32_cnt < u32_Timeout; u32_cnt++)
	{
		REG_FCIE_R(FCIE_SD_STATUS, u16_read);

		if(!(u16_read&BIT_SD_CARD_BUSY))
			break;
		eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
	}

	if(u32_Timeout == u32_cnt)
	{
		return eMMC_ST_ERR_TIMEOUT_WAITD0HIGH;
	}

	return eMMC_ST_SUCCESS;

}

U32 eMMC_FCIE_SendCmd
(
	U16 u16_Mode, U16 u16_Ctrl, U32 u32_Arg, U8 u8_CmdIdx, U8 u8_RspByteCnt
)
{
	U32 u32_err, u32_Timeout = TIME_WAIT_DAT0_HIGH;
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	REG_FCIE_W(FCIE_CMD_RSP_SIZE, (5<<BIT_CMD_SIZE_SHIFT) | u8_RspByteCnt );

	REG_FCIE_W(FCIE_SD_MODE, u16_Mode);
	// set cmd
	//   CIFC(0) = 39:32 | 47:40
    //   CIFC(1) = 23:16 | 31:24
    //   CIFC(2) = (CIFC(2) & 0xFF00) | 15:8, ignore (CRC7 | end_bit).

    REG_FCIE_W(FCIE_CIFC_ADDR(0),((u32_Arg>>24)<<8) | (0x40|u8_CmdIdx));
	REG_FCIE_W(FCIE_CIFC_ADDR(1), (u32_Arg&0xFF00) | ((u32_Arg>>16)&0xFF));
	REG_FCIE_W(FCIE_CIFC_ADDR(2),  u32_Arg&0xFF);

	if(12 != u8_CmdIdx)
	{
		u32_err = eMMC_FCIE_WaitD0High(u32_Timeout);
		if(eMMC_ST_SUCCESS != u32_err)
			goto LABEL_SEND_CMD_ERROR;
	}

	REG_FCIE_W(FCIE_SD_CTRL, u16_Ctrl);


	REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_JOB_START);


	if(g_eMMCDrv->u32_Flag & FCIE_FLAG_RSP_WAIT_D0H){
		u32_err = eMMC_FCIE_WaitD0High(u32_Timeout);
        if(eMMC_ST_SUCCESS != u32_err)
	        goto LABEL_SEND_CMD_ERROR;
	}

	// wait event
	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_SD_CMD_END, TIME_WAIT_CMDRSP_END);

	LABEL_SEND_CMD_ERROR:
	return u32_err;


}


void eMMC_FCIE_GetCIFC(U16 u16_WordPos, U16 u16_WordCnt, U16 *pu16_Buf)
{
	U16 u16_i;

	for(u16_i=u16_WordPos; u16_i<u16_WordCnt; u16_i++)
		REG_FCIE_R(FCIE_CIFC_ADDR(u16_i), pu16_Buf[u16_i]);

}


U32 eMMC_WaitCIFD_Event(U16 u16_WaitEvent, U32  u32_MicroSec)
{
	volatile U32 u32_Count;
	volatile U16 u16_Reg;

	for (u32_Count=0; u32_Count < u32_MicroSec; u32_Count++)
	{
		REG_FCIE_R(NC_CIFD_EVENT, u16_Reg);
		if ((u16_Reg & u16_WaitEvent) == u16_WaitEvent)
			break;

		eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
	}

	return u32_Count;

}

U32 eMMC_WaitGetCIFD(U8 * pu8_DataBuf, U32 u32_ByteCnt)
{
	U16 u16_i, *pu16_Data = (U16*)pu8_DataBuf;

	if(u32_ByteCnt > FCIE_CIFD_BYTE_CNT)
	{
		return eMMC_ST_ERR_INVALID_PARAM;
	}
	if (eMMC_WaitCIFD_Event(BIT_WBUF_FULL, HW_TIMER_DELAY_500ms)== (HW_TIMER_DELAY_500ms ))
	{
		return eMMC_ST_ERR_TIMEOUT_WAITCIFDEVENT;
	}

	for(u16_i=0; u16_i<(u32_ByteCnt>>1); u16_i++)
		REG_FCIE_R(NC_WBUF_CIFD_ADDR(u16_i), pu16_Data[u16_i]);

	REG_FCIE_W(NC_CIFD_EVENT, BIT_WBUF_FULL); // W1C

	REG_FCIE_W(NC_CIFD_EVENT, BIT_WBUF_EMPTY_TRI);

	return eMMC_ST_SUCCESS;

}


//------------------------------------------------
// reest, etc.
U32 eMMC_CMD0(U32 u32_Arg)
{
	U32 u32_err;
	U16 u16_ctrl;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	u16_ctrl = BIT_SD_CMD_EN;

	//LABEL_SEND_CMD:
	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(g_eMMCDrv->u16_Reg10_Mode, u16_ctrl, u32_Arg, 0, 0);

	if(u32_err)
	    eMMC_PUTS(gu8_eMMC_str, 0, (U16)u32_err);

	return u32_err;
}


// send OCR
U32 eMMC_CMD1(void)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	// (sector mode | byte mode) | (3.0|3.1|3.2|3.3|3.4 V)
	u32_arg = BIT30 | (BIT23|BIT22|BIT21|BIT20|BIT19|BIT18|BIT17|BIT16|BIT15);
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;


	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv->u16_Reg10_Mode, u16_ctrl, u32_arg, 1, eMMC_R3_BYTE_CNT);

    if(eMMC_ST_SUCCESS != u32_err)
    {
        goto LABEL_END;
    }
	// check status
	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	// R3 has no CRC, so does not check BIT_SD_RSP_CRC_ERR
	if(u16_reg & BIT_SD_RSP_TIMEOUT)
	{
	    u32_err = eMMC_ST_ERR_STATUS;
	}
	else
	{	// CMD1 ok, do things here
	    eMMC_FCIE_GetCIFC(0, 3, (U16*)g_eMMCDrv->au8_Rsp);

		if(0 == (g_eMMCDrv->au8_Rsp[1] & 0x80))
			u32_err =  eMMC_ST_ERR_CMD1_DEV_NOT_RDY;
		else
			g_eMMCDrv->u8_IfSectorMode = (g_eMMCDrv->au8_Rsp[1]&BIT6)>>6;
	}

    LABEL_END:
    if(u32_err != eMMC_ST_ERR_CMD1_DEV_NOT_RDY && u32_err !=eMMC_ST_SUCCESS)
        eMMC_PUTS(gu8_eMMC_str, 1, (U16)u32_err);

    return u32_err;
}


// send CID
U32 eMMC_CMD2(void)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	u32_arg = 0;
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_RSPR2_EN;


	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv->u16_Reg10_Mode, u16_ctrl, u32_arg, 2, eMMC_R2_BYTE_CNT);

	if(eMMC_ST_SUCCESS == u32_err)
	{	// check status
		REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
		if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
		{
			u32_err = eMMC_ST_ERR_STATUS;
		}
        else
        {
            eMMC_FCIE_GetCIFC(0, eMMC_R2_BYTE_CNT>>1, (U16*)g_eMMCDrv->au8_CID);

            if( g_eMMCDrv->au8_CID[0] != 0x3F ) // To prevent rep bit shift
            {
                u32_err = eMMC_ST_ERR_CMD2;
            }
        }
	}

    if(u32_err)
        eMMC_PUTS(gu8_eMMC_str, 2, (U16)u32_err);

	return u32_err;
}

// CMD3: assign RCA. CMD7: select device
U32 eMMC_CMD3_CMD7(U16 u16_RCA, U8 u8_CmdIdx)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	if(7 == u8_CmdIdx)
		g_eMMCDrv->u32_Flag |= FCIE_FLAG_RSP_WAIT_D0H;

	u32_arg = u16_RCA<<16;
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv->u16_Reg10_Mode, u16_ctrl, u32_arg, u8_CmdIdx, eMMC_R1_BYTE_CNT);

	if(eMMC_ST_SUCCESS == u32_err)
	{
	    // check status
		REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

		// de-select has no rsp
		if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
		{
			u32_err = eMMC_ST_ERR_STATUS;
		}
		else
		{
			u32_err = eMMC_CheckR1Error();
		}
	}

	g_eMMCDrv->u32_Flag &= ~FCIE_FLAG_RSP_WAIT_D0H;

    if(u32_err)
        eMMC_PUTS(gu8_eMMC_str, u8_CmdIdx, (U16)u32_err);
	
	return u32_err;
}

U32 eMMC_SetBusSpeed(U8 u8_BusSpeed)
{
	U32 u32_err;
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 185, u8_BusSpeed);
	if(eMMC_ST_SUCCESS != u32_err)
		return u32_err;

	g_eMMCDrv->u32_Flag &= ~FCIE_FLAG_SPEED_MASK;
	switch(u8_BusSpeed)
	{
		case eMMC_SPEED_HIGH:
			g_eMMCDrv->u32_Flag |= FCIE_FLAG_SPEED_HIGH;
			break;
		case eMMC_SPEED_HS200:
			g_eMMCDrv->u32_Flag |= FCIE_FLAG_SPEED_HS200;
			break;
		case eMMC_SPEED_HS400:
			g_eMMCDrv->u32_Flag |= FCIE_FLAG_SPEED_HS400;
			break;
		default:
			return eMMC_ST_ERR_PARAMETER;
			break;
	}

	return u32_err;
}

U32 eMMC_CMD6(U32 u32_Arg)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	u32_arg = u32_Arg;
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;
	g_eMMCDrv->u32_Flag |= FCIE_FLAG_RSP_WAIT_D0H;


	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv->u16_Reg10_Mode, u16_ctrl, u32_arg, 6, eMMC_R1b_BYTE_CNT);

	if(eMMC_ST_SUCCESS == u32_err)
	{	// check status
		REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

		if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
		{
			u32_err = eMMC_ST_ERR_STATUS;
		}
		else
		{
			u32_err = eMMC_CheckR1Error();
		}
	}

	g_eMMCDrv->u32_Flag &= ~FCIE_FLAG_RSP_WAIT_D0H;

    if(u32_err)
        eMMC_PUTS(gu8_eMMC_str, 6, (U16)u32_err);

	return u32_err;

}


U32 eMMC_CMD17_CIFD(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U16 u16_i;

    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;


	// -------------------------------
	// send cmd
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DAT_EN;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv->u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
	u16_mode = BIT_SD_DATA_CIFD | g_eMMCDrv->u16_Reg10_Mode | g_eMMCDrv->u8_BUS_WIDTH;
	eMMC_FCIE_ClearEvents();


	REG_FCIE_W(FCIE_JOB_BL_CNT, 1);

	REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, eMMC_SECTOR_512BYTE & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16, eMMC_SECTOR_512BYTE >> 16);

	u32_err = eMMC_FCIE_SendCmd(
		u16_mode, u16_ctrl, u32_arg, 17, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
		goto LABEL_END;
    }

	// -------------------------------
	// check FCIE
	//read for data
	for (u16_i=0; u16_i< (eMMC_SECTOR_512BYTE >> 6); u16_i++)
	{   // read data
		u32_err = eMMC_WaitGetCIFD((U8*)((U32)pu8_DataBuf + (u16_i << 6)), 0x40);
		if(u32_err != eMMC_ST_SUCCESS)
		{
			goto LABEL_END;
		}
	}

	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_DMA_END, TIME_WAIT_1_BLK_END);

	if(eMMC_ST_SUCCESS != u32_err)
		goto LABEL_END;

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(u16_reg & BIT_SD_FCIE_ERR_FLAGS)
	{
		u32_err = eMMC_ST_ERR_STATUS;
		goto LABEL_END;
	}

	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();

	LABEL_END:
	if(u32_err)
		eMMC_PUTS(gu8_eMMC_str, 17, (U16)u32_err);


	return u32_err;
}

U32 eMMC_CMD18_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    u32_err = eMMC_CMD23(u16_BlkCnt);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

	// -------------------------------
	// send cmd
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DAT_EN;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv->u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);

	u16_mode = g_eMMCDrv->u16_Reg10_Mode | g_eMMCDrv->u8_BUS_WIDTH;

	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);
    u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);


	REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);

	REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, (eMMC_SECTOR_512BYTE*u16_BlkCnt) & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16,( eMMC_SECTOR_512BYTE*u16_BlkCnt) >> 16);

    #ifdef CONFIG_MSTAR_MAXIM
    eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
    #endif

	u32_err = eMMC_FCIE_SendCmd(
		u16_mode, u16_ctrl, u32_arg, 18, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
		goto LABEL_END;
    }

	// -------------------------------
	// check FCIE
	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>9)));

	if(eMMC_ST_SUCCESS != u32_err)
		goto LABEL_END;

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
    if(u16_reg & BIT_SD_FCIE_ERR_FLAGS)
    {
		u32_err = eMMC_ST_ERR_STATUS;
		goto LABEL_END;
    }
	
	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();

	LABEL_END:
	if(u32_err)
		eMMC_PUTS(gu8_eMMC_str, 18, (U16)u32_err);

	return u32_err;

}

U32 eMMC_CMD23(U16 u16_BlkCnt)
{
    U32 u32_err, u32_arg;
    U16 u16_ctrl, u16_reg;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    u32_arg = u16_BlkCnt&0xFFFF;// don't set BIT24
    #if eMMC_FEATURE_RELIABLE_WRITE
        u32_arg |= BIT31;
    #endif
    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;


    eMMC_FCIE_ClearEvents();

    #ifdef CONFIG_MSTAR_MAXIM
    eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
    #endif

    u32_err = eMMC_FCIE_SendCmd(
        g_eMMCDrv->u16_Reg10_Mode, u16_ctrl, u32_arg, 23, eMMC_R1_BYTE_CNT);

    if(eMMC_ST_SUCCESS == u32_err)
    {   // check status
        REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

        if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
        {
            u32_err = eMMC_ST_ERR_STATUS;
        }
        else
        {   // CMD13 ok, do things here
            u32_err = eMMC_CheckR1Error();
        }
    }

	if(u32_err)
		eMMC_PUTS(gu8_eMMC_str, 23, (U16)u32_err);
	
    return u32_err;
}


U32 __ATTR_DRAM_CODE__ eMMC_CMD25_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    u32_err = eMMC_CMD23(u16_BlkCnt);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;
	// -------------------------------
	// send cmd
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv->u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
	u16_mode = g_eMMCDrv->u16_Reg10_Mode | g_eMMCDrv->u8_BUS_WIDTH;

	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);
    u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);

	REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);

	REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, (eMMC_SECTOR_512BYTE*u16_BlkCnt) & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16,( eMMC_SECTOR_512BYTE*u16_BlkCnt) >> 16);


	#ifdef CONFIG_MSTAR_MAXIM
    eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
    #endif

	u32_err = eMMC_FCIE_SendCmd(
		u16_mode, u16_ctrl, u32_arg, 25, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
		goto LABEL_END;
    }
    // -------------------------------
    // check device
    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
    if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR) )
    {
        u32_err = eMMC_ST_ERR_STATUS;
        goto LABEL_END;
    }

	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
		goto LABEL_END;

    // -------------------------------
    // send data
    u32_err = eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    REG_FCIE_W(FCIE_SD_CTRL, BIT_SD_DAT_EN|BIT_SD_DAT_DIR_W);
    REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_JOB_START);

	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>9)));
	if(eMMC_ST_SUCCESS != u32_err )
        goto LABEL_END;;


	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(u16_reg & (BIT_SD_W_FAIL|BIT_SD_W_CRC_ERR))
	{
        u32_err = eMMC_ST_ERR_STATUS;
	}


	LABEL_END:
	if(u32_err)
		eMMC_PUTS(gu8_eMMC_str, 25, (U16)u32_err);

	return u32_err;
}


#else

U32 eMMC_FCIE_Reset(void)
{
	// miu request reset - set
	REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_MIU_REQUEST_RST);

	// FCIE reset - set
	REG_FCIE_CLRBIT(FCIE_TEST_MODE, BIT_FCIE_SOFT_RST_n); /* active low */
	eMMC_hw_timer_delay(HW_TIMER_DELAY_100us);


	// FCIE reset - clear
	REG_FCIE_SETBIT(FCIE_TEST_MODE, BIT_FCIE_SOFT_RST_n);
	// FCIE reset - check
	//eMMC_FCIE_CheckResetDone();
	// miu request reset - clear
	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_MIU_REQUEST_RST);


	return eMMC_ST_SUCCESS;
}



U32 eMMC_FCIE_Init(void)
{
	U32 u32_err;
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;
	eMMC_PlatformResetPre();

    REG_FCIE_W(FCIE_SD_CTRL, 0);
    REG_FCIE_W(FCIE_SD_MODE, g_eMMCDrv->u16_Reg10_Mode);
	// ------------------------------------------
	u32_err = eMMC_FCIE_Reset();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		return u32_err;
	}

	REG_FCIE_W(FCIE_MIE_INT_EN, 0);
	REG_FCIE_W(FCIE_MMA_PRI_REG, BIT_MIU_CLK_FREE_RUN);
	REG_FCIE_W(FCIE_PATH_CTRL, BIT_SD_EN);
	// all cmd are 5 bytes (excluding CRC)
	REG_FCIE_W(FCIE_CMD_SIZE, eMMC_CMD_BYTE_CNT);
	// default sector size: 0x200
	REG_FCIE_W(FCIE_SDIO_CTRL, BIT_SDIO_BLK_MODE | eMMC_SECTOR_512BYTE);

	//REG_FCIE_W(FCIE_BOOT_CONFIG, 0); // set by eMMC_pads_switch
	REG_FCIE_W(FCIE_PWR_SAVE_MODE, 0);


	eMMC_FCIE_ClearEvents();
    eMMC_PlatformResetPost();

	return eMMC_ST_SUCCESS;	// ok

}


U32 eMMC_FCIE_WaitD0High(U32 u32_Timeout)
{
	U32 u32_cnt;
	U16 u16_read;

	for(u32_cnt=0; u32_cnt < u32_Timeout; u32_cnt++)
	{
		REG_FCIE_R(FCIE_SD_STATUS, u16_read);

		if((u16_read&BIT_SD_CARD_D0_ST))
			break;
		eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
	}

	if(u32_Timeout == u32_cnt)
	{
		return eMMC_ST_ERR_TIMEOUT_WAITD0HIGH;
	}

	return eMMC_ST_SUCCESS;

}


#define WAIT_FIFOCLK_READY_CNT  0x10000

U32 eMMC_FCIE_FifoClkRdy(void)
{
	U32 u32_i;
	U16 u16_Reg;

	for(u32_i=0; u32_i<TIME_WAIT_FIFOCLK_RDY; u32_i++)
	{
		REG_FCIE_R(FCIE_MMA_PRI_REG, u16_Reg);
		if(u16_Reg & BIT_FIFO_CLKRDY)
			break;
		eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
	}

	if(WAIT_FIFOCLK_READY_CNT == u32_i)
	{
	    // add debug register
		return eMMC_ST_ERR_TIMEOUT_FIFOCLKRDY;
	}


	return eMMC_ST_SUCCESS;
}


U32 eMMC_FCIE_SendCmd
(
	U16 u16_Mode, U16 u16_Ctrl, U32 u32_Arg, U8 u8_CmdIdx, U8 u8_RspByteCnt
)
{
	U32 u32_err, u32_Timeout = TIME_WAIT_DAT0_HIGH;
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	REG_FCIE_W(FCIE_RSP_SIZE, u8_RspByteCnt & BIT_SD_RSP_SIZE_MASK);

	REG_FCIE_W(FCIE_SD_MODE, u16_Mode);
	// set cmd
	//   CIFC(0) = 39:32 | 47:40
    //   CIFC(1) = 23:16 | 31:24
    //   CIFC(2) = (CIFC(2) & 0xFF00) | 15:8, ignore (CRC7 | end_bit).

    REG_FCIE_W(FCIE_CIFC_ADDR(0),((u32_Arg>>24)<<8) | (0x40|u8_CmdIdx));
	REG_FCIE_W(FCIE_CIFC_ADDR(1), (u32_Arg&0xFF00) | ((u32_Arg>>16)&0xFF));
	REG_FCIE_W(FCIE_CIFC_ADDR(2),  u32_Arg&0xFF);

	if(12 != u8_CmdIdx)
	{
		u32_err = eMMC_FCIE_WaitD0High(u32_Timeout);
		if(eMMC_ST_SUCCESS != u32_err)
			goto LABEL_SEND_CMD_ERROR;
	}

	REG_FCIE_W(FCIE_SD_CTRL, u16_Ctrl);


	if(g_eMMCDrv->u32_Flag & FCIE_FLAG_RSP_WAIT_D0H){
		u32_err = eMMC_FCIE_WaitD0High(u32_Timeout);
        if(eMMC_ST_SUCCESS != u32_err)
	        goto LABEL_SEND_CMD_ERROR;
	}

	// wait event
	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_SD_CMD_END, TIME_WAIT_CMDRSP_END);


	LABEL_SEND_CMD_ERROR:
	return u32_err;
}


void eMMC_FCIE_GetCIFC(U16 u16_WordPos, U16 u16_WordCnt, U16 *pu16_Buf)
{
	U16 u16_i;

	for(u16_i=u16_WordPos; u16_i<u16_WordCnt; u16_i++)
		REG_FCIE_R(FCIE_CIFC_ADDR(u16_i), pu16_Buf[u16_i]);
}

void eMMC_FCIE_GetCIFD(U16 u16_WordPos, U16 u16_WordCnt, U16 *pu16_Buf)
{
	U16 u16_i;

	for(u16_i=u16_WordPos; u16_i<u16_WordCnt; u16_i++)
		REG_FCIE_R(FCIE_CIFD_ADDR(u16_i), pu16_Buf[u16_i]);
}



//------------------------------------------------
// reest, etc.
U32 eMMC_CMD0(U32 u32_Arg)
{
	U32 u32_err;
	U16 u16_ctrl;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	u16_ctrl = BIT_SD_CMD_EN;

	//LABEL_SEND_CMD:
	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(g_eMMCDrv->u16_Reg10_Mode, u16_ctrl, u32_Arg, 0, 0);

	return u32_err;

}


// send OCR
U32 eMMC_CMD1(void)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	// (sector mode | byte mode) | (3.0|3.1|3.2|3.3|3.4 V)
	u32_arg = BIT30 | (BIT23|BIT22|BIT21|BIT20|BIT19|BIT18|BIT17|BIT16|BIT15);
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;


	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv->u16_Reg10_Mode, u16_ctrl, u32_arg, 1, eMMC_R3_BYTE_CNT);

    if(eMMC_ST_SUCCESS != u32_err)
		return u32_err;

	// check status
	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	// R3 has no CRC, so does not check BIT_SD_RSP_CRC_ERR
	if(u16_reg & BIT_SD_RSP_TIMEOUT)
	{
	    u32_err = eMMC_ST_ERR_CMD1;
		return u32_err;
	}
	else
	{	// CMD1 ok, do things here
	    eMMC_FCIE_GetCIFC(0, 3, (U16*)g_eMMCDrv->au8_Rsp);

		if(0 == (g_eMMCDrv->au8_Rsp[1] & 0x80))
			u32_err =  eMMC_ST_ERR_CMD1_DEV_NOT_RDY;
		else
			g_eMMCDrv->u8_IfSectorMode = (g_eMMCDrv->au8_Rsp[1]&BIT6)>>6;
	}

	return u32_err;

}


// send CID
U32 eMMC_CMD2(void)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;
	eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	u32_arg = 0;
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_RSPR2_EN;


	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv->u16_Reg10_Mode, u16_ctrl, u32_arg, 2, eMMC_R2_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        return u32_err;
    }
	else
	{	// check status
		REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
		if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
		{
			u32_err = eMMC_ST_ERR_CMD2;
		}
        else
        {
            eMMC_FCIE_GetCIFC(0, eMMC_R2_BYTE_CNT>>1, (U16*)g_eMMCDrv->au8_CID);

            if( g_eMMCDrv->au8_CID[0] != 0x3F ) // To prevent rep bit shift
                return eMMC_ST_ERR_CMD2;
        }
	}

	return u32_err;

}


// CMD3: assign RCA. CMD7: select device
U32 eMMC_CMD3_CMD7(U16 u16_RCA, U8 u8_CmdIdx)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	if(7 == u8_CmdIdx)
		g_eMMCDrv->u32_Flag |= FCIE_FLAG_RSP_WAIT_D0H;

	u32_arg = u16_RCA<<16;
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv->u16_Reg10_Mode, u16_ctrl, u32_arg, u8_CmdIdx, eMMC_R1_BYTE_CNT);

	if(eMMC_ST_SUCCESS == u32_err)
	{
	    // check status
		REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

		// de-select has no rsp
		if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
		{
			u32_err = eMMC_ST_ERR_CMD3_CMD7;
		}
		else
		{
			u32_err = eMMC_CheckR1Error();
		}
	}

	g_eMMCDrv->u32_Flag &= ~FCIE_FLAG_RSP_WAIT_D0H;
	return u32_err;

}

// SWITCH cmd
U32 eMMC_CMD6(U32 u32_Arg)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	u32_arg = u32_Arg;
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;
	g_eMMCDrv->u32_Flag |= FCIE_FLAG_RSP_WAIT_D0H;


	eMMC_FCIE_ClearEvents();
	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv->u16_Reg10_Mode, u16_ctrl, u32_arg, 6, eMMC_R1b_BYTE_CNT);

	if(eMMC_ST_SUCCESS == u32_err)
	{	// check status
		REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

		if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
		{
			u32_err = eMMC_ST_ERR_CMD6;
		}
		else
		{
			u32_err = eMMC_CheckR1Error();
		}
	}

	g_eMMCDrv->u32_Flag &= ~FCIE_FLAG_RSP_WAIT_D0H;
	return u32_err;

}


U32 eMMC_CMD17_CIFD(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;


    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	REG_FCIE_W(FCIE_CIFD_WORD_CNT, 0);


	// -------------------------------
	// send cmd
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DAT_EN;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv->u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
	u16_mode = BIT_SD_DATA_CIFD | g_eMMCDrv->u16_Reg10_Mode | g_eMMCDrv->u8_BUS_WIDTH;
	eMMC_FCIE_ClearEvents();



	u32_err = eMMC_FCIE_SendCmd(
		u16_mode, u16_ctrl, u32_arg, 17, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
		goto LABEL_END;
    }

	// -------------------------------
	// check FCIE
	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_SD_DATA_END, TIME_WAIT_1_BLK_END);

	if(eMMC_ST_SUCCESS != u32_err)
	{
		u32_err = eMMC_ST_ERR_CMD17_CIFD;
		goto LABEL_END;
	}

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(u16_reg & BIT_SD_FCIE_ERR_FLAGS)
	{
		u32_err = eMMC_ST_ERR_CMD17_CIFD;
		goto LABEL_END;
	}

	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		u32_err = eMMC_ST_ERR_CMD17_CIFD;
		goto LABEL_END;
	}

	// -------------------------------
	// CMD17 ok, do things here
	eMMC_FCIE_GetCIFD(0, eMMC_SECTOR_512BYTE>>1, (U16*)pu8_DataBuf);


	LABEL_END:


	return u32_err;
}


U32 eMMC_CMD12(U16 u16_RCA)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl, u16_reg;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    u32_arg = (u16_RCA<<16);
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;
	g_eMMCDrv->u32_Flag |= FCIE_FLAG_RSP_WAIT_D0H;


	eMMC_FCIE_ClearEvents();

	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv->u16_Reg10_Mode, u16_ctrl, u32_arg, 12, eMMC_R1b_BYTE_CNT);

	if(eMMC_ST_SUCCESS == u32_err)
	{	// check status
		REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

		if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
		{
			u32_err = eMMC_ST_ERR_CMD12;
		}
		else
		{
			u32_err = eMMC_CheckR1Error();
		}
	}

	g_eMMCDrv->u32_Flag &= ~FCIE_FLAG_RSP_WAIT_D0H;
	return u32_err;
}

U32 eMMC_CMD12_NoCheck(U16 u16_RCA)
{
	U32 u32_err, u32_arg;
	U16 u16_ctrl;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    u32_arg = (u16_RCA<<16);
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;
	g_eMMCDrv->u32_Flag |= FCIE_FLAG_RSP_WAIT_D0H;


	eMMC_FCIE_ClearEvents();

	u32_err = eMMC_FCIE_SendCmd(
		g_eMMCDrv->u16_Reg10_Mode, u16_ctrl, u32_arg, 12, eMMC_R1b_BYTE_CNT);

	g_eMMCDrv->u32_Flag &= ~FCIE_FLAG_RSP_WAIT_D0H;
	return u32_err;
}


U32 eMMC_CMD18_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

	// -------------------------------
	// send cmd
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DAT_EN;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv->u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);

	u16_mode = g_eMMCDrv->u16_Reg10_Mode | g_eMMCDrv->u8_BUS_WIDTH|BIT_SD_DMA_R_CLK_STOP;

	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);
    u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);


	REG_FCIE_W(FCIE_SDIO_ADDR0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_SDIO_ADDR1, u32_dma_addr >> 16);




	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_DMA_DIR_W);
	u32_err = eMMC_FCIE_FifoClkRdy();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		goto LABEL_END;
	}
	REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);

	u32_err = eMMC_FCIE_SendCmd(
		u16_mode, u16_ctrl, u32_arg, 18, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
		goto LABEL_END;
    }

	// -------------------------------
	// check FCIE

	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT,
	    BIT_MIU_LAST_DONE|BIT_CARD_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>9)));


	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & BIT_SD_FCIE_ERR_FLAGS))
	{
		u32_err = eMMC_ST_ERR_CMD18;
		goto LABEL_END;
	}
	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		u32_err = eMMC_ST_ERR_CMD18;
		goto LABEL_END;
	}

	LABEL_END:
    if(eMMC_ST_SUCCESS == u32_err)
	    u32_err = eMMC_CMD12_NoCheck(g_eMMCDrv->u16_RCA);

	return u32_err;

}

U32 __ATTR_DRAM_CODE__ eMMC_CMD23(U16 u16_BlkCnt)
{
    U32 u32_err, u32_arg;
    U16 u16_ctrl, u16_reg;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    u32_arg = u16_BlkCnt&0xFFFF;// don't set BIT24
    #if eMMC_FEATURE_RELIABLE_WRITE
        u32_arg |= BIT31;
    #endif
    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;


    eMMC_FCIE_ClearEvents();

    u32_err = eMMC_FCIE_SendCmd(
        g_eMMCDrv->u16_Reg10_Mode, u16_ctrl, u32_arg, 23, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        u32_err = eMMC_ST_ERR_CMD23;
        goto LABEL_END;
    }
    else
    {   // check status
        REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

        if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
        {
            u32_err = eMMC_ST_ERR_CMD23;
            goto LABEL_END;
        }
        else
        {   // CMD13 ok, do things here
            u32_err = eMMC_CheckR1Error();
            if(eMMC_ST_SUCCESS != u32_err)
            {
                u32_err = eMMC_ST_ERR_CMD23;
                goto LABEL_END;
            }
        }
    }

    LABEL_END:
    return u32_err;
}


U32 __ATTR_DRAM_CODE__ eMMC_CMD25_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    u32_err = eMMC_CMD23(u16_BlkCnt);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;
	// -------------------------------
	// send cmd
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv->u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
	u16_mode = g_eMMCDrv->u16_Reg10_Mode | g_eMMCDrv->u8_BUS_WIDTH;

	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);
    u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);

	REG_FCIE_W(FCIE_SDIO_ADDR0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_SDIO_ADDR1, u32_dma_addr >> 16);

	REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_DMA_DIR_W);
	u32_err = eMMC_FCIE_FifoClkRdy();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		goto LABEL_END;
	}
	REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);

	u32_err = eMMC_FCIE_SendCmd(
		u16_mode, u16_ctrl, u32_arg, 25, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
		goto LABEL_END;
    }
    // -------------------------------
    // check device
    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
    if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR) )
    {
        u32_err = eMMC_ST_ERR_CMD25;
        goto LABEL_END;
    }

	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		u32_err = eMMC_ST_ERR_CMD25_CHK_R1;
		goto LABEL_END;
	}
    // -------------------------------
    // send data
    u32_err = eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

	REG_FCIE_W(FCIE_SD_CTRL, BIT_SD_DAT_EN|BIT_SD_DAT_DIR_W);

	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_CARD_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>9)));


	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & (BIT_SD_W_FAIL|BIT_SD_W_CRC_ERR)))
	{
		u32_err = eMMC_ST_ERR_CMD25;
	}


	LABEL_END:
	return u32_err;

}




#endif

#endif

