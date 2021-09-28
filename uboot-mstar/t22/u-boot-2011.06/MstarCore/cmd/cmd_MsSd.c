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
#include <MsSd.h>
#include "hal_sdio.h"

U_BOOT_CMD(
	sd, 19, 0, do_MsSd,
	"MStar SD card sub-system",
	"rescan                  - card initialization & identification\n"
	"sd info                    - show SD card related infomation\n"
	"sd list                    - lists available devices\n"
	"sd dev [dev] [part]        - show or set current sd device [partition]\n"
	"sd part                    - lists available partition on current mmc device\n"
	"sd read ADDR BLKNO CNT     - read sd data to memory\n"
	"sd write ADDR BLKNO CNT    - write memory data to sd\n"
	"sd speed                   - test SD read write performance\n"
	"sd status                  - send cmd13 to get sd status\n"
	"sd burning [9999->infinite]- burning test\n"
	"sd reg [BANK]              - dump related register value\n"
#if defined(SDIO_IP_VERIFY)
	"sd ed [wcrc/wto/rspto/rdto]- test error detect\n"
	"sd tune [rsp/rstop/wrsts]  - \n"
	"sd bist                    - test BIST function\n"
	"sd pwrsave                 - test power save mode\n"
	"sd adma                    - test ADMA write read compare\n"
	"sd r2n                     - test R2N write read compare\n"
	"sd test [wo/ro] [pattern]  - write only or read only test, max 16 pattern\n"
	"sd driving [level]         - adjust sd bus driving level 0~7\n"
	#ifdef SDIO_MCM_TEST
	"sd mcm [ratio]             - miu clock manager slow down ratio test, ratio 0 means disable\n"
	#endif
	#ifdef SDIO_MIU_EFFICIENCY_TEST
	"sd miueffi                 - measure miu efficiency\n"
	#endif
	#ifdef SDIO_SUPPORT_SD30
	"sd skew [index] [value]    - config skew value\n"
	"sd scanphase [read/write]  - scan skew value to show eye wide\n"
	#endif
	"sd d1intr                  - test SDIO D1 interrupt\n"
#endif
);


