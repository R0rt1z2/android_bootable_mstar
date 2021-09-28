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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief  NDS Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_NDS_H_
#define _DRV_NDS_H_


#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define NDS_ENFLAGS_CTRL_ENC_SUPPORTED  0x01
#define NDS_ENFLAGS_CTRL_ENC_HW_FORCED  0x02    // permanently
#define NDS_ENFLAGS_CTRL_ENC_SW_FORCED  0x04    // force until reset
#define NDS_ENFLAGS_JTAG_PWD            0x08
#define NDS_ENFLAGS_BOOTROM             0x10
#define NDS_ENFLAGS_DATA_ENC            0x20
#define NDS_ENFLAGS_GETRESP2CHALLENGE   0x40


// Predefined by NDS
// ref. icq063
#define NDS_KTE_ESA_SEL_AES             0x0
#define NDS_KTE_ESA_SEL_CSA             0x1
#define NDS_KTE_ESA_SEL_DES             0x2
#define NDS_KTE_ESA_SEL_TDES            0x3
#define NDS_KTE_ESA_SEL_MULTI2          0x4
#define NDS_KTE_ESA_SEL_DFAST           0x5
#define NDS_KTE_ESA_SEL_USER            0xF

#define NDS_KTE_ESA_SUB_ECB             0x0
#define NDS_KTE_ESA_SUB_CBC             0x1
#define NDS_KTE_ESA_SUB_NSA             0x7



//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_NDS_OK,
    E_NDS_FAIL,

} NDS_Result;


typedef enum
{
    E_NDS_SC_EVENT_CARD_REMOVED     =   0x00000000,
    E_NDS_SC_EVENT_CARD_INSERTED    =   0x00000001,

} NDS_SC_Event;


typedef enum
{
    E_NDS_SC_CLK_27M_D6,
    E_NDS_SC_CLK_27M_D2,
    E_NDS_SC_CLK_27M_D4,
    E_NDS_SC_CLK_27M_D8,

} NDS_SC_Clk;


typedef enum
{
    E_NDS_SC_BAUD_RATE_9600,
    E_NDS_SC_BAUD_RATE_19200,
    E_NDS_SC_BAUD_RATE_38400,
    E_NDS_SC_BAUD_RATE_76800,
    E_NDS_SC_BAUD_RATE_153600,
    E_NDS_SC_BAUD_RATE_223200,
} NDS_SC_BaudRate;


typedef enum
{
    E_NDS_CAP_ECM_NUM               =   0x00,

    E_NDS_CAP_EMM_NUM               =   0x10,

    E_NDS_CAP_BUF_PITCH             =   0x20,
    E_NDS_CAP_BUF_ALIGN,
    E_NDS_CAP_BUF_TOTALSIZE,
    E_NDS_CAP_RASP_BUF_TOTALSIZE,

} NDS_Caps;


typedef MS_BOOL                     (*P_NDS_SC_CbSetVcc)(MS_BOOL b5V);
typedef void                        (*P_NDS_SC_CbEvent)(NDS_SC_Event event);

typedef struct _NDS_SC_Param
{
    P_NDS_SC_CbSetVcc               cbSetVcc;

    P_NDS_SC_CbEvent                cbEvent;                            // for HDI certification
    MS_BOOL                         bCommDump;                          // for card debugging // @TODO: remove

} NDS_SC_Param;


typedef struct _NDS_FLT_Param
{
    MS_PHYADDR                      ecm_emm_addr;                       // NDS_CAP_BUF_SIZE*(NDS_CAP_ECM_NUM+NDS_CAP_EMM_NUM) at least
                                                                        // NDS_CAP_BUF_ALIGN aligned
    MS_U8*                          pu8ecm_emm_buf;                     // virtual address of ecm_emm_addr
    MS_U32                          ecm_emm_size;                       // total buffer size from emm_ecm_addr
} NDS_FLT_Param;

typedef struct _NDS_RASP_Param
{
    MS_PHYADDR                      ecm_addr;                       // NDS_CAP_BUF_SIZE * NDS_CAP_ECM_NUM
    MS_U8                          *pu8ecm_buf;                     // virtual address of ecm address
    MS_U32                          ecm_size;                       // total buffer size from emm_ecm_addr

    MS_PHYADDR                      payload_addr;                   // NDS_CAP_BUF_SIZE * NDS_CAP_ECM_NUM
    MS_U8                          *pu8playload_buf;                // virtual address of ecm address
    MS_U32                          payload_size;                   // total buffer size from emm_ecm_addr

} NDS_RASP_Param;

typedef struct _NDS_Param
{
    NDS_FLT_Param                   flt;
    NDS_SC_Param                    sc;
    NDS_RASP_Param                  rasp;
} NDS_Param;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

NDS_Result                          MDrv_NDS_Init(NDS_Param *param);
NDS_Result                          MDrv_NDS_Exit(void);
NDS_Result                          MDrv_NDS_PowerOff(void);
NDS_Result                          MDrv_NDS_SetMagicValue(MS_U8 idx, MS_U16 word_0, MS_U16 word_1, MS_U16 word_2, MS_U16 word_3);
MS_U32                              MDrv_NDS_GetCaps(NDS_Caps cap);

NDS_Result                          MDrv_NDS_NSK_Open(MS_U32 nsk_id);
NDS_Result                          MDrv_NDS_NSK_Close(MS_U32 nsk_id);


#ifdef __cplusplus
}
#endif

#endif // _DRV_NDS_H_

