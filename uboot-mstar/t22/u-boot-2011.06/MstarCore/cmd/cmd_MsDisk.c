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
#include <MsDisk.h>

U_BOOT_CMD(
	disk, 7, 1, do_disk,
	"MStar Disk sub-system \n",
	"\n  dump       DiskByteAddr // for NAND\n"
	"  init       // for NAND: ftl init\n"
	"  init       // for eMMC: mmc slc 0 1\n"
	"  part       // list partitions \n"
	"  rmgpt      // remove partition table \n"
	"  create     PartName ByteCnt \n"
	"  write.p    DRAMAddr PartName ByteCnt SkipPadding\n"
	"  write.boot BootPartNo DRAMAddr StartBlk ByteCnt SkipPadding // for eMMC\n"
	"  unlzo      DRAMAddr ByteCnt PartName SkipPadding\n"
	"  unlzo.cont DRAMAddr ByteCnt PartName SkipPadding\n"
	"  erase.p    PartName\n"
	"  type       // show disk type: NAND or eMMC \n"
);


