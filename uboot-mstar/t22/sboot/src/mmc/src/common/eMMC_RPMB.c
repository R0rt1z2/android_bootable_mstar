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
U8	*gu8_eMMC_SectorBuf = (U8*)(CONFIG_BOOTRAM_LOADADDR+0x20200);
U8	*gu8_eMMC_MAC_CalBuf= (U8*)(CONFIG_BOOTRAM_LOADADDR+0x20600);
U8	*gu8_digest_golden=   (U8*)(CONFIG_BOOTRAM_LOADADDR+0x20A00);
U8	*gu8_digest_from_eMMC=(U8*)(CONFIG_BOOTRAM_LOADADDR+0x20A20);
U8	*gu8_auto_key=(U8*)(CONFIG_BOOTRAM_LOADADDR+0x20A40);
U8	*gu8_input_key=(U8*)(CONFIG_BOOTRAM_LOADADDR+0x20A60);
U8	*gu8_TestBuf=(U8*)(CONFIG_BOOTRAM_LOADADDR+0x20B00);


// enable Reliable Write
U32 __ATTR_DRAM_CODE__ eMMC_CMD23_RPMB(U16 u16_BlkCnt, U8 u8_Is_Reliable)
{
    U32 u32_err, u32_arg;
    U16 u16_ctrl, u16_reg;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    u32_arg = u16_BlkCnt&0xFFFF;// don't set BIT24
    if(u8_Is_Reliable)
        u32_arg |= BIT31;

    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;


    eMMC_FCIE_ClearEvents();

    #ifdef CONFIG_MSTAR_MAXIM
    eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
    #endif

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

U32 __ATTR_DRAM_CODE__ eMMC_CMD18_RPMB_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    u32_err = eMMC_CMD23_RPMB(u16_BlkCnt, 0);
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
	}

	LABEL_END:
	return u32_err;
}

U32 __ATTR_DRAM_CODE__ eMMC_CMD25_RPMB_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt, U8 u8_Is_Reliable)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;
    eMMC_DRIVER *g_eMMCDrv = (eMMC_DRIVER *)g_eMMCDrv_array;

    u32_err = eMMC_CMD23_RPMB(u16_BlkCnt, u8_Is_Reliable);
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
    REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_JOB_START);

	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>9)));


	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & (BIT_SD_W_FAIL|BIT_SD_W_CRC_ERR)))
	{
		u32_err = eMMC_ST_ERR_CMD25;
	}


	LABEL_END:
	return u32_err;
}

void __ATTR_DRAM_CODE__ eMMC_SetError(U16 ErrCode, U16 Pos)
{
    REG_FCIE_W(NC_CIFD_ADDR(0), 0xaaaa);
    REG_FCIE_W(NC_CIFD_ADDR(Pos), ErrCode);
    REG_FCIE_W(NC_CIFD_ADDR(2), 0xcccc);
}

U32 __ATTR_DRAM_CODE__ eMMC_RPMB_CheckResultError(void)
{
	eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

    if(g_eMMCRPMB->u16_result & 0x7F)
    {
        eMMC_SetError(g_eMMCRPMB->u16_result, 1);
        if(g_eMMCRPMB->u16_result != 0x0007)
            return ((U32)g_eMMCRPMB->u16_result);
    }

    return eMMC_ST_SUCCESS;
}

void __ATTR_DRAM_CODE__ eMMC_RPMB_Copy_RESULT(U8 *u8_dest,U8 *u8_src, U32 u32_cnt)
{
	U32 u32_i;
    for(u32_i=0;u32_i< u32_cnt;u32_i++)
    {
        u8_dest[u32_i] = u8_src[u32_cnt-u32_i-1];
    }

}

void __ATTR_DRAM_CODE__ eMMC_RPMB_SWAP_memcpy(U8 *u8_dest,U8 *u8_src, U32 u32_cnt)
{
	U32 u32_i;
    for(u32_i=0;u32_i< u32_cnt;u32_i++)
    {
        u8_dest[u32_cnt-u32_i-1] = u8_src[u32_i];
    }
}

U32 __ATTR_DRAM_CODE__ eMMC_ComapreData(U8 *pu8_Buf0, U8 *pu8_Buf1, U32 u32_ByteCnt)
{
	U32 u32_i;

	for(u32_i=0; u32_i<u32_ByteCnt; u32_i++)
	{
		if(pu8_Buf0[u32_i] != pu8_Buf1[u32_i])
			break;
	}

    if(u32_i != u32_ByteCnt)
        return eMMC_ST_ERR_DATA_MISMATCH;

    return eMMC_ST_SUCCESS;
}

#if 0
static eMMC_ALIGN0 eMMC_TEST_ALIGN_PACK_t g_eMMCTestAlignPack_t eMMC_ALIGN1;

U32 eMMC_CheckAlignPack(U8 u8_AlignByteCnt)
{
    eMMC_SetError(0x0, 3);

	// check alignment
	if((U32)&(g_eMMCTestAlignPack_t.u8_0) & (u8_AlignByteCnt-1))
	{
		eMMC_SetError(0xa0, 3);
		return eMMC_ST_ERR_NOT_ALIGN;
	}
	// check packed - 0
	if((U32)&(g_eMMCTestAlignPack_t.u16_0)-(U32)&(g_eMMCTestAlignPack_t.u8_0) != 1 ||
	   (U32)&(g_eMMCTestAlignPack_t.u32_0)-(U32)&(g_eMMCTestAlignPack_t.u8_0) != 3 ||
	   (U32)&(g_eMMCTestAlignPack_t.u32_1)-(U32)&(g_eMMCTestAlignPack_t.u8_0) != 7)
	{
		eMMC_SetError(0xa1, 3);
		return eMMC_ST_ERR_NOT_PACKED;
	}
	// check packed - 1
	if((U32)&(g_eMMCTestAlignPack_t.u16_0)-(U32)&(g_eMMCTestAlignPack_t.u8_0) != 1 ||
	   (U32)&(g_eMMCTestAlignPack_t.u32_0)-(U32)&(g_eMMCTestAlignPack_t.u16_0)!= 2 ||
	   (U32)&(g_eMMCTestAlignPack_t.u32_1)-(U32)&(g_eMMCTestAlignPack_t.u32_0)!= 4)
	{
		eMMC_SetError(0xa2, 3);
		return eMMC_ST_ERR_NOT_PACKED;
	}

	//eMMC_debug(eMMC_DEBUG_LEVEL_HIGH, 1, "ok\n");
	return eMMC_ST_SUCCESS;
}
#endif
U32 __ATTR_DRAM_CODE__ eMMC_RPMB_Test(U8* key)
{
    //U16 u16_i, u16_j;

    eMMC_SetError(0, 1);
    REG_FCIE_W(FCIE_DBG1, 0xcafe);
    REG_FCIE_W(FCIE_DBG2, 0xfeed);


    #if 0 // check key
    REG_FCIE_W(FCIE_DBG1, 0xabcd);
    REG_FCIE_W(FCIE_DBG2, 0xab00);
	GetHmacKey((U32)gu8_auto_key, 32, key);

    for(u16_i=0; u16_i<16; u16_i++)
    {
		REG_FCIE_W(NC_CIFD_ADDR(u16_i),
		gu8_auto_key[u16_i*2]<<8|gu8_auto_key[u16_i*2+1]);
    }

    REG_FCIE_W(FCIE_DBG2, 0xab22);
    while(1);
    #endif

    #if 0 // check if need to program RPMB key into eMMC
    REG_FCIE_W(FCIE_DBG2, 0xa000|eMMC_RPMB_IfKeyWritten());
    while(1);
    #endif

    #if 0 // program key
    eMMC_RPMB_program_auth_key();
    REG_FCIE_W(FCIE_DBG2, 0x0002);
    while(1);
    #endif

    #if 0 // w/r test

    for(u16_i=0; u16_i<0x20000/0x100; u16_i++)
    {
        REG_FCIE_W(FCIE_DBG1, 0x8000+u16_i);

        for(u16_j=0; u16_j<0x100; u16_j++)
        {
            gu8_TestBuf[u16_j] = 0xaa;
            gu8_TestBuf[u16_j+0x100] = ~gu8_TestBuf[u16_j];
        }

        eMMC_RPMB_Write_data(gu8_TestBuf, 0x100, u16_i);

        eMMC_RPMB_Read_data(&gu8_TestBuf[0x100], 0x100, u16_i);

        if(eMMC_ComapreData(gu8_TestBuf, &gu8_TestBuf[0x100], 0x100))
        {
            eMMC_SetError(0xb001, 3);
            while(1);
        }
    }

    eMMC_SetError(0xcccc, 3);
    eMMC_SetError(0xdddd, 4);
    eMMC_SetError(0xeeee, 5);
    while(1);
    #endif
    return 0;
}

U32 __ATTR_DRAM_CODE__ eMMC_RPMB_program_auth_key(U8* key)
{
    U32 u32_err=0;
    eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

#if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
    GetHmacKey((U32)gu8_auto_key,32,key);
#endif

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    //==============write auth key========================
    memset((void*)g_eMMCRPMB,0,sizeof(eMMC_RPMB_DATA));
    memcpy(g_eMMCRPMB->u8_auth_key,gu8_auto_key,32);

    g_eMMCRPMB->u16_req_rsp = eMMC_RPMB_REQ_AUTH_KEY;
    eMMC_RPMB_SWAP_memcpy(gu8_eMMC_SectorBuf,(U8*)g_eMMCRPMB,sizeof(eMMC_RPMB_DATA));

    u32_err = eMMC_CMD25_RPMB_MIU(0,gu8_eMMC_SectorBuf, 1, 1);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    //=============read result==========================
    memset((void*)g_eMMCRPMB,0,sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB->u16_req_rsp = eMMC_RPMB_REQ_RESULT_R;

	u32_err = eMMC_RPMB_NoDataReq();

    LABEL_END:
    eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);

    return u32_err;
}


U32 __ATTR_DRAM_CODE__ eMMC_RPMB_Verify_Hmac(U8* pu8_DataBuf, U16 u16_BlkCnt, U8* key)
{
	U32 u32_err=0;
	U16 u16_i;

#if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
	GetHmacKey((U32)gu8_auto_key,32,key);
#endif

	eMMC_RPMB_SWAP_memcpy(gu8_input_key,gu8_auto_key,32);

	for(u16_i=0;u16_i<u16_BlkCnt;u16_i++)
	{
		memcpy(gu8_eMMC_MAC_CalBuf+(u16_i*284),pu8_DataBuf+(u16_i<<9)+228,284);
	}

	memcpy(gu8_digest_from_eMMC, pu8_DataBuf+((u16_BlkCnt-1)<<9)+196 ,32);

#if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
	MDrv_AESDMA_Hmac_sha256(gu8_eMMC_MAC_CalBuf,u16_BlkCnt*284,gu8_input_key,32,gu8_digest_golden);
#endif

	if(eMMC_ComapreData(gu8_digest_golden, gu8_digest_from_eMMC,32))
		u32_err = eMMC_ST_ERR_RPMB_HMAC_ERR;

	return u32_err;
}

U32 __ATTR_DRAM_CODE__ eMMC_RPMB_Read_data(U8 *pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_BlkAddr, U8* key)
{
    U16 u16_i,u16_BlkCnt;
	U32 u32_err;
    eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

	u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
	if(u32_err)
        goto LABEL_END;

    while(u32_DataByteCnt)
    {
        memset((void*)g_eMMCRPMB, 0 ,512);
        //blk addr
        g_eMMCRPMB->u16_addr 	= u16_BlkAddr;
        g_eMMCRPMB->u16_req_rsp	= eMMC_RPMB_REQ_AUTH_DATA_R;

        eMMC_RPMB_SWAP_memcpy(gu8_eMMC_SectorBuf,(U8*)g_eMMCRPMB ,sizeof(eMMC_RPMB_DATA));
        u32_err = eMMC_CMD25_RPMB_MIU(0,gu8_eMMC_SectorBuf, 1, 0);
        if(u32_err)
            break;

        if(u16_BlkAddr & 1)//block address is odd
            u16_BlkCnt = 1;
        else//block address is even
        {
            if(u32_DataByteCnt >= 512)
                u16_BlkCnt = 2;
            else
                u16_BlkCnt = 1;
        }

        u32_err = eMMC_CMD18_RPMB_MIU(0,gu8_eMMC_SectorBuf, u16_BlkCnt);
        if(u32_err)
            break;

        u32_err = eMMC_RPMB_Verify_Hmac(gu8_eMMC_SectorBuf, u16_BlkCnt, key);
        if(u32_err)
            break;

        for(u16_i=0;u16_i<u16_BlkCnt;u16_i++)
        {
            eMMC_RPMB_SWAP_memcpy(pu8_DataBuf+(u16_i<<8),gu8_eMMC_SectorBuf+(u16_i<<9)+228,256);
            eMMC_RPMB_Copy_RESULT((U8*)g_eMMCRPMB,gu8_eMMC_SectorBuf+(u16_i<<9),sizeof(eMMC_RPMB_DATA));
            u32_err = eMMC_RPMB_CheckResultError();
            if(u32_err)
                break;
        }

        if(u16_i != u16_BlkCnt)
            break;

        u16_BlkAddr += u16_BlkCnt;
        pu8_DataBuf  += u16_BlkCnt << 8;
        u32_DataByteCnt -= u16_BlkCnt << 8;
    }

	LABEL_END:
    eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);

    return u32_err;
}

U32 __ATTR_DRAM_CODE__ eMMC_RPMB_NoDataReq(void)
{
    U32 u32_err;
    eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

    eMMC_RPMB_SWAP_memcpy(gu8_eMMC_SectorBuf,(U8*)g_eMMCRPMB,sizeof(eMMC_RPMB_DATA));

    u32_err = eMMC_CMD25_RPMB_MIU(0,gu8_eMMC_SectorBuf, 1, 0);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    u32_err = eMMC_CMD18_RPMB_MIU(0,gu8_eMMC_SectorBuf, 1);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    eMMC_RPMB_Copy_RESULT((U8*)g_eMMCRPMB,gu8_eMMC_SectorBuf,sizeof(eMMC_RPMB_DATA));

    u32_err = eMMC_RPMB_CheckResultError();

    return u32_err;
}

//0:Authentication key not yet programmed, 1:Authentication key is written, 2:real failed
U32 __ATTR_DRAM_CODE__ eMMC_RPMB_IfKeyWritten(void)
{
	U32 u32_err;
	eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        goto LABEL_END;
    }
    // read data to check key is written or not
    memset((void*)g_eMMCRPMB,0,sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB->u16_addr     = 0;
    g_eMMCRPMB->u16_req_rsp  = eMMC_RPMB_REQ_AUTH_DATA_R;

    eMMC_RPMB_SWAP_memcpy(gu8_eMMC_SectorBuf,(void*)g_eMMCRPMB,sizeof(eMMC_RPMB_DATA));
    u32_err = eMMC_CMD25_RPMB_MIU(0,gu8_eMMC_SectorBuf, 1, 0);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;


    u32_err = eMMC_CMD18_RPMB_MIU(0,gu8_eMMC_SectorBuf, 1);

    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

	eMMC_RPMB_Copy_RESULT((void*)g_eMMCRPMB,gu8_eMMC_SectorBuf,sizeof(eMMC_RPMB_DATA));
	u32_err = eMMC_RPMB_CheckResultError();

    LABEL_END:
    eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);


    if(eMMC_ST_SUCCESS != u32_err)
        return 2;

    if(g_eMMCRPMB->u16_result == 0x0007)
        return 0;
    else
        return 1;
}

U32 __ATTR_DRAM_CODE__ eMMC_RPMB_Read_Result(U16 *pu16_Result)
{
	U32 u32_err;
	eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    memset((void*)g_eMMCRPMB,0,sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB->u16_req_rsp = eMMC_RPMB_REQ_RESULT_R;

    u32_err = eMMC_RPMB_NoDataReq();
    if(eMMC_ST_SUCCESS == u32_err)
        *pu16_Result = g_eMMCRPMB->u16_result;

    return eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);
}


U32 __ATTR_DRAM_CODE__ eMMC_RPMB_Get_Counter(volatile U32 *pu32_WCnt)
{
	U32 u32_err;
	eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

    memset((void*)g_eMMCRPMB,0,sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB->u16_req_rsp = eMMC_RPMB_REQ_WRITE_CNT_VAL;

    u32_err = eMMC_RPMB_NoDataReq();
    if(eMMC_ST_SUCCESS == u32_err)
        *pu32_WCnt = g_eMMCRPMB->u32_writecnt;

    return u32_err;
}

U32 __ATTR_DRAM_CODE__ eMMC_RPMB_Write_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_blk_addr, U8* key)
{
    U32 u32_err=0;
    volatile U32 u32_wcnt;
    U16 u16_i,u16_BlkCnt;
    eMMC_RPMB_DATA *g_eMMCRPMB = (eMMC_RPMB_DATA*)eMMC_get_RPMBContext_address();

#if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
    GetHmacKey((U32)gu8_auto_key, 32, key);
#endif

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    while(u32_DataByteCnt)
    {
        u32_wcnt = 0;
        eMMC_RPMB_Get_Counter(&u32_wcnt);
        if(g_eMMCRPMB->u16_result & 0x80)//rpmb write limit times
        {
            u32_err = eMMC_ST_ERR_RPMB_WCNT_MAX;
            break;
        }
        if(u16_blk_addr & 1)//block address is odd
            u16_BlkCnt = 1;
        else//block address is even
        {
            if(u32_DataByteCnt >= 512)
                u16_BlkCnt = 2;
            else
                u16_BlkCnt = 1;
        }

        memset(gu8_eMMC_MAC_CalBuf,0,2*284);
        memset(gu8_eMMC_SectorBuf,0,2<<9);

        for(u16_i=0;u16_i< u16_BlkCnt;u16_i++)
        {
            memset((void*)g_eMMCRPMB,0,sizeof(eMMC_RPMB_DATA));
            g_eMMCRPMB->u32_writecnt = u32_wcnt;
            g_eMMCRPMB->u16_addr     = u16_blk_addr;
            g_eMMCRPMB->u16_blk_cnt  = u16_BlkCnt;
            g_eMMCRPMB->u16_req_rsp  = eMMC_RPMB_REQ_AUTH_DATA_W;
            memcpy(g_eMMCRPMB->u8_data, pu8_DataBuf+(u16_i<<8), 256);
            eMMC_RPMB_SWAP_memcpy(gu8_eMMC_MAC_CalBuf+u16_i*284, (U8*)g_eMMCRPMB, 284);
            eMMC_RPMB_SWAP_memcpy(gu8_eMMC_SectorBuf+(u16_i<<9), (U8*)g_eMMCRPMB, sizeof(eMMC_RPMB_DATA));
        }

        eMMC_RPMB_SWAP_memcpy(gu8_input_key, gu8_auto_key, 32);
#if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
        MDrv_AESDMA_Hmac_sha256(gu8_eMMC_MAC_CalBuf, u16_BlkCnt*284, gu8_input_key, 32, gu8_digest_golden);
#endif

        memcpy(gu8_eMMC_SectorBuf+(((u16_BlkCnt-1)<<9)+196),gu8_digest_golden,32);

        u32_err = eMMC_CMD25_RPMB_MIU(0,gu8_eMMC_SectorBuf, u16_BlkCnt, 1);
        if(eMMC_ST_SUCCESS != u32_err)
            break;

        //=============read result==========================
        memset((void*)g_eMMCRPMB,0,sizeof(eMMC_RPMB_DATA));
        g_eMMCRPMB->u16_req_rsp = eMMC_RPMB_REQ_RESULT_R;

        u32_err = eMMC_RPMB_NoDataReq();

        if(eMMC_ST_SUCCESS != u32_err)
            break;

        u16_blk_addr += u16_BlkCnt;
        pu8_DataBuf  += u16_BlkCnt<<8;
        u32_DataByteCnt -= u16_BlkCnt<<8;
    }



    LABEL_END:
    eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);

    return u32_err;
}

#if defined(CONFIG_PROGRAM_EMMC_RPMB_KEY)
#if defined(CONFIG_TEE_LOADER) && !defined(CONFIG_MSTAR_BUILD_TEE_LOADER_DAT)
#define putk_rpmb putk_emmc
#warning"emmc driver print"
#else
#define putk_rpmb uart_putc
#warning"uart print"
#endif
U32 eMMC_RPMB_Check_Program_Key(U8* key)
{
    U32 u32_err=0;

    if (1 == eMMC_RPMB_IfKeyWritten())
    {
        putk_rpmb('*');
    }
    else
    {
        putk_rpmb('#');
        u32_err = eMMC_RPMB_program_auth_key(key);
        putk_rpmb('&');
    }
    return u32_err;
}
#endif
#endif
