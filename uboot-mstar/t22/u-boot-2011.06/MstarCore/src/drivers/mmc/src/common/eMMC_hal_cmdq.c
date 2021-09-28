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

#include "eMMC.h"

#if defined(ENABLE_CMDQ) && ENABLE_CMDQ

U32 eMMC_Enable_CMDQ(void)
{
    U32 u32_err = 0;

    if( g_eMMCDrv.u8_ECSD308_Cmdq_Support )
    {
        u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 15, BIT0);
        if( u32_err != eMMC_ST_SUCCESS )
        {
            eMMC_debug(eMMC_DEBUG_LEVEL, 1, "Can't switch to CMDQ mode\n");
            return u32_err;
        }
    }

    return u32_err;
}

U32 eMMC_Disable_CMDQ(void)
{
    U32 u32_err = 0;

    if( g_eMMCDrv.u8_ECSD308_Cmdq_Support )
    {
        u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 15, 0);
        if( u32_err != eMMC_ST_SUCCESS )
        {
            eMMC_debug(eMMC_DEBUG_LEVEL, 1, "Can't switch to CMDQ mode\n");
            return u32_err;
        }
    }

    return u32_err;
}

U32 eMMC_CMD13_Check_QSR(U16 u16_RCA)
{
    U32 u32_err = 0;
    U32 u32_arg;
    U16 u16_ctrl;
    U16 u16_reg;

    u32_arg = (u16_RCA<<16) | BIT15;
    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

    eMMC_FCIE_ClearEvents();



    u32_err = eMMC_FCIE_SendCmd(
        g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, 13, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD13 %Xh \n", u32_err);
        eMMC_FCIE_ErrHandler_Stop();
    }
    else
    {   // check status
        REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

        if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
        {
            u32_err = eMMC_ST_ERR_CMD13;
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD13 SD_STS: %04Xh\n", u16_reg);
            eMMC_FCIE_ErrHandler_Stop();
        }
        else
        {
            // Response is QSR status now, no need to Check R1 error
            eMMC_FCIE_GetCMDFIFO(0, 3, (U16 *)g_eMMCDrv.au8_Rsp);
        }
    }

    eMMC_FCIE_CLK_DIS();

    return u32_err;
}


// Legacy way
U32 eMMC_CMD44(U16 u16_BlkCnt, U8 u8_Dir, U8 u8_Taskid)
{
    U32 u32_err = 0;
    U32 u32_arg = 0;
    U16 u16_ctrl;
    U16 u16_reg;

    if( u8_Dir > 1 )
    {
        eMMC_debug(0, 1, "Illegal direction no. %d\n", u8_Dir);
        while(1);
    }

    if( u8_Taskid > 31 )
    {
        eMMC_debug(0, 1, "Illegal task id no. %d\n", u8_Taskid);
        while(1);
    }

    u32_arg = (u8_Dir << 30) | (u8_Taskid << 16) | (u16_BlkCnt);
    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

    eMMC_FCIE_ClearEvents();



    u32_err = eMMC_FCIE_SendCmd(g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, 44, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD44 %Xh \n", u32_err);
		eMMC_FCIE_ErrHandler_Stop();
    }

    // check status
    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
    if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD44 SD_STS: %04Xh\n", u16_reg);
        eMMC_FCIE_ErrHandler_Stop();
    }

    u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
            "eMMC Err: CMD44 check R1 error: %Xh, Arg: %Xh\n", u32_err, u32_arg);
        eMMC_FCIE_ErrHandler_Stop();
    }

    eMMC_FCIE_CLK_DIS();

	return u32_err;
}

U32 eMMC_CMD45(U32 u32_eMMCBlkAddr)
{
    U32 u32_err = 0;
    U32 u32_arg = 0;
    U16 u16_ctrl;
    U16 u16_reg;

    u32_arg = u32_eMMCBlkAddr;
    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

    eMMC_FCIE_ClearEvents();



    u32_err = eMMC_FCIE_SendCmd(g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, 45, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD45 %Xh \n", u32_err);
		eMMC_FCIE_ErrHandler_Stop();
    }

    // check status
    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
    if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD45 SD_STS: %04Xh\n", u16_reg);
        eMMC_FCIE_ErrHandler_Stop();
    }

    u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
            "eMMC Err: CMD45 check R1 error: %Xh, Arg: %Xh\n", u32_err, u32_arg);
        eMMC_FCIE_ErrHandler_Stop();
    }

    eMMC_FCIE_CLK_DIS();

	return u32_err;
}

U32 eMMC_CMD46_MIU(U8 u8_Taskid, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
    U32 u32_err = 0;
    U32 u32_arg = 0;
    U16 u16_mode;
    U16 u16_ctrl;
    U16 u16_reg;
    U32 u32_dma_addr;

    if( u8_Taskid > 31 )
    {
        eMMC_debug(0, 1, "Illegal task id no. %d\n", u8_Taskid);
        while(1);
    }

    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DTRX_EN;
    u32_arg =  (u8_Taskid << 16);
    u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;

    if(g_eMMCDrv.u32_DrvFlag & DRV_FLAG_DDR_MODE)
    {
        eMMC_hw_timer_delay(TIME_WAIT_FCIE_RST_TOGGLE_CNT); // Brian needs 2T
    }

    eMMC_FCIE_ClearEvents();

    REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);

    u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);

    REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);

    REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, (eMMC_SECTOR_512BYTE*u16_BlkCnt) & 0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16,( eMMC_SECTOR_512BYTE*u16_BlkCnt) >> 16);



    u32_err = eMMC_FCIE_SendCmd(u16_mode, u16_ctrl, u32_arg, 46, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD46 %Xh, Taskid %d\n", u32_err, u8_Taskid);
        eMMC_FCIE_ErrHandler_Stop();
        goto LABEL_END;
    }

    u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>9)));

    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
    if(eMMC_ST_SUCCESS != u32_err || (u16_reg & BIT_SD_FCIE_ERR_FLAGS))
    {
        u32_err = eMMC_ST_ERR_CMD18;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,"eMMC Err: CMD46 SD_STS: %04Xh, Err: %Xh\n", u16_reg, u32_err);
        eMMC_FCIE_ErrHandler_Stop();
        goto LABEL_END;
    }

    // -------------------------------
    // check device
    u32_err = eMMC_CheckR1Error();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        u32_err = eMMC_ST_ERR_CMD18;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD46 check R1 error: %Xh, Arg: %Xh\n", u32_err, u32_arg);
        eMMC_FCIE_ErrHandler_Stop();
        goto LABEL_END;
    }

    LABEL_END:

    eMMC_FCIE_CLK_DIS();

    return u32_err;
}

U32 eMMC_CMD47_MIU(U8 u8_Taskid, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
    U32 u32_err = 0;
    U32 u32_arg = 0;
    U16 u16_mode;
    U16 u16_ctrl;
    U16 u16_reg;
    U32 u32_dma_addr;

    if( u8_Taskid > 31 )
    {
        eMMC_debug(0, 1, "Illegal task id no. %d\n", u8_Taskid);
        while(1);
    }

    // -------------------------------
    // send cmd
    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;
    u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;
    u32_arg =  (u8_Taskid << 16);

    eMMC_FCIE_ClearEvents();

    REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);

    u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);

    REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);

    REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, (eMMC_SECTOR_512BYTE*u16_BlkCnt) & 0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16, (eMMC_SECTOR_512BYTE*u16_BlkCnt) >> 16);



    u32_err = eMMC_FCIE_SendCmd(u16_mode, u16_ctrl, u32_arg, 47, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD47 %Xh, Taskid: %d\n", u32_err, u8_Taskid);
        eMMC_FCIE_ErrHandler_Stop();
        goto LABEL_END;
    }

    // -------------------------------
    // check device
    u32_err = eMMC_CheckR1Error();
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD47 check R1 error: %Xh, Arg: %Xh\n", u32_err, u32_arg);
        eMMC_FCIE_ErrHandler_Stop();
        goto LABEL_END;
    }

    // -------------------------------
    // send data
    u32_err = eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        u32_err = eMMC_ST_ERR_CMD25_WAIT_D0H;
        goto LABEL_END;
    }

    REG_FCIE_W(FCIE_SD_CTRL, BIT_SD_DTRX_EN|BIT_SD_DAT_DIR_W);
    REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_JOB_START);

    u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>7)));

    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
    if(eMMC_ST_SUCCESS != u32_err || (u16_reg & (BIT_SD_W_FAIL|BIT_SD_W_CRC_ERR)))
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD47 SD_STS: %04Xh, Err: %Xh, Arg: %Xh\n", u16_reg, u32_err, u32_arg);
        eMMC_FCIE_ErrHandler_Stop();
        goto LABEL_END;
    }

    LABEL_END:

    eMMC_FCIE_CLK_DIS();

    return u32_err;
}


U32 eMMC_CMDQ_Task(struct _LinkDescriptor *pLinkDesc, U32 *pu32BlkAddrArray, U16 u16_count)
{
    U32 u32_err = 0;
    U16 u16_i;
    U16 u16_reg;
    U16 u16_task_desc_0x00;
    U16 u16_task_desc_0x01;
    U16 u16_task_desc_0x02;
    U16 u16_task_desc_0x03;
    U16 u16_task_start_low = 0;
    U16 u16_task_start_high = 0;
    U16 u16_taskid = 0;
    U16 u16_dir = 0;
    U32 u32_dma_addr = 0;
    U32 u32_i;
    U16 au16_tmp[4];

    //eMMC_debug(0, 1, "sizeof(struct _LinkDescriptor) = %d\n", sizeof(struct _LinkDescriptor));

    if( u16_count > 32 )
    {
        eMMC_debug(0, 1, "Illegal task count %d\n", u16_count);
        while(1);
    }

    //REG_FCIE_W(FCIE_MIE_EVENT, REG_FCIE(FCIE_MIE_EVENT));

    // Set reg_job_start_clr_sel = 1
    REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_JOB_START_CLR_SEL);

    REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_SYNC_REG_FOR_CMDQ_EN);   // debug

    // Fill CMD QUEUE Fifo
    for(u16_i=0; u16_i<u16_count; u16_i++)
    {
        u16_taskid = u16_i;

        if( pLinkDesc[u16_taskid].u32_Dir > 1 )
        {
            eMMC_debug(0, 1, "Illegal direction no. %d\n", pLinkDesc[u16_i].u32_Dir);
            while(1);
        }
        while(1)
        {
            u16_dir = (pLinkDesc[u16_taskid].u32_Dir == 1) ? 0 : 1;
            //eMMC_debug(0, 1, "task id: %d, u32_dir: %d, u16_dirL %d\n", u16_taskid, pLinkDesc[u16_taskid].u32_Dir, u16_dir);
    
            u16_task_desc_0x00 = (u16_dir<<14) | ((u16_taskid)<<2);
            REG_FCIE_W(GET_REG_ADDR(FCIE4_BASE, u16_i*4), u16_task_desc_0x00);
            //eMMC_debug(0, 1, "FCIE4 reg %02X: %04X\n", u16_i*4, u16_task_desc_0x00);
    
            u16_task_desc_0x01 = (pLinkDesc[u16_taskid].u32_JobCnt);
            REG_FCIE_W(GET_REG_ADDR(FCIE4_BASE, u16_i*4+1), u16_task_desc_0x01);
            //eMMC_debug(0, 1, "FCIE4 reg %02X: %04X\n", u16_i*4+1, u16_task_desc_0x01);
    
            u16_task_desc_0x02 = (pu32BlkAddrArray[u16_i] & 0xFFFF);
            REG_FCIE_W(GET_REG_ADDR(FCIE4_BASE, u16_i*4+2), u16_task_desc_0x02);
            //eMMC_debug(0, 1, "FCIE4 reg %02X: %04X\n", u16_i*4+2, u16_task_desc_0x02);
    
            u16_task_desc_0x03 = (pu32BlkAddrArray[u16_i] >> 16);
            REG_FCIE_W(GET_REG_ADDR(FCIE4_BASE, u16_i*4+3), u16_task_desc_0x03);
            //eMMC_debug(0, 1, "FCIE4 reg %02X: %04X\n", u16_i*4+3, u16_task_desc_0x03);
			REG_FCIE_R(GET_REG_ADDR(FCIE4_BASE, u16_i*4),au16_tmp[0]);
			REG_FCIE_R(GET_REG_ADDR(FCIE4_BASE, u16_i*4+1),au16_tmp[1]);
			REG_FCIE_R(GET_REG_ADDR(FCIE4_BASE, u16_i*4+2),au16_tmp[2]);
			REG_FCIE_R(GET_REG_ADDR(FCIE4_BASE, u16_i*4+3),au16_tmp[3]);

            if(au16_tmp[0] == u16_task_desc_0x00 &&
                au16_tmp[1] == u16_task_desc_0x01 &&
                au16_tmp[2] == u16_task_desc_0x02 && 
                au16_tmp[3] == u16_task_desc_0x03)
                break;

        }
        if( u16_taskid < 16 )
            u16_task_start_low |= (1<<u16_taskid);
        else
            u16_task_start_high |= (1<<(u16_taskid-16));

    }
    REG_FCIE_SETBIT(GET_REG_ADDR(FCIE4_BASE, (u16_count-1)*4), BIT0);
    #if 0
    eMMC_debug(0, 1, "register %08X = %04X\n",
                      GET_REG_ADDR(FCIE4_BASE, (u16_i-1)*4),
                      REG_FCIE(GET_REG_ADDR(FCIE4_BASE, (u16_i-1)*4)));
    #endif

    REG_FCIE_W(FCIE_JOB_BL_CNT, 1);
    u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pLinkDesc, sizeof(struct _LinkDescriptor)*u16_count);

    REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);

    REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, 0x0010);
    REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16,0x0000);

    // Trigger rfifo full
    REG_FCIE_SETBIT(FCIE_CMDQ_FIFO_CTL, BIT_CMD_FIFO_FULL_TRIG);

    // Set auto check timer
    //REG_FCIE_W(FCIE_CHK_CNT, 0x1000);
    REG_FCIE_W(FCIE_CHK_CNT, 0x8500);

    // Set RCA for CMD13
    REG_FCIE_W(FCIE_RCA, g_eMMCDrv.u16_RCA);

    // Set task request
    REG_FCIE_W(FCIE_TSK_REQ0, u16_task_start_low);
    REG_FCIE_W(FCIE_TSK_REQ1, u16_task_start_high);

    //eMMC_debug(0, 1, "FCIE_TSK_REQ0=%04X\n", REG_FCIE(FCIE_TSK_REQ0));
    //eMMC_debug(0, 1, "FCIE_TSK_REQ1=%04X\n", REG_FCIE(FCIE_TSK_REQ1));

    #if 0 // set debug bus, can be removed
    REG_FCIE_CLRBIT(FCIE_TEST_MODE, BIT_DEBUG_MODE_MASK);
	REG_FCIE_SETBIT(FCIE_TEST_MODE, 7<<BIT_DEBUG_MODE_SHIFT);

	REG_FCIE_CLRBIT(FCIE_EMMC_DEBUG_BUS1, BIT11|BIT10|BIT9|BIT8);
	REG_FCIE_SETBIT(FCIE_EMMC_DEBUG_BUS1, 5<<8); // 5: card
	#endif

    REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_ADMA_EN); // enable ADMA to fetch link table in DRAM

    REG_FCIE_SETBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);  // enable eMMC clock

    #if 0
    eMMC_debug(0, 1, "Registers before CMDQ_EN");
    eMMC_FCIE_DumpRegisters();
    #endif


    REG_FCIE_SETBIT(FCIE_CMDQ_CTL, BIT_CMDQ_EN);

    // Wait CMDQ end interrupt
    for(u32_i=0; u32_i<TIME_WAIT_n_BLK_END; u32_i++)
	{
		eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);

		if(REG_FCIE(FCIE_CMDQ_INT) & BIT_CMDQ_END_INT)
            break;

        if( (REG_FCIE(FCIE_SD_STATUS) & BIT_SD_FCIE_ERR_FLAGS) )
        {
            eMMC_RST_L();
            eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
            eMMC_RST_H();
            eMMC_debug(0, 1, "SD_STS = %04X\n", REG_FCIE(FCIE_SD_STATUS));
            eMMC_FCIE_DumpRegisters();
            eMMC_FCIE_DumpDebugBus();
            eMMC_dump_mem((U8 *)pLinkDesc, sizeof(struct _LinkDescriptor)*(u16_count));
            while(1);
        }
    }

    if( u32_i == TIME_WAIT_n_BLK_END )
    {
        eMMC_RST_L();
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
        eMMC_RST_H();
        eMMC_debug(0, 1, "CMDQ INT timeout\n");
        u32_err = eMMC_ST_ERR_TIMEOUT_WAIT_REG0;
        eMMC_FCIE_DumpRegisters();
        eMMC_FCIE_DumpDebugBus();
        eMMC_dump_mem((U8 *)pLinkDesc, sizeof(struct _LinkDescriptor)*(u16_count));
        while(1);
        goto out_cmdq_task;
    }

    if( ((REG_FCIE(FCIE_TSK_DONE0) & u16_task_start_low) != u16_task_start_low) ||
        ((REG_FCIE(FCIE_TSK_DONE1) & u16_task_start_high) != u16_task_start_high) )
    {
        eMMC_RST_L();
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
        eMMC_RST_H();
        eMMC_debug(0, 1, "CMDQ fake DONE INT!!\n");
        u32_err = eMMC_ST_ERR_TIMEOUT_WAIT_REG0;
        eMMC_FCIE_DumpRegisters();
        eMMC_FCIE_DumpDebugBus();
        eMMC_dump_mem((U8 *)pLinkDesc, sizeof(struct _LinkDescriptor)*(u16_count));
        while(1);
    }

    REG_FCIE_W1C(FCIE_CMDQ_INT, BIT_CMDQ_END_INT);

    u16_reg = REG_FCIE(FCIE_CMDQ_STS);
    if( u16_reg & (BIT_RSP_R1_ERR|BIT_RSP_IDX_ERR) )
    {
        if( u16_reg & BIT_RSP_IDX_ERR )
        {
            eMMC_RST_L();
            eMMC_hw_timer_delay(1);
            eMMC_RST_H();
            eMMC_debug(0, 1, "CMDQ RSP IDX error\n");
            #if 1
            u32_err = eMMC_ST_ERR_CMDQ_RSP_IDX_ERR;
            eMMC_FCIE_DumpRegisters();
            eMMC_FCIE_DumpDebugBus();
            eMMC_dump_mem((U8 *)pLinkDesc, sizeof(struct _LinkDescriptor)*(u16_count));
            //goto out_cmdq_task;
            while(1);
            #endif
        }

        if( u16_reg & BIT_RSP_R1_ERR )
        {
            eMMC_RST_L();
            eMMC_hw_timer_delay(1);
            eMMC_RST_H();
            eMMC_debug(0, 1, "CMDQ RSP R1 error\n");
            #if 1
            u32_err = eMMC_ST_ERR_CMDQ_RSP_R1_ERR;
            eMMC_FCIE_DumpRegisters();
            eMMC_FCIE_DumpDebugBus();
            eMMC_dump_mem((U8 *)pLinkDesc, sizeof(struct _LinkDescriptor)*(u16_count));
            //goto out_cmdq_task;
            while(1);
            #endif
        }

        REG_FCIE_W1C(FCIE_CMDQ_STS, BIT_RSP_R1_ERR|BIT_RSP_IDX_ERR);
    }

    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(u16_reg & BIT_SD_FCIE_ERR_FLAGS)
    {
        eMMC_RST_L();
        eMMC_hw_timer_delay(HW_TIMER_DELAY_1us);
        eMMC_RST_H();
        eMMC_debug(0, 1, "CMDQ SD_STS error = %04X\n", u16_reg);
		u32_err = eMMC_ST_ERR_CMDQ_RSP_R1_ERR;
		eMMC_FCIE_DumpRegisters();
        eMMC_FCIE_DumpDebugBus();
        eMMC_dump_mem((U8 *)pLinkDesc, sizeof(struct _LinkDescriptor)*(u16_count));
        while(1);
	}

    eMMC_debug(0, 1, "CMDQ task ok\n");

    out_cmdq_task:
    eMMC_FCIE_CLK_DIS();

    return u32_err;
}

#endif

