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
#include "common.h" // memset
#include <mmc.h>
#include "hal_sdio.h"
//#include "drvFCIE5.h"
#include "DrvSd.h"

#define MAX_CLK		50000
ADMA_INFO g_ADMAInfo __attribute__((aligned(0x80)));

//extern U8 gu8TuningMode;
extern int switch_2_sdr25(struct mmc *mmc);
extern int switch_2_sdr12(struct mmc *mmc);

///////////////////////////////////////////////////////////////////////////////////////////////////


static __inline void fcie_dump_mem_line(unsigned char *buf, U32 cnt)
{
	U32 i;

	printf("  0x%08X: ", (U32)buf);
	for (i= 0; i < cnt; i++)
		printf("%02Xh ", buf[i]);

	printf(" | ");

	for (i = 0; i < cnt; i++)
		printf("%c", (buf[i] >= 32 && buf[i] < 128) ? buf[i] : '.');

	printf("\n");
}

void fcie_dump_mem(unsigned char *buf, U32 cnt)
{
	U32 i;

	for (i= 0; i < cnt; i+= 16)
		fcie_dump_mem_line(buf + i, 16);
}

int fcie_mem_cmp(const void * cs, const void * ct, int count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

E_IO_STS DrvSd_R2N_RW(U32 SectorIndex, void* pBuffer, E_RWDIR RwDir, E_DATA_BUS BusWidth, U32 Clock)
{
    CMD_RSP_INFO CmdRspInfo;
    E_IO_STS IoStatus;

    printf(LIGHT_RED"R2N %s sector %d, %d bits, clock %d\n"NONE, (RwDir==DIR_R)? "R":"W", SectorIndex, (BusWidth==BUS_1_BIT) ? 1 : 4, Clock);

	memset(&CmdRspInfo, 0, sizeof(CMD_RSP_INFO));

    CmdRspInfo.CardClock = Clock;

    CmdRspInfo.Command.Cmd.Index = (RwDir==DIR_R)? 17 : 24;
	CmdRspInfo.Command.Cmd.Arg = U32BEND2LEND(SectorIndex);

    CmdRspInfo.CmdType = CMD_TYPE_ADTC;
	CmdRspInfo.RspType = RSP_TYPE_R1;

	// data transfer related
	CmdRspInfo.ReadWriteDir = RwDir;
    CmdRspInfo.DataPath = PATH_R2N;
	CmdRspInfo.BusWidth = BusWidth;

	CmdRspInfo.MiuAddr = 0; // R2N don't need this field
	CmdRspInfo.pBufferAddr = pBuffer;
    CmdRspInfo.BlockSize = 512;
	CmdRspInfo.BlockCount = 1;
	

	IoStatus = HalSdio_SendCommand(&CmdRspInfo);
    if(IoStatus)
    {
        goto ErrorHandle;
    }

    //gDisk[u8Disk].BurstMode.eSdState = STATE_TRANSFER;

    return IO_SUCCESS;

ErrorHandle:

    printf("R2N %s fail 0x%02X!\r\n", (RwDir==DIR_R)? "read":"write", IoStatus);
    return IoStatus;
}

// [test ]
// 1. pad type:	bypass, SDR
// 2. bus width:	1, 4
// 3. clock:		300K, 25M, 50M
void sd_r2n_read_write_test(struct mmc * mmc)
{
	static unsigned int test_pattern[(512/4)];
	static unsigned int read_pattern[(512/4)];
	U32 j;
	U8 u8Result = 0;
	U8 u8PadTypeBackup;
	//const U8 u8PadType4Test[2] = {, SDIO_MODE_GPIO_PAD_SDR}
	U8 u8PadType;
	E_DATA_BUS BusWidth;
	U32 u32ClockIdx;
	U32 u32Clock4Test[3] = {50000, 25000,300};
	U32 u32ClockBackup;

	for(j=0; j<(512/4); j++)
	{
		//*(pu32_data_pattern+j) = 0xFF00FF00;
		//*(pu32_data_pattern+j) = 0xFF00FF00;
		test_pattern[j]=0xFF00FF00;
		read_pattern[j]=0x00000000;
	}

	// backup
	u8PadTypeBackup = HalSdio_GetPadType();
	u32ClockBackup = mmc->clock;

	///////////////////////////////////////////////////////////////////////////////////////////////

	for(u8PadType=SDIO_MODE_GPIO_PAD_BPS; u8PadType<=SDIO_MODE_GPIO_PAD_SDR; u8PadType++)
	{
		//HalSdio_SwitchPad(u8PadType);
		if(u8PadType==SDIO_MODE_GPIO_PAD_BPS)
		{
			switch_2_sdr12(mmc); // CMD6 switch, normal speed
		}
		else if(SDIO_MODE_GPIO_PAD_SDR)
		{
			switch_2_sdr25(mmc); // CMD6 switch, high speed
		}
		else
		{
			printf("wrong op_mode to test\n");
		}

		for(BusWidth=BUS_1_BIT; BusWidth<=BUS_4_BITS; BusWidth++)
		{
			sd_set_bus_width(mmc, (BusWidth==BUS_1_BIT)? 1: 4); // ACMD6, bus width

			for(u32ClockIdx=0; u32ClockIdx<3; u32ClockIdx++)
			{
				mmc_set_clock(mmc, u32Clock4Test[u32ClockIdx]*1000); // bus width, clock, pad

				HalSdio_SwitchPad(u8PadType); // need to hard code set pad for bypass, normal driver do not use bypass

				for(j=0; j<(512/4); j++)
				{
					read_pattern[j]=0x00000000;
				}

				if(u8PadType==SDIO_MODE_GPIO_PAD_BPS)
				{
					if(u32Clock4Test[u32ClockIdx]>25000)
					{
						continue; // bypass mode better use slow clock
					}
				}

				DrvSd_R2N_RW(0x10000, test_pattern, DIR_W, BusWidth, u32Clock4Test[u32ClockIdx]);
				DrvSd_R2N_RW(0x10000, read_pattern, DIR_R, BusWidth, u32Clock4Test[u32ClockIdx]);

				for(j=0; j<(512/4); j++)
				{
					if (read_pattern[j] != test_pattern[j])
					{
						printf("\n[%d]r:[0x%08x]!=w:[0x%08X]\n", j, read_pattern[j], test_pattern[j]);
						printf("test pattern: \n");
						fcie_dump_mem((unsigned char*)test_pattern, 512);
						printf("\nwhat we read: \n");
						fcie_dump_mem((unsigned char*)read_pattern, 512);
						u8Result = 1;
						break;
					}
				}
				if(u8Result)
				{
					printf(LIGHT_RED"R2N compare fail\n"NONE);
				}
				else
				{
					printf("R2N mode test OK\n\n");
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// restore
	HalSdio_SwitchPad(u8PadTypeBackup);
	mmc_set_clock(mmc, u32ClockBackup);


}

void sd_adma_read_write_test(void)
{
	unsigned char u8_Pattern = 0;

	for(u8_Pattern=0; u8_Pattern<=0xFE; u8_Pattern+=17)
	{
		if( HalSdio_TestAdvanceDMA(u8_Pattern))
		{
			break;
		}
	}
	if(u8_Pattern==0xFF)
	{
		printf("all adma test OK\n");
		return;
	}
	else
	{
		printf("adma test fail\n");
		while(1);
	}
}


void sd_rw_timeout_error_detect(U8 u8ReadWriteDir)
{
	// 0: rdcrc	 -> read crc error
	// 1: wcrc	 -> write crc
	// 2: wto	 -> write time out result by MIU side
	// 3: rspto	 -> response timeout / no response
	// 4: rspcrc -> response crc error
	// 5: rdto	 -> read timeout

	unsigned int u32_err = 0;
	unsigned int u32_arg;
	//U8  u8_retry = 0;
	//U32 u32PhyAddr = 0;
    CMD_RSP_INFO CmdRspInfo;
	//unsigned int u32_dma_addr;
	unsigned int micro_sec = 0;
	unsigned short u16Timer;
	unsigned short u16_i;

	memset(&CmdRspInfo, 0, sizeof(CMD_RSP_INFO));

	//printf("%s(%d)\n", __FUNCTION__, u16Timer);

	u32_arg =  0;
	//u32_dma_addr = HalSdio_TranslateVirtAddr(0x20400000, 0x200);
	CmdRspInfo.Command.Cmd.Arg = U32BEND2LEND(u32_arg);
	//CmdRspInfo.ClockStop = CLK_KEEP;
    CmdRspInfo.RspType = RSP_TYPE_R1;
	CmdRspInfo.DataPath = PATH_DMA;
	CmdRspInfo.BusWidth = BUS_4_BITS;
	CmdRspInfo.BusAddr = 0x20400000;
	CmdRspInfo.pBufferAddr = NULL; // DMA don't need this field
	CmdRspInfo.BlockSize = 512;
	//CmdRspInfo.ReadWriteTimeOut = u16Timer;

	gu8SdioSilenceTuning = TUNE_FINISH;

	// timer unit is 20.83 us when clock is 48MHz
	// 20.83 x 65536 = 1365 us
	// 1000 ms equal to 732 unit, 732 x 1365 = 999,180 us

	if(u8ReadWriteDir==1) // wcrc : write CRC error
	{
		// test CMD25 write timeout case
		// Use CMD24 but give engine 2 job block count
		// IP will write 2 blocks to card but card will not ack write CRC status to host
		// Max NCR = 64 clock cycle, so it will detect error very soon
		// 48MHz --> 20.83 us, 64 x 20.83 us = 1333.12 us = 1.3 ms
		CmdRspInfo.Command.Cmd.Index = 24;
		CmdRspInfo.ReadWriteDir = DIR_W;
		CmdRspInfo.BlockCount = 2;
    	CmdRspInfo.CmdType = CMD_TYPE_ADTC;
		CmdRspInfo.CardClock=MAX_CLK;

		for(u16_i = 0; u16_i < 5; u16_i++)
		{
			//CmdRspInfo.ReadWriteTimeOut = u16Timer;
			HalSdio_TimerStart();
		    u32_err = HalSdio_SendCommand(&CmdRspInfo);
			micro_sec = HalSdio_TimerStop();
			printf("test No.%d take %6d ms\n\n", u16_i, micro_sec/1000);
			if(!u32_err)
			{
				printf(LIGHT_RED"error not happen, test write crc fail!!!\n"NONE);
				break;
			}
		}
	}
	else if(u8ReadWriteDir==2) // wto: write time out result by MIU side, MIU mask
	{
		// test CMD25 write timeout case
		// Set MaskMiuReq to simulate MIU hang up
		// and can not transfer data to FCIE for write case.
		// we should see the larger timmer get longer time to happen timeout
		// test with 1 ~ 5 seconds timeout

		CmdRspInfo.Command.Cmd.Index = 25;
		CmdRspInfo.ReadWriteDir = DIR_W;
		CmdRspInfo.MaskMiuReq = 1;
		CmdRspInfo.BlockCount = 2;
	    CmdRspInfo.CmdType = CMD_TYPE_ADTC;

		// timer unit is 20.83 us when clock is 48MHz
		// 20.83 x 65536 = 1365 us
		// 1000 ms equal to 732 unit, 732 x 1365 = 999,180 us
		for(u16Timer = 0; u16Timer <= 3660; u16Timer+=732)
		{
			CmdRspInfo.ReadWriteTimeOut = u16Timer;
			HalSdio_TimerStart();
		    u32_err = HalSdio_SendCommand(&CmdRspInfo);
			micro_sec = HalSdio_TimerStop();
			printf("timer %d take %6d ms\n\n", u16Timer, micro_sec/1000);
			// the timeout should happen around 0, 1, 2, 3, 4, 5 sec.
			if(!u32_err)
			{
				printf(LIGHT_RED"error not happen, test write to fail!!!\n"NONE);
				DrvSd_CMD12();
				break;
			}

			DrvSd_CMD12();
		}
	}
	else if(u8ReadWriteDir==3) // rspto: response timeout
	{
		// Use wrong RCA in CMD13 to test
		// Card will not reposne to wrong RCA CMD13

		CmdRspInfo.Command.Cmd.Index = 13;
		CmdRspInfo.Command.Cmd.Arg = U32BEND2LEND(0xBEEF0000);
		CmdRspInfo.CardClock=MAX_CLK;

		//CmdRspInfo.ReadWriteDir = DIR_W;
		//CmdRspInfo.BlockCount = 2;
	    CmdRspInfo.CmdType = CMD_TYPE_AC;

		for(u16_i = 0; u16_i < 5; u16_i++)
		{
			HalSdio_TimerStart();
		    u32_err = HalSdio_SendCommand(&CmdRspInfo);
			micro_sec = HalSdio_TimerStop();
			printf("response timeout take %6d ms to detect\n\n", micro_sec/1000);
		}
	}
	else if(u8ReadWriteDir==5) // rdto: read timeout
	{
		// Send CMD17 but set job block count as 2 and DMA length as 0x400
		// This case will be like CMD18 but card send out 1 block and stop sending data out.

		CmdRspInfo.Command.Cmd.Index = 17;
		CmdRspInfo.ReadWriteDir = DIR_R;
		CmdRspInfo.BlockCount = 2;
	    CmdRspInfo.CmdType = CMD_TYPE_ADTC;

		// timer unit is 20.83 us when clock is 48MHz
		// 20.83 x 65536 = 1365 us
		// 1000 ms equal to 732 unit, 732 x 1365 = 999,180 us

		for(u16Timer = 0; u16Timer <= 3660; u16Timer+=732)
		{
			CmdRspInfo.ReadWriteTimeOut = u16Timer;
			HalSdio_TimerStart();
		    u32_err = HalSdio_SendCommand(&CmdRspInfo);
			micro_sec = HalSdio_TimerStop();
			printf("timer %d take %6d ms\n\n", u16Timer, micro_sec/1000);
			// the timeout should happen around 0, 1, 2, 3, 4, 5 sec.

			if(!u32_err)
			{
				printf(LIGHT_RED"error not happen, test read timeout fail!!!\n"NONE);
				break;
			}
		}
	}
	else
	{
		printf("wrong parameter of %s()\n", __FUNCTION__);
	}

	gu8SdioSilenceTuning = TUNE_FINISH;

}

#define TEST_BUFFER_SIZE    0x40000 // 256KB
U8 W_BUFFER[TEST_BUFFER_SIZE] __attribute__((aligned(0x80)));
U8 R_BUFFER[TEST_BUFFER_SIZE] __attribute__((aligned(0x80)));

// W_BUFFER is 1MB
// R_BUFFER is 1MB
// each 1MB read write buffer divide to 256KB x 4
// 256KB = 512 sectors
// setup different descriptor to test ADMA
//

#define AMDA_QUARTER_SIZE (TEST_BUFFER_SIZE>>2)
#define BLOCK_ADDR_4_TEST 0000

#define DEBUG_SG_ELEMENT 0

static U32 mstar_mci_prepare_descriptors(struct kernel_like_mmc_data * pData)
{
	U32 i, /*u32DescptAddr,*/ u32BusAddr, u32DeviceAddr, u32DmaLeng, u32TotalLength = 0;
	struct kernel_like_scatterlist * pScatterList = NULL;

	//printk("descriptor addr %08Xh", g_ADMAInfo.DescriptorAddr);
	//u32DescptAddr = virt_to_phys((void *)g_ADMAInfo.DescriptorAddr); // virtual to bus address
	//printk(" -- virt_to_phys --> %08Xh\n", u32DescptAddr);

	// setup descriptor
	memset(&g_ADMAInfo, 0, sizeof(ADMA_INFO)-4); // clear

	#if defined(DEBUG_SG_ELEMENT) && DEBUG_SG_ELEMENT
	printk("SG[%d] = ", pData->sg_len);
	#endif

	for(i=0; i<pData->sg_len; i++) {

		pScatterList = &(pData->sg[i]);
		//u32BusAddr = sg_dma_address(pScatterList);
		//u32DmaLeng = sg_dma_len(pScatterList);

		u32BusAddr = pScatterList->u32_dma_addr;
		u32DmaLeng = pScatterList->u32_length;

		flush_cache(u32BusAddr, u32DmaLeng);

		//BUG_ON((u32DmaLeng>>9)&0xFFFFF000);
		u32TotalLength += u32DmaLeng;
		#if defined(DEBUG_SG_ELEMENT) && DEBUG_SG_ELEMENT
		printk("%d + ", u32DmaLeng>>9);
		#endif
		printf("SG[%d] %2d sector, BA = %08Xh", i, u32DmaLeng>>9, u32BusAddr);
		#ifndef FPGA_BOARD
		if( u32BusAddr >= CONFIG_MIU1_BUSADDR) {
			u32DeviceAddr = u32BusAddr - CONFIG_MIU1_BUSADDR;
			g_ADMAInfo.Descriptor[i].adma_miu_sel = 1;
			printf(" - %08Xh(MIU1) = %08Xh\n", CONFIG_MIU1_BUSADDR , u32DeviceAddr);
		} else {
			u32DeviceAddr = u32BusAddr - CONFIG_MIU0_BUSADDR;
			printf(" - %08Xh(MIU0) = %08Xh\n", CONFIG_MIU0_BUSADDR , u32DeviceAddr);
		}
		#else
			u32DeviceAddr = u32BusAddr;
			printf(" - %08Xh(MIU0) = %08Xh\n", CONFIG_MIU0_BUSADDR , u32DeviceAddr);
		#endif
		g_ADMAInfo.Descriptor[i].adma_miu_addr = u32DeviceAddr;
		g_ADMAInfo.Descriptor[i].adma_job_cnt = u32DmaLeng>>9;
		g_ADMAInfo.Descriptor[i].adma_length = u32DmaLeng;

	}

	g_ADMAInfo.Descriptor[pData->sg_len-1].adma_end_flag = 1; // must give a end mark

	#if defined(DEBUG_SG_ELEMENT) && DEBUG_SG_ELEMENT
	printk("= %d\n", u32TotalLength>>9);
	#endif

	//MsOS_Dcache_Flush(g_ADMAInfo.Descriptor, sizeof(ADMA_INFO));

	//Chip_Clean_Cache_Range_VA_PA(g_ADMAInfo.DescriptorAddr, u32DescptAddr, sizeof(ADMA_INFO));

	//eMMC_dump_mem((U8 *)&g_ADMAInfo, 32); // debug use

	return u32TotalLength;
}


// if there is only 1 descriptor, only need to setup once
// need to think about share IP case, setup each time for safe if share IP

static void	mstar_mci_setup_descriptor_addr(CMD_RSP_INFO * pCmdRspInfo)
{
	//U32	u32DescptAddr;

	// give descriptor array addr to SDIO
	//u32DescptMiuAddr = eMMC_translate_DMA_address_Ex(g_ADMAInfo.DescriptorAddr, sizeof(ADMA_INFO));
	//printf("mstar_mci_setup_descriptor_addr1 --> %08X\n", g_ADMAInfo.DescriptorBusAddr);
	// kernel
	//u32DescptAddr = virt_to_phys((void *)g_ADMAInfo.DescriptorAddr); // virtual to bus address
	// bus addr = virtual addr in mboot

	//u32DescptAddr = g_ADMAInfo.DescriptorBusAddr;

	flush_cache(g_ADMAInfo.DescriptorBusAddr, sizeof(g_ADMAInfo));

	//printf("mstar_mci_setup_descriptor_addr2 --> %08X\n", u32DescptAddr);
	//printf("Descpt Addr: %08Xh --> %08Xh", g_ADMAInfo.DescriptorAddr, u32DescptMiuAddr);
	// bus address to device address
	/*#ifndef FPGA_BOARD
	if( u32DescptAddr >= CONFIG_MIU1_BUSADDR) {
		u32DescptAddr -= CONFIG_MIU1_BUSADDR;
		//FCIE_RIU_16_ON(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		//printk(" - %08Xh(MIU1) = %08Xh\n", MSTAR_MIU1_BUS_BASE , u32DescptMiuAddr);
	} else {
		u32DescptAddr -= CONFIG_MIU0_BUSADDR;
		//FCIE_RIU_16_OF(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		//printk(" - %08Xh(MIU0) = %08Xh\n", MSTAR_MIU0_BUS_BASE , u32DescptMiuAddr);
	}
	#endif*/
	//printf("mstar_mci_setup_descriptor_addr3 --> %08X\n", u32DescptAddr);

	pCmdRspInfo->DataPath = PATH_ADMA;
	pCmdRspInfo->BusAddr = g_ADMAInfo.DescriptorBusAddr;
	pCmdRspInfo->pBufferAddr = NULL; // DMA don't need this field
	pCmdRspInfo->BlockSize = 512;
	pCmdRspInfo->BlockCount = 1;

}


///////////////////////////////////////////////////////////////////////////////////////////////////


U32 DrvSd_CMD18_ADMA(U32 u32_eMMCBlkAddr, struct kernel_like_mmc_data * pData)
{
	U32 u32_err = 0;
    CMD_RSP_INFO CmdRspInfo;
	//U32 u32_dma_addr;

	memset(&CmdRspInfo, 0, sizeof(CMD_RSP_INFO));

	CmdRspInfo.Command.Cmd.Index = 18;
	CmdRspInfo.Command.Cmd.Arg = U32BEND2LEND(u32_eMMCBlkAddr);
    CmdRspInfo.CmdType = CMD_TYPE_ADTC;
    CmdRspInfo.RspType = RSP_TYPE_R1;
	CmdRspInfo.ReadWriteDir = DIR_R;
	CmdRspInfo.BusWidth = BUS_4_BITS;
	CmdRspInfo.CardClock=MAX_CLK;

	// setup below in mstar_mci_setup_descriptor_addr()
	#if 0
	CmdRspInfo.DataPath = PATH_ADMA;
	CmdRspInfo.MiuAddr = u32_dma_addr;
	CmdRspInfo.pBufferAddr = NULL; // DMA don't need this field
	CmdRspInfo.BlockSize = 512;
	CmdRspInfo.BlockCount = 1;
	#endif

	mstar_mci_prepare_descriptors(pData);
	mstar_mci_setup_descriptor_addr(&CmdRspInfo);

    u32_err = HalSdio_SendCommand(&CmdRspInfo);
	if(u32_err)
	{
		goto ErrorHandle;
	}

	DrvSd_CMD12();

	return 0;

ErrorHandle:

	return u32_err;
}

U32 DrvSd_CMD25_ADMA(U32 u32_eMMCBlkAddr, struct kernel_like_mmc_data * pData)
{
	U32 u32_err = 0;
    CMD_RSP_INFO CmdRspInfo;

	//U32 u32_dma_addr;
	memset(&CmdRspInfo, 0, sizeof(CMD_RSP_INFO));

	CmdRspInfo.Command.Cmd.Index = 25;
	CmdRspInfo.Command.Cmd.Arg = U32BEND2LEND(u32_eMMCBlkAddr);
    CmdRspInfo.CmdType = CMD_TYPE_ADTC;
    CmdRspInfo.RspType = RSP_TYPE_R1;
	CmdRspInfo.ReadWriteDir = DIR_W;
	CmdRspInfo.BusWidth = BUS_4_BITS;
	CmdRspInfo.CardClock=MAX_CLK;

	// setup below in mstar_mci_setup_descriptor_addr()
	#if 0
	CmdRspInfo.DataPath = PATH_ADMA;
	CmdRspInfo.MiuAddr = u32_dma_addr;
	CmdRspInfo.pBufferAddr = NULL; // DMA don't need this field
	CmdRspInfo.BlockSize = 512;
	CmdRspInfo.BlockCount = 1;
	#endif
	mstar_mci_prepare_descriptors(pData);

	//fcie_dump_mem(g_ADMAInfo.DescriptorBusAddr, 0x200);

	mstar_mci_setup_descriptor_addr(&CmdRspInfo);

    u32_err = HalSdio_SendCommand(&CmdRspInfo);
	if(u32_err)
	{
		goto ErrorHandle;
	}

	DrvSd_CMD12();

	return 0;

ErrorHandle:

	return u32_err;
}






U8 HalSdio_TestAdvanceDMA(U8 u8Pattern)
{
	struct kernel_like_mmc_data mmc_data_w, mmc_data_r;
	struct kernel_like_scatterlist sg_w[4], sg_r[4];
	U8 u8_i;

	printf("HalSdio_TestAdvanceDMA(%02Xh)\n", u8Pattern);

	g_ADMAInfo.DescriptorBusAddr = (U32)g_ADMAInfo.Descriptor; // init

	memset(W_BUFFER, 0x00, TEST_BUFFER_SIZE);
	memset(R_BUFFER, 0x00, TEST_BUFFER_SIZE);

	memset(W_BUFFER,						u8Pattern, 0x200 * 10);
	memset(W_BUFFER+AMDA_QUARTER_SIZE,		u8Pattern, 0x200 *  5);
	memset(W_BUFFER+AMDA_QUARTER_SIZE*2,	u8Pattern, 0x200 *  3);
	memset(W_BUFFER+AMDA_QUARTER_SIZE*3,	u8Pattern, 0x200 *  1);

	//fcie_dump_mem(g_ADMAInfo.DescriptorBusAddr, 0x200);

	///////////////////////////////////////////////////////////////////////////////////////////
	// write SG setup
	mmc_data_w.sg_len = 4;
	mmc_data_w.sg = sg_w;
	// 1st SG
	sg_w[0].u32_dma_addr = ((U32)W_BUFFER);
	sg_w[0].u32_length = 0x200 * 10; // 10 sectors
	// 2nd SG
	sg_w[1].u32_dma_addr = ((U32)W_BUFFER)+AMDA_QUARTER_SIZE;
	sg_w[1].u32_length = 0x200 * 5; // 5 sectors
	// 3rd SG
	sg_w[2].u32_dma_addr = ((U32)W_BUFFER)+AMDA_QUARTER_SIZE*2;
	sg_w[2].u32_length = 0x200 * 3; // 3 sectors
	// 4th SG
	sg_w[3].u32_dma_addr = ((U32)W_BUFFER)+AMDA_QUARTER_SIZE*3;
	sg_w[3].u32_length = 0x200 * 1; // 1 sector

	for(u8_i=0; u8_i<4; u8_i++){
		printf("W SG[%d]: 0x%08X\n", u8_i, sg_w[u8_i].u32_dma_addr);
	}

	DrvSd_CMD25_ADMA(BLOCK_ADDR_4_TEST, &mmc_data_w);

	///////////////////////////////////////////////////////////////////////////////////////////
	// read SG setup
	mmc_data_r.sg_len = 4;
	mmc_data_r.sg = sg_r;
	// 1st SG
	sg_r[0].u32_dma_addr = ((U32)R_BUFFER);
	sg_r[0].u32_length = 0x200 * 10; // 10 sectors
	// 2nd SG
	sg_r[1].u32_dma_addr = ((U32)R_BUFFER)+AMDA_QUARTER_SIZE;
	sg_r[1].u32_length = 0x200 * 5; // 5 sectors
	// 3rd SG
	sg_r[2].u32_dma_addr = ((U32)R_BUFFER)+AMDA_QUARTER_SIZE*2;
	sg_r[2].u32_length = 0x200 * 3; // 3 sectors
	// 4th SG
	sg_r[3].u32_dma_addr = ((U32)R_BUFFER)+AMDA_QUARTER_SIZE*3;
	sg_r[3].u32_length = 0x200 * 1; // 1 sector

	for(u8_i=0; u8_i<4; u8_i++){
		printf("R SG[%d]: 0x%08X\n", u8_i, sg_r[u8_i].u32_dma_addr);
	}

	DrvSd_CMD18_ADMA(BLOCK_ADDR_4_TEST, &mmc_data_r);

	if(memcmp(W_BUFFER, R_BUFFER, TEST_BUFFER_SIZE))
	{
		printf("ADMA test fail, compare fail\n");
		return 1;
	}
	else
	{
		printf("ADMA test OK\n");
		return 0;
	}

}

#if (defined SDIO_D1_INTR_VER) || (SDIO_D1_INTR_VER==2)

// SDR mode need to enable clock at 0x0B bit 0, bypass mode no need

#define EN_CLK_AFT_CMD()							\
	do {											\
		FCIE_RIU_16_ON(FCIE_SD_MODE, BIT_CLK_EN);	\
	} while(0);

U8 HalSdio_TestDataOneIntrEx(U8 u8TestCase)
{
	U16 u16_Reg = 0;
	U32 u32_i = 0;

	printf("\n%s(%d)\n", __FUNCTION__, u8TestCase);
	printf("short SDIO D1 pin to GND\n");

	FCIE_RIU_16_OF(FCIE_SDIO_MOD, BIT_REG_SDIO_MOD_MASK);

	// SDR mode need this...
	// FCIE_RIU_16_ON(FCIE_SD_MODE, BIT_CLK_EN);
	// If SendCommand() turn off clock after finish command request, you need to turn manually

	FCIE_RIU_16_ON(FCIE_SDIO_MOD, BIT_SDIO_INT_SW_MODE);
	FCIE_RIU_16_ON(FCIE_SDIO_MOD, BIT_SDIO_DET_INT_SRC);

	switch(u8TestCase)
	{
		case 0:	// 0b00: Continuous interrupt detect
			FCIE_RIU_16_ON(SDIO_INTR_DET, BIT_SDIO_DET_ON); // enable SDIO interrupt
			EN_CLK_AFT_CMD();
			break;

		case 1: // CMD18(3) + CMD12(1)
			FCIE_RIU_16_ON(FCIE_SDIO_MOD, SDIO_INT_MOD_3); // set to mode 3
			FCIE_RIU_16_ON(SDIO_INTR_DET, BIT_SDIO_DET_ON); // enable SDIO interrupt
			FCIE_RIU_16_ON(FCIE_SDIO_MOD, BIT_INT_STOP_DMA); //
			DrvSd_CMD18(0x10000, 10, (U8*)0x20400000);
			//FCIE_RIU_16_ON(FCIE_SD_MODE, BIT_CLK_EN); // SDR mode need this...
			FCIE_RIU_16_OF(FCIE_SDIO_MOD, BIT01); // become mode 1
			DrvSd_CMD12();
			EN_CLK_AFT_CMD();
			break;

		case 2: // CMD25(3) + CMD12(1)
			FCIE_RIU_16_ON(FCIE_SDIO_MOD, SDIO_INT_MOD_3);
			FCIE_RIU_16_ON(SDIO_INTR_DET, BIT_SDIO_DET_ON); // enable SDIO interrupt
			FCIE_RIU_16_ON(FCIE_SDIO_MOD, BIT_INT_STOP_DMA); //
			DrvSd_CMD25(0x10000, 10, (U8*)0x20400000);
			FCIE_RIU_16_OF(FCIE_SDIO_MOD, BIT01); // become mode 1
			//FCIE_RIU_16_ON(FCIE_SDIO_MOD, SDIO_INT_MOD_1);
			DrvSd_CMD12();
			EN_CLK_AFT_CMD();
			break;

		case 3: // CMD17(2) Single block read/write interrupt detect
			FCIE_RIU_16_ON(FCIE_SDIO_MOD, SDIO_INT_MOD_2);
			FCIE_RIU_16_ON(SDIO_INTR_DET, BIT_SDIO_DET_ON); // enable SDIO interrupt
			DrvSd_CMD17(0x10000, (U8*)0x20400000);
			EN_CLK_AFT_CMD();
			break;

		case 4: // CMD24(2) Single block read/write interrupt detect
			FCIE_RIU_16_ON(FCIE_SDIO_MOD, SDIO_INT_MOD_2);
			FCIE_RIU_16_ON(SDIO_INTR_DET, BIT_SDIO_DET_ON); // enable SDIO interrupt
			DrvSd_CMD24(0x10000, (U8*)0x20400000);
			EN_CLK_AFT_CMD();
			break;

	}

	if(FCIE_RIU_R16(FCIE_MIE_EVENT)&BIT_SDIO_INT)
	{
		printf(LIGHT_RED"Should not happen before D1 touch ground\n"NONE);
		while(1);
	}

	while(1) {

		u16_Reg = FCIE_RIU_R16(FCIE_MIE_EVENT);

    	if( u16_Reg & BIT_SDIO_INT )
    	{
			FCIE_RIU_16_ON(FCIE_MIE_EVENT, BIT_SDIO_INT); // W1C
		    printf("SDIO interrupt detected!\n");
			printf("remove D1 short in 3 sec\n");
			HalSdioDelayMs(1000);
			printf("3\n");
			HalSdioDelayMs(1000);
			printf("2\n");
			HalSdioDelayMs(1000);
			printf("1\n");
			break;
		}

		HalSdioDelayMs(1);
		u32_i++;

		if(u32_i%1000==999) {
			printf("%d s\n", (u32_i/1000)+1);
		}

		if(u32_i==10000) {
    	    printf(LIGHT_RED"test data 1 interrupt 10 sec timeout\n"NONE);
			return 1;
		}

	}

	return 0;

}

// check if any false alarm interrupt happen
void HalSdio_TestDataOneIntFalseAlarm(void)
{
	U32 u32_i = 0;

	FCIE_RIU_16_OF(FCIE_SDIO_MOD, BIT_REG_SDIO_MOD_MASK);

	//FCIE_RIU_16_ON(FCIE_SD_MODE, BIT_CLK_EN); // SDR mode need this...

	FCIE_RIU_16_ON(FCIE_SDIO_MOD, BIT_SDIO_INT_SW_MODE);
	FCIE_RIU_16_ON(FCIE_SDIO_MOD, BIT_SDIO_DET_INT_SRC);

	printf("Start test D1 interrupt false alram\n");

	for(u32_i = 1; u32_i<300; u32_i++)
	{
		printf("loop: %3d\r", u32_i);
		// CMD18(3) + CMD12(1)
		FCIE_RIU_16_ON(FCIE_SDIO_MOD, SDIO_INT_MOD_3);
		FCIE_RIU_16_ON(SDIO_INTR_DET, BIT_SDIO_DET_ON); // enable SDIO interrupt
		FCIE_RIU_16_ON(FCIE_SDIO_MOD, BIT_INT_STOP_DMA); //
		DrvSd_CMD18(0x10000, u32_i, (U8*)0x20400000);
		FCIE_RIU_16_OF(FCIE_SDIO_MOD, BIT01); // become mode 1
		DrvSd_CMD12();

		// CMD25(3) + CMD12(1)
		FCIE_RIU_16_ON(FCIE_SDIO_MOD, SDIO_INT_MOD_3);
		FCIE_RIU_16_ON(SDIO_INTR_DET, BIT_SDIO_DET_ON); // enable SDIO interrupt
		FCIE_RIU_16_ON(FCIE_SDIO_MOD, BIT_INT_STOP_DMA); //
		DrvSd_CMD25(0x10000, u32_i, (U8*)0x20400000);
		FCIE_RIU_16_OF(FCIE_SDIO_MOD, BIT01); // become mode 1
		DrvSd_CMD12();

		// CMD17(2) Single block read/write interrupt detect
		FCIE_RIU_16_ON(FCIE_SDIO_MOD, SDIO_INT_MOD_2);
		FCIE_RIU_16_ON(SDIO_INTR_DET, BIT_SDIO_DET_ON); // enable SDIO interrupt
		DrvSd_CMD17(0x10000, (U8*)0x20400000);

		// CMD24(2) Single block read/write interrupt detect
		FCIE_RIU_16_ON(FCIE_SDIO_MOD, SDIO_INT_MOD_2);
		FCIE_RIU_16_ON(SDIO_INTR_DET, BIT_SDIO_DET_ON); // enable SDIO interrupt
		DrvSd_CMD24(0x10000, (U8*)0x20400000);

		if(FCIE_RIU_R16(FCIE_MIE_EVENT)&BIT_SDIO_INT)
		{
			printf(LIGHT_RED"Should not happen D1 interrupt here!\n"NONE);
			while(1);
		}

	}

	printf("D1 interrupt false alram check PASS!\n");

}

// Note! test data one interrupt can not use level shift board in FPGA enviroment

void HalSdio_TestDataOneIntr(void)
{
	U8 u8TestCase = 0;

	for(u8TestCase=0 ; u8TestCase<5; u8TestCase++)
	{
		if(HalSdio_TestDataOneIntrEx(u8TestCase))
			break;
	}

	HalSdio_TestDataOneIntFalseAlarm();
}


#else

void HalSdio_TestDataOneIntr(void)
{
	U16 u16_Reg = 0;
	U32 u32_i = 0;

	FCIE_RIU_16_OF(FCIE_SDIO_MOD, BIT_REG_SDIO_MOD_MASK);

	// 0b00: Continuous interrupt detect

	// 0b01: CMD12 ot IO abort command interrupt detect
	//FCIE_RIU_16_ON(FCIE_SDIO_MOD, BIT00);

	// 0b10: Single block read/write interrupt detect
	//FCIE_RIU_16_ON(FCIE_SDIO_MOD, BIT01);

	// 0b11: multi-block read/write interrupt detect
	//FCIE_RIU_16_ON(FCIE_SDIO_MOD, BIT01|BIT00);

	FCIE_RIU_16_ON(FCIE_SDIO_MOD, BIT_SDIO_DET_ON); // enable SDIO interrupt

	FCIE_RIU_16_OF(FCIE_SDIO_MOD, BIT_SDIO_DET_INT_SRC);

	while(1) {

		u16_Reg = FCIE_RIU_R16(FCIE_MIE_EVENT);

    	if( u16_Reg & BIT_SDIO_INT )
    	{
		    printf("SDIO interrupt detected\n");
			break;
		}

		udelay(1000); // 1 ms
		u32_i++;

		if(u32_i%1000==999) {
			printf("%d s\n", (u32_i/1000)+1);
		}

		if(u32_i==10000) {
    	    printf("test data 1 interrupt 10 sec timeout\n");
			break;
		}

	}

}

#endif

U32 HalSdio_PowerSaveMode(void)
{
    //U32 u32_err = 0;
	U32 u32_i = 0;
	U16 u16_Reg = 0;

    // Enable power save test by RIU mode
    //FCIE_RIU_W16(FCIE_PWR_SAVE_CTL, 0x000D);

    /* (1) Clear HW Enable */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x00), 0x0000);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x01),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x07);

	/* (2) Clear All Interrupt */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x02), 0xffff);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x03),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x00);

	/* (3) Clear SD MODE Enable */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x04), 0x0000);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x05),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0B);

	/* (4) Clear SD CTL Enable */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x06), 0x0000);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x07),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0C);

	/* (5) Reset Start */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x08), 0x0000);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x09),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x3F);

	/* (6) Reset End */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x0A), 0x0001);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x0B),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x3F);

	/* (7) Set "SD_MOD" */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x0C), 0x0021);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x0D),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0B);

	/* (8) Enable "reg_sdio_en" */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x0E), 0x0004);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x0F),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x07);

	/* (9) Command Content, IDLE */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x10), 0x0040);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x11),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x20);

	/* (10) Command Content, STOP */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x12), 0x0000);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x13),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x21);

	/* (11) Command Content, STOP */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x14), 0x0000);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x15),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x22);

	/* (12) Command & Response Size */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x16), 0x0500);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x17),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0A);

	/* (13) Enable Interrupt, SD_CMD_END */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x18), 0x0002);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x19),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x01);

	/* (14) Command Enable + job Start */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x1A), 0x0044);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x1B),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x0C);

	/* (15) Wait Interrupt */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x1C), 0x0000);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x1D),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WINT);

	/* (16) Clear Interrupt */
	//FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x1E), 0x0002);
	//FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x1F),
    //           PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x00);

    /* (17) Clear HW Enable */
	//FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x20), 0x0000);
	//FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x21),
    //           PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_WREG | PWR_CMD_BK0 | 0x07);

	/* (18) STOP */
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x1E), 0x0000);
	FCIE_RIU_W16(GET_REG_ADDR(FCIE_POWER_SAVE_MODE_BASE, 0x1F),
               PWR_BAT_CLASS | PWR_RST_CLASS | PWR_CMD_STOP);

    printf("1s\n");	udelay(1000000);
    printf("1s\n");	udelay(1000000);
    printf("1s\n");	udelay(1000000);
    printf("1s\n");	udelay(1000000);
    printf("1s\n");	udelay(1000000);
    printf("1s\n");	udelay(1000000);
    printf("1s\n");	udelay(1000000);
    printf("1s\n");	udelay(1000000);
    printf("1s\n");	udelay(1000000);
    printf("1s\n");	udelay(1000000);

    //while(1);

    // Trigger power save mode
    FCIE_RIU_W16(FCIE_PWR_SAVE_CTL, 0x000F);

	while(1){

		u16_Reg = FCIE_RIU_R16(FCIE_PWR_SAVE_CTL);
		printf("FCIE_PWR_SAVE_CTL = %04Xh\n", u16_Reg);

    	if( u16_Reg & (BIT_POWER_SAVE_MODE_INT|BIT_RIU_SAVE_EVENT) )
    	{
		    printf("Power saving mode ok\n");
			break;
		}
		udelay(1000);
		u32_i++;
		if(u32_i==1000) {
    	    printf("Power saving mode fail\n");
			while(1);
			break;
		}
	}

    return 0;
}

// Use BIST pattern for check CRC value convininetly

static const unsigned short bist_pattern_crc_value[12][8] = {

	{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}, // BIST pattern  0: 0x00000000
	{0x0F0F, 0x0F00, 0x0F0F, 0x000F, 0x0F00, 0x0F00, 0x0F00, 0x000F}, // BIST pattern  1: 0xFFFFFFFF
	{0x0000, 0x0F0F, 0x000F, 0x0F00, 0x0F0F, 0x000F, 0x0F0F, 0x000F}, // BIST pattern  2: 0xFF00FF00
	{0x0F0F, 0x000F, 0x0F00, 0x0F0F, 0x000F, 0x0F0F, 0x000F, 0x0000}, // BIST pattern  3: 0x00FF00FF
	{0x0505, 0x0500, 0x0505, 0x0005, 0x0500, 0x0500, 0x0500, 0x0005}, // BIST pattern  4: 0x55555555
	{0x0A0A, 0x0A00, 0x0A0A, 0x000A, 0x0A00, 0x0A00, 0x0A00, 0x000A}, // BIST pattern  5: 0xAAAAAAAA
	{0x0505, 0x0A0F, 0x050A, 0x0F05, 0x0A0F, 0x050F, 0x0A0F, 0x000A}, // BIST pattern  6: 0xAA55AA55
	{0x0A0A, 0x050F, 0x0A05, 0x0F0A, 0x050F, 0x0A0F, 0x050F, 0x0005}, // BIST pattern  7: 0x55AA55AA
	{0x050A, 0x0A00, 0x0A0A, 0x000A, 0x0A00, 0x0A0F, 0x0A00, 0x0F05}, // BIST pattern  8: 0xA55AA55A
	{0x0A05, 0x0500, 0x0505, 0x0005, 0x0500, 0x050F, 0x0500, 0x0F0A}, // BIST pattern  9: 0x5AA55AA5
	{0x0209, 0x0B0C, 0x0300, 0x090A, 0x050A, 0x070E, 0x020E, 0x090F}, // BIST pattern 10: incresing
	{0x0D06, 0x040C, 0x0C0F, 0x0905, 0x0A0A, 0x080E, 0x0D0E, 0x0900}  // BIST pattern 11: decreasing
};


void sd_bist_test_n_check_CRC_value(void)
{
	unsigned char u8_i = 0, u8_j = 0;

 	for(u8_i=0; u8_i<12; u8_i++)
	{
		//printf("hit any to continue\n"); getchar();
		HalSdio_BistTest(u8_i);

		for(u8_j=0; u8_j<8; u8_j++)
		{
			if(HalSdio_ReadCRCFifo(u8_j)!=bist_pattern_crc_value[u8_i][u8_j])
			{
				printf(LIGHT_RED"CRC value compare fail, bist_pattern_crc_value[%d][%d] ", u8_i, u8_j);
				printf("%04Xh != %04Xh\n"NONE, HalSdio_ReadCRCFifo(u8_j), bist_pattern_crc_value[u8_i][u8_j]);
				//if(u8_j%8==7) printf("\n");
				while(1);
				break;
			}
		}
	}
	printf("\nbist_pattern_crc_value compare OK!\n");
}
// 0~11 pattern select

static char *gStrPatternSel[] = {	" 0: 0x00000000",
									" 1: 0xFFFFFFFF",
									" 2: 0xFF00FF00",
									" 3: 0x00FF00FF",
									" 4: 0x55555555",
									" 5: 0xAAAAAAAA",
									" 6: 0xAA55AA55",
									" 7: 0x55AA55AA",
									" 8: 0xA55AA55A",
									" 9: 0x5AA55AA5",
									"10: incresing ",
									"11: decreasing" };

U32 HalSdio_BistTest(U8 u8_PatSel)
{
    U32 u32_err = 0;

    printf("BIST pattern %s:\n", gStrPatternSel[u8_PatSel]);

    // Select pattern
    FCIE_RIU_16_OF(FCIE_BIST, 0xF<<12);
    FCIE_RIU_16_ON(FCIE_BIST, (u8_PatSel & 0xF)<<12);

    // Set BIST Mode
    FCIE_RIU_16_ON(FCIE_TEST_MODE, BIT_BIST_MODE);

	//printf("hit any key to continue\n"); getchar();

	DrvSd_CMD24(0x10000, (U8*)0x20400000);

	//printf("hit any key to continue\n"); getchar();

	DrvSd_CMD17(0x10000, (U8*)0x20400000);

    // Wait BIST Error Flag
    if( FCIE_RIU_R16(FCIE_BIST) & BIT00 )
    {
        printf("\033[7;31m%s: BIST pattern type %d fail!!\033[m\n", __FUNCTION__, u8_PatSel);
        while(1);
    }

    printf("%s: BIST pattern type %d ok!!\n", __FUNCTION__, u8_PatSel);

    FCIE_RIU_16_OF(FCIE_TEST_MODE, BIT_BIST_MODE);

    return u32_err;
}



