/*
 * (C) Copyright 2003
 * Kyle Harris, kharris@nexus-tech.net
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <watchdog.h>
#include <fat.h>
#include <drvWDT.h>
#include <MsMath.h>
#include <u-boot/crc.h>
#include <jffs2/load_kernel.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <environment.h>
#include <search.h>
#include <MsNand.h>
#include "../drivers/mstar/unfd/inc/common/drvNAND.h"
#ifdef CONFIG_ENABLE_WIDESTORAGE
#include <MsDisk.h>
#endif

#define PLAT_HW_TIMER_HZ          (12*1000*1000)

extern int nand_unfd_init(void);

static S16  RamTest(volatile U32 BaseAddr, U32 RamSize);
static void NC_Test_ErrBitStatus_R(U16 u16_PBA, U16 u16_MaxPECnt, U32 LoopCnt);
static void NC_Test_ErrBitStatus_W(U16 u16_PBA, U16 u16_MaxPECnt);
static U32  NC_Test_BIST(U16 u16_PBA);
static U32  NC_Test_APIs(U16 u16_PBA);
static void NC_Test_CheckAll0xFF(void);
static U32  NC_Test_2PAPIs(U16 u16_PBA);
static U32  NC_Test_PwrCut(U16 LoopCnt);
static U32  NC_Test_PwrCut_Toggle(void);

U8 *pu8_NandTestDatBuf = (U8*)DMA_W_ADDR; 
U8 *pu8_NandTestTmpBuf = (U8*)DMA_R_ADDR; 
U8 *pu8_NandTestRddBuf = (U8*)DMA_W_SPARE_ADDR;

// =======================================================================
// test commands
// =======================================================================
int do_msnand(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    NAND_DRIVER *pNandDrv;
    
    #ifdef CONFIG_ENABLE_WIDESTORAGE
    if(MSDISK_eMMC == ChkDiskType())
    {
        printf("%s: eMMC disk\n", __func__);
        return -1;
    }
    #endif 

    //-------------------------------
    pNandDrv = drvNAND_get_DrvContext_address();

    if (argc < 2)
        return cmd_usage(cmdtp);

       //===========================================
       if (strcmp(argv[1], "info") == 0)
    {
        nand_debug(0, 0, "\n");
        dump_nand_driver(pNandDrv);
        nand_debug(0, 0, "\n");
        dump_part_info(pNandDrv->pPartInfo);
        nand_debug(0, 0, "\n");
        
        if (argc==3 && strcmp(argv[2], "more")==0)
        {
            dump_nand_PPM();
            nand_debug(0, 0, "\n");
            
            NC_DumpRegisters();
            NC_DumpDebugBus();
        }
        return 0;
       }
    //===========================================
    else if (strcmp(argv[1], "init")==0)
    {
        if (argc == 2)
            nand_unfd_init();
        else if (argc == 5)
        {            
        }        
        return 0;
    }
    //===========================================
    else if (strcmp(argv[1], "temp")==0)
    {
    }
    #if defined (CONFIG_NAND_SECURITY_GEN) && CONFIG_NAND_SECURITY_GEN==1
    else if (strcmp(argv[1], "wp")==0)
    {
        U16 port;

        port = simple_strtoul(argv[3], NULL, 16);
        
        if (strcmp(argv[2], "range")==0)
        {
            U16 b1, b2;
            b1 = simple_strtoul(argv[3], NULL, 16);
            b2 = simple_strtoul(argv[4], NULL, 16);
            drvNAND_SetSecureRange(
                b1 << pNandDrv->u8_BlkPageCntBits,
                ((b2+1) << pNandDrv->u8_BlkPageCntBits)-1, port);
        }
        else if (strcmp(argv[2], "enable")==0)
        {
            U16 enable;
            enable = simple_strtoul(argv[3], NULL, 16);
            if(enable)
                drvNAND_EnSecureRange(port);
            else
                drvNAND_DisSecureRange(port);
        }
        else if (strcmp(argv[2], "lock")==0)
        {
            U16 lock;
            lock = simple_strtoul(argv[3], NULL, 16);
            if(lock)
                drvNAND_LockSecureRange(port);
        }    
    }
    #endif
    //===========================================
    #ifdef CONFIG_MSTAR_FTL_SD
    else if (strcmp(argv[1], "ftlsd")==0)
    {
        U32 loop, buf, BCnt;
        
        if (strcmp(argv[2], "boardinit")==0)
        {
            FTLSd_BlkDev_Init();
        }
        else if (strcmp(argv[2], "init")==0)
        {
            FTLSd_Init(0,0);
        }
        else if (strcmp(argv[2], "info")==0)
        {
            FTLSd_DumpData();

            if (strcmp(argv[3], "more")==0)
                FTLSd_DumpDevInfo();
        }
        else if(strcmp(argv[2], "wrc")==0)
        {
            loop = simple_strtoul(argv[3], NULL, 16);
            FtlSdTest_DataWRC(loop, 1);
        }
        else if(strcmp(argv[2], "rc")==0)
        {
            loop = simple_strtoul(argv[3], NULL, 16);
            FtlSdTest_DataRC(loop, 1);
        }
        else if(strcmp(argv[2], "dump")==0)
        {
            buf = simple_strtoul(argv[3], NULL, 16);
            BCnt = simple_strtoul(argv[4], NULL, 16);
            FTLSd_ReadData(buf, BCnt);
            dump_mem(buf, BCnt);            
        }
        else if(strcmp(argv[2], "write")==0)
        {
            buf = simple_strtoul(argv[3], NULL, 16);
            BCnt = simple_strtoul(argv[4], NULL, 16);
            FTLSd_WriteData(buf, BCnt);
        }
    }
    #endif
    //===========================================
    else if (strcmp(argv[1], "life")==0)
    {
        if (argc == 5)
        {
            U16 u16_PBA = simple_strtoul(argv[2], NULL, 16);
            U16 u16_MaxPECnt = simple_strtoul(argv[3], NULL, 10);
            U32 LoopCnt = simple_strtoul(argv[4], NULL, 16);
            NC_Test_ErrBitStatus_W(u16_PBA, u16_MaxPECnt);
            NC_Test_ErrBitStatus_R(u16_PBA, u16_MaxPECnt, LoopCnt);
        }
        else
            printf("incorrect argc: %u \n", argc);
    }
    //===========================================
    else if (strcmp(argv[1], "bb")==0)
    {
        U16 u16_i;
        
        // set block to be bad block purposely
        if (4==argc && strcmp(argv[2], "set")==0)
        {
            U32 u32_err;

            if (argc < 4)
                return 0;
            u16_i = simple_strtoul(argv[3], NULL, 16);
            u32_err = drvNAND_MarkBadBlk(u16_i);
            if(UNFD_ST_SUCCESS == u32_err)
                printf("\n set %Xh PBA BB ok \n", u16_i);
            else
                printf("\n set %Xh PBA BB ng \n", u16_i);

            return 0;
        }
        else if (4==argc && strcmp(argv[2], "dump")==0)
        {
            if (argc < 4)
                return 0;
            u16_i = simple_strtoul(argv[3], NULL, 16);
            drvNAND_ChkGoodBlk(u16_i, 1);
            return 0;
        }        
        
        for(u16_i=0; u16_i<pNandDrv->u16_BlkCnt; u16_i++)
        {
            if(0==drvNAND_IsGoodBlk(u16_i))
                printf("BB %4Xh \n", u16_i);
        }
        return 0;
    }
    else if (strcmp(argv[1], "erase")==0)
    {
        if (argc == 4)
        {
            U16 PBA = simple_strtoul(argv[2], NULL, 16);
            U16 cnt = simple_strtoul(argv[3], NULL, 16), i;
            for(i=PBA; i<PBA+cnt; i++)
            {
                if(NC_EraseBlk(i<<pNandDrv->u8_BlkPageCntBits))
                    printf("erase PBA %Xh fail \n", PBA);            
            }
            printf("\ndone, PBA: %Xh ~ %Xh \n", PBA, PBA+cnt-1);
        }
        else
            printf("incorrect argc: %u \n", argc);
        return 0;
    }
    //===========================================
    else if (strcmp(argv[1], "test")==0)
    {
        if(strcmp(argv[2], "temp")==0)
        {
            
            return 0;
        }
        else if(strcmp(argv[2], "BIST")==0)
        {
            U16 u16_PBA;
            if(argc < 4)
            {
                printf("argc: %u < 4 \n", argc);
                return 0;
            }
            u16_PBA = simple_strtoul(argv[3], NULL, 16);
            NC_Test_BIST(u16_PBA);
            NC_Test_APIs(u16_PBA);
            NC_Test_CheckAll0xFF();
        }
        else if(strcmp(argv[2], "pwroff")==0)
        {
            #if 0
            // SW sets PM to power-off
            nand_SysPwrOff();
            #else
            
            #if defined(ENABLE_NAND_POWER_SAVING_MODE) && ENABLE_NAND_POWER_SAVING_MODE
            U32 i=0;
            
            //while(1)
            for(i=0; i<0x10000000; i++)
                nand_CheckPowerCut();
            #endif

            #endif
        }        
        else if(strcmp(argv[2], "timer")==0)
        {
            U32 u32_tick, u32_i, u32_diff;

            for(u32_i=0; u32_i<3; u32_i++)
            {
                HWTimer_Start();
                nand_hw_timer_delay(HW_TIMER_DELAY_1ms);
                u32_tick = HWTimer_End();
                if(u32_tick > PLAT_HW_TIMER_HZ/1000) // 1ms
                {
                    u32_diff = u32_tick-PLAT_HW_TIMER_HZ/1000;
                    printf("%u, diff:%u.%u%%\n", u32_tick, 
                        u32_diff*100/(PLAT_HW_TIMER_HZ/1000), 
                        (u32_diff*1000/(PLAT_HW_TIMER_HZ/1000))%10);
                }
            }
            for(u32_i=0; u32_i<5; u32_i++)
            {
                printf("%u ", u32_i);
                nand_hw_timer_delay(HW_TIMER_DELAY_1s); // 1ms
            }           
            printf("\n");
        }
        else if(strcmp(argv[2], "mem")==0)
        {
            extern S16 RamTest(volatile U32 BaseAddr, U32 RamSize);
            U16 i, loop;
            U32 start, end;

            loop = simple_strtoul(argv[3], NULL, 10);
            
            for(i=0; i<loop; i++)
            {
                start = simple_strtoul(argv[5], NULL, 10);
                end = simple_strtoul(argv[6], NULL, 10);
                printf("[%u] %Xh %Xh \n", i, start, end);

                if(RamTest(start, end-start))
                    return 0;                
            }            
        }
        
        return 0;
    }
    //===========================================
    
    return 1;
}


// =======================================================================
// others
// =======================================================================

U32 NC_Test_2PAPIs(U16 u16_PBA)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 *p1=(U32*)pu8_NandTestDatBuf, *p2=(U32*)pu8_NandTestTmpBuf, 
        *ps=(U32*)pu8_NandTestRddBuf, i, err;

    // data pattern
    for(i=0; i<pNandDrv->u16_PageByteCnt/4; i++)
    {
        p1[i] = i;  p2[i] = ~p1[i];
    }
    memset(pu8_NandTestRddBuf, 0xFF, pNandDrv->u16_SpareByteCnt);

    // erase
    err=NC_EraseBlk2P(u16_PBA<<pNandDrv->u8_BlkPageCntBits);
    if(err)
    {   nand_debug(0,1,"NC_EraseBlk2P fail: %Xh\n", err);
        return err;
    }

    // write page    
    err=NC_WritePages2P(u16_PBA<<pNandDrv->u8_BlkPageCntBits, (U8*)p1, (U8*)ps, 1);
    if(err)
    {   nand_debug(0,1,"NC_WritePages2P fail: %Xh\n", err);
        return err;
    } 

    // read page    
    err=NC_ReadPages2P(u16_PBA<<pNandDrv->u8_BlkPageCntBits, (U8*)p2, (U8*)ps, 1);
    if(err)
    {   nand_debug(0,1,"NC_ReadPages2P fail: %Xh\n", err);
        return err;
    }
    // compare
    err=nand_CompareData((U8*)p1, (U8*)p2, pNandDrv->u16_PageByteCnt);
    if(err)
    {   nand_debug(0,1,"CompareData fail: %Xh\n", err);
        return err;
    }

    // read sectors
    err=NC_ReadSectors2P(u16_PBA<<pNandDrv->u8_BlkPageCntBits, 6, (U8*)p2, (U8*)ps, 4);
    if(err)
    {   nand_debug(0,1,"NC_ReadPages2P fail: %Xh\n", err);
        return err;
    }
    // compare
    err=nand_CompareData((U8*)p1+pNandDrv->u16_SectorByteCnt*6, 
        (U8*)p2, pNandDrv->u16_SectorByteCnt*4);
    if(err)
    {   nand_debug(0,1,"CompareData fail: %Xh\n", err);
        return err;
    }

    // erase
    err=NC_EraseBlk2P(u16_PBA<<pNandDrv->u8_BlkPageCntBits);
    if(err)
    {   nand_debug(0,1,"NC_EraseBlk2P fail: %Xh\n", err);
        return err;
    }
    nand_debug(0,1,"PBA: %Xh - test ok\n", u16_PBA);
    return UNFD_ST_SUCCESS;
}

U32 NC_Test_APIs(U16 u16_PBA)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    volatile U32 u32_err, u32_i;
    volatile U16 u16_FreeP, u16_LPIdx;

    //ftl_trace("\n");
    
    while(0==drvNAND_IsGoodBlk(u16_PBA))
    {
        nand_debug(0, 1, "bad blk: %Xh \n", u16_PBA);
        u16_PBA++;
    }

    for(u32_i=0; u32_i<pNandDrv->u16_PageByteCnt; u32_i++)
        pu8_NandTestDatBuf[u32_i]=u32_i;
    for(u32_i=0; u32_i<pNandDrv->u16_SpareByteCnt; u32_i++)
        pu8_NandTestRddBuf[u32_i]=0xFF;

    u32_err = NC_EraseBlk(u16_PBA<<pNandDrv->u8_BlkPageCntBits);
    if(UNFD_ST_SUCCESS != u32_err)
    {
        nand_debug(0,1,"erase %Xh blk fail: %Xh \n", u16_PBA, u32_err);
        return u32_err;
    }

    //ftl_trace("\n");
    if(0==drvNAND_IsFreeBlk(u16_PBA))
    {
        nand_debug(0,1,"IsFreeBlk fail: %Xh PBA \n", u16_PBA);
        return UNFD_ST_ERR_INVALID_ADDR;
    }

    // ----------------------------------------------
    for(u16_LPIdx=0, u32_i=0; u32_i<=pNandDrv->u16_BlkPageCnt; u32_i++)
    {
        //nand_debug(0,1,"LPIdx: %Xh  PIdx: %Xh \n", u16_LPIdx, u32_i);
        
        u16_FreeP = drvNAND_FindFreePage(u16_PBA);
        if(u16_FreeP != u32_i)
        {
            nand_debug(0,1,
                "FindFreePage fail: %Xh PBA, %Xh page, wrong page: %Xh \n", u16_PBA, u32_i, u16_FreeP);
            return ~UNFD_ST_SUCCESS;
        }
        if(u32_i == pNandDrv->u16_BlkPageCnt)
            break;

        if(u16_LPIdx < pNandDrv->u16_BlkLowPCnt)
        {
            u16_FreeP = drvNAND_FindFreeLSBPageIdx(u16_PBA);
            if(u16_FreeP != u16_LPIdx)//ga_tPairedPageMap[u16_LPIdx].u16_LSB)
            {   
                nand_debug(0,1,
                    "FindFreeLSBPageIdx fail: LPIdx %Xh, FreeLPIdx: %Xh \n", 
                    u16_LPIdx, u16_FreeP);
                return ~UNFD_ST_SUCCESS;
            }
        }
        
        u32_err = NC_WritePages((u16_PBA<<pNandDrv->u8_BlkPageCntBits)+u32_i, 
            pu8_NandTestDatBuf, pu8_NandTestRddBuf, 1);
        if(UNFD_ST_SUCCESS != u32_err)
        {
            nand_debug(0,1,"write %Xh blk, %Xh page fail: %Xh \n", 
                u16_PBA, u16_PBA<<pNandDrv->u8_BlkPageCntBits, u32_err);
            return u32_err;
        }

        if(u32_i == ga_tPairedPageMap[u16_LPIdx].u16_LSB)
            u16_LPIdx++;
    }

    // ----------------------------------------------
    //ftl_trace("\n");
    
    u32_err = NC_EraseBlk(u16_PBA<<pNandDrv->u8_BlkPageCntBits);
    if(UNFD_ST_SUCCESS != u32_err)
    {
        nand_debug(0,1,"erase %Xh blk fail: %Xh \n", u16_PBA, u32_err);
        return u32_err;
    }

    if(0==drvNAND_IsFreeBlk(u16_PBA))
    {
        nand_debug(0,1,"IsFreeBlk fail: %Xh PBA \n", u16_PBA);
        return ~UNFD_ST_SUCCESS;
    }

    //ftl_trace("\n");
    
    for(u32_i=0; u32_i<pNandDrv->u16_BlkLowPCnt; u32_i++)
    {
        //nand_debug(0,1,"LPIdx: %Xh \n", u32_i);
        
        u16_FreeP = drvNAND_FindFreePage(u16_PBA);
        if(u16_FreeP != ga_tPairedPageMap[u32_i].u16_LSB)
        {
            nand_debug(0,1,
                "FindFreePage fail: %Xh PBA, %Xh page, wrong page: %Xh \n", 
                u16_PBA, u32_i, u16_FreeP);
            return ~UNFD_ST_SUCCESS;
        }

        u16_FreeP = drvNAND_FindFreeLSBPageIdx(u16_PBA);
        if(u16_FreeP != u32_i)//ga_tPairedPageMap[u32_i].u16_LSB)
        {   
            nand_debug(0,1,
                "FindFreeLSBPageIdx fail: %Xh PBA, %Xh LP, wrong LP Idx: %Xh \n", 
                u16_PBA, ga_tPairedPageMap[u32_i].u16_LSB, u16_FreeP);
            return ~UNFD_ST_SUCCESS;
        }

        // ---------------------------------
        u32_err = NC_WritePages(
            (u16_PBA<<pNandDrv->u8_BlkPageCntBits)+ga_tPairedPageMap[u32_i].u16_LSB, 
            pu8_NandTestDatBuf, pu8_NandTestRddBuf, 1);
        if(UNFD_ST_SUCCESS != u32_err)
        {
            nand_debug(0,1,"write %Xh blk, %Xh page fail: %Xh \n", 
                u16_PBA, (u16_PBA<<pNandDrv->u8_BlkPageCntBits)+ga_tPairedPageMap[u32_i].u16_LSB, u32_err);
            return u32_err;
        }

        u16_FreeP = drvNAND_FindFreePage(u16_PBA);
        if(u16_FreeP != ga_tPairedPageMap[u32_i].u16_LSB+1)
        {
            nand_debug(0,1,
                "FindFreePage fail: %Xh PBA, %Xh page, wrong page: %Xh \n", 
                u16_PBA, ga_tPairedPageMap[u32_i].u16_LSB+1, u16_FreeP);
            return ~UNFD_ST_SUCCESS;
        }

        u16_FreeP = drvNAND_FindFreeLSBPageIdx(u16_PBA);
        if(u16_FreeP != u32_i+1)//ga_tPairedPageMap[u32_i+1].u16_LSB)
        {   
            nand_debug(0,1,
                "FindFreeLSBPageIdx fail: %Xh PBA, %Xh LP, wrong LP Idx: %Xh \n", 
                u16_PBA, ga_tPairedPageMap[u32_i+1].u16_LSB, u16_FreeP);
            return ~UNFD_ST_SUCCESS;
        }

        // ---------------------------------
        if(u32_i+1 < pNandDrv->u16_BlkLowPCnt)
        {
            u32_err = drvNAND_WriteDummyToPages(
                ga_tPairedPageMap[u32_i].u16_LSB+1, 
                ga_tPairedPageMap[u32_i+1].u16_LSB-1,
                u16_PBA<<pNandDrv->u8_BlkPageCntBits);
            if(UNFD_ST_SUCCESS != u32_err)
            {
                nand_debug(0,1,"drvNAND_WriteDummyToPages fail: %Xh %Xh %Xh\n", 
                    u32_i, ga_tPairedPageMap[u32_i].u16_LSB+1, ga_tPairedPageMap[u32_i+1].u16_LSB-1);
                return u32_err;
            }
        }
    }
       
    nand_debug(0,1,"OK\n");
    NC_EraseBlk(u16_PBA<<pNandDrv->u8_BlkPageCntBits);
    return UNFD_ST_SUCCESS;
    
}


// ===================================
// BIST
U32 NC_Test_Basic(U16 u16_PBA, U16 u16_Op, U16 LoopCnt)
{
    //NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    //U32 u32_Err=UNFD_ST_SUCCESS, u32_Row = u16_PBA<<pNandDrv->u8_BlkPageCntBits, i;
    
    return UNFD_ST_SUCCESS;
}

U32 NC_Test_BIST(U16 u16_PBA) 
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_err, u32_i;
    U16 PBA2;

    if(/*u16_PBA < FtlDev.u16_StartPBA ||*/ u16_PBA < 20) // hash/uboot
    {
        nand_debug(0,1,"PBA1:%Xh too small\n", u16_PBA);
        return UNFD_ST_ERR_INVALID_ADDR;
    }

    if((U32)pu8_NandTestTmpBuf-(U32)pu8_NandTestDatBuf < pNandDrv->u16_PageByteCnt)
    {
        nand_debug(0,1,"buf size:%Xh < page size: %Xh\n", 
            (U32)pu8_NandTestTmpBuf-(U32)pu8_NandTestDatBuf, 
            pNandDrv->u16_PageByteCnt);
        return UNFD_ST_ERR_INVALID_ADDR;
    }

    // --------------------------
    // get the next PBA as PBA2 to test
    // --------------------------
    #if 1
    if(0==drvNAND_IsGoodBlk(u16_PBA))
    {
        nand_debug(0,1,"PBA1:%Xh, is bad blk \n", u16_PBA);
        return UNFD_ST_ERR_INVALID_ADDR;
    }
    
    PBA2 = u16_PBA + pNandDrv->u8_PlaneCnt;
    if(0==drvNAND_IsGoodBlk(PBA2))
    {
        nand_debug(0,1,"PBA2:%Xh, is bad blk \n", PBA2);
        return UNFD_ST_ERR_INVALID_ADDR;
    }
    #endif

    u32_err = NC_EraseBlk(u16_PBA<<pNandDrv->u8_BlkPageCntBits);
    if(UNFD_ST_SUCCESS != u32_err)
    {
        nand_debug(0,1,"erase %Xh blk fail: %Xh \n", u16_PBA, u32_err);
        return u32_err;
    }

    for(u32_i=0; u32_i<pNandDrv->u16_PageByteCnt; u32_i++)
    {
        pu8_NandTestDatBuf[u32_i]=u32_i/4;
        pu8_NandTestTmpBuf[u32_i]=~pu8_NandTestDatBuf[u32_i];
    }
    pu8_NandTestRddBuf[0]=0xFF;
    for(u32_i=1; u32_i<pNandDrv->u16_SpareByteCnt; u32_i++)
        pu8_NandTestRddBuf[u32_i]=u32_i/4;

    for(u32_i=0; u32_i<pNandDrv->u16_BlkPageCnt; u32_i++)
    //for(u32_i=0; u32_i<1; u32_i++)
    {
        u32_err = NC_WritePages((u16_PBA<<pNandDrv->u8_BlkPageCntBits)+u32_i, 
            pu8_NandTestDatBuf, pu8_NandTestRddBuf, 1);
        if(UNFD_ST_SUCCESS != u32_err)
        {
            nand_debug(0,1,"write %Xh blk, %Xh page fail: %Xh \n", 
                u16_PBA, u32_i, u32_err);
            return u32_err;
        }
    }

    u32_err = NC_ReadPages(u16_PBA<<pNandDrv->u8_BlkPageCntBits, 
        pu8_NandTestTmpBuf, pu8_NandTestTmpBuf+pNandDrv->u16_PageByteCnt, 1);
    if(UNFD_ST_SUCCESS != u32_err)
    {
        nand_debug(0,1,"read %Xh blk, %Xh Row fail: %Xh \n", 
            u16_PBA, u16_PBA<<pNandDrv->u8_BlkPageCntBits, u32_err);
        return u32_err;
    }
    
    u32_err = nand_CompareData(pu8_NandTestDatBuf, pu8_NandTestTmpBuf, pNandDrv->u16_PageByteCnt);
    if(UNFD_ST_SUCCESS != u32_err)
    {
        nand_debug(0,1,"c data fail: %Xh blk fail: %Xh \n", 
            u16_PBA, u32_err);
        return u32_err;
    }

    for(u32_i=0; u32_i<pNandDrv->u16_SpareByteCnt/pNandDrv->u8_PlaneCnt; u32_i+=pNandDrv->u16_SectorSpareByteCnt)
    {
        u32_err = nand_CompareData(
            pu8_NandTestRddBuf+u32_i, (pu8_NandTestTmpBuf+pNandDrv->u16_PageByteCnt)+u32_i, 
            pNandDrv->u16_SectorSpareByteCnt-pNandDrv->u16_ECCCodeByteCnt);
        if(UNFD_ST_SUCCESS != u32_err)
        {
            nand_debug(0,1,"c spare fail: %Xh blk, %Xh-th sector fail: %Xh \n", 
                u16_PBA, u32_i/pNandDrv->u16_SectorSpareByteCnt, u32_err);
            return u32_err;            
        }
    }
    nand_debug(0,1,"E/W/R/C OK\n");

    // ===================================
    #if 0
    u32_err = NC_EraseBlk(PBA2<<pNandDrv->u8_BlkPageCntBits);
    if(UNFD_ST_SUCCESS != u32_err)
    {
        nand_debug(0,1,"erase PBA2: %Xh blk fail: %Xh \n", PBA2, u32_err);
        return u32_err;
    }
    
    u32_err = NC_PageCopy(u16_PBA<<pNandDrv->u8_BlkPageCntBits, 
        PBA2<<pNandDrv->u8_BlkPageCntBits,
        pu8_NandTestDatBuf, pu8_NandTestRddBuf, pNandDrv->u16_BlkPageCnt, &u32_tmp);    
    if(UNFD_ST_SUCCESS != u32_err)
    {
        nand_debug(0,1,"NC_PageCopy fail: %Xh \n", u32_err);
        return u32_err;
    }

    for(u32_i=0; u32_i<pNandDrv->u16_BlkPageCnt; u32_i++)
    {
        u32_err = NC_ReadPages((u16_PBA<<pNandDrv->u8_BlkPageCntBits)+u32_i, 
            pu8_NandTestDatBuf, pu8_NandTestDatBuf+pNandDrv->u16_PageByteCnt, 1);
        if(UNFD_ST_SUCCESS != u32_err)
        {
            nand_debug(0,1,"read %Xh blk, %Xh page fail: %Xh \n", 
                u16_PBA, u32_i, u32_err);
            return u32_err;
        }
        u32_err = NC_ReadPages((PBA2<<pNandDrv->u8_BlkPageCntBits)+u32_i, 
            pu8_NandTestTmpBuf, pu8_NandTestTmpBuf+pNandDrv->u16_PageByteCnt, 1);
        if(UNFD_ST_SUCCESS != u32_err)
        {
            nand_debug(0,1,"read %Xh blk, %Xh page fail: %Xh \n", 
                u16_PBA, u32_i, u32_err);
            return u32_err;
        }
        u32_err = nand_CompareData(
            pu8_NandTestDatBuf, pu8_NandTestTmpBuf, 
            pNandDrv->u16_PageByteCnt+pNandDrv->u16_SpareByteCnt/pNandDrv->u8_PlaneCnt);
        if(UNFD_ST_SUCCESS != u32_err)
        {
            nand_debug(0,1,"cmp fail: %Xh %Xh blk, %Xh page fail: %Xh \n", 
                u16_PBA, PBA2, u32_i, u32_err);
            return u32_err;
        }
    }
    
    nand_debug(0,1,"CopyPage OK\n\n");
    #endif
    NC_EraseBlk(u16_PBA<<pNandDrv->u8_BlkPageCntBits);
    NC_EraseBlk(PBA2<<pNandDrv->u8_BlkPageCntBits);
    return u32_err;
}


U32 NC_Test_PwrCut(U16 LoopCnt)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U32 u32_err, u32_i;
    U16 PBA, PBA2, u16_i;

    nand_hw_timer_delay(HW_TIMER_DELAY_1s*3);    
    nand_hw_timer_delay(HW_TIMER_DELAY_100ms*3);    
    // ===================================
    nand_debug(0,0,"\n");
    PBA = HWTimer_End();
    if(0==PBA%3)  PBA=52;
    else if(1==PBA%3)  PBA=50;
    else  
        PBA=48;
    PBA2 = PBA+1;
    nand_debug(0,1,"PBA: %Xh %Xh\n", PBA, PBA2);
    // ===================================
    
    // read to check UECC
    nand_debug(0,0,"r\n");
    for(u32_i=0; u32_i<pNandDrv->u16_BlkPageCnt; u32_i++)
    {
        u32_err=NC_ReadPages((PBA<<pNandDrv->u8_BlkPageCntBits)+u32_i, 
            pu8_NandTestDatBuf, pu8_NandTestDatBuf+pNandDrv->u16_PageByteCnt, 1);
        if(UNFD_ST_SUCCESS != u32_err)
        {
            nand_debug(0,1,"\n");
            nand_debug(0,1,"read %Xh blk, %Xh page fail: %Xh \n", 
                PBA, u32_i, u32_err);
            //while(1);
            return ~UNFD_ST_SUCCESS;
        }

        u32_err=NC_ReadPages((PBA2<<pNandDrv->u8_BlkPageCntBits)+u32_i, 
            pu8_NandTestDatBuf, pu8_NandTestDatBuf+pNandDrv->u16_PageByteCnt, 1);
        if(UNFD_ST_SUCCESS != u32_err)
        {
            nand_debug(0,1,"\n");
            nand_debug(0,1,"read %Xh blk, %Xh page fail: %Xh \n", 
                PBA, u32_i, u32_err);
            //while(1);
            return ~UNFD_ST_SUCCESS;
        }
    }

    for(u16_i=0; u16_i<LoopCnt; u16_i++)
    {
        nand_debug(0,0,"e\n");
        u32_err=NC_EraseBlk2P(PBA<<pNandDrv->u8_BlkPageCntBits);
        if(u32_err)
        {   
            nand_debug(0,1,"\n");
            nand_debug(0,1,"NC_EraseBlk2P fail: %Xh\n", u32_err);
            //while(1);
            return ~UNFD_ST_SUCCESS;
        }

        for(u32_i=0; u32_i<pNandDrv->u16_BlkPageCnt; /*u32_i++*/){
        nand_debug(0,0,"w: %Xh\n", u32_i);
        u32_err=NC_WritePages2PCache(
            (PBA<<pNandDrv->u8_BlkPageCntBits)+u32_i, 
            pNandDrv->pu8_PageDataBuf, pNandDrv->pu8_PageSpareBuf, 
            pNandDrv->u16_BlkPageCnt/4);
        u32_i += pNandDrv->u16_BlkPageCnt/4;
        if(u32_err)
        {   
            nand_debug(0,1,"\n");
            nand_debug(0,1,"NC_WritePages2PCache fail: %Xh\n", u32_err);
            //while(1);
            return ~UNFD_ST_SUCCESS;
        }}

        if(0x3697==LoopCnt)
            u16_i = 0;
    }

    return UNFD_ST_SUCCESS;   
}


U32 NC_Test_PwrCut_Toggle(void)
{
    U32 i=1;
    REG_CLR_BITS_UINT16(NC_SIGNAL, BIT_NC_WP_AUTO|BIT_NC_CE_AUTO|BIT_NC_CE_H);

    while(i!=0)
    {
        REG_SET_BITS_UINT16(NC_SIGNAL, BIT_NC_WP_H);
        nand_hw_timer_delay(HW_TIMER_DELAY_1ms/2);
        REG_CLR_BITS_UINT16(NC_SIGNAL, BIT_NC_WP_H);
        nand_hw_timer_delay(HW_TIMER_DELAY_1ms/2);
        i++;
    }
    return UNFD_ST_SUCCESS;   
}

// ===================================
void NC_Test_CheckAll0xFF(void)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U8 *pu8;
    U32 u32_i, u32_j;

    nand_debug(0, 0, "test drvNAND_CheckAll0xFF \n");

    for(u32_i=0; u32_i<pNandDrv->u16_PageByteCnt; u32_i++)
        pu8_NandTestDatBuf[u32_i] = 0xFF;

    HWTimer_Start();
    for(u32_i=0; u32_i<0x10; u32_i++)
    {
        pu8 = pu8_NandTestDatBuf + u32_i;

        for(u32_j=0; u32_j<0x400; u32_j++)
        {
            pu8[u32_j] = 0;
            if(0xFF == drvNAND_CheckAll0xFF(pu8, 0x400))
            {
                nand_debug(0, 0, "CheckAll0xFF fail: %Xh %Xh \n", u32_i, u32_j);
                return;
            }
            pu8[u32_j] = 0xFF;
        }
        if(0xFF != drvNAND_CheckAll0xFF(pu8, 0x400))
        {
            nand_debug(0, 0, "CheckAll0xFF fail: %Xh %Xh \n", u32_i, u32_j);
            return;
        }
    }
    u32_j = HWTimer_End();
    nand_debug(0, 0, "OK: %u \n", u32_j); 
    return;    
}


U32 MsNAND_Test_Timer(void) // verified ok
{
    U32 u32_i, u32_tmp;

    HWTimer_Start();
    
    for(u32_i=0; u32_i<6; u32_i++)
    {
        u32_tmp = HWTimer_End();
        
        nand_hw_timer_delay(DELAY_1s_in_us);
        
        nand_debug(0,0,"%u \n", HWTimer_End()-u32_tmp); // 12M
    }
    return u32_i;
}


U32 MsNAND_Test(U16 u16_PBA)
{
    U32 u32_err=UNFD_ST_SUCCESS;
    
    //u32_err = NC_Test_Reg5B5C(u16_PBA);
    //NC_Test_Reg717274(u16_PBA);

    //NC_Test_BIST(u16_PBA);

    //NC_Test_APIs(u16_PBA);

    //NC_Test_CheckAll0xFF();
    //MsNAND_Test_Timer();    

    return u32_err;
}


// ============================================
// lifetime test
// ============================================
#define TEST_ErrBit

typedef struct _BLK_ERR_BITS
{
    U16 u16_PBA, u16_Page, u16_ErrBitCnt; 
    
}BLK_ERR_BITS, *P_BLK_ERR_BITS;

#define IF_FORCE_LFSR      0

// --------------------------
// 1. use 6 blocks (start from the argument of the u16_PBA): 
//    - preset 3 PE cycles: 1, 1K, 2K
//    - each PE cycle: 1 blk uses FB, 1 blk uses LP only.
// 2. check ErrBit status:
//    - 6 blocks, print the Max ErrBitCnt, and the PageIdx.
//    - for block of FB, print LP/HP ErrBitCnt
// --------------------------
void NC_Test_ErrBitStatus_R(U16 u16_PBA, U16 u16_MaxPECnt, U32 LoopCnt)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U16 u16_i, u16_p, u16_PECnt, au16_PBA[3][2]; // [1, u16_MaxPECnt/2, u16_MaxPECnt] PE cycles,  [FullBlk, LP only]
    U32 u32_err, u32_i, u32_j, BlkRow, LoopCntOri=LoopCnt;
    BLK_ERR_BITS a_BlkErrBits[3][2][2]; // [1, u16_MaxPECnt/2, u16_MaxPECnt] PE cycles,  [FullBlk, LP only]
    U8  u8_RequireRandomizer = pNandDrv->u8_RequireRandomizer;

    #if IF_FORCE_LFSR  // test with LFSR
    pNandDrv->u8_RequireRandomizer = 1;

    if(pNandDrv->u8_RequireRandomizer)
        NC_EnableLFSR();
    #endif    

    memset(a_BlkErrBits, 0, sizeof(BLK_ERR_BITS)*3*2*2);
    
    u16_i=0;
    while(u16_i < 6)
    {
        #if IF_FORCE_LFSR 
        if(0)//==drvNAND_IsGoodBlk(u16_PBA)) // when LFSR enabled, need BBT.
        #else
        if(0==drvNAND_IsGoodBlk(u16_PBA)) // read 4 pages: 0,1,-2,-1, can not set LFSR.
        #endif
        {
            nand_debug(0, 1, "bad blk: %Xh \n", u16_PBA);
            u16_PBA++;
        }
        else
        {   au16_PBA[u16_i/2][u16_i%2] = u16_PBA;
            u16_PBA++;
            u16_i++;
        }
    }   
    
    nand_debug(0,0," ==============================\n");
    for(u16_i=0; u16_i<6; u16_i++)
    {
        switch(u16_i/2)
        {
            case 0:  u16_PECnt=1;  
                break;
            case 1:  u16_PECnt=u16_MaxPECnt/2;  
                break;
            case 2:  u16_PECnt=u16_MaxPECnt;  
                break;            
        }
        
        if(0==u16_i%2) // FullBlk
        {
            nand_debug(0,0,"PBA %4Xh: %4u PE, FullBlk \n", au16_PBA[u16_i/2][u16_i%2], u16_PECnt);
            
            a_BlkErrBits[u16_i/2][u16_i%2][0].u16_PBA = au16_PBA[u16_i/2][u16_i%2];
            a_BlkErrBits[u16_i/2][u16_i%2][1].u16_PBA = au16_PBA[u16_i/2][u16_i%2];
        }
        else // LP only
        {
            nand_debug(0,0,"PBA %4Xh: %4u PE, LP-Only \n", au16_PBA[u16_i/2][u16_i%2], u16_PECnt);
            
            a_BlkErrBits[u16_i/2][u16_i%2][0].u16_PBA = au16_PBA[u16_i/2][u16_i%2];
        }
    }
    nand_debug(0,0," ==============================\n\n");
        
    // --------------------------
    // reading PBAs
    u32_i=0; u32_j=0;
    while(LoopCnt){
    for(u16_i=0; u16_i<6; u16_i++)
    {
        BlkRow= au16_PBA[u16_i/2][u16_i%2]<<pNandDrv->u8_BlkPageCntBits;
        // reading
        for(u16_p=0; u16_p<pNandDrv->u16_BlkLowPCnt; u16_p++)
        {
            u32_err = NC_ReadPages(
                BlkRow + ga_tPairedPageMap[u16_p].u16_LSB,
                pu8_NandTestDatBuf, pu8_NandTestRddBuf, 1);
            if(u32_err != UNFD_ST_SUCCESS)
            {
                nand_debug(0,1,"read fail: %Xh PBA \n", au16_PBA[u16_i/2][u16_i%2]);
                goto LABEL_END;
            }                
            if(NC_GetECCBits() > a_BlkErrBits[u16_i/2][u16_i%2][0].u16_ErrBitCnt)
            {
                a_BlkErrBits[u16_i/2][u16_i%2][0].u16_ErrBitCnt = NC_GetECCBits();
                a_BlkErrBits[u16_i/2][u16_i%2][0].u16_Page = ga_tPairedPageMap[u16_p].u16_LSB;
            }

            if(ga_tPairedPageMap[u16_p].u16_MSB == 0xFFFF)
                continue;
            
            if(0==u16_i%2) // FullBlk
            {
                u32_err = NC_ReadPages(
                    BlkRow + ga_tPairedPageMap[u16_p].u16_MSB,
                    pu8_NandTestDatBuf, pu8_NandTestRddBuf, 1);
                if(u32_err != UNFD_ST_SUCCESS)
                {
                    nand_debug(0,1,"read fail: %Xh PBA \n", au16_PBA[u16_i/2][u16_i%2]);
                    goto LABEL_END;
                }                
                if(NC_GetECCBits() > a_BlkErrBits[u16_i/2][u16_i%2][1].u16_ErrBitCnt)
                {
                    a_BlkErrBits[u16_i/2][u16_i%2][1].u16_ErrBitCnt = NC_GetECCBits();
                    a_BlkErrBits[u16_i/2][u16_i%2][1].u16_Page = ga_tPairedPageMap[u16_p].u16_MSB;
                }
            }
        }   
        
        // read count        
        if(0 == u32_i%0x400)
        {
            nand_debug(0,0,"---------------------\n");
            nand_debug(0,0,"LoopCnt: %8Xh %8Xh \n", u32_j, u32_i);
            
            for(u16_p=0; u16_p<6; u16_p++)
            {
                nand_debug(0,0,"PBA:%3Xh LP:%3u EBits:%2u \n", 
                    a_BlkErrBits[u16_p/2][u16_p%2][0].u16_PBA, 
                    a_BlkErrBits[u16_p/2][u16_p%2][0].u16_Page,
                    a_BlkErrBits[u16_p/2][u16_p%2][0].u16_ErrBitCnt);
                if(0==u16_p%2)
                    nand_debug(0,0,"PBA:%3Xh HP:%3u EBits:%2u \n", 
                        a_BlkErrBits[u16_p/2][u16_p%2][1].u16_PBA, 
                        a_BlkErrBits[u16_p/2][u16_p%2][1].u16_Page,
                        a_BlkErrBits[u16_p/2][u16_p%2][1].u16_ErrBitCnt);

                nand_debug(0,0,"\n");
            }
        }                    
        if(u32_i++ == 0xFFFFFFFF)
        {u32_j++;  u32_i=0;}
    }
    if(0x3697 != LoopCntOri)
        LoopCnt--;
    }    

    LABEL_END:
    nand_debug(0,0,"LoopCnt: %8Xh %8Xh \n", u32_j, u32_i);
    for(u16_p=0; u16_p<6; u16_p++)
    {
        nand_debug(0,0,"PBA:%3Xh LP:%3u EBits:%2u \n", 
            a_BlkErrBits[u16_p/2][u16_p%2][0].u16_PBA, 
            a_BlkErrBits[u16_p/2][u16_p%2][0].u16_Page,
            a_BlkErrBits[u16_p/2][u16_p%2][0].u16_ErrBitCnt);
        if(0==u16_p%2)
            nand_debug(0,0,"PBA:%3Xh HP:%3u EBits:%2u \n", 
                a_BlkErrBits[u16_p/2][u16_p%2][1].u16_PBA, 
                a_BlkErrBits[u16_p/2][u16_p%2][1].u16_Page,
                a_BlkErrBits[u16_p/2][u16_p%2][1].u16_ErrBitCnt);

        nand_debug(0,0,"\n");
    }
    
    pNandDrv->u8_RequireRandomizer = u8_RequireRandomizer;
    if(0==u8_RequireRandomizer)
        NC_DisableLFSR();
}


void NC_Test_ErrBitStatus_W(U16 u16_PBA, U16 u16_MaxPECnt)
{
    NAND_DRIVER *pNandDrv = (NAND_DRIVER*)drvNAND_get_DrvContext_address();
    U16 u16_i, au16_PBA[3][2]; // [1, u16_MaxPECnt/2, u16_MaxPECnt] PE cycles,  [FullBlk, LP only]
    U16 u16_PECnt, u16_cnt, u16_p;
    U32 u32_err, u32_i, BlkRow;
    U8  u8_RequireRandomizer = pNandDrv->u8_RequireRandomizer;

    for(u32_i=0; u32_i<pNandDrv->u16_PageByteCnt*pNandDrv->u16_BlkPageCnt; u32_i++)
        pu8_NandTestDatBuf[u32_i]=0xFF;
    for(u32_i=0; u32_i<pNandDrv->u16_SpareByteCnt; u32_i++)
        pu8_NandTestRddBuf[u32_i]=0xFF;

    // --------------------------
    LABEL_FIND_PBAs:
    
    //NC_DisableLFSR();
    
    u16_i=0;
    while(u16_i < 6)
    {
        //NC_EraseBlk(BlkRowAddr(u16_PBA));
        
        #if IF_FORCE_LFSR 
        if(0)//==drvNAND_IsGoodBlk(u16_PBA)) // when LFSR enabled, need BBT.
        #else
        if(0==drvNAND_IsGoodBlk(u16_PBA))
        #endif
        {
            nand_debug(0, 1, "bad blk: %Xh \n", u16_PBA);
            u16_PBA++;            
        }
        else
        {   au16_PBA[u16_i/2][u16_i%2] = u16_PBA;
            u16_PBA++;
            u16_i++;
        }
    }

    #if IF_FORCE_LFSR  // test with LFSR
    pNandDrv->u8_RequireRandomizer = 1;

    if(pNandDrv->u8_RequireRandomizer)
        NC_EnableLFSR();
    #else
    //nand_debug(0,1,"suggest to enable IF_FORCE_LFSR, or data pattern is all 0xFF \n");
    #endif    
    
    nand_debug(0,1,"if enable LFSR: %Xh \n", IF_LFSR_ENABLED()?1:0);
    
    // --------------------------
    // setup PBAs
    for(u16_i=0; u16_i<6; u16_i++)
    {
        switch(u16_i/2)
        {
            case 0:  u16_PECnt=1;  
                break;
            case 1:  u16_PECnt=u16_MaxPECnt/2;  
                break;
            case 2:  u16_PECnt=u16_MaxPECnt;  
                break;            
        }

        nand_debug(0,0,"PBA %4Xh: %4u PE\n", 
            au16_PBA[u16_i/2][u16_i%2], u16_PECnt);
        
        for(u16_cnt=0; u16_cnt<u16_PECnt; u16_cnt++)
        {
            BlkRow = au16_PBA[u16_i/2][u16_i%2] << pNandDrv->u8_BlkPageCntBits;
                
            u32_err = NC_EraseBlk(BlkRow);
            if(u32_err != UNFD_ST_SUCCESS)
            {
                drvNAND_MarkBadBlk(au16_PBA[u16_i/2][u16_i%2]);
                nand_debug(0,1,"erase fail: %Xh PBA \n", au16_PBA[u16_i/2][u16_i%2]);
                goto LABEL_FIND_PBAs;
            }

            if(0==u16_i%2) // FullBlk
            for(u16_p=0; u16_p<pNandDrv->u16_BlkPageCnt; u16_p++)
            {   
                u32_err = NC_WritePages(BlkRow + u16_p, 
                    pu8_NandTestDatBuf, pu8_NandTestRddBuf, 1);
                if(u32_err != UNFD_ST_SUCCESS)
                {
                    drvNAND_MarkBadBlk(au16_PBA[u16_i/2][u16_i%2]);
                    nand_debug(0,1,"write fail: PBA.%Xh P.%Xh \n", 
                        au16_PBA[u16_i/2][u16_i%2], u16_p);
                    goto LABEL_FIND_PBAs;
                }
            }
            else // LP-Only
            {
                for(u16_p=0; u16_p<pNandDrv->u16_BlkLowPCnt; u16_p++)
                {   
                    u32_err = NC_WritePages(
                        BlkRow + ga_tPairedPageMap[u16_p].u16_LSB, 
                        pu8_NandTestDatBuf, pu8_NandTestRddBuf, 1);
                    if(u32_err != UNFD_ST_SUCCESS)
                    {
                        drvNAND_MarkBadBlk(au16_PBA[u16_i/2][u16_i%2]);
                        nand_debug(0,1,"write fail: %Xh PBA \n", au16_PBA[u16_i/2][u16_i%2]);
                        goto LABEL_FIND_PBAs;
                    }
                    #if 0
                    if(u16_p < pNandDrv->u16_BlkLowPCnt-1)
                        u32_err=drvNAND_WriteDummyToPages(
                            ga_tPairedPageMap[u16_p].u16_LSB+1, 
                            ga_tPairedPageMap[u16_p+1].u16_LSB-1, BlkRow);                    
                    else
                        u32_err=drvNAND_WriteDummyToPages(
                            ga_tPairedPageMap[u16_p].u16_LSB+1, 
                            pNandDrv->u16_BlkPageCnt-1, BlkRow);
                    if(u32_err != UNFD_ST_SUCCESS)
                    {
                        drvNAND_MarkBadBlk(au16_PBA[u16_i/2][u16_i%2]);
                        nand_debug(0,1,"write fail: %Xh PBA, %Xh \n", 
                            au16_PBA[u16_i/2][u16_i%2], u32_err);
                        goto LABEL_FIND_PBAs;
                    }
                    #endif
                }
            }
        }                   
    }
    
    nand_debug(0,1,"OK: %Xh \n\n", REG(NC_LFSR_CTRL));
    
    pNandDrv->u8_RequireRandomizer = u8_RequireRandomizer;
    if(0==u8_RequireRandomizer)
        NC_DisableLFSR();
    else
        NC_EnableLFSR();
        
    return;
}


// ============================================
// DRAM test
// ============================================

#define DATA_BUS_WIDTH    16  // Data Bus Width, need to set properly

#if (DATA_BUS_WIDTH == 8)
    typedef U8  DATUM;
#elif (DATA_BUS_WIDTH == 16)
    typedef U16  DATUM;
#elif (DATA_BUS_WIDTH == 32)
    typedef U32  DATUM;
#else
    #error "DATA_BUS_WIDTH undefined! \n"
#endif

//retrun:  0: ok,  otherwise: err
static S16 DataBusTest(volatile U32 BaseAddr)
{
    U8  u8_pattern;
    U16 u16_pattern;
    U32 u32_pattern;

    //pattern-bit == 0 -> pattern-bit grounded or 
    //                    shorted with ohter bit-line(s).
    //other bit == 1 -> some data bit-line shorted with Vcc
    //other err condition: 
    //  some data-line shorted with other route(s) (e.g.addr-line)
    
    for(u8_pattern=1; u8_pattern!=0; u8_pattern<<=1)
    {
        *(U8*)BaseAddr = u8_pattern;        
        udelay(1);
        if(*(U8*)BaseAddr != u8_pattern)
        {
            printf("err: 8-bits Data Bus  pattern: %Xh  AddrVal: %Xh \n",
                 u8_pattern, *(U8*)BaseAddr);
            return -1;
        }
    }
    for(u16_pattern=1; u16_pattern!=0; u16_pattern<<=1)
    {
        *(U16*)BaseAddr = u16_pattern;        
        udelay(1);
        if(*(U16*)BaseAddr != u16_pattern)
        {
            printf("err: 16-bits Data Bus  pattern: %Xh  AddrVal: %Xh \n",
                 u16_pattern, *(U16*)BaseAddr);
            return -1;
        }
    }
    for(u32_pattern=1; u32_pattern!=0; u32_pattern<<=1)
    {
        *(U32*)BaseAddr = u32_pattern;        
        udelay(1);
        if(*(U32*)BaseAddr != u32_pattern)
        {
            printf("err: 32-bits Data Bus  pattern: %Xh  AddrVal: %Xh \n",
                 u32_pattern, *(U32*)BaseAddr);
            return -1;
        }
    }
    return 0;
}


#define MEMTEST_ABUS_8BIT     1
#define MEMTEST_ABUS_16BIT    2
#define MEMTEST_ABUS_32BIT    3

//retrun:  0: ok,  otherwise: err
static S16 AddrBusTest(volatile U32 BaseAddr, U32 RamSize)
{
    U32 addrMask, offset1, offset2;
    U32 testData1, testData2;
    U8  BusWidth;

    BusWidth = MEMTEST_ABUS_8BIT;    

    LABEL_TEST_START:

    addrMask = RamSize-1;
    
    switch(BusWidth)
    {
        case  MEMTEST_ABUS_8BIT: 
            testData1=0xAA,
            testData2=0x55;
            
            for(offset1=sizeof(U8); (offset1&addrMask)!=0; offset1<<=1)
                ((U8*)BaseAddr)[offset1] = testData1;

            //see if some addr-line shorted with Vcc
            offset2=0;
            ((U8*)BaseAddr)[offset2] = testData2;
            for(offset1=sizeof(U8); (offset1&addrMask)!=0; offset1<<=1)
            {
                udelay(1);
                if(((U8*)BaseAddr)[offset1] != testData1)
                {
                    printf("err: Addr Bus. err#1: 0x%X 0x%X\n",
                        BaseAddr, offset1);
                    return -1;
                }
            }

            //see if some addr-line gorunded or shorted with lower addr-line
            ((U8*)BaseAddr)[offset2] = testData1;
            for(offset2=sizeof(U8); (offset2&addrMask)!=0; offset2<<=1)
            {
                ((U8*)BaseAddr)[offset2] = testData2;
                
                for(offset1=(offset2<<1); (offset1&addrMask)!=0; offset1<<=1)
                {
                    udelay(1);
                    if(((U8*)BaseAddr)[offset1]!=testData1)
                    {
                        printf("err: Addr Bus. err#2: 0x%X 0x%X\n",
                            BaseAddr, offset1);
                        return -1;
                    }
                }
            }
            break;
        case MEMTEST_ABUS_16BIT: 
            testData1=0xAAAA,
            testData2=0x5555;
            addrMask >>= 1;  

            for(offset1=sizeof(U16); (offset1&addrMask)!=0; offset1<<=1)
                ((U16*)BaseAddr)[offset1] = testData1;

            //see if some addr-line shorted with Vcc
            offset2=0;
            ((U16*)BaseAddr)[offset2] = testData2;
            for(offset1=sizeof(U16); (offset1&addrMask)!=0; offset1<<=1)
            {
                udelay(1);
                if(((U16*)BaseAddr)[offset1] != testData1)
                {
                    printf("err: Addr Bus. err#3: 0x%X 0x%X\n",
                        BaseAddr, offset1);
                    return -1;
                }
            }

            //see if some addr-line gorunded or shorted with lower addr-line
            ((U16*)BaseAddr)[offset2] = testData1;
            for(offset2=sizeof(U16); (offset2&addrMask)!=0; offset2<<=1)
            {
                ((U16*)BaseAddr)[offset2] = testData2;
                
                for(offset1=(offset2<<1); (offset1&addrMask)!=0; offset1<<=1)
                {
                    udelay(1);
                    if(((U16*)BaseAddr)[offset1]!=testData1)
                    {
                        printf("err: Addr Bus. err#4: 0x%X 0x%X\n",
                            BaseAddr, offset1);
                        return -1;
                    }
                }
            }
            break;
            
        case MEMTEST_ABUS_32BIT: 
            testData1=0xAAAAAAAA,
            testData2=0x55555555;
            addrMask >>= 2;
            
            for(offset1=sizeof(U32); (offset1&addrMask)!=0; offset1<<=1)
                ((U32*)BaseAddr)[offset1] = testData1;

            //see if some addr-line shorted with Vcc
            offset2=0;
            ((U32*)BaseAddr)[offset2] = testData2;
            for(offset1=sizeof(U32); (offset1&addrMask)!=0; offset1<<=1)
            {
                udelay(1);
                if(((U32*)BaseAddr)[offset1] != testData1)
                {
                    printf("err: Addr Bus. err#5: 0x%X 0x%X\n",
                        BaseAddr, offset1);
                    return -1;
                }
            }

            //see if some addr-line gorunded or shorted with lower addr-line
            ((U32*)BaseAddr)[offset2] = testData1;
            for(offset2=sizeof(U32); (offset2&addrMask)!=0; offset2<<=1)
            {
                ((U32*)BaseAddr)[offset2] = testData2;
                
                for(offset1=(offset2<<1); (offset1&addrMask)!=0; offset1<<=1)
                {
                    udelay(1);
                    if(((U32*)BaseAddr)[offset1]!=testData1)
                    {
                        printf("err: Addr Bus. err#6: 0x%X 0x%X\n",
                            BaseAddr, offset1);
                        return -1;
                    }
                }
            }
            break;        
    }    
    
    // -----------------------------------------
    switch(BusWidth)
    {
        case  MEMTEST_ABUS_8BIT: 
            BusWidth = MEMTEST_ABUS_16BIT;
            goto LABEL_TEST_START;
            
        case MEMTEST_ABUS_16BIT: 
            BusWidth = MEMTEST_ABUS_32BIT;
            goto LABEL_TEST_START;
            
        case MEMTEST_ABUS_32BIT: 
            break;
    }        
    return 0;
}


//retrun:  0: ok,  otherwise: err
S16 RamTest(volatile U32 BaseAddr, U32 RamSize)
{
    #if 1
    //1. test Data Bus
    if(DataBusTest(BaseAddr) != 0)  
        return -1;
    else
        printf("DataBusTest ok \n");
    #endif

    #if 1
    //2. test Addr Bus
    //size need to be 2^ times
    if(AddrBusTest(BaseAddr, RamSize) != 0)  
        return -2;
    else
        printf("AddrBusTest ok \n");
    #endif

    #if 0
    //3. test Ram Chip 
    memset(&errInfo, 0, sizeof(err_info));
    if(RamChipTest(pBaseAddr, RamSize, &errInfo) != 0)  
    {
        printf("RamChipTest error at 0x%X. \n", errInfo.errChipAddr);
        return -3;
    }
    //else
    //    printf("RamChipTest ok \n");    
    #endif
    
    printf("Test ok - Base: %Xh  Size: %Xh  End: %Xh \n",
            BaseAddr, RamSize, BaseAddr+RamSize);
    return 0;    
}
