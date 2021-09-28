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
#ifndef __UNFD_ERR_CODES_H__
#define __UNFD_ERR_CODES_H__

#define  UNFD_ST_SUCCESS                   0

#define  UNFD_ST_ERR_2L_MAP2_1P           (0x0000 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_BL0_ECCERR           (0x0001 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_BL1_ECCERR           (0x0002 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_CIS0_ECCERR          (0x0003 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_CIS1_ECCERR          (0x0004 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_CIS_NAND_ERR         (0x0005 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_CIS_PART_ERR         (0x0006 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_DATA_CMP_FAIL        (0x0007 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_DISKCONN_FAIL        (0x0008 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_DISK_CONN_FAIL       (0x0009 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_DISKINIT_FAIL        (0x000A | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_E_BUSY               (0x000B | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_ECC_FAIL             (0x000C | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_ECC_FAIL_RM          (0x000D | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_E_FAIL               (0x000E | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_E_PROTECTED          (0x000F | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_E_TIMEOUT            (0x0010 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_EXCEED_CAPACITY      (0x0011 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_HAL_R_INVALID_PARAM  (0x0012 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_HAL_W_INVALID_PARAM  (0x0013 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_INVALID_ADDR         (0x0014 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_INVALID_ECC_REG51    (0x0015 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_INVALID_ECC_REG52    (0x0016 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_INVALID_ECC_REG53    (0x0017 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_INVALID_PARAM        (0x0018 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_INVALID_PART         (0x0019 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_IOLUP_FAIL           (0x001A | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_LACK_BLK             (0x001B | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_LOGI_PART            (0x001C | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_MIU_PARAM0_ECCERR    (0x001D | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_MIU_PARAM1_ECCERR    (0x001E | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_MIU_RPATCH_TIMEOUT   (0x001F | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_BL                (0x0020 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_BLK_FOR_BL0       (0x0021 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_BLK_FOR_BL1       (0x0022 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_BLK_FOR_CIS0      (0x0023 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_BLK_FOR_CIS1      (0x0024 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_BLK_FOR_MIU0      (0x0025 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_BLK_FOR_MIU1      (0x0026 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_BLK_FOR_OS0       (0x0027 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_BLK_FOR_OS1       (0x0028 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_CIS               (0x0029 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_GOOD_BLK          (0x002A | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_MIU_PARAM         (0x002B | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_NFIE              (0x002C | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_OS                (0x002D | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NOT_READY            (0x002E | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NULL_PTR             (0x002F | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_OS0_ECCERR           (0x0030 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_OS1_ECCERR           (0x0031 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_PHY_PART             (0x0032 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_PLAT_INIT_0          (0x0033 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_RST_TIMEOUT          (0x0034 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_R_TIMEOUT            (0x0035 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_R_TIMEOUT_RM         (0x0036 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_SIGNATURE_FAIL       (0x0037 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_UNKNOWN_ECC_TYPE     (0x0038 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_UNKNOWN_ID           (0x0039 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_UNKNOWN_RSEQ         (0x003A | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_W_BUSY               (0x003B | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_W_FAIL               (0x003C | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_W_PROTECT            (0x003D | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_W_PROTECTED          (0x003E | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_W_TIMEOUT            (0x003F | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_W_TIMEOUT_RM         (0x0040 | UNFD_ST_PLAT)
#define  UNFD_ST_NO_NANDINFO              (0x0041 | UNFD_ST_PLAT)
#define  UNFD_ST_NO_PARTINFO              (0x0042 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_BAD_BLK              (0x0043 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_SEMAPHORE            (0x0044 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_FTL_NOT_FORMAT       (0x0045 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_FTL_NO_ROOT_BLK      (0x0046 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_FTL_VERSION_MISS     (0x0047 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NOT_ALIGN            (0x0048 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NOT_PACKED           (0x0049 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_FAT_SHIFT            (0x004A | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_FREE_BLK_FOR_RD   (0x004B | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_EC_SHIFT             (0x004C | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NOT_TOGGLE_DDR       (0x004D | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NO_TOGGLE_DDR_TIMING (0x004E | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_NOT_ONFI_DDR         (0x004F | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_SET_FEATURE_TIMEOUT  (0x0050 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_TOGGLE_SET_SYNC_MODE (0x0051 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_LFSR_RD_LATCH        (0x0052 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_LFSR_DDRTIMING       (0x0053 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_PROBABLE_TOGGLE_DDR       (0x0054 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_PAD_UNSUPPORT_DDR_NAND    (0x0055 | UNFD_ST_PLAT)
#define  UNFD_ST_ERR_SETUP_READ_RETRY_FAIL     (0x0056 | UNFD_ST_PLAT)
#define	 UNFD_ST_ERR_INVAL_CIFD_CNT            (0x0057 | UNFD_ST_PLAT)
#define	 UNFD_ST_ERR_NAND_BUSY                 (0x0058 | UNFD_ST_PLAT)
#define	 UNFD_ST_ERR_W_TWICE                   (0x0059 | UNFD_ST_PLAT)
#define	 UNFD_ST_ERR_RR_INIT_FAIL              (0x005A | UNFD_ST_PLAT)


#endif /* __UNFD_ERR_CODES_H__ */
