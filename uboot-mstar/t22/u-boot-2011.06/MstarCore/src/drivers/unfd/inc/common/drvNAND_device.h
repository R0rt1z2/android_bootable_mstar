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

#ifndef __UNFD_DEVICE_H__
#define __UNFD_DEVICE_H__

#include "drvNAND.h"
#if IF_IP_VERIFY
#if defined(H9LA25G25HAMBR) && H9LA25G25HAMBR
	#define NAND_DEVICE_ID               {0xAD,0x35}
	#define NAND_DEVICE_ID_LEN           2
	#define NAND_BLK_CNT                 0x800
	#define NAND_BLK_PAGE_CNT            0x20

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C3TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x200
	#define NAND_SPARE_BYTE_CNT          0x10

#elif defined(H8ACU0CE0BBR) && H8ACU0CE0BBR
    #define NAND_DEVICE_ID               {0xAD, 0x76}
	#define NAND_DEVICE_ID_LEN           2
	#define NAND_BLK_CNT                 4096
	#define NAND_BLK_PAGE_CNT            0x20

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x200
	#define NAND_SPARE_BYTE_CNT          0x10

#elif defined(TH58TVG7D2GBA) && TH58TVG7D2GBA
    #define NAND_DEVICE_ID               {0x98, 0xDE, 0x94, 0x82, 0x76, 0xD6}
	#define NAND_DEVICE_ID_LEN           6
	#define NAND_BLK_CNT                 4124 // original 2 /CE have 4124x2 blk, only use /CE0
	#define NAND_BLK_PAGE_CNT            0x100

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_40BIT1KB //

	#define NAND_PAGE_BYTE_CNT           0x2000
	#define NAND_SPARE_BYTE_CNT          640//0x200 // original 640

#elif defined(MT29F64G08CBAAB) && MT29F64G08CBAAB
    #define NAND_DEVICE_ID				 {0x2C, 0x88, 0x04, 0x4B, 0xA9, 0x00, 0x00, 0x00}
	#define NAND_DEVICE_ID_LEN           8
	#define NAND_BLK_CNT                 2048 // one /CE
	#define NAND_BLK_PAGE_CNT            0x100

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_24BIT1KB

	#define NAND_PAGE_BYTE_CNT           0x2000
	#define NAND_SPARE_BYTE_CNT          448
#elif defined(MT29F32G08CBADA) && MT29F32G08CBADA
    #define NAND_DEVICE_ID				 {0x2C, 0x44, 0x44, 0x4B, 0xA9}
	#define NAND_DEVICE_ID_LEN           5
	#define NAND_BLK_CNT                 2048 // one /CE
	#define NAND_BLK_PAGE_CNT            0x100

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_40BIT1KB

	#define NAND_PAGE_BYTE_CNT           0x2000
	#define NAND_SPARE_BYTE_CNT          704

#elif defined(H8ACU0CE0DAR) && H8ACU0CE0DAR
	#define NAND_DEVICE_ID               {0xAD,0x76}
	#define NAND_DEVICE_ID_LEN           2
	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x20

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x200
	#define NAND_SPARE_BYTE_CNT          0x10

#elif defined(TC58TEG5DCJTA) && TC58TEG5DCJTA
	#define NAND_DEVICE_ID               {0x98,0xD7,0x84,0x93,0x72,0x57}
	#define NAND_DEVICE_ID_LEN           6
	#define NAND_BLK_CNT                 0x424
	#define NAND_BLK_PAGE_CNT            0x100
	
    #define NAND_RW_ADR_CYCLE_CNT		 ADR_C5TFS0
    #define NAND_E_ADR_CYCLE_CNT		 ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_40BIT1KB
	
	#define NAND_PAGE_BYTE_CNT           0x4000
	#define NAND_SPARE_BYTE_CNT          0x500


#elif defined(TY890A111431KA) && TY890A111431KA
	#define NAND_DEVICE_ID               {0x98,0xD0,0x00,0x15,0x72,0x14}
	#define NAND_DEVICE_ID_LEN           1
	#define NAND_BLK_CNT                 0x200
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(TY8A0A111178KC4) && TY8A0A111178KC4
	#define NAND_DEVICE_ID               {0x98,0xC1,0x90,0x55,0x76,0x14}
	#define NAND_DEVICE_ID_LEN           6
	#define NAND_BLK_CNT                 0x400
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(TY890A111229KC40) && TY890A111229KC40
	#define NAND_DEVICE_ID               {0x98,0xD0,0x00,0x15,0x72,0x14}
	#define NAND_DEVICE_ID_LEN           6
	#define NAND_BLK_CNT                 0x200
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(TY8A0A111162KC40) && TY8A0A111162KC40
	#define NAND_DEVICE_ID               {0x98,0xA1,0x90,0x15,0x76,0x14}
	#define NAND_DEVICE_ID_LEN           6
	#define NAND_BLK_CNT                 0x400
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(K9F1G08X0C) && K9F1G08X0C /* K9F1G08U0C, K9F1G08B0C */
	#define NAND_DEVICE_ID               {0xEC,0xF1,0x00,0x95,0x40}
	#define NAND_DEVICE_ID_LEN           5
	#define NAND_BLK_CNT                 0x400
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT//ECC_TYPE_RS

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(K9GAG08U0E) && K9GAG08U0E /* K9GAG08U0E, K9GAG08U0E */
	#define NAND_DEVICE_ID               {0xEC, 0xD5, 0x84, 0x72, 0x50, 0x42}
	#define NAND_DEVICE_ID_LEN           6
	#define NAND_BLK_CNT                 0x800
	#define NAND_BLK_PAGE_CNT            0x80

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_24BIT1KB//ECC_TYPE_RS//ECC_TYPE_4BIT//
	#define NAND_PAGE_BYTE_CNT           0x2000
	#define NAND_SPARE_BYTE_CNT          0x1B0

#elif defined(K522H1GACE) && K522H1GACE /* K522H1GACE, K522H1GACE */
	#define NAND_DEVICE_ID               {0xEC,0xBA,0x00,0x55,0x44}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x800
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(H27UAG8T2M) && H27UAG8T2M
	#define NAND_DEVICE_ID               {0xAD,0xD5,0x14,0xB6,0x44}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x80

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x1000
	#define NAND_SPARE_BYTE_CNT          0x80

#elif defined(H27UAG8T2M_FAKE2K) && H27UAG8T2M_FAKE2K
	#define NAND_DEVICE_ID               {0xAD,0xD5,0x14,0xB6,0x44}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x80

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_24BIT1KB

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x80

#elif defined(NAND256W3A) && NAND256W3A
	#define NAND_DEVICE_ID               {0x20,0x75}
	#define NAND_DEVICE_ID_LEN           2

	#define NAND_BLK_CNT                 0x800
	#define NAND_BLK_PAGE_CNT            0x20

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C3TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x200
	#define NAND_SPARE_BYTE_CNT          0x10

#elif defined(NAND512W3A2C) && NAND512W3A2C
	#define NAND_DEVICE_ID               {0x20,0x76}
	#define NAND_DEVICE_ID_LEN           2

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x20

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x200
	#define NAND_SPARE_BYTE_CNT          0x10

#elif defined(H27UF081G2A) && H27UF081G2A
	#define NAND_DEVICE_ID               {0xAD,0xF1,0x80,0x1D}
	#define NAND_DEVICE_ID_LEN           4

	#define NAND_BLK_CNT                 0x400
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(H27UBG8T2A) && H27UBG8T2A
	#define NAND_DEVICE_ID               {0xAD,0xD7,0x94,0x9A,0x74,0x42}
	#define NAND_DEVICE_ID_LEN           6

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x100

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_24BIT1KB

	#define NAND_PAGE_BYTE_CNT           0x2000
	#define NAND_SPARE_BYTE_CNT          448

#elif defined(H27U1G8F2B) && H27U1G8F2B
	#define NAND_DEVICE_ID               {0xAD,0xF1,0x00,0x15}
	#define NAND_DEVICE_ID_LEN           4

	#define NAND_BLK_CNT                 0x400
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(HY27UF084G2B) && HY27UF084G2B
	#define NAND_DEVICE_ID               {0xAD,0xDC,0x10,0x95,0x54}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(HY27UF082G2B) && HY27UF082G2B
	//#define NAND_DEVICE_ID               "\xAD\xDA\x10\x95\x44"
	#define NAND_DEVICE_ID               {0xAD,0xDA,0x10,0x95,0x44}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x800
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40//0x100

#elif defined(H27U2G8F2CTR) && H27U2G8F2CTR
    //#define NAND_DEVICE_ID               "\xAD\xDA\x10\x95\x44"
	#define NAND_DEVICE_ID                {0xAD,0xDA,0x90,0x95,0x44}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x800
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40//0x100

#elif defined(H27U4G8F2DTR) && H27U4G8F2DTR

	#define NAND_DEVICE_ID               {0xAD,0xDC,0x90,0x95,0x54}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40//0x100

#elif defined(H27U4G8F2ETR) && H27U4G8F2ETR
	
	#define NAND_DEVICE_ID               {0xAD,0xDC,0x90,0x95,0x56}
	#define NAND_DEVICE_ID_LEN           5
	
	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x40
	
	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT
	
	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40//0x100


#elif defined(K9HAG08U1M) && K9HAG08U1M
	#define NAND_DEVICE_ID               {0xEC,0xD3,0x55,0x25,0x58}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x80

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(K511F12ACA) && K511F12ACA
	#define NAND_DEVICE_ID               {0xEC,0xA1,0x00,0x15,0x40}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x400
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C4TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C2TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(K9F4G08U0D) && K9F4G08U0D
	#define NAND_DEVICE_ID               {0xEC,0xDC,0x10,0x95,0x54}
	#define NAND_DEVICE_ID_LEN           5

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x40

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_4BIT

	#define NAND_PAGE_BYTE_CNT           0x800
	#define NAND_SPARE_BYTE_CNT          0x40

#elif defined(P1UAGA30AT) && P1UAGA30AT
	#define NAND_DEVICE_ID               {0xC8,0xD5,0x14,0x29,0x34,0x01}
	#define NAND_DEVICE_ID_LEN           0x6

	#define NAND_BLK_CNT                 0x1000
	#define NAND_BLK_PAGE_CNT            0x80

	#define NAND_RW_ADR_CYCLE_CNT        ADR_C5TFS0
	#define NAND_E_ADR_CYCLE_CNT         ADR_C3TRS0
	#define NAND_ECC_TYPE                ECC_TYPE_12BIT // only 8-bits specified

	#define NAND_PAGE_BYTE_CNT           0x1000
	#define NAND_SPARE_BYTE_CNT          218
	
#elif defined(MT29F32G08CBACA) && MT29F32G08CBACA
    #define NAND_DEVICE_ID				 {0x2C, 0x68 ,0x04 ,0x4A ,0xA9}
    #define NAND_DEVICE_ID_LEN			 0x5
   
    #define NAND_BLK_CNT				 0x1000
    #define NAND_BLK_PAGE_CNT			 0x100
   
    #define NAND_RW_ADR_CYCLE_CNT		 ADR_C5TFS0
    #define NAND_E_ADR_CYCLE_CNT		 ADR_C3TRS0
    #define NAND_ECC_TYPE				 ECC_TYPE_24BIT1KB // only 8-bits specified
   
    #define NAND_PAGE_BYTE_CNT			 0x1000
    #define NAND_SPARE_BYTE_CNT 		 224

#elif defined(MT29F32G08CBADA) && MT29F32G08CBADA
    #define NAND_DEVICE_ID				 {0x2C, 0x44 ,0x44 ,0x4B ,0xA9}
    #define NAND_DEVICE_ID_LEN			 0x5
       
    #define NAND_BLK_CNT				 0x800//2128
    #define NAND_BLK_PAGE_CNT			 0x100
       
    #define NAND_RW_ADR_CYCLE_CNT		 ADR_C5TFS0
    #define NAND_E_ADR_CYCLE_CNT		 ADR_C3TRS0
    #define NAND_ECC_TYPE				 ECC_TYPE_40BIT1KB // only 8-bits specified
       
    #define NAND_PAGE_BYTE_CNT			 0x2000
    #define NAND_SPARE_BYTE_CNT 		 640

#else
	#error "No NAND device specified\n"
#endif
#endif // IF_IP_VERIFY
#endif /* __UNFD_DEVICE_H__ */
