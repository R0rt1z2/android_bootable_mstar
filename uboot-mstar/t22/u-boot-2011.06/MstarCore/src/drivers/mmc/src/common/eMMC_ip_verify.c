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
#include "../disk/part_emmc.h"
#include <malloc.h>

#if defined (UNIFIED_eMMC_DRIVER) && UNIFIED_eMMC_DRIVER

#define DDR_TEST_BLK_CNT        8
#define DDR_TEST_BUFFER_SIZE    (eMMC_SECTOR_512BYTE*DDR_TEST_BLK_CNT)

eMMC_ALIGN0 static U8 gau8_WBuf_DDR[DDR_TEST_BUFFER_SIZE] eMMC_ALIGN1;
eMMC_ALIGN0 static U8 gau8_RBuf_DDR[DDR_TEST_BUFFER_SIZE] eMMC_ALIGN1;

U8 u8_crazy_pattern[] = {
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE,
    0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD,
    0x04, 0xFB, 0x04, 0xFB, 0x04, 0xFB, 0x04, 0xFB,
    0x08, 0xF7, 0x08, 0xF7, 0x08, 0xF7, 0x08, 0xF7,
    0x10, 0xEF, 0x10, 0xEF, 0x10, 0xEF, 0x10, 0xEF,
    0x20, 0xDF, 0x20, 0xDF, 0x20, 0xDF, 0x20, 0xDF,
    0x40, 0xBF, 0x40, 0xBF, 0x40, 0xBF, 0x40, 0xBF,
    0x80, 0x7F, 0x80, 0x7F, 0x80, 0x7F, 0x80, 0x7F,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01,
    0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02, 0xFD, 0x02,
    0xFB, 0x04, 0xFB, 0x04, 0xFB, 0x04, 0xFB, 0x04,
    0xF7, 0x08, 0xF7, 0x08, 0xF7, 0x08, 0xF7, 0x08,
    0xEF, 0x10, 0xEF, 0x10, 0xEF, 0x10, 0xEF, 0x10,
    0xDF, 0x20, 0xDF, 0x20, 0xDF, 0x20, 0xDF, 0x20,
    0xBF, 0x40, 0xBF, 0x40, 0xBF, 0x40, 0xBF, 0x40,
    0x7F, 0x80, 0x7F, 0x80, 0x7F, 0x80, 0x7F, 0x80
};

U32 eMMCTest_BlkWRC_ProbeTiming(U32 u32_eMMC_Addr)
{
	U32 u32_err = eMMC_ST_SUCCESS;
	U32 u32_i, u32_j, *pu32_W=(U32*)gau8_WBuf_DDR, *pu32_R=(U32*)gau8_RBuf_DDR;
	U32 u32_BlkCnt=eMMC_TEST_BLK_CNT, u32_BufByteCnt;

	u32_BlkCnt = u32_BlkCnt > DDR_TEST_BLK_CNT ? DDR_TEST_BLK_CNT : u32_BlkCnt;
	u32_BufByteCnt = u32_BlkCnt << eMMC_SECTOR_512BYTE_BITS;

	for(u32_i=0; u32_i<u32_BufByteCnt>>2; u32_i++)
	    pu32_R[u32_i] = 0;

	for(u32_j=0; u32_j<10; u32_j++)
	{
		// init data pattern
		switch(u32_j)
		{
			case 0: // increase
			    #if 1
                for(u32_i=0; u32_i<u32_BufByteCnt; u32_i++)
	                 gau8_WBuf_DDR[u32_i] = (U8)(u32_i & 0xFF);
			    break;
				#else
				continue;
				#endif
			case 1: // decrease
			    #if 1
				for(u32_i=0; u32_i<u32_BufByteCnt; u32_i++)
					gau8_WBuf_DDR[u32_i] = 0xFF - ((U8)(u32_i & 0xFF));
				break;
				#else
				continue;
				#endif
			case 2: // 0xF00F
				for(u32_i=0; u32_i<u32_BufByteCnt>>2; u32_i++)
				    pu32_W[u32_i]=0xF00FF00F;
				break;
			case 3: // 0xFF00
			    #if 1
				for(u32_i=0; u32_i<u32_BufByteCnt>>2; u32_i++)
				    pu32_W[u32_i]=0xFF00FF00;
				break;
				#else
				continue;
				#endif
			case 4: // 0x5AA5
				for(u32_i=0; u32_i<u32_BufByteCnt>>2; u32_i++)
				    pu32_W[u32_i]=0x5AA55AA5;
				break;
			case 5: // 0x55AA
			    #if 0
				for(u32_i=0; u32_i<u32_BufByteCnt>>2; u32_i++)
				    pu32_W[u32_i]=0x55AA55AA;
				break;
				#else
				continue;
				#endif
			case 6: // 0x5A5A
			    #if 0
				for(u32_i=0; u32_i<u32_BufByteCnt>>2; u32_i++)
				    pu32_W[u32_i]=0x5A5A5A5A;
				break;
				#else
                continue;
				#endif
			case 7: // 0x0000
			    #if 1
				for(u32_i=0; u32_i<u32_BufByteCnt>>2; u32_i++)
				    pu32_W[u32_i]=0x00000000;
				break;
				#else
				continue;
				#endif
			case 8: // 0xFFFF
			    #if 0
				for(u32_i=0; u32_i<u32_BufByteCnt>>2; u32_i++)
				    pu32_W[u32_i]=0xFFFFFFFF;
				break;
				#else
				continue;
				#endif
            case 9:
            {
                U16 u8_pat_size = sizeof(u8_crazy_pattern);
                U16 u8_rep = u32_BufByteCnt / u8_pat_size;
                U16 u8_tail = u32_BufByteCnt % u8_pat_size;

                for(u32_i=0; u32_i<u8_rep; u32_i++)
                    memcpy(gau8_WBuf_DDR+(u32_i*u8_pat_size), u8_crazy_pattern, u8_pat_size);

                if( u8_tail != 0 )
                    memcpy(gau8_WBuf_DDR+(u32_i*u8_pat_size), u8_crazy_pattern, u8_tail);
                break;
            }
		}

		#if 1
		u32_err = eMMC_CMD24_MIU(u32_eMMC_Addr, gau8_WBuf_DDR);
        if(eMMC_ST_SUCCESS != u32_err)
	        break;

		u32_err = eMMC_CMD17_MIU(u32_eMMC_Addr, gau8_RBuf_DDR);
		if(eMMC_ST_SUCCESS != u32_err)
            break;

		u32_err = eMMC_ComapreData(gau8_WBuf_DDR, gau8_RBuf_DDR, eMMC_SECTOR_512BYTE);
    	if(eMMC_ST_SUCCESS != u32_err)
	    {
		    eMMC_debug(0,1,"Err, compare fail.single, %Xh \n", u32_err);
            break;
	    }
		#endif
        u32_err = eMMC_CMD25_MIU(u32_eMMC_Addr, gau8_WBuf_DDR, u32_BlkCnt);
	    if(eMMC_ST_SUCCESS != u32_err)
	        break;

	    u32_err = eMMC_CMD18_MIU(u32_eMMC_Addr, gau8_RBuf_DDR, u32_BlkCnt);
	    if(eMMC_ST_SUCCESS != u32_err)
            break;

	    u32_err = eMMC_ComapreData(gau8_WBuf_DDR, gau8_RBuf_DDR, u32_BufByteCnt);
    	if(eMMC_ST_SUCCESS != u32_err)
	    {
		    eMMC_debug(0,1,"Err, compare fail.multi %Xh \n", u32_err);
            break;
	    }

	}

	if(eMMC_ST_SUCCESS != u32_err)
	{
		//eMMC_debug(0,0,"%s() fail %Xh\n", __FUNCTION__, u32_err);
		//eMMC_FCIE_ErrHandler_ReInit();
		eMMC_debug(1,1,"data pattern %u: %02X%02X%02X%02Xh \n\n",
		    u32_j, gau8_WBuf_DDR[3], gau8_WBuf_DDR[2], gau8_WBuf_DDR[1], gau8_WBuf_DDR[0]);
	}
	//eMMC_debug(0,0,"%s() return %Xh\n", __FUNCTION__, u32_err);
	return u32_err;
}


//=============================================================
#if defined(IF_DETECT_eMMC_DDR_TIMING) && IF_DETECT_eMMC_DDR_TIMING


// can use for RF team test
U32 eMMCTest_KeepR_TestDDR(U32 u32_LoopCnt)
{
	U32 u32_err;
	U32 u32_i, u32_j, u32_k;
	U32 u32_BlkCnt, u32_eMMC_Addr;

	u32_eMMC_Addr = eMMC_TEST_BLK_0;

	u32_BlkCnt = eMMC_TEST_BLK_CNT;
	u32_BlkCnt = u32_BlkCnt > DDR_TEST_BLK_CNT ? DDR_TEST_BLK_CNT : u32_BlkCnt;

	if(8 != u32_BlkCnt)
	{
		eMMC_debug(0,1,"Blk count needs to be 8 \n");
		while(1);
	}

	for(u32_j=0; u32_j<u32_BlkCnt; u32_j++){
		// init data pattern
		switch(u32_j)
		{
			case 0: // increase
                for(u32_i=0; u32_i<eMMC_SECTOR_512BYTE; u32_i++)
	                gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i] = u32_i+1;
			    break;
			case 1: // 0xF00F
				for(u32_i=0; u32_i<eMMC_SECTOR_512BYTE/2; u32_i++)
				{
					gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i]=0xF0;
				    gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i+1]=0x0F;
				}
				break;
			case 2: // 0xFF00
				for(u32_i=0; u32_i<eMMC_SECTOR_512BYTE/2; u32_i++)
				{
					gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i]=0xFF;
					gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i+1]=0;
				}
				break;
			case 3: // 0x5AA5
				for(u32_i=0; u32_i<eMMC_SECTOR_512BYTE/2; u32_i++)
				{
					gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i]=0x5A;
					gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i+1]=0xA5;
				}
				break;
			case 4: // 0x55AA
				for(u32_i=0; u32_i<eMMC_SECTOR_512BYTE/2; u32_i++)
				{
					gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i]=0x55;
					gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i+1]=0xAA;
				}
				break;
			case 5: // 0x5A5A
				for(u32_i=0; u32_i<eMMC_SECTOR_512BYTE/2; u32_i++)
				{
					gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i]=0x5A;
					gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i+1]=0x5A;
				}
				break;
			case 6: // 0x0000
				for(u32_i=0; u32_i<eMMC_SECTOR_512BYTE/2; u32_i++)
				{
					gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i]=0;
					gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i+1]=0;
				}
				break;
			case 7: // 0xFFFF
				for(u32_i=0; u32_i<eMMC_SECTOR_512BYTE/2; u32_i++)
				{
					gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i]=0xFF;
					gau8_WBuf_DDR[(u32_j<<eMMC_SECTOR_512BYTE_BITS)+u32_i+1]=0xFF;
				}
				break;

		}

		u32_err = eMMC_CMD25_MIU(u32_eMMC_Addr, gau8_WBuf_DDR, u32_BlkCnt);
        if(eMMC_ST_SUCCESS != u32_err)
	        break;
	}


	for(u32_k=0; u32_k<u32_LoopCnt; u32_k++){

	    u32_err = eMMC_CMD18_MIU(u32_eMMC_Addr, gau8_RBuf_DDR, u32_BlkCnt/2);
	    if(eMMC_ST_SUCCESS != u32_err)
            break;

	    u32_err = eMMC_ComapreData(gau8_WBuf_DDR, gau8_RBuf_DDR, (u32_BlkCnt/2)<<eMMC_SECTOR_512BYTE_BITS);
    	if(eMMC_ST_SUCCESS != u32_err)
	    {
		    eMMC_debug(0,1,"Err 1, compare fail, %Xh \n", u32_err);
            break;
	    }

		for(u32_j=0; u32_j<u32_BlkCnt/2; u32_j++)
		{
			u32_err = eMMC_CMD17_MIU(u32_eMMC_Addr+u32_BlkCnt/2+u32_j,
				gau8_RBuf_DDR+ ((u32_BlkCnt/2+u32_j)<<eMMC_SECTOR_512BYTE_BITS));
	        if(eMMC_ST_SUCCESS != u32_err)
                break;

			u32_err = eMMC_ComapreData(
				gau8_WBuf_DDR+ ((u32_BlkCnt/2+u32_j)<<eMMC_SECTOR_512BYTE_BITS),
				gau8_RBuf_DDR+ ((u32_BlkCnt/2+u32_j)<<eMMC_SECTOR_512BYTE_BITS),
				eMMC_SECTOR_512BYTE);
    	    if(eMMC_ST_SUCCESS != u32_err)
	        {
		        eMMC_debug(0,1,"Err 2, compare fail, %Xh \n", u32_err);
                break;
	        }
		}
	}

	return u32_err;
}


#endif


//=============================================================
#if defined(IF_IP_VERIFY) && IF_IP_VERIFY

#define TEST_BUFFER_SIZE        0x100000 // 1MB
U8 *gau8_WBuf = 0, *gau8_RBuf = 0;

U32 eMMC_BootMode(U32 *pu32_Addr, U32 u32_ByteCnt);
#define eMMC_PATTERN_00000000   0x00000000 // smooth
#define eMMC_PATTERN_FFFFFFFF   0xFFFFFFFF // smooth
#define eMMC_PATTERN_000000FF   0x000000FF // board
#define eMMC_PATTERN_0000FFFF   0x0000FFFF // board
#define eMMC_PATTERN_00FF00FF   0x00FF00FF // board
#define eMMC_PATTERN_AA55AA55   0xAA55AA55 // board

U32  eMMCTest_SingleBlkWRC_RIU(U32 u32_eMMC_Addr, U32 u32_DataPattern);
U32  eMMCTest_SingleBlkWRC_MIU(U32 u32_eMMC_Addr, U32 u32_DataPattern);
U32  eMMCTest_SingleBlkWRC_MIU_Ex(U32 u32_eMMC_Addr, U8 *pu8_W, U8 *pu8_R);

U32  eMMCTest_SgWRC_MIU(U32 u32_eMMC_Addr, U16 u16_BlkCnt, U32 u32_DataPattern);
U32  eMMCTest_SgW_MIU(U32 u32_eMMC_Addr);
U32  eMMCTest_SgR_MIU(U32 u32_eMMC_Addr);
U32  eMMCTest_MultiBlkWRC_MIU(U32 u32_eMMC_Addr, U16 u16_BlkCnt, U32 u32_DataPattern);
U32  eMMC_IPVerify_Main_Ex(U32 u32_DataPattern);
U32  eMMC_IPVerify_Main_Sg_Ex(U32 u32_DataPattern);
U32  eMMC_IPVerify_Main_API_Ex(U32 u32_DataPattern);

void eMMCTest_DownCount(U32 u32_Sec);

#define MAX_SCATTERLIST_COUNT 0x10
#define eMMC_GENERIC_WAIT_TIME  (HW_TIMER_DELAY_1s*2) // 2 sec

static struct _scatterlist
{
	U32	u32_length;
	U32	u32_dma_address;
} pSG_st[MAX_SCATTERLIST_COUNT];

struct _AdmaDescriptor p_AdmaDesc_st[MAX_SCATTERLIST_COUNT] eMMC_ALIGN1;

#if defined(ENABLE_CMDQ) && ENABLE_CMDQ

U32 eMMCTest_CMDQ_Legacy(void)
{
    int i, j;
    U32 u32_err;
    U16 u16_test_blk_cnt[] = {1, 2, 3, 4, 8};
    U32 u32_test_blk_addr[] = {0xA00000, 0xA00000+0x10, 0xA00000+0x100, 0xA00000+0x1000, 0xA00000+0x10000};
    U32 u32_QSR_sts = 0;

    // Queue Read Task test
    for(i=0; i<5; i++)
    {
        if( eMMC_CMD44(u16_test_blk_cnt[i], 1, i) != eMMC_ST_SUCCESS )
            eMMC_debug(0, 1, "CMD44 fail to queue task %d\n", i);
        if( eMMC_CMD45(u32_test_blk_addr[i]) != eMMC_ST_SUCCESS )
            eMMC_debug(0, 1, "CMD45 fail to queue task %d\n", i);
        eMMC_debug(0, 1, "Queue task %d ok\n", i);
    }

    for(j=0; j<5; j++)
    {
        eMMC_hw_timer_delay(1000000);

        u32_QSR_sts = 0;

        // Check QSR
        eMMC_CMD13_Check_QSR(g_eMMCDrv.u16_RCA);

        u32_QSR_sts = ( g_eMMCDrv.au8_Rsp[1] << 24) | (g_eMMCDrv.au8_Rsp[2] << 16) |
                      ( g_eMMCDrv.au8_Rsp[3] << 8) | (g_eMMCDrv.au8_Rsp[4]);

        for(i=0; i<5; i++)
        {
            if( u32_QSR_sts & (1<<i) )
            {
                eMMC_debug(0, 0, "Taskid %d is ready\n", i);
            }
        }

        eMMC_debug(0, 1, "QSR: u32_QSR_sts=%08X\n", u32_QSR_sts);

        if( u32_QSR_sts & (1<<j) )
        {
            u32_err = eMMC_CMD46_MIU(j, (U8 *)DMA_R_ADDR, u16_test_blk_cnt[j]);
            if( u32_err != eMMC_ST_SUCCESS )
            {
                eMMC_debug(0, 1, "Fail to read Queue task %d\n", j);
                return -1;
            }
            else
                eMMC_debug(0, 1, "Read queue task %d ok\n", j);
        }
    }

    // Queue Write Task test
    for(i=0; i<5; i++)
    {
        if( eMMC_CMD44(u16_test_blk_cnt[i], 0, i) != eMMC_ST_SUCCESS )
            eMMC_debug(0, 1, "CMD44 fail to queue task %d\n", i);
        if( eMMC_CMD45(u32_test_blk_addr[i]) != eMMC_ST_SUCCESS )
            eMMC_debug(0, 1, "CMD45 fail to queue task %d\n", i);
        eMMC_debug(0, 1, "Queue task %d ok\n", i);
    }

    for(j=0; j<5; j++)
    {
        eMMC_hw_timer_delay(1000000);

        u32_QSR_sts = 0;

        // Check QSR
        eMMC_CMD13_Check_QSR(g_eMMCDrv.u16_RCA);

        u32_QSR_sts = ( g_eMMCDrv.au8_Rsp[1] << 24) | (g_eMMCDrv.au8_Rsp[2] << 16) |
                      ( g_eMMCDrv.au8_Rsp[3] << 8) | (g_eMMCDrv.au8_Rsp[4]);

        for(i=0; i<5; i++)
        {
            if( u32_QSR_sts & (1<<i) )
            {
                eMMC_debug(0, 0, "Taskid %d is ready\n", i);
            }
        }

        eMMC_debug(0, 1, "QSR: u32_QSR_sts=%08X\n", u32_QSR_sts);

        if( u32_QSR_sts & (1<<j) )
        {
            u32_err = eMMC_CMD47_MIU(j, (U8 *)DMA_R_ADDR, u16_test_blk_cnt[j]);
            if( u32_err != eMMC_ST_SUCCESS )
            {
                eMMC_debug(0, 1, "Fail to read Queue task %d\n", j);
                return -1;
            }
            else
                eMMC_debug(0, 1, "Read queue task %d ok\n", j);
        }
    }

    return 0;
}

struct _LinkDescriptor LinkDescW[32] __attribute__((aligned(32)));
struct _LinkDescriptor LinkDescR[32] __attribute__((aligned(32)));


U32 eMMCTest_CMDQ_FCIE(U32 u32_DataPattern)
{
    U32 u32_err;
    U32 u32_i;
    U32 *pu32_W=(U32*)gau8_WBuf;
    U32 *pu32_R=(U32*)gau8_RBuf;
	U32 u32_temp;
    U8 *pu8_W = gau8_WBuf;
    U8 *pu8_R = gau8_RBuf;

    U16 u16_task_count = 5;
    U32 u32_task_id;

    U16 u16_test_blk_cnt[] = {1, 2, 32, 128, 1024};
    U32 u32_test_blk_addr[] = {0xA00000, 0xA00000+0x10, 0xA00000+0x100, 0xA00000+0x1000, 0xA00000+0x8000};


    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "Data Pattern: %08Xh\n", u32_DataPattern);

    eMMC_debug(0, 1, "Preparing pattern, wait ...\n");

    // Prepare data buffer
    if(u32_DataPattern == 0x00112233 || (u32_DataPattern == 0x33221100))
    {
        for(u32_i = 0; u32_i < TEST_BUFFER_SIZE; u32_i++)
        {
            if(u32_DataPattern == 0x00112233)
            {
                gau8_WBuf[u32_i] = u32_i & 0xFF;
            }
            else
            {
                gau8_WBuf[u32_i] = 0xFF - (u32_i & 0xFF);
            }
            gau8_RBuf[u32_i] = ~gau8_WBuf[u32_i];
        }
    }
    else
    {
        for(u32_i=0; u32_i<TEST_BUFFER_SIZE>>2; u32_i++)
        {
            pu32_W[u32_i] = u32_DataPattern;
            pu32_R[u32_i] = ~pu32_W[u32_i];
        }
    }

    eMMC_debug(0, 1, "Preparing pattern, ok\n");


    memset(LinkDescW, 0, sizeof(struct _LinkDescriptor)*32);

    // Prepare link descriptor
    u32_temp =0;
    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        u32_task_id = u32_i;

        LinkDescW[u32_task_id].u32_Address  = eMMC_translate_DMA_address_Ex((U32)(pu8_W +u32_temp), u16_test_blk_cnt[u32_i]<<9);
        LinkDescW[u32_task_id].u32_JobCnt   = u16_test_blk_cnt[u32_i];
        LinkDescW[u32_task_id].u32_Dir      = 1;    // write
        LinkDescW[u32_task_id].u32_Link     = 0;    // not ADMA mode
        LinkDescW[u32_task_id].u32_MiuSel   = 0;
        LinkDescW[u32_task_id].u32_Length   = (u16_test_blk_cnt[u32_i]<<9);
        LinkDescW[u32_task_id].u32_End      = 1;
		u32_temp += (u16_test_blk_cnt[u32_i]*512);
    }

    #if 0
    eMMC_dump_mem((U8 *)LinkDescW, sizeof(struct _LinkDescriptor)*(u16_task_count));
    #endif

    u32_err = eMMC_CMDQ_Task(LinkDescW, u32_test_blk_addr, u16_task_count);
    if( u32_err )
    {
        eMMC_debug(0, 1, "CMDQ write test fail\n");
        while(1);
    }

    eMMC_debug(0, 1, "CMDQ write test ok\n");



    memset(LinkDescR, 0, sizeof(struct _LinkDescriptor)*32);

    // Prepare link descriptor
    u32_temp =0;
    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        u32_task_id = u32_i;

        LinkDescR[u32_task_id].u32_Address  = eMMC_translate_DMA_address_Ex((U32)(pu8_R+u32_temp), u16_test_blk_cnt[u32_i]<<9);
        LinkDescR[u32_task_id].u32_JobCnt   = u16_test_blk_cnt[u32_i];
        LinkDescR[u32_task_id].u32_Dir      = 0;    // read
        LinkDescR[u32_task_id].u32_Link     = 0;    // not ADMA mode
        LinkDescR[u32_task_id].u32_MiuSel   = 0;
        LinkDescR[u32_task_id].u32_Length   = (u16_test_blk_cnt[u32_i]<<9);
        LinkDescR[u32_task_id].u32_End      = 1;

        u32_temp += (u16_test_blk_cnt[u32_i]*512);
    }

    #if 0
    eMMC_dump_mem((U8 *)LinkDescR, sizeof(struct _LinkDescriptor)*(u16_task_count));
    #endif

    u32_err = eMMC_CMDQ_Task(LinkDescR, u32_test_blk_addr, u16_task_count);
    if( u32_err )
    {
        eMMC_debug(0, 1, "CMDQ read test fail\n");
        while(1);
    }

    eMMC_debug(0, 1, "CMDQ read test ok\n");


    eMMC_debug(0, 1, "Comparing data, wait...\n");
	u32_temp =0;
    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        u32_task_id = u32_i;

        u32_err = eMMC_ComapreData(pu8_W+u32_temp, pu8_R+u32_temp, LinkDescR[u32_task_id].u32_JobCnt<<eMMC_SECTOR_512BYTE_BITS);
		u32_temp += (LinkDescR[u32_task_id].u32_JobCnt*512);

        if(u32_err)
        {
            eMMC_debug(0, 1,"CMDQ compare taskid %d item fail\n", u32_task_id);

            eMMC_FCIE_DumpRegisters();
            eMMC_FCIE_DumpDebugBus();

            while(1);

            return u32_err;
        }
    }

    eMMC_debug(0, 1, "CMDQ compare ok\n\n");


    return 0;
}


U32 eMMCTest_CMDQ_Random(U32 u32_DataPattern)
{
    U32 u32_err;
    U32 u32_i;
    U32 *pu32_W=(U32*)gau8_WBuf;
    U8 *pu8_W = gau8_WBuf;

    U16 u16_task_count;
    U32 u32_task_id;

    U16 u16_test_blk_cnt[32];
    U32 u32_test_blk_addr[32];

    U16 u16_avail_blk = 0xFE0;

    do {
        u16_task_count = rRand(0, 0) % 32;
    } while( u16_task_count == 0 );

    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        // length
        u16_test_blk_cnt[u32_i] = rRand(0, 0) % u16_avail_blk;
        if( ((u16_avail_blk - u16_test_blk_cnt[u32_i]) < 32) ||
            (u16_test_blk_cnt[u32_i] == 0) )
            u16_test_blk_cnt[u32_i] = 1;

        u16_avail_blk -= u16_test_blk_cnt[u32_i];

        // start block
        u32_test_blk_addr[u32_i] = 0xA00000+(rRand(0, 0) % g_eMMCDrv.u32_SEC_COUNT);

        if( (u32_test_blk_addr[u32_i] + u16_test_blk_cnt[u32_i]) >= g_eMMCDrv.u32_SEC_COUNT )
            u32_test_blk_addr[u32_i] = g_eMMCDrv.u32_SEC_COUNT - u16_test_blk_cnt[u32_i];
    }

    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "Data Pattern: %08Xh\n", u32_DataPattern);

    eMMC_debug(0, 1, "Preparing pattern, wait ...\n");

    // Prepare data buffer
    if(u32_DataPattern == 0x00112233 || (u32_DataPattern == 0x33221100))
    {
        for(u32_i = 0; u32_i < TEST_BUFFER_SIZE; u32_i++)
        {
            if(u32_DataPattern == 0x00112233)
            {
                gau8_WBuf[u32_i] = u32_i & 0xFF;
            }
            else
            {
                gau8_WBuf[u32_i] = 0xFF - (u32_i & 0xFF);
            }
        }
    }
    else
    {
        for(u32_i=0; u32_i<TEST_BUFFER_SIZE>>2; u32_i++)
        {
            pu32_W[u32_i] = u32_DataPattern;
        }
    }

    eMMC_debug(0, 1, "Preparing pattern, ok\n");

    memset(LinkDescW, 0, sizeof(struct _LinkDescriptor)*32);

    // Prepare link descriptor
    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        u32_task_id = u32_i;

        LinkDescW[u32_task_id].u32_Address  = eMMC_translate_DMA_address_Ex((U32)pu8_W, u16_test_blk_cnt[u32_i]<<9);
        //eMMC_debug(0, 1, "u32_Address=%p\n", LinkDescW[u32_task_id].u32_Address);
        LinkDescW[u32_task_id].u32_JobCnt   = u16_test_blk_cnt[u32_i];
        //eMMC_debug(0, 1, "u32_JobCnt=%p\n", LinkDescW[u32_task_id].u32_JobCnt);
        LinkDescW[u32_task_id].u32_Dir      = rRand(0, 0) % 2;    // write
        LinkDescW[u32_task_id].u32_Link     = 0;    // not ADMA mode
        LinkDescW[u32_task_id].u32_MiuSel   = 0;
        LinkDescW[u32_task_id].u32_Length   = (u16_test_blk_cnt[u32_i]<<9);
        //eMMC_debug(0, 1, "u32_Length=%p\n", LinkDescW[u32_task_id].u32_Length);
        LinkDescW[u32_task_id].u32_End      = 1;

        pu8_W += (u16_test_blk_cnt[u32_i]<<9);
    }

    #if 0
    eMMC_dump_mem((U8 *)LinkDescW, sizeof(struct _LinkDescriptor)*(u16_task_count));
    #endif

    u32_err = eMMC_CMDQ_Task(LinkDescW, u32_test_blk_addr, u16_task_count);
    if( u32_err )
    {
        eMMC_debug(0, 1, "CMDQ random test fail\n");
        while(1);
    }

    eMMC_debug(0, 1, "CMDQ random test ok\n");

    #if 0
    eMMC_FCIE_DumpRegisters();
    eMMC_FCIE_DumpDebugBus();
    #endif


    return 0;
}

U32 eMMCTest_CMDQ_Max_Task(U32 u32_DataPattern)
{
    U32 u32_err;
    U32 u32_i;
    U32 *pu32_W=(U32*)gau8_WBuf;
    U32 *pu32_R=(U32*)gau8_RBuf;
    U8 *pu8_W = gau8_WBuf;
    U8 *pu8_R = gau8_RBuf;
	U32 u32_temp;

    U16 u16_task_count = 31;
    U32 u32_task_id;

    U16 u16_test_blk_cnt[32];
    U32 u32_test_blk_addr[32];

    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        u16_test_blk_cnt[u32_i] = 0x40;
        u32_test_blk_addr[u32_i] = 0xA00000 + u32_i*0x40;
    }

    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "Data Pattern: %08Xh\n", u32_DataPattern);

    eMMC_debug(0, 1, "Preparing pattern, wait ...\n");

    // Prepare data buffer
    if(u32_DataPattern == 0x00112233 || (u32_DataPattern == 0x33221100))
    {
        for(u32_i = 0; u32_i < TEST_BUFFER_SIZE; u32_i++)
        {
            if(u32_DataPattern == 0x00112233)
            {
                gau8_WBuf[u32_i] = u32_i & 0xFF;
            }
            else
            {
                gau8_WBuf[u32_i] = 0xFF - (u32_i & 0xFF);
            }
            gau8_RBuf[u32_i] = ~gau8_WBuf[u32_i];
        }
    }
    else
    {
        for(u32_i=0; u32_i<TEST_BUFFER_SIZE>>2; u32_i++)
        {
            pu32_W[u32_i] = u32_DataPattern;
            pu32_R[u32_i] = ~pu32_W[u32_i];
        }
    }

    eMMC_debug(0, 1, "Preparing pattern, ok\n");

    memset(LinkDescW, 0, sizeof(struct _LinkDescriptor)*32);

    // Prepare link descriptor
    u32_temp =0;
    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        u32_task_id = u32_i;

        LinkDescW[u32_task_id].u32_Address  = eMMC_translate_DMA_address_Ex((U32)(pu8_W+u32_temp), u16_test_blk_cnt[u32_i]<<9);
        LinkDescW[u32_task_id].u32_JobCnt   = u16_test_blk_cnt[u32_i];
        LinkDescW[u32_task_id].u32_Dir      = 1;    // write
        LinkDescW[u32_task_id].u32_Link     = 0;    // not ADMA mode
        LinkDescW[u32_task_id].u32_MiuSel   = 0;
        LinkDescW[u32_task_id].u32_Length   = (u16_test_blk_cnt[u32_i]<<9);
        LinkDescW[u32_task_id].u32_End      = 1;

        u32_temp += (u16_test_blk_cnt[u32_i]*512);
    }

    #if 0
    eMMC_dump_mem((U8 *)LinkDescW, sizeof(struct _LinkDescriptor)*(u16_task_count));
    #endif

    u32_err = eMMC_CMDQ_Task(LinkDescW, u32_test_blk_addr, u16_task_count);
    if( u32_err )
    {
        eMMC_debug(0, 1, "CMDQ write test fail\n");
        while(1);
    }

    eMMC_debug(0, 1, "CMDQ write test ok\n");

    #if 0
    eMMC_FCIE_DumpRegisters();
    eMMC_FCIE_DumpDebugBus();
    #endif

    memset(LinkDescR, 0, sizeof(struct _LinkDescriptor)*32);

    // Prepare link descriptor
    u32_temp =0;
    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        u32_task_id = u32_i;

        LinkDescR[u32_task_id].u32_Address  = eMMC_translate_DMA_address_Ex((U32)(pu8_R+u32_temp), u16_test_blk_cnt[u32_i]<<9);
        LinkDescR[u32_task_id].u32_JobCnt   = u16_test_blk_cnt[u32_i];
        LinkDescR[u32_task_id].u32_Dir      = 0;    // read
        LinkDescR[u32_task_id].u32_Link     = 0;    // not ADMA mode
        LinkDescR[u32_task_id].u32_MiuSel   = 0;
        LinkDescR[u32_task_id].u32_Length   = (u16_test_blk_cnt[u32_i]<<9);
        LinkDescR[u32_task_id].u32_End      = 1;

        u32_temp += (u16_test_blk_cnt[u32_i]*512);
    }

    #if 0
    eMMC_dump_mem((U8 *)LinkDescR, sizeof(struct _LinkDescriptor)*(u16_task_count));
    #endif

    u32_err = eMMC_CMDQ_Task(LinkDescR, u32_test_blk_addr, u16_task_count);
    if( u32_err )
    {
        eMMC_debug(0, 1, "CMDQ read test fail\n");
        while(1);
    }

    eMMC_debug(0, 1, "CMDQ read test ok\n");


    eMMC_debug(0, 1, "Comparing data, wait...\n");
	u32_temp =0;
    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        u32_task_id = u32_i;

        u32_err = eMMC_ComapreData(pu8_W+u32_temp, pu8_R+u32_temp, LinkDescR[u32_task_id].u32_JobCnt<<eMMC_SECTOR_512BYTE_BITS);
		u32_temp += (LinkDescR[u32_task_id].u32_JobCnt*512);

        if(u32_err)
        {
            eMMC_debug(0, 1,"CMDQ compare taskid %d item fail\n", u32_task_id);

            eMMC_FCIE_DumpRegisters();
            eMMC_FCIE_DumpDebugBus();

            //eMMC_debug(0, 0, "W Buf:\n");
            //eMMC_dump_mem((U8*)LinkDescW[u32_task_id].u32_Address, 0x400 /*LinkDescR[u32_task_id].u32_JobCnt<<eMMC_SECTOR_512BYTE_BITS*/);
            //eMMC_debug(0, 0, "R Buf:\n");
            //eMMC_dump_mem((U8*)LinkDescW[u32_task_id].u32_Address, 0x400 /*LinkDescR[u32_task_id].u32_JobCnt<<eMMC_SECTOR_512BYTE_BITS*/);

            while(1);

            return u32_err;
        }
    }

    eMMC_debug(0, 1, "CMDQ compare ok\n\n");


    return 0;
}

struct _scatterlist pCMDQSG_st[32][MAX_SCATTERLIST_COUNT] __attribute__((aligned(32)));
struct _AdmaDescriptor p_CMDQWAdmaDesc_st[32][MAX_SCATTERLIST_COUNT] __attribute__((aligned(32)));
struct _AdmaDescriptor p_CMDQRAdmaDesc_st[32][MAX_SCATTERLIST_COUNT] __attribute__((aligned(32)));

U32 eMMCTest_CMDQ_ADMA(U32 u32_DataPattern)
{
    U32 u32_err;
    U32 u32_i;
    U32 u32_j;
    U32 *pu32_W=(U32*)gau8_WBuf;
    U32 *pu32_R=(U32*)gau8_RBuf;
    U8 *pu8_AW;
    U8 *pu8_AR;
    U32 u32_temp;
    U32 u32_sg_blocks;
    U32 u32_dmaaddr;
    U32 u32_dma_addr;
    U32 u32_dmalen;

    U16 u16_task_count = 4;
    U32 u32_task_id;

    U16 u16_test_blk_cnt[] = {0x100, 0x200, 0x300, 0x200};
    U32 u32_test_blk_addr[] = {0xA00000, 0xA00000+0x100, 0xA00000+0x300, 0xA00000+0x600};

    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "Data Pattern: %08Xh\n", u32_DataPattern);

    eMMC_debug(0, 1, "Preparing pattern, wait ...\n");

    // Prepare data buffer
    if(u32_DataPattern == 0x00112233 || (u32_DataPattern == 0x33221100))
    {
        for(u32_i = 0; u32_i < TEST_BUFFER_SIZE; u32_i++)
        {
            if(u32_DataPattern == 0x00112233)
            {
                gau8_WBuf[u32_i] = u32_i & 0xFF;
            }
            else
            {
                gau8_WBuf[u32_i] = 0xFF - (u32_i & 0xFF);
            }
            gau8_RBuf[u32_i] = ~gau8_WBuf[u32_i];
        }
    }
    else
    {
        for(u32_i=0; u32_i<TEST_BUFFER_SIZE>>2; u32_i++)
        {
            pu32_W[u32_i] = u32_DataPattern;
            pu32_R[u32_i] = ~pu32_W[u32_i];
        }
    }

    eMMC_debug(0, 1, "Preparing pattern, ok\n");

    // Prepare sglist

    //setting scatterlist dma address and dma length for write multiple block

    //eMMC_debug(0, 1, "Preparing write ADMA table\n");

    memset(pCMDQSG_st, 0, 32*MAX_SCATTERLIST_COUNT*sizeof(struct _scatterlist));
    memset(p_CMDQWAdmaDesc_st, 0, 32*MAX_SCATTERLIST_COUNT*sizeof(struct _AdmaDescriptor));

    pu8_AW = (U8*)pu32_W;

    u32_temp =0;
    
    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        //eMMC_debug(0, 1, "task id %d\n", u32_i);
        u32_sg_blocks = u16_test_blk_cnt[u32_i];
        //eMMC_debug(0, 1, "total job count = 0x%X\n", u16_test_blk_cnt[u32_i]);

        for(u32_j=0; u32_j<MAX_SCATTERLIST_COUNT; u32_j++)
        {
            //eMMC_debug(0, 1, "sglist item %d\n", u32_j);
            if( (u32_j % 4) < 3 )
            {
                pCMDQSG_st[u32_i][u32_j].u32_length = eMMC_SECTOR_512BYTE;
            }
            else
            {
                pCMDQSG_st[u32_i][u32_j].u32_length = (u32_sg_blocks/4-3)*eMMC_SECTOR_512BYTE;
            }

            pCMDQSG_st[u32_i][u32_j].u32_dma_address = ((U32)(pu8_AW+u32_temp));
            //eMMC_debug(0, 1, "pCMDQSG_st[%d][%d].u32_dma_address=0x%08X\n", u32_i, u32_j, pCMDQSG_st[u32_i][u32_j].u32_dma_address);

            u32_temp += pCMDQSG_st[u32_i][u32_j].u32_length;
            //eMMC_debug(0, 1, "job count = 0x%X\n", pCMDQSG_st[u32_i][u32_j].u32_length / 512);
            //eMMC_debug(0, 1, "pCMDQSG_st[%d][%d].u32_length=0x%08X\n", u32_i, u32_j, pCMDQSG_st[u32_i][u32_j].u32_length);
            //eMMC_debug(0, 1, "u32_temp=%08X\n", u32_temp);

            u32_dmaaddr = pCMDQSG_st[u32_i][u32_j].u32_dma_address;
            u32_dmalen  = pCMDQSG_st[u32_i][u32_j].u32_length;
            u32_dma_addr = eMMC_translate_DMA_address_Ex(u32_dmaaddr, u32_dmalen);

            p_CMDQWAdmaDesc_st[u32_i][u32_j].u32_Address = u32_dma_addr;
            p_CMDQWAdmaDesc_st[u32_i][u32_j].u32_DmaLen  = u32_dmalen;
            p_CMDQWAdmaDesc_st[u32_i][u32_j].u32_JobCnt  = u32_dmalen >> 9;
            p_CMDQWAdmaDesc_st[u32_i][u32_j].u32_MiuSel  = 0;		//FPGA Mode always miu0

        }

        p_CMDQWAdmaDesc_st[u32_i][u32_j-1].u32_End = 1;

	#if 0
        eMMC_debug(0, 0, "\nWrite ADMA table %d\n", u32_i);
        eMMC_dump_mem((U8*)(&(p_CMDQWAdmaDesc_st[u32_i][0])), MAX_SCATTERLIST_COUNT *  sizeof(struct _AdmaDescriptor));
	#endif
    }


    memset(LinkDescW, 0, sizeof(struct _LinkDescriptor)*32);

    // Prepare link descriptor
    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        u32_task_id = u32_i;

        LinkDescW[u32_task_id].u32_Address  = eMMC_translate_DMA_address_Ex((U32)(&p_CMDQWAdmaDesc_st[u32_i]), u16_test_blk_cnt[u32_i]<<9);
        LinkDescW[u32_task_id].u32_JobCnt   = u16_test_blk_cnt[u32_i];
        LinkDescW[u32_task_id].u32_Dir      = 1;    // write
        LinkDescW[u32_task_id].u32_Link     = 1;    // ADMA mode
        LinkDescW[u32_task_id].u32_MiuSel   = 0;
        LinkDescW[u32_task_id].u32_Length   = (u16_test_blk_cnt[u32_i]<<9);
        LinkDescW[u32_task_id].u32_End      = 0;
    }

    #if 0
    eMMC_debug(0, 0, "Write Link table\n");
    eMMC_dump_mem((U8 *)LinkDescW, sizeof(struct _LinkDescriptor)*(u16_task_count));
    #endif

    //eMMC_debug(0, 1, "Start write ADMA\n");

    u32_err = eMMC_CMDQ_Task(LinkDescW, u32_test_blk_addr, u16_task_count);
    if( u32_err )
    {
        eMMC_debug(0, 1, "CMDQ ADMA write test fail\n");
        while(1);
    }

    eMMC_debug(0, 1, "CMDQ ADMA write test ok\n");

    #if 0
    eMMC_FCIE_DumpRegisters();
    eMMC_FCIE_DumpDebugBus();
    #endif


    //eMMC_debug(0, 1, "Preparing read ADMA table\n");

    memset(pCMDQSG_st, 0, 32*MAX_SCATTERLIST_COUNT*sizeof(struct _scatterlist));
    memset(p_CMDQRAdmaDesc_st, 0, 32*MAX_SCATTERLIST_COUNT*sizeof(struct _AdmaDescriptor));

    pu8_AR = (U8*)pu32_R;


    u32_temp =0;
    
    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        //eMMC_debug(0, 1, "task id %d\n", u32_i);
        u32_sg_blocks = u16_test_blk_cnt[u32_i];
        //eMMC_debug(0, 1, "total job count = 0x%X\n", u16_test_blk_cnt[u32_i]);

        for(u32_j=0; u32_j<MAX_SCATTERLIST_COUNT; u32_j++)
        {
            //eMMC_debug(0, 1, "sglist item %d\n", u32_j);
            if( (u32_j % 4) < 3 )
            {
                pCMDQSG_st[u32_i][u32_j].u32_length = eMMC_SECTOR_512BYTE;
            }
            else
            {
                pCMDQSG_st[u32_i][u32_j].u32_length = (u32_sg_blocks/4-3)*eMMC_SECTOR_512BYTE;
            }

            pCMDQSG_st[u32_i][u32_j].u32_dma_address = ((U32)(pu8_AR +u32_temp));
            //eMMC_debug(0, 1, "pCMDQSG_st[%d][%d].u32_dma_address=0x%08X\n", u32_i, u32_j, pCMDQSG_st[u32_i][u32_j].u32_dma_address);

            u32_temp += pCMDQSG_st[u32_i][u32_j].u32_length;
            //eMMC_debug(0, 1, "job count = 0x%X\n", pCMDQSG_st[u32_i][u32_j].u32_length / 512);
            //eMMC_debug(0, 1, "pCMDQSG_st[%d][%d].u32_length=0x%08X\n", u32_i, u32_j, pCMDQSG_st[u32_i][u32_j].u32_length);
            //eMMC_debug(0, 1, "u32_temp=%08X\n", u32_temp);

            u32_dmaaddr = pCMDQSG_st[u32_i][u32_j].u32_dma_address;
            u32_dmalen  = pCMDQSG_st[u32_i][u32_j].u32_length;
            u32_dma_addr = eMMC_translate_DMA_address_Ex(u32_dmaaddr, u32_dmalen);

            p_CMDQRAdmaDesc_st[u32_i][u32_j].u32_Address = u32_dma_addr;
            p_CMDQRAdmaDesc_st[u32_i][u32_j].u32_DmaLen  = u32_dmalen;
            p_CMDQRAdmaDesc_st[u32_i][u32_j].u32_JobCnt  = u32_dmalen >> 9;
            p_CMDQRAdmaDesc_st[u32_i][u32_j].u32_MiuSel  = 0;		//FPGA Mode always miu0

        }

        p_CMDQRAdmaDesc_st[u32_i][u32_j-1].u32_End = 1;

	    #if 0
        eMMC_debug(0, 0, "\nRead ADMA table %d\n", u32_i);
        eMMC_dump_mem((U8*)(&(p_CMDQRAdmaDesc_st[u32_i][0])), MAX_SCATTERLIST_COUNT *  sizeof(struct _AdmaDescriptor));
	    #endif
    }


    memset(LinkDescR, 0, sizeof(struct _LinkDescriptor)*32);

    // Prepare link descriptor
    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        u32_task_id = u32_i;

        LinkDescR[u32_task_id].u32_Address  = eMMC_translate_DMA_address_Ex((U32)(&p_CMDQRAdmaDesc_st[u32_i]), u16_test_blk_cnt[u32_i]<<9);
        LinkDescR[u32_task_id].u32_JobCnt   = u16_test_blk_cnt[u32_i];
        LinkDescR[u32_task_id].u32_Dir      = 0;    // read
        LinkDescR[u32_task_id].u32_Link     = 1;    // ADMA mode
        LinkDescR[u32_task_id].u32_MiuSel   = 0;
        LinkDescR[u32_task_id].u32_Length   = (u16_test_blk_cnt[u32_i]<<9);
        LinkDescR[u32_task_id].u32_End      = 0;

        pu32_R += (u16_test_blk_cnt[u32_i]*512);
    }

    #if 0
    eMMC_debug(0, 0, "Read Link table\n");
    eMMC_dump_mem((U8 *)LinkDescR, sizeof(struct _LinkDescriptor)*(u16_task_count));
    #endif

    //eMMC_debug(0, 1, "Start read ADMA\n");

    u32_err = eMMC_CMDQ_Task(LinkDescR, u32_test_blk_addr, u16_task_count);
    if( u32_err )
    {
        eMMC_debug(0, 1, "CMDQ ADMA read test fail\n");
        while(1);
    }

    eMMC_debug(0, 1, "CMDQ ADMA read test ok\n");

    //eMMC_FCIE_DumpRegisters();
    //eMMC_FCIE_DumpDebugBus();

    eMMC_debug(0, 1, "Comparing data, wait...\n");
	u32_temp =0;

    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        u32_task_id = u32_i;

        u32_err = eMMC_ComapreData(gau8_WBuf+u32_temp, gau8_RBuf+u32_temp ,LinkDescR[u32_task_id].u32_JobCnt<<eMMC_SECTOR_512BYTE_BITS);
		u32_temp += LinkDescR[u32_task_id].u32_JobCnt;

        if(u32_err)
        {
            eMMC_debug(0, 1,"CMDQ compare taskid %d item fail\n", u32_task_id);

            eMMC_FCIE_DumpRegisters();
            eMMC_FCIE_DumpDebugBus();

            while(1);

            return u32_err;
        }
    }

    eMMC_debug(0, 1, "CMDQ ADMA compare ok\n\n");

    return 0;
}

U32 eMMCTest_CMDQ_Random_ADMA(U32 u32_DataPattern)
{
    U32 u32_err;
    U32 u32_i;
    U32 u32_j;
    U32 *pu32_W=(U32*)gau8_WBuf;
    U8 *pu8_AW;
    U32 u32_temp;
    U32 u32_sg_blocks;
    U32 u32_temp_blocks;
    U32 u32_dmaaddr;
    U32 u32_dma_addr;
    U32 u32_dmalen;

    U16 u16_task_count;
    U32 u32_task_id;

    U16 u16_test_blk_cnt[32];
    U32 u32_test_blk_addr[32];

    U16 u16_avail_blk = 0xFE0;

    memset(u16_test_blk_cnt, 0, 32*sizeof(U16));
    memset(u32_test_blk_addr, 0, 32*sizeof(U32));

    for(u32_i=0; u32_i<32; u32_i++)
    {
        // length
        u16_test_blk_cnt[u32_i] = rRand(0, 0) % u16_avail_blk;
        if( u16_test_blk_cnt[u32_i] == 0 )
            u16_test_blk_cnt[u32_i] = 1;

        u16_avail_blk -= u16_test_blk_cnt[u32_i];

        // start block
        u32_test_blk_addr[u32_i] = 0xA00000 + (rRand(0, 0) % g_eMMCDrv.u32_SEC_COUNT);

        if( (u32_test_blk_addr[u32_i] + u16_test_blk_cnt[u32_i]) >= g_eMMCDrv.u32_SEC_COUNT )
            u32_test_blk_addr[u32_i] = g_eMMCDrv.u32_SEC_COUNT - u16_test_blk_cnt[u32_i];

        if( u16_avail_blk == 0 )
            break;
    }

    u16_task_count = u32_i;

    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "Data Pattern: %08Xh\n", u32_DataPattern);

    eMMC_debug(0, 1, "Preparing pattern, wait ...\n");

    // Prepare data buffer
    if(u32_DataPattern == 0x00112233 || (u32_DataPattern == 0x33221100))
    {
        for(u32_i = 0; u32_i < TEST_BUFFER_SIZE; u32_i++)
        {
            if(u32_DataPattern == 0x00112233)
            {
                gau8_WBuf[u32_i] = u32_i & 0xFF;
            }
            else
            {
                gau8_WBuf[u32_i] = 0xFF - (u32_i & 0xFF);
            }
        }
    }
    else
    {
        for(u32_i=0; u32_i<TEST_BUFFER_SIZE>>2; u32_i++)
        {
            pu32_W[u32_i] = u32_DataPattern;
        }
    }

    eMMC_debug(0, 1, "Preparing pattern, ok\n");

    // Prepare sglist

    //setting scatterlist dma address and dma length for write multiple block

    pu8_AW = (U8*)pu32_W;
    u32_temp =0;

    memset(pCMDQSG_st, 0, 32*MAX_SCATTERLIST_COUNT*sizeof(struct _scatterlist));
    memset(p_CMDQWAdmaDesc_st, 0, 32*MAX_SCATTERLIST_COUNT*sizeof(struct _AdmaDescriptor));

    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        //eMMC_debug(0, 1, "task id %d\n", u32_i);
        u32_sg_blocks = u16_test_blk_cnt[u32_i];
        //eMMC_debug(0, 1, "total job count = 0x%X\n", u16_test_blk_cnt[u32_i]);

        for(u32_j=0; u32_j<MAX_SCATTERLIST_COUNT; u32_j++)
        {
            //eMMC_debug(0, 1, "sglist item %d\n", u32_j);
            if( u32_j == (MAX_SCATTERLIST_COUNT-1) )
            {
                pCMDQSG_st[u32_i][u32_j].u32_length = (u32_sg_blocks << 9);
                p_CMDQWAdmaDesc_st[u32_i][u32_j].u32_End = 1;
            }
            else
            {
                if( u32_sg_blocks > 1 )
                {
                    do
                    {
                        u32_temp_blocks = rRand(0, 0) % u32_sg_blocks;
                    } while( u32_temp_blocks == 0 );
                }
                else
                    u32_temp_blocks = 1;

                pCMDQSG_st[u32_i][u32_j].u32_length = (u32_temp_blocks << 9);

                u32_sg_blocks -= u32_temp_blocks;

                if( u32_sg_blocks == 0 )
                {
                    p_CMDQWAdmaDesc_st[u32_i][u32_j].u32_End = 1;
                }
            }

            pCMDQSG_st[u32_i][u32_j].u32_dma_address = ((U32)(pu8_AW+u32_temp));
            //eMMC_debug(0, 1, "pCMDQSG_st[%d][%d].u32_dma_address=0x%08X\n", u32_i, u32_j, pCMDQSG_st[u32_i][u32_j].u32_dma_address);

            u32_temp += pCMDQSG_st[u32_i][u32_j].u32_length;
            //eMMC_debug(0, 1, "job count = 0x%X\n", u32_temp_blocks);
            //eMMC_debug(0, 1, "pCMDQSG_st[%d][%d].u32_length=0x%08X\n", u32_i, u32_j, pCMDQSG_st[u32_i][u32_j].u32_length);
            //eMMC_debug(0, 1, "u32_temp=%08X\n", u32_temp);

            u32_dmaaddr = pCMDQSG_st[u32_i][u32_j].u32_dma_address;
            u32_dmalen  = pCMDQSG_st[u32_i][u32_j].u32_length;
            u32_dma_addr = eMMC_translate_DMA_address_Ex(u32_dmaaddr, u32_dmalen);

            p_CMDQWAdmaDesc_st[u32_i][u32_j].u32_Address = u32_dma_addr;
            p_CMDQWAdmaDesc_st[u32_i][u32_j].u32_DmaLen  = u32_dmalen;
            p_CMDQWAdmaDesc_st[u32_i][u32_j].u32_JobCnt  = u32_dmalen >> 9;
            p_CMDQWAdmaDesc_st[u32_i][u32_j].u32_MiuSel  = 0;		//FPGA Mode always miu0

            if( p_CMDQWAdmaDesc_st[u32_i][u32_j].u32_End == 1 )
                break;
        }

        #if 0
        eMMC_debug(0, 0, "\nTask %d ADMA table\n", u32_i);
        eMMC_dump_mem((U8*)(&p_CMDQWAdmaDesc_st[u32_i][0]), (u32_j+1) *  sizeof(struct _AdmaDescriptor));
        #endif
    }


    memset(LinkDescW, 0, sizeof(struct _LinkDescriptor)*32);

    // Prepare link descriptor
    for(u32_i=0; u32_i<u16_task_count; u32_i++)
    {
        u32_task_id = u32_i;

        LinkDescW[u32_task_id].u32_Address  = eMMC_translate_DMA_address_Ex((U32)(&p_CMDQWAdmaDesc_st[u32_i]), u16_test_blk_cnt[u32_i]<<9);
        LinkDescW[u32_task_id].u32_JobCnt   = u16_test_blk_cnt[u32_i];
        LinkDescW[u32_task_id].u32_Dir      = rRand(0, 0) % 2;
        LinkDescW[u32_task_id].u32_Link     = 1;    // ADMA mode
        LinkDescW[u32_task_id].u32_MiuSel   = 0;
        LinkDescW[u32_task_id].u32_Length   = (u16_test_blk_cnt[u32_i]<<9);
        LinkDescW[u32_task_id].u32_End      = 0;
    }

    #if 0
    eMMC_debug(0, 0, "Write Link table\n");
    eMMC_dump_mem((U8 *)LinkDescW, sizeof(struct _LinkDescriptor)*(u16_task_count));
    #endif

    u32_err = eMMC_CMDQ_Task(LinkDescW, u32_test_blk_addr, u16_task_count);
    if( u32_err )
    {
        eMMC_debug(0, 1, "CMDQ random ADMA test fail\n");
        while(1);
    }

    eMMC_debug(0, 1, "CMDQ random ADMAtest ok\n");

    #if 0
    eMMC_FCIE_DumpRegisters();
    eMMC_FCIE_DumpDebugBus();
    #endif


    return 0;
}

#endif

U32 eMMCTest_SingleBlkWRC_RIU(U32 u32_eMMC_Addr, U32 u32_DataPattern)
{
	U32 u32_err;
	U32 u32_i, *pu32_W=(U32*)gau8_WBuf, *pu32_R=(U32*)gau8_RBuf;

    if(u32_DataPattern == 0x00112233 || (u32_DataPattern == 0x33221100))
    {
        for(u32_i = 0; u32_i < TEST_BUFFER_SIZE; u32_i ++)
        {
            if(u32_DataPattern == 0x00112233)
            {
                gau8_WBuf[u32_i] = u32_i & 0xFF;
            }
            else
            {
                gau8_WBuf[u32_i] = 0xFF - (u32_i & 0xFF);
            }
            gau8_RBuf[u32_i] = ~gau8_WBuf[u32_i];
        }
    }
    else
    {
        for(u32_i=0; u32_i<TEST_BUFFER_SIZE>>2; u32_i++)
        {
            pu32_W[u32_i] = u32_DataPattern;
            pu32_R[u32_i] = ~pu32_W[u32_i];
        }
    }

	u32_err = eMMC_CMD24_CIFD(u32_eMMC_Addr, gau8_WBuf);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(0,1,"Err, SingleBlkWRC W, %Xh \n", u32_err);
		return u32_err;
	}

	u32_err = eMMC_CMD17_CIFD(u32_eMMC_Addr, gau8_RBuf);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(0,1,"Err, SingleBlkWRC R, %Xh \n", u32_err);
		return u32_err;
	}

	u32_err = eMMC_ComapreData(gau8_WBuf, gau8_RBuf, FCIE_CIFD_BYTE_CNT);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(0,1,"Err, SingleBlkWRC C, %Xh \n", u32_err);
		return u32_err;
	}

	return u32_err;
}

extern U32 gu32_DMAAddr;
U32 eMMCTest_SingleBlkWRC_MIU(U32 u32_eMMC_Addr, U32 u32_DataPattern)
{
	volatile U32 u32_i, *pu32_W=(U32*)gau8_WBuf, *pu32_R=(U32*)gau8_RBuf;

    if(u32_DataPattern == 0x00112233 || (u32_DataPattern == 0x33221100))
    {
        for(u32_i = 0; u32_i < TEST_BUFFER_SIZE; u32_i ++)
        {
            if(u32_DataPattern == 0x00112233)
            {
                gau8_WBuf[u32_i] = u32_i & 0xFF;
            }
            else
            {
                gau8_WBuf[u32_i] = 0xFF - (u32_i & 0xFF);
            }
            gau8_RBuf[u32_i] = ~gau8_WBuf[u32_i];
        }
    }
    else
    {
        for(u32_i=0; u32_i<TEST_BUFFER_SIZE>>2; u32_i++)
        {
            pu32_W[u32_i] = u32_DataPattern;
            pu32_R[u32_i] = ~pu32_W[u32_i];
        }
    }

	return eMMCTest_SingleBlkWRC_MIU_Ex(u32_eMMC_Addr, (U8*)pu32_W, (U8*)pu32_R);
}

U32 eMMCTest_SingleBlkWRC_MIU_Ex(U32 u32_eMMC_Addr, U8 *pu8_W, U8 *pu8_R)
{
	U32 u32_err;

	u32_err = eMMC_CMD24_MIU(u32_eMMC_Addr, pu8_W);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(0,1,"Err, SingleBlkWRC W, %Xh \n", u32_err);
		return u32_err;
	}

	u32_err = eMMC_CMD17_MIU(u32_eMMC_Addr, pu8_R);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(0,1,"Err, SingleBlkWRC R, %Xh \n", u32_err);
		return u32_err;
	}

	u32_err = eMMC_ComapreData(pu8_W, pu8_R, eMMC_SECTOR_BYTECNT);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(0,1,"Err, SingleBlkWRC C, %Xh \n", u32_err);
		return u32_err;
	}

	return u32_err;
}

#define eMMC_RETRY_CNT  10

#ifndef IP_FCIE_VERSION_5
U32  eMMCTest_SgW_MIU(U32 u32_eMMC_Addr)
{
	U16 u16_mode, u16_ctrl=0, u16_reg;
    U32 u32_dmaaddr,u32_dma_addr,u32_dmalen,u32_arg;
	U32 u32_err,u32_err_12=eMMC_ST_SUCCESS,u32_i;
	U8  u8_retry_cnt=0;

	LABEL_RETRY:
	eMMC_FCIE_ClearEvents();
	// -------------------------------
	// send cmd
    u16_ctrl = (BIT_SD_CMD_EN | BIT_SD_RSP_EN);
    u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;

    // -------------------------------

    u32_arg =  u32_eMMC_Addr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);

    u32_err = eMMC_FCIE_SendCmd(
    	u16_mode, u16_ctrl, u32_arg, 25, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
		if(u8_retry_cnt++ < eMMC_RETRY_CNT)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: CMD fail: %Xh \n", u32_err);
			eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
			goto LABEL_RETRY;
		}
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD fail: %Xh \n", u32_err);
		eMMC_FCIE_ErrHandler_Stop();
    	goto LABEL_END;
    }

	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		if(u8_retry_cnt++ < eMMC_RETRY_CNT)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: R1 fail: %Xh \n", u32_err);
			eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
			goto LABEL_RETRY;
		}
		u32_err = eMMC_ST_ERR_CMD25_CHK_R1;
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
		    "eMMC Err: R1 fail: %Xh\n", u32_err);
		eMMC_FCIE_ErrHandler_Stop();
		goto LABEL_END;
	}

    if(g_eMMCDrv.u32_DrvFlag & (DRV_FLAG_DDR_MODE|DRV_FLAG_SPEED_HS200) )
	{
		REG_FCIE_W(FCIE_TOGGLE_CNT, (g_eMMCDrv.u32_DrvFlag&DRV_FLAG_SPEED_HS200) ? TOGGLE_CNT_512_CLK_W : TOGGLE_CNT_256_CLK_W);
		REG_FCIE_SETBIT(FCIE_MACRO_REDNT, BIT_MACRO_DIR);
	}

    for(u32_i=0; u32_i< MAX_SCATTERLIST_COUNT; u32_i++)
    {
        u32_dmaaddr = pSG_st[u32_i].u32_dma_address;
		u32_dmalen  = pSG_st[u32_i].u32_length;
		u32_dma_addr = eMMC_translate_DMA_address_Ex(u32_dmaaddr, u32_dmalen);

        eMMC_FCIE_ClearEvents_Reg0();
		u32_err = eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH);
		if(eMMC_ST_SUCCESS != u32_err)
		{
			if(u8_retry_cnt++ < eMMC_RETRY_CNT)
		    {
			    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: Wait D0 H TO: %Xh \n", u32_err);
			    eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
			    goto LABEL_RETRY;
		    }
			u32_err = eMMC_ST_ERR_CMD25_WAIT_D0H;
			goto LABEL_END;
		}

        REG_FCIE_W(FCIE_JOB_BL_CNT, u32_dmalen>>eMMC_SECTOR_512BYTE_BITS);
        REG_FCIE_W(FCIE_SDIO_ADDR0, u32_dma_addr & 0xFFFF);
        REG_FCIE_W(FCIE_SDIO_ADDR1, u32_dma_addr >> 16);

        REG_FCIE_SETBIT(FCIE_MMA_PRI_REG, BIT_DMA_DIR_W);
        if(u32_i ==0)
        {
      	    u32_err = eMMC_FCIE_FifoClkRdy(BIT_DMA_DIR_W);

            if(eMMC_ST_SUCCESS != u32_err)
            {
				if(u8_retry_cnt++ < eMMC_RETRY_CNT)
		        {
			        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: FifoClkRdy fail: %Xh \n", u32_err);
			        eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
			        goto LABEL_RETRY;
		        }
      	        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: FifoClkRdy fail\n");
      	        eMMC_FCIE_ErrHandler_Stop();
      	        goto LABEL_END;
            }
        }
        REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);
        REG_FCIE_W(FCIE_SD_CTRL, BIT_SD_DAT_EN|BIT_SD_DAT_DIR_W);
        #if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
        REG_FCIE_W(FCIE_MIE_INT_EN, BIT_CARD_DMA_END);
	    #endif
        //u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_CARD_DMA_END|BIT_MIU_LAST_DONE, eMMC_GENERIC_WAIT_TIME);
        u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_CARD_DMA_END, eMMC_GENERIC_WAIT_TIME);
		if(u32_err!= eMMC_ST_SUCCESS)
        {
			if(u8_retry_cnt++ < eMMC_RETRY_CNT)
		    {
			    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: event TO: %Xh \n", u32_err);
			    eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
			    goto LABEL_RETRY;
		    }
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: event TO \n");
			//eMMC_die("\n");
			g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_ERROR_RETRY;
			break;
        }
		//eMMC_hw_timer_delay(1000000);
    }

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if((u16_reg & BIT_SD_FCIE_ERR_FLAGS)||(g_eMMCDrv.u32_DrvFlag & DRV_FLAG_ERROR_RETRY))
	{
		if(u8_retry_cnt++ < eMMC_RETRY_CNT)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: Reg.12h: %Xh fail: %Xh \n", u16_reg, u32_err);
			eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
			goto LABEL_RETRY;
		}
		u32_err = eMMC_ST_ERR_CMD25;
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 0,
		    "eMMC Err: Reg.12h: %04Xh, Err: %Xh\n", u16_reg, u32_err);
		eMMC_FCIE_ErrHandler_Stop();
		goto LABEL_END;
	}

	LABEL_END:
	u32_err_12 = eMMC_CMD12(g_eMMCDrv.u16_RCA);
	if(eMMC_ST_SUCCESS != u32_err_12 && u8_retry_cnt++ < eMMC_RETRY_CNT)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: CMD12 fail: %Xh \n", u32_err);
		eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
		goto LABEL_RETRY;
	}

	REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
	return u32_err ? u32_err : u32_err_12;
}

U32  eMMCTest_SgR_MIU(U32 u32_eMMC_Addr)
{
	U16 u16_mode, u16_ctrl=0, u16_reg;
    U32 u32_dmaaddr,u32_dma_addr,u32_dmalen,u32_arg;
	U32 u32_err,u32_err_12=eMMC_ST_SUCCESS,u32_i;
	U8  u8_retry_cnt=0;

    LABEL_RETRY:
    eMMC_FCIE_ClearEvents();

	u16_ctrl = BIT_SD_DAT_EN;
    u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;
	u16_mode |= BIT_SD_DMA_R_CLK_STOP;

	REG_FCIE_W(FCIE_SD_MODE, u16_mode);

	if(g_eMMCDrv.u32_DrvFlag & (DRV_FLAG_DDR_MODE|DRV_FLAG_SPEED_HS200) )
	{
		REG_FCIE_W(FCIE_TOGGLE_CNT, (g_eMMCDrv.u32_DrvFlag&DRV_FLAG_SPEED_HS200) ? TOGGLE_CNT_512_CLK_R : TOGGLE_CNT_256_CLK_R);
		REG_FCIE_SETBIT(FCIE_MACRO_REDNT, BIT_TOGGLE_CNT_RST);
		REG_FCIE_CLRBIT(FCIE_MACRO_REDNT, BIT_MACRO_DIR);
		eMMC_hw_timer_delay(TIME_WAIT_FCIE_RST_TOGGLE_CNT); // Brian needs 2T
		REG_FCIE_CLRBIT(FCIE_MACRO_REDNT, BIT_TOGGLE_CNT_RST);
	}

	u32_dmaaddr = pSG_st[0].u32_dma_address;
	u32_dmalen	= pSG_st[0].u32_length;
	u32_dma_addr = eMMC_translate_DMA_address_Ex(u32_dmaaddr, u32_dmalen);

	REG_FCIE_W(FCIE_JOB_BL_CNT, u32_dmalen>>eMMC_SECTOR_512BYTE_BITS);
	REG_FCIE_W(FCIE_SDIO_ADDR0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_SDIO_ADDR1, u32_dma_addr >> 16);

	REG_FCIE_CLRBIT(FCIE_MMA_PRI_REG, BIT_DMA_DIR_W);

	u32_err = eMMC_FCIE_FifoClkRdy(0);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		if(u8_retry_cnt++ < eMMC_RETRY_CNT)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: FifoClkRdy fail: %Xh \n", u32_err);
			eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
			goto LABEL_RETRY;
		}
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: FifoClkRdy fail\n");
		eMMC_FCIE_ErrHandler_Stop();
		goto LABEL_END;
	}
	REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);

	// -------------------------------
	// send cmd
    u16_ctrl |= (BIT_SD_CMD_EN | BIT_SD_RSP_EN);

    // -------------------------------
    u32_arg =  u32_eMMC_Addr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);

    u32_err = eMMC_FCIE_SendCmd(
    	u16_mode, u16_ctrl, u32_arg, 18, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
		if(u8_retry_cnt++ < eMMC_RETRY_CNT)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: CMD fail: %Xh \n", u32_err);
			eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
			goto LABEL_RETRY;
		}
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 0, "eMMC Err: CMD fail %Xh \n", u32_err);
		eMMC_FCIE_ErrHandler_Stop();
		goto LABEL_END;
    }
    else
    {
		#if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
        REG_FCIE_W(FCIE_MIE_INT_EN, BIT_MIU_LAST_DONE);
        #endif

		u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT,
			BIT_MIU_LAST_DONE, eMMC_GENERIC_WAIT_TIME);
		if(u32_err!= eMMC_ST_SUCCESS)
        {
			if(u8_retry_cnt++ < eMMC_RETRY_CNT)
		    {
		        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: event TO.0: %Xh \n", u32_err);
		        eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
		        goto LABEL_RETRY;
		    }
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: event TO.0 \n");
   		    //eMMC_die("\n");
   		    g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_ERROR_RETRY;
   		    return u32_err;
        }
		REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
		if((eMMC_ST_SUCCESS != u32_err) || (u16_reg & BIT_SD_FCIE_ERR_FLAGS))
		{
			if(u8_retry_cnt++ < eMMC_RETRY_CNT)
		    {
			    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: Reg.12h: %Xh fail: %Xh \n", u16_reg, u32_err);
			    eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
			    goto LABEL_RETRY;
		    }
			u32_err = eMMC_ST_ERR_CMD18;
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 0,
				"eMMC Err: Reg.12h: %Xh fail: %Xh\n", u16_reg, u32_err);
			eMMC_FCIE_ErrHandler_Stop();
			goto LABEL_END;
		}

        for(u32_i=1; u32_i< MAX_SCATTERLIST_COUNT; u32_i++)
        {
            eMMC_FCIE_ClearEvents_Reg0();

            u32_dmaaddr = pSG_st[u32_i].u32_dma_address;
	        u32_dmalen  = pSG_st[u32_i].u32_length;
			u32_dma_addr = eMMC_translate_DMA_address_Ex(u32_dmaaddr, u32_dmalen);

            REG_FCIE_W(FCIE_JOB_BL_CNT, u32_dmalen>>9);
            REG_FCIE_W(FCIE_SDIO_ADDR0, u32_dma_addr & 0xFFFF);
            REG_FCIE_W(FCIE_SDIO_ADDR1, u32_dma_addr >> 16);
            REG_FCIE_SETBIT(FCIE_PATH_CTRL, BIT_MMA_EN);
            REG_FCIE_W(FCIE_SD_CTRL, BIT_SD_DAT_EN);
            #if defined(ENABLE_eMMC_INTERRUPT_MODE) && ENABLE_eMMC_INTERRUPT_MODE
            REG_FCIE_W(FCIE_MIE_INT_EN, BIT_MIU_LAST_DONE);
            #endif
            u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_MIU_LAST_DONE, TIME_WAIT_n_BLK_END*(1+(u32_dmalen>>24)));
    		if(u32_err!= eMMC_ST_SUCCESS)
            {
				if(u8_retry_cnt++ < eMMC_RETRY_CNT)
		        {
			        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: event TO.1: %Xh \n", u32_err);
			        eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
			        goto LABEL_RETRY;
		        }
                eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"eMMC Err: event TO.1 \n");
   			    //eMMC_die("\n");
   			    g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_ERROR_RETRY;
   			    break;
            }
        }
    }

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if((u16_reg & BIT_SD_FCIE_ERR_FLAGS)||(g_eMMCDrv.u32_DrvFlag & DRV_FLAG_ERROR_RETRY))
	{
		if(u8_retry_cnt++ < eMMC_RETRY_CNT)
		{
		    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: Reg.12h: %Xh fail: %Xh \n", u16_reg, u32_err);
		    eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
		    goto LABEL_RETRY;
		}
		u32_err = eMMC_ST_ERR_CMD18;
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
			"eMMC Err: Reg.12h: %Xh fail: %Xh\n", u16_reg, u32_err);
		eMMC_FCIE_ErrHandler_Stop();
		goto LABEL_END;
	}

	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		if(u8_retry_cnt++ < eMMC_RETRY_CNT)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: R1 fail: %Xh \n", u32_err);
			eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
			goto LABEL_RETRY;
		}
		u32_err = eMMC_ST_ERR_CMD18;
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
	        "eMMC Err: R1 fail: %Xh\n", u32_err);
	    eMMC_FCIE_ErrHandler_Stop();
		goto LABEL_END;
	}

	LABEL_END:
	if(g_eMMCDrv.u32_DrvFlag & DRV_FLAG_DDR_MODE)
		REG_FCIE_SETBIT(FCIE_MACRO_REDNT, BIT_MACRO_DIR);

	u32_err_12 = eMMC_CMD12(g_eMMCDrv.u16_RCA);
	if(eMMC_ST_SUCCESS != u32_err_12 && u8_retry_cnt++ < eMMC_RETRY_CNT)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL, 1, "eMMC Warn: CMD12 fail: %Xh \n", u32_err);
		eMMC_FCIE_ErrHandler_ReInit();  eMMC_FCIE_ErrHandler_Retry();
		goto LABEL_RETRY;
	}

	REG_FCIE_CLRBIT(FCIE_SD_MODE, BIT_SD_CLK_EN);
	return u32_err ? u32_err : u32_err_12;
}
#endif

U32  eMMCTest_SgWRC_MIU(U32 u32_eMMC_Addr, U16 u16_BlkCnt, U32 u32_DataPattern)
{
    U32 u32_err;
    U32 u32_i, u32_j, *pu32_W=(U32*)gau8_WBuf, *pu32_R=(U32*)gau8_RBuf;
    U32 u32_temp, u32_sg_blocks;
    #ifdef IP_FCIE_VERSION_5
    U32 u32_dmaaddr,u32_dma_addr,u32_dmalen;
    #endif

    if(u32_DataPattern == 0x00112233 || (u32_DataPattern == 0x33221100))
    {
        for(u32_i = 0; u32_i < TEST_BUFFER_SIZE; u32_i ++)
        {
            if(u32_DataPattern == 0x00112233)
            {
                gau8_WBuf[u32_i] = u32_i & 0xFF;
            }
            else
            {
                gau8_WBuf[u32_i] = 0xFF - (u32_i & 0xFF);
            }
            gau8_RBuf[u32_i] = ~gau8_WBuf[u32_i];
        }
    }
    else
    {
        for(u32_i=0; u32_i<TEST_BUFFER_SIZE>>2; u32_i++)
        {
            pu32_W[u32_i] = u32_DataPattern;
            pu32_R[u32_i] = ~pu32_W[u32_i];
        }
    }

    u32_sg_blocks = u16_BlkCnt/MAX_SCATTERLIST_COUNT;
    u32_temp =0;
    memset(pSG_st, 0, MAX_SCATTERLIST_COUNT *  sizeof(struct _scatterlist));

    //setting scatterlist dma address and dma length for write multiple block
    for(u32_i=0; u32_i< MAX_SCATTERLIST_COUNT/4; u32_i++)
    {
        for(u32_j=0; u32_j< 4; u32_j++) // 4 groups
        {
            pSG_st[u32_i*4+u32_j].u32_dma_address = (U32)(gau8_WBuf+ u32_temp);
            if(u32_j < 3) // first 3 have 1 block
            {
                pSG_st[u32_i*4+u32_j].u32_length = eMMC_SECTOR_512BYTE;
                u32_temp += eMMC_SECTOR_512BYTE;
            }
            else
            {
                pSG_st[u32_i*4+u32_j].u32_length = (4*u32_sg_blocks-3)<<eMMC_SECTOR_512BYTE_BITS;
                u32_temp += (4*u32_sg_blocks-3)<<eMMC_SECTOR_512BYTE_BITS;
            }
        }
    }

    #ifdef IP_FCIE_VERSION_5
    memset(p_AdmaDesc_st, 0, MAX_SCATTERLIST_COUNT *  sizeof(struct _AdmaDescriptor));

    for(u32_i=0; u32_i< MAX_SCATTERLIST_COUNT; u32_i++)
    {
        if( pSG_st[u32_i].u32_dma_address && pSG_st[u32_i].u32_length )
        {
            u32_dmaaddr = pSG_st[u32_i].u32_dma_address;
            u32_dmalen  = pSG_st[u32_i].u32_length;

            u32_dma_addr = eMMC_translate_DMA_address_Ex(u32_dmaaddr, u32_dmalen);

            //parsing dma address, length to descriptor
            p_AdmaDesc_st[u32_i].u32_Address =(u32_dma_addr);
            p_AdmaDesc_st[u32_i].u32_DmaLen = (u32_dmalen);
            p_AdmaDesc_st[u32_i].u32_JobCnt = (u32_dmalen >> 9);
            p_AdmaDesc_st[u32_i].u32_MiuSel = 0;		//FPGA Mode always miu0
        }
        else
            break;
    }

    if( u32_i > 0 )
        p_AdmaDesc_st[u32_i-1].u32_End = 1;
    else
        p_AdmaDesc_st[0].u32_End = 1;

    //eMMC_dump_mem((U8*)p_AdmaDesc_st, u32_i * sizeof(struct _AdmaDescriptor));
    #endif

    #ifndef IP_FCIE_VERSION_5
    //write scatterlist
    u32_err = eMMCTest_SgW_MIU(u32_eMMC_Addr);
    #else
    u32_err = eMMC_CMD25_ADMA(u32_eMMC_Addr, (U8 *)p_AdmaDesc_st, sizeof(struct _AdmaDescriptor)*u32_i);
    #endif

    if(u32_err != eMMC_ST_SUCCESS)
    {
        eMMC_debug(0,1,"Err, SgWRC_MIU W, %Xh \n", u32_err);
        return u32_err;
    }

    //eMMC_debug(1, 0, "===================================================\n");

    u32_temp =0;
    memset(pSG_st, 0, MAX_SCATTERLIST_COUNT *  sizeof(struct _scatterlist));

    //setting scatterlist dma address and dma length for read multiple block
    for(u32_i=0; u32_i< MAX_SCATTERLIST_COUNT/4; u32_i++)
    {
        for(u32_j=0; u32_j< 4; u32_j++) // 4 groups
        {
            pSG_st[u32_i*4+u32_j].u32_dma_address = (U32)(gau8_RBuf+ u32_temp);
            if(u32_j < 3) // first 3 have 1 block
            {
                pSG_st[u32_i*4+u32_j].u32_length = eMMC_SECTOR_512BYTE;
                u32_temp += eMMC_SECTOR_512BYTE;
            }
            else
            {
                pSG_st[u32_i*4+u32_j].u32_length = (4*u32_sg_blocks-3)<<eMMC_SECTOR_512BYTE_BITS;
                u32_temp += (4*u32_sg_blocks-3)<<eMMC_SECTOR_512BYTE_BITS;
            }
        }
    }

    #ifdef IP_FCIE_VERSION_5
    memset(p_AdmaDesc_st, 0, MAX_SCATTERLIST_COUNT *  sizeof(struct _AdmaDescriptor));

    u32_i=0;
    for(u32_i=0; u32_i< MAX_SCATTERLIST_COUNT; u32_i++)
    {
        if( pSG_st[u32_i].u32_dma_address && pSG_st[u32_i].u32_length )
        {
            u32_dmaaddr = pSG_st[u32_i].u32_dma_address;
            u32_dmalen  = pSG_st[u32_i].u32_length;

            u32_dma_addr = eMMC_translate_DMA_address_Ex(u32_dmaaddr, u32_dmalen);

            //parsing dma address, length to descriptor
            p_AdmaDesc_st[u32_i].u32_Address =(u32_dma_addr);
            p_AdmaDesc_st[u32_i].u32_DmaLen = (u32_dmalen);
            p_AdmaDesc_st[u32_i].u32_JobCnt = (u32_dmalen >> 9);
            p_AdmaDesc_st[u32_i].u32_MiuSel  = 0;
        }
        else
            break;
    }

    if( u32_i > 0 )
        p_AdmaDesc_st[u32_i-1].u32_End = 1;
    else
        p_AdmaDesc_st[0].u32_End = 1;

    //eMMC_dump_mem((U8*)p_AdmaDesc_st, u32_i *  sizeof(struct _AdmaDescriptor));
    #endif

    #ifndef IP_FCIE_VERSION_5
    //read scatterlist
    u32_err = eMMCTest_SgR_MIU(u32_eMMC_Addr);
    #else
    u32_err = eMMC_CMD18_ADMA(u32_eMMC_Addr, (U8 *)p_AdmaDesc_st, sizeof(struct _AdmaDescriptor)*u32_i);
    #ifndef NEED_POST_INVALIDATE_CACHE
    flush_cache((U32)gau8_RBuf, TEST_BUFFER_SIZE);
    #endif
    #endif
    if(u32_err != eMMC_ST_SUCCESS)
    {
        eMMC_debug(0,1,"Err, SgWRC_MIU R, %Xh \n", u32_err);
        return u32_err;
    }

    #if defined(NEED_POST_INVALIDATE_CACHE) && NEED_POST_INVALIDATE_CACHE
    for(u32_i=0; u32_i< MAX_SCATTERLIST_COUNT; u32_i++)
    {
        if( pSG_st[u32_i].u32_dma_address && pSG_st[u32_i].u32_length )
        {
            u32_dmaaddr = pSG_st[u32_i].u32_dma_address;
            u32_dmalen  = pSG_st[u32_i].u32_length;
            eMMC_Invalidate_data_cache_buffer(u32_dmaaddr, u32_dmalen);
        }
        else
            break;
    }
    #endif

    u32_err = eMMC_ComapreData(
                  gau8_WBuf, gau8_RBuf, u16_BlkCnt<<eMMC_SECTOR_512BYTE_BITS);

    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(0,1,"Err, SgWRC_MIU C, %u, %Xh \n", u32_i, u32_err);
        return u32_err;
    }
    return eMMC_ST_SUCCESS;
}

U32 eMMCTest_MultiBlkWRC_MIU(U32 u32_eMMC_Addr, U16 u16_BlkCnt, U32 u32_DataPattern)
{
	U32 u32_err=0;
	U32 u32_i, *pu32_W=(U32*)gau8_WBuf, *pu32_R=(U32*)gau8_RBuf;
    U16 u16_BlkCnt_tmp;

    // init data pattern
	for(u32_i=0; u32_i<TEST_BUFFER_SIZE>>2; u32_i++)
	{
        pu32_W[u32_i] = u32_DataPattern;
		pu32_R[u32_i] = ~pu32_W[u32_i];
	}

	u16_BlkCnt_tmp = TEST_BUFFER_SIZE >> eMMC_SECTOR_512BYTE_BITS;
	u16_BlkCnt_tmp = u16_BlkCnt_tmp > u16_BlkCnt ? u16_BlkCnt : u16_BlkCnt_tmp;

	for(u32_i=0; u32_i<u16_BlkCnt_tmp; )
	{
		u32_err = eMMC_CMD25_MIU(
			u32_eMMC_Addr+u32_i, gau8_WBuf+(u32_i<<eMMC_SECTOR_512BYTE_BITS), u16_BlkCnt_tmp-u32_i);
	    if(eMMC_ST_SUCCESS != u32_err)
	    {
		    eMMC_debug(0,1,"Err, MultiBlkWRC W, %u, %Xh \n", u32_i, u32_err);
		    goto LABEL_OF_ERROR;
	    }

		u32_err = eMMC_CMD18_MIU(
			u32_eMMC_Addr+u32_i, gau8_RBuf+(u32_i<<eMMC_SECTOR_512BYTE_BITS), u16_BlkCnt_tmp-u32_i);
	    if(eMMC_ST_SUCCESS != u32_err)
	    {
		    eMMC_debug(0,1,"Err, MultiBlkWRC R, %u, %Xh \n", u32_i, u32_err);
		    goto LABEL_OF_ERROR;
	    }

	    u32_err = eMMC_ComapreData(
			gau8_WBuf+(u32_i<<eMMC_SECTOR_512BYTE_BITS), gau8_RBuf+(u32_i<<eMMC_SECTOR_512BYTE_BITS), (u16_BlkCnt_tmp-u32_i)<<eMMC_SECTOR_512BYTE_BITS);
	    if(eMMC_ST_SUCCESS != u32_err)
    	{
    		eMMC_debug(0,1,"Err, MultiBlkWRC C, %u, %Xh \n", u32_i, u32_err);
    		goto LABEL_OF_ERROR;
	    }

		u32_i += u16_BlkCnt_tmp-u32_i;
	}

	return u32_err;

	LABEL_OF_ERROR:
	return u32_err;
}

U32 eMMCTest_MultiBlkWRC_MIU_Large(U32 u32_eMMC_Addr, U32 u32_DataPattern)
{
    U32 u32_StartSector=0, u32_SectorCnt=0, u32_err = 0;
    U32 u32_i, *pu32_W=(U32*)DMA_W_ADDR, *pu32_R=(U32*)DMA_R_ADDR, u32_ByteCnt;
    U8 *pu8_W=(U8*)DMA_W_ADDR, *pu8_R=(U8*)DMA_R_ADDR;
    u32_StartSector = u32_eMMC_Addr;

    u32_SectorCnt = 0xFFFF;         //max block count for register

    u32_ByteCnt = (u32_SectorCnt<<9);

    if((U32)pu8_R>(U32)pu8_W)
    {
        if(((U32)pu8_R - (U32)pu8_W) < (u32_SectorCnt <<9))
        {
            eMMC_debug(0,1,"Err, Please Reconfigure your Write/Read buffer address W: %X, R: %Xh \n", (U32)pu8_W,(U32) pu8_R);
            goto LABEL_IP_VERIFY_ERROR;
        }
    }
    else
    {
        if(((U32)pu8_W - (U32)pu8_R) < (u32_SectorCnt <<9))
        {
            eMMC_debug(0,1,"Err, Please Reconfigure your Write/Read buffer address W: %X, R: %Xh \n", (U32)pu8_W,(U32) pu8_R);
            goto LABEL_IP_VERIFY_ERROR;
        }
    }


    // init data pattern
    for(u32_i=0; u32_i<u32_ByteCnt>>2; u32_i++)
    {
            pu32_W[u32_i] = u32_DataPattern;
            pu32_R[u32_i] = ~pu32_W[u32_i];
    }

    for(u32_i=0; u32_i<u32_SectorCnt; )
    {
        u32_err = eMMC_CMD25_MIU(
            u32_StartSector+u32_i, pu8_W+(u32_i<<eMMC_SECTOR_512BYTE_BITS), u32_SectorCnt-u32_i);
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(0,1,"Err, MultiBlkWRC W, %u, %Xh \n", u32_i, u32_err);
            goto LABEL_IP_VERIFY_ERROR;
        }

        u32_err = eMMC_CMD18_MIU(
            u32_StartSector+u32_i, pu8_R+(u32_i<<eMMC_SECTOR_512BYTE_BITS), u32_SectorCnt-u32_i);
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(0,1,"Err, MultiBlkWRC R, %u, %Xh \n", u32_i, u32_err);
            goto LABEL_IP_VERIFY_ERROR;
        }

        u32_err = eMMC_ComapreData(
            pu8_W+(u32_i<<eMMC_SECTOR_512BYTE_BITS), pu8_R+(u32_i<<eMMC_SECTOR_512BYTE_BITS), (u32_SectorCnt-u32_i)<<eMMC_SECTOR_512BYTE_BITS);
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(0,1,"Err, MultiBlkWRC C, %u, %Xh \n", u32_i, u32_err);
            goto LABEL_IP_VERIFY_ERROR;
        }

        u32_i += u32_SectorCnt-u32_i;
    }

    // ===============================================
    //eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n");
    //eMMC_debug(eMMC_DEBUG_LEVEL, 1, "[OK] \n\n");
    return eMMC_ST_SUCCESS;

    LABEL_IP_VERIFY_ERROR:
    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n\n");
    eMMC_debug(eMMC_DEBUG_LEVEL,1,"Total Sec: %Xh, Test: StartSec: %Xh, SecCnt: %Xh \n",
            g_eMMCDrv.u32_SEC_COUNT, u32_StartSector, u32_SectorCnt);
    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "fail: %Xh \n\n", u32_err);
    eMMC_DumpDriverStatus();
    while(1);
    return u32_err;
}

// =========================================================
U32 eMMC_BootMode(U32 *pu32_Addr, U32 u32_ByteCnt)
{
    #ifndef IP_FCIE_VERSION_5
    *pu32_Addr=0;
    u32_ByteCnt=0;

    return eMMC_ST_SUCCESS;
    #else
    U32 u32_err=eMMC_ST_SUCCESS;
    U32 u32_boot_partition_size,u32_ByteCnt_Hash0,u32_ByteCnt_Hash1,u32_ByteCnt_Hash2;
    U32 u32_eMMC_ChkSum_rom = 0,u32_eMMC_ChkSum_tmp = 0;
    u32_ByteCnt = (u32_ByteCnt & eMMC_SECTOR_512BYTE_MASK)? (u32_ByteCnt+eMMC_SECTOR_512BYTE) : u32_ByteCnt;
    u32_boot_partition_size = g_eMMCDrv.u8_BOOT_SIZE_MULT*128*1024;
    if(u32_ByteCnt > u32_boot_partition_size)
    {
        eMMC_debug(0,1,"ByteCnt:%xh overflow eMMC boot partition size:%xh !!\n",u32_ByteCnt, u32_boot_partition_size);
        goto LABEL_Boot_Mode_END;
    }
    else if(u32_ByteCnt < (3*eMMC_SECTOR_512BYTE))
    {
        eMMC_debug(0,1,"ByteCnt:%xh less than 3 block!!\n",u32_ByteCnt);
        goto LABEL_Boot_Mode_END;

    }

    u32_ByteCnt_Hash0 = u32_ByteCnt_Hash1 = ((u32_ByteCnt/eMMC_SECTOR_512BYTE)/3)*eMMC_SECTOR_512BYTE;
    u32_ByteCnt_Hash2 = u32_ByteCnt - u32_ByteCnt_Hash0 - u32_ByteCnt_Hash1;
    eMMC_hw_timer_start();


    u32_err = eMMC_LoadImages(pu32_Addr, u32_ByteCnt_Hash0,0);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_Boot_Mode_END;

    u32_err = eMMC_LoadImages(pu32_Addr+(u32_ByteCnt_Hash0>>2), u32_ByteCnt_Hash1,1);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_Boot_Mode_END;

    u32_err = eMMC_LoadImages(pu32_Addr+((u32_ByteCnt_Hash0+u32_ByteCnt_Hash1)>>2), u32_ByteCnt_Hash2,2);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_Boot_Mode_END;

    u32_eMMC_ChkSum_rom = eMMC_ChkSum((U8*)pu32_Addr,u32_ByteCnt);
    // ----------------------------------------
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();
    if(eMMC_ST_SUCCESS != u32_err)
        goto  LABEL_Boot_Mode_END;

    u32_err = eMMC_Init_Device_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
        goto  LABEL_Boot_Mode_END;

    u32_err = eMMC_FCIE_ChooseSpeedMode();
    if(eMMC_ST_SUCCESS != u32_err)
        goto  LABEL_Boot_Mode_END;

    u32_err = eMMC_ReadBootPart((U8*)pu32_Addr,u32_ByteCnt,0,1);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_Boot_Mode_END;

    u32_eMMC_ChkSum_tmp = eMMC_ChkSum((U8*)pu32_Addr,u32_ByteCnt);

    if(u32_eMMC_ChkSum_rom != u32_eMMC_ChkSum_tmp)
    {
        eMMC_debug(0,0,"eMMC Rom boot checksum:%xh misatch read boot partition checksum:%xh !!\n",u32_eMMC_ChkSum_rom,u32_eMMC_ChkSum_tmp);
        goto LABEL_Boot_Mode_END;
    }
    eMMC_debug(0,0,"eMMC Rom boot verify pass!!\n");
    return   eMMC_ST_SUCCESS;
    LABEL_Boot_Mode_END:
    eMMC_FCIE_ErrHandler_Stop();
	return u32_err;
    #endif
}

void eMMCTest_Disable_Uart(void)
{
    volatile u32 *ptr;

    printf("Disable UART\n");

    ptr = (u32 *)(RIU_BASE+ (0x0E12<<1));
    *ptr &= ~0x1800;

    while(1);
}

//========================================================
// exposed function
//========================================================
#define eMMC_SPEED_TEST_COUNTDOWN          3

#define eMMC_SPEED_TEST_SINGLE_BLKCNT_W    500
#define eMMC_SPEED_TEST_MULTIPLE_BLKCNT_W  ((U32)30*(1024*1024/512)) // total 30MB

#define eMMC_SPEED_TEST_R_MULTI            10
#define eMMC_SPEED_TEST_SINGLE_BLKCNT_R    ((U32)eMMC_SPEED_TEST_SINGLE_BLKCNT_W*eMMC_SPEED_TEST_R_MULTI)
#define eMMC_SPEED_TEST_MULTIPLE_BLKCNT_R  ((U32)eMMC_SPEED_TEST_MULTIPLE_BLKCNT_W*eMMC_SPEED_TEST_R_MULTI)

void eMMCTest_DownCount(U32 u32_Sec) // verify the HW Timer
{
	U32 u32_i, u32_t0, u32_t1;//, au32_tmp[6];

	//eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"2013 0125 x\n");
	for(u32_i=0; u32_i<u32_Sec; u32_i++)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"%u \n", u32_Sec-u32_i);
		#if 0
		eMMC_hw_timer_start();
		au32_tmp[u32_i*2] = eMMC_hw_timer_tick();
		eMMC_hw_timer_delay(HW_TIMER_DELAY_1s);
		au32_tmp[u32_i*2+1] = eMMC_hw_timer_tick();
		#else
		eMMC_hw_timer_start();
        u32_t0 = eMMC_hw_timer_tick();
		u32_t1 = eMMC_hw_timer_tick();
		if(u32_t0 > u32_t1)
			while(0==(u32_t0-eMMC_hw_timer_tick())/eMMC_HW_TIMER_HZ)
				;
		else
			while(0==(eMMC_hw_timer_tick()-u32_t0)/eMMC_HW_TIMER_HZ)
				;
		#endif
	}
	eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,".\n");
	//eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"%u %u   %u %u   %u %u \n",
	//	au32_tmp[0], au32_tmp[1], au32_tmp[2], au32_tmp[3], au32_tmp[4], au32_tmp[5]);
}

U32 eMMC_IPVerify_Performance(void)
{
	volatile U32 u32_err, u32_i, u32_SectorAddr=0, u32_SectorCnt=0, u32_t0, u32_t1, u32_t;
	U8 u8_LoopCnt=0;

    if( !gau8_WBuf )
        gau8_WBuf = (U8*)memalign(eMMC_CACHE_LINE, TEST_BUFFER_SIZE);

    if( !gau8_RBuf )
        gau8_RBuf = (U8*)memalign(eMMC_CACHE_LINE, TEST_BUFFER_SIZE);

	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,1,"\n");
	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,1,"SectorBuf:%Xh, RBuf:%Xh, WBuf:%Xh\n",
		(U32)gau8_eMMC_SectorBuf, (U32)gau8_RBuf, (U32)gau8_WBuf);

	// ===============================================
	eMMC_PlatformInit();
	u32_err = eMMC_FCIE_Init();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "Err, eMMC_FCIE_Init fail: %Xh \n", u32_err);
		goto LABEL_PERF_END;
	}
	u32_err = eMMC_Init_Device_Ex();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "Err, eMMC_Init_Device_Ex fail: %Xh \n", u32_err);
		goto LABEL_PERF_END;
	}

	eMMCTest_DownCount(eMMC_SPEED_TEST_COUNTDOWN);
	for(u32_i=0; u32_i<TEST_BUFFER_SIZE; u32_i++)
		gau8_WBuf[u32_i] = (U8)u32_i;

	// ===============================================
	u32_err = eMMC_FCIE_EnableSDRMode();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: EnableSDRMode fail: %Xh\n", u32_err);
	    eMMC_DumpDriverStatus();
	    goto LABEL_PERF_END;
	}
	eMMC_clock_setting(gau8_FCIEClkSel[0]);
	eMMC_debug(0, 0, "\n\nSDR, clk: %u.%u MHz\n",
	g_eMMCDrv.u32_ClkKHz/1000, (g_eMMCDrv.u32_ClkKHz/100)%10);

	// ===============================================
	LABEL_TEST_START:
	eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"Single Write, total: %u KB, %u blocks, burst size: %Xh\n",
		(eMMC_SPEED_TEST_SINGLE_BLKCNT_W<<eMMC_SECTOR_512BYTE_BITS)>>10,
		eMMC_SPEED_TEST_SINGLE_BLKCNT_W, 1<<eMMC_SECTOR_512BYTE_BITS);
	eMMC_hw_timer_start();
	u32_t0 = eMMC_hw_timer_tick();
	for(u32_i=0; u32_i<eMMC_SPEED_TEST_SINGLE_BLKCNT_W; u32_i++)
	{
		u32_SectorAddr += (g_eMMCDrv.u32_SEC_COUNT>>2) + 333*u32_i;
		u32_SectorAddr %= g_eMMCDrv.u32_SEC_COUNT;
		if(u32_SectorAddr < g_eMMCDrv.u32_SEC_COUNT>>1)
			u32_SectorAddr += g_eMMCDrv.u32_SEC_COUNT>>1;
		u32_SectorAddr -= eMMC_SPEED_TEST_SINGLE_BLKCNT_W;
		u32_SectorAddr = eMMC_TEST_BLK_0+(u32_SectorAddr%eMMC_TEST_BLK_CNT);

		//eMMC_debug(0,0,"%08Xh \n", u32_SectorAddr+u32_i);
		#if eMMC_FEATURE_RELIABLE_WRITE
		u32_err = eMMC_CMD24(u32_SectorAddr, gau8_WBuf);
		#else
		u32_err = eMMC_CMD24_MIU(u32_SectorAddr, gau8_WBuf);
		#endif
		if(eMMC_ST_SUCCESS != u32_err)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh \n", u32_err);
			goto LABEL_PERF_END;
		}
	}
	eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH);
	u32_t1 = eMMC_hw_timer_tick();
	u32_t = u32_t1 > u32_t0 ? u32_t1-u32_t0 : 0xFFFFFFFF-u32_t0+u32_t1;
	u32_t = u32_t/(eMMC_HW_TIMER_HZ/1000); // ms
	if(0!=u32_t)
		eMMC_debug(0,0," %u KB/s \n",
		    ((eMMC_SPEED_TEST_SINGLE_BLKCNT_W<<eMMC_SECTOR_512BYTE_BITS)*1000/u32_t)>>10);
	else
		eMMC_debug(0,0,"t=0 \n");

	// -----------------------------------------
	eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"Single Read, total: %u KB, %u blocks, burst size: %Xh\n",
		((eMMC_SPEED_TEST_SINGLE_BLKCNT_R)<<eMMC_SECTOR_512BYTE_BITS)>>10,
		eMMC_SPEED_TEST_SINGLE_BLKCNT_R, 1<<eMMC_SECTOR_512BYTE_BITS);
	eMMC_hw_timer_start();
	u32_t0 = eMMC_hw_timer_tick();
	for(u32_i=0; u32_i<eMMC_SPEED_TEST_SINGLE_BLKCNT_R; u32_i++)
	{
		u32_SectorAddr += (g_eMMCDrv.u32_SEC_COUNT>>2) + 333*u32_i;
		u32_SectorAddr %= g_eMMCDrv.u32_SEC_COUNT;
		if(u32_SectorAddr < g_eMMCDrv.u32_SEC_COUNT>>1)
			u32_SectorAddr += g_eMMCDrv.u32_SEC_COUNT>>1;
		u32_SectorAddr -= eMMC_SPEED_TEST_SINGLE_BLKCNT_R;
		u32_SectorAddr = eMMC_TEST_BLK_0+(u32_SectorAddr%eMMC_TEST_BLK_CNT);

		u32_err = eMMC_CMD17_MIU(u32_SectorAddr, gau8_RBuf);
		if(eMMC_ST_SUCCESS != u32_err)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh \n", u32_err);
			goto LABEL_PERF_END;
		}
	}
	eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH);
	u32_t1 = eMMC_hw_timer_tick();
	u32_t = u32_t1 > u32_t0 ? u32_t1-u32_t0 : 0xFFFFFFFF-u32_t0+u32_t1;
	u32_t = u32_t/(eMMC_HW_TIMER_HZ/1000); // ms
	if(0!=u32_t)
		eMMC_debug(0,0," %u KB/s \n",
		    ((eMMC_SPEED_TEST_SINGLE_BLKCNT_R<<eMMC_SECTOR_512BYTE_BITS)*1000/u32_t)>>10);
	else
		eMMC_debug(0,0,"t=0 \n");

	// ==================================
	u32_SectorCnt = 0;
	LABEL_MULTI:
	eMMC_debug(0,0,"------------------------------\n");
	switch(u32_SectorCnt)
	{
		case 0:
			u32_SectorCnt = 0x400*4 >> eMMC_SECTOR_512BYTE_BITS; //4KB
		    break;
		case 0x400*4 >> eMMC_SECTOR_512BYTE_BITS:
			u32_SectorCnt = 0x400*16 >> eMMC_SECTOR_512BYTE_BITS; //16KB
		    break;
		case 0x400*16 >> eMMC_SECTOR_512BYTE_BITS:
			u32_SectorCnt = 0x400*64 >> eMMC_SECTOR_512BYTE_BITS; //64KB
		    break;
		case 0x400*64 >> eMMC_SECTOR_512BYTE_BITS:
			u32_SectorCnt = 0x400*1024 >> eMMC_SECTOR_512BYTE_BITS; //1MB
		    break;
	}
	eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"Multi Write, total: %u MB, burst size: %u KB\n",
		(eMMC_SPEED_TEST_MULTIPLE_BLKCNT_W<<eMMC_SECTOR_512BYTE_BITS)>>20, u32_SectorCnt>>1);

	u32_SectorAddr = eMMC_TEST_BLK_0;
	eMMC_hw_timer_start();
	u32_t0 = eMMC_hw_timer_tick();
	for(u32_i=0; u32_i<eMMC_SPEED_TEST_MULTIPLE_BLKCNT_W; )
	{
		u32_err = eMMC_CMD25_MIU(u32_SectorAddr, gau8_WBuf, u32_SectorCnt);
		if(eMMC_ST_SUCCESS != u32_err)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh \n", u32_err);
			goto LABEL_PERF_END;
		}
		u32_i += u32_SectorCnt;
	}
	eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH);
	u32_t1 = eMMC_hw_timer_tick();
	u32_t = u32_t1 > u32_t0 ? u32_t1-u32_t0 : 0xFFFFFFFF-u32_t0+u32_t1;
	u32_t = u32_t/(eMMC_HW_TIMER_HZ/100); // 10ms
	if(0!=u32_t)
		eMMC_debug(0,0," %u.%u MB/s \n",
		    ((eMMC_SPEED_TEST_MULTIPLE_BLKCNT_W<<eMMC_SECTOR_512BYTE_BITS)*100/u32_t)>>20,
		    (((eMMC_SPEED_TEST_MULTIPLE_BLKCNT_W<<eMMC_SECTOR_512BYTE_BITS)*100/u32_t)>>10)*10/1024%10);
	else
		eMMC_debug(0,0,"t=0 \n");

	// -----------------------------------------
	eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"Multi Read, total: %uMB, burst size: %u KB \n",
		(eMMC_SPEED_TEST_MULTIPLE_BLKCNT_R<<eMMC_SECTOR_512BYTE_BITS)>>20, u32_SectorCnt>>1);

	u32_SectorAddr = eMMC_TEST_BLK_0;
	eMMC_hw_timer_start();
	u32_t0 = eMMC_hw_timer_tick();
	for(u32_i=0; u32_i<eMMC_SPEED_TEST_MULTIPLE_BLKCNT_R;)
	{
		u32_err = eMMC_CMD18_MIU(u32_SectorAddr, gau8_RBuf, u32_SectorCnt);
		if(eMMC_ST_SUCCESS != u32_err)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: %Xh \n", u32_err);
			goto LABEL_PERF_END;
		}
		u32_i += u32_SectorCnt;
	}
	eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH);
	u32_t1 = eMMC_hw_timer_tick();
	u32_t = u32_t1 > u32_t0 ? u32_t1-u32_t0 : 0xFFFFFFFF-u32_t0+u32_t1;
	u32_t = u32_t/(eMMC_HW_TIMER_HZ/10); // 100ms
	if(0!=u32_t)
        eMMC_debug(0,0," %u.%u MB/s \n",
		    ((eMMC_SPEED_TEST_MULTIPLE_BLKCNT_R<<eMMC_SECTOR_512BYTE_BITS)*10/u32_t)>>20,
		    (((eMMC_SPEED_TEST_MULTIPLE_BLKCNT_R<<eMMC_SECTOR_512BYTE_BITS)*10/u32_t)>>10)*10/1024%10);
	else
		eMMC_debug(0,0,"t=0 \n");

	if(u32_SectorCnt != 0x400*1024 >> eMMC_SECTOR_512BYTE_BITS)
		goto LABEL_MULTI;

	// ===============================================
	if(0==u8_LoopCnt)
	{
		u8_LoopCnt++;

		eMMC_debug(0,0,"\n\n");
		// test for next mode
	    u32_err = eMMC_FCIE_ChooseSpeedMode();
        if(eMMC_ST_SUCCESS != u32_err)
		{
       	    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: ChooseSpeedMode fail: %Xh\n", u32_err);
       	    goto LABEL_PERF_END;
		}

		goto LABEL_TEST_START;
	}

    LABEL_PERF_END:

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

	return u32_err;
}

U8 u8_BusWidth[3] = { 1, 4, 8};

U32 u32_test_pattern[] = {
    0x00112233,
    0x33221100,
    0x00000000,
    0xFFFFFFFF,
    0xFF00FF00,
    0x00FF00FF,
    0x55555555,
    0xAAAAAAAA,
    0xAA55AA55,
    0x55AA55AA,
    0xA55AA55A,
    0x5AA55AA5
};

U8 u8_sdr_crc[12][16] =
{
    {0x53, 0x39, 0x96, 0x41, 0x31, 0x63, 0x0B, 0x06, 0x15, 0x70, 0x34, 0xEB, 0x5D, 0xB4, 0xCF, 0x7E},
    {0x53, 0x39, 0x96, 0xBE, 0xCE, 0x63, 0xF4, 0xF9, 0x15, 0x8F, 0x34, 0xEB, 0xA2, 0x4B, 0xCF, 0x81},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF},
    {0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00},
    {0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xFF},
    {0x00, 0x00, 0x00, 0x55, 0x55, 0x00, 0x55, 0x55, 0x00, 0x55, 0x00, 0x00, 0x55, 0x55, 0x00, 0x55},
    {0x00, 0x00, 0x00, 0xAA, 0xAA, 0x00, 0xAA, 0xAA, 0x00, 0xAA, 0x00, 0x00, 0xAA, 0xAA, 0x00, 0xAA},
    {0xFF, 0xFF, 0x00, 0xAA, 0xAA, 0xFF, 0xAA, 0x55, 0xFF, 0x55, 0x00, 0xFF, 0xAA, 0x55, 0xFF, 0x55},
    {0xFF, 0xFF, 0x00, 0x55, 0x55, 0xFF, 0x55, 0xAA, 0xFF, 0xAA, 0x00, 0xFF, 0x55, 0xAA, 0xFF, 0xAA},
    {0xFF, 0xFF, 0x00, 0xA5, 0xA5, 0xFF, 0xA5, 0x5A, 0xFF, 0x5A, 0x00, 0xFF, 0xA5, 0x5A, 0xFF, 0x5A},
    {0xFF, 0xFF, 0x00, 0x5A, 0x5A, 0xFF, 0x5A, 0xA5, 0xFF, 0xA5, 0x00, 0xFF, 0x5A, 0xA5, 0xFF, 0xA5}
};

#ifdef IP_FCIE_VERSION_5
#define FCIE_BYPASS_VERIFY              1
#define FCIE_BYPASS_RIU_VERIFY          1
#define FCIE_GPIO_SDR_VERIFY            0
#define FCIE_SDR_VERIFY                 1
#define FCIE_DDR_VERIFY                 1
#define FCIE_HS200_VERIFY               1
#define FCIE_HS400_VERIFY               1
#define FCIE_BIST_TEST                  0
#define FCIE_POWER_SAVING_MODE_TEST     0
#define FCIE_RSP_SHIFT_TUNING_TEST      0
#define FCIE_WCRC_SHIFT_TUNING_TEST     0
#define FCIE_RSTOP_SHIFT_TUNING_TEST    0
#define FCIE_READ_CRC_ERROR_INT_TEST    0
#define FCIE_WRITE_CRC_ERROR_INT_TEST   0
#define FCIE_WRITE_TIMEOUT_INT_TEST     0
#define FCIE_READ_TIMEOUT_INT_TEST      0
#define FCIE_CMD_NO_RESPONSE_INT_TEST   0
#define FCIE_CMD_RSP_CRC_ERROR_INT_TEST 0
#define FCIE_R3_R4_RESPONSE_TEST        0
#define FCIE_CHECK_RESET_PIN            0
#define FCIE_LAST_BLOCK_CRC_TEST        0
#define FCIE_CMDQ_VERIFY                0

static U32 eMMC_Init_For_Test(void)
{
    U32 u32_err;

    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;

    // init platform & FCIE
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();
    if(u32_err)
        goto  LABEL_INIT_END;

    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_INIT_DONE;

    // init eMMC device
    u32_err = eMMC_Identify();
    if(u32_err)
        return u32_err;

    eMMC_clock_setting(FCIE_SLOW_CLK);

    // determine device parameters, from CSD
    u32_err = eMMC_CSD_Config();
    if(eMMC_ST_SUCCESS != u32_err)
        goto  LABEL_INIT_END;

    // setup eMMC device
    // CMD7
    u32_err = eMMC_CMD3_CMD7(g_eMMCDrv.u16_RCA, 7);
    if(eMMC_ST_SUCCESS != u32_err)
        goto  LABEL_INIT_END;

    // determine device parameters, from Ext_CSD
    u32_err = eMMC_ExtCSD_Config();
    if(eMMC_ST_SUCCESS != u32_err)
        goto  LABEL_INIT_END;

    LABEL_INIT_END:
    g_eMMCDrv.u32_DrvFlag |= DRV_FLAG_INIT_DONE;

    return u32_err;
}

U32 eMMCTest_BIST(U8 u8_PatSel)
{
    U32 u32_err = eMMC_ST_SUCCESS;

    //eMMC_debug(0, 1, " mode %d\n", u8_PatSel);

    // Select pattern
    REG_FCIE_SETBIT(GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x36), (u8_PatSel & 0xF)<<12);

    // Set BIST Mode
    REG_FCIE_SETBIT(FCIE_TEST_MODE, BIT_BIST_MODE);

    // Do Block Write (CMD24)
    eMMC_CMD24(0, gau8_WBuf);

    // Do Block Read (CMD17)
    eMMC_CMD17(0, gau8_RBuf);

    // Wait BIST Error Flag
    if( (REG_FCIE(GET_REG_ADDR(FCIE_REG_BASE_ADDR, 0x36)) & BIT0) == BIT0 )
    {
        printf("\033[7;31m%s: BIST pattern type %d fail!!\033[m\n", __FUNCTION__, u8_PatSel);
        while(1);
        //return eMMC_ST_ERR_BIST_FAIL;
    }

    printf("%s: BIST pattern type %d ok!!\n", __FUNCTION__, u8_PatSel);

    REG_FCIE_CLRBIT(FCIE_TEST_MODE, BIT_BIST_MODE);

    return u32_err;
}

U32 eMMCTest_Power_Saving_Mode(void)
{
    U32 u32_err = 0;

    // Enable power save test by RIU mode
    REG_FCIE_W(FCIE_PWR_SAVE_CTL, 0x000D);

    /* (1) Clear HW Enable */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x00), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x01),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x07);

    /* (2) Clear All Interrupt */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x02), 0xffff);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x03),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x00);

    /* (3) Clear SD MODE Enable */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x04), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x05),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0B);

    /* (4) Clear SD CTL Enable */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x06), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x07),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0C);

    /* (5) Reset Start */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x08), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x09),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x3F);

    /* (6) Reset End */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x0A), 0x0001);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x0B),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x3F);

    /* (7) Set "SD_MOD" */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x0C), 0x0021);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x0D),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0B);

    /* (8) Enable "reg_sd_en" */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x0E), 0x0001);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x0F),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x07);

    /* (9) Command Content, IDLE */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x10), 0x0040);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x11),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x20);

    /* (10) Command Content, STOP */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x12), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x13),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x21);

    /* (11) Command Content, STOP */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x14), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x15),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x22);

    /* (12) Command & Response Size */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x16), 0x0500);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x17),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0A);

    /* (13) Enable Interrupt, SD_CMD_END */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x18), 0x0002);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x19),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x01);

    /* (14) Command Enable + job Start */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x1A), 0x0044);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x1B),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0C);

    /* (15) Wait Interrupt */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x1C), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x1D),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WINT);

    /* (16) STOP */
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x1E), 0x0000);
    REG_FCIE_W(GET_REG_ADDR(FCIE_POWEER_SAVE_MODE_BASE, 0x1F),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_STOP);

    eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);

    // Trigger power save mode
    REG_FCIE_W(FCIE_PWR_SAVE_CTL, 0x000F);

    u32_err = eMMC_FCIE_WaitEvents(FCIE_PWR_SAVE_CTL, BIT_POWER_SAVE_MODE_INT|BIT_RIU_SAVE_EVENT, TIME_WAIT_CMDRSP_END);
    if( u32_err )
    {
        eMMC_debug(0, 1, "Power saving mode fail\n");
        while(1);
    }

    eMMC_debug(0, 1, "Power saving mode ok\n");

    return 0;
}


U32 eMMCTest_RSP_Shift_Tuning_Ex(U32 u32_eMMC_Addr, U8 *pu8_W, U8 *pu8_R)
{
	U32 u32_err;
    U8  u8_i;
    U8  u8_RSP_test_result[16] = {0};

	u32_err = eMMC_CMD24_MIU(u32_eMMC_Addr, pu8_W);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(0,1,"Err, SingleBlkWRC W, %Xh \n", u32_err);
		return eMMC_ST_ERR_CMD24_MIU;
	}

    eMMC_DISABLE_RETRY(1);

    for(u8_i=0; u8_i<16; u8_i++)
    {
        REG_FCIE_CLRBIT(FCIE_RSP_SHIFT_CNT, BIT_RSP_SHIFT_TUNE_MASK);
        REG_FCIE_SETBIT(FCIE_RSP_SHIFT_CNT, BIT_RSP_SHIFT_SEL|(u8_i&0xF));

        memset(pu8_R, 0, 512);

        eMMC_DISABLE_LOG(1);
	    u32_err = eMMC_CMD17_MIU(u32_eMMC_Addr, pu8_R);
        eMMC_DISABLE_LOG(0);
	    if(eMMC_ST_SUCCESS != u32_err)
	    {
		    eMMC_debug(0,1,"RSP shift tuning \033[7;35m%d is not ok\033[m\n", u8_i);
            u8_RSP_test_result[u8_i] = 0;
		    continue;
	    }

	    u32_err = eMMC_ComapreData(pu8_W, pu8_R, eMMC_SECTOR_BYTECNT);
	    if(eMMC_ST_SUCCESS != u32_err)
	    {
		    eMMC_debug(0,1,"Err, SingleBlkWRC C, %Xh \n", u32_err);
            return eMMC_ST_ERR_DATA_MISMATCH;
	    }

        eMMC_debug(0,1,"RSP shift tuning \033[7;34m%d is ok\033[m\n", u8_i);
        u8_RSP_test_result[u8_i] = 1;
    }

    REG_FCIE_CLRBIT(FCIE_RSP_SHIFT_CNT, BIT_RSP_SHIFT_SEL);

    eMMC_DISABLE_RETRY(0);

    for(u8_i=0; u8_i<16; u8_i++)
    {
        if( u8_RSP_test_result[u8_i] )
            return eMMC_ST_SUCCESS;
    }

    return u32_err;
}

U32 eMMCTest_RSP_Shift_Tuning(U32 u32_eMMC_Addr, U32 u32_DataPattern)
{
	volatile U32 u32_i, *pu32_W=(U32*)gau8_WBuf, *pu32_R=(U32*)gau8_RBuf;

    eMMC_debug(0, 1, "Data pattern %08X\n", u32_DataPattern);

	// init data pattern
	if(u32_DataPattern == 0x00112233 || (u32_DataPattern == 0x33221100))
	{
		for(u32_i = 0; u32_i < TEST_BUFFER_SIZE; u32_i ++)
		{
			if(u32_DataPattern == 0x00112233)
			{
				gau8_WBuf[u32_i] = u32_i & 0xFF;
			}
			else
			{
				gau8_WBuf[u32_i] = 0xFF - (u32_i & 0xFF);
			}
			gau8_RBuf[u32_i] = ~gau8_WBuf[u32_i];
		}
	}
	else
	{
		for(u32_i=0; u32_i<TEST_BUFFER_SIZE>>2; u32_i++)
		{
			pu32_W[u32_i] = u32_DataPattern;
			pu32_R[u32_i] = ~pu32_W[u32_i];
		}
	}

	return eMMCTest_RSP_Shift_Tuning_Ex(u32_eMMC_Addr, (U8*)pu32_W, (U8*)pu32_R);
}

U32 eMMCTest_WCRC_Shift_Tuning_Ex(U32 u32_eMMC_Addr, U8 *pu8_W, U8 *pu8_R)
{
	U32 u32_err;
    U8  u8_i;
    U8  u8_WCRC_test_result[16] = {0};

    for(u8_i=0; u8_i<16; u8_i++)
    {
        eMMC_DISABLE_RETRY(1);

        //eMMC_debug(0, 0, "\033[7;35m1 FCIE_RX_SHIFT_CNT=%04X\033[m\n", REG_FCIE(FCIE_RX_SHIFT_CNT));
        REG_FCIE_CLRBIT(FCIE_RX_SHIFT_CNT, BIT_WRSTS_SHIFT_TUNE_MASK);
        REG_FCIE_SETBIT(FCIE_RX_SHIFT_CNT, BIT_WRSTS_SHIFT_SEL|((u8_i&0xF)<<8));
        //eMMC_debug(0, 0, "\033[7;36m2 FCIE_RX_SHIFT_CNT=%04X\033[m\n", REG_FCIE(FCIE_RX_SHIFT_CNT));

        eMMC_DISABLE_LOG(1);
        u32_err = eMMC_CMD24_MIU(u32_eMMC_Addr, pu8_W);
        eMMC_DISABLE_LOG(0);
	    if(eMMC_ST_SUCCESS != u32_err)
	    {
		    eMMC_debug(0,1,"WCRC shift tuning \033[7;35m%d is not ok\033[m\n", u8_i);
		    continue;
	    }

        eMMC_DISABLE_RETRY(0);

        memset(pu8_R, 0, 512);

	    u32_err = eMMC_CMD17_MIU(u32_eMMC_Addr, pu8_R);
	    if(eMMC_ST_SUCCESS != u32_err)
	    {
            eMMC_debug(0,1,"Err, SingleBlkWRC R, %Xh \n", u32_err);
		    while(1);
	    }

	    u32_err = eMMC_ComapreData(pu8_W, pu8_R, eMMC_SECTOR_BYTECNT);
	    if(eMMC_ST_SUCCESS != u32_err)
	    {
		    eMMC_debug(0,1,"Err, SingleBlkWRC C, %Xh \n", u32_err);
            while(1);
	    }

        eMMC_debug(0,1,"WCRC shift tuning \033[7;34m%d is ok\033[m\n", u8_i);

        u8_WCRC_test_result[u8_i] = 1;
    }

    REG_FCIE_CLRBIT(FCIE_RX_SHIFT_CNT, BIT_WRSTS_SHIFT_SEL);

    eMMC_DISABLE_RETRY(0);

    for(u8_i=0; u8_i<16; u8_i++)
    {
        if( u8_WCRC_test_result[u8_i] )
            return eMMC_ST_SUCCESS;
    }

    return u32_err;
}

U32 eMMCTest_WCRC_Shift_Tuning(U32 u32_eMMC_Addr, U32 u32_DataPattern)
{
	volatile U32 u32_i, *pu32_W=(U32*)gau8_WBuf, *pu32_R=(U32*)gau8_RBuf;

    eMMC_debug(0, 1, "Data pattern %08X\n", u32_DataPattern);

	// init data pattern
	if(u32_DataPattern == 0x00112233 || (u32_DataPattern == 0x33221100))
	{
		for(u32_i = 0; u32_i < TEST_BUFFER_SIZE; u32_i ++)
		{
			if(u32_DataPattern == 0x00112233)
			{
				gau8_WBuf[u32_i] = u32_i & 0xFF;
			}
			else
			{
				gau8_WBuf[u32_i] = 0xFF - (u32_i & 0xFF);
			}
			gau8_RBuf[u32_i] = ~gau8_WBuf[u32_i];
		}
	}
	else
	{
		for(u32_i=0; u32_i<TEST_BUFFER_SIZE>>2; u32_i++)
		{
			pu32_W[u32_i] = u32_DataPattern;
			pu32_R[u32_i] = ~pu32_W[u32_i];
		}
	}

	return eMMCTest_WCRC_Shift_Tuning_Ex(u32_eMMC_Addr, (U8*)pu32_W, (U8*)pu32_R);
}

U32 eMMCTest_RSTOP_Shift_Tuning_Ex(U32 u32_eMMC_Addr, U8 *pu8_W, U8 *pu8_R)
{
	U32 u32_err;
    U8  u8_i = 15;
    U8  u8_RSTOP_test_result[16] = {0};

	u32_err = eMMC_CMD24_MIU(u32_eMMC_Addr, pu8_W);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(0,1,"Err, SingleBlkWRC W, %Xh \n", u32_err);
		while(1);
	}

    eMMC_DISABLE_RETRY(1);

    for(u8_i=0; u8_i<16; u8_i++)
    {
        REG_FCIE_CLRBIT(FCIE_RX_SHIFT_CNT, BIT_RSTOP_SHIFT_TUNE_MASK);
        REG_FCIE_SETBIT(FCIE_RX_SHIFT_CNT, BIT_RSTOP_SHIFT_SEL|u8_i);

        memset(pu8_R, 0, 512);

        eMMC_DISABLE_LOG(1);
	    u32_err = eMMC_CMD17_MIU(u32_eMMC_Addr, pu8_R);
        eMMC_DISABLE_LOG(0);
	    if(eMMC_ST_SUCCESS != u32_err)
	    {
		    eMMC_debug(0,1,"RStop shift tuning \033[7;35m%d is not ok\033[m\n", u8_i);
		    continue;
		    //return 0;
	    }

	    u32_err = eMMC_ComapreData(pu8_W, pu8_R, eMMC_SECTOR_BYTECNT);
	    if(eMMC_ST_SUCCESS != u32_err)
	    {
		    eMMC_debug(0,1,"Err, SingleBlkWRC C, %Xh \n", u32_err);
            while(1);
	    }

        eMMC_debug(0,1,"RStop shift tuning \033[7;34m%d is ok!!!\033[m\n", u8_i);

        u8_RSTOP_test_result[u8_i] = 1;
    }

    REG_FCIE_CLRBIT(FCIE_RX_SHIFT_CNT, BIT_RSTOP_SHIFT_SEL);

    eMMC_DISABLE_RETRY(0);

    for(u8_i=0; u8_i<16; u8_i++)
    {
        if( u8_RSTOP_test_result[u8_i] )
            return eMMC_ST_SUCCESS;
    }

    return u32_err;
}

U32 eMMCTest_RSTOP_Shift_Tuning(U32 u32_eMMC_Addr, U32 u32_DataPattern)
{
	volatile U32 u32_i, *pu32_W=(U32*)gau8_WBuf, *pu32_R=(U32*)gau8_RBuf;

    eMMC_debug(0, 1, "Data pattern %08X\n", u32_DataPattern);

	// init data pattern
	if(u32_DataPattern == 0x00112233 || (u32_DataPattern == 0x33221100))
	{
		for(u32_i = 0; u32_i < TEST_BUFFER_SIZE; u32_i ++)
		{
			if(u32_DataPattern == 0x00112233)
			{
				gau8_WBuf[u32_i] = u32_i & 0xFF;
			}
			else
			{
				gau8_WBuf[u32_i] = 0xFF - (u32_i & 0xFF);
			}
			gau8_RBuf[u32_i] = ~gau8_WBuf[u32_i];
		}
	}
	else
	{
		for(u32_i=0; u32_i<TEST_BUFFER_SIZE>>2; u32_i++)
		{
			pu32_W[u32_i] = u32_DataPattern;
			pu32_R[u32_i] = ~pu32_W[u32_i];
		}
	}

	return eMMCTest_RSTOP_Shift_Tuning_Ex(u32_eMMC_Addr, (U8*)pu32_W, (U8*)pu32_R);
}

U32 eMMCTest_READ_CRC_ERR(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;

    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DTRX_EN | BIT_ERR_DET_ON;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
	u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;

	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, 1);
	u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE);

	REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);

    REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, (eMMC_SECTOR_512BYTE+1) & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16, (eMMC_SECTOR_512BYTE+1) >> 16);
    REG_FCIE_W(FCIE_BLK_SIZE, eMMC_SECTOR_512BYTE+1);

    #if 0
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	#endif

	u32_err = eMMC_FCIE_SendCmd(
		u16_mode, u16_ctrl, u32_arg, 17, eMMC_R1_BYTE_CNT);
	if(eMMC_ST_SUCCESS != u32_err)
	{
	    //eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD17 %Xh, Arg: %Xh \n", u32_err, u32_arg);
        eMMC_FCIE_ErrHandler_ReInit();
		goto LABEL_END;
	}

	// -------------------------------
	// check FCIE

	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT,
	    BIT_DMA_END, TIME_WAIT_1_BLK_END);

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & BIT_SD_FCIE_ERR_FLAGS))
	{
        u32_err = eMMC_ST_ERR_CMD17_MIU;
		//eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
		//	       "eMMC Err: CMD17 SD_STS: %04Xh, Err: %Xh, Arg: %Xh\n", u16_reg, u32_err, u32_arg);
        eMMC_FCIE_ErrHandler_ReInit();
        goto LABEL_END;
	}

	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
        u32_err = eMMC_ST_ERR_CMD17_MIU;
		//eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
		//           "eMMC Err: CMD17 check R1 error: %Xh, Arg: %Xh\n", u32_err, u32_arg);
		eMMC_FCIE_ErrHandler_ReInit();
		goto LABEL_END;
	}

	LABEL_END:

	eMMC_FCIE_CLK_DIS();
	return u32_err;
}

U32 eMMCTest_WRITE_CRC_ERR(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;

	// -------------------------------
	// send cmd
	u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
	u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;

	eMMC_FCIE_ClearEvents();

	REG_FCIE_W(FCIE_JOB_BL_CNT, 1);

	u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE);

	REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);

    REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, (eMMC_SECTOR_512BYTE-1) & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16,( eMMC_SECTOR_512BYTE-1) >> 16);
    REG_FCIE_W(FCIE_BLK_SIZE, eMMC_SECTOR_512BYTE-1);

    #if 0
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	#endif

	u32_err = eMMC_FCIE_SendCmd(
		u16_mode, u16_ctrl, u32_arg, 24, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD24 %Xh, Arg: %Xh \n", u32_err, u32_arg);
		eMMC_FCIE_ErrHandler_ReInit();
		goto LABEL_END;
    }

	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
        u32_err = eMMC_ST_ERR_CMD24_MIU_CHK_R1;
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
		           "eMMC Err: CMD24 check R1 error: %Xh, Arg: %Xh\n", u32_err, u32_arg);
		eMMC_FCIE_ErrHandler_ReInit();
		goto LABEL_END;
	}

	// -------------------------------
	// send data
	u32_err = eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH);
	if(eMMC_ST_SUCCESS != u32_err)
	{
	    u32_err = eMMC_ST_ERR_CMD24_MIU_WAIT_D0H;
		goto LABEL_END;
	}

    REG_FCIE_W(FCIE_SD_CTRL, BIT_SD_DTRX_EN|BIT_SD_DAT_DIR_W|BIT_ERR_DET_ON);
    REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_JOB_START);

	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_DMA_END, TIME_WAIT_1_BLK_END);

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & (BIT_SD_W_FAIL|BIT_SD_W_CRC_ERR)))
	{
        u32_err = eMMC_ST_ERR_CMD24_MIU;
		//eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
        //           "eMMC Err: CMD24 SD_STS: %04Xh, Err: %Xh, Arg: %Xh\n", u16_reg, u32_err, u32_arg);
		eMMC_FCIE_ErrHandler_ReInit();
		goto LABEL_END;
	}

	LABEL_END:
	eMMC_FCIE_CLK_DIS();
	return u32_err;
}

U32 eMMCTest_WRITE_TIMEOUT_ERR(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf, U16 u16_BlkCnt)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;

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

    REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, (eMMC_SECTOR_512BYTE) & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16, (eMMC_SECTOR_512BYTE) >> 16);

	#if 0
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	#endif

	u32_err = eMMC_FCIE_SendCmd(u16_mode, u16_ctrl, u32_arg, 25, eMMC_R1_BYTE_CNT);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD25 %Xh, Arg: %Xh \n", u32_err, u32_arg);
        eMMC_FCIE_ErrHandler_ReInit();
		goto LABEL_END;
    }

	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
	    u32_err = eMMC_ST_ERR_CMD25_CHK_R1;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                   "eMMC Err: CMD25 check R1 error: %Xh, Arg: %Xh\n", u32_err, u32_arg);
        eMMC_FCIE_ErrHandler_ReInit();
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

    #if 0
    eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
    #endif

    REG_FCIE_W(FCIE_WR_SBIT_TIMER, BIT_WR_SBIT_TIMER_EN|0x01);
    REG_FCIE_W(FCIE_SD_CTRL, BIT_ERR_DET_ON);
    REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_SD_DTRX_EN|BIT_SD_DAT_DIR_W);
    REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_JOB_START);

    u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_DMA_END, HW_TIMER_DELAY_1s*10);

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & (BIT_SD_W_FAIL|BIT_SD_W_CRC_ERR)))
	{
	    u32_err = eMMC_ST_ERR_CMD25;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
                   "eMMC Err: CMD25 SD_STS: %04Xh, Err: %Xh, Arg: %Xh\n", u16_reg, u32_err, u32_arg);
		eMMC_FCIE_ErrHandler_ReInit();
		return u32_err;
	}

	LABEL_END:
    if(eMMC_ST_SUCCESS == u32_err)
    {
	    if(eMMC_ST_SUCCESS != eMMC_CMD12(g_eMMCDrv.u16_RCA))
		    eMMC_CMD12_NoCheck(g_eMMCDrv.u16_RCA);
    }

    eMMC_FCIE_CLK_DIS();
	return u32_err;
}

U32 eMMCTest_READ_TIMEOUT_ERR(U32 u32_Arg)
{
	U32 u32_err;
	U16 u16_ctrl;
    U32 u32_dma_addr;

    eMMC_FCIE_ClearEvents();

    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_DTRX_EN | BIT_ERR_DET_ON;

    REG_FCIE_W(FCIE_JOB_BL_CNT, 1);
    u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)DMA_R_ADDR, eMMC_SECTOR_512BYTE);

    REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
    REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);
    REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, eMMC_SECTOR_512BYTE & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16, eMMC_SECTOR_512BYTE >> 16);

    REG_FCIE_W(FCIE_RD_SBIT_TIMER, BIT_RD_SBIT_TIMER_EN|0x1);

    #if 0
    eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
    #endif

    REG_FCIE_CLRBIT(FCIE_CMD_RSP_SIZE, BIT_RSP_SIZE_MASK);
    REG_FCIE_W(FCIE_SD_MODE, g_eMMCDrv.u16_Reg10_Mode);

    REG_FCIE_W(FCIE_CMDFIFO_ADDR(0), 0x40);
    REG_FCIE_W(FCIE_CMDFIFO_ADDR(1), 0);
    REG_FCIE_W(FCIE_CMDFIFO_ADDR(2), 0);

    u32_err = eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    REG_FCIE_W(FCIE_SD_CTRL, u16_ctrl);
    REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_JOB_START);

    // wait event
    u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_CMD_END, TIME_WAIT_CMDRSP_END*2);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: CMD0 %Xh \n", u32_err);
        eMMC_FCIE_ErrHandler_ReInit();
	}

    LABEL_END:
    eMMC_FCIE_CLK_DIS();

	return u32_err;
}

U32 eMMCTest_CMD_NO_RSP_ERR(U32 u32_Arg)
{
	U32 u32_err;
	U16 u16_ctrl;

    eMMC_FCIE_ClearEvents();

    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_ERR_DET_ON;

    #if 0
    eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
    #endif

	u32_err = eMMC_FCIE_SendCmd(g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_Arg, 0, 0);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_FCIE_ErrHandler_ReInit();
	}

	eMMC_FCIE_CLK_DIS();
	return u32_err;
}

U32 eMMCTest_CMD_RSP_CRC_ERR(U32 u32_eMMCBlkAddr, U8 *pu8_DataBuf)
{
	U32 u32_err, u32_arg;
	U16 u16_mode, u16_ctrl, u16_reg;
	U32 u32_dma_addr;

    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_SD_DTRX_EN | BIT_ERR_DET_ON;

	u32_arg =  u32_eMMCBlkAddr << (g_eMMCDrv.u8_IfSectorMode?0:eMMC_SECTOR_512BYTE_BITS);
	u16_mode = g_eMMCDrv.u16_Reg10_Mode | g_eMMCDrv.u8_BUS_WIDTH;

	eMMC_FCIE_ClearEvents();
	REG_FCIE_W(FCIE_JOB_BL_CNT, 1);
	u32_dma_addr = eMMC_translate_DMA_address_Ex((U32)pu8_DataBuf, eMMC_SECTOR_512BYTE);

	REG_FCIE_W(FCIE_MIU_DMA_ADDR_15_0, u32_dma_addr & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_ADDR_31_16, u32_dma_addr >> 16);

	REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, eMMC_SECTOR_512BYTE & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16, eMMC_SECTOR_512BYTE >> 16);
    REG_FCIE_W(FCIE_BLK_SIZE,  eMMC_SECTOR_512BYTE);

    #if 0
    eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	eMMC_debug(0, 1, "1s\n");
	mdelay(1000);
	#endif

    REG_FCIE_CLRBIT(FCIE_CMD_RSP_SIZE, BIT_RSP_SIZE_MASK);
    REG_FCIE_SETBIT(FCIE_CMD_RSP_SIZE, (eMMC_R1_BYTE_CNT+1) & BIT_RSP_SIZE_MASK);
    REG_FCIE_W(FCIE_SD_MODE, u16_mode);

    REG_FCIE_W(FCIE_CMDFIFO_ADDR(0), ((u32_arg>>24)<<8) | (0x40|17));
    REG_FCIE_W(FCIE_CMDFIFO_ADDR(1), (u32_arg&0xFF00) | ((u32_arg>>16)&0xFF));
    REG_FCIE_W(FCIE_CMDFIFO_ADDR(2), u32_arg&0xFF);

    u32_err = eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH);
    if(eMMC_ST_SUCCESS != u32_err)
        goto LABEL_END;

    REG_FCIE_W(FCIE_SD_CTRL, u16_ctrl);
    REG_FCIE_SETBIT(FCIE_SD_CTRL, BIT_JOB_START);

    // wait event
    u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_CMD_END, TIME_WAIT_CMDRSP_END);

	if(eMMC_ST_SUCCESS != u32_err)
	{
        eMMC_FCIE_ErrHandler_ReInit();
		goto LABEL_END;
	}

	// -------------------------------
	// check FCIE

	u32_err = eMMC_FCIE_WaitEvents(FCIE_MIE_EVENT, BIT_DMA_END, TIME_WAIT_1_BLK_END);

	REG_FCIE_R(FCIE_SD_STATUS, u16_reg);
	if(eMMC_ST_SUCCESS != u32_err || (u16_reg & BIT_SD_FCIE_ERR_FLAGS))
	{
        u32_err = eMMC_ST_ERR_CMD17_MIU;
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
		    	   "eMMC Err: CMD17 SD_STS: %04Xh, Err: %Xh, Arg: %Xh\n", u16_reg, u32_err, u32_arg);
        eMMC_FCIE_ErrHandler_ReInit();
            goto LABEL_END;
	}

	// -------------------------------
	// check device
	u32_err = eMMC_CheckR1Error();
	if(eMMC_ST_SUCCESS != u32_err)
	{
        u32_err = eMMC_ST_ERR_CMD17_MIU;
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
		           "eMMC Err: CMD17 check R1 error: %Xh, Arg: %Xh\n", u32_err, u32_arg);
		eMMC_FCIE_ErrHandler_ReInit();
        goto LABEL_END;
	}

	LABEL_END:

	eMMC_FCIE_CLK_DIS();
	return u32_err;
}

U32 eMMCTest_R3_R4_Response(void)
{
    U32 u32_err, u32_arg;
    U16 u16_ctrl, u16_reg;

    // (sector mode | byte mode) | (3.0|3.1|3.2|3.3|3.4 V)
    u32_arg = BIT30 | (BIT23|BIT22|BIT21|BIT20|BIT19|BIT18|BIT17|BIT16|BIT15|BIT7);
    u16_ctrl = BIT_SD_CMD_EN | BIT_SD_RSP_EN | BIT_R3R4_RSP_EN;

    eMMC_FCIE_ClearEvents();
    u32_err = eMMC_FCIE_SendCmd(
        g_eMMCDrv.u16_Reg10_Mode, u16_ctrl, u32_arg, 1, eMMC_R3_BYTE_CNT);

    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
            "eMMC Warn: CMD1 send CMD fail: %08Xh \n", u32_err);
        return u32_err;
    }

    // check status
    REG_FCIE_R(FCIE_SD_STATUS, u16_reg);

    // R3 has no CRC, so does not check BIT_SD_RSP_CRC_ERR
    if(u16_reg & BIT_SD_RSP_TIMEOUT)
    {
        u32_err = eMMC_ST_ERR_CMD1;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
            "eMMC Warn: CMD1 no Rsp, Reg.12: %04Xh \n", u16_reg);

        return u32_err;
    }
    else if( u16_reg & BIT_CMD_RSP_CERR)
    {
        u32_err = eMMC_ST_ERR_CMD1;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1,
            "eMMC Warn: CMD1 CMD Rsp CRC error, Reg.12: %04Xh \n", u16_reg);
        return u32_err;
    }

    return u32_err;
}

U32 eMMCTest_Check_RST_Pin(void)
{
    U32 u32_err = 0;

    u32_err = eMMC_CMD17_MIU(eMMC_TEST_BLK_0, gau8_RBuf);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(0,1,"eMMC_CMD17_MIU fail %Xh \n", u32_err);
        while(1);
    }

    u32_err = eMMC_CMD13(g_eMMCDrv.u16_RCA);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(0,1,"eMMC_CMD13 fail %Xh \n", u32_err);
        while(1);
    }

    if( ((g_eMMCDrv.au8_Rsp[3] >> 1) & 0xF) != 4 )
    {
        eMMC_debug(0, 1, "eMMC is not in transfer state!!\n");
        while(1);
    }

    eMMC_debug(0, 0, "Toggle RST pin now\n");
    eMMC_RST_L(); udelay(1);
    eMMC_RST_H(); udelay(1);

    eMMC_DISABLE_RETRY(1);

    eMMC_DISABLE_LOG(1);
    u32_err = eMMC_CMD13(g_eMMCDrv.u16_RCA);
    eMMC_DISABLE_LOG(0);

    eMMC_DISABLE_RETRY(0);

    return u32_err;

}

#endif

U32 eMMC_IPVerify_Main(void)
{
	U32 u32_err;
    #ifdef IP_FCIE_VERSION_5
    U8 u8_i;
    U8 u8_j;
    int i;
    U16 tmp[16];
    #endif

    if( !gau8_WBuf )
        gau8_WBuf = (U8*)memalign(eMMC_CACHE_LINE, TEST_BUFFER_SIZE);

    if( !gau8_RBuf )
        gau8_RBuf = (U8*)memalign(eMMC_CACHE_LINE, TEST_BUFFER_SIZE);

	eMMC_debug(eMMC_DEBUG_LEVEL, 1,"eMMC_IPVerify_Main\n");
	eMMC_debug(eMMC_DEBUG_LEVEL, 1,"%Xh %Xh %Xh\n",
		(U32)gau8_eMMC_SectorBuf, (U32)gau8_RBuf, (U32)gau8_WBuf);

    #ifndef IP_FCIE_VERSION_5
    // ===============================================
	u32_err = eMMC_Init();
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "Err, eMMC_Init fail: %Xh \n", u32_err);
		return u32_err;
	}
	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,1,"[eMMC_Init ok] \n");

	// ===============================================
	eMMC_IPVerify_Main_Ex(eMMC_PATTERN_FFFFFFFF);
	eMMC_IPVerify_Main_Ex(eMMC_PATTERN_00000000);
	eMMC_debug(eMMC_DEBUG_LEVEL, 0, "normal pattern test ok\n\n");

	eMMC_IPVerify_Main_Ex(eMMC_PATTERN_000000FF);
	eMMC_IPVerify_Main_Ex(eMMC_PATTERN_0000FFFF);
	eMMC_IPVerify_Main_Ex(eMMC_PATTERN_00FF00FF);
	eMMC_IPVerify_Main_Ex(eMMC_PATTERN_AA55AA55);
	eMMC_debug(eMMC_DEBUG_LEVEL, 0, "specific pattern test ok\n\n");

	#if 1
	eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_FFFFFFFF);
	eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_00000000);
	eMMC_debug(eMMC_DEBUG_LEVEL, 0, "normal pattern test ok\n\n");

	eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_000000FF);
	eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_0000FFFF);
	eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_00FF00FF);
	eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_AA55AA55);
	eMMC_debug(eMMC_DEBUG_LEVEL, 0, "specific pattern test ok\n\n");
    #endif

	#if 1
    eMMC_IPVerify_Main_API_Ex(eMMC_PATTERN_00FF00FF);
	eMMC_IPVerify_Main_API_Ex(eMMC_PATTERN_AA55AA55);
    eMMC_debug(eMMC_DEBUG_LEVEL, 0, "APIs test ok\n\n");
	#endif

    #else   // FCIE5

    // To avoid compiling warning
    u8_i = 0;
    u8_j = 0;
    i = 0;
    memset(tmp, 0, 16);

    // ===============================================
    #if (0 || FCIE_BYPASS_VERIFY)   // BYPASS

    for(u8_i=0; u8_i<3; u8_i++)     // width
    {
        for(u8_j=0; u8_j<2; u8_j++) // speed
        {
            u32_err = eMMC_Init_For_Test();
            if(eMMC_ST_SUCCESS != u32_err)
                while(1);

            if(u8_j)    // High Speed
            {
                u32_err = eMMC_SetBusSpeed(eMMC_SPEED_HIGH);
                if(eMMC_ST_SUCCESS != u32_err)
                    while(1);
            }

            u32_err = eMMC_SetBusWidth(u8_BusWidth[u8_i], 0);
            if(eMMC_ST_SUCCESS != u32_err)
                while(1);

            eMMC_pads_switch(FCIE_eMMC_BYPASS);
            eMMC_clock_setting(FCIE_SLOW_CLK);

            eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;33meMMC BYPASS mode speed %s bus width %d\033[m\n\n",
                                            (u8_j) ? "HIGH" : "LOW", u8_BusWidth[u8_i]);

            for(i=0; i<12; i++)
            {
                u32_err = eMMC_IPVerify_Main_Ex(u32_test_pattern[i]);
                if(eMMC_ST_SUCCESS != u32_err)
                    while(1);
            }
            eMMC_debug(eMMC_DEBUG_LEVEL, 1, "normal pattern test ok\n\n");

            for(i=0; i<12; i++)
            {
                u32_err = eMMC_IPVerify_Main_Sg_Ex(u32_test_pattern[i]);
                if(eMMC_ST_SUCCESS != u32_err)
                    while(1);
            }
            eMMC_debug(eMMC_DEBUG_LEVEL, 1, "sglist pattern test ok\n\n");

            eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;32meMMC BYPASS mode speed %s bus width %d verify ok\033[m\n\n",
                                            (u8_j) ? "HIGH" : "LOW", u8_BusWidth[u8_i]);
        }

	}

	#endif

    #if (0 || FCIE_BYPASS_RIU_VERIFY)   // BYPASS + RIU mode

    for(u8_i=0; u8_i<3; u8_i++)     // width
    {
        u32_err = eMMC_Init_For_Test();
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);

        u32_err = eMMC_SetBusSpeed(eMMC_SPEED_HIGH);
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);

        u32_err = eMMC_SetBusWidth(u8_BusWidth[u8_i], 0);
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);

        eMMC_pads_switch(FCIE_eMMC_BYPASS);
        eMMC_clock_setting(FCIE_SLOW_CLK);

        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;33meMMC RIU BYPASS mode speed HIGH bus width %d\033[m\n\n",
                                         u8_BusWidth[u8_i]);

        for(i=0; i<12; i++)
        {
            u32_err = eMMC_IPVerify_RIU_Main_Ex(u32_test_pattern[i]);
            if(eMMC_ST_SUCCESS != u32_err)
                while(1);
        }
        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "normal pattern test ok\n\n");

        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;32meMMC RIU BYPASS mode speed HIGH bus width %d verify ok\033[m\n\n",
                                         u8_BusWidth[u8_i]);

	}

	#endif

    #if (0 || FCIE_GPIO_SDR_VERIFY)   // GPIO_SDR

    for(u8_i=0; u8_i<3; u8_i++)     // width
    {
        for(u8_j=0; u8_j<2; u8_j++) // speed
        {
            u32_err = eMMC_Init_For_Test();
            if(eMMC_ST_SUCCESS != u32_err)
                while(1);

            if(u8_j)    // High Speed
            {
                u32_err = eMMC_SetBusSpeed(eMMC_SPEED_HIGH);
                if(eMMC_ST_SUCCESS != u32_err)
                    while(1);
            }

            u32_err = eMMC_SetBusWidth(u8_BusWidth[u8_i], 0);
            if(eMMC_ST_SUCCESS != u32_err)
                while(1);

            eMMC_pads_switch(FCIE_eMMC_GPIO_SDR);
            eMMC_clock_setting(FCIE_SLOW_CLK);

            eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;33meMMC GPIO SDR mode speed %s bus width %d\033[m\n\n",
                                            (u8_j) ? "HIGH" : "LOW", u8_BusWidth[u8_i]);

            for(i=0; i<12; i++)
            {
                u32_err = eMMC_IPVerify_Main_Ex(u32_test_pattern[i]);
                if(eMMC_ST_SUCCESS != u32_err)
                    while(1);
            }
            eMMC_debug(eMMC_DEBUG_LEVEL, 1, "normal pattern test ok\n\n");

            for(i=0; i<12; i++)
            {
                u32_err = eMMC_IPVerify_Main_Sg_Ex(u32_test_pattern[i]);
                if(eMMC_ST_SUCCESS != u32_err)
                    while(1);
            }
            eMMC_debug(eMMC_DEBUG_LEVEL, 1, "sglist pattern test ok\n\n");

            eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;32meMMC GPIO SDR mode speed %s bus width %d verify ok\033[m\n\n",
                                            (u8_j) ? "HIGH" : "LOW", u8_BusWidth[u8_i]);
        }

	}

	#endif

    #if (0 || FCIE_SDR_VERIFY) // SDR
    for(u8_i=1; u8_i<3; u8_i++)
    {
        u32_err = eMMC_Init_For_Test();
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);

        u32_err = eMMC_SetBusSpeed(eMMC_SPEED_HIGH);
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);

        u32_err = eMMC_SetBusWidth(u8_BusWidth[u8_i], 0);
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);

        eMMC_pads_switch(FCIE_eMMC_SDR);
        eMMC_clock_setting(FCIE_DEFAULT_CLK);

        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;33meMMC SDR mode, bus width %d\033[m\n\n",
                                        u8_BusWidth[u8_i]);

        for(i=0; i<12; i++)
        {
            u32_err = eMMC_IPVerify_Main_Ex(u32_test_pattern[i]);
            if(eMMC_ST_SUCCESS != u32_err)
                while(1);
        }
        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "normal pattern test ok\n\n");

        for(i=0; i<12; i++)
        {
            u32_err = eMMC_IPVerify_Main_Sg_Ex(u32_test_pattern[i]);
            if(eMMC_ST_SUCCESS != u32_err)
                while(1);
        }
        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "sglist pattern test ok\n\n");

        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;32meMMC SDR mode, bus width %d verify ok\033[m\n\n",
                                        u8_BusWidth[u8_i]);
    }

	#endif

    #if (0 || FCIE_DDR_VERIFY) && (defined(ENABLE_eMMC_ATOP) && ENABLE_eMMC_ATOP)// DDR

    //for(u8_i=1; u8_i<3; u8_i++)   //FIXME: building 4-bit table
    for(u8_i=2; u8_i<3; u8_i++)
    {
        u32_err = eMMC_Init_For_Test();
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);

        u32_err = eMMC_FCIE_EnableFastMode_Ex(FCIE_eMMC_DDR);
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);

        u32_err = eMMC_FCIE_BuildDDRTimingTable();
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);

        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;33meMMC DDR mode, bus width %d\033[m\n\n",
                                        u8_BusWidth[u8_i]);

        for(i=0; i<12; i++)
        {
            u32_err = eMMC_IPVerify_Main_Ex(u32_test_pattern[i]);
            if(eMMC_ST_SUCCESS != u32_err)
                while(1);
        }
        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "normal pattern test ok\n\n");

        for(i=0; i<12; i++)
        {
            u32_err = eMMC_IPVerify_Main_Sg_Ex(u32_test_pattern[i]);
            if(eMMC_ST_SUCCESS != u32_err)
                while(1);
        }
        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "sglist pattern test ok\n\n");

        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;32meMMC DDR mode, bus width %d verify ok\033[m\n\n",
                                        u8_BusWidth[u8_i]);
    }

	#endif

    #if (0 || FCIE_HS200_VERIFY) && (defined(ENABLE_eMMC_HS200) && ENABLE_eMMC_HS200)   // HS200

    //for(u8_i=1; u8_i<3; u8_i++)   //FIXME: building 4-bit table
    for(u8_i = 2; u8_i < 3; u8_i ++)
    {
        u32_err = eMMC_Init_For_Test();
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);

        u32_err = eMMC_FCIE_EnableFastMode_Ex(FCIE_eMMC_HS200);
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);

        u32_err = eMMC_FCIE_BuildHS200TimingTable();
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);

        #if defined(ENABLE_eMMC_AFIFO) && ENABLE_eMMC_AFIFO
        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;33meMMC HS200 AFIFO mode, bus width %d\033[m\n\n",
                                        u8_BusWidth[u8_i]);
        #else
        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;33meMMC HS200 mode, bus width %d\033[m\n\n",
                                        u8_BusWidth[u8_i]);
        #endif

        u32_err = eMMC_CMD21();
        if( u32_err )
            while(1);
        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "CMD21 tuning pattern test ok\n\n");

        for(i=0; i<12; i++)
        {
            u32_err = eMMC_IPVerify_Main_Ex(u32_test_pattern[i]);
            if(eMMC_ST_SUCCESS != u32_err)
                while(1);
        }
        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "normal pattern test ok\n\n");

        for(i=0; i<12; i++)
        {
            u32_err = eMMC_IPVerify_Main_Sg_Ex(u32_test_pattern[i]);
            if(eMMC_ST_SUCCESS != u32_err)
                while(1);
        }
        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "sglist pattern test ok\n\n");

        #if defined(ENABLE_eMMC_AFIFO) && ENABLE_eMMC_AFIFO
        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;32meMMC HS200 AFIFO mode, bus width %d verify ok\033[m\n\n",
                                        u8_BusWidth[u8_i]);
        #else
        eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;32meMMC HS200 mode, bus width %d verify ok\033[m\n\n",
                                        u8_BusWidth[u8_i]);
        #endif
    }

    #endif

    #if (0 || FCIE_HS400_VERIFY) && (defined(ENABLE_eMMC_HS400) && ENABLE_eMMC_HS400)   // HS400

    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;

    // init platform & FCIE
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();
    if(u32_err)
        while(1);

    u32_err = eMMC_Init_Device();
    if(u32_err)
        while(1);

    #if defined(ENABLE_eMMC_AFIFO) && ENABLE_eMMC_AFIFO
    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;33meMMC HS400 AFIFO mode, bus width 8\033[m\n\n");
    #else
    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;33meMMC HS400 mode, bus width 8\033[m\n\n");
    #endif

    for(i=0; i<12; i++)
    {
	    u32_err = eMMC_IPVerify_Main_Ex(u32_test_pattern[i]);
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);
    }
    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "normal pattern test ok\n\n");

    for(i=0; i<12; i++)
    {
        u32_err = eMMC_IPVerify_Main_Sg_Ex(u32_test_pattern[i]);
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);
    }
    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "sglist pattern test ok\n\n");

    #if defined(ENABLE_eMMC_AFIFO) && ENABLE_eMMC_AFIFO
    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;32meMMC HS400 AFIFO mode, bus width 8 verify ok\033[m\n\n");
    #else
    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;32meMMC HS400 mode, bus width 8 verify ok\033[m\n\n");
    #endif

	#endif

    #if (0 || FCIE_HS400_VERIFY) && (defined(ENABLE_eMMC_HS400_5_1) && ENABLE_eMMC_HS400_5_1)   // HS400

    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;

    // init platform & FCIE
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();
    if(u32_err)
        while(1);

    u32_err = eMMC_Init_Device_Ex();
    if(eMMC_ST_SUCCESS != u32_err)
        while(1);

    u32_err = eMMC_FCIE_EnableFastMode_Ex(FCIE_eMMC_HS400_5_1);
    if(eMMC_ST_SUCCESS != u32_err)
        while(1);

    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;33meMMC HS400 enhence mode, bus width 8\033[m\n\n");

    for(i=0; i<12; i++)
    {
	    u32_err = eMMC_IPVerify_Main_Ex(u32_test_pattern[i]);
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);
    }
    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "normal pattern test ok\n\n");

    for(i=0; i<12; i++)
    {
        u32_err = eMMC_IPVerify_Main_Sg_Ex(u32_test_pattern[i]);
        if(eMMC_ST_SUCCESS != u32_err)
            while(1);
    }
    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "sglist pattern test ok\n\n");

    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\033[7;32meMMC HS400 enhence mode, bus width 8 verify ok\033[m\n\n");

    #endif

    #if (0 || FCIE_BIST_TEST)

    eMMC_FCIE_ErrHandler_ReInit();

    eMMC_debug(0, 1, "\033[7;33meMMC BIST Verify\033[m\n\n");

    for(u8_i=0; u8_i<12; u8_i++)
    {
        u32_err = eMMCTest_BIST(u8_i);
        if( u32_err ) break;
    }

    if( u32_err )
    {
        eMMC_debug(0, 1, "\033[7;31meMMC BIST Verify Fail\033[m\n\n");
        while(1);
    }
    else
        eMMC_debug(0, 1, "\033[7;32meMMC BIST Verify OK\033[m\n\n");

    #endif

    #if (0 || FCIE_POWER_SAVING_MODE_TEST)

    eMMC_FCIE_ErrHandler_ReInit();

    eMMC_debug(0, 0, "\033[7;31mTest power saving mode\033[m\n");
    eMMCTest_Power_Saving_Mode();

    // Check with LA

    #endif

    #if (0 || FCIE_RSP_SHIFT_TUNING_TEST)

    eMMC_FCIE_ErrHandler_ReInit();

    eMMC_debug(0, 1, "\033[7;33meMMC RSP Shift Tuning\033[m\n\n");

    for(u8_i=0; u8_i<12; u8_i++)
    {
        u32_err = eMMCTest_RSP_Shift_Tuning(eMMC_TEST_BLK_0, u32_test_pattern[u8_i]);
        if( u32_err ) break;
    }

    if( u32_err )
    {
        eMMC_debug(0, 1, "\033[7;31meMMC RSP Shift Tuning Fail\033[m\n\n");
        while(1);
    }
    else
        eMMC_debug(0, 1, "\033[7;32meMMC RSP Shift Tuning OK\033[m\n\n");

    #endif

    #if (0 || FCIE_WCRC_SHIFT_TUNING_TEST)

    eMMC_FCIE_ErrHandler_ReInit();

    eMMC_debug(0, 1, "\033[7;33meMMC WCRC Shift Tuning\033[m\n\n");

    for(u8_i=0; u8_i<12; u8_i++)
    {
        u32_err = eMMCTest_WCRC_Shift_Tuning(eMMC_TEST_BLK_0, u32_test_pattern[u8_i]);
        if( u32_err ) break;
    }

    if( u32_err )
    {
        eMMC_debug(0, 1, "\033[7;31meMMC WCRC Shift Tuning fail\033[m\n\n");
        while(1);
    }
    else
        eMMC_debug(0, 1, "\033[7;32meMMC WCRC Shift Tuning OK\033[m\n\n");

    #endif

    #if (0 || FCIE_RSTOP_SHIFT_TUNING_TEST)

    eMMC_FCIE_ErrHandler_ReInit();

    eMMC_debug(0, 1, "\033[7;33meMMC RStop Shift Tuning\033[m\n\n");

    for(u8_i=0; u8_i<12; u8_i++)
    {
        u32_err = eMMCTest_RSTOP_Shift_Tuning(eMMC_TEST_BLK_0, u32_test_pattern[u8_i]);
        if( u32_err ) break;
    }

    if( u32_err)
    {
        eMMC_debug(0, 1, "\033[7;31meMMC RStop Shift Tuning fail\033[m\n\n");
        while(1);
    }
    else
        eMMC_debug(0, 1, "\033[7;32meMMC RStop Shift Tuning OK\033[m\n\n");

    #endif

    #if (0 || FCIE_READ_CRC_ERROR_INT_TEST)
    // Test Read CRC error int

    eMMC_FCIE_ErrHandler_ReInit();

    eMMC_debug(0, 1, "\033[7;33mTest read CRC error int\033[m\n");

    u32_err = eMMCTest_READ_CRC_ERR(eMMC_TEST_BLK_0, gau8_RBuf);

    if( u32_err )
        eMMC_debug(0, 1, "\033[7;32meMMC Read CRC error int OK\033[m\n\n");
    else
    {
        eMMC_debug(0, 1, "\033[7;31meMMC Read CRC error int Fail\033[m\n\n");
        while(1);
    }

    #endif

    #if (0 || FCIE_WRITE_CRC_ERROR_INT_TEST)
    // Test Write CRC error int

    eMMC_FCIE_ErrHandler_ReInit();

    eMMC_debug(0, 1, "\033[7;33mTest write CRC error int\033[m\n");

    u32_err = eMMCTest_WRITE_CRC_ERR(eMMC_TEST_BLK_0, gau8_WBuf);

    if( u32_err )
        eMMC_debug(0, 1, "\033[7;32meMMC Write CRC error int OK\033[m\n\n");
    else
    {
        eMMC_debug(0, 1, "\033[7;31meMMC Write CRC error int Fail\033[m\n\n");
        while(1);
    }

    #endif

    #if (0 || FCIE_WRITE_TIMEOUT_INT_TEST)
    // Test Write timeout error int

    eMMC_FCIE_ErrHandler_ReInit();

    eMMC_debug(0, 1, "\033[7;33mTest write timeout int\033[m\n");

    u32_err = eMMCTest_WRITE_TIMEOUT_ERR(eMMC_TEST_BLK_0, gau8_WBuf, 2);

    if( u32_err )
        eMMC_debug(0, 1, "\033[7;32meMMC Write timeout error int OK\033[m\n\n");
    else
    {
        eMMC_debug(0, 1, "\033[7;31meMMC Write timeout error int Fail\033[m\n\n");
        while(1);
    }

    #endif

    #if (0 || FCIE_READ_TIMEOUT_INT_TEST)
    // Test read timeout error int

    eMMC_FCIE_ErrHandler_ReInit();

    eMMC_debug(0, 1, "\033[7;33mTest read timeout int\033[m\n");

    u32_err = eMMCTest_READ_TIMEOUT_ERR(0);

    if( u32_err )
        eMMC_debug(0, 1, "\033[7;32meMMC Read timeout error int OK\033[m\n\n");
    else
    {
        eMMC_debug(0, 1, "\033[7;31meMMC Read timeout error int Fail\033[m\n\n");
        while(1);
    }

    #endif

    #if (0 || FCIE_CMD_NO_RESPONSE_INT_TEST)
    // Test commnad No response int

    eMMC_FCIE_ErrHandler_ReInit();

    eMMC_debug(0, 1, "\033[7;33mTest commnad no response int\033[m\n");

    u32_err = eMMCTest_CMD_NO_RSP_ERR(0);

    if( u32_err )
        eMMC_debug(0, 1, "\033[7;32meMMC command no response int OK\033[m\n\n");
    else
    {
        eMMC_debug(0, 1, "\033[7;31meMMC command no response int Fail\033[m\n\n");
        while(1);
    }

    #endif

    #if (0 || FCIE_CMD_RSP_CRC_ERROR_INT_TEST)
    // Test command response CRC error int

    eMMC_FCIE_ErrHandler_ReInit();

    eMMC_debug(0, 1, "\033[7;33mTest commnad response CRC error int\033[m\n");

    u32_err = eMMCTest_CMD_RSP_CRC_ERR(eMMC_TEST_BLK_0, gau8_RBuf);

    if( u32_err )
        eMMC_debug(0, 1, "\033[7;32meMMC command response CRC error int OK\033[m\n\n");
    else
    {
        eMMC_debug(0, 1, "\033[7;31meMMC command response CRC error int Fail\033[m\n\n");
        while(1);
    }

    #endif

    #if (0 || FCIE_R3_R4_RESPONSE_TEST)

    g_eMMCDrv.u16_Reg10_Mode = BIT_SD_DEFAULT_MODE_REG;

    // init platform & FCIE
    eMMC_PlatformInit();
    u32_err = eMMC_FCIE_Init();
    if(u32_err)
        while(1);

    g_eMMCDrv.u16_RCA=1;
    g_eMMCDrv.u8_BUS_WIDTH = BIT_SD_DATA_WIDTH_1;
    g_eMMCDrv.u16_Reg10_Mode &= ~BIT_SD_DATA_WIDTH_MASK;
    g_eMMCDrv.u32_DrvFlag &= ~DRV_FLAG_SPEED_MASK;

    if(eMMC_ST_SUCCESS != eMMC_FCIE_WaitD0High(TIME_WAIT_DAT0_HIGH))
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: WaitD0High TO\n");
        eMMC_FCIE_ErrHandler_Stop();
    }
    eMMC_RST_L();  eMMC_hw_timer_sleep(1);
    eMMC_RST_H();  eMMC_hw_timer_sleep(1);

    // CMD0
    u32_err = eMMC_CMD0(0); // reset to idle state
    if(eMMC_ST_SUCCESS != u32_err)
        while(1);

    eMMC_debug(0, 1, "\033[7;33mTest R3/R4 response CRC error patch\033[m\n");

    u32_err = eMMCTest_R3_R4_Response();
    if(eMMC_ST_SUCCESS == u32_err)
        eMMC_debug(0, 1, "\033[7;32mTest R3/R4 response CRC error patch OK\033[m\n\n");
    else
    {
        eMMC_debug(0, 1, "\033[7;31mTest R3/R4 response CRC error patch Fail\033[m\n\n");
        while(1);
    }

    eMMC_FCIE_ErrHandler_ReInit();

    #endif

    #if (0 || FCIE_CHECK_RESET_PIN)

    eMMC_FCIE_ErrHandler_ReInit();

    eMMC_debug(0, 1, "\033[7;33mTest reset pin\033[m\n");

    u32_err = eMMCTest_Check_RST_Pin();

    if( eMMC_ST_SUCCESS != u32_err )
        eMMC_debug(0, 1, "\033[7;32mTest reset pin ok\033[m\n");
    else
    {
        eMMC_debug(0, 1, "\033[7;31mTest reset pin fail\033[m\n");
        while(1);
    }

    eMMC_FCIE_ErrHandler_ReInit();

    #endif

    #if (0 || FCIE_LAST_BLOCK_CRC_TEST)

    {
    U32 *pu32_W=(U32*)gau8_WBuf, *pu32_R=(U32*)gau8_RBuf;

    u32_err = eMMC_Init_For_Test();
    if(eMMC_ST_SUCCESS != u32_err)
        while(1);

    u32_err = eMMC_SetBusSpeed(eMMC_SPEED_HIGH);
    if(eMMC_ST_SUCCESS != u32_err)
        while(1);

    u32_err = eMMC_SetBusWidth(8, 0);
    if(eMMC_ST_SUCCESS != u32_err)
        while(1);

    eMMC_pads_switch(FCIE_eMMC_SDR);
    eMMC_clock_setting(FCIE_DEFAULT_CLK);

    eMMC_debug(0, 1, "\033[7;33mTest last CRC bank\033[m\n");

    for(u8_i=0; u8_i<12; u8_i++)
    {
        eMMC_debug(0, 1, "Data Pattern: %08X\n", u32_test_pattern[u8_i]);

        if( (u32_test_pattern[u8_i] == 0x00112233) || (u32_test_pattern[u8_i] == 0x33221100) )
        {
            for(i=0; i<512; i++)
            {
                if(u32_test_pattern[u8_i] == 0x00112233)
                    gau8_WBuf[i] = i & 0xFF;
                else if(u32_test_pattern[u8_i] == 0x33221100)
                    gau8_WBuf[i] = 0xFF - (i & 0xFF);
                gau8_RBuf[i] = ~gau8_WBuf[i];
            }
        }
        else
        {
            for(i=0; i<128; i++)
            {
                pu32_W[i] = u32_test_pattern[u8_i];
                pu32_R[i] = ~pu32_W[i];
            }
        }

        eMMC_DISABLE_RETRY(1);

        u32_err = eMMC_CMD25_MIU(eMMC_TEST_BLK_0, gau8_WBuf, 1);
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(0,1,"eMMC_CMD24_MIU fail %Xh \n", u32_err);
            while(1);
        }

        u32_err = eMMC_CMD17_MIU(eMMC_TEST_BLK_0, gau8_RBuf);
        if(eMMC_ST_SUCCESS != u32_err)
        {
            eMMC_debug(0,1,"eMMC_CMD17_MIU fail %Xh \n", u32_err);
            while(1);
        }

        eMMC_DISABLE_RETRY(0);

        for(i=0; i<16; i++)
        {
            REG_FCIE_R(GET_REG_ADDR(FCIE1_BASE, 0x40+i), tmp[i]);
        }

        if( memcmp(tmp, &u8_sdr_crc[u8_i], 16) != 0 )
        {
            eMMC_debug(0, 1, "Dump CRC Bank:\n");
            eMMC_dump_mem((U8*)tmp, 16);
            eMMC_debug(0, 1, "Correct CRC Bank:\n");
            eMMC_dump_mem((U8*)&u8_sdr_crc[u8_i], 16);
            eMMC_debug(0, 1, "\033[7;31mTest last CRC bank fail\033[m\n");
            while(1);
        }

        eMMC_debug(0, 1, "[OK]\n");

    }

    eMMC_debug(0, 1, "\033[7;32mTest last CRC bank ok\033[m\n");

    }

    #endif

    #if (0 || FCIE_CMDQ_VERIFY)

    eMMC_debug(0, 1, "Switch to CMDQ mode\n");

    u32_err = eMMC_Enable_CMDQ();
    if( u32_err != eMMC_ST_SUCCESS )
    {
        eMMC_debug(0, 1, "eMMC_Enable_CMDQ returns %X\n", u32_err);
        while(1);
    }
	
    #if 0
    if( eMMCTest_CMDQ_Legacy() != eMMC_ST_SUCCESS )
        eMMC_debug(0, 1, "CMDQ legacy way test fail\n");
    #endif

    for(i=0; i<12; i++)
    {
        u32_err = eMMCTest_CMDQ_FCIE(u32_test_pattern[i]);

        if( u32_err != eMMC_ST_SUCCESS )
        {
            eMMC_debug(0, 1, "CMDQ new design test fail\n");
            break;
        }
    }


    for(i=0; i<12; i++)
    {
        u32_err = eMMCTest_CMDQ_Max_Task(u32_test_pattern[i]);

        if( u32_err != eMMC_ST_SUCCESS )
        {
            eMMC_debug(0, 1, "CMDQ new design test fail\n");
            break;
        }
    }


    for(i=0; i<12; i++)
    {
        u32_err = eMMCTest_CMDQ_Random(u32_test_pattern[i]);

        if( u32_err != eMMC_ST_SUCCESS )
        {
            eMMC_debug(0, 1, "CMDQ new design test fail\n");
            break;
        }
    }


    for(i=0; i<12; i++)
    {
        u32_err = eMMCTest_CMDQ_ADMA(u32_test_pattern[i]);

        if( u32_err != eMMC_ST_SUCCESS )
        {
            eMMC_debug(0, 1, "CMDQ new design test fail\n");
            break;
        }
    }

    for(i=0; i<12; i++)
    {
        u32_err = eMMCTest_CMDQ_Random_ADMA(u32_test_pattern[i]);

        if( u32_err != eMMC_ST_SUCCESS )
        {
            eMMC_debug(0, 1, "CMDQ new design test fail\n");
            break;
        }
    }

    u32_err = eMMC_Disable_CMDQ();
    if( u32_err != eMMC_ST_SUCCESS )
    {
        eMMC_debug(0, 1, "eMMC_Enable_CMDQ returns %X\n", u32_err);
        while(1);
    }

    eMMC_debug(0, 1, "CMDQ new design test ok\n");

    #endif

    #endif

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

	return eMMC_ST_SUCCESS;
}


U32  eMMC_IPVerify_Main_API_Ex(U32 u32_DataPattern)
{
	U32 u32_i, *pu32_W=(U32*)gau8_WBuf, *pu32_R=(U32*)gau8_RBuf, u32_err;

	// init data pattern
	for(u32_i=0; u32_i<TEST_BUFFER_SIZE>>2; u32_i++)
	{
        pu32_W[u32_i] = u32_DataPattern;
		pu32_R[u32_i] = ~pu32_W[u32_i];
	}

	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "Data Pattern: %08Xh\n", u32_DataPattern);

    u32_err = eMMC_WriteData((U8*)pu32_W, TEST_BUFFER_SIZE, eMMC_TEST_BLK_0);
	if(eMMC_ST_SUCCESS != u32_err)
    {
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "Err, eMMC_WriteData fail: %Xh \n", u32_err);
		goto LABEL_IP_VERIFY_ERROR;
    }
	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,1,"[eMMC_WriteData ok] \n");

	u32_err = eMMC_ReadData((U8*)pu32_R, TEST_BUFFER_SIZE, eMMC_TEST_BLK_0);
	if(eMMC_ST_SUCCESS != u32_err)
    {
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "Err, eMMC_ReadData fail: %Xh \n", u32_err);
		goto LABEL_IP_VERIFY_ERROR;
    }
	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,1,"[eMMC_ReadData ok] \n");

	u32_err = eMMC_ComapreData((U8*)pu32_W, (U8*)pu32_R, TEST_BUFFER_SIZE);
	if(eMMC_ST_SUCCESS != u32_err)
    {
    	eMMC_debug(0,1,"Err, eMMC_ComapreData fail: %u, %Xh \n", u32_i, u32_err);
    	goto LABEL_IP_VERIFY_ERROR;
	}

	// ===============================================
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n");
    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "[OK] \n\n");
	return eMMC_ST_SUCCESS;

	LABEL_IP_VERIFY_ERROR:
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n\n");
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "[eMMC IPVerify Fail: %Xh] \n\n", u32_err);
	eMMC_DumpDriverStatus();
	while(1);
	return u32_err;
}

U32  eMMC_IPVerify_Main_Sg_Ex(U32 u32_DataPattern)
{
	static U32 u32_StartSector=0, u32_SectorCnt=0, u32_err;

	// make StartSector SectorCnt random
 	u32_StartSector = eMMC_TEST_BLK_0;
	u32_SectorCnt = eMMC_TEST_BLK_CNT;

	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "Data Pattern: %08Xh\n", u32_DataPattern);

	u32_err = eMMCTest_SgWRC_MIU(u32_StartSector, u32_SectorCnt, u32_DataPattern);
	if(eMMC_ST_SUCCESS != u32_err)
    {
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "Err, eMMCTest_SgWRC_MIU fail: %Xh \n", u32_err);
		goto LABEL_IP_VERIFY_ERROR;
    }
	eMMC_debug(eMMC_DEBUG_LEVEL, 1,"[eMMCTest_SgWRC_MIU ok] \n");

	// ===============================================
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n");
    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "[OK] \n\n");
	return eMMC_ST_SUCCESS;

	LABEL_IP_VERIFY_ERROR:
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n\n");
	eMMC_debug(eMMC_DEBUG_LEVEL,1,"Total Sec: %Xh, Test: StartSec: %Xh, SecCnt: %Xh \n",
			g_eMMCDrv.u32_SEC_COUNT, u32_StartSector, u32_SectorCnt);
    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "[eMMC IPVerify Fail: %Xh] \n\n", u32_err);
	eMMC_DumpDriverStatus();
	while(1);
	return u32_err;
}

U32 eMMC_IPVerify_Main_Big_Ex(U32 u32_DataPattern)
{
	U32 u32_StartSector=0, u32_err;

	// make StartSector SectorCnt random
 	u32_StartSector = eMMC_TEST_BLK_0;

	#if 1
	u32_err = eMMCTest_MultiBlkWRC_MIU_Large(u32_StartSector, u32_DataPattern);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "Err, eMMCTest_MultiBlkWRC_MIU_Large fail: %Xh \n", u32_err);
		goto LABEL_IP_VERIFY_ERROR;
	}
	eMMC_debug(eMMC_DEBUG_LEVEL,1,"[eMMCTest_MultiBlkWRC_MIU_Large ok] \n");
	#endif

	// ===============================================
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n");
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "[OK] \n\n");
	return eMMC_ST_SUCCESS;

	LABEL_IP_VERIFY_ERROR:
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n\n");
	eMMC_debug(eMMC_DEBUG_LEVEL,1,"Total Sec: %Xh, Test: StartSec: %Xh\n",
			g_eMMCDrv.u32_SEC_COUNT, u32_StartSector);
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "[eMMC IPVerify Fail: %Xh] \n\n", u32_err);
	eMMC_DumpDriverStatus();
	while(1);
	return u32_err;
}


U32 eMMC_IPVerify_Main_Ex(U32 u32_DataPattern)
{
	static U32 u32_StartSector=0, u32_SectorCnt=0, u32_err;

 	u32_StartSector = eMMC_TEST_BLK_0;
    u32_SectorCnt = eMMC_TEST_BLK_CNT;

	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "Data Pattern: %08Xh\n", u32_DataPattern);
	//eMMC_debug(0,1,"Total Sec: %Xh, StartSec: %Xh, SecCnt: %Xh \n",
	//		g_eMMCDrv.u32_SEC_COUNT, u32_StartSector, u32_SectorCnt);

	#if 0//because setting BIT_PAD_IN_SEL_SD and BIT_FALL_LATCH bit cause cifd crc error
	if(g_eMMCDrv.u8_PadType==FCIE_eMMC_BYPASS){
	u32_err = eMMCTest_SingleBlkWRC_RIU(u32_StartSector, u32_DataPattern);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "Err, eMMCTest_SingleBlkWRC_RIU fail: %Xh \n", u32_err);
		//goto LABEL_IP_VERIFY_ERROR;
	}
	eMMC_debug(eMMC_DEBUG_LEVEL,1,"[eMMCTest_SingleBlkWRC_RIU ok] \n");}
	#endif

	#if 1
	u32_err = eMMCTest_SingleBlkWRC_MIU(u32_StartSector, u32_DataPattern);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "Err, eMMCTest_SingleBlkWRC_MIU fail: %Xh \n", u32_err);
		goto LABEL_IP_VERIFY_ERROR;
	}
	eMMC_debug(eMMC_DEBUG_LEVEL,1,"[eMMCTest_SingleBlkWRC_MIU ok] \n");
	#endif

	#if 1
	u32_err = eMMCTest_MultiBlkWRC_MIU(u32_StartSector, u32_SectorCnt, u32_DataPattern);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "Err, eMMCTest_MultiBlkWRC_MIU fail: %Xh \n", u32_err);
		goto LABEL_IP_VERIFY_ERROR;
	}
	eMMC_debug(eMMC_DEBUG_LEVEL,1,"[eMMCTest_MultiBlkWRC_MIU ok] \n");
	#endif


	// ===============================================
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n");
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "[OK] \n\n");
	return eMMC_ST_SUCCESS;

	LABEL_IP_VERIFY_ERROR:
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n\n");
	eMMC_debug(eMMC_DEBUG_LEVEL,1,"Total Sec: %Xh, Test: StartSec: %Xh, SecCnt: %Xh \n",
			g_eMMCDrv.u32_SEC_COUNT, u32_StartSector, u32_SectorCnt);
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "[eMMC IPVerify Fail: %Xh] \n\n", u32_err);
	eMMC_DumpDriverStatus();
	while(1);
	return u32_err;
}

U32 eMMC_IPVerify_RIU_Main_Ex(U32 u32_DataPattern)
{
	static U32 u32_StartSector=0, u32_SectorCnt=0, u32_err;

	// make StartSector SectorCnt random
 	u32_StartSector = eMMC_TEST_BLK_0;
	u32_SectorCnt++;
	while(u32_SectorCnt > eMMC_TEST_BLK_CNT)
		u32_SectorCnt = 1;

	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "Data Pattern: %08Xh\n", u32_DataPattern);
	//eMMC_debug(0,1,"Total Sec: %Xh, StartSec: %Xh, SecCnt: %Xh \n",
	//		g_eMMCDrv.u32_SEC_COUNT, u32_StartSector, u32_SectorCnt);

	if(g_eMMCDrv.u8_PadType==FCIE_eMMC_BYPASS){
	u32_err = eMMCTest_SingleBlkWRC_RIU(u32_StartSector, u32_DataPattern);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "Err, eMMCTest_SingleBlkWRC_RIU fail: %Xh \n", u32_err);
		goto LABEL_IP_VERIFY_ERROR;
	}
	eMMC_debug(eMMC_DEBUG_LEVEL,1,"[eMMCTest_SingleBlkWRC_RIU ok] \n");}

	// ===============================================
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n");
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "[OK] \n\n");
	return eMMC_ST_SUCCESS;

	LABEL_IP_VERIFY_ERROR:
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n\n");
	eMMC_debug(eMMC_DEBUG_LEVEL,1,"Total Sec: %Xh, Test: StartSec: %Xh, SecCnt: %Xh \n",
			g_eMMCDrv.u32_SEC_COUNT, u32_StartSector, u32_SectorCnt);
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "[eMMC IPVerify Fail: %Xh] \n\n", u32_err);
	eMMC_DumpDriverStatus();
	while(1);
	return u32_err;
}


U32 eMMC_IPVerify_ReadOnly(void)
{
	static U32 u32_StartSector=0, u32_SectorCnt=0, u32_err;

	// make StartSector SectorCnt random
 	u32_StartSector = eMMC_TEST_BLK_0;
	#if 0
    u32_SectorCnt++;
    while(u32_SectorCnt > eMMC_TEST_BLK_CNT)
        u32_SectorCnt = 1;
    #else
    u32_SectorCnt = eMMC_TEST_BLK_CNT;
    #endif
	if( !gau8_RBuf )
        gau8_RBuf = (U8*)memalign(eMMC_CACHE_LINE, TEST_BUFFER_SIZE);

	#if 0
	if(eMMC_IF_NORMAL_SDR()){
	u32_err = eMMC_CMD17_CIFD(u32_StartSector, gau8_RBuf);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(0,1,"Err, CMD17_CIFD fail: %Xh \n", u32_err);
		goto LABEL_IP_VERIFY_ERROR;
	}
	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,1,"[CMD17_CIFD ok] \n");}
	#endif

	#if 0
	u32_err = eMMC_CMD17_MIU(u32_StartSector, gau8_RBuf);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(0,1,"Err, CMD17_MIU fail: %Xh \n", u32_err);
		goto LABEL_IP_VERIFY_ERROR;
	}
	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,1,"[CMD17_MIU ok] \n");
	#endif

	#if 1
	u32_err = eMMC_CMD18_MIU(u32_StartSector, gau8_RBuf, u32_SectorCnt);
	if(eMMC_ST_SUCCESS != u32_err)
	{
	    eMMC_debug(0,1,"Err, CMD18_MIU fail: %Xh \n", u32_err);
	    goto LABEL_IP_VERIFY_ERROR;
	}
	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,1,"[CMD18_MIU ok] \n");
	#endif
    if( gau8_RBuf )
    {
        free(gau8_RBuf);
        gau8_RBuf = 0;
    }
	// ===============================================
	//eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n");
	//eMMC_debug(eMMC_DEBUG_LEVEL, 1, "[OK] \n\n");
	return eMMC_ST_SUCCESS;

	LABEL_IP_VERIFY_ERROR:
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n\n");
	eMMC_debug(eMMC_DEBUG_LEVEL,1,"Total Sec: %Xh, Test: StartSec: %Xh, SecCnt: %Xh \n",
			g_eMMCDrv.u32_SEC_COUNT, u32_StartSector, u32_SectorCnt);
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "fail: %Xh \n\n", u32_err);
	eMMC_DumpDriverStatus();
    if( gau8_RBuf )
    {
        free(gau8_RBuf);
        gau8_RBuf = 0;
    }
	while(1);
	return u32_err;
}


U32 eMMC_IPVerify_WriteOnly(U16 u16_TestPattern)
{
	static U32 u32_StartSector=0, u32_SectorCnt=0, u32_err;
	U32 u32_i, *pu32_W, u32_ByteCnt;


    if( !gau8_WBuf )
        gau8_WBuf = (U8*)memalign(eMMC_CACHE_LINE, TEST_BUFFER_SIZE);

	pu32_W=(U32*)gau8_WBuf;

	// make StartSector SectorCnt random
 	u32_StartSector = eMMC_TEST_BLK_0;
    #if 0
	u32_SectorCnt++;
	while(u32_SectorCnt > eMMC_TEST_BLK_CNT)
		u32_SectorCnt = 1;
    #else
    u32_SectorCnt = eMMC_TEST_BLK_CNT;
    #endif

	u32_ByteCnt = (u32_SectorCnt<<9) > TEST_BUFFER_SIZE ?
		TEST_BUFFER_SIZE : (u32_SectorCnt<<9);

    // init data pattern
	for(u32_i=0; u32_i<u32_ByteCnt>>2; u32_i++)
	{
        pu32_W[u32_i] = (u16_TestPattern<<16)+u16_TestPattern;
	}

	#if 0
	if(eMMC_IF_NORMAL_SDR()){
	u32_err = eMMC_CMD24_CIFD(u32_StartSector, gau8_WBuf);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(0,1,"Err, CMD24_CIFD fail: %Xh \n", u32_err);
		goto LABEL_IP_VERIFY_ERROR;
	}
	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,1,"[CMD24_CIFD ok] \n");}
	#endif

	#if 0
	u32_err = eMMC_CMD24_MIU(u32_StartSector, gau8_WBuf);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(0,1,"Err, CMD24_MIU fail: %Xh \n", u32_err);
		goto LABEL_IP_VERIFY_ERROR;
	}
	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,1,"[CMD24_MIU ok] \n");
	#endif

	#if 1
	u32_err = eMMC_CMD25_MIU(u32_StartSector, gau8_WBuf, u32_SectorCnt);
	if(eMMC_ST_SUCCESS != u32_err)
	{
	    eMMC_debug(0,1,"Err, CMD25_MIU fail: %Xh \n", u32_err);
	    goto LABEL_IP_VERIFY_ERROR;
	}
	eMMC_debug(eMMC_DEBUG_LEVEL_HIGH,1,"[CMD25_MIU ok] \n");
	#endif

	// ===============================================
	//eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n");
	//eMMC_debug(eMMC_DEBUG_LEVEL, 1, "[OK] \n\n");
    if( gau8_WBuf )
    {
        free(gau8_WBuf);
        gau8_WBuf = 0;
    }

	return eMMC_ST_SUCCESS;

	LABEL_IP_VERIFY_ERROR:
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n\n");
	eMMC_debug(eMMC_DEBUG_LEVEL,1,"Total Sec: %Xh, Test: StartSec: %Xh, SecCnt: %Xh \n",
			g_eMMCDrv.u32_SEC_COUNT, u32_StartSector, u32_SectorCnt);
	eMMC_debug(eMMC_DEBUG_LEVEL, 1, "fail: %Xh \n\n", u32_err);
	eMMC_DumpDriverStatus();
    if( gau8_WBuf )
    {
        free(gau8_WBuf);
        gau8_WBuf = 0;
    }

	while(1);
	return u32_err;
}

U32  eMMC_Crazy_WriteOnly(U16 *u16_pattern_test,U16 u16_pattern_cnt)
{
    U32 u32_i,u32_j,u32_err=eMMC_ST_SUCCESS;
    U32 u32_StartSector=eMMC_TEST_BLK_0, u32_SectorCnt=eMMC_TEST_BLK_CNT;
    U32 *pu32_W;

    if( !gau8_WBuf )
        gau8_WBuf = (U8*)memalign(eMMC_CACHE_LINE, TEST_BUFFER_SIZE);

	pu32_W=(U32*)gau8_WBuf;

    if(u16_pattern_cnt==0)
        goto LABEL_IP_VERIFY_ERROR;

    // init data pattern
	for(u32_i=0; u32_i< (TEST_BUFFER_SIZE/u16_pattern_cnt)>>2; u32_i++)
	{
        for(u32_j=0;u32_j<u16_pattern_cnt;u32_j++)
            pu32_W[u32_i*u16_pattern_cnt+u32_j] = (u16_pattern_test[u32_j]<<16)+u16_pattern_test[u32_j];
	}


    u32_err = eMMC_CMD25_MIU(u32_StartSector, (U8*)pu32_W, u32_SectorCnt);
    if(eMMC_ST_SUCCESS != u32_err)
    {
        eMMC_debug(0,1,"Err, CMD25_MIU fail: %Xh \n", u32_err);
        goto LABEL_IP_VERIFY_ERROR;
    }
    eMMC_debug(0,1,"[CMD25_MIU ok] \n");

    if( gau8_WBuf )
    {
        free(gau8_WBuf);
        gau8_WBuf = 0;
    }

    return eMMC_ST_SUCCESS;
    LABEL_IP_VERIFY_ERROR:
    eMMC_debug(eMMC_DEBUG_LEVEL, 1, "fail: %Xh \n\n", u32_err);
    eMMC_DumpDriverStatus();
    if( gau8_WBuf )
    {
        free(gau8_WBuf);
        gau8_WBuf = 0;
    }

    while(1);

    return u32_err;
}

U32 eMMC_IPVerify_Main_Big(void)
{

    eMMC_IPVerify_Main_Big_Ex(eMMC_PATTERN_FFFFFFFF);
    eMMC_IPVerify_Main_Big_Ex(eMMC_PATTERN_00000000);
    eMMC_debug(eMMC_DEBUG_LEVEL, 0, "normal pattern test ok\n\n");

    eMMC_IPVerify_Main_Big_Ex(eMMC_PATTERN_000000FF);
    eMMC_IPVerify_Main_Big_Ex(eMMC_PATTERN_0000FFFF);
    eMMC_IPVerify_Main_Big_Ex(eMMC_PATTERN_00FF00FF);
    eMMC_IPVerify_Main_Big_Ex(eMMC_PATTERN_AA55AA55);
    eMMC_debug(eMMC_DEBUG_LEVEL, 0, "specific pattern test ok\n\n");

    // ===============================================
    //eMMC_debug(eMMC_DEBUG_LEVEL, 1, "\n");
    //eMMC_debug(eMMC_DEBUG_LEVEL, 1, "[OK] \n\n");
    return eMMC_ST_SUCCESS;
}


U32 eMMCTest_Lifetime(U8 u8_TestMode)
{
	U32 u32_i, u32_err=eMMC_ST_SUCCESS, u32_BlkAddr;
	U32 u32_BlkCnt=0, u32_LoopCnt=0,u32_Write_GB=0;
	U32 u32_ChkSum;
    #if 0
	// set to slow & safe clock
	eMMC_clock_setting(gau8_FCIEClkSel[eMMC_FCIE_VALID_CLK_CNT-1]);
    #endif
	eMMC_debug(eMMC_DEBUG_LEVEL,0,"===========================================\n");
	eMMC_debug(eMMC_DEBUG_LEVEL,0,"CAUTION: will damage images in eMMC \n");
	eMMC_debug(eMMC_DEBUG_LEVEL,0,"===========================================\n");

    if( !gau8_WBuf )
        gau8_WBuf = (U8*)memalign(eMMC_CACHE_LINE, TEST_BUFFER_SIZE);

    if( !gau8_RBuf )
        gau8_RBuf = (U8*)memalign(eMMC_CACHE_LINE, TEST_BUFFER_SIZE);
    
	//-------------------------------------------------
	u32_err = eMMC_CMD17_MIU(eMMC_LIFE_TEST_BYTE_CNT_BLK, gau8_RBuf);
	if(eMMC_ST_SUCCESS != u32_err)
	{
		eMMC_debug(0,1,"Err, CMD17_MIU, %Xh \n", u32_err);
		eMMC_FCIE_ErrHandler_Stop();
	}
	memcpy((U8*)&g_eMMCDrv.Life_Bytes_Cnt_t, gau8_RBuf, sizeof(g_eMMCDrv.Life_Bytes_Cnt_t));

	u32_ChkSum = eMMC_ChkSum((U8*)&g_eMMCDrv.Life_Bytes_Cnt_t, sizeof(g_eMMCDrv.Life_Bytes_Cnt_t)-4);

	if(eMMC_LIFETIME_TEST_FIXED == u8_TestMode)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL,1,"fixed addr: %u th block\n",EMMC_RESERVED_BLK_FOR_MAP+EMMC_UBOOT_SIZE+EMMC_MPOOL_SIZE);
		if(u32_ChkSum != g_eMMCDrv.Life_Bytes_Cnt_t.u32_ChkSum)
		{
			u32_Write_GB = 0;
			eMMC_debug(0,1,"No Life Bytes Count Store in eMMC\n");
		}
		else
		{
			u32_Write_GB = g_eMMCDrv.Life_Bytes_Cnt_t.u32_LIFETIME_TEST_FIXED_GBytes;
			eMMC_debug(0,1,"Life Bytes Count Store in eMMC %u GBytes\n",u32_Write_GB);
		}
		while(1)
		{
			eMMC_reset_WatchDog();

            if((U32)(0-1) == u32_BlkCnt)
            {
				eMMC_debug(eMMC_DEBUG_LEVEL,0,"\n eMMC: %Xh blocks tested, stop \n",
					u32_BlkCnt);
				while(1);
            }
			for(u32_i=0; u32_i<eMMC_SECTOR_BYTECNT; u32_i++)
				gau8_WBuf[u32_i] = u32_BlkCnt+u32_i;

			u32_err = eMMC_CMD24_MIU(EMMC_RESERVED_BLK_FOR_MAP+EMMC_UBOOT_SIZE+EMMC_MPOOL_SIZE, gau8_WBuf);
	        if(eMMC_ST_SUCCESS != u32_err)
	        {
		        eMMC_debug(0,1,"Err, CMD24_MIU, %Xh \n", u32_err);
		        break;
	        }

			u32_err = eMMC_CMD18_MIU( // to drop data from possible cache inside eMMC
			    0, gau8_RBuf, TEST_BUFFER_SIZE>>eMMC_SECTOR_BYTECNT_BITS);
	        if(eMMC_ST_SUCCESS != u32_err)
	        {
		        eMMC_debug(0,1,"Err, CMD18_MIU, %Xh \n", u32_err);
		        break;
	        }
			u32_err = eMMC_CMD18_MIU( // to drop data from possible cache inside eMMC
			    100, gau8_RBuf, TEST_BUFFER_SIZE>>eMMC_SECTOR_BYTECNT_BITS);
	        if(eMMC_ST_SUCCESS != u32_err)
	        {
		        eMMC_debug(0,1,"Err, CMD18_MIU, %Xh \n", u32_err);
		        break;
	        }

			u32_err = eMMC_CMD17_MIU(EMMC_RESERVED_BLK_FOR_MAP+EMMC_UBOOT_SIZE+EMMC_MPOOL_SIZE, gau8_RBuf);
	        if(eMMC_ST_SUCCESS != u32_err)
	        {
		        eMMC_debug(0,1,"Err, CMD17_MIU, %Xh \n", u32_err);
		        break;
	        }

	        u32_err = eMMC_ComapreData(gau8_WBuf, gau8_RBuf, eMMC_SECTOR_BYTECNT);
	        if(eMMC_ST_SUCCESS != u32_err)
        	{
		        eMMC_debug(0,1,"Err, ComapreData, %Xh \n", u32_err);
        		break;
        	}

			u32_BlkCnt+=1;

			if(((u32_BlkCnt<<eMMC_SECTOR_BYTECNT_BITS)&0x3FFFFFFF)==0)
			{
				u32_Write_GB++;
				eMMC_debug(0,1,"Write %u GBytes\n", u32_Write_GB);
				g_eMMCDrv.Life_Bytes_Cnt_t.u32_LIFETIME_TEST_FIXED_GBytes = u32_Write_GB;
				g_eMMCDrv.Life_Bytes_Cnt_t.u32_ChkSum=eMMC_ChkSum((U8*)&g_eMMCDrv.Life_Bytes_Cnt_t, sizeof(g_eMMCDrv.Life_Bytes_Cnt_t)-4);
				memset(gau8_WBuf,0,eMMC_SECTOR_BYTECNT);
                memcpy(gau8_WBuf,(U8*)&g_eMMCDrv.Life_Bytes_Cnt_t,sizeof(g_eMMCDrv.Life_Bytes_Cnt_t));
    			u32_err = eMMC_CMD24_MIU(eMMC_LIFE_TEST_BYTE_CNT_BLK, gau8_WBuf);
    	        if(eMMC_ST_SUCCESS != u32_err)
    	        {
    		        eMMC_debug(0,1,"Err, CMD24_MIU, %Xh \n", u32_err);
    		        break;
    	        }
			}
		}

		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"\neMMC Err: fail: %Xh \n", u32_err);
		eMMC_FCIE_ErrHandler_Stop();

	}
	//-------------------------------------------------
	else if(eMMC_LIFETIME_TEST_FILLED == u8_TestMode)
	{
        U32 TmpMB, TmpGB=0;
        
		if(u32_ChkSum != g_eMMCDrv.Life_Bytes_Cnt_t.u32_ChkSum)
		{
			u32_Write_GB = 0;
			eMMC_debug(0,1,"No Life Bytes Count Store in eMMC\n");
		}
		else
		{
			u32_Write_GB = g_eMMCDrv.Life_Bytes_Cnt_t.u32_LIFETIME_TEST_FILLED_GBytes;
			eMMC_debug(0,1,"Life Bytes Count Store in eMMC %u GBytes\n",u32_Write_GB);
		}
		while(1)
		{
			eMMC_reset_WatchDog();

			if((U32)(0-1) == u32_LoopCnt)
			{
				eMMC_debug(eMMC_DEBUG_LEVEL,0,"\n eMMC: %u loops tested, stop \n",
					u32_LoopCnt);
				while(1);
			}

			for(u32_i=0; u32_i<TEST_BUFFER_SIZE; u32_i++)
				gau8_WBuf[u32_i] = u32_LoopCnt+u32_i;
            
			// w
			TmpMB = 0;
			for(u32_i=EMMC_RESERVED_BLK_FOR_MAP+EMMC_UBOOT_SIZE+EMMC_MPOOL_SIZE; u32_i<g_eMMCDrv.u32_SEC_COUNT;)
			{
				u32_err = eMMC_CMD25_MIU(u32_i, gau8_WBuf, TEST_BUFFER_SIZE>>eMMC_SECTOR_BYTECNT_BITS);
				if(eMMC_ST_SUCCESS != u32_err)
				{
					eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"\neMMC Err: w fail: %Xh \n", u32_err);
					eMMC_debug(eMMC_DEBUG_LEVEL,0,"eMMC: (%Xh x %Xh) blocks tested, stop \n",
					    u32_LoopCnt, g_eMMCDrv.u32_SEC_COUNT);
					eMMC_FCIE_ErrHandler_Stop();
				}

				u32_i += TEST_BUFFER_SIZE>>eMMC_SECTOR_BYTECNT_BITS;
				if(g_eMMCDrv.u32_SEC_COUNT-u32_i <
					(TEST_BUFFER_SIZE>>eMMC_SECTOR_BYTECNT_BITS))
					break;

                TmpMB += TEST_BUFFER_SIZE>>20;
                if(0 == TmpMB % 0x400)
                {
                    TmpGB++;  TmpMB=0;
                }
                if((TmpMB % 100)==0)
                    eMMC_debug(eMMC_DEBUG_LEVEL,0,"\r write %u GB, %u MB ......",
                        TmpGB, TmpMB);                
			}
			// r & c
			TmpMB = 0;
			for(u32_i=EMMC_RESERVED_BLK_FOR_MAP+EMMC_UBOOT_SIZE+EMMC_MPOOL_SIZE; u32_i<g_eMMCDrv.u32_SEC_COUNT;)
			{
				u32_err = eMMC_CMD18_MIU(u32_i, gau8_RBuf, TEST_BUFFER_SIZE>>eMMC_SECTOR_BYTECNT_BITS);
				if(eMMC_ST_SUCCESS != u32_err)
				{
					eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"\neMMC Err: r fail: %Xh \n", u32_err);
					eMMC_debug(eMMC_DEBUG_LEVEL,0,"eMMC: (%Xh x %Xh) blocks tested, stop \n",
					    u32_LoopCnt, g_eMMCDrv.u32_SEC_COUNT);
					eMMC_FCIE_ErrHandler_Stop();
				}

				u32_err = eMMC_ComapreData(gau8_WBuf, gau8_RBuf, TEST_BUFFER_SIZE);
	            if(eMMC_ST_SUCCESS != u32_err)
	            {
					eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"\neMMC Err: c fail: %Xh \n", u32_err);
					eMMC_debug(eMMC_DEBUG_LEVEL,0,"eMMC: (%Xh x %Xh) blocks tested, stop \n",
					    u32_LoopCnt, g_eMMCDrv.u32_SEC_COUNT);
					eMMC_FCIE_ErrHandler_Stop();
	            }

				u32_i += TEST_BUFFER_SIZE>>eMMC_SECTOR_BYTECNT_BITS;
				if(g_eMMCDrv.u32_SEC_COUNT-u32_i <
					(TEST_BUFFER_SIZE>>eMMC_SECTOR_BYTECNT_BITS))
					break;

                TmpMB += TEST_BUFFER_SIZE>>20;
                if((TmpMB % 100)==0)
                    eMMC_debug(eMMC_DEBUG_LEVEL,0,"\r read & check %u%% ... ",
                        u32_i*100/g_eMMCDrv.u32_SEC_COUNT+1);         

			}

			u32_LoopCnt += 1;

			u32_Write_GB+=((g_eMMCDrv.u32_SEC_COUNT-EMMC_RESERVED_BLK_FOR_MAP-EMMC_UBOOT_SIZE-EMMC_MPOOL_SIZE)>>(30-eMMC_SECTOR_BYTECNT_BITS));
			g_eMMCDrv.Life_Bytes_Cnt_t.u32_LIFETIME_TEST_FILLED_GBytes = u32_Write_GB;
			g_eMMCDrv.Life_Bytes_Cnt_t.u32_ChkSum=eMMC_ChkSum((U8*)&g_eMMCDrv.Life_Bytes_Cnt_t, sizeof(g_eMMCDrv.Life_Bytes_Cnt_t)-4);
			memset(gau8_WBuf,0,eMMC_SECTOR_BYTECNT);
            memcpy(gau8_WBuf,(U8*)&g_eMMCDrv.Life_Bytes_Cnt_t,sizeof(g_eMMCDrv.Life_Bytes_Cnt_t));
			u32_err = eMMC_CMD24_MIU(eMMC_LIFE_TEST_BYTE_CNT_BLK, gau8_WBuf);
	        if(eMMC_ST_SUCCESS != u32_err)
	        {
		        eMMC_debug(0,1,"Err, CMD24_MIU, %Xh \n", u32_err);
		        eMMC_FCIE_ErrHandler_Stop();
	        }
		}
	}
	//-------------------------------------------------
	else if(eMMC_LIFETIME_TEST_RANDOM == u8_TestMode)
	{
		eMMC_hw_timer_start(); // use to get tick as a random seed
		if(u32_ChkSum != g_eMMCDrv.Life_Bytes_Cnt_t.u32_ChkSum)
		{
			u32_Write_GB = 0;
			eMMC_debug(0,1,"No Life Bytes Count Store in eMMC\n");
		}
		else
		{
			u32_Write_GB = g_eMMCDrv.Life_Bytes_Cnt_t.u32_LIFETIME_TEST_RANDOM_GBytes;
			eMMC_debug(0,1,"Life Bytes Count Store in eMMC %u GBytes\n",u32_Write_GB);
		}

		while(1)
		{
			eMMC_reset_WatchDog();

			if((U32)(0-1) == u32_LoopCnt)
			{
				eMMC_debug(eMMC_DEBUG_LEVEL,0,"\n eMMC: %u loops tested, stop \n",
					u32_LoopCnt);
				while(1);
			}

			for(u32_i=0; u32_i<TEST_BUFFER_SIZE; u32_i++)
				gau8_WBuf[u32_i] = u32_LoopCnt+u32_i;

			u32_BlkAddr = eMMC_hw_timer_tick()*11;
			u32_BlkAddr %= g_eMMCDrv.u32_SEC_COUNT;
			if(g_eMMCDrv.u32_SEC_COUNT - u32_BlkAddr
				< (TEST_BUFFER_SIZE>>eMMC_SECTOR_BYTECNT_BITS))
				u32_BlkAddr = g_eMMCDrv.u32_SEC_COUNT
				    - (TEST_BUFFER_SIZE>>eMMC_SECTOR_BYTECNT_BITS) - 1;

			if(u32_BlkAddr < (EMMC_RESERVED_BLK_FOR_MAP+EMMC_UBOOT_SIZE+EMMC_MPOOL_SIZE))
			{
                 u32_BlkAddr += EMMC_RESERVED_BLK_FOR_MAP+EMMC_UBOOT_SIZE+EMMC_MPOOL_SIZE;
			}
			// w
			u32_err = eMMC_CMD25_MIU(u32_BlkAddr, gau8_WBuf, TEST_BUFFER_SIZE>>eMMC_SECTOR_BYTECNT_BITS);
			if(eMMC_ST_SUCCESS != u32_err)
			{
				eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"\neMMC Err: w fail: %Xh, BlkAddr: %Xh \n", u32_err, u32_BlkAddr);
				eMMC_debug(eMMC_DEBUG_LEVEL,0,"eMMC: (%Xh x %Xh) blocks tested, stop \n",
				    u32_LoopCnt, g_eMMCDrv.u32_SEC_COUNT);
				eMMC_FCIE_ErrHandler_Stop();
			}

			// r & c
			u32_err = eMMC_CMD18_MIU(u32_BlkAddr, gau8_RBuf, TEST_BUFFER_SIZE>>eMMC_SECTOR_BYTECNT_BITS);
			if(eMMC_ST_SUCCESS != u32_err)
			{
				eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"\neMMC Err: r fail: %Xh, BlkAddr: %Xh \n", u32_err, u32_BlkAddr);
				eMMC_debug(eMMC_DEBUG_LEVEL,0,"eMMC: (%Xh x %Xh) blocks tested, stop \n",
				    u32_LoopCnt, g_eMMCDrv.u32_SEC_COUNT);
				eMMC_FCIE_ErrHandler_Stop();
			}
			u32_err = eMMC_ComapreData(gau8_WBuf, gau8_RBuf, TEST_BUFFER_SIZE);
	        if(eMMC_ST_SUCCESS != u32_err)
	        {
				eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,"\neMMC Err: c fail: %Xh, BlkAddr: %Xh \n", u32_err, u32_BlkAddr);
				eMMC_debug(eMMC_DEBUG_LEVEL,0,"eMMC: (%Xh x %Xh) blocks tested, stop \n",
				    u32_LoopCnt, g_eMMCDrv.u32_SEC_COUNT);
				eMMC_FCIE_ErrHandler_Stop();
	        }

			u32_LoopCnt += 1;

			if(((u32_LoopCnt*TEST_BUFFER_SIZE)&0x3FFFFFFF)==0)
			{
    			u32_Write_GB++;
				eMMC_debug(0,1,"Write %u GBytes\n", u32_Write_GB);
    			g_eMMCDrv.Life_Bytes_Cnt_t.u32_LIFETIME_TEST_RANDOM_GBytes = u32_Write_GB;
    			g_eMMCDrv.Life_Bytes_Cnt_t.u32_ChkSum=eMMC_ChkSum((U8*)&g_eMMCDrv.Life_Bytes_Cnt_t, sizeof(g_eMMCDrv.Life_Bytes_Cnt_t)-4);
    			memset(gau8_WBuf,0,eMMC_SECTOR_BYTECNT);
                memcpy(gau8_WBuf,(U8*)&g_eMMCDrv.Life_Bytes_Cnt_t,sizeof(g_eMMCDrv.Life_Bytes_Cnt_t));
    			u32_err = eMMC_CMD24_MIU(eMMC_LIFE_TEST_BYTE_CNT_BLK, gau8_WBuf);
    	        if(eMMC_ST_SUCCESS != u32_err)
    	        {
    		        eMMC_debug(0,1,"Err, CMD24_MIU, %Xh \n", u32_err);
    		        eMMC_FCIE_ErrHandler_Stop();
    	        }
			}
		}
	}

	return u32_err;
}


U32 eMMC_IPVerify_SDRDDR_AllClkTemp(void)
{
	U32 u32_err;
	U8  u8_SDRClkIdx;

	u32_err = eMMC_FCIE_ChooseSpeedMode();
	if(eMMC_ST_SUCCESS != u32_err){
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: ChooseSpeedMode fail: %Xh\n", u32_err);
		return u32_err;
	}
	eMMC_DumpTimingTable();
	u8_SDRClkIdx = 0;

	while(1)
	{
		#if 1
		//eMMC_hw_timer_delay(HW_TIMER_DELAY_1s);
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"\n");
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"======================================\n");
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"SDR\n");
		u32_err = eMMC_FCIE_EnableSDRMode();
	    if(eMMC_ST_SUCCESS != u32_err){
		    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: EnableSDRMode fail: %Xh\n", u32_err);
			eMMC_DumpDriverStatus();
		    return u32_err;
		}
		#ifndef IP_FCIE_VERSION_5
		eMMC_clock_setting(gau8_FCIEClkSel[u8_SDRClkIdx]);
		eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC Clk: %u %sHz\n",
			g_eMMCDrv.u32_ClkKHz>1000 ? g_eMMCDrv.u32_ClkKHz/1000 : g_eMMCDrv.u32_ClkKHz,
			g_eMMCDrv.u32_ClkKHz>1000 ? "M" : "K");
		u8_SDRClkIdx++;
		//u8_SDRClkIdx = u8_SDRClkIdx%(eMMC_FCIE_VALID_CLK_CNT-1);// skip 300KHz
		u8_SDRClkIdx = u8_SDRClkIdx%eMMC_FCIE_VALID_CLK_CNT;
        #endif
		eMMC_IPVerify_Main_Ex(eMMC_PATTERN_FFFFFFFF);
		eMMC_IPVerify_Main_Ex(eMMC_PATTERN_00000000);
		eMMC_IPVerify_Main_Ex(eMMC_PATTERN_000000FF);
	    eMMC_IPVerify_Main_Ex(eMMC_PATTERN_0000FFFF);
	    eMMC_IPVerify_Main_Ex(eMMC_PATTERN_00FF00FF);
	    eMMC_IPVerify_Main_Ex(eMMC_PATTERN_AA55AA55);

		eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_FFFFFFFF);
		eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_00000000);
		eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_000000FF);
	    eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_0000FFFF);
	    eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_00FF00FF);
	    eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_AA55AA55);
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"\n");
        #endif

		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"======================================\n");
		// [CAUTION]: switch to fast mode,  Brian said a fixed clock for emmcpll, do not switch emmcpll clock. [2013 Nov.. Napoli back]
		u32_err = eMMC_FCIE_ChooseSpeedMode();
	    if(eMMC_ST_SUCCESS != u32_err){
		    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: ChooseSpeedMode fail: %Xh\n", u32_err);
			eMMC_DumpDriverStatus();
		    return u32_err;
		}
		eMMC_IPVerify_Main_Ex(eMMC_PATTERN_FFFFFFFF);
		eMMC_IPVerify_Main_Ex(eMMC_PATTERN_00000000);
		eMMC_IPVerify_Main_Ex(eMMC_PATTERN_000000FF);
		eMMC_IPVerify_Main_Ex(eMMC_PATTERN_0000FFFF);
		eMMC_IPVerify_Main_Ex(eMMC_PATTERN_00FF00FF);
		eMMC_IPVerify_Main_Ex(eMMC_PATTERN_AA55AA55);

		eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_FFFFFFFF);
		eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_00000000);
		eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_000000FF);
	    eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_0000FFFF);
	    eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_00FF00FF);
	    eMMC_IPVerify_Main_Sg_Ex(eMMC_PATTERN_AA55AA55);
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"\n");
	}

	return eMMC_ST_SUCCESS;
}


#define eMMC_PWRCUT_DATA0      0x55
#define eMMC_PWRCUT_DATA1      0xAA
#define eMMC_PWRCUT_DATA_CNT   2

#define eMMC_PWRCUT_TEST_UNIT_BYTECNT    0x1000000
#define eMMC_PWRCUT_TEST_SPACE_BYTECNT   (16*eMMC_PWRCUT_TEST_UNIT_BYTECNT) // 256MB
#define eMMC_PWRCUT_TEST_UNIT_CNT        (eMMC_PWRCUT_TEST_SPACE_BYTECNT/eMMC_PWRCUT_TEST_UNIT_BYTECNT)

typedef eMMC_PACK0 struct _eMMC_PWRCUT_CTRLBLK {

    U32 u32_CheckSum;
	U8  u8_TargetUnit;
	U8  au8_TaegetData[eMMC_PWRCUT_TEST_UNIT_CNT];

} eMMC_PACK1 eMMC_PWRCUT_CTRLBLK_t;

void eMMC_DumpsPwrCutCtrlBlk(eMMC_PWRCUT_CTRLBLK_t *pCtrlBlk_t)
{
	U32 u32_i;

	eMMC_debug(eMMC_DEBUG_LEVEL, 0,
		"eMMC: CheckSum: %Xh\n", pCtrlBlk_t->u32_CheckSum);
	eMMC_debug(eMMC_DEBUG_LEVEL, 0,
		"eMMC: TargetUnit: %Xh\n", pCtrlBlk_t->u8_TargetUnit);

	eMMC_debug(eMMC_DEBUG_LEVEL, 0, "  Target Data: ");
	for(u32_i=0; u32_i<eMMC_PWRCUT_TEST_UNIT_CNT; u32_i++)
	{
		if(0 == (u32_i & 0xF))
			eMMC_debug(eMMC_DEBUG_LEVEL, 0, "\n");

		eMMC_debug(eMMC_DEBUG_LEVEL, 0, " %Xh",
			pCtrlBlk_t->au8_TaegetData[u32_i]);
	}
	eMMC_debug(eMMC_DEBUG_LEVEL, 0, "\n");
}

U32 eMMC_CheckPwrCutCtrlBlk(U8* u8_DataBuf, U32 u32_BlkAddr)
{
	U32 u32_err;
	eMMC_PWRCUT_CTRLBLK_t *pCtrlBlk_t;

	u32_err = eMMC_ReadData(u8_DataBuf, eMMC_SECTOR_512BYTE, u32_BlkAddr);
	if(u32_err != eMMC_ST_SUCCESS)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
			"eMMC Err: read CtrlBlk, Err:%x\n", u32_err);
	    return u32_err;
    }

	pCtrlBlk_t = (eMMC_PWRCUT_CTRLBLK_t*)u8_DataBuf;

	if(pCtrlBlk_t->u32_CheckSum !=
		eMMC_ChkSum(u8_DataBuf+4, sizeof(eMMC_PWRCUT_CTRLBLK_t)-4))
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
			"eMMC Err: read CtrlBlk, CheckSum fail\n");
	    return eMMC_ST_ERR_CHKSUM;
	}

	return eMMC_ST_SUCCESS;
}


U32 eMMCTest_PwrCut_InitData(U8* u8_DataBuf, U32 u32_BlkStartAddr)
{
	U32 u32_i, u32_j;
	U32 u32_err = eMMC_ST_SUCCESS;
	eMMC_PWRCUT_CTRLBLK_t CtrlBlk_t;

    struct mmc *mmc = find_mmc_device(0);
    mmc_slc_mode(mmc, 0, 1); // enable reliable

	// ---------------------------------------
	CtrlBlk_t.u8_TargetUnit = 0xFF;

	for(u32_i=0; u32_i<eMMC_PWRCUT_TEST_UNIT_CNT; u32_i++)
		CtrlBlk_t.au8_TaegetData[u32_i] = eMMC_PWRCUT_DATA0;

	CtrlBlk_t.u32_CheckSum = eMMC_ChkSum((U8*)&CtrlBlk_t.u8_TargetUnit,
		sizeof(eMMC_PWRCUT_CTRLBLK_t)-4);

	u32_err = eMMC_WriteData((U8*)&CtrlBlk_t, eMMC_SECTOR_512BYTE, u32_BlkStartAddr);
	if(u32_err != eMMC_ST_SUCCESS)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
			"eMMC Err: write CtrlBlk, Err:%x\n", u32_err);
	    return u32_err;
    }

	// ---------------------------------------
	u32_BlkStartAddr+=1; // skip Ctrl Blk

    memset(u8_DataBuf, eMMC_PWRCUT_DATA0, eMMC_PWRCUT_TEST_UNIT_BYTECNT);
	eMMC_debug(eMMC_DEBUG_LEVEL,1,"Reliable write ...\n");

	for(u32_i=0; u32_i<eMMC_PWRCUT_TEST_UNIT_CNT; u32_i++)
	{
		u32_err = eMMC_WriteData(u8_DataBuf, eMMC_PWRCUT_TEST_UNIT_BYTECNT,
			u32_BlkStartAddr + u32_i*(eMMC_PWRCUT_TEST_UNIT_BYTECNT>>9));
	    if(u32_err != eMMC_ST_SUCCESS)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
				"eMMC Err: Reliable write Fail @ %u unit, Err:%x\n", u32_i, u32_err);
	        return u32_err;
        }
		eMMC_debug(eMMC_DEBUG_LEVEL, 0,"%03u%% \r", (u32_i+1)*100/eMMC_PWRCUT_TEST_UNIT_CNT);
	}

	// ---------------------------------------
	eMMC_debug(eMMC_DEBUG_LEVEL, 0,"\ncheck ...\n");

	if(eMMC_ST_SUCCESS != eMMC_CheckPwrCutCtrlBlk(u8_DataBuf, u32_BlkStartAddr-1))
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: read CtrlBlk fail\n");
	    return eMMC_ST_ERR_CHKSUM;
	}

	for(u32_i=0; u32_i<eMMC_PWRCUT_TEST_UNIT_CNT; u32_i++)
	{
		u32_err = eMMC_ReadData(u8_DataBuf, eMMC_PWRCUT_TEST_UNIT_BYTECNT,
			u32_BlkStartAddr + u32_i*(eMMC_PWRCUT_TEST_UNIT_BYTECNT>>9));
	    if(u32_err != eMMC_ST_SUCCESS)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
				"eMMC Err: Reliable write Fail @ %u unit, Err:%x\n", u32_i, u32_err);
	        return u32_err;
        }

		for (u32_j=0; u32_j < eMMC_PWRCUT_TEST_UNIT_BYTECNT; u32_j++)
	    {
		    if (u8_DataBuf[u32_j] != eMMC_PWRCUT_DATA0)
	 	    {
			    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
					"eMMC Err: Data Mismatch:  Blk:%Xh  ByteIdx:%Xh  ByteVal:%Xh \n",
					u32_BlkStartAddr+u32_i*(eMMC_PWRCUT_TEST_UNIT_BYTECNT>>9)+(u32_j>>9),
					u32_j & 0x1FF, u8_DataBuf[u32_j]);
			    return eMMC_ST_ERR_DATA_MISMATCH;
		    }
	    }

		eMMC_debug(eMMC_DEBUG_LEVEL, 0,"%03u%% \r", (u32_i+1)*100/eMMC_PWRCUT_TEST_UNIT_CNT);
	}

	eMMC_debug(eMMC_DEBUG_LEVEL,0,"\n init success\n");
	return eMMC_ST_SUCCESS;
}


U32 eMMCTest_PwrCut_Test(U8* u8_DataBuf, U32 u32_BlkStartAddr)
{
	U32 u32_i, u32_j;
	U32 u32_err = eMMC_ST_SUCCESS;
	U32 u32_T0, u32_BlkAddr;
	eMMC_PWRCUT_CTRLBLK_t CtrlBlk_t;

	eMMC_hw_timer_start();

	// ---------------------------------------
	eMMC_debug(eMMC_DEBUG_LEVEL, 0,"\n eMMC PwrCut Test, checking ...\n");

	// get Ctrl Blk
	if(eMMC_ST_SUCCESS != eMMC_CheckPwrCutCtrlBlk(u8_DataBuf, u32_BlkStartAddr))
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR, 1, "eMMC Err: read CtrlBlk fail\n");
		//return eMMC_ST_ERR_CHKSUM;
		while(1);
	}
	memcpy(&CtrlBlk_t, u8_DataBuf, sizeof(eMMC_PWRCUT_CTRLBLK_t));

	// check data
	u32_BlkStartAddr++;

	for(u32_i=0; u32_i<eMMC_PWRCUT_TEST_UNIT_CNT; u32_i++)
	{
		u32_err = eMMC_ReadData(u8_DataBuf, eMMC_PWRCUT_TEST_UNIT_BYTECNT,
			u32_BlkStartAddr + u32_i*(eMMC_PWRCUT_TEST_UNIT_BYTECNT>>9));
	    if(u32_err != eMMC_ST_SUCCESS)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,
				"\neMMC Err: Reliable write Fail @ %u unit, Err:%x\n", u32_i, u32_err);
	        //return u32_err;
	        while(1);
        }

		if(u32_i == CtrlBlk_t.u8_TargetUnit){
		for (u32_j=0; u32_j < eMMC_PWRCUT_TEST_UNIT_BYTECNT; u32_j++)
	    {
		    if (u8_DataBuf[u32_j]!=eMMC_PWRCUT_DATA0 &&
				u8_DataBuf[u32_j]!=eMMC_PWRCUT_DATA1)
	 	    {
			    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,
					"\neMMC Err: Target Data Mismatch:  Unit:%Xh  "
					"Blk: %Xh + %Xh x %Xh + %Xh = %Xh, ByteIdx:%Xh  ByteVal:%Xh \n",
					u32_i,
					u32_BlkStartAddr, u32_i, (eMMC_PWRCUT_TEST_UNIT_BYTECNT>>9), (u32_j>>9),
					u32_BlkStartAddr+u32_i*(eMMC_PWRCUT_TEST_UNIT_BYTECNT>>9)+(u32_j>>9),
					u32_j & 0x1FF, u8_DataBuf[u32_j]);
				eMMC_DumpsPwrCutCtrlBlk(&CtrlBlk_t);
				eMMC_dump_mem(&u8_DataBuf[u32_j]-0x10, 0x30);
			    //return eMMC_ST_ERR_DATA_MISMATCH;
			    while(1);
		    }
	    }
		// recover TargetUnit
		u32_BlkAddr = u32_BlkStartAddr +
			CtrlBlk_t.u8_TargetUnit *
			(eMMC_PWRCUT_TEST_UNIT_BYTECNT>>eMMC_SECTOR_512BYTE_BITS);
		memset(u8_DataBuf, CtrlBlk_t.au8_TaegetData[CtrlBlk_t.u8_TargetUnit], eMMC_PWRCUT_TEST_UNIT_BYTECNT);
        u32_err = eMMC_WriteData(u8_DataBuf, eMMC_PWRCUT_TEST_UNIT_BYTECNT, u32_BlkAddr);
		if(u32_err != eMMC_ST_SUCCESS)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: write TargetUnit fail, Err:%x\n",u32_err);
			//return u32_err;
			while(1);
		}}
		else{
		for (u32_j=0; u32_j < eMMC_PWRCUT_TEST_UNIT_BYTECNT; u32_j++)
	    {
		    if (u8_DataBuf[u32_j]!=CtrlBlk_t.au8_TaegetData[u32_i])
	 	    {
			    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,0,
					"\neMMC Err: Data Mismatch:  Unit:%Xh  "
					"Blk: %Xh + %Xh x %Xh + %Xh = %Xh, ByteIdx:%Xh  ByteVal:%Xh \n",
					u32_i,
					u32_BlkStartAddr, u32_i, (eMMC_PWRCUT_TEST_UNIT_BYTECNT>>9), (u32_j>>9),
					u32_BlkStartAddr+u32_i*(eMMC_PWRCUT_TEST_UNIT_BYTECNT>>9)+(u32_j>>9),
					u32_j & 0x1FF, u8_DataBuf[u32_j]);
				eMMC_DumpsPwrCutCtrlBlk(&CtrlBlk_t);
				eMMC_dump_mem(&u8_DataBuf[u32_j]-0x10, 0x30);
			    //return eMMC_ST_ERR_DATA_MISMATCH;
				while(1);
		    }
	    }}

		eMMC_debug(eMMC_DEBUG_LEVEL, 0,"%03u%% \r", (u32_i+1)*100/eMMC_PWRCUT_TEST_UNIT_CNT);
	}

	eMMC_debug(eMMC_DEBUG_LEVEL, 0,"   ok\n");

    // ---------------------------------------
	while(1)
    {
		u32_T0 = eMMC_hw_timer_tick();
		CtrlBlk_t.u8_TargetUnit = u32_T0 % eMMC_PWRCUT_TEST_UNIT_CNT;
		CtrlBlk_t.au8_TaegetData[CtrlBlk_t.u8_TargetUnit] ^= 0xFF;
		CtrlBlk_t.u32_CheckSum = eMMC_ChkSum((U8*)&CtrlBlk_t.u8_TargetUnit,
		    sizeof(eMMC_PWRCUT_CTRLBLK_t)-4);
		u32_err = eMMC_WriteData((U8*)&CtrlBlk_t, eMMC_SECTOR_512BYTE, u32_BlkStartAddr-1);
	    if(u32_err != eMMC_ST_SUCCESS)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,
			    "eMMC Err: write CtrlBlk, Err:%x\n", u32_err);
	        return u32_err;
        }

		u32_BlkAddr = u32_BlkStartAddr +
			CtrlBlk_t.u8_TargetUnit *
			(eMMC_PWRCUT_TEST_UNIT_BYTECNT>>eMMC_SECTOR_512BYTE_BITS);
		memset(u8_DataBuf, CtrlBlk_t.au8_TaegetData[CtrlBlk_t.u8_TargetUnit], eMMC_PWRCUT_TEST_UNIT_BYTECNT);

		eMMC_debug(eMMC_DEBUG_LEVEL, 0 ,"writing... %02Xh %08Xh %02Xh\n",
			CtrlBlk_t.u8_TargetUnit, u32_BlkAddr, CtrlBlk_t.au8_TaegetData[CtrlBlk_t.u8_TargetUnit]);

		u32_err = eMMC_WriteData(u8_DataBuf, eMMC_PWRCUT_TEST_UNIT_BYTECNT, u32_BlkAddr);
		//u32_err = eMMC_CMD25(u32_BlkAddr, u8_DataBuf, eMMC_PWRCUT_TEST_UNIT_BYTECNT>>9);
		if(u32_err != eMMC_ST_SUCCESS)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: write Fail, Err:%x\n",u32_err);
			return u32_err;
		}
    }

	return eMMC_ST_SUCCESS;
}


U32 eMMCTest_PwrCut_Test2(U8* u8_DataBuf, U32 u32_BlkStartAddr)
{
	U32 u32_blk_cnt ,u32_i,u32_test_loop;
	U32 u32_err = eMMC_ST_SUCCESS;
	U8 data;

    struct mmc *mmc = find_mmc_device(0);

    if (!mmc)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: no mmc device at slot\n");
        return 1;
    }

    if(!mmc->has_init)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Need mmc init first!\n");
        return 1;
    }


    eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC Info: Reliable write ext_csd[167] 0x%02x, ext_csd[166] 0x%02x\n", mmc->ext_csd[167], mmc->ext_csd[166]);

	if (mmc->reliable_write == 1)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC Info: Reliable write is supported but unconfigured\n");

        u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, EXT_CSD_WR_REL_SET, 0x1f);
	    if(eMMC_ST_SUCCESS != u32_err)
		    return u32_err;

        // complete the partition configuration
        u32_err = eMMC_ModifyExtCSD(eMMC_ExtCSD_WByte, EXT_CSD_PARTITION_SETTING_COMPLETED, 0x01);
	    if(eMMC_ST_SUCCESS != u32_err)
		    return u32_err;

        mmc->reliable_write = 2;

        while (1)
            eMMC_debug(eMMC_DEBUG_LEVEL,1,"eMMC Info: Please reset the board!!!!!! Reliable write would be active after reset!!!!!!\n");
	}
	else if (mmc->reliable_write == 2)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL, 1,"eMMC Info: Reliable write is supported and has been configured\n");
	}
	else if (mmc->reliable_write == 0)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Info: Reliable write is unsupported\n");
		while(1);
	}

	u32_blk_cnt = (48 *1024 * 1024)>>eMMC_SECTOR_512BYTE_BITS;

	memset(u8_DataBuf, 0, (48 *1024 * 1024));
	//step1
	u32_err = eMMC_ReadData(u8_DataBuf, u32_blk_cnt<<eMMC_SECTOR_512BYTE_BITS, u32_BlkStartAddr);
	if(u32_err != eMMC_ST_SUCCESS)
	{
		eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Reliable read Fail,Err:%x\n",u32_err);
		return 1;
	}

	data = u8_DataBuf[0];
	for (u32_i=0; u32_i < (48 *1024 * 1024); u32_i++)
	{
		if (u8_DataBuf[u32_i] != data)
		{
			eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Data Mismatch: Blk:%X  Byte:%X  buf:%X data:%x\n", u32_i>>9,u32_i & 0x1FF, u8_DataBuf[u32_i], data);
			return eMMC_ST_ERR_DATA_MISMATCH;
		}
	}
	eMMC_debug(eMMC_DEBUG_LEVEL,1,"Reliable read compare success\n");

	data = (U8)eMMC_hw_timer_tick();//rRand((unsigned int)data, (unsigned int)(u8_DataBuf - data));
   	memset(u8_DataBuf, data, (48 *1024 * 1024));
	u32_test_loop=0;

    u32_err = eMMC_WriteData(u8_DataBuf, (48 *1024 * 1024), u32_BlkStartAddr);
	if(u32_err != eMMC_ST_SUCCESS)
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Reliable write Fail,Err:%x\n",u32_err);
	    return u32_err;
    }
	eMMC_debug(eMMC_DEBUG_LEVEL,1,"Reliable write Success\n");

	eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"power cut test loop star\n");

	u32_BlkStartAddr += ((48 *1024 * 1024) >> eMMC_SECTOR_512BYTE_BITS);
	u32_blk_cnt = (4 *1024 * 1024)>>eMMC_SECTOR_512BYTE_BITS;
    while(1)
    {
		//step3
		for (u32_i=0; u32_i < u32_blk_cnt; u32_i++)
		{
			u32_err = eMMC_WriteData(u8_DataBuf+(u32_i<<eMMC_SECTOR_512BYTE_BITS), 512, u32_BlkStartAddr+u32_i);
			if(u32_err != eMMC_ST_SUCCESS)
			{
				eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: Reliable write Fail,Err:%x\n",u32_err);
				return u32_err;
			}
		}

		u32_test_loop++;
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"Test_loop:%X\n",u32_test_loop);
    }

	return eMMC_ST_SUCCESS;
}
#if defined(DMA_MBOOT_ADDR)
U32 eMMCTest_Read_disturbance(void)
{
    emmc_partition_t    mpart;
    U32 u32_err=0;
    U32 u32_read_allcnt=0;
    U32 u32_i;
    U32 *pu32_R=(U32*)DMA_R_ADDR;
    U32 *u32_mboot_addr = (U32*)DMA_MBOOT_ADDR;
    U32 u32_mboot_start=0,u32_mboot_size=0;

    for(u32_i=1;u32_i<= EMMC_RESERVED_FOR_MAP_V2;u32_i++)
    {
        if(eMMC_ReadData((U8*)(&mpart), eMMC_SECTOR_512BYTE, u32_i) == eMMC_ST_SUCCESS)
        {
            if(strcmp("MBOOT", (const char *)mpart.name)==0)
            {
                u32_mboot_start = mpart.start_block;
                u32_mboot_size = mpart.block_count<<9;
                u32_err = eMMC_ReadData((U8*)u32_mboot_addr, u32_mboot_size, u32_mboot_start);
                if(u32_err != eMMC_ST_SUCCESS)
                {
                    eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: read Fail,Err:%x\n",u32_err);
                    return u32_err;
                }
                printf("** Find MBOOT **\n");
            }
        }
        else
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: read Fail,Err:%x\n",u32_err);
            return u32_err;
        }
        if((u32_mboot_start !=0) && (u32_mboot_size !=0))
            break;
    }
    if((u32_mboot_start ==0) && (u32_mboot_size ==0))
    {
        eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"No mboot partition\n");
        return eMMC_ST_SUCCESS;
    }

    while(1)
    {
        u32_err = eMMC_ReadData((U8*)pu32_R, u32_mboot_size, u32_mboot_start);
        if(u32_err != eMMC_ST_SUCCESS)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: read Fail,Err:%x\n",u32_err);
            return u32_err;
        }

        u32_err = eMMC_ComapreData((U8*)pu32_R, (U8*)u32_mboot_addr, u32_mboot_size);
        if(u32_err != eMMC_ST_SUCCESS)
        {
            eMMC_debug(eMMC_DEBUG_LEVEL_ERROR,1,"eMMC Err: mboot mismatch,Err:%x\n",u32_err);
            return u32_err;
        }
        u32_read_allcnt++;
        if((u32_read_allcnt%2000)==0)
            eMMC_debug(0,0,"eMMC read cnt:%8Xh\n",u32_read_allcnt);
    }

    return eMMC_ST_SUCCESS;
}
#endif
#endif
#endif
