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
#include <command.h>
#include <mmc.h>
#include <part.h>
#include <malloc.h>
#include <asm/errno.h>
#include <asm/io.h>
#include "drvFCIE_config.h"

#ifdef SD_USE_FCIE5
#include  "drvFCIE5.h"
#else
#include "drvFCIE.h"
#endif
#define DRIVER_NAME "mstar-sd"

#define DBGMMC(X)   //X

struct fcie_regs {
	u32 mie_event;
	u32 mie_int_en;
	u32 mma_pri_reg;
	u32 miu_dma_addr_h;
	u32 miu_dma_addr_l;
	u32 card_event;
    u32 card_int_en;
    u32 card_det;
    u32 card_pwr;
    u32 force_int;
    u32 mie_path_ctl;
    u32 job_bl_cnt;
    u32 tr_bk_cnt;
    u32 rsp_size;
    u32 cmd_size;
    u32 card_wd_cnt;
    u32 sd_mode;
    u32 sd_ctl;
    u32 sd_sts;
    u32 ms_mode;
    u32 ms_ctl;
    u32 ms_sts;
    u32 cf_inf_ctl1;
    u32 cf_inf_ctl2;
    u32 cf_bus;
    u32 cf_stb_set;
    u32 cf_ctl;
    u32 sdio_ctl;
    u32 sdio_addrl;
    u32 sdio_addrh;
    u32 sdio_sts;
};

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
static int fcie_mmc_init(void);

extern unsigned char mmc_ddr_mode;

extern void halFCIE_Reset_Toggle_Count(void);
extern void halFCIE_Disable_Stop_Clock(void);

int board_mmc_init()
{
	fcie_mmc_init();
	return 0;
}
//extern U32 SD_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt);
U32 SD_translate_DMA_address_Ex(U32 u32_DMAAddr, U32 u32_ByteCnt)
{
	flush_cache(u32_DMAAddr, u32_ByteCnt);

    #if defined(CONFIG_MIPS32)

    Chip_Flush_Memory();
    return  (u32_DMAAddr & 0x1fffffff);

	#elif CONFIG_MCU_ARM
	//printf("MIU0[%Xh], MIU1[%Xh]\n", CONFIG_MIU0_BUSADDR, CONFIG_MIU1_BUSADDR);
	if(u32_DMAAddr < CONFIG_MIU1_BUSADDR) // MIU0
	{
		//REG_FCIE_CLRBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
		return  (u32_DMAAddr - CONFIG_MIU0_BUSADDR);
	}
	else
	{
	   // REG_FCIE_SETBIT(FCIE_MIU_DMA_26_16, BIT_MIU1_SELECT);
	   printf("MIU1\n");
		return  (u32_DMAAddr - CONFIG_MIU1_BUSADDR);
	}
	#endif
}
#ifdef SD_USE_FCIE5
static void fcie_setup_data(struct fcie_host *host, struct mmc_data *data)
{
	unsigned int nob = data->blocks;
	unsigned int blksz = data->blocksize;
	unsigned int datasize = nob * blksz;
    unsigned int bufaddr;

  //  unsigned int job_bl_cnt_reg = (unsigned int)(&host->base->job_bl_cnt);
   // unsigned int sdio_ctl_reg = (unsigned int)(&host->base->sdio_ctl);
  //  unsigned int sdio_addrl_reg = (unsigned int)(&host->base->sdio_addrl);
//    unsigned int sdio_addrh_reg = (unsigned int)(&host->base->sdio_addrh);
  
	host->data = data;
    DBGMMC(printf("fcie_setup_data(%d,%d)\n", nob,blksz));

    if (nob > 4095)
        DBGMMC(printf("fcie:error job cnt > 4095!\n"));

	//if (data->dma==ADMA_MODE)
	  // REG(FCIE_JOB_BL_CNT)=1;
	//else
	   REG(FCIE_JOB_BL_CNT)=nob;
	REG(FCIE_BLK_SIZE)=blksz;
	//writew(nob, job_bl_cnt_reg);
	//writew(blksz+0x8000, sdio_ctl_reg);  // Use the byte mode
	host->datasize = datasize;
        if (host->data->flags & MMC_DATA_READ) {
                 bufaddr = (unsigned int)data->dest;

		   flush_cache(bufaddr, datasize);
                 invalidate_dcache_range((ulong)bufaddr, datasize);
	 
                } 
		else 
                {
                  bufaddr = (unsigned int)data->src;
                  flush_cache(bufaddr, datasize);

                }


//	printf("bufaddr:0x%X, size:%d\n", bufaddr, datasize);
   // halFCIE_Flush_Cache(bufaddr, datasize);
   // bufaddr = halFCIE_PHY2MIUAddr(bufaddr);


   bufaddr = SD_translate_DMA_address_Ex(bufaddr,nob*blksz);

    REG(FCIE_MIU_DMA_ADDR_15_0)=bufaddr & 0xffff;
    REG(FCIE_MIU_DMA_ADDR_31_16)=(bufaddr>>16);

#if 0
	if (data->dma==ADMA_MODE)
    {
    	REG_FCIE_W(FCIE_MIU_DMA_LEN_15_0, 0x10 & 0xFFFF);
	REG_FCIE_W(FCIE_MIU_DMA_LEN_31_16,0x10 >> 16);


     }
    else
#endif		
    {
       REG(FCIE_MIU_DMA_LEN_15_0)=datasize & 0xffff;
       REG(FCIE_MIU_DMA_LEN_31_16)=(datasize >> 16);	
    }
    //writew(bufaddr, sdio_addrl_reg);
    //writew((bufaddr>>16), sdio_addrh_reg);

}
#else
static void fcie_setup_data(struct fcie_host *host, struct mmc_data *data)
{
	unsigned int nob = data->blocks;
	unsigned int blksz = data->blocksize;
	unsigned int datasize = nob * blksz;
    unsigned int bufaddr;

    unsigned int job_bl_cnt_reg = (unsigned int)(&host->base->job_bl_cnt);
    unsigned int sdio_ctl_reg = (unsigned int)(&host->base->sdio_ctl);
    unsigned int sdio_addrl_reg = (unsigned int)(&host->base->sdio_addrl);
    unsigned int sdio_addrh_reg = (unsigned int)(&host->base->sdio_addrh);

	host->data = data;
    DBGMMC(printf("fcie_setup_data(%d,%d)\n", nob,blksz));

    if (nob > 4095)
        DBGMMC(printf("fcie:error job cnt > 4095!\n"));

	writew(nob, job_bl_cnt_reg);
	writew(blksz+0x8000, sdio_ctl_reg);  // Use the byte mode
	
	host->datasize = datasize;
        if (host->data->flags & MMC_DATA_READ) {
                 bufaddr = (unsigned int)data->dest;
              //   flush_cache(bufaddr, datasize);
               //  invalidate_dcache_range((ulong)bufaddr, datasize);
                } 
		else 
                {
                  bufaddr = (unsigned int)data->src;
               // flush_cache(bufaddr, datasize);

                }


	DBGMMC(printf("bufaddr:0x%X, size:%d\n", bufaddr, datasize));
   // halFCIE_Flush_Cache(bufaddr, datasize);
   // bufaddr = halFCIE_PHY2MIUAddr(bufaddr);


   bufaddr = SD_translate_DMA_address_Ex(bufaddr,nob*blksz);
	
    writew(bufaddr, sdio_addrl_reg);
    writew((bufaddr>>16), sdio_addrh_reg);

}
#endif
static int fcie_start_cmd(struct fcie_host *host, struct mmc_cmd *cmd)
{
    CMD_RSP_INFO CmdRspInfo={0};

    if (host->cmd != NULL)
		printf("fcie_mmc : error!\n");
	host->cmd = cmd;

   // printf("[CMD %d][Arq 0x%x]\n", cmd->cmdidx, cmd->cmdarg);

	switch (cmd->resp_type) {
	case MMC_RSP_R1: /* short CRC, OPCODE */
	case MMC_RSP_R1b:/* short CRC, OPCODE, BUSY */
        CmdRspInfo.RspType = RSP_TYPE_R1;
        CmdRspInfo.RspSize = 0x06;
		break;
	case MMC_RSP_R2: /* long 136 bit + CRC */
        CmdRspInfo.RspType = RSP_TYPE_R2;
        CmdRspInfo.RspSize = 17;
		break;
	case MMC_RSP_R3: /* short */
        CmdRspInfo.RspType = RSP_TYPE_R3;
        CmdRspInfo.RspSize = 0x06;
		break;
	case MMC_RSP_NONE:
        CmdRspInfo.RspType = RSP_TYPE_NO;
        CmdRspInfo.RspSize = 0;
		break;
	default:
		printf("mxcmci: unhandled response type 0x%x\n",
				cmd->resp_type);
		return -EINVAL;
	}    
		
    CmdRspInfo.CardClock = host->clock;
    CmdRspInfo.CmdSize = 5;
    CmdRspInfo.CmdRsp.Command[0] = 0x40+cmd->cmdidx;
    cmd->cmdarg=swab32(host->cmd->cmdarg);

    //memcpy((u8*)&(CmdRspInfo.CmdRsp.Command[1]), (u8 *)&cmd->cmdarg, 4);
    CmdRspInfo.CmdRsp.Command[1] = cmd->cmdarg & 0xFF;
    CmdRspInfo.CmdRsp.Command[2] = (cmd->cmdarg >> 8) & 0xFF;
	CmdRspInfo.CmdRsp.Command[3] = (cmd->cmdarg >> 16) & 0xFF;
	CmdRspInfo.CmdRsp.Command[4] = cmd->cmdarg >> 24;

    if (host->data)
    {
        CmdRspInfo.CmdType = CMD_TYPE_ADTC;
        if (host->data->flags & MMC_DATA_WRITE)
            CmdRspInfo.Dir = DAT_WRITE;
        else
            CmdRspInfo.Dir = DAT_READ;
    }

    if( mmc_ddr_mode )
    {
        if( (host->cmd->cmdidx == 17) || (host->cmd->cmdidx == 18) )
        {
            halFCIE_Set_Read_Toggle_Count();
            halFCIE_Reset_Toggle_Count();
        }
        else if( (host->cmd->cmdidx == 24) || (host->cmd->cmdidx == 25) )
        {
            halFCIE_Set_Write_Toggle_Count();
            halFCIE_Reset_Toggle_Count();
            halFCIE_Disable_Stop_Clock();
        }
        else //if( host->cmd->cmdidx == 12 )
        {
            halFCIE_Disable_Stop_Clock();
        }
    }

    return (halFCIE_SendCmdEx(&CmdRspInfo, (u8 *)&(cmd->response[0])));

}

static void fcie_finish_request(struct fcie_host *host,
		struct mmc_cmd *cmd, struct mmc_data *data)
{
	host->cmd = NULL;
	host->data = NULL;
}
#ifdef SD_USE_FCIE5
static int fcie_read_response(struct fcie_host *host)
{
	struct mmc_cmd *cmd = host->cmd;
   // unsigned int sd_sts_reg = (unsigned int)(&host->base->sd_sts);
    u16 stat;

	if (!cmd)
		return 0;

    stat=REG(FCIE_SD_STATUS);
    FCIE_RIU_W16(FCIE_SD_STATUS, stat);

	if (cmd->resp_type == MMC_RSP_R3) {		
		stat &= ~BIT_CMD_RSP_CERR;
    }    
    
	if (stat & BIT_CMD_NO_RSP) {
        //if ((cmd->cmdidx!=8)&&(cmd->cmdidx!=55))
		     printf("cmd[%d] timeout , stat:%x\n", cmd->cmdidx,stat);
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
        //cmd->response[3]=swab32(cmd->response[3]);
 //       printf("\r\ncmd->response[0]:%lX\n", cmd->response[0]);
  //      printf("cmd->response[1]:%lX\n", cmd->response[1]);
    //    printf("cmd->response[2]:%lX\n", cmd->response[2]);
    //    printf("cmd->response[3]:%lX\n", cmd->response[3]);
    }

	return 0;
}

#else
static int fcie_read_response(struct fcie_host *host)
{
	struct mmc_cmd *cmd = host->cmd;
    unsigned int sd_sts_reg = (unsigned int)(&host->base->sd_sts);
    u16 stat;

	if (!cmd)
		return 0;


    stat=readw(sd_sts_reg);
    writew(stat, sd_sts_reg);


	if (cmd->resp_type == MMC_RSP_R3) {		
		stat &= ~R_SD_CMDRSP_CERR;
    }    
    
	if (stat & R_SD_CMD_NORSP) {
        //if ((cmd->cmdidx!=8)&&(cmd->cmdidx!=55))
		     printf("cmd[%d] timeout , stat:%x\n", cmd->cmdidx,stat);
		return TIMEOUT;
	} else if (stat & (R_SD_CMDRSP_CERR+ \
	                   R_SD_DAT_STSNEG+  \
	                   R_SD_DAT_STSERR+  \
	                   R_SD_DAT_CERR)    \
	           && cmd->resp_type & MMC_RSP_CRC) {
		printf("cmd crc error\n");
        printf("CMD%d stat=%04X\n", cmd->cmdidx, stat);
		return -EILSEQ;
	}

    if (cmd->resp_type & MMC_RSP_PRESENT) {
        cmd->response[0]=swab32(cmd->response[0]);
        cmd->response[1]=swab32(cmd->response[1]);
        cmd->response[2]=swab32(cmd->response[2]);
        cmd->response[3]=swab32(cmd->response[3]);
        //printf("\r\ncmd->response[0]:%lX\n", cmd->response[0]);
        //printf("cmd->response[1]:%lX\n", cmd->response[1]);
        //printf("cmd->response[2]:%lX\n", cmd->response[2]);
        //printf("cmd->response[3]:%lX\n", cmd->response[3]);
    }

	return 0;
}
#endif
static int fcie_transfer_data(struct fcie_host *host)
{
	struct mmc_data *data = host->data;
	u8 stat;
	unsigned long length;

	length = data->blocks * data->blocksize;
	host->datasize = 0;

	if (data->flags & MMC_DATA_READ) {
		stat = halFCIE_TransferData(DAT_READ);
		if (stat)
			return stat;
		host->datasize += length;
		//halFCIE_Flush_Cache((ulong)data->dest, length); //Fix cache problem
	} else {
		stat = halFCIE_TransferData(DAT_WRITE);
		if (stat)
			return stat;
		host->datasize += length;
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
	u8 datastat;
	int ret;

	ret = fcie_read_response(host);

	if (ret) {
		fcie_finish_request(host, host->cmd, host->data);
		return ret;
	}

	if (!host->data) {
		fcie_finish_request(host, host->cmd, host->data);
		return 0;
	}

	datastat = fcie_transfer_data(host);

	
	ret = fcie_finish_data(host, datastat);
	fcie_finish_request(host, host->cmd, host->data);

	return ret;
}

static int fcie_request(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data)
{
	struct fcie_host *host = mmc->priv;	

	if (data) {
		fcie_setup_data(host, data);
	}

    fcie_start_cmd(host, cmd);

	return fcie_cmd_done(host);
}


static void fcie_set_ios(struct mmc *mmc)
{
	struct fcie_host *host = mmc->priv;

    DBGMMC(printf("fcie_mmc : fcie_set_ios:[%d],Bus[%d]\n", mmc->clock, mmc->bus_width));

    if (mmc->bus_width == 8)
        halFCIE_SetBusWidth(&host->buswidth, BUS_8_BITS);
	else if (mmc->bus_width == 4)
		halFCIE_SetBusWidth(&host->buswidth, BUS_4_BITS);
	else
		halFCIE_SetBusWidth(&host->buswidth, BUS_1_BIT);

    host->clock = mmc->clock/1000;

	if (host->clock) {
		halFCIE_Platform_SetFcieClock(host->clock);
        halFCIE_OpenClock(1);
	} else {
		halFCIE_OpenClock(0);
	}

}


static int fcie_init(struct mmc *mmc)
{
printf("fcie init\n");
	halFCIE_Init();
	return 0;
}

static int fcie_initialize(void)
{
	struct mmc *mmc = NULL;

	mmc = malloc(sizeof(struct mmc));

	if (!mmc)
		return -ENOMEM;
	memset(mmc, 0, sizeof(struct mmc));

	sprintf(mmc->name, DRIVER_NAME);
	mmc->send_cmd = fcie_request;
	mmc->set_ios = fcie_set_ios;
	mmc->init = fcie_init;
	#ifdef CONFIG_SD
    mmc->host_caps =  MMC_MODE_4BIT;
	mmc->host_caps |= MMC_MODE_HS;
	#else
    mmc->host_caps =  MMC_MODE_8BIT;
	mmc->host_caps |= MMC_MODE_HS_52MHz | MMC_MODE_HS;
	#endif    

	host->base = (struct fcie_regs *)RIU_BASE_FCIE;
	mmc->priv = host;
	host->mmc = mmc;

	mmc->voltages = MMC_VDD_27_28 | MMC_VDD_28_29 | MMC_VDD_29_30 | MMC_VDD_30_31 | MMC_VDD_31_32 |
					MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_34_35 | MMC_VDD_35_36;

	mmc->f_min = 150000;
	mmc->f_max = 48000000;

	mmc_register(mmc);

	return 0;
}

int fcie_mmc_init(void)
{
	return fcie_initialize();
}


