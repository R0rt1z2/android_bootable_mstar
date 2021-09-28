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
#if defined(CONFIG_SECURITY_STORE_IN_EMMC_FLASH) && CONFIG_SECURITY_STORE_IN_EMMC_FLASH

#include <secure/crypto_hmac.h>
#include <secure/crypto_sha.h>


eMMC_RPMB_DATA g_eMMCRPMB;

void eMMC_RPMB_SWAP_memcpy(U8 *u8_dest,U8 *u8_src, U32 u32_cnt)
{
	U32 u32_i;
    for(u32_i=0;u32_i< u32_cnt;u32_i++)
    {
        u8_dest[u32_cnt-u32_i-1] = u8_src[u32_i];
    }
}

void eMMC_RPMB_Copy_RESULT(U8 *u8_dest,U8 *u8_src, U32 u32_cnt)
{
	U32 u32_i;
    for(u32_i=0;u32_i< u32_cnt;u32_i++)
    {
        u8_dest[u32_i] = u8_src[u32_cnt-u32_i-1];
    }

}
#ifdef IP_FCIE_VERSION_5

U32 eMMC_CMD23_RPMB(U16 u16_BlkCnt, U8 u8_Is_Reliable)
{
    U32 u32_err, u32_arg;
    U16 u16_ctrl, u16_reg;
    U8  u8_retry_r1=0, u8_retry_fcie=0, u8_retry_cmd=0;

    u32_arg = u16_BlkCnt&0xFFFF; // don't set BIT24
    if(u8_Is_Reliable)
        u32_arg |= BIT31; // don't set BIT24
    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

    LABEL_SEND_CMD:
    eMMC_FCIE_ClearEvents();
    u32_err = eMMC_FCIE_SendCmd(
        g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, 23, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        if(u8_retry_cmd < eMMC_FCIE_CMD_RSP_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
        {
            u8_retry_cmd++;
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC WARN: CMD23 retry:%u, %Xh \n", u8_retry_cmd, u32_err);
            eMMC_FCIE_ErrHandler_Retry();
            goto LABEL_SEND_CMD;
        }
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD23 retry:%u, %Xh \n", u8_retry_cmd, u32_err);
        eMMC_FCIE_ErrHandler_Stop();
    }
    else
    {   // check status
        REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

        if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
        {
            if(u8_retry_fcie < eMMC_CMD_API_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
            {
                u8_retry_fcie++;
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                    "eMMC WARN: CMD23 SD_STS: %04Xh, Retry: %u\n", u16_reg, u8_retry_fcie);
                eMMC_FCIE_ErrHandler_Retry();
                goto LABEL_SEND_CMD;
            }

            u32_err = eMMC_ST_ERR_CMD23;
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                    "eMMC Err: CMD23 SD_STS: %04Xh, Retry: %u\n", u16_reg, u8_retry_fcie);
            eMMC_FCIE_ErrHandler_Stop();
        }
        else
        {   // CMD13 ok, do things here
            u32_err = eMMC_CheckR1Error();
            if(eMMC_ST_SUCCESS != u32_err)
            {
                if(u8_retry_r1 < eMMC_CMD_API_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY()){
                    u8_retry_r1++;
                    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                        "eMMC WARN: CMD23 check R1 error: %Xh, retry: %u\n", u32_err, u8_retry_r1);
                    eMMC_FCIE_ErrHandler_Retry();
                    goto LABEL_SEND_CMD;
                }
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                    "eMMC Err: CMD23 check R1 error: %Xh, retry: %u\n", u32_err, u8_retry_r1);
                eMMC_FCIE_ErrHandler_Stop();
            }
        }
    }


    return u32_err;
}

U32 eMMC_CMD18_RPMB_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
    U32 u32_err, u32_arg;
    U16 u16_mode, u16_ctrl, u16_reg;
    U8  u8_retry_fcie=0, u8_retry_r1=0, u8_retry_cmd=0;
    U32 u32_dma_addr;

    // -------------------------------
    if(0 == eMMC_IF_DISABLE_RETRY())
        eMMC_FCIE_ErrHandler_RestoreClk();

    LABEL_SEND_CMD:
    u32_err = eMMC_CMD23_RPMB(u16_BlkCnt, 0);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    // -------------------------------
    // send cmd
    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DTRX_EN;


    u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
    u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;

    eMMC_FCIE_ClearEvents();
    REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);
    REG_FCIE_W(FCIE_BLK_SIZE, 0x200); // 64 bytes tuning pattern

    u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);

    REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);

    REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, (eMMC_SECTOR_512BYTE*u16_BlkCnt) & 0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16,(eMMC_SECTOR_512BYTE*u16_BlkCnt) >> 16);

    u32_err = eMMC_FCIE_SendCmd(u16_mode, u16_ctrl, u32_arg, 18, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        if(u8_retry_cmd < eMMC_FCIE_CMD_RSP_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
        {
            u8_retry_cmd++;
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC WARN: CMD18 retry:%u, %Xh \n", u8_retry_cmd, u32_err);
            eMMC_FCIE_ErrHandler_ReInit();
            eMMC_FCIE_ErrHandler_Retry();
            goto LABEL_SEND_CMD;
        }
        u32_err = eMMC_ST_ERR_CMD18_RSP;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD18 retry:%u, %Xh \n", u8_retry_cmd, u32_err);
        eMMC_FCIE_ErrHandler_Stop();
        goto LABEL_END;
    }

    // -------------------------------
    // check FCIE
    u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT,BIT_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>11)));

    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
    if(eMMC_ST_SUCCESS != u32_err || (u16_reg & BIT_SD_FCIE_ERR_FLAGS))
    {
        if(u8_retry_fcie < eMMC_CMD_API_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
        {
            u8_retry_fcie++;
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                "eMMC WARN: CMD18 Reg.12: %04Xh, Err: %Xh, Retry: %u\n", u16_reg, u32_err, u8_retry_fcie);

            eMMC_FCIE_ErrHandler_ReInit();
            eMMC_FCIE_ErrHandler_Retry();

            goto LABEL_SEND_CMD;
        }
        u32_err = eMMC_ST_ERR_CMD18;
        if(u16_reg & BIT_SD_RSP_CRC_ERR)
            u32_err = eMMC_ST_ERR_CMD18_RSP;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
            "eMMC Err: CMD18 Reg.12: %04Xh, Err: %Xh, Retry: %u\n", u16_reg, u32_err, u8_retry_fcie);
        eMMC_FCIE_ErrHandler_Stop();
        goto LABEL_END;
    }

    // -------------------------------
    // check device
    u32_err = eMMC_CheckR1Error();
    if( eMMC_ST_SUCCESS != u32_err)
    {
        if(u8_retry_r1 < eMMC_CMD_API_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
        {
            u8_retry_r1++;
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                "eMMC WARN: CMD18 check R1 error: %Xh, Retry: %u, Arg: %Xh\n", u32_err, u8_retry_r1, u32_arg);

            eMMC_CMD12_NoCheck(g_eMMCDrv.u16_RCA);
            eMMC_FCIE_ErrHandler_ReInit();
            eMMC_FCIE_ErrHandler_Retry();
            goto LABEL_SEND_CMD;
        }
        u32_err = eMMC_ST_ERR_CMD18;
        eMMC_debug(1, 1, "eMMC Err: CMD18 check R1 error: %Xh, Retry: %u, Arg: %Xh\n", u32_err, u8_retry_r1, u32_arg);
        eMMC_FCIE_ErrHandler_Stop();
        goto LABEL_END;
    }

    LABEL_END:

    if(eMMC_ST_SUCCESS == u32_err)
        u32_err = eMMC_CMD13(g_eMMCDrv.u16_RCA);



    return u32_err;

}

U32 eMMC_CMD25_RPMB_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt, U8 u8_Is_Reliable)
{
    U32 u32_err, u32_arg;
    U16 u16_mode, u16_ctrl, u16_reg;
    U8  u8_retry_fcie=0, u8_retry_r1=0, u8_retry_cmd=0;
    U32 u32_dma_addr;


    // -------------------------------
    // restore clock to max
    if(0 == eMMC_IF_DISABLE_RETRY())
        eMMC_FCIE_ErrHandler_RestoreClk();

    LABEL_SEND_CMD:
    u32_err = eMMC_CMD23_RPMB(u16_BlkCnt, u8_Is_Reliable);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    // -------------------------------
    // send cmd
    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;


    u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;
    u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);

    eMMC_FCIE_ClearEvents();
    REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);
    u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);
    REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);

    REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, (eMMC_SECTOR_512BYTE*u16_BlkCnt) & 0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16,(eMMC_SECTOR_512BYTE*u16_BlkCnt) >> 16);

    u32_err = eMMC_FCIE_SendCmd(
        u16_mode, u16_ctrl, u32_arg, 25, eMMC_R1_BYTE_CNT);
    if( eMMC_ST_SUCCESS != u32_err)
    {
        if(u8_retry_cmd < eMMC_FCIE_CMD_RSP_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
        {
            u8_retry_cmd++;
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC WARN: CMD25 retry:%u, %Xh, Arg: %Xh \n", u8_retry_cmd, u32_err, u32_arg);
            eMMC_CMD12_NoCheck(g_eMMCDrv.u16_RCA);
            eMMC_FCIE_ErrHandler_ReInit();
            eMMC_FCIE_ErrHandler_Retry();
            goto LABEL_SEND_CMD;
        }
        u32_err = eMMC_ST_ERR_CMD25_RSP;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD25 retry:%u, %Xh, Arg: %Xh \n", u8_retry_cmd, u32_err, u32_arg);
        eMMC_FCIE_ErrHandler_Stop();
        goto LABEL_END;
    }

    // -------------------------------
    // check device
    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
    if( u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR) )
    {
        if(u8_retry_fcie < eMMC_CMD_API_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
        {
            u8_retry_fcie++;
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                "eMMC WARN: CMD25 Reg.12: %04Xh, Err: %Xh, Retry: %u, Arg: %Xh\n", u16_reg, u32_err, u8_retry_fcie, u32_arg);
            eMMC_CMD12_NoCheck(g_eMMCDrv.u16_RCA);
            eMMC_FCIE_ErrHandler_ReInit();
            eMMC_FCIE_ErrHandler_Retry();
            goto LABEL_SEND_CMD;
        }
        u32_err = eMMC_ST_ERR_CMD25;
        if(u16_reg & BIT_SD_RSP_CRC_ERR)
            u32_err = eMMC_ST_ERR_CMD25_RSP;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
            "eMMC Err: CMD25 Reg.12: %04Xh, Err: %Xh, Retry: %u, Arg: %Xh\n", u16_reg, u32_err, u8_retry_fcie, u32_arg);
        eMMC_FCIE_ErrHandler_Stop();
        goto LABEL_END;
    }

    u32_err = eMMC_CheckR1Error();
    if( eMMC_ST_SUCCESS != u32_err)
    {
        if(u8_retry_r1 < eMMC_CMD_API_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
        {
            u8_retry_r1++;
            eMMC_debug(1, 1, "eMMC WARN: CMD25 check R1 error: %Xh, Retry: %u, Arg: %Xh\n", u32_err, u8_retry_r1, u32_arg);
            eMMC_CMD12_NoCheck(g_eMMCDrv.u16_RCA);
            eMMC_FCIE_ErrHandler_ReInit();
            eMMC_FCIE_ErrHandler_Retry();
            goto LABEL_SEND_CMD;
        }
        u32_err = eMMC_ST_ERR_CMD25_CHK_R1;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
            "eMMC Err: CMD25 check R1 error: %Xh, Retry: %u, Arg: %Xh\n", u32_err, u8_retry_r1, u32_arg);
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

    eMMC_CheckPowerCut();
    REG_FCIE_W(FCIE_SD_CTRL, BIT_SD_DTRX_EN|BIT_SD_DAT_DIR_W);
    REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_JOB_START);


    #if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
    REG_FCIE_W(FCIE_MIE_INT_EN, BIT_DMA_END);
    #endif

    u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT,
        BIT_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>9)));

    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
    if( eMMC_ST_SUCCESS != u32_err || (u16_reg & (BIT_SD_W_FAIL|BIT_SD_W_CRC_ERR)))
    {
        if(u8_retry_fcie < eMMC_CMD_API_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
        {
            u8_retry_fcie++;
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                "eMMC WARN: CMD25 Reg.12: %04Xh, Err: %Xh, Retry: %u, Arg: %Xh\n", u16_reg, u32_err, u8_retry_fcie, u32_arg);
            eMMC_CMD12_NoCheck(g_eMMCDrv.u16_RCA);
            eMMC_FCIE_ErrHandler_ReInit();
            eMMC_FCIE_ErrHandler_Retry();
            goto LABEL_SEND_CMD;
        }
        u32_err = eMMC_ST_ERR_CMD25;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
            "eMMC Err: CMD25 Reg.12: %04Xh, Err: %Xh, Retry: %u, Arg: %Xh\n", u16_reg, u32_err, u8_retry_fcie, u32_arg);
        eMMC_FCIE_ErrHandler_Stop();
        goto LABEL_END;
    }


    LABEL_END:
    if(eMMC_ST_SUCCESS == u32_err)
         u32_err = eMMC_CMD13(g_eMMCDrv.u16_RCA);



    return u32_err;

}
#else
U32 eMMC_CMD23_RPMB(U16 u16_BlkCnt, U8 u8_Is_Reliable)
{
    U32 u32_err, u32_arg;
    U16 u16_ctrl, u16_reg;
    U8  u8_retry_r1=0, u8_retry_fcie=0, u8_retry_cmd=0;

    u32_arg = u16_BlkCnt&0xFFFF; // don't set BIT24
    if(u8_Is_Reliable)
        u32_arg |= BIT31; // don't set BIT24
    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

    LABEL_SEND_CMD:
    eMMC_FCIE_ClearEvents();
    u32_err = eMMC_FCIE_SendCmd(
        g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, 23, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        if(u8_retry_cmd < eMMC_FCIE_CMD_RSP_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
        {
            u8_retry_cmd++;
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC WARN: CMD23 retry:%u, %Xh \n", u8_retry_cmd, u32_err);
            eMMC_FCIE_ErrHandler_ReInit();
            eMMC_FCIE_ErrHandler_Retry();
            goto LABEL_SEND_CMD;
        }
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD23 retry:%u, %Xh \n", u8_retry_cmd, u32_err);
        eMMC_FCIE_ErrHandler_Stop();
    }
    else
    {   // check status
        REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

        if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR))
        {
            if(u8_retry_fcie < eMMC_CMD_API_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
            {
                u8_retry_fcie++;
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                    "eMMC WARN: CMD23 Reg.12: %04Xh, Retry: %u\n", u16_reg, u8_retry_fcie);
                eMMC_FCIE_ErrHandler_ReInit();
                eMMC_FCIE_ErrHandler_Retry();
                goto LABEL_SEND_CMD;
            }

            u32_err = eMMC_ST_ERR_CMD23;
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                    "eMMC Err: CMD23 Reg.12: %04Xh, Retry: %u\n", u16_reg, u8_retry_fcie);
            eMMC_FCIE_ErrHandler_Stop();
        }
        else
        {   // CMD13 ok, do things here
            u32_err = eMMC_CheckR1Error();
            if(eMMC_ST_SUCCESS != u32_err)
            {
                if(u8_retry_r1 < eMMC_CMD_API_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY()){
                    u8_retry_r1++;
                    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                        "eMMC WARN: CMD23 check R1 error: %Xh, retry: %u\n", u32_err, u8_retry_r1);
                    eMMC_FCIE_ErrHandler_ReInit();
                    eMMC_FCIE_ErrHandler_Retry();
                    goto LABEL_SEND_CMD;
                }
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                    "eMMC Err: CMD23 check R1 error: %Xh, retry: %u\n", u32_err, u8_retry_r1);
                eMMC_FCIE_ErrHandler_Stop();
            }
        }
    }



	return u32_err;
}

U32 eMMC_CMD18_RPMB_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
    U8  u8_retry_fifoclk=0, u8_retry_fcie=0, u8_retry_r1=0, u8_retry_cmd=0;
	U32 u32_dma_addr;

	// -------------------------------
	if(0 == eMMC_IF_DISABLE_RETRY())
		eMMC_FCIE_ErrHandler_RestoreClk();

    LABEL_SEND_CMD:
    u32_err = eMMC_CMD23_RPMB(u16_BlkCnt, 0);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;
	// -------------------------------
	// send cmd
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DAT_EN;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
	u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;
	if(g_eMMCDrv.u32_DrvFlag & (DRV_FLAG_DDR_MODE|DRV_FLAG_SPEED_HS200) )
	{
		REG_FCIE_W(FCIE_TOGGLE_CNT, (g_eMMCDrv.u32_DrvFlag&DRV_FLAG_SPEED_HS200) ? TOGGLE_CNT_512_CLK_R : TOGGLE_CNT_256_CLK_R);
		REG_FCIE_SETBIT(FCIE_MACRO_REDNT, BIT_TOGGLE_CNT_RST);
		REG_FCIE_CLRBIT(FCIE_MACRO_REDNT, BIT_MACRO_DIR);
		eMMC_hw_timer_delay(TIME_WAIT_FCIE_RST_TOGGLE_CNT); // Brian needs 2T
		REG_FCIE_CLRBIT(FCIE_MACRO_REDNT, BIT_TOGGLE_CNT_RST);
	}
	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);
	u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);
    #if FICE_BYTE_MODE_ENABLE
	REG_FCIE_W(FCIE_SDIO_ADDR0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_SDIO_ADDR1, u32_dma_addr >> 16);
    #else
	REG_FCIE_W(FCIE_MIU_DMA_15_0, (u32_dma_addr >> MIU_BUS_WIDTH_BITS)& 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_26_16,(u32_dma_addr >> MIU_BUS_WIDTH_BITS)>>16);
    #endif
	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_DMA_DIR_W);
	u32_err = eMMC_FCIE_FifoClkRdy(0);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		if(u8_retry_fifoclk < eMMC_CMD_API_WAIT_FIFOCLK_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
		{
			u8_retry_fifoclk++;
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC WARN: CMD18 wait FIFOClk retry: %u \n", u8_retry_fifoclk);
			eMMC_FCIE_Init();
			goto LABEL_SEND_CMD;
		}
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: CMD18 wait FIFOClk retry: %u \n", u8_retry_fifoclk);
		eMMC_FCIE_ErrHandler_Stop();
		goto LABEL_END;
	}
	REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);

	u32_err = eMMC_FCIE_SendCmd(u16_mode, u16_ctrl, u32_arg, 18, eMMC_R1_BYTE_CNT);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		if(u8_retry_cmd < eMMC_FCIE_CMD_RSP_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
		{
			u8_retry_cmd++;
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC WARN: CMD18 retry:%u, %Xh \n", u8_retry_cmd, u32_err);
			eMMC_FCIE_ErrHandler_ReInit();
			eMMC_FCIE_ErrHandler_Retry();
			goto LABEL_SEND_CMD;
		}
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD18 retry:%u, %Xh \n", u8_retry_cmd, u32_err);
		eMMC_FCIE_ErrHandler_Stop();
		goto LABEL_END;
	}

	// -------------------------------
	// check FCIE
    #if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
	REG_FCIE_W(FCIE_MIE_INT_EN, BIT_MIU_LAST_DONE);
    #endif
	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT,
	    BIT_MIU_LAST_DONE|BIT_CARD_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>11)));

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & BIT_SD_FCIE_ERR_FLAGS))
	{
		if(u8_retry_fcie < eMMC_CMD_API_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
		{
			u8_retry_fcie++;
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
				"eMMC WARN: CMD18 Reg.12: %04Xh, Err: %Xh, Retry: %u\n", u16_reg, u32_err, u8_retry_fcie);

			eMMC_FCIE_ErrHandler_ReInit();
			eMMC_FCIE_ErrHandler_Retry();

			goto LABEL_SEND_CMD;
		}
		u32_err = eMMC_ST_ERR_CMD18;
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
			"eMMC Err: CMD18 Reg.12: %04Xh, Err: %Xh, Retry: %u\n", u16_reg, u32_err, u8_retry_fcie);
		eMMC_FCIE_ErrHandler_Stop();
		goto LABEL_END;
	}

	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		if(u8_retry_r1 < eMMC_CMD_API_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
		{
			u8_retry_r1++;
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
		        "eMMC WARN: CMD18 check R1 error: %Xh, Retry: %u, Arg: %Xh\n", u32_err, u8_retry_r1, u32_arg);

			eMMC_CMD12_NoCheck(g_eMMCDrv.u16_RCA);
			eMMC_FCIE_ErrHandler_ReInit();
			eMMC_FCIE_ErrHandler_Retry();
			goto LABEL_SEND_CMD;
		}
		u32_err = eMMC_ST_ERR_CMD18;
		eMMC_debug(1, 1, "eMMC Err: CMD18 check R1 error: %Xh, Retry: %u, Arg: %Xh\n", u32_err, u8_retry_r1, u32_arg);
		eMMC_FCIE_ErrHandler_Stop();
		goto LABEL_END;
	}


	LABEL_END:
	if(g_eMMCDrv.u32_DrvFlag & DRV_FLAG_DDR_MODE)
		REG_FCIE_SETBIT(FCIE_MACRO_REDNT, BIT_MACRO_DIR);

    if(eMMC_ST_SUCCESS == u32_err)
        u32_err = eMMC_CMD13(g_eMMCDrv.u16_RCA);

	return u32_err;
}

U32 eMMC_CMD25_RPMB_MIU(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt, U8 u8_Is_Reliable)
{
    U32 u32_err, u32_arg;
    U16 u16_mode, u16_ctrl, u16_reg;
    U8  u8_retry_fifoclk=0, u8_retry_fcie=0, u8_retry_r1=0, u8_retry_cmd=0;
    U32 u32_dma_addr;

	// -------------------------------
	// restore clock to max
	if(0 == eMMC_IF_DISABLE_RETRY())
		eMMC_FCIE_ErrHandler_RestoreClk();

    LABEL_SEND_CMD:
    u32_err = eMMC_CMD23_RPMB(u16_BlkCnt, u8_Is_Reliable);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;
    // -------------------------------
    // send cmd
    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;
	u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;
	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
	if(g_eMMCDrv.u32_DrvFlag & (DRV_FLAG_DDR_MODE|DRV_FLAG_SPEED_HS200) )
	{
		REG_FCIE_W(FCIE_TOGGLE_CNT, (g_eMMCDrv.u32_DrvFlag&DRV_FLAG_SPEED_HS200) ? TOGGLE_CNT_512_CLK_W : TOGGLE_CNT_256_CLK_W);
		REG_FCIE_SETBIT(FCIE_MACRO_REDNT, BIT_MACRO_DIR);
	}
	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, u16_BlkCnt);
	u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE*u16_BlkCnt);
    #if FICE_BYTE_MODE_ENABLE
	REG_FCIE_W(FCIE_SDIO_ADDR0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_SDIO_ADDR1, u32_dma_addr >> 16);
    #else
	REG_FCIE_W(FCIE_MIU_DMA_15_0, (u32_dma_addr >> MIU_BUS_WIDTH_BITS)& 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_26_16,(u32_dma_addr >> MIU_BUS_WIDTH_BITS)>>16);
    #endif
	REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_DMA_DIR_W);
	u32_err = eMMC_FCIE_FifoClkRdy(BIT_DMA_DIR_W);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		if(u8_retry_fifoclk < eMMC_CMD_API_WAIT_FIFOCLK_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
		{
			u8_retry_fifoclk++;
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC WARN: CMD25 wait FIFOClk retry: %u \n", u8_retry_fifoclk);
			eMMC_FCIE_Init();
			goto LABEL_SEND_CMD;
		}
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: CMD25 wait FIFOClk retry: %u \n", u8_retry_fifoclk);
		eMMC_FCIE_ErrHandler_Stop();
		goto LABEL_END;
	}
	REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);

	u32_err = eMMC_FCIE_SendCmd(
		u16_mode, u16_ctrl, u32_arg, 25, eMMC_R1_BYTE_CNT);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		if(u8_retry_cmd < eMMC_FCIE_CMD_RSP_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
		{
			u8_retry_cmd++;
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC WARN: CMD25 retry:%u, %Xh, Arg: %Xh \n", u8_retry_cmd, u32_err, u32_arg);
			eMMC_CMD12_NoCheck(g_eMMCDrv.u16_RCA);
			eMMC_FCIE_ErrHandler_ReInit();
			eMMC_FCIE_ErrHandler_Retry();
			goto LABEL_SEND_CMD;
		}
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD25 retry:%u, %Xh, Arg: %Xh \n", u8_retry_cmd, u32_err, u32_arg);
		eMMC_FCIE_ErrHandler_Stop();
		goto LABEL_END;
	}

	// -------------------------------
	// check device
	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(u16_reg & (BIT_SD_RSP_TIMEOUT|BIT_SD_RSP_CRC_ERR) )
	{
		if(u8_retry_fcie < eMMC_CMD_API_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
		{
			u8_retry_fcie++;
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
				"eMMC WARN: CMD25 Reg.12: %04Xh, Err: %Xh, Retry: %u, Arg: %Xh\n", u16_reg, u32_err, u8_retry_fcie, u32_arg);
			eMMC_CMD12_NoCheck(g_eMMCDrv.u16_RCA);
			eMMC_FCIE_ErrHandler_ReInit();
			eMMC_FCIE_ErrHandler_Retry();
			goto LABEL_SEND_CMD;
		}
		u32_err = eMMC_ST_ERR_CMD25;
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
		    "eMMC Err: CMD25 Reg.12: %04Xh, Err: %Xh, Retry: %u, Arg: %Xh\n", u16_reg, u32_err, u8_retry_fcie, u32_arg);
		eMMC_FCIE_ErrHandler_Stop();
		goto LABEL_END;
	}

	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		if(u8_retry_r1 < eMMC_CMD_API_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
		{
			u8_retry_r1++;
			eMMC_debug(1, 1, "eMMC WARN: CMD25 check R1 error: %Xh, Retry: %u, Arg: %Xh\n", u32_err, u8_retry_r1, u32_arg);
			eMMC_CMD12_NoCheck(g_eMMCDrv.u16_RCA);
			eMMC_FCIE_ErrHandler_ReInit();
			eMMC_FCIE_ErrHandler_Retry();
			goto LABEL_SEND_CMD;
		}
		u32_err = eMMC_ST_ERR_CMD25_CHK_R1;
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
		    "eMMC Err: CMD25 check R1 error: %Xh, Retry: %u, Arg: %Xh\n", u32_err, u8_retry_r1, u32_arg);
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

    eMMC_CheckPowerCut();
	REG_FCIE_W(FCIE_SD_CTRL, BIT_SD_DAT_EN|BIT_SD_DAT_DIR_W);

    #if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
    REG_FCIE_W(FCIE_MIE_INT_EN, BIT_CARD_DMA_END);
    #endif

	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT,
	    BIT_CARD_DMA_END, TIME_WAIT_n_BLK_END*(1+(u16_BlkCnt>>9)));

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & (BIT_SD_W_FAIL|BIT_SD_W_CRC_ERR)))
	{
		if(u8_retry_fcie < eMMC_CMD_API_ERR_RETRY_CNT && 0==eMMC_IF_DISABLE_RETRY())
		{
			u8_retry_fcie++;
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
				"eMMC WARN: CMD25 Reg.12: %04Xh, Err: %Xh, Retry: %u, Arg: %Xh\n", u16_reg, u32_err, u8_retry_fcie, u32_arg);
			eMMC_CMD12_NoCheck(g_eMMCDrv.u16_RCA);
			eMMC_FCIE_ErrHandler_ReInit();
			eMMC_FCIE_ErrHandler_Retry();
			goto LABEL_SEND_CMD;
		}
		u32_err = eMMC_ST_ERR_CMD25;
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
		    "eMMC Err: CMD25 Reg.12: %04Xh, Err: %Xh, Retry: %u, Arg: %Xh\n", u16_reg, u32_err, u8_retry_fcie, u32_arg);
		eMMC_FCIE_ErrHandler_Stop();
		goto LABEL_END;
	}


    LABEL_END:
    if(eMMC_ST_SUCCESS == u32_err)
        u32_err = eMMC_CMD13(g_eMMCDrv.u16_RCA);


    return u32_err;
}
#endif

U32  eMMC_RPMB_CheckResultError(void)
{
    if(g_eMMCRPMB.u16_result & 0x7F)
    {
        if(g_eMMCRPMB.u16_result != 0x0007)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Result Error: %Xh\n", g_eMMCRPMB.u16_result);
            return ((U32)g_eMMCRPMB.u16_result);
        }
    }

    return eMMC_ST_SUCCESS;
}

#if 0
U32 eMMC_RPMB_Read_Result(U16 *pu16_Result)
{
	U32 u32_err;

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    memset((void*)(&g_eMMCRPMB),0,sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB.u16_req_rsp = eMMC_RPMB_REQ_RESULT_R;

    u32_err = eMMC_RPMB_NoDataReq();
    if(eMMC_ST_SUCCESS == u32_err)
        *pu16_Result = g_eMMCRPMB.u16_result;

    return eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3);

}
#endif
U32  eMMC_RPMB_NoDataReq(void)
{
    U32 u32_err;

    eMMC_RPMB_SWAP_memcpy(gau8_eMMC_SectorBuf,(U8*)(&g_eMMCRPMB),sizeof(eMMC_RPMB_DATA));

    u32_err = eMMC_CMD25_RPMB_MIU(0,gau8_eMMC_SectorBuf, 1, 0);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    u32_err = eMMC_CMD18_RPMB_MIU(0,gau8_eMMC_SectorBuf, 1);
    if(eMMC_ST_SUCCESS != u32_err)
        return u32_err;

    eMMC_RPMB_Copy_RESULT((U8*)(&g_eMMCRPMB),gau8_eMMC_SectorBuf,sizeof(eMMC_RPMB_DATA));

    u32_err = eMMC_RPMB_CheckResultError();

    return u32_err;
}


U32 eMMC_RPMB_IfKeyWritten(void)
{
	U32 u32_err;

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh, eMMC, set Ext_CSD[179]: %Xh fail\n",
            u32_err, BIT3|BIT1|BIT0);
        goto LABEL_END;
    }
    // read data to check key is written or not
    memset(&g_eMMCRPMB,'\0',sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB.u16_addr     = 0;
    g_eMMCRPMB.u16_req_rsp  = eMMC_RPMB_REQ_AUTH_DATA_R;

    eMMC_RPMB_SWAP_memcpy(gau8_eMMC_SectorBuf,(U8*)(&g_eMMCRPMB),sizeof(eMMC_RPMB_DATA));
    u32_err = eMMC_CMD25_RPMB_MIU(0,gau8_eMMC_SectorBuf, 1, 0);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;


    u32_err = eMMC_CMD18_RPMB_MIU(0,gau8_eMMC_SectorBuf, 1);

    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: R fail: %Xh\n", u32_err);
        goto LABEL_END;
    }

	eMMC_RPMB_Copy_RESULT((U8*)(&g_eMMCRPMB),gau8_eMMC_SectorBuf,sizeof(eMMC_RPMB_DATA));
	u32_err = eMMC_RPMB_CheckResultError();

    LABEL_END:
    if(eMMC_ST_SUCCESS != eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3))
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC, set Ext_CSD[179]: %Xh fail\n",BIT3);
        eMMC_FCIE_ErrHandler_Stop();
	}

    if(eMMC_ST_SUCCESS != u32_err)
        eMMC_FCIE_ErrHandler_Stop();

    if(g_eMMCRPMB.u16_result == 0x0007)
        return 0;
    else
        return 1;
}


U32 eMMC_RPMB_program_auth_key(void)
{
	U32 u32_err=0;

    U8  u8_auto_key[32];

    GetHmacKey(u8_auto_key);

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh, eMMC, set Ext_CSD[179]: %Xh fail\n",
            u32_err, BIT3|BIT1|BIT0);
        goto LABEL_END;
    }

    //==============write auth key========================
    memset((void*)(&g_eMMCRPMB),'\0',sizeof(eMMC_RPMB_DATA));
    memcpy(g_eMMCRPMB.u8_auth_key,u8_auto_key,32);

    g_eMMCRPMB.u16_req_rsp = eMMC_RPMB_REQ_AUTH_KEY;
    eMMC_RPMB_SWAP_memcpy(gau8_eMMC_SectorBuf,(U8*)(&g_eMMCRPMB),sizeof(eMMC_RPMB_DATA));

    u32_err = eMMC_CMD25_RPMB_MIU(0,gau8_eMMC_SectorBuf, 1, 1);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    //=============read result==========================
    memset((void*)(&g_eMMCRPMB),'\0',sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB.u16_req_rsp = eMMC_RPMB_REQ_RESULT_R;

	u32_err = eMMC_RPMB_NoDataReq();

    LABEL_END:
    if(eMMC_ST_SUCCESS != eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3))
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: set Ext_CSD[179]: %Xh fail\n", BIT3);
        eMMC_FCIE_ErrHandler_Stop();
    }
    return u32_err;
}



U32 eMMC_RPMB_Get_Counter_for_cmd(U32 *pu32_WCnt)
{
	U32 u32_err;
    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh, eMMC, set Ext_CSD[179]: %Xh fail\n",
            u32_err, BIT3|BIT1|BIT0);
        goto LABEL_END;
    }

    memset((void*)(&g_eMMCRPMB),'\0',sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB.u16_req_rsp = eMMC_RPMB_REQ_WRITE_CNT_VAL;

    u32_err = eMMC_RPMB_NoDataReq();
    if(eMMC_ST_SUCCESS == u32_err)
        *pu32_WCnt = g_eMMCRPMB.u32_writecnt;

    LABEL_END:
    if(eMMC_ST_SUCCESS != eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3))
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC, set Ext_CSD[179]: %Xh fail\n",BIT3);
        eMMC_FCIE_ErrHandler_Stop();
    }

    return u32_err;
}


U32 eMMC_RPMB_Get_Counter(volatile U32 *pu32_WCnt)
{
	U32 u32_err;

    memset((void*)(&g_eMMCRPMB),'\0',sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB.u16_req_rsp = eMMC_RPMB_REQ_WRITE_CNT_VAL;

    u32_err = eMMC_RPMB_NoDataReq();
    if(eMMC_ST_SUCCESS == u32_err)
        *pu32_WCnt = g_eMMCRPMB.u32_writecnt;

    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Get Counter fail: %Xh \n", u32_err);
    }

    return u32_err;
}


U32 eMMC_RPMB_Write_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_blk_addr)
{
    U32 u32_err=0;
    volatile U32 u32_wcnt;
    U16 u16_i,u16_BlkCnt;
    U8  u8_eMMC_MAC_CalBuf[568];
    U8  u8_digest[32];
    U8  u8_auto_key[32];
    U8  u8_input_key[32];

    if(NULL == pu8_DataBuf)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Write buf is NULL: %Xh\n", (U32)pu8_DataBuf);
        return eMMC_ST_ERR_INVALID_PARAM;
    }

    if(u32_DataByteCnt & 0xFF)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: data not 256B boundary: %Xh\n",u32_DataByteCnt);
        return eMMC_ST_ERR_INVALID_PARAM;
    }
    if(u16_blk_addr + (u32_DataByteCnt>>8)> g_eMMCDrv.u32_RPMB_SIZE_SEC_CNT)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: invalid data range, %Xh > %Xh \n",
            u16_blk_addr + (u32_DataByteCnt>>8), g_eMMCDrv.u32_RPMB_SIZE_SEC_CNT);
        return eMMC_ST_ERR_INVALID_PARAM;
    }

    GetHmacKey(u8_auto_key);

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh, eMMC, set Ext_CSD[179]: %Xh fail\n",
            u32_err, BIT3|BIT1|BIT0);
        goto LABEL_END;
    }
    // write data
    while(u32_DataByteCnt)
    {
        u32_wcnt = 0;
        eMMC_RPMB_Get_Counter(&u32_wcnt);
        if(g_eMMCRPMB.u16_result & 0x80)
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

        memset(u8_eMMC_MAC_CalBuf,'\0',2*284);
        memset(gau8_eMMC_SectorBuf,'\0',2<<9);

        for(u16_i=0;u16_i<u16_BlkCnt;u16_i++)
        {
            memset(&g_eMMCRPMB,'\0',sizeof(eMMC_RPMB_DATA));
            g_eMMCRPMB.u32_writecnt = u32_wcnt;
            g_eMMCRPMB.u16_addr     = u16_blk_addr;
            g_eMMCRPMB.u16_blk_cnt  = u16_BlkCnt;
            g_eMMCRPMB.u16_req_rsp  = eMMC_RPMB_REQ_AUTH_DATA_W;
            memcpy(g_eMMCRPMB.u8_data,pu8_DataBuf+(u16_i<<8),256);
            eMMC_RPMB_SWAP_memcpy(u8_eMMC_MAC_CalBuf+u16_i*284,(U8*)(&g_eMMCRPMB),284);
            eMMC_RPMB_SWAP_memcpy(gau8_eMMC_SectorBuf+(u16_i<<9),(U8*)(&g_eMMCRPMB),sizeof(eMMC_RPMB_DATA));
        }

        eMMC_RPMB_SWAP_memcpy(u8_input_key,u8_auto_key,32);
        hmac_sha256(u8_eMMC_MAC_CalBuf,u16_i*284,u8_input_key,32,u8_digest);
        memcpy(gau8_eMMC_SectorBuf+(((u16_i-1)<<9)+196),u8_digest,32);

        u32_err = eMMC_CMD25_RPMB_MIU(0,gau8_eMMC_SectorBuf, u16_BlkCnt, 1);
        if(eMMC_ST_SUCCESS != u32_err)
            break;

        //=============read result==========================
        memset(&g_eMMCRPMB,'\0',sizeof(eMMC_RPMB_DATA));
        g_eMMCRPMB.u16_req_rsp = eMMC_RPMB_REQ_RESULT_R;
		u32_err = eMMC_RPMB_NoDataReq();

        if(eMMC_ST_SUCCESS != u32_err)
            break;

        u16_blk_addr += u16_BlkCnt;
        pu8_DataBuf  += u16_BlkCnt<<8;
        u32_DataByteCnt -= u16_BlkCnt<<8;
    }

    LABEL_END:
    if(eMMC_ST_SUCCESS != eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3))
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC, set Ext_CSD[179]: %Xh fail\n",BIT3);
        eMMC_FCIE_ErrHandler_Stop();
    }
    return u32_err;

}

U32 eMMC_RPMB_Verify_Hmac(U8* pu8_DataBuf, U16 u16_BlkCnt)
{
     U32 u32_err=0;
     U16 u16_i;
     U8  u8_eMMC_MAC_CalBuf[568];
     U8  u8_digest_golden[32];
     U8  u8_digest_from_eMMC[32];
     U8  u8_auto_key[32];
     U8  u8_input_key[32];
     GetHmacKey(u8_auto_key);
     eMMC_RPMB_SWAP_memcpy(u8_input_key,u8_auto_key,32);

     for(u16_i=0;u16_i<u16_BlkCnt;u16_i++)
     {
         memcpy(u8_eMMC_MAC_CalBuf+(u16_i*284),pu8_DataBuf+(u16_i<<9)+228,284);
     }

     memcpy(u8_digest_from_eMMC, pu8_DataBuf+((u16_BlkCnt-1)<<9)+196 ,32);
     hmac_sha256(u8_eMMC_MAC_CalBuf,u16_BlkCnt*284,u8_input_key,32,u8_digest_golden);

     if(memcmp(u8_digest_golden, u8_digest_from_eMMC,32))
     {
         eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: HMAC Mismatch!\n");
         eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"HMAC real:\n");
         eMMC_dump_mem(u8_digest_golden,32);
         eMMC_debug(0,0,"\n");
         eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"HMAC from eMMC:\n");
         eMMC_dump_mem(u8_digest_from_eMMC,32);
         eMMC_debug(0,0,"\n");
         u32_err = eMMC_ST_ERR_RPMB_HMAC_ERR;
     }

     return u32_err;
}

U32 __eMMC_RPMB_Read_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_blk_addr, U8 verify)
{
    U32 u32_err=0;
    U16 u16_i,u16_BlkCnt;

    if(NULL == pu8_DataBuf)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: r data buf is NULL: %Xh\n", (U32)pu8_DataBuf);
        return eMMC_ST_ERR_INVALID_PARAM;
    }

    if(u32_DataByteCnt & 0xFF)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: data not 256B boundary: %Xh\n",u32_DataByteCnt);
        return eMMC_ST_ERR_INVALID_PARAM;
    }
    if(u16_blk_addr + (u32_DataByteCnt>>8)> g_eMMCDrv.u32_RPMB_SIZE_SEC_CNT)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: invalid data range, %Xh > %Xh \n",
            u16_blk_addr + (u32_DataByteCnt>>8), g_eMMCDrv.u32_RPMB_SIZE_SEC_CNT);
        return eMMC_ST_ERR_INVALID_PARAM;
    }

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh, eMMC, set Ext_CSD[179]: %Xh fail\n",
            u32_err, BIT3|BIT1|BIT0);
        goto LABEL_END;
    }
    // read data
    while(u32_DataByteCnt)
    {
        memset(&g_eMMCRPMB,'\0',sizeof(eMMC_RPMB_DATA));
        g_eMMCRPMB.u16_addr     = u16_blk_addr;
        g_eMMCRPMB.u16_req_rsp  = eMMC_RPMB_REQ_AUTH_DATA_R;

        eMMC_RPMB_SWAP_memcpy(gau8_eMMC_SectorBuf,(U8*)(&g_eMMCRPMB),sizeof(eMMC_RPMB_DATA));
        u32_err = eMMC_CMD25_RPMB_MIU(0,gau8_eMMC_SectorBuf, 1, 0);
        if(eMMC_ST_SUCCESS != u32_err)
            break;

        if(u16_blk_addr & 1)//block address is odd
            u16_BlkCnt = 1;
        else//block address is even
        {
            if(u32_DataByteCnt >= 512)
                u16_BlkCnt = 2;
            else
                u16_BlkCnt = 1;
        }
        u32_err = eMMC_CMD18_RPMB_MIU(0,gau8_eMMC_SectorBuf, u16_BlkCnt);

        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: R fail: %Xh\n", u32_err);
            break;
        }

        if( verify != 0 )
        {
            u32_err = eMMC_RPMB_Verify_Hmac(gau8_eMMC_SectorBuf, u16_BlkCnt);
            if(eMMC_ST_SUCCESS != u32_err)
            {
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: R fail: %Xh\n", u32_err);
                break;
            }
        }

        for(u16_i=0;u16_i<u16_BlkCnt;u16_i++)
        {
            eMMC_RPMB_SWAP_memcpy(pu8_DataBuf+(u16_i<<8),gau8_eMMC_SectorBuf+(u16_i<<9)+228,256);
            eMMC_RPMB_Copy_RESULT((U8*)(&g_eMMCRPMB),gau8_eMMC_SectorBuf+(u16_i<<9),sizeof(eMMC_RPMB_DATA));
            u32_err = eMMC_RPMB_CheckResultError();
            if(u32_err)
                break;
        }

        if(u16_i != u16_BlkCnt)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: R fail: %Xh\n", u32_err);
            break;
        }

        u16_blk_addr += u16_BlkCnt;
        pu8_DataBuf  += u16_BlkCnt << 8;
        u32_DataByteCnt -= u16_BlkCnt << 8;
    }

    LABEL_END:
    if(eMMC_ST_SUCCESS != eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3))
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC, set Ext_CSD[179]: %Xh fail\n",BIT3);
        eMMC_FCIE_ErrHandler_Stop();
    }


    return u32_err;
}

U32 eMMC_RPMB_Read_data(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_blk_addr)
{
    return __eMMC_RPMB_Read_data(pu8_DataBuf, u32_DataByteCnt, u16_blk_addr, 1);
}

U32 eMMC_RPMB_Read_data_no_verify(U8* pu8_DataBuf, U32 u32_DataByteCnt, U16 u16_blk_addr)
{
    return __eMMC_RPMB_Read_data(pu8_DataBuf, u32_DataByteCnt, u16_blk_addr, 0);
}

U32 eMMC_RPMB_Read_Blk(U8 *pu8_DataBuf, U8 *pu8_nonce, U16 u16_BlkAddr)
{
    U32 u32_err=0;
    U16 u16_i,u16_BlkCnt;

    if(NULL == pu8_DataBuf)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: data buf is NULL: %Xh\n", (U32)pu8_DataBuf);
        return eMMC_ST_ERR_INVALID_PARAM;
    }

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh, eMMC, set Ext_CSD[179]: %Xh fail\n",
            u32_err, BIT3|BIT1|BIT0);
        goto LABEL_END;
    }
    // read RPMB raw data
    memset(&g_eMMCRPMB,'\0',sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB.u16_addr     = u16_BlkAddr;
    g_eMMCRPMB.u16_req_rsp  = eMMC_RPMB_REQ_AUTH_DATA_R;
    memcpy(g_eMMCRPMB.u8_nonce, pu8_nonce, 16);


    eMMC_RPMB_SWAP_memcpy(gau8_eMMC_SectorBuf,(U8*)(&g_eMMCRPMB),sizeof(eMMC_RPMB_DATA));
    u32_err = eMMC_CMD25_RPMB_MIU(0, gau8_eMMC_SectorBuf, 1, 0);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    u32_err = eMMC_CMD18_RPMB_MIU(0, pu8_DataBuf, 1);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    eMMC_RPMB_Copy_RESULT((U8*)(&g_eMMCRPMB), pu8_DataBuf, sizeof(eMMC_RPMB_DATA));
    u32_err = eMMC_RPMB_CheckResultError();

    LABEL_END:
    if(eMMC_ST_SUCCESS != eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3))
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC, set Ext_CSD[179]: %Xh fail\n",BIT3);
        eMMC_FCIE_ErrHandler_Stop();
    }


    return u32_err;
}

U32 eMMC_RPMB_Write_Blk(U8 *pu8_DataBuf)
{
    U32 u32_err=0;


    if(NULL == pu8_DataBuf)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Write buf is NULL: %Xh\n", (U32)pu8_DataBuf);
        return eMMC_ST_ERR_INVALID_PARAM;
    }

    u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3|BIT1|BIT0);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh, eMMC, set Ext_CSD[179]: %Xh fail\n",
            u32_err, BIT3|BIT1|BIT0);
        goto LABEL_END;
    }
    // copy nonce
    memcpy(&g_eMMCRPMB, pu8_DataBuf, sizeof(eMMC_RPMB_DATA));


    // write data
    eMMC_RPMB_SWAP_memcpy(gau8_eMMC_SectorBuf, pu8_DataBuf, 512);
    u32_err = eMMC_CMD25_RPMB_MIU(0,gau8_eMMC_SectorBuf, 1, 1);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    //=============read result==========================
    memset(&g_eMMCRPMB,'\0',sizeof(eMMC_RPMB_DATA));
    g_eMMCRPMB.u16_req_rsp = eMMC_RPMB_REQ_RESULT_R;
    u32_err = eMMC_RPMB_NoDataReq();

    LABEL_END:
    if(eMMC_ST_SUCCESS != eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, 179, BIT3))
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: eMMC, set Ext_CSD[179]: %Xh fail\n",BIT3);
        eMMC_FCIE_ErrHandler_Stop();
    }
    return u32_err;
}

U32 eMMC_RPMB_Test(void)
{
    U32 u32_err=0;
    #if defined(IF_IP_VERIFY) && IF_IP_VERIFY
    U32 u32_i,u32_DataByteCnt=128*1024;
    U16 u16_blk_addr=0;

    if(g_eMMCDrv.u32_RPMB_SIZE_SEC_CNT ==0)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC has no RPMB partition size\n");
        goto LABEL_END;
    }

    if( !gau8_WBuf )
        gau8_WBuf = (U8*)memalign(eMMC_CACHE_LINE, u32_DataByteCnt);

    if( !gau8_RBuf)
        gau8_RBuf = (U8*)memalign(eMMC_CACHE_LINE, u32_DataByteCnt);

    for(u32_i=0; u32_i<u32_DataByteCnt; u32_i++)
    {
        gau8_WBuf[u32_i] = (U8)(u32_i & 0xFF);
        gau8_RBuf[u32_i] = ~gau8_WBuf[u32_i];
    }

    u32_err = eMMC_RPMB_Write_data(gau8_WBuf,u32_DataByteCnt,u16_blk_addr);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;
    u32_err = eMMC_RPMB_Read_data(gau8_RBuf,u32_DataByteCnt,u16_blk_addr);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    u32_err = eMMC_ComapreData(gau8_WBuf,gau8_RBuf,u32_DataByteCnt);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    LABEL_END:

    if( gau8_WBuf )
    {
        free(gau8_WBuf);
        gau8_WBuf = 0;
    }

    if( gau8_RBuf )
    {
        free(gau8_RBuf);
        gau8_RBuf = 0;
    }

    #endif
    return u32_err;
}
#endif

