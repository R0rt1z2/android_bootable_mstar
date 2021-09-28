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
#include <MsNand.h>


U_BOOT_CMD(
	msnand, 10, 1, do_msnand,
	"MStar NAND sub-system \n",
	"\n  info - dump nni, pni \n"
	"  info more - dump nni, pni, ppm \n"
	"  init - run unfd init \n"
	"  bb - list all bad blocks PBA \n"
	"  bb set PBA(hex) - make the PBA as bb \n"
	"  bb dump PBA(hex) - check the PBA and dump page + spare\n"
	"  erase PBA(hex) cnt(hex) - erase PBA ~ PBA+cnt blocks \n"
	"  temp -  \n"
	"  life PBA(hex) MaxPECycle(dec) LoopCnt(hex)- use 6 blocks from the PBA, to test ErrBit growing speed. \n"
	"  test BIST PBA(hex) - test HAL @ PBA: erase, write, read, compare \n"	
	"  test pwroff - verify PM power off \n"
	"  test timer - verify HW timer"
);


