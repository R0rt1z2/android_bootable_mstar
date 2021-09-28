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

#include "drvNAND.h"
#include "drvNAND_device.h"
#include "drvNAND_utl.h"

#if defined(__VER_UNFD_FTL__) && __VER_UNFD_FTL__

#if UNFD_FTL_WL
static U16 gu16_Erase_Counter[MAX_LUT_CNT*1024];
#endif


static U32  nand_check_IfInitBB(U16 u16_PBA, U16 *pu16_InitBBArray, U16 u16_Cnt);
//static void nand_setup_FreeBlocks(void);
static void nand_ECC_ErrorHandler(U16 u16_PBA);
static U32  nand_format_DataBlk(U16 u16_PBA, U16 u16_LBA);
static U32  nand_init_parameters(void);
static U32  nand_init_parameters_Ex(void);
static U16  nand_get_FreeBlk_Zone0(void);
static U16  nand_get_FreeBlk_Zone1(void);
static U32  nand_check_FBExist(U16 *pu16_FBIdx, U16 u16_PBA, U16 u16_FBSlotCnt, U8 u8_FBCnt, U8 u8_FBHead);
static void nand_set_FreeBlk(U16 u16_PBA);
static void nand_erase_and_set_FreeBlk(U16 u16_PBA);
static U32  nand_build_LUT0(void);
static void nand_build_LUT1(void);
static U16  nand_DuplicatePBAHandler(U16 u16_PBA0, U16 u16_PBA1, U16 u16_LBA);
static U32  nand_search_RootBlk(void);
static void nand_dump_DiskInfo(U8 u8_DiskCnt);
static void nand_dump_FreeBlk(U8 u8_ZoneID);
static U32  nand_copy_data(U16 u16_ToBlk, U16 u16_SectorInBlk, U16 u16_FromBlk, U16 u16_SectorCnt);
static U16  nand_FindFreePage(U16 u16_PBA);
#if UNFD_FTL_WBQ
static U32  nand_WBQ_WriteBack(U8 u8_WBQIdx, U16 u16_SectorInBlkCopyEnd);
static void nand_WBQ_UpdatePPB(void);
static void nand_dump_WBQ(void);
static void nand_reset_WBQs(void);
#endif
#if UNFD_FTL_FMAP
static U32  nand_FM_WriteFlow(U32 u32_StartSectorIdx, U32 u32_SectorCnt, U32 u32_RWBufferAddress);
static U32  nand_FM_ReadFlow(U32 u32_StartSectorIdx, U32 u32_SectorCnt, U32 u32_RWBufferAddress);
void nand_dump_FMap(void);
static void nand_FM_MoveBlk(U8 u8_i, U8 u8_j);
static void nand_FM_Init(void);
#endif
#if UNFD_FTL_WL || UNFD_FTL_RD
static void nand_validate_WLRD_PBA(U16 u16_PBA, U8 u8_ActID);
#endif
#if UNFD_FTL_RD
static void nand_RD_ErrorHandler(void);
static void nand_RD_UpdateRDB(void);
static void nand_RD_WriteRDBPage(U8 u8_RD_Status);
void nand_ReadDisturbance_ftl(void);
#endif

#define UNFD_FTL_CODE_LEVEL_FOR_SOURCE_INSIGHT


static U32  nand_WritePages_Raw(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
static U32  nand_ReadPages_Raw(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt);
//static U32  nand_WriteSectors_Raw(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);
static U32  nand_ReadSectors_Raw(U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt);


#define PREREAD_CHECK 0
#if defined(PREREAD_CHECK) && PREREAD_CHECK
U8 ftl_au8_PageBuf[0x2000];
U8 ftl_au8_SpareBuf[1664];
#endif

static U32  nand_EraseBlk(U32 u32_PhyRowIdx)
{
    #if defined(ASSERT_FTL_ACCESS_BLK0) && ASSERT_FTL_ACCESS_BLK0
    if(u32_PhyRowIdx == 0)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"Error: Row: %lX \n", u32_PhyRowIdx);
        nand_die();
    }
    #endif

    return NC_EraseBlk(u32_PhyRowIdx);
}

static U32  nand_WritePages_Raw(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    int i;
    U16 u16_SectorSpareFree = pNandDrv->u16_SectorSpareByteCnt - pNandDrv->u16_ECCCodeByteCnt;
    U8 u8_Length = UNFD_FTL_SPARE_BYTE_CNT;
    U8 u8_SpareBufTmp[UNFD_FTL_SPARE_BYTE_CNT];
    U8 u8_WriteLength = 0;
	U32 u32_Err, u32_i;
	
    #if defined(ASSERT_FTL_ACCESS_BLK0) && ASSERT_FTL_ACCESS_BLK0
    if(u32_PhyRowIdx == 0)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"Error: Row: %lX \n", u32_PhyRowIdx);
        nand_die();
    }
    #endif
    #if defined(PREREAD_CHECK) && PREREAD_CHECK
        for(u32_i = 0; u32_i < u32_PageCnt; u32_i ++)
        {
            u32_Err = NC_ReadPages(u32_PhyRowIdx + u32_i, ftl_au8_PageBuf, ftl_au8_SpareBuf, 1);
            if(u32_Err != UNFD_ST_SUCCESS)
                return u32_Err;
            if(drvNAND_CheckAll0xFF(ftl_au8_PageBuf, pNandDrv->u16_PageByteCnt) == 0)
			{
				nand_debug(0,1, "write twice row %X\n", u32_PhyRowIdx + u32_i);
				dump_mem(ftl_au8_PageBuf, 0x10);
                return UNFD_ST_ERR_W_FAIL;
			}
            if(drvNAND_CheckAll0xFF(ftl_au8_SpareBuf, pNandDrv->u16_SpareByteCnt) == 0)
			{
				nand_debug(0,1, "write twice row %X\n", u32_PhyRowIdx + u32_i);
                return UNFD_ST_ERR_W_FAIL;
			}
        }
    #endif

    if (u16_SectorSpareFree < UNFD_FTL_SPARE_BYTE_CNT && (NULL != pu8_SpareBuf))
    {
        memcpy(u8_SpareBufTmp, pu8_SpareBuf, UNFD_FTL_SPARE_BYTE_CNT);

        for (i = 0; u8_Length > 0; i++)
        {
            u8_WriteLength = u8_Length > u16_SectorSpareFree? u16_SectorSpareFree : u8_Length;

            memcpy(&pu8_SpareBuf[i*pNandDrv->u16_SectorSpareByteCnt],&u8_SpareBufTmp[i*u8_WriteLength], u8_WriteLength);

            u8_Length -= u8_WriteLength;
        }
    }

    return NC_WritePages(u32_PhyRowIdx, pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);
}

// [CAUTION]: pu8_SpareBuf takes the last page's first sector's spare data
static U32  nand_ReadPages_Raw(U32 u32_PhyRowIdx, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_PageCnt)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U32 u32_UnfdErrCode = UNFD_ST_ERR_NOT_READY;
    int i;
    U16 u16_SectorSpareFree = pNandDrv->u16_SectorSpareByteCnt - pNandDrv->u16_ECCCodeByteCnt;
    U8 u8_Length = UNFD_FTL_SPARE_BYTE_CNT;
    U8 u8_ReadLength = 0;

    #if defined(ASSERT_FTL_ACCESS_BLK0) && ASSERT_FTL_ACCESS_BLK0
    if(u32_PhyRowIdx == 0)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"Error: Row: %lX \n", u32_PhyRowIdx);
        nand_die();
    }
    #endif

    if (u16_SectorSpareFree < UNFD_FTL_SPARE_BYTE_CNT)
    {
        u32_UnfdErrCode = NC_ReadPages(u32_PhyRowIdx, pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);

        if (UNFD_ST_SUCCESS != u32_UnfdErrCode )
        {
            return u32_UnfdErrCode;
        }
        else if (NULL != pu8_SpareBuf)
        {	
            u8_ReadLength = u8_Length > u16_SectorSpareFree? u16_SectorSpareFree : u8_Length;
            u8_Length -= u8_ReadLength;

            for (i=1; u8_Length>0; i++)
            {		
                u8_ReadLength = u8_Length > u16_SectorSpareFree? u16_SectorSpareFree : u8_Length;

                memcpy(&pu8_SpareBuf[i*u16_SectorSpareFree], &pu8_SpareBuf[i*pNandDrv->u16_SectorSpareByteCnt], u8_ReadLength);

                u8_Length -= u8_ReadLength;
            }
        }

        return u32_UnfdErrCode;
    }

    return NC_ReadPages(u32_PhyRowIdx, pu8_DataBuf, pu8_SpareBuf, u32_PageCnt);
}


#if 0
static U32  nand_WriteSectors_Raw
(
    U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt
)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    int i;
    U16 u16_SectorSpareFree = pNandDrv->u16_SectorSpareByteCnt - pNandDrv->u16_ECCCodeByteCnt;
    U8 u8_Length = UNFD_FTL_SPARE_BYTE_CNT;
    U8 u8_SpareBufTmp[UNFD_FTL_SPARE_BYTE_CNT];
	
	
    #if defined(ASSERT_FTL_ACCESS_BLK0) && ASSERT_FTL_ACCESS_BLK0
    if(u32_PhyRowIdx == 0)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"Error: Row: %X \n", u32_PhyRowIdx);
        nand_die();
    }
    #endif

    if (u16_SectorSpareFree < UNFD_FTL_SPARE_BYTE_CNT && (NULL != pu8_SpareBuf))
    {
        memcpy(u8_SpareBufTmp, pu8_SpareBuf, UNFD_FTL_SPARE_BYTE_CNT);
					
        for (i = 0; u8_Length > 0; i++)
        {
            memcpy(&pu8_SpareBuf[i*pNandDrv->u16_SectorSpareByteCnt], &u8_SpareBufTmp[i*u16_SectorSpareFree], u16_SectorSpareFree);

            u8_Length -= u16_SectorSpareFree;

            if (i >= 4)
                break;
        }

        return NC_WritePages(u32_PhyRowIdx, pu8_DataBuf, pu8_SpareBuf, 1);
    }
	
    return NC_WriteSectors(u32_PhyRowIdx, u8_SectorInPage, pu8_DataBuf, pu8_SpareBuf, u32_SectorCnt);
}
#endif

static U32  nand_ReadSectors_Raw
(
    U32 u32_PhyRowIdx, U8 u8_SectorInPage, U8 *pu8_DataBuf, U8 *pu8_SpareBuf, U32 u32_SectorCnt
)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U32 u32_UnfdErrCode = UNFD_ST_ERR_NOT_READY;
    int i;
    U16 u16_SectorSpareFree = pNandDrv->u16_SectorSpareByteCnt - pNandDrv->u16_ECCCodeByteCnt;
    U8 u8_Length = UNFD_FTL_SPARE_BYTE_CNT;
    U8 u8_ReadLength = 0;

    #if defined(ASSERT_FTL_ACCESS_BLK0) && ASSERT_FTL_ACCESS_BLK0
    if(u32_PhyRowIdx == 0)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"Error: Row: %X \n", u32_PhyRowIdx);
        nand_die();
    }
    #endif

    if (u16_SectorSpareFree < UNFD_FTL_SPARE_BYTE_CNT && (0 == u8_SectorInPage))
    {
        u32_UnfdErrCode = NC_ReadPages(u32_PhyRowIdx, pu8_DataBuf, pu8_SpareBuf, 1);	

        if (UNFD_ST_SUCCESS != u32_UnfdErrCode )
        {		
            dump_nand_driver(pNandDrv);

            return u32_UnfdErrCode;
        }
        else if (NULL != pu8_SpareBuf)
        {	
            u8_ReadLength = u8_Length > u16_SectorSpareFree? u16_SectorSpareFree : u8_Length;
            u8_Length -= u8_ReadLength;

            for (i=1; u8_Length>0; i++)
            {		
                u8_ReadLength = u8_Length > u16_SectorSpareFree? u16_SectorSpareFree : u8_Length;

                memcpy(&pu8_SpareBuf[i*u16_SectorSpareFree], &pu8_SpareBuf[i*pNandDrv->u16_SectorSpareByteCnt], u8_ReadLength);

                u8_Length -= u8_ReadLength;
            }
        }

        return u32_UnfdErrCode;
    }

    return NC_ReadSectors_Retry(u32_PhyRowIdx, u8_SectorInPage, pu8_DataBuf, pu8_SpareBuf, u32_SectorCnt);
}

static void nand_ECC_ErrorHandler(U16 u16_ErrPBA)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
    U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
    volatile U16 u16_FreePBA=0xFFFF, u16_i;
    U32 u32_SrcRow, u32_DestRow, u32_Err;

    u32_SrcRow = u16_ErrPBA << pNandDrv->u8_BlkPageCntBits;

    LABEL_START_MOVE:
    if(u16_ErrPBA >= pNandDrv->u16_FirstPBA_Zone1)
        u16_FreePBA = nand_get_FreeBlk_Zone1();
    else if(u16_ErrPBA >= pNandDrv->u16_FirstPBA_Zone0)
        u16_FreePBA = nand_get_FreeBlk_Zone0();
    else
        nand_die();

    u32_DestRow = u16_FreePBA << pNandDrv->u8_BlkPageCntBits;

    for(u16_i=0; u16_i<pNandDrv->u16_BlkPageCnt; u16_i++)
    {
        nand_ReadPages_Raw(u32_SrcRow + u16_i, au8_PageBuf, au8_SpareBuf, 1);
        u32_Err = nand_WritePages_Raw(u32_DestRow + u16_i, au8_PageBuf, au8_SpareBuf, 1);
        if(UNFD_ST_SUCCESS != u32_Err)
        {
            if(UNFD_ST_ERR_W_FAIL == u32_Err)
            {
                drvNAND_MarkBadBlk(u16_FreePBA);
                goto LABEL_START_MOVE;
            }
            else
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Err: may be HW (PCB) error around NAND: %lX\n", u32_Err);
                nand_die();
            }
        }
    }

    drvNAND_MarkBadBlk(u16_ErrPBA);
    nand_die(); // trigger assert to re-init
}

#if UNFD_FTL_WL || UNFD_FTL_RD
// call only before WL or RD, to check if the PBA valid for WL or RD.
static void nand_validate_WLRD_PBA(U16 u16_PBA, U8 u8_ActID)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8  u8_head, u8_cnt, u8_last, u8_i;
    U16 u16_i, *pu16_buf;
    U32 u32_Ret;

    //---------------------------
    // check - FreeQueue
    if(u16_PBA < pNandDrv->u16_FirstPBA_Zone1)
    {
        u8_head = pNandDrv->u8_FBHead_0; // zone_0
        u8_cnt = pNandDrv->u8_FBCnt_0;
        u8_last = pNandDrv->u8_FBMaxCnt_0;
        pu16_buf = pNandDrv->au16_FBIdx_0;
    }
    else
    {
        u8_head = pNandDrv->u8_FBHead_1; // zone_1
        u8_cnt = pNandDrv->u8_FBCnt_1;
        u8_last = pNandDrv->u8_FBMaxCnt_1;
        pu16_buf = pNandDrv->au16_FBIdx_1;
    }

    for(u16_i=0; u16_i<u8_cnt; u16_i++)
    {
        if(u16_PBA == pu16_buf[u8_head])
        {
            nand_debug(0,1,"WLRD_PBA, Free PBA: %X \n", u16_PBA);

            #if UNFD_FTL_RD
            if(UNFD_FTL_BLK_ID_RD == u8_ActID)
                pNandDrv->u16_RD_SrcPBA = 0;
            #endif

            return;
        }

        u8_head++;
        if(u8_head == u8_last)
            u8_head = 0;
    }

    //---------------------------
    // check - WBQ
    #if UNFD_FTL_WBQ
    for(u16_i=0; u16_i<MAX_WBQ_CNT; u16_i++)
    {
        if(u16_PBA == pNandDrv->aWBQ_t[u16_i].u16_OldPBA)
        {
            nand_debug(0,1,"WLRD_PBA, = OldPBA, PBA:%X \n", u16_PBA);

            #if UNFD_FTL_RD
            if(UNFD_FTL_BLK_ID_RD == u8_ActID)
                pNandDrv->u16_RD_SrcPBA = 0;
            #endif

            return;
        }
        else if(0 != pNandDrv->aWBQ_t[u16_i].u16_OldPBA &&
                u16_PBA == pNandDrv->aWBQ_t[u16_i].u16_NewPBA)
        {
            nand_debug(0,1,"WLRD_PBA, write-back: %X, PBA: %X \n", u16_i, u16_PBA);
            nand_FlushWBQ(u16_i);
            return;
        }
    }

    // check - PPB
    if(u16_PBA == pNandDrv->u16_PPB)
    {
        nand_debug(0,1,"hits PPB \n");

        #if UNFD_FTL_RD
        if(UNFD_FTL_BLK_ID_RD == u8_ActID)
            pNandDrv->u16_RD_SrcPBA = 0;
        #endif

        return;
    }
    #endif

    //---------------------------
    // check - FMAP
    #if UNFD_FTL_FMAP
    for(u16_i=0; u16_i<pNandDrv->u16_FM_LBACnt; u16_i++)
        for(u8_i=0; u8_i<FMAP_SPACE_FACTOR; u8_i++)
        {
            if(u16_PBA == pNandDrv->au16_FM_BL2P[u16_i][u8_i])
            {
                nand_debug(0,1,"hits FM: [%X][%X], PBA: %X \n", u16_i, u8_i, u16_PBA);

                #if UNFD_FTL_RD
                if(UNFD_FTL_BLK_ID_RD == u8_ActID)
                    pNandDrv->u16_RD_SrcPBA = 0;
                #endif

                return;
            }
        }
    #endif

    //---------------------------
    // check - RD
    #if UNFD_FTL_RD
    if(u16_PBA == pNandDrv->u16_RDB)
    {
        nand_debug(0,1,"hits RDB: %X \n", u16_PBA);
        if(UNFD_FTL_BLK_ID_RD == u8_ActID)
            pNandDrv->u16_RD_SrcPBA = 0;
        return;
    }
    #endif

    //---------------------------
    // check - WL
    #if UNFD_FTL_WL
    for(u8_i=0; u8_i<MAX_LUT_CNT; u8_i++)
        if(u16_PBA == pNandDrv->au16_WL_ECntPBA[u8_i])
        {
            nand_debug(0,1,"hits WL_ECntPBA: %X \n", u16_PBA);

            #if UNFD_FTL_RD
            if(UNFD_FTL_BLK_ID_RD == u8_ActID)
                pNandDrv->u16_RD_SrcPBA = 0;
            #endif

            return;
        }
    #endif

    //---------------------------
    // check - bad
    u32_Ret = nand_ReadPages_Raw(u16_PBA << pNandDrv->u8_BlkPageCntBits,
            pNandDrv->pu8_PageDataBuf, pNandDrv->pu8_PageSpareBuf, 1);
    if(UNFD_ST_SUCCESS != u32_Ret || 0xFF != pNandDrv->pu8_PageSpareBuf[0])
    {
        if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
            nand_ECC_ErrorHandler(u16_PBA);

        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"is a bad blk: %X\n", u16_PBA);
        drvNAND_MarkBadBlk(u16_PBA);

        #if UNFD_FTL_RD
        if(UNFD_FTL_BLK_ID_RD == u8_ActID)
            pNandDrv->u16_RD_SrcPBA = 0;
        #endif
        return;
    }
    // check - free blk
    if(0xFF == drvNAND_CheckAll0xFF(pNandDrv->pu8_PageSpareBuf,
                pNandDrv->u16_SectorSpareByteCnt - pNandDrv->u16_ECCCodeByteCnt))
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"is a free blk: %X\n", u16_PBA);

        #if UNFD_FTL_RD
        if(UNFD_FTL_BLK_ID_RD == u8_ActID)
            pNandDrv->u16_RD_SrcPBA = 0;
        #endif
        return;
    }

}
#endif


#if UNFD_FTL_RD
static void nand_RD_ErrorHandler(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    RD_BLK_DATA_t *pRDBlkData_t = (P_RD_BLK_DATA_t)pNandDrv->pu8_PageDataBuf;
    RD_BLK_SPARE_t *pRDBlkSpare_t = (P_RD_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    U16 u16_SrcBlk, u16_DestBlk;
    U32 u32_Checksum, u32_Temp;
    volatile U32 u32_Ret;
    U8 u8_IfNeedCpBack;

    pNandDrv->u8_RD_ActiveFlag = 1;
    //---------------------------
    // get RD info
    u32_Ret = nand_ReadSectors_Raw(
        (pNandDrv->u16_RDB << pNandDrv->u8_BlkPageCntBits) + pNandDrv->u16_RDB_Page - 1,
        0, (U8*)pRDBlkData_t, (U8*)pRDBlkSpare_t, 1);
    if(UNFD_ST_SUCCESS != u32_Ret)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,
            "RD_ErrorHandler, power-cut at the mark of UNFD_FTL_RD_ST_START, %lXh \n", u32_Ret);
        nand_RD_WriteRDBPage(UNFD_FTL_RD_ST_IDLE);
        pNandDrv->u8_RD_ActiveFlag = 0;
        return;
    }
    u16_SrcBlk = pRDBlkData_t->u16_SrcPBA;
    u16_DestBlk = pRDBlkData_t->u16_DestPBA;
    u32_Checksum = pRDBlkData_t->u32_Checksum;
    nand_debug(UNFD_DEBUG_LEVEL, 1, "RD_ErrorHandler: SrcBlk:%Xh, DestBlk:%Xh, Checksum:%lXh \n",
        u16_SrcBlk, u16_DestBlk, u32_Checksum);

    //---------------------------
    // check SrcBlk
    u32_Ret = nand_ReadPages_Raw(((u16_SrcBlk+1)<<pNandDrv->u8_BlkPageCntBits)-1,
        (U8*)pRDBlkData_t, (U8*)pRDBlkSpare_t, 1);
    if(UNFD_ST_SUCCESS != u32_Ret)
    {
        u8_IfNeedCpBack = 1;
    }
    else
    {
        u32_Temp = drvNAND_CheckSum((U8*)pRDBlkData_t, pNandDrv->u16_PageByteCnt);
        u32_Temp += drvNAND_CheckSum((U8*)pRDBlkSpare_t, pNandDrv->u16_SpareByteCnt);
        if(u32_Temp == u32_Checksum)
            u8_IfNeedCpBack = 0;
        else
        {
            u8_IfNeedCpBack = 1;
        }
    }
    nand_debug(UNFD_DEBUG_LEVEL, 1, "RD_ErrorHandler: IfNeedCpBack:%X \n", u8_IfNeedCpBack);

    //---------------------------
    // action
    if(u8_IfNeedCpBack)
    {
        //nand_reset_WatchDog();
        if(UNFD_ST_SUCCESS != nand_EraseBlk(u16_SrcBlk << pNandDrv->u8_BlkPageCntBits) ||
            UNFD_ST_SUCCESS != nand_copy_data(u16_SrcBlk, 0, u16_DestBlk, pNandDrv->u16_BlkSectorCnt))
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"Error. RD_ErrorHandler, write-back fail: %X \n",
                u16_SrcBlk);
            nand_die();
        }
    }

    nand_erase_and_set_FreeBlk(u16_DestBlk);
    nand_RD_WriteRDBPage(UNFD_FTL_RD_ST_IDLE);
    pNandDrv->u8_RD_ActiveFlag = 0;
}

static void nand_RD_UpdateRDB(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

    if(pNandDrv->u16_RDB_Page >= pNandDrv->u16_BlkPageCnt)
    {
        nand_erase_and_set_FreeBlk(pNandDrv->u16_RDB);
        pNandDrv->u16_RDB = nand_get_FreeBlk_Zone0();
        pNandDrv->u16_RDB_Page = 0;
    }
}

static void nand_RD_WriteRDBPage(U8 u8_RD_Status)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    RD_BLK_DATA_t *pRDBlkData_t = (P_RD_BLK_DATA_t)pNandDrv->pu8_PageDataBuf;
    RD_BLK_SPARE_t *pRDBlkSpare_t = (P_RD_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    U32 u32_Ret;

    pRDBlkSpare_t->u8_BadBlkMark = 0xFF;
    pRDBlkSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_RD;
    pRDBlkSpare_t->u8_St = u8_RD_Status;
    pRDBlkSpare_t->u8_RDD3 = 0xFF;
    pRDBlkSpare_t->u8_RDD4 = 0xFF;
    pRDBlkSpare_t->u8_RDD5 = 0xFF;

    LABEL_WRITE_RDB:
    #if 0
    u32_Ret = NC_WriteSectors(
        (pNandDrv->u16_RDB << pNandDrv->u8_BlkPageCntBits) + pNandDrv->u16_RDB_Page,
        0, (U8*)pRDBlkData_t, (U8*)pRDBlkSpare_t, 1);
    #else
    u32_Ret = nand_WritePages_Raw(
        (pNandDrv->u16_RDB << pNandDrv->u8_BlkPageCntBits) + pNandDrv->u16_RDB_Page,
        (U8*)pRDBlkData_t, (U8*)pRDBlkSpare_t, 1);
    #endif

    if(UNFD_ST_SUCCESS == u32_Ret)
    {
        pNandDrv->u16_RDB_Page++;

        if(UNFD_FTL_RD_ST_IDLE == u8_RD_Status)
            nand_RD_UpdateRDB();
    }
    else
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"W RDB fail: %lXh \n", u32_Ret);
        if(u32_Ret == UNFD_ST_ERR_W_FAIL)
            drvNAND_MarkBadBlk(pNandDrv->u16_RDB);
        pNandDrv->u16_RDB = nand_get_FreeBlk_Zone0();
        pNandDrv->u16_RDB_Page = 0;

        if(UNFD_FTL_RD_ST_START == u8_RD_Status)
            goto LABEL_WRITE_RDB;
    }
}


void nand_ReadDisturbance_ftl(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    RD_BLK_DATA_t *pRDBlkData_t = (P_RD_BLK_DATA_t)pNandDrv->pu8_PageDataBuf;
    RD_BLK_SPARE_t *pRDBlkSpare_t = (P_RD_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    DATA_BLK_SPARE_t *pDataBlkSpare_t = (P_DATA_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    volatile U16 u16_FreeBlk;
    U16 u16_LBA, *pu16_L2P;
    volatile U32 u32_Ret, u32_Checksum;

    pNandDrv->u8_RD_ActiveFlag = 1;

    //---------------------------
    // check if the PBA valid for RD
    nand_validate_WLRD_PBA(pNandDrv->u16_RD_SrcPBA, UNFD_FTL_BLK_ID_RD);
    //nand_debug(0,1,"RD_SrcBlk: %X \n", pNandDrv->u16_RD_SrcPBA);
    if(0 == pNandDrv->u16_RD_SrcPBA)
        return;

    //---------------------------
    // init RDB, RDB_Page
    if(0 == pNandDrv->u16_RDB)
    {
        pNandDrv->u16_RDB = nand_get_FreeBlk_Zone0();
        pNandDrv->u16_RDB_Page = 0;
        nand_debug(UNFD_DEBUG_LEVEL,1,"RD, init RDB: %X  RDB_Page: %X \n",
            pNandDrv->u16_RDB, pNandDrv->u16_RDB_Page);
    }

    LABEL_START_RD:
    //---------------------------
    // RD execute ...
    // physical
    if(pNandDrv->u16_RD_SrcPBA < pNandDrv->u16_FirstPBA_LogiArea)
    {
        //nand_debug(0,1,"RD, Phy, PBA: %X\n", pNandDrv->u16_RD_SrcPBA);

        u16_FreeBlk = nand_get_FreeBlk_Zone0();
        //--------------
        // setup RDB
        u32_Ret = nand_ReadPages_Raw(((pNandDrv->u16_RD_SrcPBA+1)<<pNandDrv->u8_BlkPageCntBits)-1,
            (U8*)pRDBlkData_t, (U8*)pRDBlkSpare_t, 1);
        if(UNFD_ST_SUCCESS != u32_Ret)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, Read RD_SrcPBA last page: %X\n",
                pNandDrv->u16_RD_SrcPBA);
            if(UNFD_ST_ERR_ECC_FAIL != u32_Ret) // ECC fail does NOT assert for CUS
                nand_die();
        }
        u32_Checksum = drvNAND_CheckSum((U8*)pRDBlkData_t, pNandDrv->u16_PageByteCnt);
        u32_Checksum += drvNAND_CheckSum((U8*)pRDBlkSpare_t, pNandDrv->u16_SpareByteCnt);
        pRDBlkData_t->u16_SrcPBA = pNandDrv->u16_RD_SrcPBA;
        pRDBlkData_t->u16_DestPBA = u16_FreeBlk;
        pRDBlkData_t->u32_Checksum = u32_Checksum;

        //nand_debug(0,1,"RD_ST_START, SrcBlk:%X  DestBlk:%X  BlkID:%X \n",
        //	pRDBlkData_t->u16_SrcPBA, pRDBlkData_t->u16_DestPBA, pRDBlkSpare_t->u8_BlkID);

        nand_RD_WriteRDBPage(UNFD_FTL_RD_ST_START);

        //--------------
        // move data to FreeBlk
        u32_Ret = nand_copy_data(u16_FreeBlk, 0, pNandDrv->u16_RD_SrcPBA, pNandDrv->u16_BlkSectorCnt);
        if(UNFD_ST_SUCCESS != u32_Ret)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"RD Error.1, \n");
            if(u32_Ret != UNFD_ST_ERR_W_FAIL)
                nand_erase_and_set_FreeBlk(u16_FreeBlk);
            nand_RD_WriteRDBPage(UNFD_FTL_RD_ST_IDLE);
            goto LABEL_START_RD;
        }

        //--------------
        // move data back to SrcBlk
        if(UNFD_ST_SUCCESS != nand_EraseBlk(pNandDrv->u16_RD_SrcPBA<<pNandDrv->u8_BlkPageCntBits) ||
            UNFD_ST_SUCCESS != nand_copy_data(pNandDrv->u16_RD_SrcPBA, 0, u16_FreeBlk, pNandDrv->u16_BlkSectorCnt))
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"RD Error. Phy write-back error: %X \n", pNandDrv->u16_RD_SrcPBA);
            nand_die();
        }

        nand_erase_and_set_FreeBlk(u16_FreeBlk);
        //--------------
        // clean RD status
        nand_RD_WriteRDBPage(UNFD_FTL_RD_ST_IDLE);

    }
    else // logical
    {
        //--------------
        // get free blk
        if(pNandDrv->u16_RD_SrcPBA < pNandDrv->u16_FirstPBA_Zone1)
        {    // zone_0
            u16_FreeBlk = nand_get_FreeBlk_Zone0();
            pu16_L2P = pNandDrv->au16_L2P_0;
            //nand_debug(0,1,"RD, zone_0, SrcPBA: %X  FreeBlk: %X\n", pNandDrv->u16_RD_SrcPBA, u16_FreeBlk);
        }
        else // zone_1
        {
            u16_FreeBlk = nand_get_FreeBlk_Zone1();
            pu16_L2P = pNandDrv->au16_L2P_1;
            //nand_debug(0,1,"RD, zone_1, SrcPBA: %X  FreeBlk: %X\n", pNandDrv->u16_RD_SrcPBA, u16_FreeBlk);
        }

        //--------------
        // get LBA
        u32_Ret = nand_ReadPages_Raw(pNandDrv->u16_RD_SrcPBA<<pNandDrv->u8_BlkPageCntBits,
            (U8*)pRDBlkData_t, (U8*)pDataBlkSpare_t, 1);
        if(UNFD_ST_SUCCESS != u32_Ret)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, Read RD_SrcPBA last page: %X\n",
                pNandDrv->u16_RD_SrcPBA);

            if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                nand_ECC_ErrorHandler(pNandDrv->u16_RD_SrcPBA);
        }
        u16_LBA = pDataBlkSpare_t->u16_LBA;

        //--------------
        // move data
        u32_Ret = nand_copy_data(u16_FreeBlk, 0, pNandDrv->u16_RD_SrcPBA, pNandDrv->u16_BlkSectorCnt);
        if(UNFD_ST_SUCCESS != u32_Ret)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "RD Error.0, \n");
            if(u32_Ret != UNFD_ST_ERR_W_FAIL)
                nand_erase_and_set_FreeBlk(u16_FreeBlk);
            goto LABEL_START_RD;
        }

        //--------------
        // update LUT
        pu16_L2P[u16_LBA] = u16_FreeBlk;

        if(pNandDrv->u8_RD_ECCBitsCnt < 
            (pNandDrv->u16_ECCCorretableBit-pNandDrv->u16_BitflipThreshold/2))
        {
            nand_erase_and_set_FreeBlk(pNandDrv->u16_RD_SrcPBA);
        }
        else
        {
            nand_debug(0,1,"NAND, RD, mark bad, ECC bits: %X,  Max: %X\n",
                pNandDrv->u8_RD_ECCBitsCnt, (pNandDrv->u16_ECCCorretableBit-pNandDrv->u16_BitflipThreshold/2));
            drvNAND_MarkBadBlk(pNandDrv->u16_RD_SrcPBA);
        }
    }
}
#endif


#if UNFD_FTL_WBQ
static void nand_dump_WBQ(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8 u8_i;
    WBQ_t *pWBQ;

    nand_debug(0,0,"PPB: %X  PPBP:%X \n", pNandDrv->u16_PPB, pNandDrv->u16_PPB_Page);

    for(u8_i=0; u8_i<MAX_WBQ_CNT; u8_i++)
    {
        pWBQ = &pNandDrv->aWBQ_t[u8_i];

        nand_debug(0,0, "WBQ[%X], L:%03X, O:%03X, N:%03X, P:%02X, S:%02X, PPBP:%X \n",
            u8_i, pWBQ->u16_LBA, pWBQ->u16_OldPBA, pWBQ->u16_NewPBA,
            pWBQ->u16_PagePos, pWBQ->u16_SectorPos, pWBQ->u16_PPBPageIdx);
    }
}

static void nand_reset_WBQs(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8 u8_i;

    for(u8_i=0; u8_i<MAX_WBQ_CNT; u8_i++)
        pNandDrv->aWBQ_t[u8_i].u16_OldPBA = 0;

    pNandDrv->u16_PPB = nand_get_FreeBlk_Zone0();
    pNandDrv->u16_PPB_Page = 0;
}

static void nand_WBQ_UpdatePPB(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    PPB_BLK_SPARE_t *pPPBSpare_t = (P_PPB_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    volatile U16 u16_FreeBlk, u16_Page, u16_i;
    volatile U8 au8_WBQIdx[MAX_WBQ_CNT], u8_ActiveWBQCnt, u8_i, u8_j;
    U32 u32_Ret, u32_SrcRow, u32_DestRow;

    pNandDrv->u16_PPB_Page++;

    if(pNandDrv->u16_PPB_Page >= pNandDrv->u16_BlkPageCnt)
    {
        LABEL_UpdatePPB_START:
        u16_FreeBlk = nand_get_FreeBlk_Zone0();
        u16_Page = 0;
        //nand_debug(0,1,"UpdatePPB: from %X to %X\n", pNandDrv->u16_PPB, u16_FreeBlk);

        // select the valid WBQ
        u8_ActiveWBQCnt = 0;
        memset((void *)au8_WBQIdx, MAX_WBQ_CNT, MAX_WBQ_CNT);
        for(u8_i=0; u8_i<MAX_WBQ_CNT; u8_i++)
        {
            if(0 != pNandDrv->aWBQ_t[u8_i].u16_OldPBA && 0 != pNandDrv->aWBQ_t[u8_i].u16_SectorPos)
            {
                au8_WBQIdx[u8_ActiveWBQCnt] = u8_i;
                u8_ActiveWBQCnt++;
            }
        }

        // move the data from PPB to FreeBlk
        u32_SrcRow = ((pNandDrv->u16_PPB+1)<<pNandDrv->u8_BlkPageCntBits)-1;
        u32_DestRow = u16_FreeBlk << pNandDrv->u8_BlkPageCntBits;
        u8_i = 0;
        for(u16_i=0; u16_i<pNandDrv->u16_BlkPageCnt; u16_i++)
        {
            if(u8_i == u8_ActiveWBQCnt)
                break;

            u32_Ret = nand_ReadPages_Raw(u32_SrcRow-u16_i,
                pNandDrv->pu8_PageDataBuf, (U8*)pPPBSpare_t, 1);
            if(UNFD_ST_SUCCESS != u32_Ret)
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Error 0\n");
                if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                    nand_ECC_ErrorHandler((u32_SrcRow-u16_i)>>pNandDrv->u8_BlkPageCntBits);

            }

            for(u8_j=0; u8_j<u8_ActiveWBQCnt; u8_j++)
                if(au8_WBQIdx[u8_j] == pPPBSpare_t->u8_WBQIdx)
                {
                    u32_Ret = nand_WritePages_Raw(u32_DestRow + u16_Page,
                        pNandDrv->pu8_PageDataBuf, (U8*)pPPBSpare_t, 1);
                    if(UNFD_ST_SUCCESS != u32_Ret)
                    {
                        if(u32_Ret == UNFD_ST_ERR_W_FAIL)
                            drvNAND_MarkBadBlk(u16_FreeBlk);
                        goto LABEL_UpdatePPB_START;
                    }

                    pNandDrv->aWBQ_t[pPPBSpare_t->u8_WBQIdx].u16_PPBPageIdx = u16_Page;
                    u16_Page++;

                    au8_WBQIdx[u8_j] = MAX_WBQ_CNT; // finish then de-select
                    u8_i++;
                    break;
                }

        }

        nand_erase_and_set_FreeBlk(pNandDrv->u16_PPB);
        pNandDrv->u16_PPB = u16_FreeBlk;
        pNandDrv->u16_PPB_Page = u16_Page;
    }
}


// from SectorInBlkStart to SectorInBlkCopyEnd.
// if WBQ_t.u16_SectorPos is not 0, take care of PPB.
static U32 nand_WBQ_WriteBack(U8 u8_WBQIdx, U16 u16_SectorInBlkCopyEnd)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
    DATA_BLK_SPARE_t *pDataBlkSpare_t = (P_DATA_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    U32 u32_Row;
    U32 u32_Ret, u32_SectorInBlkStart;

    u32_SectorInBlkStart = pNandDrv->aWBQ_t[u8_WBQIdx].u16_PagePos << pNandDrv->u8_PageSectorCntBits;
    u32_SectorInBlkStart += pNandDrv->aWBQ_t[u8_WBQIdx].u16_SectorPos;

    #if UNFD_FTL_CHECK_ERROR
    if(u32_SectorInBlkStart > u16_SectorInBlkCopyEnd)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, should never be here\n");
        nand_die();
    }
    #endif

    // ------------------------------------
    // check & read if data kept in PPB
    if(pNandDrv->aWBQ_t[u8_WBQIdx].u16_SectorPos)
    {
        u32_Row = pNandDrv->u16_PPB << pNandDrv->u8_BlkPageCntBits;
        u32_Row += pNandDrv->aWBQ_t[u8_WBQIdx].u16_PPBPageIdx;
        u32_Ret = nand_ReadSectors_Raw(u32_Row, 0, au8_PageBuf, (U8*)pDataBlkSpare_t,
            pNandDrv->aWBQ_t[u8_WBQIdx].u16_SectorPos);
        if(UNFD_ST_SUCCESS != u32_Ret)
        {
            if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                nand_ECC_ErrorHandler(u32_Row >> pNandDrv->u8_BlkPageCntBits);

            nand_erase_and_set_FreeBlk(pNandDrv->aWBQ_t[u8_WBQIdx].u16_NewPBA);
            pNandDrv->aWBQ_t[u8_WBQIdx].u16_OldPBA = 0; // write back fail, skip this WBQ
            return u32_Ret;
        }
        pDataBlkSpare_t->u8_BadBlkMark = 0xFF;
        pDataBlkSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_DATA;
        pDataBlkSpare_t->u16_LBA = pNandDrv->aWBQ_t[u8_WBQIdx].u16_LBA;
        pDataBlkSpare_t->u8_RDD4 = 0xFF;
        pDataBlkSpare_t->u8_RDD5 = 0xFF;
    }

    if(u32_SectorInBlkStart == u16_SectorInBlkCopyEnd)
    {
        ftl_rw_debug(nand_debug(0, 1, "no write back\n"));
        return UNFD_ST_SUCCESS;
    }

    // ------------------------------------
    // write back
    ftl_rw_debug(nand_debug(0, 1, "OldPBA:%X, StartSec:%lX, NewPBA:%X, SecCnt:%lX\n",
        pNandDrv->aWBQ_t[u8_WBQIdx].u16_OldPBA,
        u32_SectorInBlkStart,
        pNandDrv->aWBQ_t[u8_WBQIdx].u16_NewPBA,
        u16_SectorInBlkCopyEnd - u32_SectorInBlkStart));

    u32_Ret = nand_copy_data(pNandDrv->aWBQ_t[u8_WBQIdx].u16_NewPBA,
                u32_SectorInBlkStart,
                pNandDrv->aWBQ_t[u8_WBQIdx].u16_OldPBA,
                u16_SectorInBlkCopyEnd - u32_SectorInBlkStart);
    if(UNFD_ST_SUCCESS != u32_Ret)
    {
        if(u32_Ret != UNFD_ST_ERR_W_FAIL)
            nand_erase_and_set_FreeBlk(pNandDrv->aWBQ_t[u8_WBQIdx].u16_NewPBA);
        pNandDrv->aWBQ_t[u8_WBQIdx].u16_OldPBA = 0; // write back fail, skip this WBQ
    }

    return u32_Ret;
}


U32 nand_FlushWBQ(U8 u8_WBQIdx)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U32 u32_Ret;
    WBQ_t *pWBQ = &pNandDrv->aWBQ_t[u8_WBQIdx];

    if(0 == pWBQ->u16_OldPBA)
        return UNFD_ST_SUCCESS;

    ftl_rw_debug(nand_debug(0,1,"flush WBQ[%X], LBA: %X\n", u8_WBQIdx, pWBQ->u16_LBA));

    u32_Ret = nand_WBQ_WriteBack(u8_WBQIdx, pNandDrv->u16_BlkSectorCnt);
    if(UNFD_ST_SUCCESS != u32_Ret)
        return u32_Ret;

    nand_erase_and_set_FreeBlk(pWBQ->u16_OldPBA);
    pWBQ->u16_OldPBA = 0;

    if(pWBQ->u16_NewPBA < pNandDrv->u16_FirstPBA_Zone1)
        pNandDrv->au16_L2P_0[pWBQ->u16_LBA] = pWBQ->u16_NewPBA;
    else
        pNandDrv->au16_L2P_1[pWBQ->u16_LBA] = pWBQ->u16_NewPBA;

    return UNFD_ST_SUCCESS;
}
#endif

/* -----------------------------------------------------------
 * nand_WriteFlow
 *   @u32_SectorCnt        : write sector numbers
 *   @u32_StartSectorIdx   : write sector start No.
 *   @u32_RWBufferAddress  : write data from get buffer address
 * ------------------------------------------------------------*/
static U32 nand_copy_data
(
    U16 u16_ToBlk, U16 u16_SectorInBlk, U16 u16_FromBlk, U16 u16_SectorCnt
)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
    U8 *au8_SpareBuf = (U8*)pNandDrv->pu8_PageSpareBuf;
    U32 u32_Ret;
    U32 u32_FromRow, u32_ToRow, u32_row;
    static U16 u16_OneTimeSectorCnt, u16_OneTimePageCnt, u16_PageSectorPos;

    ftl_rw_debug(nand_debug(0,1,"copy sec cnt: %X\n", u16_SectorCnt));

    #if defined(ASSERT_FTL_ACCESS_BLK0) && ASSERT_FTL_ACCESS_BLK0
    if(u16_ToBlk == 0 || u16_FromBlk == 0)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"Error: From Blk0x%X To Blk0x%X\n", u16_FromBlk, u16_ToBlk);
        nand_die();
    }
    #endif

    while(u16_SectorCnt > 0)
    {
        //--------------------------------------------
        u32_row = u16_SectorInBlk >> pNandDrv->u8_PageSectorCntBits;
        u32_FromRow = (u16_FromBlk << pNandDrv->u8_BlkPageCntBits) + u32_row;
        u32_ToRow = (u16_ToBlk << pNandDrv->u8_BlkPageCntBits) + u32_row;

        //--------------------------------------------
        // partial R/W
        if(u16_SectorInBlk & pNandDrv->u16_PageSectorCntMask)
        {			
            u16_PageSectorPos = u16_SectorInBlk & pNandDrv->u16_PageSectorCntMask;

            u16_OneTimeSectorCnt = pNandDrv->u16_PageSectorCnt - u16_PageSectorPos;
            u16_OneTimeSectorCnt = u16_OneTimeSectorCnt > u16_SectorCnt ?
                u16_SectorCnt : u16_OneTimeSectorCnt;

            ftl_rw_debug(nand_debug(0,1,"cp partial 1: %X %X\n",
                u16_PageSectorPos, u16_OneTimeSectorCnt));

            u32_Ret = nand_ReadSectors_Raw(u32_FromRow,
                u16_PageSectorPos,
                au8_PageBuf + (u16_PageSectorPos << 9),
                au8_SpareBuf + (u16_PageSectorPos * pNandDrv->u16_SectorSpareByteCnt),
                u16_OneTimeSectorCnt);
            if(UNFD_ST_SUCCESS != u32_Ret)
            {
                if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                    nand_ECC_ErrorHandler(u32_FromRow >> pNandDrv->u8_BlkPageCntBits);
                return u32_Ret;
            }

            if(u16_PageSectorPos+u16_OneTimeSectorCnt == pNandDrv->u16_PageSectorCnt)
            {
                u32_Ret = nand_WritePages_Raw(u32_ToRow, au8_PageBuf, au8_SpareBuf, 1);
                if(UNFD_ST_SUCCESS != u32_Ret)
                {
                    if(u32_Ret == UNFD_ST_ERR_W_FAIL)
                        drvNAND_MarkBadBlk(u16_ToBlk);
                    return u32_Ret;
                }
            }
        }
        //--------------------------------------------
        else
        {						
            u16_OneTimeSectorCnt =
                u16_SectorCnt > pNandDrv->u16_BlkSectorCnt-u16_SectorInBlk ?
                pNandDrv->u16_BlkSectorCnt-u16_SectorInBlk : u16_SectorCnt;

            u16_OneTimePageCnt = u16_OneTimeSectorCnt >> pNandDrv->u8_PageSectorCntBits;

            if(u16_OneTimePageCnt) // page R/W
            {
                U16 u16_Tmp;
                ftl_rw_debug(nand_debug(0,1,"cp page %X\n", u16_OneTimePageCnt));

                {
                    for(u16_Tmp=0; u16_Tmp<u16_OneTimePageCnt; u16_Tmp++)
                    {
                        u32_Ret = nand_ReadPages_Raw(
                            u32_FromRow+u16_Tmp, au8_PageBuf, au8_SpareBuf, 1);
                        if(UNFD_ST_SUCCESS != u32_Ret)
                        {
                            if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                                nand_ECC_ErrorHandler((u32_FromRow+u16_Tmp)>>pNandDrv->u8_BlkPageCntBits);
                            return u32_Ret;
                        }

                        u32_Ret = nand_WritePages_Raw(
                            u32_ToRow+u16_Tmp, au8_PageBuf, au8_SpareBuf, 1);
                        if(UNFD_ST_SUCCESS != u32_Ret)
                        {
                            if(u32_Ret == UNFD_ST_ERR_W_FAIL)
                                drvNAND_MarkBadBlk(u16_ToBlk);
                            return u32_Ret;
                        }
                    }
                }

                u16_OneTimeSectorCnt = u16_OneTimePageCnt << pNandDrv->u8_PageSectorCntBits;
            }
            else // partial R/W
            {
                ftl_rw_debug(nand_debug(0,1,"cp partial 2: %X %X\n",
                    u16_SectorInBlk & pNandDrv->u16_PageSectorCntMask, u16_OneTimeSectorCnt));

                u32_Ret = nand_ReadSectors_Raw(u32_FromRow, 0, au8_PageBuf,
                    au8_SpareBuf, u16_OneTimeSectorCnt);
                if(UNFD_ST_SUCCESS != u32_Ret)
                {
                    if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                        nand_ECC_ErrorHandler(u32_FromRow >> pNandDrv->u8_BlkPageCntBits);
                    return u32_Ret;
                }
            }
        }

        //--------------------------------------------
        u16_SectorInBlk += u16_OneTimeSectorCnt;
        u16_SectorCnt -= u16_OneTimeSectorCnt;
    }

    return UNFD_ST_SUCCESS;
}


U32 nand_WriteFlow(U32 u32_StartSectorIdx, U32 u32_SectorCnt, U32 u32_RWBufferAddress)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    DATA_BLK_SPARE_t *pDataBlkSpare_t = (P_DATA_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    volatile U16 u16_FreePBA, u16_LBA, u16_PBA, *au16_L2P;
    volatile U16 u16_OneTimeSectorCnt, u16_Stg2SectorCnt, u16_TmpSectorCnt, u16_TmpPageCnt,
                u16_PageSectorPos;
    volatile U32 u32_Ret, u32_Row;
    #if UNFD_FTL_WBQ
    PPB_BLK_SPARE_t *pPPBSpare_t = (P_PPB_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    volatile U16 u16_StartSectorIdxInBlk, u16_WBQPageCnt, u16_temp;
    volatile U8 u8_j=0, u8_IfStg1;
    U8 u8_CurWBQIdx;
    #endif

    if (0 == drvNAND_ChkRdy(0))
        return UNFD_ST_ERR_NOT_READY;

    //printf("[%s][%d] ===== MsOS_ObtainMutex ww =====\n", __FUNCTION__, __LINE__);
    //MsOS_ObtainMutex(_s32UnfdTaskMutexId, MSOS_WAIT_FOREVER);

    while(u32_SectorCnt > 0)
    {
        //-----------------------------------------------------------------
        // 1. pick up the PBA, and a FreeBlk
        // 2. one time loop for one block.
        //-----------------------------------------------------------------
        u16_LBA = u32_StartSectorIdx >> pNandDrv->u8_BlkSectorCntBits;

        #if UNFD_FTL_FMAP
        if (u16_LBA >= pNandDrv->u16_FM_FirstLBA &&
            u16_LBA < pNandDrv->u16_FM_FirstLBA + pNandDrv->u16_FM_LBACnt)
        {
            volatile U32 u32_FM_SectorCnt;

            u32_FM_SectorCnt = ((pNandDrv->u16_FM_FirstLBA + pNandDrv->u16_FM_LBACnt) <<
                pNandDrv->u8_BlkSectorCntBits) - u32_StartSectorIdx;

            u32_FM_SectorCnt = u32_FM_SectorCnt > u32_SectorCnt ?
                            u32_SectorCnt : u32_FM_SectorCnt;

            u32_Ret = nand_FM_WriteFlow(u32_StartSectorIdx, u32_FM_SectorCnt, u32_RWBufferAddress);
            if(UNFD_ST_SUCCESS != u32_Ret)
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, FM_WriteFlow: %lX\n", u32_Ret);
                goto Exit;
            }

            u32_StartSectorIdx += u32_FM_SectorCnt;
            u32_SectorCnt -= u32_FM_SectorCnt;
            u32_RWBufferAddress += u32_FM_SectorCnt<<9;
            continue;
        }
        #endif

        if(u16_LBA < pNandDrv->u16_Zone0LBACnt) // Zone_0
        {
            au16_L2P = pNandDrv->au16_L2P_0;
            u16_PBA = au16_L2P[u16_LBA];
            u16_FreePBA = nand_get_FreeBlk_Zone0();
            ftl_rw_debug(nand_debug(0,1,"SecAdr: %lX, SecCnt: %lX, Zone_0: LBA: %X, PBA: %X, Free: %X \n",
            u32_StartSectorIdx, u32_SectorCnt, u16_LBA, u16_PBA, u16_FreePBA));
        }
        else
        {
            u16_LBA -= pNandDrv->u16_Zone0LBACnt;
            au16_L2P = pNandDrv->au16_L2P_1;
            u16_PBA = au16_L2P[u16_LBA];
            while(0 == u16_PBA)
            {
                nand_build_LUT1();
                u16_PBA = au16_L2P[u16_LBA];
            }

            u16_FreePBA = nand_get_FreeBlk_Zone1();
            ftl_rw_debug(nand_debug(0,1,"SecAdr: %lX, SecCnt: %lX, Zone_1: LBA: %X, PBA: %X, Free: %X \n",
                u32_StartSectorIdx, u32_SectorCnt, u16_LBA, u16_PBA, u16_FreePBA));
        }

        #if UNFD_FTL_WBQ
        //--------------------------------------------
        // check & flush WBQ if necessary
        for(u8_CurWBQIdx=0; u8_CurWBQIdx<MAX_WBQ_CNT; u8_CurWBQIdx++)
            if(u16_PBA == pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_OldPBA)
                break;

        //nand_debug(0,1,"PBA:%03X, %X, %X \n", u16_PBA, u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask, u32_SectorCnt);
        //nand_dump_WBQ();

        if(u8_CurWBQIdx < MAX_WBQ_CNT)
        {   // ---- inside WBQ
            u16_StartSectorIdxInBlk = u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask;

            if(u16_StartSectorIdxInBlk <
                (pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_PagePos<<pNandDrv->u8_PageSectorCntBits)+pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_SectorPos)
            {
                // -- write-back
                //nand_debug(0,1,"WBQ WB: %X %X\n",
                //	pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_PagePos, pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_SectorPos);

                u32_Ret = nand_WBQ_WriteBack(u8_CurWBQIdx, pNandDrv->u16_BlkSectorCnt);
                if(UNFD_ST_SUCCESS != u32_Ret)
                {
                    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, R, WB 0 \n");
                    nand_set_FreeBlk(u16_FreePBA); // give back free PBA to free queue
                    goto Exit;
                }
                nand_erase_and_set_FreeBlk(u16_PBA);
                pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_OldPBA = pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_NewPBA;
                au16_L2P[u16_LBA] = pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_NewPBA;
                u16_PBA = au16_L2P[u16_LBA];
                u8_IfStg1 = 1;
            }
            else
            {	// -- partial write-back
                //nand_debug(0,1,"WBQ active: %X \n", u8_CurWBQIdx);

                u32_Ret = nand_WBQ_WriteBack(u8_CurWBQIdx, u16_StartSectorIdxInBlk);
                if(UNFD_ST_SUCCESS != u32_Ret)
                {
                    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, R, WB 1 \n");
                    nand_set_FreeBlk(u16_FreePBA); // give back free PBA to free queue
                    goto Exit;
                }
                nand_set_FreeBlk(u16_FreePBA);
                u16_FreePBA = pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_NewPBA;
                u8_IfStg1 = 0;
            }
        }
        else
        {   // ---- pick up one WBQ
            u16_WBQPageCnt = 0;
            for(u8_CurWBQIdx=0; u8_CurWBQIdx<MAX_WBQ_CNT; u8_CurWBQIdx++)
            {
                if(0 == pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_OldPBA)
                    break;
                if(u16_WBQPageCnt < pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_PagePos)
                {
                    u16_WBQPageCnt = pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_PagePos;
                    u8_j = u8_CurWBQIdx;
                }
            }
            if(u8_CurWBQIdx == MAX_WBQ_CNT) // no empty WBQ, flush one having less free page
            {
                u8_CurWBQIdx = u8_j;
                u32_Ret = nand_WBQ_WriteBack(u8_CurWBQIdx, pNandDrv->u16_BlkSectorCnt);
                if(UNFD_ST_SUCCESS != u32_Ret)
                {
                    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, R, WB 2 \n");
                    nand_set_FreeBlk(u16_FreePBA); // give back free PBA to free queue
                    goto Exit;
                }
                nand_erase_and_set_FreeBlk(pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_OldPBA);
                if(pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_NewPBA < pNandDrv->u16_FirstPBA_Zone1)
                {
                    //nand_debug(0,1, "flush one WBQ, Zone0\n");
                    pNandDrv->au16_L2P_0[pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_LBA] = pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_NewPBA;
                }
                else
                {
                    //nand_debug(0,1, "flush one WBQ, Zone1\n");
                    pNandDrv->au16_L2P_1[pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_LBA] = pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_NewPBA;
                }
            }
            //nand_debug(0,1,"WBQ new: %X \n", u8_CurWBQIdx);
            pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_OldPBA = u16_PBA;
            u8_IfStg1 = 1;
        }
        #endif

        //--------------------------------------------
        // stage 1
        #if UNFD_FTL_WBQ
        if(u8_IfStg1)
        #endif
        u16_OneTimeSectorCnt = u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask;
        u32_Ret = nand_copy_data(u16_FreePBA, 0, u16_PBA, u16_OneTimeSectorCnt);
        if(u32_Ret != UNFD_ST_SUCCESS)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND, stage 1 fail: %lX\n", u32_Ret);
            if(u32_Ret != UNFD_ST_ERR_W_FAIL)
                nand_erase_and_set_FreeBlk(u16_FreePBA);
            pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_OldPBA = 0;
            goto Exit;
        }

        //--------------------------------------------
        // stage 2
        u16_OneTimeSectorCnt = pNandDrv->u16_BlkSectorCnt -
            (u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask);
        u16_OneTimeSectorCnt = u16_OneTimeSectorCnt > u32_SectorCnt ?
            u32_SectorCnt : u16_OneTimeSectorCnt;

        u16_Stg2SectorCnt = u16_OneTimeSectorCnt;
        ftl_rw_debug(nand_debug(0,1,"stg2, SectorCnt: %X \n", u16_Stg2SectorCnt));

        pDataBlkSpare_t->u8_BadBlkMark = 0xFF;
        pDataBlkSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_DATA;
        pDataBlkSpare_t->u16_LBA = u16_LBA;
        pDataBlkSpare_t->u8_RDD4 = 0xFF;
        pDataBlkSpare_t->u8_RDD5 = 0xFF;

        while(u16_Stg2SectorCnt)
        {
            //-----------------------
            u32_Row = (u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask) >>
                pNandDrv->u8_PageSectorCntBits;
            u32_Row += u16_FreePBA << pNandDrv->u8_BlkPageCntBits;

            //-----------------------
            // partial write
            if(u32_StartSectorIdx & pNandDrv->u16_PageSectorCntMask)
            {
                u16_PageSectorPos = u32_StartSectorIdx & pNandDrv->u16_PageSectorCntMask;

                u16_TmpSectorCnt = pNandDrv->u16_PageSectorCnt - u16_PageSectorPos;
                u16_TmpSectorCnt = u16_TmpSectorCnt > u16_Stg2SectorCnt ?
                    u16_Stg2SectorCnt : u16_TmpSectorCnt;

                ftl_rw_debug(nand_debug(0, 1, "partial 1: %X %X \n",
                    u16_PageSectorPos, u16_TmpSectorCnt));

                memcpy((void*)(pNandDrv->pu8_PageDataBuf + (u16_PageSectorPos<<9)),
                (void*)(u32_RWBufferAddress), u16_TmpSectorCnt<<9);

                if(u16_PageSectorPos+u16_TmpSectorCnt == pNandDrv->u16_PageSectorCnt)
                {
                    u32_Ret = nand_WritePages_Raw(u32_Row, pNandDrv->pu8_PageDataBuf,
                        (U8*)pDataBlkSpare_t, 1);
                    if(UNFD_ST_SUCCESS != u32_Ret)
                    {
                        if(u32_Ret == UNFD_ST_ERR_W_FAIL)
                            drvNAND_MarkBadBlk(u16_FreePBA);
                        else
                            nand_erase_and_set_FreeBlk(u16_FreePBA);
                        pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_OldPBA = 0;
                        goto Exit;
                    }
                }
            }
            //-----------------------
            else
            {
                u16_TmpSectorCnt = pNandDrv->u16_BlkSectorCnt -
                    (u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask);
                u16_TmpSectorCnt = u16_TmpSectorCnt > u16_Stg2SectorCnt ?
                    u16_Stg2SectorCnt : u16_TmpSectorCnt;

                u16_TmpPageCnt = u16_TmpSectorCnt >> pNandDrv->u8_PageSectorCntBits;

                if(u16_TmpPageCnt)
                {   // page write
                    ftl_rw_debug(nand_debug(0,1,"page w: %X \n", u16_TmpPageCnt));

                    u32_Ret = nand_WritePages_Raw(u32_Row,
                        (U8*)u32_RWBufferAddress, (U8*)pDataBlkSpare_t, u16_TmpPageCnt);
                    if(u32_Ret != UNFD_ST_SUCCESS)
                    {
                        if(u32_Ret == UNFD_ST_ERR_W_FAIL)
                            drvNAND_MarkBadBlk(u16_FreePBA);
                        else
                            nand_erase_and_set_FreeBlk(u16_FreePBA);
                        pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_OldPBA = 0;
                        goto Exit;
                    }

                    u16_TmpSectorCnt = u16_TmpPageCnt << pNandDrv->u8_PageSectorCntBits;
                }
                else
                {   // partial write
                    ftl_rw_debug(nand_debug(0,1,"partial 2: %lX %X\n",
                        (u32_StartSectorIdx & pNandDrv->u16_PageSectorCntMask), u16_TmpSectorCnt));

                    memcpy((void*)pNandDrv->pu8_PageDataBuf,
                        (void*)(u32_RWBufferAddress), u16_TmpSectorCnt<<9);
                }
            }

            //-----------------------
            u32_RWBufferAddress += u16_TmpSectorCnt<<9;
            u32_StartSectorIdx += u16_TmpSectorCnt;
            u16_Stg2SectorCnt -= u16_TmpSectorCnt;

            ftl_rw_debug(nand_debug(0,1,"stg.2. rest SecCnt: %X\n", u16_Stg2SectorCnt));
        }

        u32_SectorCnt -= u16_OneTimeSectorCnt;

        #if UNFD_FTL_WBQ
        //--------------------------------------------
        // possible stg.4
        if(0 == (u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask))
        {
            ftl_rw_debug(nand_debug(0,1, "finish a blk of W Flow, %lX\n", u32_SectorCnt));
            nand_erase_and_set_FreeBlk(u16_PBA);
            au16_L2P[u16_LBA] = u16_FreePBA;

            pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_OldPBA = 0; // clean the WBQ
        }
        //--------------------------------------------
        // setup the WBQ
        else if(0 == u32_SectorCnt)
        {
            u16_temp = u32_StartSectorIdx & pNandDrv->u16_PageSectorCntMask;
            ftl_rw_debug(nand_debug(0,1, "end of W Flow, WBQ:%X, LBA:%X, Old:%X, New:%X, Page:%X, Sec:%X, PPBP:%X\n",
                u8_CurWBQIdx, u16_LBA, u16_PBA, u16_FreePBA,
                (u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask) >> pNandDrv->u8_PageSectorCntBits,
                u16_temp, pNandDrv->u16_PPB_Page));

            pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_LBA = u16_LBA;
            pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_OldPBA = u16_PBA;
            pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_NewPBA = u16_FreePBA;
            pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_PagePos =
                (u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask) >> pNandDrv->u8_PageSectorCntBits;
            pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_SectorPos = u16_temp;
            pNandDrv->aWBQ_t[u8_CurWBQIdx].u16_PPBPageIdx = pNandDrv->u16_PPB_Page;

            // if partital page
            if(u16_temp)
            {
                pPPBSpare_t->u8_BadBlkMark = 0xFF;
                pPPBSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_PPB;
                pPPBSpare_t->u8_WBQIdx = u8_CurWBQIdx;
                pPPBSpare_t->u8_RDD3 = 0xFF;
                pPPBSpare_t->u8_RDD4 = 0xFF;
                pPPBSpare_t->u8_RDD5 = 0xFF;
                u32_Ret = nand_WritePages_Raw(
                    (pNandDrv->u16_PPB<<pNandDrv->u8_BlkPageCntBits)+pNandDrv->u16_PPB_Page,
                    pNandDrv->pu8_PageDataBuf, (U8*)pPPBSpare_t, 1);
                if(UNFD_ST_SUCCESS != u32_Ret)
                {
                    // [CAUTION]: special handle for the error here
                    pNandDrv->u16_PPB_Page = pNandDrv->u16_BlkPageCnt;
                    nand_WBQ_UpdatePPB();
                    //drvNAND_MarkBadBlk(u16_FreePBA);
                    goto Exit;
                }
                nand_WBQ_UpdatePPB();
            }
        }
        else
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, NAND WBQ, should not happen\n");
            nand_die();
        }
        #else
        //--------------------------------------------
        // stage 3
        u16_OneTimeSectorCnt = pNandDrv->u16_BlkSectorCnt -
            u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask;
        u32_Ret = nand_copy_data(u16_FreePBA, u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask,
            u16_PBA, u16_OneTimeSectorCnt);
        if(u32_Ret != UNFD_ST_SUCCESS)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, NAND, stage 3 fail: %lX\n", u32_Ret);
            if(u32_Ret != UNFD_ST_ERR_W_FAIL)
                nand_erase_and_set_FreeBlk(u16_FreePBA);
            goto Exit;
        }

        //--------------------------------------------
        // stage 4
        nand_erase_and_set_FreeBlk(u16_PBA);
        au16_L2P[u16_LBA] = u16_FreePBA;
        #endif
    }

    #if UNFD_FTL_WBQ

	drvNAND_BGThread(NAND_MSG_FORCE_WRITE_BACK);
//    drvNAND_SendMsg2BkgndTask(NAND_MSG_FORCE_WRITE_BACK);
//    drvNAND_SendMsg2BkgndTask(NAND_MSG_TOGGLE_BUSY);
    #endif

    #if UNFD_FTL_WL_TEST
    pNandDrv->u16_WL_TestCnt++;
    if(0 == (pNandDrv->u16_WL_TestCnt & 0x3F))
        nand_Wear_Leveling();
    //if(0 == (pNandDrv->u16_WL_TestCnt & 0x7F))
    //	nand_Wear_Leveling1();
    #endif

    #if 0//UNFD_FTL_RD_TEST
    if(0 != pNandDrv->u16_RD_SrcPBA)
    {
        //drvNAND_SendMsg2BkgndTask(NAND_MSG_READ_DISTURBANCE);
        nand_ReadDisturbance_ftl();
    }
    #endif
    //nand_dump_FreeBlk(0);


    //MsOS_ReleaseMutex(_s32UnfdTaskMutexId);
    //printf("[%s][%d] ===== MsOS_ReleaseMutex ww =====\n", __FUNCTION__, __LINE__);
    return UNFD_ST_SUCCESS;

Exit:

    //MsOS_ReleaseMutex(_s32UnfdTaskMutexId);
    //printf("[%s][%d] ===== MsOS_ReleaseMutex ww =====\n", __FUNCTION__, __LINE__);
    return u32_Ret;
}

#if NAND_DMA_RACING_PATCH
U32 u32_Addr0 = 0, u32_Addr1 = 0;
U32 u32_Addr0Dat = 0, u32_Addr1Dat = 0;
static void drvNAND_CheckMIUSignature(volatile U32 *pu32_Addr0, volatile U32 *pu32_Addr1)
{
    if(pu32_Addr0[0] == NAND_DMA_RACING_PATTERN0 || pu32_Addr1[0] == NAND_DMA_RACING_PATTERN1)
    {
        nand_translate_DMA_address_Ex((U32)pu32_Addr0, sizeof(U32), READ_FROM_NAND);
        nand_translate_DMA_address_Ex((U32)pu32_Addr1, sizeof(U32), READ_FROM_NAND);

        nand_hw_timer_delay(HW_TIMER_DELAY_1ms);

        // retry again
        if(pu32_Addr0[0] == NAND_DMA_RACING_PATTERN0 || pu32_Addr1[0] == NAND_DMA_RACING_PATTERN1)
        {
            #ifdef __NAND_DEBUG_CODE__
            u32_Addr0 = (U32)pu32_Addr0;
            u32_Addr1 = (U32)pu32_Addr1;
            u32_Addr0Dat = pu32_Addr0[0];
            u32_Addr1Dat = pu32_Addr1[1];
            nand_debug(UNFD_DEBUG_LEVEL,1,"u32_Addr0(%lX) u32_Addr1(%lX) u32_Addr0Dat(%lX) u32_Addr1Dat(%lX)\n", u32_Addr0, u32_Addr0Dat, u32_Addr1, u32_Addr1Dat);
            nand_die();
            #else
            nand_translate_DMA_address_Ex((U32)pu32_Addr0, sizeof(U32), READ_FROM_NAND);
            nand_translate_DMA_address_Ex((U32)pu32_Addr1, sizeof(U32), READ_FROM_NAND);
            #endif
        }
    }
}
#endif

U32 nand_ReadFlow(U32 u32_StartSectorIdx, U32 u32_SectorCnt, U32 u32_RWBufferAddress)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U32 u32_Row;
    U16 u16_LBA, u16_PBA, *au16_L2P;
    volatile U16 u16_StartSectorIdxInBlk, u16_SectorCnt, u16_OneTimeSectorCnt, u16_OneTimePageCnt;
    U32 u32_Ret;
    U8  u8_i;

    #if NAND_DMA_RACING_PATCH
    volatile U32 *pu32_Addr0, *pu32_Addr1;
    #endif

    if(0 == drvNAND_ChkRdy(0))
        return UNFD_ST_ERR_NOT_READY;


    //printf("[%s][%d] ===== MsOS_ObtainMutex rr =====\n", __FUNCTION__, __LINE__);
    //MsOS_ObtainMutex(_s32UnfdTaskMutexId, MSOS_WAIT_FOREVER);
    #if NAND_DMA_RACING_PATCH
    pu32_Addr0 = (U32*)((u32_RWBufferAddress +3) & ~3);
    pu32_Addr1 = (U32*)((u32_RWBufferAddress + (u32_SectorCnt<<9) - 4) & ~3);
    pu32_Addr0[0] = NAND_DMA_RACING_PATTERN0;
    pu32_Addr1[0] = NAND_DMA_RACING_PATTERN1;
    #endif

    while(u32_SectorCnt > 0)
    {
        //-----------------------------------------------------------------
        // 1. pick up the PBA
        // 2. one time loop may NOT absolutely for one block (maybe for the same block)
        //-----------------------------------------------------------------
        u16_LBA = u32_StartSectorIdx >> pNandDrv->u8_BlkSectorCntBits;

        #if UNFD_FTL_FMAP
        if(u16_LBA >= pNandDrv->u16_FM_FirstLBA &&
            u16_LBA < pNandDrv->u16_FM_FirstLBA + pNandDrv->u16_FM_LBACnt)
        {
            volatile U32 u32_FM_SectorCnt;

            u32_FM_SectorCnt = ((pNandDrv->u16_FM_FirstLBA + pNandDrv->u16_FM_LBACnt) <<
                pNandDrv->u8_BlkSectorCntBits) - u32_StartSectorIdx;

            u32_FM_SectorCnt = u32_FM_SectorCnt > u32_SectorCnt ?
                u32_SectorCnt : u32_FM_SectorCnt;

            u32_Ret = nand_FM_ReadFlow(u32_StartSectorIdx, u32_FM_SectorCnt, u32_RWBufferAddress);
            if(UNFD_ST_SUCCESS != u32_Ret)
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, nand_FM_ReadFlow: %lX\n", u32_Ret);
                goto Exit;
            }

            u32_StartSectorIdx += u32_FM_SectorCnt;
            u32_SectorCnt -= u32_FM_SectorCnt;
            u32_RWBufferAddress += u32_FM_SectorCnt<<9;
            continue;
        }
        #endif

        //--------------------------------------------
        if(u16_LBA < pNandDrv->u16_Zone0LBACnt) // Zone_0
        {
            au16_L2P = pNandDrv->au16_L2P_0;
            u16_PBA = au16_L2P[u16_LBA];

            ftl_rw_debug(nand_debug(0,1,"SecAdr: %lX, SecCnt: %lX, Zone_0: LBA: %X, PBA: %X \n",
                u32_StartSectorIdx, u32_SectorCnt, u16_LBA, u16_PBA));
        }
        else // Zone_1
        {
            u16_LBA -= pNandDrv->u16_Zone0LBACnt;
            au16_L2P = pNandDrv->au16_L2P_1;
            u16_PBA = au16_L2P[u16_LBA];
            while(0 == u16_PBA)
            {
                nand_build_LUT1();
                u16_PBA = au16_L2P[u16_LBA];
            }

            ftl_rw_debug(nand_debug(0,1,"SecAdr: %lX, SecCnt: %lX, Zone_1: LBA: %X, PBA: %X \n",
                u32_StartSectorIdx, u32_SectorCnt, u16_LBA, u16_PBA));
        }

        #if UNFD_FTL_WBQ
        //--------------------------------------------
        // check & flush WBQ if necessary
        for(u8_i=0; u8_i<MAX_WBQ_CNT; u8_i++)
            if(u16_PBA == pNandDrv->aWBQ_t[u8_i].u16_OldPBA)
				break;
        if(u8_i < MAX_WBQ_CNT) // inside WBQ
        {
            //nand_dump_WBQ();

            u16_StartSectorIdxInBlk = u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask;

            u16_PBA = pNandDrv->aWBQ_t[u8_i].u16_NewPBA;
            u32_Row = (u16_PBA << pNandDrv->u8_BlkPageCntBits) +
                (u16_StartSectorIdxInBlk >> pNandDrv->u8_PageSectorCntBits);

            u16_SectorCnt = (U16)u32_SectorCnt;

            if(((U32)u16_StartSectorIdxInBlk + u32_SectorCnt) <
                (((U32)pNandDrv->aWBQ_t[u8_i].u16_PagePos) << pNandDrv->u8_PageSectorCntBits))
            {
                //nand_debug(0,1,"  no \n");
            }
            else
            {
                // check & read if data kept in PPB
                if(pNandDrv->aWBQ_t[u8_i].u16_SectorPos)
                {
                    if(u16_StartSectorIdxInBlk <
                        (pNandDrv->aWBQ_t[u8_i].u16_PagePos << pNandDrv->u8_PageSectorCntBits))
                    {
                        u16_SectorCnt =
                            (pNandDrv->aWBQ_t[u8_i].u16_PagePos << pNandDrv->u8_PageSectorCntBits)
                            - u16_StartSectorIdxInBlk;
                    }
                    else if(u16_StartSectorIdxInBlk <
                        (pNandDrv->aWBQ_t[u8_i].u16_PagePos << pNandDrv->u8_PageSectorCntBits) +
                        pNandDrv->aWBQ_t[u8_i].u16_SectorPos)
                    {
                        u32_Row = pNandDrv->u16_PPB << pNandDrv->u8_BlkPageCntBits;
                        u32_Row += pNandDrv->aWBQ_t[u8_i].u16_PPBPageIdx;

                        u16_SectorCnt = pNandDrv->aWBQ_t[u8_i].u16_SectorPos;
                        u16_SectorCnt = u16_SectorCnt > u32_SectorCnt ? u32_SectorCnt : u16_SectorCnt;
                    }
                    else
                    {
                        u16_PBA = pNandDrv->aWBQ_t[u8_i].u16_OldPBA;
                        u32_Row = (u16_PBA << pNandDrv->u8_BlkPageCntBits) +
                            (u16_StartSectorIdxInBlk >> pNandDrv->u8_PageSectorCntBits);
                    }
                }
                else
                {   // in WBQ, but not related to PPB
                    if(u16_StartSectorIdxInBlk >=
                        (pNandDrv->aWBQ_t[u8_i].u16_PagePos << pNandDrv->u8_PageSectorCntBits))
                    {
                        u16_PBA = pNandDrv->aWBQ_t[u8_i].u16_OldPBA;
                        u32_Row = (u16_PBA << pNandDrv->u8_BlkPageCntBits) +
                            (u16_StartSectorIdxInBlk >> pNandDrv->u8_PageSectorCntBits);
                    }
                }
            }
        }
        else
        #endif
        {
            //--------------------------------------------
            u16_StartSectorIdxInBlk = u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask;
            u32_Row = (u16_PBA << pNandDrv->u8_BlkPageCntBits) +
                (u16_StartSectorIdxInBlk >> pNandDrv->u8_PageSectorCntBits);
            u16_SectorCnt = (U16)u32_SectorCnt;
        }

        //--------------------------------------------
        // partial read
        if(u16_StartSectorIdxInBlk & pNandDrv->u16_PageSectorCntMask)
        {
            u16_OneTimeSectorCnt = pNandDrv->u16_PageSectorCnt -
                (u16_StartSectorIdxInBlk & pNandDrv->u16_PageSectorCntMask);

            u16_OneTimeSectorCnt = u16_OneTimeSectorCnt > u16_SectorCnt ?
                u16_SectorCnt : u16_OneTimeSectorCnt;

            ftl_rw_debug(nand_debug(0,1,"partial 1: %lX %X \n",
                (u32_StartSectorIdx & pNandDrv->u16_PageSectorCntMask), u16_OneTimeSectorCnt));

            u32_Ret = nand_ReadSectors_Raw(u32_Row,
                u16_StartSectorIdxInBlk & pNandDrv->u16_PageSectorCntMask,
                (U8*)u32_RWBufferAddress, (U8*)NULL, u16_OneTimeSectorCnt);
            if(u32_Ret != UNFD_ST_SUCCESS)
            {
                if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                    nand_ECC_ErrorHandler(u32_Row >> pNandDrv->u8_BlkPageCntBits);
                goto Exit;
            }
        }
        else
        {
            u16_OneTimeSectorCnt =
                u16_SectorCnt > (U32)pNandDrv->u16_BlkSectorCnt-(U32)u16_StartSectorIdxInBlk ?
                (U32)pNandDrv->u16_BlkSectorCnt-(U32)u16_StartSectorIdxInBlk : u16_SectorCnt;

            u16_OneTimePageCnt = u16_OneTimeSectorCnt >> pNandDrv->u8_PageSectorCntBits;

            if(u16_OneTimePageCnt)
            {
                ftl_rw_debug(nand_debug(0,1,"page R. %X \n", u16_OneTimePageCnt));
                // page read
                u32_Ret = nand_ReadPages_Raw(u32_Row, (U8*)u32_RWBufferAddress, NULL, u16_OneTimePageCnt);
                if(UNFD_ST_SUCCESS != u32_Ret)
                {
                    if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                        nand_ECC_ErrorHandler(u32_Row>>pNandDrv->u8_BlkPageCntBits);
                    goto Exit;
                }

                u16_OneTimeSectorCnt = u16_OneTimePageCnt << pNandDrv->u8_PageSectorCntBits;
            }
            else
            {   // partial read
                ftl_rw_debug(nand_debug(0,1,"partial 2: %lX %X \n",
                    (u32_StartSectorIdx & pNandDrv->u16_PageSectorCntMask), u16_OneTimeSectorCnt));
                u32_Ret = nand_ReadSectors_Raw(u32_Row, 0,
                    (U8*)u32_RWBufferAddress, NULL, u16_OneTimeSectorCnt);
                if(UNFD_ST_SUCCESS != u32_Ret)
                {
                    if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                        nand_ECC_ErrorHandler(u32_Row >> pNandDrv->u8_BlkPageCntBits);
                    goto Exit;
                }
            }
        }

        //--------------------------------------------
        u32_StartSectorIdx += u16_OneTimeSectorCnt;
        u32_SectorCnt -= u16_OneTimeSectorCnt;
        u32_RWBufferAddress += u16_OneTimeSectorCnt<<9;
    }

    #if 0//UNFD_FTL_WBQ
    drvNAND_SendMsg2BkgndTask(NAND_MSG_TOGGLE_BUSY);
    #endif

    #if UNFD_FTL_RD
    if(0 != pNandDrv->u16_RD_SrcPBA)
    {
        //nand_dump_FreeBlk(0);
        //drvNAND_SendMsg2BkgndTask(NAND_MSG_READ_DISTURBANCE);
        nand_ReadDisturbance_ftl();
        pNandDrv->u16_RD_SrcPBA = 0;
        pNandDrv->u8_RD_ECCBitsCnt = 0;
        pNandDrv->u8_RD_ActiveFlag = 0;
    }
    #endif

    #if NAND_DMA_RACING_PATCH
    drvNAND_CheckMIUSignature(pu32_Addr0, pu32_Addr1);
    #endif

    //MsOS_ReleaseMutex(_s32UnfdTaskMutexId);
    //printf("[%s][%d] ===== MsOS_ReleaseMutex rr =====\n", __FUNCTION__, __LINE__);
    return UNFD_ST_SUCCESS;

Exit:

    //MsOS_ReleaseMutex(_s32UnfdTaskMutexId);
    //printf("[%s][%d] ===== MsOS_ReleaseMutex rr =====\n", __FUNCTION__, __LINE__);
    return u32_Ret;
}


U32 nand_get_partition_sector_cnt(U32 u32_PartNo)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U32 u32_SectorCnt;

    if(u32_PartNo >= UNFD_MAX_DISK_NUM)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR: Invalid partition number!\n");
        return 0;
    }
    u32_SectorCnt = pNandDrv->DiskInfo_t[u32_PartNo].u32_SectorCnt;

    nand_debug(UNFD_DEBUG_LEVEL_LOW, 1, "PartNo: 0x%02lX, Capacity: 0x%08lX sectors\n",
        u32_PartNo, u32_SectorCnt);

    return (u32_SectorCnt);
}


// return 0xFFFF: no free page
static U16 nand_FindFreePage(U16 u16_PBA) // binary search
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    DATA_BLK_SPARE_t *pDataBlkSpare_t = (P_DATA_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    U8  *pu8_DataBuf = pNandDrv->pu8_PageDataBuf;
    U8  u8_i;
    S8  s8_Dir=0;
    #if UNFD_FTL_SPEED_UP
    register U32 u32_Row;
    #else
    U32 u32_Row;
    #endif
    U32 u32_Ret, u32_FinalRow;

    //----------------------------------
    // binary search
    u32_Row = u16_PBA << pNandDrv->u8_BlkPageCntBits;
    u32_Row += pNandDrv->u16_BlkPageCnt>>1;

    // [CAUTION]: for 2 pages/blk NAND, need to update this routine
    for(u8_i=0; u8_i<pNandDrv->u8_BlkPageCntBits-1; u8_i++)
    {
        u32_Ret = nand_ReadSectors_Raw(u32_Row, 0, pu8_DataBuf, (U8*)pDataBlkSpare_t, 1);

        if(0xFFFF==pDataBlkSpare_t->u16_LBA && UNFD_ST_SUCCESS==u32_Ret)
        {
            s8_Dir = -1;
            u32_Row -= pNandDrv->u16_BlkPageCnt >> (u8_i+2);
        }
        else
        {
            s8_Dir = 1;
            u32_Row += pNandDrv->u16_BlkPageCnt >> (u8_i+2);
        }
    }

    //----------------------------------
    // 2nd search
    if(s8_Dir < 0)
    {
        u32_FinalRow = u16_PBA << pNandDrv->u8_BlkPageCntBits;
        do
        {
            u32_Ret = nand_ReadSectors_Raw(u32_Row, 0, pu8_DataBuf, (U8*)pDataBlkSpare_t, 1);

            if(0xFFFF!=pDataBlkSpare_t->u16_LBA || UNFD_ST_SUCCESS!=u32_Ret)
                break;

            u32_Row--;
        }
        while(u32_Row >= u32_FinalRow);

        return (U8)((u32_Row+1)&pNandDrv->u16_BlkPageCntMask);
    }
    else
    {
        u32_FinalRow = (u16_PBA+1) << pNandDrv->u8_BlkPageCntBits;
        do
        {
            u32_Ret = nand_ReadSectors_Raw(u32_Row, 0, pu8_DataBuf, (U8*)pDataBlkSpare_t, 1);

            if(0xFFFF == pDataBlkSpare_t->u16_LBA && UNFD_ST_SUCCESS==u32_Ret)
                return (U8)((u32_Row)&pNandDrv->u16_BlkPageCntMask);

            u32_Row++;
        }
        while(u32_Row < u32_FinalRow);
    }

    //----------------------------------
    return 0xFFFF; // no free page
}


// ----------------------------------------
// call this API, only when we did not have
// RootBlk or FTL version miss.
// ----------------------------------------
#if 0
#define ROOT_BLK_WORNOUT_CHECK(u32_Cnt)    {\
            u32_Cnt++;                      \
            if(UNFD_ROOT_BLK_CNT == u32_Cnt)\
            {                               \
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Fatal ERROR, NAND, 4 blk for RootBlk are worn out!!\n");\
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Fatal ERROR, NAND, 4 blk for RootBlk are worn out!!\n");\
                nand_die();\
            }}
#define ROOT_BLK_ERROR_HANDLER(u32_Cnt) {   \
            ROOT_BLK_WORNOUT_CHECK(u32_Cnt) \
            pNandDrv->u16_RootBlkPhyIdx++;  \
            if(pNandDrv->u16_RootBlkPhyIdx >= pNandDrv->u16_FirstPBA_LogiArea+UNFD_ROOT_BLK_CNT)\
                pNandDrv->u16_RootBlkPhyIdx = pNandDrv->u16_FirstPBA_LogiArea;}
#endif

#if UNFD_FTL_FMAP
static U32 nand_FM_WriteFlow(U32 u32_StartSectorIdx, U32 u32_SectorCnt, U32 u32_RWBufferAddress)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8  *pu8_DataBuf = pNandDrv->pu8_PageDataBuf;
    FMAP_BLK_SPARE_t *pFMapBlkSpare_t = (P_FMAP_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    U8  u8_i, u8_j, u8_page, u8_SecPos, u8_SecCnt;
    U32 u32_Row, u32_SrcRow, u32_DestRow, u32_Ret;
    U16 u16_LBA, u16_SectorInBlk;

    if(pNandDrv->u8_FM_init == 0)
    {
        nand_FM_Init();
        pNandDrv->u8_FM_init = 1;
    }

    pFMapBlkSpare_t->u8_BadBlkMark = 0xFF;
    pFMapBlkSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_FMAP;
    pFMapBlkSpare_t->u8_RDD5 = 0xFF;

    //nand_dump_FMap();

    //nand_debug(0,1,"StartSec: %X  SecCnt: %X\n", u32_StartSectorIdx, u32_SectorCnt);

    while(u32_SectorCnt > 0) // one loop for one page
    {
        //-------------------------------
        u16_LBA = u32_StartSectorIdx >> pNandDrv->u8_BlkSectorCntBits;
        u16_SectorInBlk = u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask;

        u8_i = (U8)(u16_LBA - pNandDrv->u16_FM_FirstLBA);
        u8_page = (U8)(u16_SectorInBlk>> pNandDrv->u8_PageSectorCntBits);
        u8_j = u8_page / pNandDrv->u8_FM_PBAPageCnt;

        u8_SecPos = u16_SectorInBlk & pNandDrv->u16_PageSectorCntMask;
        u8_SecCnt = ((U32)pNandDrv->u16_PageSectorCnt- (U32)u8_SecPos) > u32_SectorCnt ?
            u32_SectorCnt : ((U32)pNandDrv->u16_PageSectorCnt- (U32)u8_SecPos);

        u32_Row = pNandDrv->au16_FM_BL2P[u8_i][u8_j] << pNandDrv->u8_BlkPageCntBits;
        u32_DestRow = u32_Row + pNandDrv->au8_FM_FreePagePos[u8_i][u8_j];

        pFMapBlkSpare_t->u16_LBA = u16_LBA;
        pFMapBlkSpare_t->u8_PageIdx = u8_page;

        //-------------------------------
        if(0 == u8_SecPos)
        {
            //nand_debug(0,1,"i:%X, j:%X, page:%X, Row:%X, DRow:%X, SecPos:%X, SecCnt:%X\n",
            //    u8_i, u8_j, u8_page, u32_Row, u32_DestRow, u8_SecPos, u8_SecCnt);

            if(u8_SecCnt == pNandDrv->u16_PageSectorCnt)
                u32_Ret = nand_WritePages_Raw(u32_DestRow,
                    (U8*)u32_RWBufferAddress, (U8*)pFMapBlkSpare_t, 1);
            else
            {   // copy data
                memcpy((void*)pu8_DataBuf,
                    (const void*)u32_RWBufferAddress, u8_SecCnt<<9);
                // copy NAND
                u32_SrcRow = u32_Row + pNandDrv->au8_FM_PL2P[u8_i][u8_page];
                u32_Ret = nand_ReadSectors_Raw(u32_SrcRow, u8_SecCnt,
                    pu8_DataBuf+(u8_SecCnt<<9), NULL, pNandDrv->u16_PageSectorCnt-u8_SecCnt);
                if(UNFD_ST_SUCCESS != u32_Ret)
                {
                    nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR d, %lX\n", u32_Ret);
                    if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                        nand_ECC_ErrorHandler(u32_SrcRow >> pNandDrv->u8_BlkPageCntBits);
                    return u32_Ret;
                }
                // write NAND
                u32_Ret = nand_WritePages_Raw(u32_DestRow,
                    pu8_DataBuf, (U8*)pFMapBlkSpare_t, 1);
            }
        }
        else
        {	
            U8 u8_temp;

            // copy NAND
            u32_SrcRow = u32_Row + pNandDrv->au8_FM_PL2P[u8_i][u8_page];
            u32_Ret = nand_ReadSectors_Raw(u32_SrcRow, 0, pu8_DataBuf, NULL, u8_SecPos);
            if(UNFD_ST_SUCCESS != u32_Ret)
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR a, %lX\n", u32_Ret);
                if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                    nand_ECC_ErrorHandler(u32_SrcRow >> pNandDrv->u8_BlkPageCntBits);
                return u32_Ret;
            }

            // copy data
            memcpy((void*)(pu8_DataBuf+(u8_SecPos<<9)),
                (const void*)u32_RWBufferAddress, u8_SecCnt<<9);

            // copy NAND
            u8_temp = u8_SecPos + u8_SecCnt;
            if(u8_temp < pNandDrv->u16_PageSectorCnt)
            {
                u32_Ret = nand_ReadSectors_Raw(u32_SrcRow, u8_temp,
                    pu8_DataBuf+(u8_temp<<9), NULL, pNandDrv->u16_PageSectorCnt-u8_temp);
                if(UNFD_ST_SUCCESS != u32_Ret)
                {
                    nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR b, %lX\n", u32_Ret);
                    if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                        nand_ECC_ErrorHandler(u32_SrcRow >> pNandDrv->u8_BlkPageCntBits);
                    return u32_Ret;
                }
            }

            //nand_debug(0,1,"i:%X, j:%X, page:%X, Row:%X, SRow:%X, DRow:%X, SecPos:%X, SecCnt:%X\n",
            //    u8_i, u8_j, u8_page, u32_Row, u32_SrcRow, u32_DestRow, u8_SecPos, u8_SecCnt);

            // write NAND
            u32_Ret = nand_WritePages_Raw(u32_DestRow, pu8_DataBuf, (U8*)pFMapBlkSpare_t, 1);
        }

        //-------------------------------
        // update
        pNandDrv->au8_FM_PL2P[u8_i][u8_page] = pNandDrv->au8_FM_FreePagePos[u8_i][u8_j];
        pNandDrv->au8_FM_FreePagePos[u8_i][u8_j] += 1;
        if(pNandDrv->au8_FM_FreePagePos[u8_i][u8_j] >= pNandDrv->u16_BlkPageCnt)
            nand_FM_MoveBlk(u8_i, u8_j);

        // error handle
        if(UNFD_ST_SUCCESS != u32_Ret)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR c, %lX\n", u32_Ret);
            return u32_Ret;
        }

        u32_StartSectorIdx += u8_SecCnt;
        u32_SectorCnt -= u8_SecCnt;
        u32_RWBufferAddress += u8_SecCnt << 9;
    }

    return UNFD_ST_SUCCESS;
}

static U32 nand_FM_ReadFlow(U32 u32_StartSectorIdx, U32 u32_SectorCnt, U32 u32_RWBufferAddress)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8  u8_i, u8_j, u8_page, u8_SecPos, u8_SecCnt;
    U32 u32_Row, u32_Ret;
    U16 u16_SectorInBlk;

    if(pNandDrv->u8_FM_init == 0)
    {
        nand_FM_Init();
        pNandDrv->u8_FM_init = 1;
    }

    //nand_dump_FMap();
    //nand_debug(0,1,"StartSec: %X  SecCnt: %X\n", u32_StartSectorIdx, u32_SectorCnt);

    while(u32_SectorCnt > 0) // one loop for one page
    {
        u16_SectorInBlk = u32_StartSectorIdx & pNandDrv->u16_BlkSectorCntMask;

        u8_i = (U8)((u32_StartSectorIdx >> pNandDrv->u8_BlkSectorCntBits) -
            pNandDrv->u16_FM_FirstLBA);
        u8_page = (U8)(u16_SectorInBlk >> pNandDrv->u8_PageSectorCntBits);
        u8_j = u8_page / pNandDrv->u8_FM_PBAPageCnt;

        u32_Row = (pNandDrv->au16_FM_BL2P[u8_i][u8_j] << pNandDrv->u8_BlkPageCntBits) +
            pNandDrv->au8_FM_PL2P[u8_i][u8_page];

        u8_SecPos = u16_SectorInBlk & pNandDrv->u16_PageSectorCntMask;
        u8_SecCnt = ((U32)pNandDrv->u16_PageSectorCnt - (U32)u8_SecPos) > u32_SectorCnt ?
            u32_SectorCnt : ((U32)pNandDrv->u16_PageSectorCnt - (U32)u8_SecPos);

        if(u8_SecPos != 0 || u8_SecCnt != pNandDrv->u16_PageSectorCnt)
            u32_Ret = nand_ReadSectors_Raw(u32_Row, u8_SecPos, (U8*)u32_RWBufferAddress, NULL, u8_SecCnt);
        else
            u32_Ret = nand_ReadPages_Raw(u32_Row, (U8*)u32_RWBufferAddress, NULL, 1);

        //nand_debug(0,1,"i:%X, j:%X, page:%X, Row:%X, SecPos:%X, SecCnt:%X\n",
        //	    u8_i, u8_j, u8_page, u32_Row, u8_SecPos, u8_SecCnt);
        if(UNFD_ST_SUCCESS != u32_Ret)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, %lX\n", u32_Ret);

            if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                nand_ECC_ErrorHandler(u32_Row>>pNandDrv->u8_BlkPageCntBits);

            return u32_Ret;
        }

        u32_StartSectorIdx += u8_SecCnt;
        u32_SectorCnt -= u8_SecCnt;
        u32_RWBufferAddress += u8_SecCnt << 9;
    }

    return UNFD_ST_SUCCESS;
}


void nand_dump_FMap(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

    nand_debug(0,0,"\n");
    nand_debug(0,1,"FM_FirstLBA: %X, FM_LBACnt: %X, FM_PBACnt: %X, FM_PBAPageCnt: %X \n",
        pNandDrv->u16_FM_FirstLBA, pNandDrv->u16_FM_LBACnt, pNandDrv->u16_FM_PBACnt, pNandDrv->u8_FM_PBAPageCnt);
    nand_debug(0,1,"FM_BL2P: \n");
    dump_mem((U8*)pNandDrv->au16_FM_BL2P, pNandDrv->u16_FM_PBACnt<<1);
    nand_debug(0,1,"FM_FreePagePos: \n");
    dump_mem((U8*)pNandDrv->au8_FM_FreePagePos, pNandDrv->u16_FM_PBACnt);
    nand_debug(0,1,"FM_PL2P: \n");
    dump_mem((U8*)pNandDrv->au8_FM_PL2P, pNandDrv->u16_FM_LBACnt << pNandDrv->u8_BlkPageCntBits);
    nand_debug(0,0,"\n");
}

static void nand_FM_MoveBlk(U8 u8_i, U8 u8_j)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    FMAP_BLK_SPARE_t *pFMapBlkSpare_t = (P_FMAP_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    U8 *pu8_DataBuf = pNandDrv->pu8_PageDataBuf;

    U16 u16_OldPBA, u16_FreePBA, u16_Page;
    U32 u32_OldRow, u32_SrcRow, u32_FreeRow, u32_DestRow;
    U32 u32_Ret;
    U8  u8_p0, u8_p1, u8_p;

    //nand_dump_FMap();

    u16_OldPBA = pNandDrv->au16_FM_BL2P[u8_i][u8_j];
    u32_OldRow = u16_OldPBA << pNandDrv->u8_BlkPageCntBits;

    u8_p0 = pNandDrv->u8_FM_PBAPageCnt * u8_j;
    u8_p1 = u8_p0 + pNandDrv->u8_FM_PBAPageCnt;

    LABEL_START_MOVE:
    u16_FreePBA = nand_get_FreeBlk_Zone0();
    u32_FreeRow = u16_FreePBA << pNandDrv->u8_BlkPageCntBits;
    u16_Page = 0;

    //nand_debug(0,1,"i:%X, j:%X, p0:%X, p1:%X, OBlk:%X, ORow:%X, FBlk:%X, FRow:%X\n",
    //		    u8_i, u8_j, u8_p0, u8_p1,
    //		    u16_OldPBA, u32_OldRow, u16_FreePBA, u32_FreeRow);
    //nand_dump_FMap();

    for(u8_p=u8_p0; u8_p < u8_p1; u8_p++)
    {
        u32_SrcRow = u32_OldRow + pNandDrv->au8_FM_PL2P[u8_i][u8_p];
        u32_Ret = nand_ReadPages_Raw(u32_SrcRow, pu8_DataBuf, (U8*)pFMapBlkSpare_t, 1);

        //nand_debug(0,1,"SRow:%X, p:%X, PageIdx:%X \n", u32_SrcRow, u8_p, pFMapBlkSpare_t->u8_PageIdx);

        if(UNFD_ST_SUCCESS == u32_Ret && u8_p == pFMapBlkSpare_t->u8_PageIdx)
        {
            u32_DestRow = u32_FreeRow + u16_Page;

            u32_Ret = nand_WritePages_Raw(u32_DestRow, pu8_DataBuf, (U8*)pFMapBlkSpare_t, 1);
            if(UNFD_ST_SUCCESS != u32_Ret)
            {

                nand_debug(UNFD_DEBUG_LEVEL_WARNING,1,"Write Error\n");
                goto LABEL_START_MOVE;
            }
            pNandDrv->au8_FM_PL2P[u8_i][u8_p] = (U8)u16_Page;
            u16_Page++;

        }
        #if UNFD_FTL_CHECK_ERROR
        else if(UNFD_ST_SUCCESS != u32_Ret)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, should not happen! st:%lX, p:%X, fp:%X\n",
                u32_Ret, u8_p, pFMapBlkSpare_t->u8_PageIdx);

            if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                nand_ECC_ErrorHandler(u32_SrcRow>>pNandDrv->u8_BlkPageCntBits);
        }
        #endif
    }

    nand_erase_and_set_FreeBlk(u16_OldPBA);
    pNandDrv->au16_FM_BL2P[u8_i][u8_j] = u16_FreePBA;
    pNandDrv->au8_FM_FreePagePos[u8_i][u8_j] = (U8)u16_Page;
}


static U32 nand_format_FMapBlk(U16 u16_PBA, U16 u16_LBA)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    FMAP_BLK_SPARE_t *pFMapBlkSpare_t = (P_FMAP_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    U8 *pu8_DataBuf = pNandDrv->pu8_PageDataBuf;
    U32 u32_Ret, u32_Row;

    pFMapBlkSpare_t->u8_BadBlkMark = 0xFF;
    pFMapBlkSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_FMAP;
    pFMapBlkSpare_t->u16_LBA = u16_LBA;
    pFMapBlkSpare_t->u8_PageIdx = 0;
    pFMapBlkSpare_t->u8_RDD5 = 0xFF;

    u32_Row = u16_PBA << pNandDrv->u8_BlkPageCntBits;
    //u32_Ret = nand_WriteSectors_Raw(u32_Row, 0, pu8_DataBuf, (U8*)pFMapBlkSpare_t, 1);
    u32_Ret = nand_WritePages_Raw(u32_Row, pu8_DataBuf, (U8*)pFMapBlkSpare_t, 1);
    if(UNFD_ST_SUCCESS != u32_Ret)
    {
        if(u32_Ret == UNFD_ST_ERR_W_FAIL)
            drvNAND_MarkBadBlk(u16_PBA);
        return u32_Ret;
    }
    #if 0
    u32_Row += pNandDrv->u16_BlkPageCnt-1;
    u32_Ret = NC_WriteSectors(u32_Row, 0, pu8_DataBuf, (U8*)pFMapBlkSpare_t, 1);
    if(UNFD_ST_SUCCESS != u32_Ret)
    {
        drvNAND_MarkBadBlk(u16_PBA);
        return u32_Ret;
    }
    #endif

    return UNFD_ST_SUCCESS;
}

static void nand_FM_Init(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
    DATA_BLK_SPARE_t *pDataBlkSpare_t = (P_DATA_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    FMAP_BLK_SPARE_t *pFMapBlkSpare_t = (P_FMAP_BLK_SPARE_t)pDataBlkSpare_t;
    U16 u16_PBA;
    U32 u32_Row, u32_Temp, u32_Ret;
    U8  u8_i, u8_j, u8_k;
    U16 u16_FreePage;
    U32 u32_Flag;

    // setup au8_FM_PBAPagePos, au8_FM_PL2P
    for(u8_i=0; u8_i<pNandDrv->u16_FM_LBACnt; u8_i++)
        for(u8_j=0; u8_j<FMAP_SPACE_FACTOR; u8_j++)
        {
            u16_PBA = pNandDrv->au16_FM_BL2P[u8_i][u8_j];

            if(0 != u16_PBA)
            {
                // find the free page
                u16_FreePage = nand_FindFreePage(u16_PBA);
                //nand_debug(0,1,"i:%X, j:%X, FreePage:%X \n", u8_i, u8_j, u16_FreePage);
                if(0xFFFF == u16_FreePage)
                    u8_k = (U8)pNandDrv->u16_BlkPageCnt;
                else
                    u8_k = (U8)u16_FreePage;

                pNandDrv->au8_FM_FreePagePos[u8_i][u8_j] = u8_k;

                u32_Flag = (1<<pNandDrv->u8_FM_PBAPageCnt)-1;
                u32_Temp = u16_PBA << pNandDrv->u8_BlkPageCntBits;

                for( ; u8_k>0; u8_k--)
                {
                    u32_Row = u32_Temp + u8_k - 1;

                    u32_Ret = nand_ReadSectors_Raw(u32_Row, 0, au8_PageBuf, (U8*)pFMapBlkSpare_t, 1);
                    if(UNFD_ST_SUCCESS == u32_Ret)
                    {
                        if(0==pNandDrv->au8_FM_PL2P[u8_i][pFMapBlkSpare_t->u8_PageIdx])
                        {
                            pNandDrv->au8_FM_PL2P[u8_i][pFMapBlkSpare_t->u8_PageIdx] = u8_k-1;
                            u32_Flag &= ~((U32)1 << (pFMapBlkSpare_t->u8_PageIdx/pNandDrv->u8_FM_PBAPageCnt));
                            if(0 == u32_Flag)
                                break;
                        }
                    }
                }

                if(0xFFFF == u16_FreePage)
                {   // no free page -> PBA full, move to another free blk
                    //nand_debug(0,1,"FM PBA full, i:%X, j:%X \n", u8_i, u8_j);
                    nand_FM_MoveBlk(u8_i, u8_j);
                }
            }
            else
            {
                pNandDrv->au16_FM_BL2P[u8_i][u8_j] = nand_get_FreeBlk_Zone0();
                pNandDrv->au8_FM_FreePagePos[u8_i][u8_j] = 0;
            }
        }
    //nand_dump_FMap();

}
#endif

#if UNFD_FTL_WL
static U32 nand_format_ECBlk(U16 u16_PBA, U8 u8_ECntIdx)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    WLECNT_BLK_SPARE_t *pWLECNTBlkSpare_t = (P_WLECNT_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    U32 u32_Ret, u32_Row;
    U8  u8_i;

    memset((void*)&gu16_Erase_Counter[u8_ECntIdx*1024], 0, 1024*2);
    u32_Row = u16_PBA << pNandDrv->u8_BlkPageCntBits;

    if (pNandDrv->u16_PageByteCnt == 512) // small blk
    {
        for (u8_i = 0; u8_i < 4; u8_i++)
        {
            pWLECNTBlkSpare_t->u8_BadBlkMark = 0xFF;
            pWLECNTBlkSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_WLECNT;
            pWLECNTBlkSpare_t->u8_ECntIdx = u8_ECntIdx;
            pWLECNTBlkSpare_t->u8_ECntSubIdx = u8_i;
            pWLECNTBlkSpare_t->u8_RDD4 = 0xFF;
            pWLECNTBlkSpare_t->u8_RDD5 = 0xFF;
            u32_Ret = nand_WritePages_Raw(u32_Row + u8_i, (U8*)&gu16_Erase_Counter[u8_ECntIdx*1024+(u8_i<<8)],
                (U8*)pWLECNTBlkSpare_t, 1);
            if(UNFD_ST_SUCCESS != u32_Ret)
            {
                if(u32_Ret == UNFD_ST_ERR_W_FAIL)
                    drvNAND_MarkBadBlk(u16_PBA);
                return u32_Ret;
            }
        }
        pNandDrv->au16_WL_ECntPBA_Page[u8_ECntIdx] = 3;
    }
    else // big blk
    {
        pWLECNTBlkSpare_t->u8_BadBlkMark = 0xFF;
        pWLECNTBlkSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_WLECNT;
        pWLECNTBlkSpare_t->u8_ECntIdx = u8_ECntIdx;
        pWLECNTBlkSpare_t->u8_ECntSubIdx = 0xFF;
        pWLECNTBlkSpare_t->u8_RDD4 = 0xFF;
        pWLECNTBlkSpare_t->u8_RDD5 = 0xFF;
        u32_Ret = nand_WritePages_Raw(u32_Row , (U8*)&gu16_Erase_Counter[u8_ECntIdx*1024],
            (U8*)pWLECNTBlkSpare_t, 1);
        if(UNFD_ST_SUCCESS != u32_Ret)
        {
            if(u32_Ret == UNFD_ST_ERR_W_FAIL)
                drvNAND_MarkBadBlk(u16_PBA);
            return u32_Ret;
        }

        pNandDrv->au16_WL_ECntPBA_Page[u8_ECntIdx] = 0;
    }

    pNandDrv->au16_WL_ECntPBA[u8_ECntIdx] = u16_PBA;

    return UNFD_ST_SUCCESS;
}


static U16 nand_DuplicateECHandler(U16 u16_PBA0, U16 u16_PBA1, U8 u8_ECntIdx)
{
    U16 u16_EraseBlk, u16_RetBlk;
    U16 u16_FreePage0, u16_FreePage1;

    nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"NAND, duplicate PBA %X %X mapped to a EC %X \n", u16_PBA0, u16_PBA1, u8_ECntIdx);

    u16_FreePage0 = nand_FindFreePage(u16_PBA0);
    u16_FreePage1 = nand_FindFreePage(u16_PBA1);

    if(u16_FreePage0 > u16_FreePage1)
    {
        u16_EraseBlk = u16_PBA1;
        u16_RetBlk = u16_PBA0;
    }
    else
    {
        u16_EraseBlk = u16_PBA0;
        u16_RetBlk = u16_PBA1;
    }

    nand_erase_and_set_FreeBlk(u16_EraseBlk);

    nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"EraseBlk: %X  RetBlk: %X \n", u16_EraseBlk, u16_RetBlk);
    return u16_RetBlk;
}

static U32 nand_FindMaxECntFreeBlkInZone0(U16 *pu16_PBA, U16 *pu16_MaxECnt, U8 *pu8_Pos)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8  u8_index;
    U8  u8_i;
    U16 u16_MaxEC;

    if(pNandDrv->u8_FBCnt_0 == 0)
    {
        // [CAUTION]: fatal error (should never be here, unless too many bad blk)
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, Zone 0 has no free Blk\n");
        nand_die();
    }

    // get max ECnt from free blocks
    u8_index = pNandDrv->u8_FBHead_0;
    u16_MaxEC = 0;
    *pu8_Pos = 0;
    *pu16_PBA = pNandDrv->au16_FBIdx_0[pNandDrv->u8_FBHead_0];

    for (u8_i = 0; u8_i < pNandDrv->u8_FBCnt_0; u8_i++)
    {
        //comapre
        if(gu16_Erase_Counter[pNandDrv->au16_FBIdx_0[u8_index] - pNandDrv->u16_FirstPBA_Zone0]
            > u16_MaxEC)
        {
            u16_MaxEC = gu16_Erase_Counter[pNandDrv->au16_FBIdx_0[u8_index] - pNandDrv->u16_FirstPBA_Zone0];
            *pu8_Pos = u8_i;
            *pu16_PBA = pNandDrv->au16_FBIdx_0[u8_index];
        }

        u8_index++;
        if(u8_index == pNandDrv->u8_FBMaxCnt_0)
        {
            u8_index = 0;
        }
    }

    *pu16_MaxECnt = u16_MaxEC;

    return UNFD_ST_SUCCESS;
}

static U32 nand_delete_FreeBlk_Zone0(U8 u8_FBPos)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8 u8_i;

    if(u8_FBPos >= pNandDrv->u8_FBCnt_0)
    {
        // [CAUTION]: fatal error (should never be here, unless too many bad blk)
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, Zone 0 has no free Blk");
        nand_die();
    }

    for (u8_i = 0; u8_i < pNandDrv->u8_FBCnt_0-u8_FBPos-1; u8_i++)
    {
        pNandDrv->au16_FBIdx_0[(pNandDrv->u8_FBHead_0+u8_FBPos+u8_i)%pNandDrv->u8_FBMaxCnt_0]
            = pNandDrv->au16_FBIdx_0[(pNandDrv->u8_FBHead_0+u8_FBPos+u8_i+1)%pNandDrv->u8_FBMaxCnt_0];
    }

    pNandDrv->u8_FBCnt_0--;
    if(pNandDrv->u8_FBTail_0 == 0)
        pNandDrv->u8_FBTail_0 = pNandDrv->u8_FBMaxCnt_0-1;
    else
        pNandDrv->u8_FBTail_0--;

    return UNFD_ST_SUCCESS;
}

static U32 nand_FindMaxECntFreeBlkInZone1(U16 *pu16_PBA, U16 *pu16_MaxECnt, U8 *pu8_Pos)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8 u8_index;
    U8 u8_i;
    U16 u16_MaxEC;

    if(pNandDrv->u8_FBCnt_1 == 0)
    {
        // [CAUTION]: fatal error (should never be here, unless too many bad blk)
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, Zone 1 has no free Blk");
        nand_die();

        return UNFD_ST_ERR_LACK_BLK;
    }

    ////get max erase counter of free block
    u8_index = pNandDrv->u8_FBHead_1;
    u16_MaxEC = 0;
    *pu8_Pos = 0;
    *pu16_PBA = pNandDrv->au16_FBIdx_1[pNandDrv->u8_FBHead_1];

    for (u8_i = 0; u8_i < pNandDrv->u8_FBCnt_1; u8_i++)
    {
        //comapre
        if (gu16_Erase_Counter[pNandDrv->au16_FBIdx_1[u8_index] - pNandDrv->u16_FirstPBA_Zone0] > u16_MaxEC)
        {
            u16_MaxEC = gu16_Erase_Counter[pNandDrv->au16_FBIdx_1[u8_index] - pNandDrv->u16_FirstPBA_Zone0];
            *pu8_Pos = u8_i;
            *pu16_PBA = pNandDrv->au16_FBIdx_1[u8_index];
        }

        u8_index++;
        if(u8_index == pNandDrv->u8_FBMaxCnt_1)
        {
            u8_index = 0;
        }
    }

    *pu16_MaxECnt = u16_MaxEC;

    return UNFD_ST_SUCCESS;
}

static U32 nand_delete_FreeBlk_Zone1(U8 u8_FBPos)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8 u8_i;
    U8 u8_FBMaxCnt_1;
    if(u8_FBPos >= pNandDrv->u8_FBCnt_1)
    {
        // [CAUTION]: fatal error (should never be here, unless too many bad blk)
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, Zone 1 has no free Blk");
        nand_die();

        return UNFD_ST_ERR_LACK_BLK;
    }

    u8_FBMaxCnt_1 = pNandDrv->u8_FBMaxCnt_1;
    for (u8_i = 0; u8_i < pNandDrv->u8_FBCnt_1-u8_FBPos-1; u8_i++)
    {
        pNandDrv->au16_FBIdx_1[(pNandDrv->u8_FBHead_1+u8_FBPos+u8_i)%u8_FBMaxCnt_1]
            = pNandDrv->au16_FBIdx_1[(pNandDrv->u8_FBHead_1+u8_FBPos+u8_i+1)%u8_FBMaxCnt_1];
    }

    pNandDrv->u8_FBCnt_1--;
    if(pNandDrv->u8_FBTail_1 == 0)
        pNandDrv->u8_FBTail_1 = u8_FBMaxCnt_1-1;
    else
        pNandDrv->u8_FBTail_1--;

    return UNFD_ST_SUCCESS;
}

void nand_SaveEraseCounter(U8 u8_EraseCounterZoneIndex)
{
    U16 u16_i;
    U16 u16_tmp = 0;
    U8  u8_need_erase = 0;
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    WLECNT_BLK_SPARE_t *pWLECNTBlkSpare_t = (P_WLECNT_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    U32 u32_Ret;

    pNandDrv->au16_WL_ECntPBA_Page[u8_EraseCounterZoneIndex]++;
    if(pNandDrv->au16_WL_ECntPBA_Page[u8_EraseCounterZoneIndex] >= pNandDrv->u16_BlkPageCnt)
    {
        u16_tmp = pNandDrv->au16_WL_ECntPBA[u8_EraseCounterZoneIndex];

        pNandDrv->au16_WL_ECntPBA[u8_EraseCounterZoneIndex] = nand_get_FreeBlk_Zone0();
        pNandDrv->au16_WL_ECntPBA_Page[u8_EraseCounterZoneIndex] = 0;
        u8_need_erase = 1;
        nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "Next EC[%X]: %X, %X \n", u8_EraseCounterZoneIndex,
            pNandDrv->au16_WL_ECntPBA[u8_EraseCounterZoneIndex],
            pNandDrv->au16_WL_ECntPBA_Page[u8_EraseCounterZoneIndex]);
    }

    u32_Ret = UNFD_ST_ERR_W_FAIL;
    while(u32_Ret !=UNFD_ST_SUCCESS)
    {
        if (pNandDrv->u16_PageByteCnt > 512)
        {
            pWLECNTBlkSpare_t->u8_BadBlkMark = 0xFF;
            pWLECNTBlkSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_WLECNT;
            pWLECNTBlkSpare_t->u8_ECntIdx = u8_EraseCounterZoneIndex;
            pWLECNTBlkSpare_t->u8_ECntSubIdx = 0xFF;
            pWLECNTBlkSpare_t->u8_RDD4 = 0xFF;
            pWLECNTBlkSpare_t->u8_RDD5 = 0xFF;

            nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "write blk : %X, page : %X \n", pNandDrv->au16_WL_ECntPBA[u8_EraseCounterZoneIndex], pNandDrv->au16_WL_ECntPBA_Page[u8_EraseCounterZoneIndex]);

            u32_Ret = nand_WritePages_Raw((pNandDrv->au16_WL_ECntPBA[u8_EraseCounterZoneIndex] << pNandDrv->u8_BlkPageCntBits) +
                pNandDrv->au16_WL_ECntPBA_Page[u8_EraseCounterZoneIndex],
                (U8*)&gu16_Erase_Counter[u8_EraseCounterZoneIndex*1024], (U8*)pWLECNTBlkSpare_t, 1);
            if(UNFD_ST_SUCCESS != u32_Ret)
            {
                if(u32_Ret == UNFD_ST_ERR_W_FAIL)
                    drvNAND_MarkBadBlk(pNandDrv->au16_WL_ECntPBA[u8_EraseCounterZoneIndex]);
            }
        }
        else
        {
            for (u16_i = 0; u16_i < 4; u16_i++)
            {
                pWLECNTBlkSpare_t->u8_BadBlkMark = 0xFF;
                pWLECNTBlkSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_WLECNT;
                pWLECNTBlkSpare_t->u8_ECntIdx = u8_EraseCounterZoneIndex;
                pWLECNTBlkSpare_t->u8_ECntSubIdx = u16_i;
                pWLECNTBlkSpare_t->u8_RDD4 = 0xFF;
                pWLECNTBlkSpare_t->u8_RDD5 = 0xFF;

                u32_Ret = nand_WritePages_Raw((pNandDrv->au16_WL_ECntPBA[u8_EraseCounterZoneIndex] << pNandDrv->u8_BlkPageCntBits) +
                    pNandDrv->au16_WL_ECntPBA_Page[u8_EraseCounterZoneIndex] + u16_i,
                    (U8*)&gu16_Erase_Counter[u8_EraseCounterZoneIndex*1024+(u16_i<<8)], (U8*)pWLECNTBlkSpare_t, 1);
                if(UNFD_ST_SUCCESS != u32_Ret)
                {
                    if(u32_Ret == UNFD_ST_ERR_W_FAIL)
                        drvNAND_MarkBadBlk(pNandDrv->au16_WL_ECntPBA[u8_EraseCounterZoneIndex]);
                    break;
                }
            }
            pNandDrv->au16_WL_ECntPBA_Page[u8_EraseCounterZoneIndex] += 3;
        }

        if(UNFD_ST_SUCCESS != u32_Ret)
        {
            pNandDrv->au16_WL_ECntPBA[u8_EraseCounterZoneIndex] = nand_get_FreeBlk_Zone0();
            pNandDrv->au16_WL_ECntPBA_Page[u8_EraseCounterZoneIndex] = 0;
            nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "Next EC[%X]: %X, %X \n", u8_EraseCounterZoneIndex,
                pNandDrv->au16_WL_ECntPBA[u8_EraseCounterZoneIndex],
                pNandDrv->au16_WL_ECntPBA_Page[u8_EraseCounterZoneIndex]);
        }
        else
        {
            break;
        }
    }

    if(u8_need_erase)
        nand_erase_and_set_FreeBlk(u16_tmp);

    nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "EC[%X]: %X, %X \n", u8_EraseCounterZoneIndex,
        pNandDrv->au16_WL_ECntPBA[u8_EraseCounterZoneIndex],
        pNandDrv->au16_WL_ECntPBA_Page[u8_EraseCounterZoneIndex]);

    pNandDrv->au32_ZoneTotalECnt[u8_EraseCounterZoneIndex] = 0;
}

void nand_Wear_Leveling(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    //U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
    //DATA_BLK_SPARE_t *pDataBlkSpare_t = (P_DATA_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    U8  u8_FBPos;
    U16 u16_MaxECnt, u16_MaxECnt_FreeBlk, u16_MinECnt, u16_MinECnt_PBA, u16_MinECnt_LBA;
    U16 u16_i;
    U32 u32_Ret;
    #if UNFD_PRINT_EC
    static U32 WL_loop=0;
    #endif

    printf("nand_Wear_Leveling\n");
    if(pNandDrv->u8_FBCnt_0 == 0)
    {
        nand_debug(UNFD_DEBUG_LEVEL, 1, "No Free block\n");
        nand_die();
    }

    #if UNFD_FTL_WBQ
    for(u16_i=0; u16_i<MAX_WBQ_CNT; u16_i++)
    {
        u32_Ret = nand_FlushWBQ(u16_i);
    }
    #endif

    // find FreeBlk has Max ECnt in Zone0
    u32_Ret = nand_FindMaxECntFreeBlkInZone0(&u16_MaxECnt_FreeBlk, &u16_MaxECnt, &u8_FBPos);
    if(u32_Ret != UNFD_ST_SUCCESS)
        return;
    //nand_debug(0, 1, "FBPos: %X, MaxECnt_FreeBlk: %X, MaxECnt: %X\n",
    //	u8_FBPos, u16_MaxECnt_FreeBlk, u16_MaxECnt);

    // find DataBlk has Min ECnt in Zone0
    u16_MinECnt = 0xFFFF;
    u16_MinECnt_LBA = 0;
    u16_MinECnt_PBA = pNandDrv->au16_L2P_0[0];
    for(u16_i = 0; u16_i < pNandDrv->u16_Zone0LBACnt; u16_i++)
    {
        #if UNFD_FTL_FMAP
        // skip searching FMAP blk
        if(u16_i >= pNandDrv->u16_FM_FirstLBA &&
            u16_i < pNandDrv->u16_FM_FirstLBA + pNandDrv->u16_FM_LBACnt)
            continue;
        #endif

        // comapre
        if (u16_MinECnt > gu16_Erase_Counter[pNandDrv->au16_L2P_0[u16_i] - pNandDrv->u16_FirstPBA_Zone0])
        {
            u16_MinECnt = gu16_Erase_Counter[pNandDrv->au16_L2P_0[u16_i] - pNandDrv->u16_FirstPBA_Zone0];
            u16_MinECnt_PBA = pNandDrv->au16_L2P_0[u16_i];
            u16_MinECnt_LBA = u16_i;
        }
    }
    //nand_debug(0, 1, "MinECnt_LBA: %X, MinECnt_PBA: %X, MinECnt: %X\n",
    //	u16_MinECnt_LBA, u16_MinECnt_PBA, u16_MinECnt);

    #if UNFD_FTL_RD // can be ignored
    if(u16_MinECnt_PBA == pNandDrv->u16_RDB)
    {
        nand_debug(UNFD_DEBUG_LEVEL,1,"WL Blk meets RD Blk: %X\n", pNandDrv->u16_RDB);
        return; // do nothing
    }
    #endif

    // check is exceed threshold
    if((u16_MaxECnt < u16_MinECnt) || ((u16_MaxECnt - u16_MinECnt) < WL_EC_THRESHOLD))
    {
        //nand_debug(0, 1, "Not exceed threshold\n");
        return;
    }

    #if 0
    ////check data blk
    NC_ReadSectors_Retry((u16_MinECnt_PBA << pNandDrv->u8_BlkPageCntBits), 0, au8_PageBuf, (U8*)pDataBlkSpare_t, 1);
    if (pDataBlkSpare_t->u8_BlkID != UNFD_FTL_BLK_ID_DATA)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"not data block: %X\n", pDataBlkSpare_t->u8_BlkID);

        nand_die();
        return;
    }

    ////check free blk
    NC_ReadSectors_Retry((u16_MaxECnt_FreeBlk << pNandDrv->u8_BlkPageCntBits), 0, au8_PageBuf, (U8*)pDataBlkSpare_t, 1);
    if (pDataBlkSpare_t->u8_BlkID != 0xFF)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"not free block: %X\n", pDataBlkSpare_t->u8_BlkID);

        nand_die();
        return;
    }
    #endif

    if(pNandDrv->au16_L2P_0[u16_MinECnt_LBA] != 0)
    {
        nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "WL Copy %X to %X \n", u16_MinECnt_PBA, u16_MaxECnt_FreeBlk);

        nand_delete_FreeBlk_Zone0(u8_FBPos);

        u32_Ret = nand_copy_data(u16_MaxECnt_FreeBlk, 0, u16_MinECnt_PBA, pNandDrv->u16_BlkSectorCnt);
        if(UNFD_ST_SUCCESS != u32_Ret)
        {
            if(u32_Ret != UNFD_ST_ERR_W_FAIL)
                nand_erase_and_set_FreeBlk(u16_MaxECnt_FreeBlk);

            gu16_Erase_Counter[u16_MinECnt_PBA - pNandDrv->u16_FirstPBA_Zone0]++;

            return;
        }

        pNandDrv->au16_L2P_0[u16_MinECnt_LBA] = u16_MaxECnt_FreeBlk;

        nand_erase_and_set_FreeBlk(u16_MinECnt_PBA);
    }
    else
    {
        nand_debug(UNFD_DEBUG_LEVEL,1,"ERROR, No Mapping\n");
        nand_die();
    }

    #if UNFD_PRINT_EC
    if((++WL_loop) % 64 == 0)
    {
        nand_debug(UNFD_DEBUG_LEVEL,1,"Print Zone0 Erase Counter\n");
        dump_mem((U8*)&gu16_Erase_Counter[0], (pNandDrv->u16_FirstPBA_Zone1-pNandDrv->u16_FirstPBA_Zone0)*2);
    }
    #endif
}


void nand_Wear_Leveling1(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    //U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
    //DATA_BLK_SPARE_t *pDataBlkSpare_t = (P_DATA_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    U8  u8_FBPos;
    U16 u16_MaxECnt, u16_MaxECnt_FreeBlk, u16_MinECnt, u16_MinECnt_PBA, u16_MinECnt_LBA;
    U16 u16_i;
    U32 u32_Ret;
    U16 u16_Zone1LBACnt;
    U8  u8_has_build;
    #if UNFD_PRINT_EC
    static U32 WL1_loop=0;
    #endif

    printf("nand_Wear_Leveling1\n");
    nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "+\n");

    if(pNandDrv->u8_FBCnt_1 == 0)
    {
        nand_debug(UNFD_DEBUG_LEVEL, 1, "No Free block\n");
        return;
    }

    #if UNFD_FTL_WBQ
    for(u16_i=0; u16_i<MAX_WBQ_CNT; u16_i++)
    {
        u32_Ret = nand_FlushWBQ(u16_i);
    }
    #endif

    // query free block with max erase counter
    u32_Ret = nand_FindMaxECntFreeBlkInZone1(&u16_MaxECnt_FreeBlk, &u16_MaxECnt, &u8_FBPos);
    if(u32_Ret != UNFD_ST_SUCCESS)
        return;
    //nand_debug(0, 1, "FBPos: %X, MaxECnt_FreeBlk: %X, MaxECnt: %X\n",
    //	u8_FBPos, u16_MaxECnt_FreeBlk, u16_MaxECnt);

    if (u16_MaxECnt < WL_EC_THRESHOLD)
        return;

    //get min erase counter from LUT
    u16_Zone1LBACnt = pNandDrv->u16_LastPBA_LogiArea - pNandDrv->u16_FirstPBA_Zone1 -
        pNandDrv->u8_FBMaxCnt_1;
    u16_MinECnt = 0xFFFF;
    u16_MinECnt_LBA = 0;
    u16_MinECnt_PBA = pNandDrv->au16_L2P_1[0];
    u8_has_build = 0;
    for (u16_i = 0; u16_i < u16_Zone1LBACnt; u16_i++)
    {
        if(pNandDrv->au16_L2P_1[u16_i] == 0 )
        {
            if (u8_has_build == 0) // we could build another LUT one time if this subzone is not built.
            {
                nand_build_LUT1(); // free blk will append to au16_FBIdx_1, and won't impact u8_FBPos(ordinal)
                u8_has_build = 1;
            }
        }

        if (pNandDrv->au16_L2P_1[u16_i] != 0)
        {
            //comapre
            if (u16_MinECnt > gu16_Erase_Counter[pNandDrv->au16_L2P_1[u16_i] - pNandDrv->u16_FirstPBA_Zone0])
            {
                u16_MinECnt = gu16_Erase_Counter[pNandDrv->au16_L2P_1[u16_i] - pNandDrv->u16_FirstPBA_Zone0];
                u16_MinECnt_PBA = pNandDrv->au16_L2P_1[u16_i];
                u16_MinECnt_LBA = u16_i;
            }
        }
    }
    //nand_debug(0, 1, "MinECnt_LBA: %X, MinECnt_PBA: %X, MinECnt: %X\n",
    //	u16_MinECnt_LBA, u16_MinECnt_PBA, u16_MinECnt);

    //check is exceed threshold
    if ((u16_MaxECnt < u16_MinECnt) || ((u16_MaxECnt - u16_MinECnt) < WL_EC_THRESHOLD))
    {
        //nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "Not exceed threshold\n");
        return;
    }

#if 0
    ////check data blk
    NC_ReadSectors_Retry((u16_MinECnt_PBA << pNandDrv->u8_BlkPageCntBits), 0, au8_PageBuf, (U8*)pDataBlkSpare_t, 1);
    if (pDataBlkSpare_t->u8_BlkID != UNFD_FTL_BLK_ID_DATA)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"not data block: %X\n", pDataBlkSpare_t->u8_BlkID);

        nand_die();
        return;
    }

    ////check free blk
    NC_ReadSectors_Retry((u16_MaxECnt_FreeBlk << pNandDrv->u8_BlkPageCntBits), 0, au8_PageBuf, (U8*)pDataBlkSpare_t, 1);
    if (pDataBlkSpare_t->u8_BlkID != 0xFF)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"not free block: %X\n", pDataBlkSpare_t->u8_BlkID);

        nand_die();
        return;
    }
#endif

    if (pNandDrv->au16_L2P_1[u16_MinECnt_LBA] != 0)
    {
        nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "Copy %X to %X \n", u16_MinECnt_PBA, u16_MaxECnt_FreeBlk);

        nand_delete_FreeBlk_Zone1(u8_FBPos);

        u32_Ret = nand_copy_data(u16_MaxECnt_FreeBlk, 0, u16_MinECnt_PBA, pNandDrv->u16_BlkSectorCnt);
        if(UNFD_ST_SUCCESS != u32_Ret)
        {
            if(u32_Ret != UNFD_ST_ERR_W_FAIL)
                nand_erase_and_set_FreeBlk(u16_MaxECnt_FreeBlk);

            gu16_Erase_Counter[u16_MinECnt_PBA - pNandDrv->u16_FirstPBA_Zone0]++;

            return;
        }


        pNandDrv->au16_L2P_1[u16_MinECnt_LBA] = u16_MaxECnt_FreeBlk;

        nand_erase_and_set_FreeBlk(u16_MinECnt_PBA);
    }
    else
    {
        nand_debug(UNFD_DEBUG_LEVEL,1,"No Mapping\n");
        nand_die();
    }

    #if UNFD_PRINT_EC
    if( (++WL1_loop) % 64 == 0)
    {
        nand_debug(UNFD_DEBUG_LEVEL,1,"Print Zone1 Erase Counter\n");
        dump_mem((U8*)&gu16_Erase_Counter[(pNandDrv->u16_FirstPBA_Zone1-pNandDrv->u16_FirstPBA_Zone0)],
            (pNandDrv->u16_LastPBA_LogiArea-pNandDrv->u16_FirstPBA_Zone1)*2);
    }
    #endif
}
#endif

static U32 nand_format_DataBlk(U16 u16_PBA, U16 u16_LBA)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    DATA_BLK_SPARE_t *pDataBlkSpare_t = (P_DATA_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    U8 *pu8_DataBuf = pNandDrv->pu8_PageDataBuf;
    U32 u32_Ret, u32_Row;

    pDataBlkSpare_t->u8_BadBlkMark = 0xFF;
    pDataBlkSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_DATA;
    pDataBlkSpare_t->u16_LBA = u16_LBA;
    pDataBlkSpare_t->u8_RDD4 = 0xFF;
    pDataBlkSpare_t->u8_RDD5 = 0xFF;

    u32_Row = u16_PBA << pNandDrv->u8_BlkPageCntBits;
    //u32_Ret = nand_WriteSectors_Raw(u32_Row, 0, pu8_DataBuf, (U8*)pDataBlkSpare_t, 1);
    u32_Ret = nand_WritePages_Raw(u32_Row, pu8_DataBuf, (U8*)pDataBlkSpare_t, 1);
    if(UNFD_ST_SUCCESS != u32_Ret)
    {
        if(u32_Ret == UNFD_ST_ERR_W_FAIL)
            drvNAND_MarkBadBlk(u16_PBA);
        return u32_Ret;
    }

    u32_Row += pNandDrv->u16_BlkPageCnt-1;
    //u32_Ret = nand_WriteSectors_Raw(u32_Row, 0, pu8_DataBuf, (U8*)pDataBlkSpare_t, 1);
    u32_Ret = nand_WritePages_Raw(u32_Row, pu8_DataBuf, (U8*)pDataBlkSpare_t, 1);
    if(UNFD_ST_SUCCESS != u32_Ret)
    {
        if(u32_Ret == UNFD_ST_ERR_W_FAIL)
            drvNAND_MarkBadBlk(u16_PBA);
        return u32_Ret;
    }
    return UNFD_ST_SUCCESS;
}


U32 nand_Low_Level_Format(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    ROOT_BLK_DATA_t *pRootBlkData_t = (P_ROOT_BLK_DATA_t)pNandDrv->pu8_PageDataBuf;
    ROOT_BLK_SPARE_t *pRootBlkSpare_t = (P_ROOT_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    U32 u32_Ret;
    volatile U16 u16_PBA, u16_LBA, u16_Zone1LBACnt, u16_BadBlkCnt;
    volatile U32 u32_Row;
    #if UNFD_FTL_WL
    volatile U8  u8_ECntIdx;
    #endif

    nand_debug(UNFD_DEBUG_LEVEL, 1, "+: %Xh\n", pNandDrv->u16_FirstPBA_LogiArea);

    //-----------------------------------------
    // erase all good blocks in logical area
    //-----------------------------------------
    for(u16_PBA=pNandDrv->u16_FirstPBA_LogiArea; u16_PBA<pNandDrv->u16_LastPBA_LogiArea; u16_PBA++)
    {
        if(0 == drvNAND_IsGoodBlk(u16_PBA))
            continue;

        u32_Ret = nand_EraseBlk(u16_PBA << pNandDrv->u8_BlkPageCntBits);
        if(UNFD_ST_SUCCESS != u32_Ret)
        {
            if(u32_Ret == UNFD_ST_ERR_E_FAIL)
                drvNAND_MarkBadBlk(u16_PBA);
        }
    }
    //-----------------------------------------
    // setup RootBlk
    //   [CAUTION]: put at the end of low level format
    //-----------------------------------------
    pNandDrv->u16_RootBlkPhyIdx = pNandDrv->u16_FirstPBA_LogiArea;
    u16_BadBlkCnt = 0;

    LABEL_SETUP_ROOT_BLK:
    if(0 == drvNAND_IsGoodBlk(pNandDrv->u16_RootBlkPhyIdx))
    {
        pNandDrv->u16_RootBlkPhyIdx++;
        u16_BadBlkCnt++;
        goto LABEL_SETUP_ROOT_BLK;
    }

    #if 0
    pRootBlkSpare_t->u8_BadBlkMark = 0;
    pRootBlkSpare_t->u8_BlkID = 0;
    pRootBlkSpare_t->u8_FTLVerNum = 0;
    pRootBlkSpare_t->u8_RDD3 = 0;
    pRootBlkSpare_t->u8_RDD4 = 0;
    pRootBlkSpare_t->u8_RDD5 = 0;
    NC_ReadSectors_Retry(u32_Row, 0, (U8*)pRootBlkData_t, (U8*)pRootBlkSpare_t, 1);
    dump_mem(pRootBlkSpare_t, 0x10);
    #endif
    nand_debug(UNFD_DEBUG_LEVEL, 1, "bad blk cnt for RootBlk %Xh: %X \n", pNandDrv->u16_RootBlkPhyIdx, u16_BadBlkCnt);

    //-----------------------------------------
    // re-calculate parameters
    //-----------------------------------------
    u32_Ret = nand_init_parameters();
    if(UNFD_ST_SUCCESS != u32_Ret)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Fatal ERROR !\n");
        nand_die();
    }

    //-----------------------------------------
    // setup DataBlk, for Zone0
    //-----------------------------------------
    u16_LBA = 0;
    u16_BadBlkCnt = 0;

    for(u16_PBA = pNandDrv->u16_FirstPBA_Zone0;
        u16_PBA < pNandDrv->u16_FirstPBA_Zone1;
        u16_PBA++)
    {
        if(pNandDrv->u16_Zone0LBACnt == u16_LBA)
            break;

        if(0 == drvNAND_IsGoodBlk(u16_PBA))
        {
            u16_BadBlkCnt++;
            continue;
        }

        #if UNFD_FTL_FMAP
        if(u16_LBA >= pNandDrv->u16_FM_FirstLBA &&
            u16_LBA < pNandDrv->u16_FM_FirstLBA + pNandDrv->u16_FM_LBACnt)
        {
            u32_Ret = nand_format_FMapBlk(u16_PBA, u16_LBA);
            if(u32_Ret != UNFD_ST_SUCCESS)
            {
                u16_BadBlkCnt++;
                continue;
            }
            u16_LBA++;
            continue;
        }
        #endif

        u32_Ret = nand_format_DataBlk(u16_PBA, u16_LBA);
        if(u32_Ret != UNFD_ST_SUCCESS)
        {
            u16_BadBlkCnt++;
            continue;
        }

        u16_LBA++;
    }

    //-----------------------------------------
    // setup ECBlk
    //-----------------------------------------
    #if UNFD_FTL_WL
    u8_ECntIdx = 0;
    for( ; u16_PBA < pNandDrv->u16_FirstPBA_Zone1 ; u16_PBA++)
    {
        if((pNandDrv->u8_Zone1SubZoneCnt+1) == u8_ECntIdx)
            break;

        if(0 == drvNAND_IsGoodBlk(u16_PBA))
        {
            u16_BadBlkCnt++;
            continue;
        }

        u32_Ret = nand_format_ECBlk(u16_PBA, u8_ECntIdx);
        if(u32_Ret != UNFD_ST_SUCCESS)
        {
            u16_BadBlkCnt++;
            continue;
        }

        nand_debug(UNFD_DEBUG_LEVEL, 1, "EC[%X]: %X \n", u8_ECntIdx, u16_PBA);
        u8_ECntIdx++;
    }
    #endif

    // scan BadBlk
    while(u16_PBA < pNandDrv->u16_FirstPBA_Zone1)
    {
        if(0 == drvNAND_IsGoodBlk(u16_PBA))
            u16_BadBlkCnt++;

        u16_PBA++;
    }

    nand_debug(UNFD_DEBUG_LEVEL, 1, "Zone_0, PBA from %X to %X, LBA Cnt: %X \n",
        pNandDrv->u16_FirstPBA_Zone0, pNandDrv->u16_FirstPBA_Zone1, pNandDrv->u16_Zone0LBACnt);

    //-----------------------------------------
    // setup DataBlk, for Zone1
    //-----------------------------------------
    // scan for 8 free blks at first
    u16_BadBlkCnt = 0;
    u16_PBA = pNandDrv->u16_FirstPBA_Zone1;
    while((u16_PBA - pNandDrv->u16_FirstPBA_Zone1- u16_BadBlkCnt < 8) &&
        u16_PBA < pNandDrv->u16_LastPBA_LogiArea)
    {
        if(0 == drvNAND_IsGoodBlk(u16_PBA))
            u16_BadBlkCnt++;

        u16_PBA++;
    }

    if(pNandDrv->u8_Zone1SubZoneCnt)
        u16_Zone1LBACnt = pNandDrv->u16_LogiDataBlkCnt - pNandDrv->u16_Zone0LBACnt;
    else 
        u16_Zone1LBACnt = 0;

    u16_LBA = 0;

    for(;
        u16_PBA < pNandDrv->u16_LastPBA_LogiArea;
        u16_PBA++)
    {
        if(u16_Zone1LBACnt == u16_LBA)
        break;

        if(0 == drvNAND_IsGoodBlk(u16_PBA))
        {
            u16_BadBlkCnt++;
            continue;
        }

        u32_Ret = nand_format_DataBlk(u16_PBA, u16_LBA);
        if(u32_Ret != UNFD_ST_SUCCESS)
        {
            u16_BadBlkCnt++;
            continue;
        }

        u16_LBA++;
    }
    nand_debug(UNFD_DEBUG_LEVEL, 1, "Zone_1, PBA from %X to %X, LBA Cnt: %X \n",
        pNandDrv->u16_FirstPBA_Zone1,
        pNandDrv->u16_LastPBA_LogiArea, u16_LBA);

    // ------------------------------------
    // write RootBlk
    pRootBlkSpare_t->u8_BadBlkMark = 0xFF;
    pRootBlkSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_ROOT;
    pRootBlkSpare_t->u8_FTLVerNum = UNFD_FTL_VERSION;
    pRootBlkSpare_t->u8_RDD3 = 0xFF;
    pRootBlkSpare_t->u8_RDD4 = 0xFF;
    pRootBlkSpare_t->u8_RDD5 = 0xFF;

    u32_Row = pNandDrv->u16_RootBlkPhyIdx << pNandDrv->u8_BlkPageCntBits;
    //u32_Ret = nand_WriteSectors_Raw(u32_Row, 0, (U8*)pRootBlkData_t, (U8*)pRootBlkSpare_t, 1);
    u32_Ret = nand_WritePages_Raw(u32_Row, (U8*)pRootBlkData_t, (U8*)pRootBlkSpare_t, 1);
    if(UNFD_ST_SUCCESS != u32_Ret)
    {
        if(u32_Ret == UNFD_ST_ERR_W_FAIL)
            drvNAND_MarkBadBlk(pNandDrv->u16_RootBlkPhyIdx);
        pNandDrv->u16_RootBlkPhyIdx++;
        u16_BadBlkCnt++;
        nand_debug(UNFD_DEBUG_LEVEL, 1, "ERROR, write RootBlk fail, please re-download \n");
        return u32_Ret;
    }

    nand_debug(UNFD_DEBUG_LEVEL, 1, "-\n");
    return UNFD_ST_SUCCESS;
}


void nand_LUT_Test(void)
{
    nand_debug(0,1,"\n\n");
    //nand_build_LUT1();
    //nand_debug(0,1,"\n\n");
    //nand_build_LUT0();
    //nand_debug(0,1,"\n\n");
    //nand_build_LUT1();

    nand_dump_WBQ();
    nand_dump_FreeBlk(0);
}

static void nand_build_LUT1(void) /* u8_Zone1LUTFlag is zero-based */
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
    DATA_BLK_SPARE_t *pDataBlkSpare_t = (P_DATA_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;

    U16 u16_PBA, u16_PBAEnd, *au16_L2P;
    U32 u32_Row, u32_Ret;
    U16 u16_LBA;

    U16  u16_Cnt;
    U16 *pu16_InitBBArray;

    pu16_InitBBArray =(U16*) malloc(506);


    // [CAUTION]: for Zone_1, can not reset FB_Queue
    //pNandDrv->u8_FBCnt_1 = 0;
    //pNandDrv->u8_FBHead_1 = 0;
    //pNandDrv->u8_FBTail_1 = 0;

    u16_PBA = pNandDrv->u16_FirstPBA_Zone1 + LUT_PBA_CNT*pNandDrv->u8_Zone1LUTFlag;
    if(u16_PBA > pNandDrv->u16_LastPBA_LogiArea)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, PBA:%X, BlkCnt:%X, Zone:%X\n",
            u16_PBA, pNandDrv->u16_LastPBA_LogiArea, pNandDrv->u8_Zone1LUTFlag);
        nand_die();
    }

    u16_PBAEnd = (pNandDrv->u16_LastPBA_LogiArea - u16_PBA) > LUT_PBA_CNT ?
        LUT_PBA_CNT : pNandDrv->u16_LastPBA_LogiArea - u16_PBA;
    u16_PBAEnd += u16_PBA;
    nand_debug(UNFD_DEBUG_LEVEL, 1, "Zone: %X, PBA from %X to %X\n",
        pNandDrv->u8_Zone1LUTFlag, u16_PBA, u16_PBAEnd);

    pNandDrv->u8_Zone1LUTFlag++;
    if(pNandDrv->u8_Zone1LUTFlag > pNandDrv->u8_Zone1SubZoneCnt)
    {
        // should not be here
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, invalid ZoneNo:%X to build LUT1 \n", pNandDrv->u8_Zone1LUTFlag);
        nand_die();
    }

    au16_L2P = pNandDrv->au16_L2P_1;

    // ----------------------------------------
    // get InitBBT
    //u32_Ret = nand_get_InitBBT(pu16_InitBBArray,&u16_Cnt);
    //if(u32_Ret != UNFD_ST_SUCCESS)
        //nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"Not found Initial BBT\n");

    for(; u16_PBA < u16_PBAEnd; u16_PBA++)
    {
        //if(nand_check_IfInitBB(u16_PBA, pu16_InitBBArray, u16_Cnt))
        //{
            //nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Initial BadBlk: %X:\n",u16_PBA);
        //}
        //else
        {
            // check LAB in last page
            u32_Row = (u16_PBA << pNandDrv->u8_BlkPageCntBits) +
                pNandDrv->u16_BlkPageCnt - 1;
            u32_Ret = nand_ReadSectors_Raw(u32_Row, 0, au8_PageBuf, (U8*)pDataBlkSpare_t, 1);

            if(UNFD_ST_SUCCESS == u32_Ret && 0xFF == pDataBlkSpare_t->u8_BadBlkMark)
            {
                if(UNFD_FTL_BLK_ID_DATA == pDataBlkSpare_t->u8_BlkID)
                {
                    if(0 == au16_L2P[pDataBlkSpare_t->u16_LBA])
                        au16_L2P[pDataBlkSpare_t->u16_LBA] = u16_PBA;
                    else
                    {
                        if(au16_L2P[pDataBlkSpare_t->u16_LBA] != u16_PBA)
                        {
                            u16_LBA = pDataBlkSpare_t->u16_LBA;
                            au16_L2P[u16_LBA] =
                                nand_DuplicatePBAHandler(au16_L2P[u16_LBA],
                                u16_PBA, u16_LBA);
                        }
                    }
                }
                else if(0xFF == pDataBlkSpare_t->u8_BlkID) // last page is free
                {
                    // check the first page
                    u32_Row = (u16_PBA << pNandDrv->u8_BlkPageCntBits);
                    u32_Ret = nand_ReadSectors_Raw(u32_Row, 0, au8_PageBuf, (U8*)pDataBlkSpare_t, 1);
                    if(UNFD_ST_SUCCESS == u32_Ret)
                    {
                        if(0xFF == pDataBlkSpare_t->u8_BlkID)
                        {
                            nand_erase_and_set_FreeBlk(u16_PBA);
                        }
                        else
                        {
                            nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "NAND, unknow BlkID, PBA p.0: %X, BlkID: %X\n",
                                u16_PBA, pDataBlkSpare_t->u8_BlkID);
                            // do things here
                            // ...
                            // else
                            nand_erase_and_set_FreeBlk(u16_PBA);
                        }
                    }
                    else if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
                    {
                        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1,
                            "ERROR, NAND, ECC failed at PBA:%X's p.0 page \n", u16_PBA);
                        nand_erase_and_set_FreeBlk(u16_PBA);
                    }
                    else
                    {   // other error, should never be here
                        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Error Code, p.0: %lX \n", u32_Ret);
                        nand_die();
                    }
                }
                else  // LBA in last page is unknown
                {
                    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "NAND, unknow BlkID, PBA p.last: %X, BlkID: %X\n",
                        u16_PBA, pDataBlkSpare_t->u8_BlkID);
                    // do things here
                    // ...
                    // else
                    nand_erase_and_set_FreeBlk(u16_PBA);
                }
            }
            else if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1,
                    "ERROR, NAND, ECC failed at PBA:%X's p.last page \n", u16_PBA);
                nand_erase_and_set_FreeBlk(u16_PBA);
            }
            else
            {
                if(0xFF != pDataBlkSpare_t->u8_BadBlkMark)
                {
                    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "BadBlk: %X, BB_Mark: %X \n",
                        u16_PBA, pDataBlkSpare_t->u8_BadBlkMark);
                }
                else
                {
                    // other error, should never be here
                    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Error Code, p.last: %lX \n", u32_Ret);
                    nand_die();
                }
            }
        }
    }

    free(pu16_InitBBArray);

    #if UNFD_FTL_RW_DEBUG
    dump_mem((U8*)au16_L2P, 0x200);
    ftl_rw_debug(nand_debug(0,1,"\n"));
    dump_mem((U8*)(pNandDrv->au16_FBIdx_1), pNandDrv->u8_FBMaxCnt_1);
    #endif

    // if no free blk, continue to build LUT_1 to find free blocks
    while(0 == pNandDrv->u8_FBCnt_1)
    {
        if(pNandDrv->u8_Zone1LUTFlag == pNandDrv->u8_Zone1SubZoneCnt)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"Error, Zone1 has no FreeBlk! \n");
            nand_die();
        }

        nand_build_LUT1();
    }
}


static U32 nand_build_LUT0(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
    DATA_BLK_SPARE_t *pDataBlkSpare_t = (P_DATA_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    #if UNFD_FTL_FMAP
    FMAP_BLK_SPARE_t *pFMapBlkSpare_t = (P_FMAP_BLK_SPARE_t)pDataBlkSpare_t;
    #endif
    #if UNFD_FTL_WL
    WLECNT_BLK_SPARE_t *pWLECNTBlkSpare_t = (P_WLECNT_BLK_SPARE_t)pDataBlkSpare_t;
    U8 u8_flag_need_Erase;
    #endif
    U16 u16_PBA, u16_PBAEnd, *au16_L2P;
    U32 u32_Row, u32_Ret;
    U16 u16_Temp;
    #if 1//UNFD_FTL_FMAP
    U8  u8_i, u8_k;
    U16 u16_FreePage;
    #endif
    U16 u16_LBA;
    U8 u8_TempIdx;

    U16  u16_Cnt;
    U16 *pu16_InitBBArray;

    pu16_InitBBArray =(U16*) malloc(506);


    // to avid repeating PBA set into au16_FBIdx_0 when re-call nand_build_LUT0
    pNandDrv->u8_FBCnt_0 = 0;
    pNandDrv->u8_FBHead_0 = 0;
    pNandDrv->u8_FBTail_0 = 0;

    u16_PBA = pNandDrv->u16_FirstPBA_Zone0;
    u16_PBAEnd = pNandDrv->u16_FirstPBA_Zone1;
    nand_debug(UNFD_DEBUG_LEVEL, 1, "zone 0, PBA from %X to %X\n", u16_PBA, u16_PBAEnd);

    au16_L2P = pNandDrv->au16_L2P_0;

    // ----------------------------------------
    // get InitBBT
    //u32_Ret = nand_get_InitBBT(pu16_InitBBArray,&u16_Cnt);
    //if(u32_Ret != UNFD_ST_SUCCESS)
        //nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"Not found Initial BBT\n");

    for(; u16_PBA < u16_PBAEnd; u16_PBA++)
    {
        //if(nand_check_IfInitBB(u16_PBA, pu16_InitBBArray, u16_Cnt))
        //{
            //nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Initial BadBlk: %X:\n",u16_PBA);
        //}
        //else
        {
            u32_Row = u16_PBA << pNandDrv->u8_BlkPageCntBits;
            u32_Ret = nand_ReadSectors_Raw(u32_Row, 0, au8_PageBuf, (U8*)pDataBlkSpare_t, 1);

            if(UNFD_ST_SUCCESS == u32_Ret && 0xFF == pDataBlkSpare_t->u8_BadBlkMark)
            {
                if(UNFD_FTL_BLK_ID_DATA == pDataBlkSpare_t->u8_BlkID)
                {
                    if(0 == au16_L2P[pDataBlkSpare_t->u16_LBA])
                        au16_L2P[pDataBlkSpare_t->u16_LBA] = u16_PBA;
                    else
                    {
                        if(au16_L2P[pDataBlkSpare_t->u16_LBA] != u16_PBA)
                        {
                            u16_LBA = pDataBlkSpare_t->u16_LBA;
                            au16_L2P[u16_LBA] =
                                nand_DuplicatePBAHandler(au16_L2P[u16_LBA],
                                u16_PBA, u16_LBA);
                        }
                        else
                        {
                            nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, called beside nand_Init_FTL ! \n");
                            nand_die();
                        }
                    }
                }
                #if UNFD_FTL_FMAP
                // setup au16_FM_BL2P
                else if(UNFD_FTL_BLK_ID_FMAP == pDataBlkSpare_t->u8_BlkID)
                {
                    if(!(pFMapBlkSpare_t->u16_LBA >= pNandDrv->u16_FM_FirstLBA &&
                        pFMapBlkSpare_t->u16_LBA < pNandDrv->u16_FM_FirstLBA + pNandDrv->u16_FM_LBACnt))
                    {
                        // error handle -> FAT partition shifted (by PARTITION_v2.pni changed)
                        nand_debug(UNFD_DEBUG_LEVEL,1,"FAT partition shifted, case.0 -> Low Level Format! %X\n",
                            pFMapBlkSpare_t->u16_LBA);
                        return UNFD_ST_ERR_FAT_SHIFT;
                    }

                    if(0 == pNandDrv->au16_FM_BL2P[pFMapBlkSpare_t->u16_LBA - pNandDrv->u16_FM_FirstLBA]
                        [pFMapBlkSpare_t->u8_PageIdx / pNandDrv->u8_FM_PBAPageCnt])
                    pNandDrv->au16_FM_BL2P[pFMapBlkSpare_t->u16_LBA - pNandDrv->u16_FM_FirstLBA]
                        [pFMapBlkSpare_t->u8_PageIdx / pNandDrv->u8_FM_PBAPageCnt]
                        = u16_PBA;
                    else
                    {
                        u16_LBA = pFMapBlkSpare_t->u16_LBA;
                        u8_TempIdx = pFMapBlkSpare_t->u8_PageIdx;
                        pNandDrv->au16_FM_BL2P[u16_LBA - pNandDrv->u16_FM_FirstLBA]
                            [u8_TempIdx / pNandDrv->u8_FM_PBAPageCnt]
                        = nand_DuplicatePBAHandler(
                        pNandDrv->au16_FM_BL2P[u16_LBA - pNandDrv->u16_FM_FirstLBA][u8_TempIdx / pNandDrv->u8_FM_PBAPageCnt],
                            u16_PBA, u16_LBA);
                    }
                }
                #endif
                #if UNFD_FTL_WL
                // setup au16_WL_ECntPBA
                else if(UNFD_FTL_BLK_ID_WLECNT == pWLECNTBlkSpare_t->u8_BlkID)
                {
                    if (pWLECNTBlkSpare_t->u8_ECntIdx > pNandDrv->u8_Zone1SubZoneCnt)
                    {
                        nand_debug(UNFD_DEBUG_LEVEL,1,"EC shifted, case.0 -> need Low Level Format! %X\n", pWLECNTBlkSpare_t->u8_ECntIdx);
                        return UNFD_ST_ERR_EC_SHIFT;
                    }


                    if(0 == pNandDrv->au16_WL_ECntPBA[pWLECNTBlkSpare_t->u8_ECntIdx])
                        pNandDrv->au16_WL_ECntPBA[pWLECNTBlkSpare_t->u8_ECntIdx] = u16_PBA;
                    else
                    {
                        if(pNandDrv->au16_WL_ECntPBA[pWLECNTBlkSpare_t->u8_ECntIdx] != u16_PBA)
                        {
                            u8_TempIdx = pWLECNTBlkSpare_t->u8_ECntIdx;
                            pNandDrv->au16_WL_ECntPBA[u8_TempIdx] =
                                nand_DuplicateECHandler(pNandDrv->au16_WL_ECntPBA[u8_TempIdx],
                                u16_PBA, u8_TempIdx);
                        }
                        else
                        {
                            nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, called beside nand_Init_FTL ! \n");
                            nand_die();
                        }
                    }
                }
                #endif
                else if(0xFF == pDataBlkSpare_t->u8_BlkID)
                {
                    #if 0//UNFD_FTL_CHECK_ERROR
                    nand_check_FBExist(pNandDrv->au16_FBIdx_0, u16_PBA,
                        pNandDrv->u8_FBMaxCnt_0, pNandDrv->u8_FBCnt_0, pNandDrv->u8_FBHead_0);
                    #endif
                    nand_erase_and_set_FreeBlk(u16_PBA);
                }
                #if UNFD_FTL_WBQ
                else if(UNFD_FTL_BLK_ID_PPB == pDataBlkSpare_t->u8_BlkID)
                {
                    nand_debug(UNFD_DEBUG_LEVEL, 1, "NAND, clean WBQ PPB: %X, BlkID: %X\n",
                        u16_PBA, pDataBlkSpare_t->u8_BlkID);

                    nand_erase_and_set_FreeBlk(u16_PBA);
                }
                #endif
                #if UNFD_FTL_RD
                else if(UNFD_FTL_BLK_ID_RD == pDataBlkSpare_t->u8_BlkID)
                {
                    pNandDrv->u16_RDB = u16_PBA;
                    pNandDrv->u16_RDB_Page = nand_FindFreePage(pNandDrv->u16_RDB);
                    nand_debug(0,1,"RD, get RDB: %X  RDB_Page: %X \n",
                        pNandDrv->u16_RDB, pNandDrv->u16_RDB_Page);
                    if(0xFFFF == pNandDrv->u16_RDB_Page)
                    {
                        nand_erase_and_set_FreeBlk(pNandDrv->u16_RDB);
                        pNandDrv->u16_RDB = 0;
                        pNandDrv->u16_RDB_Page = 0;
                    }
                }
                #endif
                else if(UNFD_FTL_BLK_ID_END < pDataBlkSpare_t->u8_BlkID)
                {
                    nand_debug(UNFD_DEBUG_LEVEL, 1, "NAND, unknow PBA: %X, BlkID: %X\n",
                        u16_PBA, pDataBlkSpare_t->u8_BlkID);
                    nand_die();
                }
                else
                {
                    nand_debug(UNFD_DEBUG_LEVEL, 1, "NAND, meet Phy area blk, PBA: %X, BlkID: %X\n",
                        u16_PBA, pDataBlkSpare_t->u8_BlkID);
                }
            }
            else if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1,
                    "ERROR, NAND, ECC failed at PBA:%X's p.0 \n", u16_PBA);
                nand_erase_and_set_FreeBlk(u16_PBA);
            }
            else
            {
                if(0xFF != pDataBlkSpare_t->u8_BadBlkMark)
                {
                    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "BadBlk: %X, BB_Mark: %X \n",
                        u16_PBA, pDataBlkSpare_t->u8_BadBlkMark);
                }
                else
                {   // should never be here
                    nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Error Code, p.0: %lX \n", u32_Ret);
                    nand_die();
                }
            }
        }
    }

    free(pu16_InitBBArray);

    #if UNFD_FTL_RD
    if(pNandDrv->u16_RDB_Page & 1)
        nand_RD_ErrorHandler();
    #endif

    #if UNFD_FTL_WBQ
    nand_reset_WBQs();
    #endif

    #if 0//UNFD_FTL_FMAP
    // error handle -> FAT partition shifted (by PARTITION_v2.pni changed)
    for(u8_i=0; u8_i<pNandDrv->u16_FM_LBACnt; u8_i++)
        if(0 == pNandDrv->au16_FM_BL2P[u8_i][0])
        {
            nand_debug(UNFD_DEBUG_LEVEL,1,"FAT partition shifted, case.1 -> Low Level Format!\n");
            return UNFD_ST_ERR_FAT_SHIFT;
        }

    // Move FM init to the first time using FAT
    #endif

    #if UNFD_FTL_WL
    for(u8_i=0; u8_i<(pNandDrv->u8_Zone1SubZoneCnt+1); u8_i++)
    {
        u32_Row = pNandDrv->au16_WL_ECntPBA[u8_i] << pNandDrv->u8_BlkPageCntBits;

        if(0 == pNandDrv->au16_WL_ECntPBA[u8_i])
        {
            nand_debug(UNFD_DEBUG_LEVEL,1,"EC shifted, case.1 -> need Low Level Format!\n");
            return UNFD_ST_ERR_FAT_SHIFT;
        }
        else
        {
            u8_flag_need_Erase = 0;
            // find the free page
            u16_FreePage = nand_FindFreePage(pNandDrv->au16_WL_ECntPBA[u8_i]);
            if(0xFFFF == u16_FreePage)
                pNandDrv->au16_WL_ECntPBA_Page[u8_i] = (U8)pNandDrv->u16_BlkPageCnt-1;
            else
                pNandDrv->au16_WL_ECntPBA_Page[u8_i] = (U8)u16_FreePage-1;

            while(pNandDrv->au16_WL_ECntPBA_Page[u8_i] < 0xFFFF)
            {
                u32_Ret = nand_ReadPages_Raw(u32_Row + pNandDrv->au16_WL_ECntPBA_Page[u8_i],
                    (U8*)au8_PageBuf, (U8*)pWLECNTBlkSpare_t, 1);
                if(UNFD_ST_SUCCESS != u32_Ret)
                    u8_flag_need_Erase = 1;
                else
                    break;

                pNandDrv->au16_WL_ECntPBA_Page[u8_i]--;
            }
        }

        if(pNandDrv->u16_PageByteCnt == 512) // small blk
        {
            if((pNandDrv->au16_WL_ECntPBA_Page[u8_i] & 0x03) != 3)	//that should not write complete
            {
                u8_flag_need_Erase = 1;
                pNandDrv->au16_WL_ECntPBA_Page[u8_i] =
                    (pNandDrv->au16_WL_ECntPBA_Page[u8_i] & ~0x03) - 1;
            }

            if(0xFFFF != pNandDrv->au16_WL_ECntPBA_Page[u8_i])
                for(u8_k = 0; u8_k < 4; u8_k++)
                    nand_ReadPages_Raw(u32_Row + (pNandDrv->au16_WL_ECntPBA_Page[u8_i] & ~0x03) + u8_k,
                        (U8*)&gu16_Erase_Counter[u8_i*1024+(u8_k<<8)], (U8*)pWLECNTBlkSpare_t, 1);
            else
                memset((void*)&gu16_Erase_Counter[u8_i*1024], 0, 1024*2);
        }
        else
        {
            memcpy((void*)&gu16_Erase_Counter[u8_i*1024], (void*)au8_PageBuf, 2048);
        }

        if(u8_flag_need_Erase == 1)
        {
            if(pNandDrv->u16_PageByteCnt > 512)
            {
                u16_Temp = pNandDrv->au16_WL_ECntPBA[u8_i];

                LABEL_REDO_EC_ERASE0:
                pNandDrv->au16_WL_ECntPBA[u8_i] = nand_get_FreeBlk_Zone0();
                u32_Row = pNandDrv->au16_WL_ECntPBA[u8_i] << pNandDrv->u8_BlkPageCntBits;

                pWLECNTBlkSpare_t->u8_BadBlkMark = 0xFF;
                pWLECNTBlkSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_WLECNT;
                pWLECNTBlkSpare_t->u8_ECntIdx = u8_i;
                pWLECNTBlkSpare_t->u8_ECntSubIdx = 0xFF;
                pWLECNTBlkSpare_t->u8_RDD4 = 0xFF;
                pWLECNTBlkSpare_t->u8_RDD5 = 0xFF;
                u32_Ret = nand_WritePages_Raw(u32_Row, (U8*)&gu16_Erase_Counter[u8_i*1024], (U8*)pWLECNTBlkSpare_t, 1);
                if(UNFD_ST_SUCCESS != u32_Ret)
                {
                    if(u32_Ret == UNFD_ST_ERR_W_FAIL)
                        drvNAND_MarkBadBlk(pNandDrv->au16_WL_ECntPBA[u8_i]);
                    nand_debug(UNFD_DEBUG_LEVEL, 1, "Write New EC failed: %lX\n", u32_Row);

                    goto LABEL_REDO_EC_ERASE0;
                }

                // erase old ECntPBA
                u32_Ret = nand_EraseBlk((U32)u16_Temp << pNandDrv->u8_BlkPageCntBits);
                if(UNFD_ST_SUCCESS != u32_Ret)
                {
                    if(u32_Ret == UNFD_ST_ERR_E_FAIL)
                        drvNAND_MarkBadBlk(u16_Temp);
                }
                else
                {
                    nand_set_FreeBlk(u16_Temp);
                    nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "WL flag_need_Erase: %X\n", u16_Temp);
                }

                pNandDrv->au16_WL_ECntPBA_Page[u8_i] = 0;
            }
            else
            {
                u16_Temp = pNandDrv->au16_WL_ECntPBA[u8_i];
                REDO_EC_ERASE1:
                pNandDrv->au16_WL_ECntPBA[u8_i] = nand_get_FreeBlk_Zone0();
                u32_Row = pNandDrv->au16_WL_ECntPBA[u8_i] << pNandDrv->u8_BlkPageCntBits;

                for (u8_k = 0; u8_k < 4; u8_k++)
                {
                    pWLECNTBlkSpare_t->u8_BadBlkMark = 0xFF;
                    pWLECNTBlkSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_WLECNT;
                    pWLECNTBlkSpare_t->u8_ECntIdx = u8_i;
                    pWLECNTBlkSpare_t->u8_ECntSubIdx = u8_k;
                    pWLECNTBlkSpare_t->u8_RDD4 = 0xFF;
                    pWLECNTBlkSpare_t->u8_RDD5 = 0xFF;
                    u32_Ret = nand_WritePages_Raw(u32_Row + u8_k, (U8*)&gu16_Erase_Counter[u8_i*1024+(u8_k<<8)],
                        (U8*)pWLECNTBlkSpare_t, 1);
                    if(UNFD_ST_SUCCESS != u32_Ret)
                    {
                        if(u32_Ret == UNFD_ST_ERR_W_FAIL)
                            drvNAND_MarkBadBlk(pNandDrv->au16_WL_ECntPBA[u8_i]);
                        nand_debug(UNFD_DEBUG_LEVEL, 1, "Write New EC failed: %lX\n", u32_Row + u8_k);

                        goto REDO_EC_ERASE1;
                    }
                }
                // erase old ECntPBA
                u32_Ret = nand_EraseBlk((U32)u16_Temp << pNandDrv->u8_BlkPageCntBits);
                if(UNFD_ST_SUCCESS != u32_Ret)
                {
                    nand_debug(UNFD_DEBUG_LEVEL, 1, "buildLUT NAND erase tmp failed: %X\n", u16_Temp);
                    if(u32_Ret == UNFD_ST_ERR_E_FAIL)
                        drvNAND_MarkBadBlk(u16_Temp);
                }
                else
                {
                    nand_set_FreeBlk(u16_Temp);
                    nand_debug(UNFD_DEBUG_LEVEL_HIGH, 1, "WL: flag_need_Erase: %X\n", u16_Temp);
                }

                pNandDrv->au16_WL_ECntPBA_Page[u8_i] = 3;
            }
        }

        nand_debug(UNFD_DEBUG_LEVEL, 1, "%X->au16_WL_ECntPBA: %X, au16_WL_ECntPBA_Page: %X\n",
            u8_i, pNandDrv->au16_WL_ECntPBA[u8_i], pNandDrv->au16_WL_ECntPBA_Page[u8_i]);

        //dump_mem((U8*)&gu16_Erase_Counter[u8_i*1024], 2048);

    }
    #endif

    #if defined(ASSERT_FTL_LBA_NOT_MAP) && ASSERT_FTL_LBA_NOT_MAP
        #if UNFD_FTL_FMAP
        for(u16_Temp=0 ; u16_Temp<pNandDrv->u16_FM_FirstLBA ; u16_Temp++)
        {
            if(au16_L2P[u16_Temp] == 0)
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"Assert at au16_L2P[%d]=0\n", u16_Temp);
                nand_die();
            }

        }
        for(u16_Temp=(pNandDrv->u16_FM_FirstLBA+pNandDrv->u16_FM_LBACnt) ; u16_Temp<pNandDrv->u16_Zone0LBACnt ; u16_Temp++)
        {
            if(au16_L2P[u16_Temp] == 0)
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"Assert at au16_L2P[%d]=0\n", u16_Temp);
                nand_die();
            }

        }
        #else
        for(u16_Temp=0 ; u16_Temp<pNandDrv->u16_Zone0LBACnt ; u16_Temp++)
        {
            if(au16_L2P[u16_Temp] == 0)
            {
                nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"Assert at au16_L2P[%d]=0\n", u16_Temp);
                nand_die();
            }
        }
        #endif
    #endif

    #if UNFD_FTL_RW_DEBUG
    nand_debug(0,1,"\n\n");
    dump_mem((U8*)au16_L2P, 0x800);
    //ftl_rw_debug(nand_debug(0,1,"\n"));
    nand_debug(0,1,"\n\n");
    nand_dump_FreeBlk(0);
    #endif

    return UNFD_ST_SUCCESS;
}


static U16 nand_DuplicatePBAHandler(U16 u16_PBA0, U16 u16_PBA1, U16 u16_LBA)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8 *au8_PageBuf = (U8*)pNandDrv->pu8_PageDataBuf;
    DATA_BLK_SPARE_t *pDataBlkSpare_t = (P_DATA_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    U16 u16_EraseBlk=0xFFFF, u16_RetBlk=0xFFFF;
    U16 u16_LBA0, u16_LBA1;
    U16 u16_i;
    U32 u32_Ret;

    nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"NAND, duplicate PBA %X %X mapped to a LBA %X \n",
        u16_PBA0, u16_PBA1, u16_LBA);

    // read LBA from PBA0 last page
    u32_Ret = nand_ReadSectors_Raw(((u16_PBA0+1)<<pNandDrv->u8_BlkPageCntBits)-1,
        0, au8_PageBuf, (U8*)pDataBlkSpare_t, 1);
    if(UNFD_ST_SUCCESS!= u32_Ret)
    {
        u16_EraseBlk = u16_PBA0;
        u16_RetBlk = u16_PBA1;
        goto LABEL_ENDOF_DuplicatePBAHandler;
    }
    u16_LBA0 = pDataBlkSpare_t->u16_LBA;

    // read LBA from PBA1 last page
    u32_Ret = nand_ReadSectors_Raw(((u16_PBA1+1)<<pNandDrv->u8_BlkPageCntBits)-1,
        0, au8_PageBuf, (U8*)pDataBlkSpare_t, 1);
    if(UNFD_ST_SUCCESS != u32_Ret)
    {
        u16_EraseBlk = u16_PBA1;
        u16_RetBlk = u16_PBA0;
        goto LABEL_ENDOF_DuplicatePBAHandler;
    }
    u16_LBA1 = pDataBlkSpare_t->u16_LBA;

    // error handle
    if(u16_LBA0==u16_LBA && u16_LBA1!=u16_LBA) // case.0
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "case.0 !!\n");
        u16_EraseBlk = u16_PBA1;
        u16_RetBlk = u16_PBA0;
        goto LABEL_ENDOF_DuplicatePBAHandler;
    }
    if(u16_LBA0!=u16_LBA && u16_LBA1==u16_LBA) // case.1
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "case.1 !!\n");
        u16_EraseBlk = u16_PBA0;
        u16_RetBlk = u16_PBA1;
        goto LABEL_ENDOF_DuplicatePBAHandler;
    }
    if(u16_LBA0!=u16_LBA && u16_LBA1!=u16_LBA) // case.2
    {
        U8 u8_BlkID=0;
        #if 1
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "Fatal Error - case.2, should never happen !!\n");
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "error handling...\n");

        // power-cut: data lose, but still can normal boot.
        u32_Ret = nand_ReadPages_Raw(u16_PBA0<<pNandDrv->u8_BlkPageCntBits,
            au8_PageBuf, (U8*)pDataBlkSpare_t, 1);
        if(UNFD_ST_SUCCESS == u32_Ret)
            u8_BlkID = pDataBlkSpare_t->u8_BlkID;

        if(UNFD_FTL_BLK_ID_DATA == u8_BlkID)
        {
            u16_RetBlk = u16_PBA1;
            u16_EraseBlk = 0xFFFF;

            LABEL_CASE2_ERASE:
            u32_Ret = nand_EraseBlk(u16_RetBlk<<pNandDrv->u8_BlkPageCntBits);
            if(UNFD_ST_SUCCESS != u32_Ret)
            {
                if(u32_Ret == UNFD_ST_ERR_E_FAIL)
                    drvNAND_MarkBadBlk(u16_RetBlk);
                if(0xFFFF != u16_EraseBlk)
                    goto LABEL_CASE2_FAIL;
                u16_RetBlk = u16_PBA0;
                u16_EraseBlk = u16_PBA1;
                goto LABEL_CASE2_ERASE;
            }

            u32_Ret = nand_format_DataBlk(u16_RetBlk, u16_LBA);
            if(UNFD_ST_SUCCESS != u32_Ret)
            {
                if(0xFFFF != u16_EraseBlk)
                    goto LABEL_CASE2_FAIL;
                u16_RetBlk = u16_PBA0;
                u16_EraseBlk = u16_PBA1;
                goto LABEL_CASE2_ERASE;
            }

            if(0xFFFF == u16_EraseBlk)
            {
                u16_EraseBlk = u16_PBA0;
                nand_erase_and_set_FreeBlk(u16_EraseBlk);
            }

            nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"case.2, EraseBlk: %X  RetBlk: %X \n", u16_EraseBlk, u16_RetBlk);
            return u16_RetBlk;
        }

        //if(UNFD_ST_ERR_ECC_FAIL == u32_Ret)
        //	nand_ECC_ErrorHandler(u16_PBA0);

        LABEL_CASE2_FAIL:
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "error handling fail !!\n");
        nand_die();

        #else
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "%X scan PBA:%X pages\n", u16_PBA0);
        for(u16_i=0; u16_i<pNandDrv->u16_BlkPageCnt; u16_i++)
        {
            u32_Ret = NC_ReadPages
                ((u16_PBA0<<pNandDrv->u8_BlkPageCntBits)+u16_i, au8_PageBuf, (U8*)pDataBlkSpare_t, 1);
            if(UNFD_ST_SUCCESS != u32_Ret)
            {
                u16_EraseBlk = u16_PBA0;
                u16_RetBlk = u16_PBA1;
                goto LABEL_ENDOF_DuplicatePBAHandler;
            }
        }

        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "%X scan PBA:%X pages\n", u16_PBA1);
        for(u16_i=0; u16_i<pNandDrv->u16_BlkPageCnt; u16_i++)
        {
            u32_Ret = NC_ReadPages
                ((u16_PBA1<<pNandDrv->u8_BlkPageCntBits)+u16_i, au8_PageBuf, (U8*)pDataBlkSpare_t, 1);
            if(UNFD_ST_SUCCESS != u32_Ret)
            {
                u16_EraseBlk = u16_PBA0;
                u16_RetBlk = u16_PBA1;
                goto LABEL_ENDOF_DuplicatePBAHandler;
            }
        }
        #endif
    }
    if(u16_LBA0==u16_LBA && u16_LBA1==u16_LBA) // case.3
    {
        for(u16_i=0 ; u16_i<pNandDrv->u16_BlkPageCnt; u16_i++)
        {
            u32_Ret = nand_ReadPages_Raw(((u16_PBA0<<pNandDrv->u8_BlkPageCntBits)+u16_i),
                au8_PageBuf, (U8*)pDataBlkSpare_t, 1);
            if(UNFD_ST_SUCCESS != u32_Ret)
            {
                u16_EraseBlk = u16_PBA0;
                u16_RetBlk = u16_PBA1;
                goto LABEL_ENDOF_DuplicatePBAHandler;
            }
        }
    }

    u16_EraseBlk = u16_PBA1;
    u16_RetBlk = u16_PBA0;

    LABEL_ENDOF_DuplicatePBAHandler:
    nand_erase_and_set_FreeBlk(u16_EraseBlk);

    nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"EraseBlk: %X  RetBlk: %X \n", u16_EraseBlk, u16_RetBlk);
    return u16_RetBlk;
}


// return
//  - 0: no FreeBlk available
//  - other: FreeBlk physical index
static U16 nand_get_FreeBlk_Zone1(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U16 u16_PBA;

    if(pNandDrv->u8_FBCnt_1 > 0)
    {
        u16_PBA = pNandDrv->au16_FBIdx_1[pNandDrv->u8_FBHead_1];

        pNandDrv->u8_FBHead_1++;
        if(pNandDrv->u8_FBHead_1 == pNandDrv->u8_FBMaxCnt_1)
            pNandDrv->u8_FBHead_1 = 0;

        pNandDrv->u8_FBCnt_1--;

        return u16_PBA;
    }

    // [CAUTION]: fatal error
    nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, Zone 1 has no free Blk");
    nand_die();
    return 0;
}

static U16 nand_get_FreeBlk_Zone0(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U16 u16_PBA;

    if(pNandDrv->u8_FBCnt_0 > 0)
    {
        u16_PBA = pNandDrv->au16_FBIdx_0[pNandDrv->u8_FBHead_0];

        pNandDrv->u8_FBHead_0++;
        if(pNandDrv->u8_FBHead_0 == pNandDrv->u8_FBMaxCnt_0)
            pNandDrv->u8_FBHead_0 = 0;

        pNandDrv->u8_FBCnt_0--;

        return u16_PBA;
    }

    // [CAUTION]: fatal error (should never be here, unless too many bad blk)
    nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"ERROR, Zone 0 has no free Blk");
    nand_die();
    return 0;
}


// 1: exist, 0: not exist
static U32 nand_check_FBExist
(
    U16 *pu16_FBIdx, U16 u16_PBA, U16 u16_FBSlotCnt, U8 u8_FBCnt, U8 u8_FBHead
)
{
    U8  u8_i;
    U16 *pu16_buf;

    // check if the PBA already in FBQueue_1
    pu16_buf = &pu16_FBIdx[u8_FBHead];

    // nand_debug(0, 1, "%X %X %X\n", pu16_buf, pNandDrv->au16_FBIdx_1, pNandDrv->u8_FBHead_1);
    for(u8_i=0; u8_i<u8_FBCnt; u8_i++)
    {
        if(u16_PBA == pu16_buf[0])
            break;

        pu16_buf++;
        if(pu16_buf == &pu16_FBIdx[u16_FBSlotCnt])
            pu16_buf = pu16_FBIdx;
    }

    if(u8_i < u8_FBCnt)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"Error, FreeBlk already exist: %X\n", u16_PBA);
        nand_die();
        return 1;
    }

    return 0;
}


static void nand_set_FreeBlk(U16 u16_PBA)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

    if(u16_PBA < pNandDrv->u16_FirstPBA_Zone1) // zone 0
    {
        if(pNandDrv->u8_FBCnt_0 < pNandDrv->u8_FBMaxCnt_0)
        {
            pNandDrv->au16_FBIdx_0[pNandDrv->u8_FBTail_0] = u16_PBA;

            pNandDrv->u8_FBTail_0++;
            if(pNandDrv->u8_FBTail_0 == pNandDrv->u8_FBMaxCnt_0)
                pNandDrv->u8_FBTail_0 = 0;

            pNandDrv->u8_FBCnt_0++;
        }
    }
    else // zone 1
    {
        #if UNFD_FTL_CHECK_ERROR
        nand_check_FBExist(pNandDrv->au16_FBIdx_1, u16_PBA,
            pNandDrv->u8_FBMaxCnt_1, pNandDrv->u8_FBCnt_1, pNandDrv->u8_FBHead_1);
        #endif

        // set into FBQueue_1
        if(pNandDrv->u8_FBCnt_1 < pNandDrv->u8_FBMaxCnt_1)
        {
            pNandDrv->au16_FBIdx_1[pNandDrv->u8_FBTail_1] = u16_PBA;

            pNandDrv->u8_FBTail_1++;
            if(pNandDrv->u8_FBTail_1 == pNandDrv->u8_FBMaxCnt_1)
                pNandDrv->u8_FBTail_1 = 0;

            pNandDrv->u8_FBCnt_1++;
        }
    }
}

static void nand_erase_and_set_FreeBlk(U16 u16_PBA)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U32 u32_Ret;

    u32_Ret = nand_EraseBlk(u16_PBA << pNandDrv->u8_BlkPageCntBits);
    if(UNFD_ST_SUCCESS == u32_Ret)
        nand_set_FreeBlk(u16_PBA);
    else
    {
        if(u32_Ret == UNFD_ST_ERR_E_FAIL)
            drvNAND_MarkBadBlk(u16_PBA);
    }
    #if UNFD_FTL_WL // trigger here
    if(1 == drvNAND_ChkRdy(0)) // ignore when building LUT
    {
        gu16_Erase_Counter[(u16_PBA - pNandDrv->u16_FirstPBA_Zone0)]++;
        if (gu16_Erase_Counter[(u16_PBA - pNandDrv->u16_FirstPBA_Zone0)] >= 0xFFF0)
        {
            nand_debug(UNFD_DEBUG_LEVEL, 1, "WL, meet the FFF0 ECnt\n");
            memset((void*)gu16_Erase_Counter, 0, (pNandDrv->u8_Zone1SubZoneCnt+1)*LUT_PBA_CNT);
        }

        pNandDrv->au32_ZoneTotalECnt[(u16_PBA - pNandDrv->u16_FirstPBA_Zone0)/LUT_PBA_CNT]++;

        if((gu16_Erase_Counter[(u16_PBA - pNandDrv->u16_FirstPBA_Zone0)] & WL_CHECK_EC_MASK)
            == 0)
        {
            ///*
            if(u16_PBA < pNandDrv->u16_FirstPBA_Zone1) // zone 0
                drvNAND_BGThread(NAND_MSG_WEAR_LEVELING);
                //drvNAND_SendMsg2BkgndTask(NAND_MSG_WEAR_LEVELING);
            else
                //drvNAND_SendMsg2BkgndTask(NAND_MSG_WEAR_LEVELING1);
                drvNAND_BGThread(NAND_MSG_WEAR_LEVELING1);
              //*/
        }
    }
    #endif
}

static void nand_dump_FreeBlk(U8 u8_ZoneID)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();

    U8  u8_head, u8_cnt, u8_last, u8_tail;
    U16 *pu16_buf;

    if(0 == u8_ZoneID)
    {
        u8_head = pNandDrv->u8_FBHead_0; // zone_0
        u8_tail = pNandDrv->u8_FBTail_0;
        u8_cnt = pNandDrv->u8_FBCnt_0;
        u8_last = pNandDrv->u8_FBMaxCnt_0;
        pu16_buf = pNandDrv->au16_FBIdx_0;
    }
    else
    {
        u8_head = pNandDrv->u8_FBHead_1; // zone_1
        u8_tail = pNandDrv->u8_FBTail_1;
        u8_cnt = pNandDrv->u8_FBCnt_1;
        u8_last = pNandDrv->u8_FBMaxCnt_1;
        pu16_buf = pNandDrv->au16_FBIdx_1;
    }

    nand_debug(0,1, "head:%X, tail:%X, cnt:%X, max:%X, buf:%lX \n",
        u8_head, u8_tail, u8_cnt, u8_last, (U32)pu16_buf);
    dump_mem((U8*)pu16_buf, u8_last<<1);

}

static void nand_dump_DiskInfo(U8 u8_DiskCnt)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8 u8_i;

    pNandDrv = pNandDrv;	// fix warning

    for(u8_i=0; u8_i<u8_DiskCnt; u8_i++)
        nand_debug(0,1,"LP[%01X], LBA:%04lX, Cnt: %04lX\n",
            u8_i, pNandDrv->DiskInfo_t[u8_i].u32_StartSector>>pNandDrv->u8_BlkSectorCntBits,
            pNandDrv->DiskInfo_t[u8_i].u32_SectorCnt>>pNandDrv->u8_BlkSectorCntBits);
    nand_debug(0,0,"\n");
}

static U32 nand_check_IfInitBB(U16 u16_PBA, U16 *pu16_InitBBArray, U16 u16_Cnt)
{
    U8 u8_i;

    for(u8_i=0; u8_i< u16_Cnt; u8_i++)
    {
        if(u16_PBA == pu16_InitBBArray[u8_i])
        {
            return 1;
        }
    }

    return 0;
}

U32 nand_get_InitBBT(U16 *pu16_InitBBArray, U16 *u16_Cnt)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U8 u8_i;
    U32 u32_Ret;
    U32 u32_ChkSum;

    INIT_BBT_t *pInitBBT = (INIT_BBT_t *)pNandDrv->pu8_PageDataBuf;
    DATA_BLK_SPARE_t *pDataBlkSpare_t = (P_DATA_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;


    // ----------------------------------------
    // get InitBBT
    for(u8_i=0; u8_i<10; u8_i++)
    {
        u32_Ret = nand_ReadSectors_Raw(
            (u8_i << pNandDrv->u8_BlkPageCntBits)+2, 0,
            (U8*)pInitBBT, (U8*)pDataBlkSpare_t, 1);
        if(UNFD_ST_SUCCESS == u32_Ret)
        {
            u32_ChkSum = drvNAND_CheckSum((U8*)(&(pInitBBT->u16_Cnt)), (512-4));
            if (u32_ChkSum == pInitBBT->u32_ChkSum)
            {
                nand_debug(UNFD_DEBUG_LEVEL_WARNING, 1, "get IBBT @ PBA:%Xh, ChkSum: %lXh, BBCnt: %u\n",
                    u8_i, u32_ChkSum, pInitBBT->u16_Cnt);
                break;
            }

        }
    }
    *u16_Cnt =  pInitBBT->u16_Cnt;
    if(*u16_Cnt > 506/2)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"NAND Err: too many BB: %u \n", *u16_Cnt);
        nand_die();
    }
    memcpy(pu16_InitBBArray,pInitBBT->au8_BadBlkTbl,506);

    if(u8_i < 10)
        u32_Ret = UNFD_ST_SUCCESS;
    else
        u32_Ret = 1;
    return  u32_Ret;
}

#if 0
static void nand_setup_FreeBlocks(void)
{
	NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	U8 u8_i;
	U32 u32_Ret;
    U16 u16_Cnt;

	U16 *pu16_InitBBArray;

    pu16_InitBBArray =(U16*) malloc(506);
	// ----------------------------------------
	// get InitBBT
   	u32_Ret = nand_get_InitBBT(pu16_InitBBArray,&u16_Cnt);

    if(u32_Ret != UNFD_ST_SUCCESS)
		nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"search Initial BBT fail: %Xh\n", u16_Cnt);

	//nand_debug(UNFD_DEBUG_LEVEL, 1, "dump InitBBT, blk:%X\n", u8_i);
	//dump_mem((U8*)pInitBBT, 0x200);
	//nand_debug(UNFD_DEBUG_LEVEL, 1, "\n\n");

	pNandDrv->u8_FBMaxCnt_0 = 0;
	pNandDrv->u8_FBMaxCnt_1 = 0;

	for(u8_i=0; u8_i<u16_Cnt; u8_i++)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR,1,"i: %X, bad PBA: %X \n",
		    u8_i, pu16_InitBBArray[u8_i]);

		if(pu16_InitBBArray[u8_i] >= pNandDrv->u16_FirstPBA_Zone1)
			pNandDrv->u8_FBMaxCnt_1++;
		else if(pu16_InitBBArray[u8_i] >= pNandDrv->u16_FirstPBA_Zone0)
			pNandDrv->u8_FBMaxCnt_0++;
	}

	free(pu16_InitBBArray);
	// ----------------------------------------
	//check if bad blk too many
	if(pNandDrv->u8_FBMaxCnt_0 > UNFD_MAX_ALLOWED_BB_CNT)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, ZONE_0 too many bad blocks: %X \n", pNandDrv->u8_FBMaxCnt_0);
		nand_die();
	}
	if(pNandDrv->u8_FBMaxCnt_1 > UNFD_MAX_ALLOWED_BB_CNT)
	{
		nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "ERROR, ZONE_1 too many bad blocks: %X \n", pNandDrv->u8_FBMaxCnt_0);
		nand_die();
	}

	// ----------------------------------------
	// reserved free blk cnt for bad blk
	pNandDrv->u8_FBMaxCnt_0 += UNFD_Z0_FREE_BLK_CNT;

	if(pNandDrv->u8_Zone1SubZoneCnt)
		pNandDrv->u8_FBMaxCnt_1 += UNFD_Z1_FREE_BLK_CNT;

	pNandDrv->u16_LogiFreeBlkCnt =
		pNandDrv->u8_FBMaxCnt_0+ pNandDrv->u8_FBMaxCnt_1;

	nand_debug(0,1,"FBMaxCnt_0: %Xh,  FBMaxCnt_1: %Xh \n",
		pNandDrv->u8_FBMaxCnt_0, pNandDrv->u8_FBMaxCnt_1);

}
#endif

static U32 nand_init_parameters(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    PARTITION_RECORD_t *pRecord = pNandDrv->pPartInfo->records, *pRecord1stLogi;
    U16 u16_i;
    U32 u32_Ret;
    U8  u8_FirstLogiPartIdx;

    u32_Ret = nand_init_parameters_Ex();
    if(u32_Ret != UNFD_ST_SUCCESS)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "NAND, no logical partition in PNI\n");
        return UNFD_ST_ERR_FTL_NO_ROOT_BLK;
    }

    //----------------------------------------------
    // - FirstPBA_Zone0
    // - FirstPBA_Zone1

    // FBMaxCnt_0 covers RootBlk cnt 
    pNandDrv->u16_FirstPBA_Zone0 = 
        pNandDrv->u16_RootBlkPhyIdx + UNFD_ROOT_BLK_CNT;
	
    pNandDrv->u16_FirstPBA_Zone1 = 
        pNandDrv->u16_FirstPBA_LogiArea + //pNandDrv->u16_FirstPBA_Zone0 + 
        pNandDrv->u16_Zone0LBACnt + pNandDrv->u8_FBMaxCnt_0;

    pNandDrv->u8_Zone1SubZoneCnt =  (pNandDrv->u16_LogiDataBlkCnt - pNandDrv->u16_Zone0LBACnt) / LUT_PBA_CNT;
    pNandDrv->u8_Zone1SubZoneCnt += (pNandDrv->u16_LogiDataBlkCnt - pNandDrv->u16_Zone0LBACnt) % LUT_PBA_CNT ?
			                        1 : 0;
    //----------------------------------------------
    #if UNFD_FTL_FMAP
    pNandDrv->u16_FM_FirstLBA = 0;//pNandDrv->u16_FirstPBA_Zone0;
    
    pNandDrv->u16_FM_LBACnt = FMAP_MAX_SECTOR_CNT >> pNandDrv->u8_BlkSectorCntBits;
    if(pNandDrv->u16_FM_LBACnt > FMAP_MAX_LBA_CNT)
        pNandDrv->u16_FM_LBACnt = FMAP_MAX_LBA_CNT;
    else if(0==pNandDrv->u16_FM_LBACnt)
        pNandDrv->u16_FM_LBACnt = 1;

    pNandDrv->u16_FM_PBACnt = pNandDrv->u16_FM_LBACnt * FMAP_SPACE_FACTOR;

    if(pNandDrv->u16_FM_PBACnt > pNandDrv->u8_FBMaxCnt_0-12) // 12 is the min FreeBlkCnt
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "FTL Err: no space for FMAP !\n");
        nand_die();
    }
    
    pNandDrv->u8_FM_PBAPageCnt = pNandDrv->u16_BlkPageCnt / FMAP_SPACE_FACTOR;
    #endif
    //----------------------------------------------
    // for DiskInfos	
    pRecord1stLogi = pNandDrv->pPartInfo->records;
    while(0 == (pRecord1stLogi->u16_PartType & UNFD_LOGI_PART))
        pRecord1stLogi++;
	
    u8_FirstLogiPartIdx = pRecord1stLogi - pNandDrv->pPartInfo->records;

    pRecord = pRecord1stLogi;
    for(u16_i=u8_FirstLogiPartIdx; u16_i<pNandDrv->pPartInfo->u16_PartCnt; u16_i++)
    {
        if(UNFD_LOGI_PART == (pRecord->u16_PartType & UNFD_LOGI_PART) && (u16_i-u8_FirstLogiPartIdx)<UNFD_MAX_DISK_NUM)
        {
            // [CAUTION]: should be the only one FAT partition
            pNandDrv->DiskInfo_t[u16_i-u8_FirstLogiPartIdx].u32_StartSector =
                (pRecord->u16_StartBlk - pNandDrv->u16_FirstPBA_LogiArea)
                << pNandDrv->u8_BlkSectorCntBits;

            pNandDrv->DiskInfo_t[u16_i-u8_FirstLogiPartIdx].u32_SectorCnt =
            pRecord->u16_BlkCnt << pNandDrv->u8_BlkSectorCntBits;
        }
        pRecord++;
    }

    #if 1 // debug
    nand_dump_FTL_Info();
    nand_dump_DiskInfo(u16_i-u8_FirstLogiPartIdx);
    #endif
    return UNFD_ST_SUCCESS;
}


static U32 nand_init_parameters_Ex(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    PARTITION_RECORD_t *pRecord1stLogi, *pRecordLastLogi;
    U16 u16_temp;

    if(pNandDrv->u16_FirstPBA_LogiArea)
        return UNFD_ST_SUCCESS;
	
    //----------------------------------------------
    // setup parameters
    //----------------------------------------------
    // - FirstPBA_LogiArea 
    // - LastPBA_LogiArea
    // - LogiDataBlkCnt
    // - Zone0LBACnt	(i.e. Z0DataBlkCnt)
    // - LogiFreeBlkCnt
    // - FBMaxCnt_0
    // - FBMaxCnt_1	
    pRecord1stLogi = pNandDrv->pPartInfo->records;
    while(0 == (pRecord1stLogi->u16_PartType & UNFD_LOGI_PART))
        pRecord1stLogi++;	
    
    if(pRecord1stLogi - pNandDrv->pPartInfo->records >= pNandDrv->pPartInfo->u16_PartCnt)       //no logical partition in pni
        return UNFD_ST_ERR_LOGI_PART;

    pNandDrv->u16_FirstPBA_LogiArea = pRecord1stLogi->u16_StartBlk;
	
    pRecordLastLogi = pRecord1stLogi;
    while(pRecordLastLogi->u16_PartType & UNFD_LOGI_PART)
        pRecordLastLogi++;
    pRecordLastLogi--;
	
    pNandDrv->u16_LastPBA_LogiArea = 
        pRecordLastLogi->u16_StartBlk + pRecordLastLogi->u16_BlkCnt;

    // ----------------------------
    // what you see is what you get.
    pNandDrv->u16_LogiDataBlkCnt = pNandDrv->u16_LastPBA_LogiArea - pNandDrv->u16_FirstPBA_LogiArea;
    // ----------------------------

    u16_temp = pNandDrv->u16_LogiDataBlkCnt > LUT_PBA_CNT ? LUT_PBA_CNT : pNandDrv->u16_LogiDataBlkCnt;
    pNandDrv->u16_Zone0LBACnt = u16_temp;
    pNandDrv->u8_FBMaxCnt_0 = BACKUP_BBCNT(u16_temp) + UNFD_Z0_FREE_BLK_CNT;

    if(pNandDrv->u16_LogiDataBlkCnt > u16_temp)
    {
        u16_temp = pNandDrv->u16_LogiDataBlkCnt - u16_temp;
        pNandDrv->u8_FBMaxCnt_1 = BACKUP_BBCNT(u16_temp) + UNFD_Z1_FREE_BLK_CNT;		
    }
    else
        pNandDrv->u8_FBMaxCnt_1 = 0;

    pNandDrv->u16_LogiFreeBlkCnt = pNandDrv->u8_FBMaxCnt_0 + pNandDrv->u8_FBMaxCnt_1;
	 
    pNandDrv->u16_LastPBA_LogiArea += pNandDrv->u16_LogiFreeBlkCnt;
    #if 0
    if(0 == pNandDrv->u16_RootBlkPhyIdx)
    {   
        pNandDrv->u16_RootBlkPhyIdx = pNandDrv->u16_FirstPBA_LogiArea;
        while(0 == drvNAND_IsGoodBlk(pNandDrv->u16_RootBlkPhyIdx))
            pNandDrv->u16_RootBlkPhyIdx++;
    }
    pNandDrv->u16_LastPBA_LogiArea += pNandDrv->u16_RootBlkPhyIdx - pNandDrv->u16_FirstPBA_LogiArea;
    #endif
    if(pNandDrv->u16_LastPBA_LogiArea > pNandDrv->u16_BlkCnt)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "NAND Err: LogiSpace > NAND space\n");
        nand_die();
    }	

    return UNFD_ST_SUCCESS;
}

void nand_dump_FTL_Info(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
	
    nand_debug(UNFD_DEBUG_LEVEL, 1,
        "NAND, FirstPBA_LogiArea: %Xh  RootBlkPhyIdx: %Xh  LastPBA_LogiArea: %Xh  \n",
        pNandDrv->u16_FirstPBA_LogiArea, pNandDrv->u16_RootBlkPhyIdx, pNandDrv->u16_LastPBA_LogiArea);
    nand_debug(UNFD_DEBUG_LEVEL, 1,
        "NAND, LogiDataBlkCnt: %Xh	LogiFreeBlkCnt: %Xh  \n",
        pNandDrv->u16_LogiDataBlkCnt, pNandDrv->u16_LogiFreeBlkCnt);
    nand_debug(UNFD_DEBUG_LEVEL, 1,
        "NAND, FirstPBA_Zone0: %Xh	Zone0LBACnt: %Xh  FBMaxCnt_0: %Xh \n",
        pNandDrv->u16_FirstPBA_Zone0, pNandDrv->u16_Zone0LBACnt, pNandDrv->u8_FBMaxCnt_0);
    nand_debug(UNFD_DEBUG_LEVEL, 1,
        "NAND, FirstPBA_Zone1: %Xh	Zone1SubZoneCnt: %Xh  FBMaxCnt_1: %Xh \n",
        pNandDrv->u16_FirstPBA_Zone1, pNandDrv->u8_Zone1SubZoneCnt, pNandDrv->u8_FBMaxCnt_1);
}

U32 nand_Init_FTL(void)
{
    U32 u32_Ret;

    nand_debug(UNFD_DEBUG_LEVEL, 1, "2011_0726, +\n");

    //----------------------------------------------
    // check FTL version
    //----------------------------------------------
    u32_Ret = nand_search_RootBlk();
    if(UNFD_ST_SUCCESS != u32_Ret)
        goto LABEL_END_OF_FTL_INIT;

    //----------------------------------------------
    // init parameters
    //----------------------------------------------
    u32_Ret = nand_init_parameters();
    if(UNFD_ST_SUCCESS != u32_Ret)
        goto LABEL_END_OF_FTL_INIT;
    
    //----------------------------------------------
    // build LUT
    //----------------------------------------------
    u32_Ret = nand_build_LUT0();

    LABEL_END_OF_FTL_INIT:
    nand_debug(UNFD_DEBUG_LEVEL, 1, "-\n");
    return u32_Ret;
}


static U32 nand_search_RootBlk(void)
{
    NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
    U32 u32_Ret;
    U32 u32_Row, u32_Flag;
    U16 u16_i, u16_BadBlkCnt;
    ROOT_BLK_DATA_t *pRootBlkData_t;
    ROOT_BLK_SPARE_t *pRootBlkSpare_t;
	
    u32_Ret = nand_init_parameters_Ex();
    if(u32_Ret != UNFD_ST_SUCCESS)
    {
        nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "NAND, no logical partition in PNI\n");
        return UNFD_ST_ERR_FTL_NO_ROOT_BLK;
    }
	
    //----------------------------------------------
    pRootBlkData_t = (P_ROOT_BLK_DATA_t)pNandDrv->pu8_PageDataBuf;
    pRootBlkSpare_t = (P_ROOT_BLK_SPARE_t)pNandDrv->pu8_PageSpareBuf;
    u32_Flag = 0;
    u16_BadBlkCnt = 0;

    for(u16_i=0; u16_i<UNFD_ROOT_BLK_CNT; u16_i++)
    {
        u32_Row = (pNandDrv->u16_FirstPBA_LogiArea + u16_i + u16_BadBlkCnt) <<
            pNandDrv->u8_BlkPageCntBits;
        u32_Ret = nand_ReadSectors_Raw(u32_Row, 0, (U8*)pRootBlkData_t, (U8*)pRootBlkSpare_t, 1);
        if(UNFD_ST_SUCCESS == u32_Ret)
        {
            if(0xFF == pRootBlkSpare_t->u8_BadBlkMark &&
                UNFD_FTL_BLK_ID_ROOT == pRootBlkSpare_t->u8_BlkID)
            {
                pNandDrv->u16_RootBlkPhyIdx = pNandDrv->u16_FirstPBA_LogiArea +
                    u16_i + u16_BadBlkCnt;

                nand_debug(UNFD_DEBUG_LEVEL,1,"NAND, FTL ver. NAND:%X Code:%X\n", pRootBlkSpare_t->u8_FTLVerNum, UNFD_FTL_VERSION);

                if(UNFD_FTL_VERSION == pRootBlkSpare_t->u8_FTLVerNum)
                    u32_Flag = 1; // find the Root Blk
                else
                    u32_Ret = UNFD_ST_ERR_FTL_VERSION_MISS;

                break;
            }
            else if(0xFF != pRootBlkSpare_t->u8_BadBlkMark)
            {
                u16_BadBlkCnt++;
                u16_i--; // skip a PBA for this bad blk
                continue;
            }
            else
            {
                nand_debug(UNFD_DEBUG_LEVEL,1,"Row:%lX  Blk:%lX  BBMark:%X  BlkID:%X\n",
                    u32_Row, u32_Row>>pNandDrv->u8_BlkPageCntBits,
                    pRootBlkSpare_t->u8_BadBlkMark, pRootBlkSpare_t->u8_BlkID);
                    dump_mem((U8*)pRootBlkSpare_t, 0x10);
                u32_Ret = UNFD_ST_ERR_FTL_NO_ROOT_BLK;
                break;
            }
        }
        else// ERROR_Handle, should NOT be here
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "NAND, ERROR_Handle, read RootBlk failed: %X\n",
                pNandDrv->u16_FirstPBA_LogiArea + u16_i);

            if(UNFD_ST_ERR_ECC_FAIL == u32_Ret) // do read-disturbance for RootBlk
            {
                // write RootBlk
                pRootBlkSpare_t->u8_BadBlkMark = 0xFF;
                pRootBlkSpare_t->u8_BlkID = UNFD_FTL_BLK_ID_ROOT;
                pRootBlkSpare_t->u8_FTLVerNum = UNFD_FTL_VERSION;
                pRootBlkSpare_t->u8_RDD3 = 0xFF;
                pRootBlkSpare_t->u8_RDD4 = 0xFF;
                pRootBlkSpare_t->u8_RDD5 = 0xFF;
                u32_Ret = nand_EraseBlk(u32_Row);
                if(UNFD_ST_SUCCESS == u32_Ret)
                    //u32_Ret = nand_WriteSectors_Raw(u32_Row, 0, (U8*)pRootBlkData_t, (U8*)pRootBlkSpare_t, 1);
                    u32_Ret = nand_WritePages_Raw(u32_Row, (U8*)pRootBlkData_t, (U8*)pRootBlkSpare_t, 1);
            }
            if(UNFD_ST_SUCCESS != u32_Ret)
            {
                drvNAND_MarkBadBlk(u32_Row >> pNandDrv->u8_BlkPageCntBits);
                nand_die();
            }

            nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "NAND, re-build RootBlk ok\n");
            u32_Flag = 1;
        }
    }

    //----------------------------------------------
    if(0 == u32_Flag)
    {
        if(UNFD_ST_ERR_FTL_VERSION_MISS == u32_Ret)
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "NAND, FTL_VERSION mismatch, NAND:%X Code:%X\n",
                pRootBlkSpare_t->u8_FTLVerNum, UNFD_FTL_VERSION);
            return UNFD_ST_ERR_FTL_VERSION_MISS;
        }
        else
        {
            nand_debug(UNFD_DEBUG_LEVEL_ERROR, 1, "NAND, no Root Blk\n");
            return UNFD_ST_ERR_FTL_NO_ROOT_BLK;
        }
    }

	nand_debug(UNFD_DEBUG_LEVEL, 1, "RootBlk:%X\n", pNandDrv->u16_RootBlkPhyIdx);
    return UNFD_ST_SUCCESS;
}

#endif // defined(__VER_UNFD_FTL__)
