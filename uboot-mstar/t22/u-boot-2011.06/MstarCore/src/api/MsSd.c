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

//#include <config.h>
#include <common.h>
#include <mmc.h>
#include "DrvSd.h"
#include "hal_sdio.h"

static int curr_device = -1;
extern void sd_r2n_read_write_test(struct mmc * mmc);


static void print_sdinfo(struct mmc *mmc)
{
	printf("Device: %s\n", mmc->name);
	printf("Manufacturer ID: %x\n", mmc->cid[0] >> 24);
	printf("OEM: %x\n", (mmc->cid[0] >> 8) & 0xffff);
	printf("Name: %c%c%c%c%c \n", mmc->cid[0] & 0xff,
			(mmc->cid[1] >> 24), (mmc->cid[1] >> 16) & 0xff,
			(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff);

	//printf("Tran Speed: %d\n", mmc->tran_speed);
	printf("Rd Block Len: %d\n", mmc->read_bl_len);

	printf("%s version %d.%d\n", IS_SD(mmc) ? "SD" : "MMC",
			(mmc->version >> 4) & 0xf, mmc->version & 0xf);
	printf("Operation mode: ");
	switch(mmc->op_mode)
	{
		case SD30_MODE_SDR104:
			printf("SDR104\n");
			break;
		case SD30_MODE_DDR50:
			printf("DDR50\n");
			break;
		case SD30_MODE_SDR50:
			printf("SDR50\n");
			break;
		case MMC_MODE_HS:
			printf("SDR25\n");
			break;
		default:
			printf("SDR12\n");
			break;
	}
	printf("clock: %d Hz\n", mmc->clock);
	printf("RCA: %04Xh\n", mmc->rca);
	printf("High Capacity: %s\n", mmc->high_capacity ? "Yes" : "No");
	puts("Capacity: ");
	print_size(mmc->capacity, "\n");

	printf("Bus Width: %d-bit\n", mmc->bus_width);
}


int do_MsSd(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	struct mmc *mmc;
	//static unsigned char debug = 0;

	if (argc < 2)
		return cmd_usage(cmdtp);

	if (curr_device < 0) {
		if (get_sd_num() > 0)
			curr_device = 0;
		else {
			puts("No MMC device available\n");
			return 1;
		}
	}

	mmc = find_sd_device(curr_device);
	if (!mmc) {
		printf("no mmc device at slot %x\n", curr_device);
		return 1;
	}

	// sd command parser

	if (strcmp(argv[1], "rescan") == 0)
	{
		int i, loop = 1;
		if (argc == 3)
		{
			loop = simple_strtoul(argv[2], NULL, 10);
		}
		for(i=0; i<loop; i++)
		{
			mmc->has_init = 0;
			sd_init(mmc);
		}
	}
	else if(strcmp(argv[1], "info") == 0)
	{
		print_sdinfo(mmc);
	}
	else if (strcmp(argv[1], "list") == 0)
	{
		print_sd_devices('\n');
	}
	else if (strcmp(argv[1], "dev") == 0)
	{
		int dev, part = -1;

		if (argc == 2)
		{
			dev = curr_device;
		}
		else if (argc == 3)
		{
			dev = simple_strtoul(argv[2], NULL, 10);
		}
		else if (argc == 4)
		{
			dev = (int)simple_strtoul(argv[2], NULL, 10);
			part = (int)simple_strtoul(argv[3], NULL, 10);
			if (part > PART_ACCESS_MASK) {
				printf("#part_num shouldn't be larger"
					" than %d\n", PART_ACCESS_MASK);
				return 1;
			}
		}
		else
		{
			return cmd_usage(cmdtp);
		}

		//sd_init(mmc);
		if (part != -1) {
			int ret;
			if (mmc->part_config == MMCPART_NOAVAILABLE) {
				printf("Card doesn't support part_switch\n");
				return 1;
			}

			if (part != mmc->part_num) {
				ret = sd_switch_part(dev, part);
				if (!ret)
					mmc->part_num = part;

				printf("switch to partions #%d, %s\n",
						part, (!ret) ? "OK" : "ERROR");
			}
		}
		curr_device = dev;
		if (mmc->part_config == MMCPART_NOAVAILABLE)
			printf("sd%d is current device\n", curr_device);
		else
			printf("sd%d(part %d) is current device\n",
				curr_device, mmc->part_num);

		return 0;
	}
	else if (strncmp(argv[1], "part", 4) == 0)
	{
		block_dev_desc_t *mmc_dev;

		mmc_dev = sd_get_dev(curr_device);
		if (mmc_dev != NULL && mmc_dev->type != DEV_TYPE_UNKNOWN) {
			print_part(mmc_dev);
		}
		else {
			puts("get mmc type error!\n");
		}
	}
	else if (strcmp(argv[1], "read") == 0)
	{
		void * addr;
		u32 cnt;
		u32 n;
		u32 blk;

		if(argc!=5)
		{
			return cmd_usage(cmdtp);
		}

		addr = (void *)simple_strtoul(argv[2], NULL, 16);
		cnt = simple_strtoul(argv[4], NULL, 16);
		blk = simple_strtoul(argv[3], NULL, 16);

		if( (((u32)addr&0xF0000000)!=0x20000000) && (((u32)addr&0xF0000000)!=0xA0000000) )
		{
			printf("wrong memeory addr parameter!\n");
			return 1;
		}

		printf("SD read: dev # %d, block # %d, count %d ... ",
				curr_device, blk, cnt);

		//sd_init(mmc);

		n = mmc->block_dev.block_read(curr_device, blk, cnt, addr);

		/* flush cache after read */
		flush_cache((ulong)addr, cnt * 512); /* FIXME */

		printf("%d blocks read: %s\n", n, (n==cnt) ? "OK" : "ERROR");

		if(n!=cnt) return 1;

	}
	else if (strcmp(argv[1], "write") == 0)
	{
		void *addr = (void *)simple_strtoul(argv[2], NULL, 16);
		u32 cnt = simple_strtoul(argv[4], NULL, 16);
		u32 n;

		int blk = simple_strtoul(argv[3], NULL, 16);


		printf("SD write: dev # %d, block # %d, count %d ... ", curr_device, blk, cnt);

		//sd_init(mmc);

		n = mmc->block_dev.block_write(curr_device, blk, cnt, addr);

		printf("%d blocks written: %s\n", n, (n == cnt) ? "OK" : "ERROR");

		if(n!=cnt) return 1;
	}
	else if (strcmp(argv[1], "speed") == 0)
	{
		unsigned int micro_sec = 0;
		unsigned int sector_index;
		unsigned int sector_num;
		unsigned int sector_total;
		unsigned int n;
		unsigned int speed;
		unsigned char rw;

		printf("It will take several minutes to run...\nNotice that this is pure IO performance!\n");
		HalSdio_TimerTest();

		for(rw=0; rw<2; rw++)
		{
			if(rw==0) // read
			{
				sector_total = 122880; // 60MB
			}
			else // write
			{
				sector_total = 61440; // 30MB
			}

			printf("\n");
			//for(sector_num = 16; sector_num <= 128; sector_num<<=1 )
			for(sector_num = 8; sector_num <= 32768; sector_num<<=1 )
			{
				printf("\tBurstUnit: %5d KB, ", sector_num/2);

				HalSdio_TimerStart();
				for(sector_index = 100000; sector_index<(100000+sector_total); sector_index+=sector_num)
				{
					//printf("sector_index = %d\n", sector_index);
					if(rw==0)
					{
						n = mmc->block_dev.block_read(curr_device, sector_index, sector_num, (void*)0x20400000);
						if(n!=sector_num) {printf("read %d fail %d!\n", sector_num, n); break;}
					}
					else
					{
						n = mmc->block_dev.block_write(curr_device, sector_index, sector_num, (void*)0x20400000);
						if(n!=sector_num) {printf("write fail!\n"); break;}
					}
				}
				micro_sec = HalSdio_TimerStop();
				speed = ((sector_total>>11)*1000000) / (micro_sec/100);

				printf("%s %d MB use %6d ms, speed = %2d.%02d MByte/s\n", (rw==0)?"R":"W", sector_total>>11, micro_sec/1000, speed/100, speed%100);
			}
		}

	}

	else if (strcmp(argv[1], "status") == 0)
	{
		DrvSd_CMD13(mmc->rca);
	}
	/*else if (strcmp(argv[1], "dbg") == 0)
	{
		debug ^= 1;
		if(debug) {
			printf("Enable sdcard debug mode\n");
		}
		else {
			printf("Disable sdcard debug mode\n");
		}
	}*/
	else if (strcmp(argv[1], "burning") == 0)
	{
		u32 u32Loop = simple_strtoul(argv[2], NULL, 10);
		mmc_read_write_test(curr_device, u32Loop);
	}
	else if (strcmp(argv[1], "mode") == 0)
	{
		mmc_read_write_test_each_op_mode(curr_device);
	}
	else if (strcmp(argv[1], "reg") == 0)
	{
		if(argc==3)
		{
			u16 BankNo = simple_strtoul(argv[2], NULL, 16);
			HalSdio_DumpRegisterBank(BankNo, 0,0x80);
		}
		else if(argc==2)
		{
			HalSdio_DumpRegister();
		}
	}
	#ifdef SDIO_IP_VERIFY
	else if (strcmp(argv[1], "tune") == 0)
	{
		const u32 blk = 10000; // notice! hardcode block address here . . .
		const u32 cnt = 2;
		void * addr = (void *)0x20400000;
		u32 n = 0;
		u8 u8_i = 0;
		u8 Ret[16] = {0};
		u8 u8Type = 0;

		//sd_init(mmc);

		gu8SdioSilenceTuning = 1;

		if (strncmp(argv[2], "rsp", 3) == 0)
		{
			//   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
			//  NG  OK  OK  OK  OK  OK  OK  OK  NG  NG  NG  NG  NG  NG  NG  NG
			u8Type = 1;
		}
		else if (strncmp(argv[2], "rstop", 5) == 0)
		{
			//   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
			//  NG  NG  NG  NG  NG  NG  NG  NG  NG  NG  NG  NG  OK  OK  OK  OK
			u8Type = 2;
		}
		else if (strncmp(argv[2], "wrsts", 5) == 0)
		{
			//   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
			//  OK  OK  OK  OK  OK  OK  OK  OK  OK  NG  NG  NG  NG  NG  NG  NG
			u8Type = 3;
		}
		else
		{
			return cmd_usage(cmdtp);
		}

		for(u8_i=0; u8_i<16; u8_i++)
		{
			//mmc->has_init = 0; sd_init(mmc); // init again for clean enviroment
			HalSdio_SetShiftTune(1, u8Type, u8_i);
			if(u8Type!=3)
			{
				n = mmc->block_dev.block_read(curr_device, blk, cnt, addr);
			}
			else
			{
				n = mmc->block_dev.block_write(curr_device, blk, cnt, addr);
			}
			if(n==cnt) Ret[u8_i] = 1;
			HalSdio_SetShiftTune(0, u8Type, 0); // restore back for init again
		}

		for(u8_i=0; u8_i<16; u8_i++)
		{
			printf("  %2d", u8_i);
		}
		printf("\n");
		for(u8_i=0; u8_i<16; u8_i++)
		{
			printf("  %s", Ret[u8_i] ? "OK" : "NG");
		}
		printf("\n");

		gu8SdioSilenceTuning = TUNE_FINISH;

	}
	else if (strcmp(argv[1], "bist") == 0)
	{
		sd_bist_test_n_check_CRC_value();
	}
	else if (strcmp(argv[1], "pwrsave") == 0)
	{
		return HalSdio_PowerSaveMode();
	}
	else if (strcmp(argv[1], "d1intr") == 0)
	{
		HalSdio_TestDataOneIntr();
	}
	else if (strcmp(argv[1], "adma") == 0)
	{
		sd_adma_read_write_test();
	}
	else if (strcmp(argv[1], "r2n") == 0)
	{
		sd_r2n_read_write_test(mmc);
	}
	else if (strcmp(argv[1], "test") == 0)
	{
		unsigned int n = 0;
		unsigned short u16_pattern[16] = {0};
		unsigned int u32PatternNum = 0;
		unsigned short * pTestBuf = (unsigned short *)0x20400000;
		unsigned int i;

		if(strcmp(argv[2], "wo") == 0)
		{
			if( argc<4 || argc>19 ) return cmd_usage(cmdtp); // check at least one pattern given

			u32PatternNum = argc-3;
			//printf("u32PatternNum = %d\n", u32PatternNum);
			for(i=0; i<u32PatternNum; i++)
			{
 				u16_pattern[i] = simple_strtoul(argv[3+i], NULL, 16);
				//printf("u16_pattern[%d] = %04Xh\n", i, u16_pattern[i]);
			}
			for(i=0; i<256; i++)
			{
				*(pTestBuf+i) = u16_pattern[n++];
				if((n+1)>u32PatternNum) n = 0;
			}

			printf("the test pattern you are using:\n");
			fcie_dump_mem((unsigned char *)pTestBuf, 512);

		}

		while(1)
		{
			if(strcmp(argv[2], "ro") == 0)
			{
				n = mmc->block_dev.block_read(curr_device, 100000, 128, (void*)0x20400000);
				printf("%d blocks read: %s\r", n, (n == 128) ? "OK" : "ERROR");
			}
			else if(strcmp(argv[2], "wo") == 0)
			{
				n = mmc->block_dev.block_write(curr_device, 100000, 128, (void*)0x20400000);
				printf("%d blocks written: %s\r", n, (n == 128) ? "OK" : "ERROR");
			}
			if(n!=128) break;
  		}
	}
	else if (strcmp(argv[1], "driving") == 0)
	{
		unsigned short u16DrivingLevel;
		u16DrivingLevel = simple_strtoul(argv[2], NULL, 10);
		if(u16DrivingLevel>7) return cmd_usage(cmdtp);
		HalSdio_SetDrivingStrength(u16DrivingLevel);
	}
	else if (strcmp(argv[1], "ed") == 0)
	{
		// 0: rdcrc	 -> read crc error
		// 1: wcrc	 -> write crc
		// 2: wto	 -> write time out result by MIU side
		// 3: rspto	 -> response timeout / no response
		// 4: rspcrc -> response crc error
		// 5: rdto	 -> read timeout

		if(strcmp(argv[2], "rdcrc") == 0)
		{
			sd_rw_timeout_error_detect(0);
		}
		else if(strcmp(argv[2], "wcrc") == 0)
		{
			sd_rw_timeout_error_detect(1);
		}
		else if(strcmp(argv[2], "wto") == 0)
		{
			sd_rw_timeout_error_detect(2);
		}
		else if(strcmp(argv[2], "rspto") == 0)
		{
			sd_rw_timeout_error_detect(3);
		}
		else if(strcmp(argv[2], "rspcrc") == 0)
		{
			sd_rw_timeout_error_detect(4);
		}
		else if(strcmp(argv[2], "rdto") == 0)
		{
			sd_rw_timeout_error_detect(5);
		}
		else
		{
			return cmd_usage(cmdtp);
		}
	}
	#ifdef SDIO_MCM_TEST
	else if (strcmp(argv[1], "mcm") == 0)
	{
		if(argc!=3)
		{
			return cmd_usage(cmdtp);
		}
		else
		{
			unsigned char ratio;
			ratio = simple_strtoul(argv[2], NULL, 10);
			printf("Slow down ratio: %d\n", ratio);
			HalSdio_McmSlowDownRatio(ratio);
		}
	}
	#endif
	#ifdef SDIO_MIU_EFFICIENCY_TEST
	else if (strcmp(argv[1], "miueffi") == 0)
	{
		U8 u8BurstLength;
		U32 u32BlockCount;
		static U16 u16WorseEffi = 0xFFFF;

		for(u8BurstLength=1; u8BurstLength<=8; u8BurstLength=u8BurstLength<<1)
		{
			HalSdio_SetupMiuEfficiencyMeasure();
			HalSdio_SetMiuBurstLength(u8BurstLength);
			u16WorseEffi = HalSdio_GetMiuEfficiencyResult();
			//printf("reset u16WorseEffi to %d\n", u16WorseEffi);
			for(u32BlockCount=1; u32BlockCount<100000; u32BlockCount+=1000)
			{
 				mmc->block_dev.block_read(curr_device, 100000, u32BlockCount, (void*)0x20400000);
				if(u16WorseEffi>HalSdio_GetMiuEfficiencyResult())
				{
					u16WorseEffi = HalSdio_GetMiuEfficiencyResult();
				}
			}
			printf("R BurstLength %d, Efficincy: %d\n", u8BurstLength, u16WorseEffi);
		}
		for(u8BurstLength=1; u8BurstLength<=8; u8BurstLength=u8BurstLength<<1)
		{
			HalSdio_SetupMiuEfficiencyMeasure();
			HalSdio_SetMiuBurstLength(u8BurstLength);
			u16WorseEffi = HalSdio_GetMiuEfficiencyResult();
			//printf("reset u16WorseEffi to %d\n", u16WorseEffi);
			for(u32BlockCount=1; u32BlockCount<100000; u32BlockCount+=1000)
			{
 				mmc->block_dev.block_write(curr_device, 100000, u32BlockCount, (void*)0x20400000);
				if(u16WorseEffi>HalSdio_GetMiuEfficiencyResult())
				{
					u16WorseEffi = HalSdio_GetMiuEfficiencyResult();
				}
			}
			printf("W BurstLength %d, Efficincy: %d\n", u8BurstLength, u16WorseEffi);
		}
	}
	#endif
	#ifdef SDIO_SUPPORT_SD30
	else if (strcmp(argv[1], "skew") == 0)
	{
		if(argc!=4)
		{
			return cmd_usage(cmdtp);
		}
		else
		{
			unsigned char u8SkewIdx;
			unsigned int u32SkewValue;

			u8SkewIdx = simple_strtoul(argv[2], NULL, 10);
			u32SkewValue = simple_strtoul(argv[3], NULL, 10);
			HalSdio_SetSkew(u8SkewIdx, u32SkewValue);
		}
	}
	else if (strcmp(argv[1], "scanphase") == 0)
	{
		unsigned int n;
		unsigned short u16_pattern;
		unsigned char result[18][18];// = {0};
		unsigned char skew1 = 0, skew1_start = 0, skew1_end = 18, skew4 = 0, trig_level = 0;
		unsigned char u8RescanAfterFail = 0;
		const unsigned int TEST_BLOCK_COUNT = 1;
		// If use more than 1 block count, CMD12 might fail while scanning. It will result all fail
		// case and can not communicate with SD anymore. We can restore back skew1 & skew4 when CMD25
		// CMD18 fail, or set block count to 1.

		if(strcmp(argv[2], "read") == 0)
		{
			if(strcmp(argv[3], "rescan") == 0)
			{
				u8RescanAfterFail = 1;
			}

			gu8SdioSilenceTuning = (strcmp(argv[3], "log") == 0) ? TUNE_NOW:(TUNE_SILENCE+TUNE_NOW);

			for(trig_level=0; trig_level<8; trig_level++)
			{
				if(!(gu8SdioSilenceTuning&TUNE_SILENCE)) printf("trig_level = %d\n", trig_level);
				for(skew4=0; skew4<18; skew4++)
				{
					HalSdio_SetTriggerLevel(trig_level);
					HalSdio_SetSkew(4, skew4);
					n = mmc->block_dev.block_read(curr_device, 100000, TEST_BLOCK_COUNT, (void*)0x20400000);
					if(n == TEST_BLOCK_COUNT)
					{
						result[trig_level][skew4] = 0; // OK
					}
					else
					{
						result[trig_level][skew4] = 1; // NG
						if(u8RescanAfterFail)
						{
							mmc->has_init = 0; sd_init(mmc); // init again for clean enviroment
							gu8SdioSilenceTuning = TUNE_SILENCE+TUNE_NOW;
						}
					}
				}
			}

			gu8SdioSilenceTuning = TUNE_FINISH;

			printf("TL\\S4: ");
			for(skew4=0; skew4<18; skew4++)
			{
				printf("[%2d] ", skew4);
			}
			printf("\n");

			for(trig_level=0; trig_level<8; trig_level++)
			{
				printf("  [%2d] ", trig_level);
				for(skew4=0; skew4<18; skew4++)
				{
					//printf("[%2d] ", skew4);
					printf(" %s  ", (result[trig_level][skew4]==0) ? "OK" : LIGHT_RED"NG"NONE);
				}
				printf("\n");
			}

			// restore back
			HalSdio_SetTriggerLevel(0);
			HalSdio_SetSkew(1, 0);
			HalSdio_SetSkew(4, gu32BestSdioSkew4);

			return 0;
		}
		else if(strcmp(argv[2], "write") == 0)
		{
			// prepare buffer
			u16_pattern = simple_strtoul(argv[3], NULL, 16);
			memset((void*)0x20400000, u16_pattern, TEST_BLOCK_COUNT<<9);

			if(strcmp(argv[3], "rescan") == 0)
			{
				u8RescanAfterFail = 1;
			}

			if(strcmp(argv[3], "skew1") == 0)
			{
				skew1_start = simple_strtoul(argv[4], NULL, 10);
				skew1_end = skew1_start + 1;
				if(skew1_start>17) return cmd_usage(cmdtp);
			}

			gu8SdioSilenceTuning = (strcmp(argv[argc-1], "log") == 0) ? TUNE_NOW:(TUNE_SILENCE+TUNE_NOW);

			for(skew1=skew1_start; skew1<skew1_end; skew1++)
			{
				if(!(gu8SdioSilenceTuning&TUNE_SILENCE)) printf("skew1 = %d\n", skew1);
				for(skew4=0; skew4<18; skew4++)
				{
					if(!(gu8SdioSilenceTuning&TUNE_SILENCE)) printf("skew4 = %d\n", skew4);
					//printf("\nS1[%d]S4[%d]\n", skew1, skew4);
					HalSdio_SetSkew(1, skew1);
					HalSdio_SetSkew(4, skew4);

					n = mmc->block_dev.block_write(curr_device, 100000, TEST_BLOCK_COUNT, (void*)0x20400000);
					if(n == TEST_BLOCK_COUNT)
					{
						result[skew1][skew4] = 0; // OK
					}
					else
					{
						result[skew1][skew4] = 1; // NG
						if(u8RescanAfterFail)
						{
							mmc->has_init = 0; sd_init(mmc); // init again for clean enviroment
							gu8SdioSilenceTuning = TUNE_SILENCE+TUNE_NOW;
						}
					}
				}
			}

			gu8SdioSilenceTuning = TUNE_FINISH;

			printf("S1\\S4: ");
			for(skew4=0; skew4<18; skew4++)
			{
				printf("[%2d] ", skew4);
			}
			printf("\n");

			for(skew1=skew1_start; skew1<skew1_end; skew1++)
			{
				printf("  [%2d] ", skew1);
				for(skew4=0; skew4<18; skew4++)
				{
					//printf("[%2d] ", skew4);
					printf(" %s  ", (result[skew1][skew4]==0) ? "OK" : LIGHT_RED"NG"NONE);
				}
				printf("\n");
			}

			// restore back
			HalSdio_SetSkew(1, 0);
			HalSdio_SetSkew(4, gu32BestSdioSkew4);

			return 0;
		}

	}
	#endif // #ifdef SDIO_SUPPORT_SD30
	#endif // #ifdef SDIO_IP_VERIFY
	else
	{
		return cmd_usage(cmdtp);
	}

	printf("\n");

	return 0;

}


