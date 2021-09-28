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
#include <common.h>
#include <mmc.h>
#include <malloc.h>
#include <errno.h>
//#include <asm/io.h>
#include "hal_sdio.h"

#ifdef SD_USE_FCIE5
//#include  "drvFCIE5.h"
#else
//#include "drvFCIE.h"
#endif

#define DRIVER_NAME "SdioHost"


#define DBGMMC(X)   //X

struct fcie_host {

	struct mmc          *mmc;
	struct fcie_regs    *base;
	int                 irq;
	int                 detect_irq;
	int                 dma;
	int                 do_dma;
	unsigned int        power_mode;

	struct mmc_cmd      *cmd;
	struct mmc_data     *data;

	unsigned int        dma_nents;
	unsigned int        datasize;
	unsigned int        dma_dir;

	u16                 rev_no;
	unsigned char       buswidth;

	int                 clock;
};

static struct fcie_host fcie_host;
static struct fcie_host *host = &fcie_host;
static int fcie_initialize(void);

extern unsigned char sd_ddr_mode;


int board_sd_init(bd_t *bis)
{
	if(bis!=NULL)
	{
		fcie_initialize();
	}
	return 0;
}


static void fcie_finish_request(struct fcie_host *host)
{
	host->cmd = NULL;
	host->data = NULL;
}

static int fcie_read_response(struct fcie_host *host)
{
	struct mmc_cmd *cmd = host->cmd;
    u16 stat;

	if (!cmd)
		return 0;

    stat = FCIE_RIU_R16(FCIE_SD_STATUS);
    FCIE_RIU_W16(FCIE_SD_STATUS, stat);

	if (stat & BIT_CMD_NO_RSP) {

        //if ((cmd->cmdidx!=8)&&(cmd->cmdidx!=55))
        printf("cmd[%d] timeout, stat: %04Xh\n", cmd->cmdidx, stat);
		printf("command no response\n");
		return TIMEOUT;

	} else if (stat & (BIT_CMD_RSP_CERR+ \
	                   BIT_DAT_WR_TOUT+  \
	                   BIT_DAT_WR_CERR+  \
	                   BIT_DAT_RD_CERR)    \
	           && cmd->resp_type & MMC_RSP_CRC) {
		printf("cmd crc error\n");
        printf("CMD%d stat=%04X\n", cmd->cmdidx, stat);
		return -EILSEQ;
	}

    if (cmd->resp_type & MMC_RSP_PRESENT) {
		// cmd->response[0]=swab32(cmd->response[0]);
		// cmd->response[1]=swab32(cmd->response[1]);
		// cmd->response[2]=swab32(cmd->response[2]);
		// cmd->response[3]=swab32(cmd->response[3]);
		// printf("\r\ncmd->response[0]:%lX\n", cmd->response[0]);
		// printf("cmd->response[1]:%lX\n", cmd->response[1]);
		// printf("cmd->response[2]:%lX\n", cmd->response[2]);
		// printf("cmd->response[3]:%lX\n", cmd->response[3]);
    }

	return 0;
}





static int fcie_finish_data(struct fcie_host *host, unsigned char stat)
{
	int data_error = 0;

	if (stat) {
		printf("fcie_mmc : request failed. status: 0x%08x\n",
				stat);
		if (stat == IO_ERROR) {
			data_error = -EILSEQ;
		} else if (stat == IO_TIME_OUT) {
			data_error = TIMEOUT;
		} else {
			data_error = -EIO;
		}
	}

	host->data = NULL;

	return data_error;
}

static int fcie_cmd_done(struct fcie_host *host)
{
	u8 datastat = 0;
	int ret;

	ret = fcie_read_response(host);

	if (ret) {
		fcie_finish_request(host);
		return ret;
	}

	if (!host->data) {
		fcie_finish_request(host);
		return 0;
	}

	//datastat = fcie_transfer_data(host);

	ret = fcie_finish_data(host, datastat);
	fcie_finish_request(host);

	return ret;
}


#define U32BEND2LEND(X) ( ((X&0x000000FF)<<24) + ((X&0x0000FF00)<<8) + ((X&0x00FF0000)>>8) + ((X&0xFF000000)>>24) )
#define U16BEND2LEND(X) ( ((X&0x00FF)<<8) + ((X&0xFF00)>>8) )

static int fcie_request(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data)
{
	struct fcie_host *host = mmc->priv;
	U32 u32_err;
    CMD_RSP_INFO CmdRspInfo;
	U8 * pTemp;
	U8 u8RspByteNum;
	U8 u8_i;

	memset(&CmdRspInfo, 0, sizeof(CMD_RSP_INFO));

	CmdRspInfo.CardClock = host->clock;
	CmdRspInfo.Command.Cmd.Index = cmd->cmdidx;
	CmdRspInfo.Command.Cmd.Arg = U32BEND2LEND(cmd->cmdarg);
	CmdRspInfo.ClockStop = CLK_STOP;

	//printf("sdio request()\n");
	//printf("cmd->cmdarg = %08lX\n", cmd->cmdarg);
	//printf("CmdRspInfo.Command.Cmd.Arg = %08lX\n", CmdRspInfo.Command.Cmd.Arg);

	if(!data)
	{
		CmdRspInfo.CmdType = CMD_TYPE_NDTC; // simple command
	}
	else
	{
		CmdRspInfo.CmdType = CMD_TYPE_ADTC; // data transder command
	}

	switch (cmd->resp_type) {

		case MMC_RSP_R1:
		//case MMC_RSP_R5:
		//case MMC_RSP_R6:
		//case MMC_RSP_R7:
			CmdRspInfo.RspType = RSP_TYPE_R1;
			break;
		case MMC_RSP_NONE:
			CmdRspInfo.RspType = RSP_TYPE_NO;
			break;

		case MMC_RSP_R1b:
			CmdRspInfo.RspType = RSP_TYPE_R1b;
			break;
		case MMC_RSP_R2:
			CmdRspInfo.RspType = RSP_TYPE_R2;
			break;
		case MMC_RSP_R3:
		//case MMC_RSP_R4:
			CmdRspInfo.RspType = RSP_TYPE_R3;
			break;
		default:
			printf("SDIO Err: not support response type");
			break;
	}

	if(data) // command with data transfer
	{
		switch(data->flags) {
			case MMC_DATA_WRITE:
				//printf("MMC_DATA_WRITE, ");
				CmdRspInfo.ReadWriteDir = DIR_W;
				CmdRspInfo.ReadWriteTimeOut = 77; // not NCRC, detect MIU side hang up
				break;
			case MMC_DATA_READ:
				//printf("MMC_DATA_READ, ");
				CmdRspInfo.ReadWriteDir = DIR_R;
				CmdRspInfo.ReadWriteTimeOut = 77;	// 20ns x 65536 x 77 = 100.9254 ms
				break;
			default:
				printf("SD Err: data->flags = %08Xh, ", data->flags);
				break;
		}

		switch(host->buswidth)
		{
			case BUS_1_BIT:
				CmdRspInfo.BusWidth = BUS_1_BIT;
				//printf("1 bits\n");
				break;
			case BUS_4_BITS:
				CmdRspInfo.BusWidth = BUS_4_BITS;
				//printf("4 bits\n");
				break;
			case BUS_8_BITS:
				CmdRspInfo.BusWidth = BUS_8_BITS;
				//printf("8 bits\n");
				break;
			default:
				printf("SD Err: wrong bus width!\n");
				break;
		}

		#if 0 // defined(ENABLE_eMMC_ADMA_MODE) && ENABLE_eMMC_ADMA_MODE

			/*data->bytes_xfered = */mstar_mci_prepare_descriptors(data);
			mstar_mci_setup_descriptor_addr(&CmdRspInfo);

		#else

			if (data->flags & MMC_DATA_READ)
			{
				CmdRspInfo.BusAddr = (unsigned int)data->dest;
				flush_cache(CmdRspInfo.BusAddr, data->blocks * data->blocksize);
				Chip_Flush_Memory();
			}
			else
			{
				CmdRspInfo.BusAddr = (unsigned int)data->src;
				flush_cache(CmdRspInfo.BusAddr, data->blocks * data->blocksize);
			}

			CmdRspInfo.DataPath = PATH_DMA;
			CmdRspInfo.pBufferAddr = NULL; // DMA don't need this field
			CmdRspInfo.BlockSize = data->blocksize;
			CmdRspInfo.BlockCount = data->blocks;

		#endif

	}


	u32_err = HalSdio_SendCommand(&CmdRspInfo);


	if(u32_err)
	{
		#if 0
			//cmd->error = -EIO;
			printf("request happen error %02Xh\n", u32_err);
			return (int)u32_err;
		#else
			//printf("send command fail %Xh\n", u32_err);
			switch(u32_err)
			{
				case IO_TIME_OUT:
				case IO_RSP_CRC_ERROR:
				case IO_R_DATA_CRC_ERROR:
				case IO_W_DATA_STS_ERROR:
				case IO_W_DATA_STS_NEGATIVE:
					return u32_err;
					break;

				case IO_CMD_NO_RSP:
					return NO_CARD_ERR;
					break;
			}
		#endif

		if(data) // command with data transfer
		{
			// dump out what we read
			//cmd->data->bytes_xfered = 0;
		}
	}
	else
	{
		//cmd->error = 0;

		pTemp = (U8*)&(cmd->response[0]);
		u8RspByteNum = (cmd->resp_type&MMC_RSP_136)?16:4; // decide response length

		//printf("u8RspByteNum = %d\n", u8RspByteNum);

		for(u8_i=0; u8_i<u8RspByteNum; u8_i++) {
			pTemp[ (3-(u8_i%4)) + ((u8_i/4)<<2) ] = HalSdio_CmdFifoGet((U8)(u8_i+1));
		}
	}

	return fcie_cmd_done(host);

}


static void fcie_set_ios(struct mmc *mmc)
{
	struct fcie_host *host = mmc->priv;

    //printf("fcie_set_ios: [%d KHz], [%d bits]\n", mmc->clock/1000, mmc->bus_width);

    if (mmc->bus_width == 8)
        HalSdio_SetBusWidth(&host->buswidth, BUS_8_BITS);
	else if (mmc->bus_width == 4)
		HalSdio_SetBusWidth(&host->buswidth, BUS_4_BITS);
	else
		HalSdio_SetBusWidth(&host->buswidth, BUS_1_BIT);

    host->clock = mmc->clock/1000; // use KHz as unit

	switch (mmc->op_mode)
	{
		#ifdef SDIO_SUPPORT_SD30
		case SD30_MODE_SDR104:
		case SD30_MODE_SDR50:
			#if 0
			if(host->clock >= 200000)
			{
				HalSdio_clock_setting(0x90); // eMMC_PLL_CLK_200M
				//HalSdio_clock_setting(0x8F); // eMMC_PLL_CLK_100M
			}
			else if(host->clock >= 100000)
			{
				HalSdio_clock_setting(0x8C); // eMMC_PLL_CLK_100M
			}
			else
			{
				HalSdio_clock_setting(0x86); // eMMC_PLL_CLK__48M
			}
			#else
				HalSdio_SetClock(host->clock);
			#endif
			break;

		case SD30_MODE_DDR50: // 8 bits macro, IP:PLL = 1:4
			//HalSdio_clock_setting(0x90); // 200/4 = 50 MHz
			HalSdio_SetClock(host->clock);
			break;
		#endif
		default:
			if(host->clock)
			{
				mmc->clock = HalSdio_SetClock(host->clock) * 1000; // update real clock back to mmc struct
				//printf("mmc->clock = %d\n", mmc->clock);
		        HalSdio_OpenClock(1);
			}
			else
			{
				HalSdio_OpenClock(0);
			}
			break;
	}

	switch (mmc->op_mode)
	{
		#ifdef SDIO_SUPPORT_SD30
		case SD30_MODE_SDR104:
		case SD30_MODE_SDR50:
			HalSdio_SwitchPad(SDIO_PAD_SDR104);
			break;
		case SD30_MODE_DDR50:
			HalSdio_SwitchPad(SDIO_PAD_DDR50);
			break;
		#endif
		case MMC_MODE_HS:
			HalSdio_SwitchPad(SDIO_PAD_SDR25);
			break;
		default:
			HalSdio_SwitchPad(SDIO_PAD_SDR12);
			break;
	}
}

static int fcie_init(struct mmc *mmc)
{
	if(mmc!=NULL)
	{
		HalSdio_Init();
	}
	return 0;
}

static int fcie_initialize(void)
{
	struct mmc *mmc = NULL;

	#ifndef FPGA_BOARD
		mmc = malloc(sizeof(struct mmc));
	#else
		mmc = &g_st_mmc;
	#endif

	if (!mmc)
		return -ENOMEM;
	memset(mmc, 0, sizeof(struct mmc));

	sprintf(mmc->name, DRIVER_NAME);
	mmc->send_cmd = fcie_request;
	mmc->set_ios = fcie_set_ios;
	mmc->init = fcie_init;

    mmc->host_caps =  MMC_MODE_4BIT;
	mmc->host_caps |= MMC_MODE_HS;

	host->base = NULL;
	mmc->priv = host;
	host->mmc = mmc;

	mmc->voltages = MMC_VDD_27_28 | MMC_VDD_28_29 | MMC_VDD_29_30 | MMC_VDD_30_31 | MMC_VDD_31_32 |
					MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_34_35 | MMC_VDD_35_36;

	mmc->f_min =    300000; // 300 KHz

#ifdef SDIO_SUPPORT_SD30
	mmc->f_max = 200000000; // 200 MHz
#else
	mmc->f_max =  50000000; //  50 MHz
#endif

	sd_register(mmc);

	mmc->init(mmc);

	return 0;
}
