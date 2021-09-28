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
#include "ftlsd_api.h"
#define MICRON_ID     0x2C
#define TOSHIBA_ID    0x98
#define HYNIX_ID      0xAD


U8 MicronIDArray[MicronSetCnt][NAND_ID_BYTE_CNT] = {
    {0x2C, 0x44, 0x44, 0x4B, 0xA9}, //MT29F32G08CBADA, L83A SDR MLC
    {0x2C, 0x64, 0x44, 0x4B, 0xA9}, //MT29F32G08CBADB, L83A DDR MLC
    {0x2C, 0x68, 0x04, 0x4A, 0xA9}  //MT29F32G08CBACA, L74A SDR MLC    
};
U8 MicronIDByteCnt[MicronSetCnt]  = {5, 5, 5};
U8 MicronIDPlaneCnt[MicronSetCnt] = {2, 2, 2};

U8 ToshibaIDArray[ToshibaSetCnt][NAND_ID_BYTE_CNT] = {
    {0x98, 0xD7, 0x84, 0x93, 0x72, 0x51} //TC58TEG5DCLTA00 15nm 1-plane
};
U8 ToshibaIDByteCnt[ToshibaSetCnt]  = {6};
U8 ToshibaIDPlaneCnt[ToshibaSetCnt] = {1};


U8 HynixIDArray[HynixSetCnt][NAND_ID_BYTE_CNT] = {
    {0xAD, 0xDE, 0x14, 0xAB, 0x42, 0x4A } //H27UCG8T2FTR or I3T-8GQ8T2H5TARC 16nm 1-plane
};
U8 HynixIDByteCnt[HynixSetCnt]  = {6};
U8 HynixIDPlaneCnt[HynixSetCnt] = {1};

// ===================================
// 1. Root Region does not need multi-plane, since RootInfo, BBT < 2KB.
// 2. Main Region uses multi-plane if the NAND supported, include CtrlBlk, DCB, DB.
U32 FTLSd_CheckMultiPlane(void)
{
    U8 i, j, *pID_HardCode;

    FtlSdDev.u8_PlaneCnt = 1;

    #if 0
    // --------------------------
    // set PlaneCnt
    switch(FtlSdDev.au8_ID[0])
    {
        case MICRON_ID:
            for(i=0; i<GET_ID_SetCnt(Micron); i++)
            {
                pID_HardCode = GET_ID_ByteStr(Micron, i);

                for(j=1; j<GET_ID_ByteCnt(Micron, i); j++)
                    if(FtlSdDev.au8_ID[j] != pID_HardCode[j])
                        break;

                if(j == GET_ID_ByteCnt(Micron, i))
                {
                    FtlSdDev.u8_PlaneCnt = GET_ID_PlaneCnt(Micron, i);
                    break;
                }
            }
            break;
        case TOSHIBA_ID:
            for(i=0; i<GET_ID_SetCnt(Toshiba); i++)
            {
                pID_HardCode = GET_ID_ByteStr(Toshiba, i);

                for(j=1; j<GET_ID_ByteCnt(Toshiba, i); j++)
                    if(FtlSdDev.au8_ID[j] != pID_HardCode[j])
                        break;

                if(j == GET_ID_ByteCnt(Toshiba, i))
                {
                    FtlSdDev.u8_PlaneCnt = GET_ID_PlaneCnt(Toshiba, i);
                    break;
                }
            }
            break;
		case HYNIX_ID:
			for(i=0; i<GET_ID_SetCnt(Hynix); i++)
			{
				pID_HardCode = GET_ID_ByteStr(Hynix, i);
		
				for(j=1; j<GET_ID_ByteCnt(Hynix, i); j++)
					if(FtlSdDev.au8_ID[j] != pID_HardCode[j])
						break;
		
				if(j == GET_ID_ByteCnt(Hynix, i))
				{
					FtlSdDev.u8_PlaneCnt = GET_ID_PlaneCnt(Hynix, i);
					break;
				}
			}
			break;

    }
    #endif
    // --------------------------
    // set FTL HAL APIs
    switch(FtlSdDev.u8_PlaneCnt)
    {
        case 1:
            FtlSdDev.pFnReadPages = FTLSd_ReadPages;
            FtlSdDev.pFnWritePages = FTLSd_WritePages;
            FtlSdDev.pFnReadSectors = FTLSd_ReadSectors;
            FtlSdDev.pFnWriteSectors = FTLSd_WriteSectors;
            FtlSdDev.pFnEraseBlk = FTLSd_EraseBlk;
            //FtlSdDev.pFnNC_PageCopy = NC_PageCopy;
            #if MSFTL_LINUX
            FtlSdDev.pFnReadSendCmd = FTLSd_ReadSendCmd;
            #endif
            break;
        #if 0
        case 2:
            FtlSdDev.pFnReadPages = FTLSd_ReadPages2PCache;//FTLSd_ReadPages2P
            FtlSdDev.pFnWritePages = FTLSd_WritePages2PCache;//FTLSd_WritePages2P
            FtlSdDev.pFnReadSectors = FTLSd_ReadSectors2P;
            FtlSdDev.pFnWriteSectors = FTLSd_WriteSectors2P;
            FtlSdDev.pFnEraseBlk = FTLSd_EraseBlk2P;
            FtlSdDev.pFnNC_PageCopy = NC_PageCopy2P;
            #if MSFTL_LINUX
            FtlSdDev.pFnReadSendCmd = FTLSd_ReadSendCmd2P;
            #endif
            break;
        #endif
    }
    
    // --------------------------
    // re-config NAND parameters
    ftl_dbg(FTLSd_DBG_LV_LOG,0,"FTLSd: enable %u-Plane\n", FtlSdDev.u8_PlaneCnt);
    
    FtlSdDev.u16_PageByteCnt *= FtlSdDev.u8_PlaneCnt;
    FtlSdDev.u16_SpareByteCnt*= FtlSdDev.u8_PlaneCnt;
    FtlSdDev.u16_PBACnt = FtlSdDev.u16_PBACnt / FtlSdDev.u8_PlaneCnt;
    
    return FTLSd_OK;
}



