/*
 * Copyright 2008, Freescale Semiconductor, Inc
 * Andy Fleming
 *
 * Based vaguely on the Linux code
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
//#include <command.h>
#include <mmc.h>
//#include <part.h>
//#include <malloc.h>
//#include <linux/list.h>
//#include <div64.h>

//#include "drvFCIE5.h"
#include "DrvSd.h"
#include "hal_sdio.h"

// local function prototype
int	HalSdio_ScanSkew_SetBest(struct mmc *mmc);
int switch_2_sdr25(struct mmc *mmc);
int switch_2_sdr12(struct mmc *mmc);
#ifdef SDIO_SUPPORT_SD30
int switch_2_ddr50(struct mmc *mmc);
int switch_2_sdr50(struct mmc *mmc);
int switch_2_sdr104(struct mmc *mmc);
#endif

// external function prototype
extern int board_sd_init(bd_t *bis);
//extern void HalSdio_SetVDD(U8 u8OnOff);


/* Set block count limit because of 16 bit register limit on some hardware*/
#ifndef CONFIG_SYS_MMC_MAX_BLK_COUNT
#define CONFIG_SYS_MMC_MAX_BLK_COUNT 65535
#endif
//#define TEST_18V  1
//#define CONFIG_MMC_TRACE 1

unsigned char sd_ddr_mode=0;

#ifndef FPGA_BOARD
	static struct list_head mmc_devices;
#else
	struct mmc g_st_mmc;
#endif

static int cur_sd_dev_num = -1;

int __board_sd_getcd(u8 *cd, struct mmc *mmc) {
	if(mmc)
		printf("cd = %d\n", *cd);
	return -1;
}

int board_mmc_getcd(u8 *cd, struct mmc *mmc)__attribute__((weak,
	alias("__board_sd_getcd")));

int mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data)
{
#ifdef CONFIG_MMC_TRACE
	int ret;
	//int i;
	//u8 *ptr;

	//printf("\n");
	//printf("CMD%d_%08X", cmd->cmdidx, cmd->cmdarg);
	//printf("\tFLAG %d\n", cmd->flags);
	ret = mmc->send_cmd(mmc, cmd, data);

	#if 0
	switch (cmd->resp_type) {
		case MMC_RSP_NONE:
			printf("\t\tMMC_RSP_NONE\n");
			break;
		case MMC_RSP_R1:
			printf("\t\tMMC_RSP_R1,5,6,7 \t 0x%08X \n", cmd->response[0]);
			break;
		case MMC_RSP_R1b:
			printf("\t\tMMC_RSP_R1b\t\t 0x%08X \n", cmd->response[0]);
			break;
		case MMC_RSP_R2:
			printf("\t\tMMC_RSP_R2\t\t 0x%08X \n", cmd->response[0]);
			printf("\t\t          \t\t 0x%08X \n", cmd->response[1]);
			printf("\t\t          \t\t 0x%08X \n", cmd->response[2]);
			printf("\t\t          \t\t 0x%08X \n", cmd->response[3]);
			printf("\n");
			printf("\t\t\t\t\tDUMPING DATA\n");
			for (i = 0; i < 4; i++) {
				int j;
				printf("\t\t\t\t\t%03d - ", i*4);
				ptr = &cmd->response[i];
				ptr += 3;
				for (j = 0; j < 4; j++)
					printf("%02X ", *ptr--);
				printf("\n");
			}
			break;
		case MMC_RSP_R3:
			printf("\t\tMMC_RSP_R3,4\t\t 0x%08X \n", cmd->response[0]);
			break;
		default:
			printf("\t\tERROR MMC rsp not supported\n");
			break;
	}
	#endif

	return ret;

#else

	return mmc->send_cmd(mmc, cmd, data);

#endif

}

int mmc_send_status(struct mmc *mmc, int timeout)
{
	struct mmc_cmd cmd;
	int err;
#ifdef CONFIG_MMC_TRACE
	int status;
#endif

	cmd.cmdidx = MMC_CMD_SEND_STATUS;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;

	do {
		err = mmc_send_cmd(mmc, &cmd, NULL);
		if (err)
			return err;
		else if (cmd.response[0] & MMC_STATUS_RDY_FOR_DATA)
			break;

		udelay(1000);

		if (cmd.response[0] & MMC_STATUS_MASK) {
			printf("Status Error: 0x%08X\n", cmd.response[0]);
			return COMM_ERR;
		}
	} while (timeout--);

#ifdef CONFIG_MMC_TRACE
	status = (cmd.response[0] & MMC_STATUS_CURR_STATE) >> 9;
	printf("CURR STATE:%d\n", status);
#endif
	if (!timeout) {
		printf("Timeout waiting card ready\n");
		return TIMEOUT;
	}

	return 0;
}

int mmc_set_blocklen(struct mmc *mmc, int len)
{
	struct mmc_cmd cmd;

	cmd.cmdidx = MMC_CMD_SET_BLOCKLEN;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = len;
	cmd.flags = 0;

	return mmc_send_cmd(mmc, &cmd, NULL);
}

struct mmc *find_sd_device(int dev_num)
{

	#if !defined FPGA_BOARD || FPGA_BOARD==0

	struct mmc *m;
	struct list_head *entry;

	list_for_each(entry, &mmc_devices) {
		m = list_entry(entry, struct mmc, link);

		if (m->block_dev.dev == dev_num)
		{
			return m;
		}
	}

	printf("SD device %d not found\n", dev_num);

	return NULL;

	#else // FPGA use

		if(dev_num)
		{
			printf("find sd device not zero %d\n", dev_num);
		}
		return &g_st_mmc;

	#endif

}

static ulong
mmc_write_blocks(struct mmc *mmc, ulong start, lbaint_t blkcnt, const void*src)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int timeout = 1000;
	unsigned char write_result = 0;

	if ((start + blkcnt) > mmc->block_dev.lba) {
		printf("MMC: block number 0x%lx exceeds max(0x%lx)\n",
			start + blkcnt, mmc->block_dev.lba);
		return 0;
	}

	if (blkcnt > 1)
		cmd.cmdidx = MMC_CMD_WRITE_MULTIPLE_BLOCK;
	else
		cmd.cmdidx = MMC_CMD_WRITE_SINGLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * mmc->write_bl_len;

	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;

	data.src = src;
	data.blocks = blkcnt;
	data.blocksize = mmc->write_bl_len;
	data.flags = MMC_DATA_WRITE;

	if (mmc_send_cmd(mmc, &cmd, &data)) {
		write_result = 1;
		printf("mmc fail to write\n");
		//return 0;
	}

	/* SPI multiblock writes terminate using a special
	 * token, not a STOP_TRANSMISSION request.
	 */
	if (!mmc_host_is_spi(mmc) && blkcnt > 1) {
		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1b;
		cmd.flags = 0;
		if (mmc_send_cmd(mmc, &cmd, NULL)) {
			printf("mmc fail to send stop cmd\n");
			write_result = 2;
			//return 0;
		}

		/* Waiting for the ready status */
		mmc_send_status(mmc, timeout);
	}

	if(write_result)
	{
		return 0;
	}

	return blkcnt;
}

static ulong
mmc_bwrite(int dev_num, ulong start, lbaint_t blkcnt, const void*src)
{
	lbaint_t cur, blocks_todo = blkcnt;
	unsigned char * pBuf = (unsigned char *)src;

	struct mmc *mmc = find_sd_device(dev_num);
	if (!mmc)
		return 0;

	/*if (mmc_set_blocklen(mmc, mmc->write_bl_len))
		return 0;*/

	do {
		cur = (blocks_todo > mmc->b_max) ?  mmc->b_max : blocks_todo;
		if(mmc_write_blocks(mmc, start, cur, pBuf) != cur)
			return 0;
		blocks_todo -= cur;
		start += cur;
		pBuf += cur * mmc->write_bl_len;
	} while (blocks_todo > 0);

	return blkcnt;
}

ulong mmc_read_blocks(struct mmc *mmc, void *dst, ulong start, lbaint_t blkcnt)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	//int timeout = 1000;
	unsigned char read_result = 0;

	if (blkcnt > 1)
		cmd.cmdidx = MMC_CMD_READ_MULTIPLE_BLOCK;
	else
		cmd.cmdidx = MMC_CMD_READ_SINGLE_BLOCK;

	if (mmc->high_capacity)
		cmd.cmdarg = start;
	else
		cmd.cmdarg = start * mmc->read_bl_len;

	cmd.resp_type = MMC_RSP_R1;
	cmd.flags = 0;


	data.dest = dst;
	data.blocks = blkcnt;
	data.blocksize = mmc->read_bl_len;
	data.flags = MMC_DATA_READ;

	//printf("data.blocksize = %d\n", data.blocksize);

	read_result = mmc_send_cmd(mmc, &cmd, &data);
	if(read_result)
	{
		printf("mmc fail to read %Xh\n", read_result);
		read_result = 1;
	}

	if (blkcnt > 1) {
		cmd.cmdidx = MMC_CMD_STOP_TRANSMISSION;
		cmd.cmdarg = 0;
		cmd.resp_type = MMC_RSP_R1b;
		cmd.flags = 0;
		if (mmc_send_cmd(mmc, &cmd, NULL)) {
			//printf("mmc fail to send stop cmd\n");
			return 0;
		}

		/* Waiting for the ready status */
		//mmc_send_status(mmc, timeout);
	}

	if(read_result)
	{
		return 0;
	}

	return blkcnt;
}

static ulong mmc_bread(int dev_num, ulong start, lbaint_t blkcnt, void *dst)
{
	lbaint_t cur, blocks_todo = blkcnt;
	unsigned char * pBuf = (unsigned char *)dst;

	if (blkcnt == 0)
		return 0;

	struct mmc *mmc = find_sd_device(dev_num);
	if (!mmc)
		return 0;

	if ((start + blkcnt) > mmc->block_dev.lba) {
		printf("MMC: block number 0x%lx exceeds max(0x%lx)\n",
			start + blkcnt, mmc->block_dev.lba);
		return 0;
	}

	/*if (mmc_set_blocklen(mmc, mmc->read_bl_len))
	{
		printf("fail at set block length\n");
		return 0;
	}*/

	do {
		cur = (blocks_todo > mmc->b_max) ?  mmc->b_max : blocks_todo;
		if(mmc_read_blocks(mmc, pBuf, start, cur) != cur)
		{
			//printf("fail at read block\n");
			return 0;
		}
		blocks_todo -= cur;
		start += cur;
		pBuf += cur * mmc->read_bl_len;
	} while (blocks_todo > 0);

	return blkcnt;
}

void mmc_read_write_test(int dev_num, unsigned int u32Loop)
{
	u32 n, i, j;
	u32 u32BlockAddr = 10000; // block address
	static u8 test_pattern[5120];
	static u8 read_pattern[5120];
	struct mmc *mmc = find_sd_device(dev_num);

	//mmc->has_init = 0;
	//sd_init(mmc);


	if(u32Loop==0) u32Loop = 1000;
	if(u32Loop==9999) u32Loop = 0;
	i = 0;

	for(;;)
	{
		if( (i++==u32Loop) && (u32Loop!=0) ) break;

		printf("loop: %3d\r",i);
		for(j=0; j<5120; j++)
		{
			test_pattern[j]=j+i;
		}

		n = mmc->block_dev.block_write(dev_num, u32BlockAddr, 10, test_pattern);
		if(n==0)
		{
			printf("write fail\n"); while(1);
		}

		n = mmc->block_dev.block_read(dev_num, u32BlockAddr, 10, read_pattern);
		if(n==0)
		{
			printf("write fail\n"); while(1);
		}

		for(j=0; j<5120; j++)
		{
			if (read_pattern[j] != test_pattern[j])
			{
				printf("\n[%d]r:[0x%x]!=w:[0x%x]\n", j,read_pattern[j],test_pattern[j]);
				while(1);
			}
		}
	}

	printf("burning OK\n");

}

void mmc_read_write_test_each_op_mode(int dev_num)
{
	struct mmc *mmc = find_sd_device(dev_num);

#ifdef SDIO_SUPPORT_SD30

	if( (mmc->volt_sw_18v) && (mmc->card_caps & SD30_MODE_SDR104) )
	{
		switch_2_sdr104(mmc);
		mmc_read_write_test(0, 1000);
		sd_adma_read_write_test();
	}

	if( (mmc->volt_sw_18v) && (mmc->card_caps & SD30_MODE_DDR50) )
	{
		switch_2_ddr50(mmc);
		mmc_read_write_test(0, 1000);
		sd_adma_read_write_test();
	}

	if( (mmc->volt_sw_18v) && (mmc->card_caps & SD30_MODE_SDR50) )
	{
		switch_2_sdr50(mmc);
		mmc_read_write_test(0, 1000);
		sd_adma_read_write_test();
	}

#endif

	switch_2_sdr25(mmc);
	mmc_read_write_test(0, 1000);
	sd_adma_read_write_test();

}

int mmc_go_idle(struct mmc* mmc)
{
	struct mmc_cmd cmd;
	int err;

	udelay(1000);

	cmd.cmdidx = MMC_CMD_GO_IDLE_STATE;
	cmd.cmdarg = 0;
	cmd.resp_type = MMC_RSP_NONE;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	udelay(2000);

	return 0;
}

int sd_voltage_switch(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;

	cmd.cmdidx = SD_CMD_VOLT_SWITCH;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err) return err;

	return 0;
}

int sd_check_18V(struct mmc *mmc)
{
	//int timeout = 1000;
	int err;
  	struct mmc_cmd cmd;

	while (1)
	{
		cmd.cmdidx = MMC_CMD_APP_CMD;
		cmd.resp_type = MMC_RSP_R1;
		cmd.cmdarg = 0;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		cmd.cmdidx = SD_CMD_APP_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmdarg = mmc_host_is_spi(mmc) ? 0 :
			(mmc->voltages & 0xff8000);

		if (mmc->version == SD_VERSION_2)
			cmd.cmdarg |= OCR_HCS|0x1000000;	//set S18R

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		printf("S18R:%x\n",cmd.response[0]);

		if ( cmd.response[0] & OCR_BUSY) break;
		udelay(1000);
	}
	if ((cmd.response[0] & 0x41000000) == 0x41000000) return 0;	//support 1.8V
	else return -1;
}

int sd_send_op_cond(struct mmc *mmc)
{
	int timeout = 1000;
	int err;
	struct mmc_cmd cmd;

	do {
		cmd.cmdidx = MMC_CMD_APP_CMD;
		cmd.resp_type = MMC_RSP_R1;
		cmd.cmdarg = 0;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		cmd.cmdidx = SD_CMD_APP_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;

		/*
		 * Most cards do not answer if some reserved bits
		 * in the ocr are set. However, Some controller
		 * can set bit 7 (reserved for low voltages), but
		 * how to manage low voltages SD card is not yet
		 * specified.
		 */
		//cmd.cmdarg = mmc_host_is_spi(mmc) ? 0 :
		//	(mmc->voltages & 0xff8000);

		//if (mmc->version == SD_VERSION_2)
		//	cmd.cmdarg |= OCR_HCS;

		//cmd.cmdarg = 0x40018000; // SD 2.0 host
		cmd.cmdarg = 0x41018000; // SD 3.0 host

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err) {
			//printf("sd_send_op_cond return %d\n", err);
			return err;
		}
		//printf("sd send op cond --> cmd.response[0] = %08X\n", cmd.response[0]);

		udelay(1000);

	} while ((!(cmd.response[0] & OCR_BUSY)) && timeout--);

	if (timeout <= 0)
		return UNUSABLE_ERR;

	if (mmc->version != SD_VERSION_2)
		mmc->version = SD_VERSION_1_0;

	if (mmc_host_is_spi(mmc)) { /* read OCR for spi */
		cmd.cmdidx = MMC_CMD_SPI_READ_OCR;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmdarg = 0;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;
	}

	mmc->ocr = cmd.response[0];

	//if(mmc->ocr&OCR_HCS) printf("SDHC\n");
	if(mmc->ocr&OCR_UHS2) printf("UHS-II\n");

	if(mmc->ocr&OCR_S18R) {
		//printf("S18A\n");
		mmc->volt_sw_18v = 1;
	}
	else
	{
		mmc->volt_sw_18v = 0;
	}

	mmc->high_capacity = ((mmc->ocr & OCR_HCS) == OCR_HCS);
	mmc->rca = 0;

	return 0;
}

int mmc_send_op_cond(struct mmc *mmc)
{
	int timeout = 10000;
	struct mmc_cmd cmd;
	int err;

	/* Some cards seem to need this */
	mmc_go_idle(mmc);

 	/* Asking to the card its capabilities */
 	cmd.cmdidx = MMC_CMD_SEND_OP_COND;
 	cmd.resp_type = MMC_RSP_R3;
 	cmd.cmdarg = 0;
 	cmd.flags = 0;

 	err = mmc_send_cmd(mmc, &cmd, NULL);

 	if (err)
 		return err;

 	udelay(1000);

	do {
		cmd.cmdidx = MMC_CMD_SEND_OP_COND;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmdarg = (mmc_host_is_spi(mmc) ? 0 :
				(mmc->voltages &
				(cmd.response[0] & OCR_VOLTAGE_MASK)) |
				(cmd.response[0] & OCR_ACCESS_MODE));
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		udelay(1000);
	} while (!(cmd.response[0] & OCR_BUSY) && timeout--);

	if (timeout <= 0)
		return UNUSABLE_ERR;

	if (mmc_host_is_spi(mmc)) { /* read OCR for spi */
		cmd.cmdidx = MMC_CMD_SPI_READ_OCR;
		cmd.resp_type = MMC_RSP_R3;
		cmd.cmdarg = 0;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;
	}

	mmc->version = MMC_VERSION_UNKNOWN;
	mmc->ocr = cmd.response[0];

	mmc->high_capacity = ((mmc->ocr & OCR_HCS) == OCR_HCS);
	mmc->rca = 0;

	return 0;
}


int sd_send_ext_csd(struct mmc *mmc, char *ext_csd)
{
	struct mmc_cmd cmd;
	struct mmc_data data;
	int err;

	/* Get the Card Status Register */
	cmd.cmdidx = MMC_CMD_SEND_EXT_CSD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	data.dest = ext_csd;
	data.blocks = 1;
	data.blocksize = 512;
	data.flags = MMC_DATA_READ;

	err = mmc_send_cmd(mmc, &cmd, &data);

	return err;
}


int sd_mmc_switch(struct mmc *mmc, u8 set, u8 index, u8 value)
{
	struct mmc_cmd cmd;
	int timeout = 1000;
	int ret;

	if(set != EXT_CSD_CMD_SET_NORMAL) printf("SdErr: wrong command set\n");

	cmd.cmdidx = MMC_CMD_SWITCH;
	cmd.resp_type = MMC_RSP_R1b;
	cmd.cmdarg = (MMC_SWITCH_MODE_WRITE_BYTE << 24) |
				 (index << 16) |
				 (value << 8);
	cmd.flags = 0;

	ret = mmc_send_cmd(mmc, &cmd, NULL);

	/* Waiting for the ready status */
	mmc_send_status(mmc, timeout);

	return ret;

}

int mmc_change_freq(struct mmc *mmc)
{
	char ext_csd[512];
	char cardtype;
	int err;

	mmc->card_caps = 0;

	if (mmc_host_is_spi(mmc))
		return 0;

	/* Only version 4 supports high-speed */
	if (mmc->version < MMC_VERSION_4)
		return 0;

	mmc->card_caps |= MMC_MODE_4BIT;

	err = sd_send_ext_csd(mmc, ext_csd);

	if (err)
		return err;

	cardtype = ext_csd[196] & 0xf;

	err = sd_mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1);

	if (err)
		return err;

	/* Now check to see that it worked */
	err = sd_send_ext_csd(mmc, ext_csd);

	if (err)
		return err;

	/* No high-speed support */
	if (!ext_csd[185])
		return 0;

	/* High Speed is set, there are two types: 52MHz and 26MHz */
	if (cardtype & MMC_HS_52MHZ)
		mmc->card_caps |= MMC_MODE_HS_52MHz | MMC_MODE_HS;
	else
		mmc->card_caps |= MMC_MODE_HS;

	return 0;
}

int sd_switch_part(int dev_num, unsigned int part_num)
{
	struct mmc *mmc = find_sd_device(dev_num);

	if (!mmc)
		return -1;

	return sd_mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_PART_CONF,
			  (mmc->part_config & ~PART_ACCESS_MASK)
			  | (part_num & PART_ACCESS_MASK));
}

int sd_switch(struct mmc *mmc, int mode, int group, u8 value, u8 *resp)
{
	struct mmc_cmd cmd;
	struct mmc_data data;

	/* Switch the frequency */
	cmd.cmdidx = SD_CMD_SWITCH_FUNC;
	cmd.resp_type = MMC_RSP_R1b;
	cmd.cmdarg = (mode << 31) | 0xffffff;
	cmd.cmdarg &= ~(0xf << ((group-1) * 4));
	cmd.cmdarg |= value << ((group-1) * 4);
	cmd.flags = 0;

	data.dest = (char *)resp;
	data.blocksize = 64;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	return mmc_send_cmd(mmc, &cmd, &data);
}


int sd_tuning(struct mmc *mmc, u8 * data_buf)
{
	struct mmc_cmd cmd;
	struct mmc_data data;

	memset(data_buf, 0x00, 64);

	/* Switch the frequency */
	cmd.cmdidx = SD_CMD_SEND_TUNING_PATTERN;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	data.dest = (char *)data_buf;
	data.blocksize = 64;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;

	return mmc_send_cmd(mmc, &cmd, &data);
}


static uint scr[2] __attribute__((aligned(0x80)));
static uint switch_status[16] __attribute__((aligned(0x80)));

int sd_change_freq(struct mmc *mmc)
{
	int err;
	struct mmc_cmd cmd;
	//uint scr[2];
	//uint switch_status[16];
	struct mmc_data data;
	int timeout;

	mmc->card_caps = 0;

	if (mmc_host_is_spi(mmc))
		return 0;

	/* Read the SCR to find out if this card supports higher speeds */
	cmd.cmdidx = MMC_CMD_APP_CMD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL); // CMD55

	if (err)
		return err;

	cmd.cmdidx = SD_CMD_APP_SEND_SCR;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	timeout = 3;

retry_scr:

	data.dest = (char *)&scr;
	data.blocksize = 8;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;
	scr[0] = 0;	scr[1] = 0; // clear to zero

	err = mmc_send_cmd(mmc, &cmd, &data); // ACMD51

	if (err) {
		if (timeout--)
			goto retry_scr;

		return err;
	}

	//printf("scr[0] = %08X\n", scr[0]);
	//printf("scr[1] = %08X\n", scr[1]);

	//flush_cache((ulong)&scr[0], 8); /* FIXME */
	mmc->scr[0] = __be32_to_cpu(scr[0]);
	mmc->scr[1] = __be32_to_cpu(scr[1]);

	//printf("mmc->scr[0] = %08X\n", mmc->scr[0]);
	//printf("mmc->scr[1] = %08X\n", mmc->scr[1]);

	switch ((mmc->scr[0] >> 24) & 0xf) {
		case 0:
			mmc->version = SD_VERSION_1_0;
			break;
		case 1:
			mmc->version = SD_VERSION_1_10;
			break;
		case 2:
			mmc->version = SD_VERSION_2;
			break;
		default:
			mmc->version = SD_VERSION_1_0;
			break;
	}

	if(mmc->version==SD_VERSION_2)
	{
		if( mmc->scr[0] & 0x8000 ) {
			mmc->version = SD_VERSION_3;
		}
	}

	if (mmc->scr[0] & SD_DATA_4BIT)
		mmc->card_caps |= MMC_MODE_4BIT;

	/* Version 1.0 doesn't support switching */
	if (mmc->version == SD_VERSION_1_0)
		return 0;

	// check if SD card support high speed
	timeout = 4;
	while (timeout--) {
		err = sd_switch(mmc, SD_SWITCH_CHECK, SD_SW_GROUP_ACS_MODE, 1,	(u8 *)&switch_status);

		if (err) {
			printf("CMD6 check error\n");
			return err;
		}

		/* The high-speed function is busy.  Try again */
		if (!(__be32_to_cpu(switch_status[7]) & SD_HIGHSPEED_BUSY))
			break;
	}

	//printf("switch_status[3] = %08Xh\n", __be32_to_cpu(switch_status[3]));

	// SDR104 208MHz clock
	if (__be32_to_cpu(switch_status[3]) & SD_SDR104_SUPPORTED)
	{
		//printf("SDR104 ");
		mmc->card_caps |= SD30_MODE_SDR104;
	}

	// SDR50 100MHz clock
	if (__be32_to_cpu(switch_status[3]) & SD_SDR50_SUPPORTED)
	{
		//printf("SDR50 ");
		mmc->card_caps |= SD30_MODE_SDR50;
	}

	// DDR50 50MHz clock
	if (__be32_to_cpu(switch_status[3]) & SD_DDR50_SUPPORTED)
	{
		//printf("DDR50 ");
		mmc->card_caps |= SD30_MODE_DDR50;
	}

	// HS 50MHz clock
	if (__be32_to_cpu(switch_status[3]) & SD_HIGHSPEED_SUPPORTED)
	{
		//printf("SDR25");
		mmc->card_caps |= MMC_MODE_HS;
	}

	/* If high-speed isn't supported, we return */
	if (!(__be32_to_cpu(switch_status[3]) & SD_HIGHSPEED_SUPPORTED))
	{
		//printf("SDR12\n");
	}

	//printf("\n");

	return 0;
}

/* frequency bases */
/* divided by 10 to be nice to platforms without floating point */
static const int fbase[] = {
	10000,
	100000,
	1000000,
	10000000,
};

/* Multiplier values for TRAN_SPEED.  Multiplied by 10 to be nice
 * to platforms without floating point.
 */
static const int multipliers[] = {
	0,	/* reserved */
	10,
	12,
	13,
	15,
	20,
	25,
	30,
	35,
	40,
	45,
	50,
	55,
	60,
	70,
	80,
};

void mmc_set_ios(struct mmc *mmc)
{
	mmc->set_ios(mmc); // fcie_set_ios()
}

void mmc_set_clock(struct mmc *mmc, uint clock)
{
	if (clock > mmc->f_max)
		clock = mmc->f_max;

	if (clock < mmc->f_min)
		clock = mmc->f_min;

	mmc->clock = clock;

	mmc_set_ios(mmc);
}

void mmc_set_bus_width(struct mmc *mmc, uint width)
{
    //printf("mmc_set_bus_width(%d)\n", width);
	mmc->bus_width = width;

	mmc_set_ios(mmc); // fcie_set_ios()
}

int sd_set_bus_width(struct mmc * mmc, uint width)
{
	struct mmc_cmd cmd;
	int err;

	cmd.cmdidx = MMC_CMD_APP_CMD;
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;
	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err) return err;

	cmd.cmdidx = SD_CMD_APP_SET_BUS_WIDTH; // ACMD6 set 4 bits
	cmd.resp_type = MMC_RSP_R1;
	cmd.cmdarg = (width==4) ? 2 : 0;
	cmd.flags = 0;
	err = mmc_send_cmd(mmc, &cmd, NULL);
	if (err) return err;

	mmc_set_bus_width(mmc, width);

	return 0;

}

#ifdef SDIO_SUPPORT_SD30

static U32 gu32BestDqs;

unsigned char temp_buf[512] __attribute__((aligned(0x80)));

static const unsigned char sd_tunning_pattern[64] = {

	0xFF,0x0F,0xFF,0x00,  0xFF,0xCC,0xC3,0xCC,  0xC3,0x3C,0xCC,0xFF,  0xFE,0xFF,0xFE,0xEF,
    0xFF,0xDF,0xFF,0xDD,  0xFF,0xFB,0xFF,0xFB,  0xBF,0xFF,0x7F,0xFF,  0x77,0xF7,0xBD,0xEF,
    0xFF,0xF0,0xFF,0xF0,  0x0F,0xFC,0xCC,0x3C,  0xCC,0x33,0xCC,0xCF,  0xFF,0xEF,0xFF,0xEE,
    0xFF,0xFD,0xFF,0xFD,  0xDF,0xFF,0xBF,0xFF,  0xBB,0xFF,0xF7,0xFF,  0xF7,0x7F,0x7B,0xDE
};

#define DBG_SKEW_SCAN(MSG)             //MSG

int HalSdio_ScanSkew_SetBest(struct mmc *mmc)
{
	unsigned int u32_i;
	unsigned int u32_Skew4Result = 0;
	SKEWER stSkewer;
	int result;

	memset(&stSkewer, 0, sizeof(SKEWER));
	gu8SdioSilenceTuning = TUNE_SILENCE+TUNE_NOW;

	for(u32_i=0; u32_i<18; u32_i++)
	{
		mmc_set_ios(mmc); // need to config clock again
		HalSdio_SetSkew(4, u32_i);
		result = sd_tuning(mmc, temp_buf);
		if(result)
		{
			DBG_SKEW_SCAN(printf("CMD19 fail @ skew4 = %d %04Xh\n", u32_i, result));
		}

		if( (result==0) || (result==1) ) // success or only CRC error
		{
			if(memcmp((void*)sd_tunning_pattern, (void*)temp_buf, 64))
			{
				DBG_SKEW_SCAN(printf("tuning pattern compare error @ skew4 = %d\n", u32_i));
				//fcie_dump_mem(temp_buf, 64);
			}
			else
			{
				//printf("tuning pattern compare OK\n");
				u32_Skew4Result |= (1<<u32_i);
			}
		}
	}

	gu8SdioSilenceTuning = TUNE_FINISH;

	if(HalSdio_SlectBestPhase(u32_Skew4Result, &stSkewer))
	{
		printf(LIGHT_RED"SdErr: Select skew4, %04Xh, fail\n"NONE, u32_Skew4Result);
	}

	#if (defined PATCH_MINUS2PLUS2) && (PATCH_MINUS2PLUS2)
		stSkewer.u32LatchBest = HalSdio_Skew4MetaPatch(stSkewer.u32LatchBest);
	#endif

	gu32BestSdioSkew4 = stSkewer.u32LatchBest;

	HalSdio_SetSkew(4, gu32BestSdioSkew4);

	return stSkewer.u32LatchBest;
}

int HalSdio_ScanDqs_SetBest(struct mmc *mmc)
{
	unsigned int u32_i;
	unsigned int u32_Skew4Result = 0;
	SKEWER stSkewer;
	int result;

	memset(&stSkewer, 0, sizeof(SKEWER));
	gu8SdioSilenceTuning = TUNE_SILENCE+TUNE_NOW;

	for(u32_i=0; u32_i<8; u32_i++)
	{
		if(!(gu8SdioSilenceTuning&TUNE_SILENCE)) printf("DQS = %d\n", u32_i);

		mmc_set_ios(mmc); // need to config clock again

		HalSdio_SetDqs(u32_i);

		result = DrvSd_CMD17(0, (U8*)0x20400000);

		if(result==0) u32_Skew4Result |= (1<<u32_i);
	}

	gu8SdioSilenceTuning = TUNE_FINISH;

	if(HalSdio_SlectBestPhase(u32_Skew4Result, &stSkewer))
	{
		printf(LIGHT_RED"SdErr: Select dqs fail\n"NONE);
	}

	gu32BestDqs = stSkewer.u32LatchBest;

	HalSdio_SetDqs(gu32BestDqs);

	return u32_i;
}


int switch_2_sdr104(struct mmc *mmc) // 100MB/s, max clock: 208 MHz
{
	int err;
	//struct mmc * mmc = find_sd_device(curr_device);

	// set to SDR104
	//printf(LIGHT_CYAN"switch 2 SDR104\n"NONE);
	err = sd_switch(mmc, SD_SWITCH_SWITCH, SD_SW_GROUP_ACS_MODE, 3, (u8 *)&switch_status);
	if (err) return err;
	if ((__be32_to_cpu(switch_status[4]) & 0x0F000000) == 0x03000000)
	{
		mmc->op_mode = SD30_MODE_SDR104;
	}
	else
	{
		printf("%s fail, swsts = %08Xh\n", __FUNCTION__, (__be32_to_cpu(switch_status[4]) & 0x0F000000));
		return 1;
	}

	mmc_set_clock(mmc, 200000000);

 	HalSdio_ScanSkew_SetBest(mmc);

	return 0;
}

int switch_2_ddr50(struct mmc *mmc) // 50MB/s, max clock: 50 MHz
{
	int err;
	//struct mmc * mmc = find_sd_device(curr_device);

	// set to DDR50
	//printf(LIGHT_CYAN"switch 2 DDR50\n"NONE);
	err = sd_switch(mmc, SD_SWITCH_SWITCH, SD_SW_GROUP_ACS_MODE, 4, (u8 *)&switch_status);
	if (err) return err;
	if ((__be32_to_cpu(switch_status[4]) & 0x0F000000) == 0x04000000)
	{
		mmc->op_mode = SD30_MODE_DDR50;
	}
	else
	{
		printf("%s fail, swsts = %08Xh\n", __FUNCTION__, (__be32_to_cpu(switch_status[4]) & 0x0F000000));
		return 1;
	}

	mmc_set_clock(mmc, 50000000);

	HalSdio_ScanDqs_SetBest(mmc);

	return 0;
}

int switch_2_sdr50(struct mmc *mmc) // 50MB/s, max clock: 100 MHz
{
	int err;
	//struct mmc * mmc = find_sd_device(curr_device);

	// set to SDR50
	//printf(LIGHT_CYAN"switch 2 SDR50\n"NONE);
	err = sd_switch(mmc, SD_SWITCH_SWITCH, SD_SW_GROUP_ACS_MODE, 2, (u8 *)&switch_status);
	if (err) return err;
	if ((__be32_to_cpu(switch_status[4]) & 0x0F000000) == 0x02000000)
	{
		mmc->op_mode = SD30_MODE_SDR50;
	}
	else
	{
		printf("%s fail, swsts = %08Xh\n", __FUNCTION__, (__be32_to_cpu(switch_status[4]) & 0x0F000000));
		return 1;
	}

	mmc_set_clock(mmc, 100000000);

	HalSdio_ScanSkew_SetBest(mmc);

	return 0;
}

#endif


int switch_2_sdr25(struct mmc *mmc) // 25MB/s (legacy high speed), max clock: 50 MHz
{
	int err;
	//struct mmc * mmc = find_sd_device(curr_device);

	// switch SD card to high speed
	//printf(LIGHT_CYAN"switch 2 high speed\n"NONE);
	err = sd_switch(mmc, SD_SWITCH_SWITCH, SD_SW_GROUP_ACS_MODE, 1, (u8 *)&switch_status);
	if (err) return err;
	if ((__be32_to_cpu(switch_status[4]) & 0x0F000000) == 0x01000000)
	{
		mmc->op_mode = MMC_MODE_HS;
	}
	else
	{
		printf("%s fail, swsts = %08Xh\n", __FUNCTION__, (__be32_to_cpu(switch_status[4]) & 0x0F000000));
		return 1;
	}

	mmc_set_clock(mmc, 50000000); // 50MHz

	return 0;
}

int switch_2_sdr12(struct mmc *mmc) // 12MB/s (legacy normal speed), max clock: 25 MHz
{
	int err;
	//struct mmc * mmc = find_sd_device(curr_device);

	// switch SD card to normal speed
	//printf(LIGHT_CYAN"switch 2 normal speed\n"NONE);
	err = sd_switch(mmc, SD_SWITCH_SWITCH, SD_SW_GROUP_ACS_MODE, 0, (u8 *)&switch_status);
	if (err) return err;
	if ((__be32_to_cpu(switch_status[4]) & 0x0F000000) == 0x00000000)
	{
		mmc->op_mode = 0;
	}
	else
	{
		printf("%s fail, swsts = %08Xh\n", __FUNCTION__, (__be32_to_cpu(switch_status[4]) & 0x0F000000));
		return 1;
	}

	mmc_set_clock(mmc, 25000000); // 50MHz

	return 0;
}


int mmc_startup(struct mmc *mmc)
{
	int err;
	uint mult, freq;
	u64 cmult, csize;
	struct mmc_cmd cmd;
	char ext_csd[512];
	//int timeout = 1000;

#ifdef CONFIG_MMC_SPI_CRC_ON
	if (mmc_host_is_spi(mmc)) { /* enable CRC check for spi */
		cmd.cmdidx = MMC_CMD_SPI_CRC_ON_OFF;
		cmd.resp_type = MMC_RSP_R1;
		cmd.cmdarg = 1;
		cmd.flags = 0;
		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;
	}
#endif

	/* Put the Card in Identify Mode */
	cmd.cmdidx = mmc_host_is_spi(mmc) ? MMC_CMD_SEND_CID :
		MMC_CMD_ALL_SEND_CID; /* cmd not supported in spi */
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmdarg = 0;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	memcpy(mmc->cid, cmd.response, 16);

	/*
	 * For MMC cards, set the Relative Address.
	 * For SD cards, get the Relatvie Address.
	 * This also puts the cards into Standby State
	 */
	if (!mmc_host_is_spi(mmc)) { /* cmd not supported in spi */
		cmd.cmdidx = SD_CMD_SEND_RELATIVE_ADDR;
		cmd.cmdarg = mmc->rca << 16;
		cmd.resp_type = MMC_RSP_R6;
		cmd.flags = 0;

		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;

		if (IS_SD(mmc))
			mmc->rca = (cmd.response[0] >> 16) & 0xffff;
	}

	/* Get the Card-Specific Data */
	cmd.cmdidx = MMC_CMD_SEND_CSD;
	cmd.resp_type = MMC_RSP_R2;
	cmd.cmdarg = mmc->rca << 16;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

#if 0
	/* Waiting for the ready status */
	mmc_send_status(mmc, timeout);

	if (err)
		return err;
#endif
	mmc->csd[0] = cmd.response[0];
	mmc->csd[1] = cmd.response[1];
	mmc->csd[2] = cmd.response[2];
	mmc->csd[3] = cmd.response[3];

	if (mmc->version == MMC_VERSION_UNKNOWN) {
		int version = (cmd.response[0] >> 26) & 0xf;

		switch (version) {
			case 0:
				mmc->version = MMC_VERSION_1_2;
				break;
			case 1:
				mmc->version = MMC_VERSION_1_4;
				break;
			case 2:
				mmc->version = MMC_VERSION_2_2;
				break;
			case 3:
				mmc->version = MMC_VERSION_3;
				break;
			case 4:
				mmc->version = MMC_VERSION_4;
				break;
			default:
				mmc->version = MMC_VERSION_1_2;
				break;
		}
	}

	/* divide frequency by 10, since the mults are 10x bigger */
	freq = fbase[(cmd.response[0] & 0x7)];
	mult = multipliers[((cmd.response[0] >> 3) & 0xf)];

	mmc->tran_speed = freq * mult;

	mmc->read_bl_len = 1 << ((cmd.response[1] >> 16) & 0xf);

	if (IS_SD(mmc))
		mmc->write_bl_len = mmc->read_bl_len;
	else
		mmc->write_bl_len = 1 << ((cmd.response[3] >> 22) & 0xf);

	if (mmc->high_capacity) {
		csize = (mmc->csd[1] & 0x3f) << 16
			| (mmc->csd[2] & 0xffff0000) >> 16;
		cmult = 8;
	} else {
		csize = (mmc->csd[1] & 0x3ff) << 2
			| (mmc->csd[2] & 0xc0000000) >> 30;
		cmult = (mmc->csd[2] & 0x00038000) >> 15;
	}

	//printf("csize = %llu\n", csize);
	//printf("cmult = %llu\n", cmult);
	//printf("mmc->read_bl_len = %u\n", mmc->read_bl_len);

	mmc->capacity = (csize + 1) << (cmult + 2);
	//printf("1. mmc->capacity = [%llXh]\n", mmc->capacity);

	mmc->capacity *= mmc->read_bl_len;
	//printf("2. mmc->capacity = [%llXh]\n", mmc->capacity);

	if (mmc->read_bl_len > 512)
		mmc->read_bl_len = 512;

	if (mmc->write_bl_len > 512)
		mmc->write_bl_len = 512;

	/* Select the card, and put it into Transfer Mode */
	if (!mmc_host_is_spi(mmc)) { /* cmd not supported in spi */
		cmd.cmdidx = MMC_CMD_SELECT_CARD;
		cmd.resp_type = MMC_RSP_R1b;
		cmd.cmdarg = mmc->rca << 16;
		cmd.flags = 0;
		err = mmc_send_cmd(mmc, &cmd, NULL);

		if (err)
			return err;
	}

	mmc->part_config = MMCPART_NOAVAILABLE;
	if (!IS_SD(mmc) && (mmc->version >= MMC_VERSION_4)) {
		/* check  ext_csd version and capacity */
		err = sd_send_ext_csd(mmc, ext_csd);
		if (!err & (ext_csd[192] >= 2)) {
			mmc->capacity = ext_csd[212] << 0 | ext_csd[213] << 8 |
					ext_csd[214] << 16 | ext_csd[215] << 24;
			mmc->capacity *= 512;
		}

		/* store the partition info of emmc */
		if (ext_csd[160] & PART_SUPPORT)
			mmc->part_config = ext_csd[179];
	}

	if (IS_SD(mmc))
	{
		err = sd_change_freq(mmc);
		// 1. ACMD51 to read SCR for checking SD SPEC version
		// 2. CMD6 to check if higher speed supported ?
	}
	else
	{
		err = mmc_change_freq(mmc);
	}

	if (err)
		return err;

	/* Restrict card's capabilities by what the host can do */
	//printf("mmc->card_caps = %08Xh\n", mmc->card_caps);
	//printf("mmc->host_caps = %08Xh\n", mmc->host_caps);

	// TODO: need to edit host_caps
	//mmc->card_caps &= mmc->host_caps;

	//printf("mmc start up\n");

	if (IS_SD(mmc))
	{
		if (mmc->card_caps & MMC_MODE_4BIT) {
			if(sd_set_bus_width(mmc, 4)) {
				printf("SdErr: switch 2 4 bits bus fail\n");
			}
		}

		#ifdef SDIO_SUPPORT_SD30

		#if 1 // disable SDR104 to run DDR in UHS-I card
		if( (mmc->volt_sw_18v) && (mmc->card_caps & SD30_MODE_SDR104) )
		{
			switch_2_sdr104(mmc);
		}
		else
		#endif
		if( (mmc->volt_sw_18v) && (mmc->card_caps & SD30_MODE_DDR50) )
		{
			switch_2_ddr50(mmc);
		}
		else if( (mmc->volt_sw_18v) && (mmc->card_caps & SD30_MODE_SDR50) )
		{
			switch_2_sdr50(mmc);
		}
		else

		#endif

		if (mmc->card_caps & MMC_MODE_HS)
		{
			switch_2_sdr25(mmc);
		}
		else
		{
			printf(LIGHT_CYAN"normal speed, not switch\n"NONE);
			mmc_set_clock(mmc, 25000000); // 25MHz
		}

	}
	else
	{
		// printf("MMC card\n");
		if (mmc->card_caps & MMC_MODE_4BIT) {
			/* Set the card to use 4 bit*/
			err = sd_mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_BUS_WIDTH,
					EXT_CSD_BUS_WIDTH_4);

			if (err)
				return err;

			mmc_set_bus_width(mmc, 4);
		} else if (mmc->card_caps & MMC_MODE_8BIT) {
			/* Set the card to use 8 bit*/
			err = sd_mmc_switch(mmc, EXT_CSD_CMD_SET_NORMAL,
					EXT_CSD_BUS_WIDTH,
					EXT_CSD_BUS_WIDTH_8);

			if (err)
				return err;

			mmc_set_bus_width(mmc, 8);
		}

		if (mmc->card_caps & MMC_MODE_HS) {
			if (mmc->card_caps & MMC_MODE_HS_52MHz)
				mmc_set_clock(mmc, 52000000);
			else
				mmc_set_clock(mmc, 26000000);
		} else
			mmc_set_clock(mmc, 20000000);
	}

	if(mmc->capacity > 1UL<<30) // GB
	{
		printf("Find SDHC ");
		switch(mmc->op_mode)
		{
			case SD30_MODE_SDR104:
				printf("SDR104");
				break;
			case SD30_MODE_DDR50:
				printf("DDR50");
				break;
			case SD30_MODE_SDR50:
				printf("SDR50");
				break;
			case MMC_MODE_HS:
				printf("SDR25");
				break;
			default:
				printf("SDR12");
				break;
		}
		printf(" %llu.%llu GB [%llXh]\n", mmc->capacity>>30, (mmc->capacity&0x3FFFFFFFULL)>>20, mmc->capacity>>9);
	}
	else
	{
		printf("Find SD %llu MB [%llXh]\n", mmc->capacity>>20, mmc->capacity>>9);
	}

	/* fill in device description */
	mmc->block_dev.lun = 0;
	mmc->block_dev.type = 0;
	mmc->block_dev.blksz = mmc->read_bl_len;
	mmc->block_dev.lba = mmc->capacity / mmc->read_bl_len;

#if (!defined FPGA_BOARD) || (FPGA_BOARD == 0)

//	sprintf(mmc->block_dev.vendor, "Man %06x Snr %08x", mmc->cid[0] >> 8,
	//		(mmc->cid[2] << 8) | (mmc->cid[3] >> 24));
//	sprintf(mmc->block_dev.product, "%c%c%c%c%c", mmc->cid[0] & 0xff,
		//	(mmc->cid[1] >> 24), (mmc->cid[1] >> 16) & 0xff,
		//	(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff);
//	sprintf(mmc->block_dev.revision, "%d.%d", mmc->cid[2] >> 28,
	//		(mmc->cid[2] >> 24) & 0xf);

//	init_part(&mmc->block_dev);

#endif

	return 0;
}

int mmc_send_if_cond(struct mmc *mmc)
{
	struct mmc_cmd cmd;
	int err;

	cmd.cmdidx = SD_CMD_SEND_IF_COND;
	/* We set the bit if the host supports voltages between 2.7 and 3.6 V */
	cmd.cmdarg = ((mmc->voltages & 0xff8000) != 0) << 8 | 0xaa;
	cmd.resp_type = MMC_RSP_R7;
	cmd.flags = 0;

	err = mmc_send_cmd(mmc, &cmd, NULL);

	if (err)
		return err;

	if ((cmd.response[0] & 0xff) != 0xaa)
		return UNUSABLE_ERR;
	else
		mmc->version = SD_VERSION_2;

	return 0;
}

int sd_register(struct mmc *mmc)
{
	/* Setup the universal parts of the block interface just once */
	mmc->block_dev.if_type = IF_TYPE_SD;
	mmc->block_dev.dev = cur_sd_dev_num++;
	mmc->block_dev.removable = 1;
	mmc->block_dev.block_read = mmc_bread;
	mmc->block_dev.block_write = mmc_bwrite;
	if (!mmc->b_max)
		mmc->b_max = CONFIG_SYS_MMC_MAX_BLK_COUNT;

	#ifndef FPGA_BOARD
		INIT_LIST_HEAD (&mmc->link);
		list_add_tail (&mmc->link, &mmc_devices);
	#endif

	return 0;
}

block_dev_desc_t *sd_get_dev(int dev)
{
	struct mmc *mmc = find_sd_device(dev);

	return mmc ? &mmc->block_dev : NULL;
}

int sd_init(struct mmc *mmc)
{
	int err;

	if (mmc->has_init)
		return 0;

	mmc->op_mode = 0; // start from default speed

	/*err = mmc->init(mmc);

	if (err)
		return err;*/

	#ifndef FPGA_BOARD
		#ifdef SDIO_SUPPORT_SD30
			HalSdio_SetCardPower(0); // power off
			HalSdio_SetIOVoltage(0); 	// turn off IO voltage LDO
			HalSdio_DelaySecond(2); // 2 seconds
			HalSdio_SetCardPower(1); // power on
			HalSdio_SetIOVoltage(1); // 3.3V IO voltage
			udelay(10000);				// delay 10 ms
		#else
			HalSdio_SetCardPower(0); // power off
			udelay(1000000);udelay(1000000); // delay 500 ms
			HalSdio_SetCardPower(1); // power on
			udelay(10000);				// delay 10 ms
		#endif
	#endif

	mmc_set_bus_width(mmc, 1);
	mmc_set_clock(mmc, 1);

	/* Reset the Card */
	err = mmc_go_idle(mmc);
	if (err)
		return err;

	/* The internal partition reset to user partition(0) at every CMD0*/
	mmc->part_num = 0;

	/* Test for SD version 2 */
	err = mmc_send_if_cond(mmc);

	/* Now try to get the SD card's operating condition */
	err = sd_send_op_cond(mmc);

	/* If the command timed out, we check for an MMC card */
	if (err == TIMEOUT) {
		err = mmc_send_op_cond(mmc);

		if (err) {
			printf("Card did not respond to voltage select!\n");
			return UNUSABLE_ERR;
		}
	}
	else if(err) { // other error
		printf("sd_send_op_cond() get %d\n", err);
		return UNUSABLE_ERR;
	}

	#ifdef SDIO_SUPPORT_SD30

	if(mmc->volt_sw_18v)
	{
		sd_voltage_switch(mmc); // CMD11

		//printf("volt_sw_18v switch to 1.8\n");

		#ifndef FPGA_BOARD
			HalSdio_SetIOVoltage(2); // 1.8V
			udelay(5000); // 5ms
		#else
			printf("switch FPGA daughter board to 1.8 in 5 sec\n");
			printf("5 "); HalSdioDelayMs(1000);
			printf("4 "); HalSdioDelayMs(1000);
			printf("3 "); HalSdioDelayMs(1000);
			printf("2 "); HalSdioDelayMs(1000);
			printf("1\n"); HalSdioDelayMs(1000);
		#endif

		if(HalSdio_SendClocknWaitD0High())
		{
			printf("Data 0 not high after switch 1.8V\n");
			return 1;
		}
	}
	else
	{
		//printf("no volt_sw_18v\n");
	}

	#endif

	err = mmc_startup(mmc);
	if (err)
		mmc->has_init = 0;
	else
		mmc->has_init = 1;
	return err;
}

#ifndef FPGA_BOARD
/*
 * CPU and board-specific MMC initializations.  Aliased function
 * signals caller to move on
 */
static int __def_mmc_init(bd_t *bis)
{
	return -1;
}

int cpu_mmc_init(bd_t *bis) __attribute__((weak, alias("__def_mmc_init")));
//int board_sd_init(bd_t *bis) __attribute__((weak, alias("__def_mmc_init")));

void print_sd_devices(char separator)
{
	struct mmc *m;
	struct list_head *entry;

	list_for_each(entry, &mmc_devices) {
		m = list_entry(entry, struct mmc, link);

		printf("%s: %d", m->name, m->block_dev.dev);

		if (entry->next != &mmc_devices)
			printf("%c ", separator);
	}

	printf("\n");
}
#endif

int get_sd_num(void)
{
	return cur_sd_dev_num;
}

int sd_initialize(bd_t *bis)
{
	#ifndef FPGA_BOARD
	INIT_LIST_HEAD (&mmc_devices);
	#endif

	cur_sd_dev_num = 0;

	printf(YELLOW"build time %s %s\n"NONE, __DATE__, __TIME__);

	board_sd_init(bis);

	#ifndef FPGA_BOARD
		print_sd_devices(',');
	#endif

	return 0;
}

