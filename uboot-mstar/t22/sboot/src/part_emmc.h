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
/*
 * See also Linux sources, fs/partitions/mac.h
 *
 * This file describes structures and values related to the standard
 * Apple SCSI disk partitioning scheme. For more information see:
 * http://developer.apple.com/techpubs/mac/Devices/Devices-126.html#MARKER-14-92
 * Refined by rui.wang@mstarsemi.com
 */

#ifndef _DISK_PART_EMMC_H
#define _DISK_PART_EMMC_H

/*      0 ~ 32KB for partition table. 
    32KB ~ 64KB reserved for partition table, but not used
    64KB ~   2MB reserved for emmc driver test
      2MB ~  7MB  reserved for chunk header, pm, uboot and env
      7MB ~          ordinary partition data start
*/

#define EMMC_RESERVED_FOR_MAP 10         
#define EMMC_RESERVED_FOR_MAP_V2 63   //0~32kb for partition table. 32kb~64kb reserved for partition table, but not used
#define EMMC_CHUNK_HEAD_START 4096        // chunk head start addr
#define EMMC_PARTITION_START 14336       // ordinary partition data start addr

#define EMMC_DRIVER_MAGIC   0x1630

/*
 * Driver Descriptor Structure, in block 0.
 * This block is (and shall remain) 512 bytes long.
 * Note that there is an alignment problem for the driver descriptor map!
 */
typedef struct emmc_driver_desc {
        U16   signature;           /* expected to be EMMC_DRIVER_MAGIC  */
        U16   blk_size;             /* block size of device */
        U32   blk_count;           /* number of blocks on device */
        U16   dev_type;            /* device type */
        U16   dev_id;                /* device id */
        U32   data;                   /* reserved */
        U16   version;               /* version number of partition table */
        U16   drvr_cnt;             /* number of blocks reserved for partition table */
        U16   drvr_map[246];    /* driver descriptor map */
} emmc_driver_desc_t;

/*
 * Device Driver Entry
 * (Cannot be included in emmc_driver_desc because of alignment problems)
 */
typedef struct emmc_driver_entry {
        U32   block;          /* block number of starting block */
        U16   size;            /* size of driver, in 512 byte blocks */
        U16   type;           /* OS Type */
} emmc_driver_entry_t;


#define EMMC_PARTITION_MAGIC    0x5840
#define EMMC_PARTITIONTABLE_VERSION2 0x2000

/* type field value for A/UX or other Unix partitions */
#define APPLE_AUX_TYPE  "Apple_UNIX_SVR2"

/*
 * Each Partition Map entry (in blocks 1 ... N) has this format:
 */
typedef struct EMMC_partition {
        U16   signature;          /* expected to be EMMC_PARTITION_MAGIC   */
        U16   sig_pad;            /* reserved */
        U32   map_count;       /* blocks in partition map   */
        U32   start_block;       /* abs. starting block # of partition    */
        U32   block_count;      /* number of blocks in partition  */
        unsigned char    name[32];          /* partition name   */
        unsigned char    type[32];           /* string type description */
        U32   data_start;        /* rel block # of first data block   */
        U32   data_count;       /* number of data blocks  */
        U32   status;               /* partition status bits  */
        U32   boot_start;         /* first block of boot code */
        U32   boot_size;          /* size of boot code, in bytes  */
        U32   boot_load;          /* boot code load address   */
        U32   boot_load2;        /* reserved  */
        U32   boot_entry;        /* boot code entry point */
        U32   boot_entry2;      /* reserved   */
        U32   boot_cksum;      /* boot code checksum  */
        unsigned char    processor[16];    /* Type of Processor */
        U16   part_pad[188];   /* reserved    */
#ifdef CONFIG_ISO_PARTITION
	unsigned char   iso_dummy[2048];/* Reservere enough room for an ISO partition block to fit */
#endif
} emmc_partition_t;

#define EMMC_STATUS_BOOTABLE    8   /* partition is bootable */

#endif  /* _DISK_PART_EMMC_H */
