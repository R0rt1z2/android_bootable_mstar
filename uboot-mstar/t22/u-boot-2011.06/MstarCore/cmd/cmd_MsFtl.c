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

#include <command.h>
#include <MsFtl.h>

U_BOOT_CMD(
	ftl, 7, 1, do_ftl,
	"MStar FTL sub-system \n",
	"\n  read    DRAMAddr BlkAddr ByteCnt \n"
	"  read.p  DRAMAddr PartName PartBlkAddr ByteCnt \n"
	"  write   DRAMAddr BlkAddr ByteCnt \n"
	"  write.p DRAMAddr PartName PartBlkAddr ByteCnt \n"
	"  create  PartName SizeByteCnt StartAddrByteCnt \n"
	"  rmgpt - remove all partitions \n"
	"  part - list partitions info \n"
);


U_BOOT_CMD(
	ftltest, 7, 1, do_ftltest,
	"MStar FTL sub-system \n",
	"\n  info - dump FTL info \n"
	"  info LBA n(hex) - dump the LBA info \n"
	"  info DCBI - dump the DCB info \n"
	"  init [count] - init FTL. [count] used to profile init time \n"
	"  format - format (clean) NAND FTL area \n"
	"  hal PBA(hex) - test HAL APIs \n"
	"  speed - test FTL w/r speed \n"
    "  life - test FTL NAND lifetime \n"    
    "  pwr_cut init 1 - init for power-cut test \n"    
    "  pwr_cut w cnt - w for power-cut test, loop cnt \n"    
    "  pwr_cut rc cnt - check for power-cut test, loop cnt \n"
    "  pwr_cut wrc cnt - wrc for power-cut test, loop cnt \n"
    "      for auto test: \n"
    "        1. ftltest pwr_cut init 1 \n"
    "        2. @ board.c / board_init_r \n"
    "          - after nand_init(), call FtlTest_PwrCutTestCheck(),\n"
    "          - and while(1) FtlTest_PwrCutTestRun(); \n"
    "        3. then auto random power_cut. \n"
    "  temp -  \n"
    "  512BAddr 512BCnt count [w/rc] - test FTL \n"
    "      count=3697 means infinite loop \n"
    "      no [w/rc] means WRC \n"
);


