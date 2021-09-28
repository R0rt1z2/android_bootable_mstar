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
#include <MsDisk.h>
#include "../drivers/mstar/unfd/ftl/ftl.h"

extern int nand_unfd_init(void);

#ifdef CONFIG_LZO
#include <linux/lzo.h>
#endif
#include "../disk/part_emmc.h"

//----------------------------------
UNFD_ALIGN0 static unsigned char tmp_buf[0x200] UNFD_ALIGN1; // used inside this file
#define CMD_LEN  20
static char command[CMD_LEN];

int do_disk(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc < 2)
		return cmd_usage(cmdtp);

	memset(command, 0, CMD_LEN);
	
	if(strncmp(argv[1], "dump", 4) == 0)
	{
		if(MSDISK_NAND == ChkDiskType())
		{
			sprintf(command,"nand dump %s", argv[2]);
			//printf("\n %s \n", command);
		    return run_command((const char *)command, 0);
		}
		else 
			cmd_usage(cmdtp);
			
		return 0;
	}
	//----------------------------------
	else if(strncmp(argv[1], "init", 4) == 0)
    {
		if(MSDISK_NAND == ChkDiskType())
		{
			sprintf(command,"ftl init");
			printf("\n %s \n", command);
			return run_command((const char *)command, 0);
		}
		else if(MSDISK_eMMC == ChkDiskType())
		{
			sprintf(command,"mmc slc 0 1");
			printf("\n %s \n", command);
			return run_command((const char *)command, 0);
		}
	}
	//----------------------------------
	else if(strncmp(argv[1], "part", 4) == 0)
    {
		if(MSDISK_NAND == ChkDiskType())
		{
			sprintf(command,"ftl part");
			printf("\n %s \n", command);
			return run_command((const char *)command, 0);
		}
		else if(MSDISK_eMMC == ChkDiskType())
		{
			sprintf(command,"mmc part");
			printf("\n %s \n", command);
			return run_command((const char *)command, 0);
		}
	}
	//----------------------------------
	else if(strncmp(argv[1], "rmgpt", 5) == 0)
    {
		if(MSDISK_NAND == ChkDiskType())
		{
			sprintf(command,"ftl rmgpt");
			printf("\n %s \n", command);
			return run_command((const char *)command, 0);
		}
		else if(MSDISK_eMMC == ChkDiskType())
		{
			sprintf(command,"mmc rmgpt");
			printf("\n %s \n", command);
			return run_command((const char *)command, 0);
		}
	}
	//----------------------------------
	else if(strncmp(argv[1], "create", 6) == 0)
    {
		if(MSDISK_NAND == ChkDiskType())
		{
			sprintf(command,"ftl create %s %s", argv[2], argv[3]);
			printf("\n %s \n", command);
			return run_command((const char *)command, 0);
		}
		else if(MSDISK_eMMC == ChkDiskType())
		{
			sprintf(command,"mmc create %s %s", argv[2], argv[3]);
			printf("\n %s \n", command);
			return run_command((const char *)command, 0);
		}
	}
	//----------------------------------
	else if(strncmp(argv[1], "write", 5) == 0)
    {
		char *s=NULL;
		
		s = strchr(argv[1], '.');
		if(NULL == s)
			return cmd_usage(cmdtp);
		
		if(MSDISK_NAND == ChkDiskType())
		{
			if(strncmp(s, ".p", 2) == 0)
			{
				sprintf(command,"ftl write.p %s %s %s %s", 
					argv[2], argv[3], argv[4], argv[5]);
				printf("\n %s \n", command);
				return run_command((const char *)command, 0);
			}
		}
		else if(MSDISK_eMMC == ChkDiskType())
		{
			if(s)
			{
				#if 1
				if(strncmp(s, ".p", 2) == 0)
					sprintf(command,"mmc write.p %s %s %s %s", 
					    argv[2], argv[3], argv[4], argv[5]);
				else if(strncmp(s, ".boot", 5) == 0)
					sprintf(command,"mmc write.boot %s %s %s %s", 
					    argv[2], argv[3], argv[4], argv[5]);
				else
					return cmd_usage(cmdtp);
				#else
                sprintf(command,"mmc %s %s %s %s %s", 
					argv[1], argv[2], argv[3], argv[4], argv[5]);
				#endif				
				printf("\n %s \n", command);
			}
			return run_command((const char *)command, 0);
		}
	}
	//----------------------------------
	else if(strncmp(argv[1], "erase", 5) == 0)
    {
		char *s=NULL;
		
		s = strchr(argv[1], '.');
		if(NULL == s || strncmp(s, ".p", 2))
			return cmd_usage(cmdtp);

		if(MSDISK_NAND == ChkDiskType())
		{
			sprintf(command,"nand erase.part %s ", argv[2]);
			printf("\n %s \n", command);
			return run_command((const char *)command, 0);
		}
		else if(MSDISK_eMMC == ChkDiskType())
		{
			sprintf(command,"mmc erase.p %s", argv[2]);
			printf("\n %s \n", command);
			return run_command((const char *)command, 0);
		}
		
	}
	//----------------------------------
	else if(strncmp(argv[1], "unlzo", 5) == 0)
    {
		if(MSDISK_NAND == ChkDiskType())
		{
			sprintf(command,"ftl %s %s %s %s", 
				argv[1], argv[2], argv[3], argv[4]);
			printf("\n %s \n", command);
			return run_command((const char *)command, 0);
		}
		else if(MSDISK_eMMC == ChkDiskType())
		{
			sprintf(command,"mmc %s %s %s %s", 
				argv[1], argv[2], argv[3], argv[4]);
			printf("\n %s \n", command);
			return run_command((const char *)command, 0);
		}
	}
	//----------------------------------
    else if(strncmp(argv[1], "type", 4) == 0)
    {
		if(MSDISK_eMMC == ChkDiskType())
			printf(" eMMC \n");
		else if(MSDISK_NAND == ChkDiskType())
			printf(" NAND \n");
		else
			printf(" unknown disk \n");

		return 0;
    }
	
	return cmd_usage(cmdtp);
}


int ChkDiskType(void)
{
	volatile int type=0;
	
	if(REG_FCIE(FCIE_MIE_FUNC_CTL)&BIT_EMMC_EN)
		type |= MSDISK_eMMC;

	if(REG_FCIE(NC_FUN_CTL)&BIT_NC_EN)
		type |= MSDISK_NAND;
	
	if(type != MSDISK_eMMC && type != MSDISK_NAND)
	{
		printf("\n\n Disk Err: unknown disk type: %Xh \n", type);
		while(1);
	}

	return type;
}

