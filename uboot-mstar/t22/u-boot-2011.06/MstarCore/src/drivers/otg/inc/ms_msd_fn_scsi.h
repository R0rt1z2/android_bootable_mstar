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
#ifndef _USB_MSD_FN_DEFINES_H_
#define _USB_MSD_FN_DEFINES_H_

//#include "type.h"

/* bInterfaceProtocol */
#define MSDFN_CBI_PROTOCOL           0x00
#define MSDFN_CB_PROTOCOL            0x01
#define MSDFN_BOT_PROTOCOL           0x50

/* bInterfaceSubClass */
#define MSDFN_RBC_SUBCLASS           0x01
#define MSDFN_ATAPI_SUBCLASS         0x02
#define MSDFN_QIC_157_SUBCLASS       0x03
#define MSDFN_UFI_SUBCLASS           0x04
#define MSDFN_SFF_8070i_SUBCLASS     0x05
#define MSDFN_SCSI_SUBCLASS          0x06

/* BOT Only Protocol */
#define MSDFN_BOT_CBW_SIGNATURE      0x43425355
#define MSDFN_BOT_CSW_SIGNATURE      0x53425355
#define MSDFN_BOT_CBW_LENGTH          31
#define MSDFN_BOT_CSW_LENGTH          13
#define MSDFN_BOT_LUN_MASK            0xf0

/* Mass Storage Device BOT Command Execution State */
#define MSDFN_BOT_COMMAND_PASSED      0x00
#define MSDFN_BOT_COMMAND_FAILED      0x01
#define MSDFN_BOT_PHASE_ERROR         0x02

/* Data Transfer Direction */
#define DATA_DIRECTION_MASK         0x80
#define DATA_OUT                    0x00
#define DATA_IN                     0x80

/* MSD BOT Functional Characteristics */
#define MSDFN_BOT_RESET               0xFF
#define MSDFN_BOT_GET_MAXLUN          0xFE

/* MSD BOT Host-Device Error Condtions */
#define CASEOK                       0
#define CASE_Hn_eq_Dn                1
#define CASE_Hn_lt_Di                2
#define CASE_Hn_lt_Do                3
#define CASE_Hi_gt_Dn                4
#define CASE_Hi_gt_Di                5
#define CASE_Hi_eq_Di                6
#define CASE_Hi_lt_Di                7
#define CASE_Hi_ne_Do                8
#define CASE_Ho_gt_Dn                9
#define CASE_Ho_ne_Di               10
#define CASE_Ho_gt_Do               11
#define CASE_Ho_eq_Do               12
#define CASE_Ho_lt_Do               13
#define CASECBW                     14
#define CASE_ERROR                  15

/* Data Transfer Length for different commands */
#define FMT_CACP_LEN        12 /* Format Capacity */
#define MODE_SEN_LEN        4  /* Mode Sense */
#define INQ_DATA_LEN        36 /* Inquiry */
#define REQ_SENS_LEN        18 /* Request Sense */
#define RD_CAPCY_LEN        8  /* Read Capacity */

/* SCSI Commands */
#define SCSI_FN_INQUIRY             0x12
#define SCSI_FN_REQUEST_SENSE       0x03
#define SCSI_FN_SEND_DIAGNOSTIC     0x1D
#define SCSI_FN_TEST_UNIT_READY     0x00
#define SCSI_FN_FORMAT_UNIT         0x04
#define SCSI_FN_READ6               0x08
#define SCSI_FN_WRITE6              0x0A
#define SCSI_FN_READ10              0x28
#define SCSI_FN_WRITE10             0x2A
#define SCSI_FN_READ_CAPACITY       0x25
#define SCSI_FN_RELEASE             0x17
#define SCSI_FN_RESERVE             0x16
#define SCSI_FN_RD_FMT_CAPC         0x23
#define SCSI_FN_MODE_SENSE          0x1A
#define SCSI_FN_VERIFY              0x2F
#define SCSI_FN_STARTSTOP           0x1B
#define SCSI_FN_PREVENT             0x1E
#define SCSI_1STID_VENDOR           0xE0
//#define SCSI_NAND_NANDINFO          0xE2
//#define SCSI_NAND_ERASEBLOCK        0xE3
//#define SCSI_NAND_READPHYPAGE       0xE4
//#define SCSI_NAND_WRITEPHYPAGE      0xE5
//#define SCSI_NAND_WRITERDD          0xE6
//#define SCSI_NAND_EJECT_INSERT      0xE7
#define SCSI_VENDOR_XROM            0xE8
#define SCSI_VENDOR_TEST            0xE9
#define SCSI_V_RWCompare_TEST       0xEA
#define SCSI_V_DA_FIFO_TEST         0xEB
#define SCSI_V_DA_DMA_TEST          0xEC

#define KEY_NO_SENSE            0x00
#define KEY_RECOVERED_ERROR	    0x01
#define KEY_NOT_READY           0x02
#define KEY_MEDIUM_ERROR        0x03
#define KEY_HARDWARE_ERROR      0x04
#define KEY_ILLEGAL_REQUEST     0x05
#define KEY_UNIT_ATTENTION      0x06
#define KEY_DATA_PROTECT        0x07
#define KEY_BLANK_CHECK         0x08
#define KEY_VENDOR_SPECIFIC     0x09
#define KEY_COPY_ABORTED        0x0A
#define KEY_ABORTED_CMD         0x0B
#define KEY_OBSOLETE            0x0C
#define KEY_VOLUMN_OVERFLOW     0x0D
#define KEY_MISCOMPARE          0x0E
#define KEY_RESERVED            0x0F

// KEY_NO_SENSE: Additional key
#define ADDKEY_NO_ADDITIONAL            0x00

// KEY_NOT_READY: Additional key
#define ADDKEY_LOGICAL_UNIT_NOT_SUPPORT 0x25
#define ADDKEY_MEDIA_CHANGE             0x28
#define ADDKEY_MEDIUM_NOT_PRESENT       0x3A

// KEY_ILLEGAL_REQUEST: Additional key
#define ADDKEY_INVALID_CMD_OP_CODE	    0x20
#define ADDKEY_INVALID_FIELD_IN_CMD 	0x24
#define ADDKEY_MEDIUM_FORMAT_CORRUPTED  0x31

U8 FnVendor_Cmds(void *pcbw, void *pcsw, void *pUsbStruct);
U8 FnDisk_Cmds_Rom(void *pcbw, void *pcsw, void *pUsbStruct);
U8 FnDisk_Cmds_Ram(void *pcbw, void *pcsw, void *pUsbStruct);
void MSDFNCMDloop(USB_VAR *gUSBStruct);
#endif
