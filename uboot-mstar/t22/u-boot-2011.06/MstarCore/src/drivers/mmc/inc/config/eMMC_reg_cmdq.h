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
#ifndef __eMMC_REG_CMDQ_H__
#define __eMMC_REG_CMDQ_H__

#ifndef BIT0
#define BIT0    (1<<0)
#endif
#ifndef BIT1
#define BIT1    (1<<1)
#endif
#ifndef BIT2
#define BIT2    (1<<2)
#endif
#ifndef BIT3
#define BIT3    (1<<3)
#endif
#ifndef BIT4
#define BIT4    (1<<4)
#endif
#ifndef BIT5
#define BIT5    (1<<5)
#endif
#ifndef BIT6
#define BIT6    (1<<6)
#endif
#ifndef BIT7
#define BIT7    (1<<7)
#endif
#ifndef BIT8
#define BIT8    (1<<8)
#endif
#ifndef BIT9
#define BIT9    (1<<9)
#endif
#ifndef BIT10
#define BIT10   (1<<10)
#endif
#ifndef BIT11
#define BIT11   (1<<11)
#endif
#ifndef BIT12
#define BIT12   (1<<12)
#endif
#ifndef BIT13
#define BIT13   (1<<13)
#endif
#ifndef BIT14
#define BIT14   (1<<14)
#endif
#ifndef BIT15
#define BIT15   (1<<15)
#endif
#ifndef BIT16
#define BIT16   (1<<16)
#endif
#ifndef BIT17
#define BIT17   (1<<17)
#endif
#ifndef BIT18
#define BIT18   (1<<18)
#endif
#ifndef BIT19
#define BIT19   (1<<19)
#endif
#ifndef BIT20
#define BIT20   (1<<20)
#endif
#ifndef BIT21
#define BIT21   (1<<21)
#endif
#ifndef BIT22
#define BIT22   (1<<22)
#endif
#ifndef BIT23
#define BIT23   (1<<23)
#endif
#ifndef BIT24
#define BIT24   (1<<24)
#endif
#ifndef BIT25
#define BIT25   (1<<25)
#endif
#ifndef BIT26
#define BIT26   (1<<26)
#endif
#ifndef BIT27
#define BIT27   (1<<27)
#endif
#ifndef BIT28
#define BIT28   (1<<28)
#endif
#ifndef BIT29
#define BIT29   (1<<29)
#endif
#ifndef BIT30
#define BIT30   (1<<30)
#endif
#ifndef BIT31
#define BIT31   (1<<31)
#endif

//------------------------------------------------------------------
#define FCIE_CMDQ_INT                   GET_REG_ADDR(FCIE2_BASE, 0x40)
#define FCIE_CMDQ_INT_EN                GET_REG_ADDR(FCIE2_BASE, 0x41)
#define FCIE_CMDQ_STS                   GET_REG_ADDR(FCIE2_BASE, 0x42)
#define FCIE_TSK_RDY0                   GET_REG_ADDR(FCIE2_BASE, 0x43)
#define FCIE_TSK_RDY1                   GET_REG_ADDR(FCIE2_BASE, 0x44)
#define FCIE_TSK_EMPTY0                 GET_REG_ADDR(FCIE2_BASE, 0x45)
#define FCIE_TSK_EMPTY1                 GET_REG_ADDR(FCIE2_BASE, 0x46)
#define FCIE_TSK_DONE0                  GET_REG_ADDR(FCIE2_BASE, 0x47)
#define FCIE_TSK_DONE1                  GET_REG_ADDR(FCIE2_BASE, 0x48)
#define FCIE_CHK_CNT                    GET_REG_ADDR(FCIE2_BASE, 0x49)
#define FCIE_RCA                        GET_REG_ADDR(FCIE2_BASE, 0x4A)
#define FCIE_TSK_REQ0                   GET_REG_ADDR(FCIE2_BASE, 0x4B)
#define FCIE_TSK_REQ1                   GET_REG_ADDR(FCIE2_BASE, 0x4C)
#define FCIE_CMDQ_CTL                   GET_REG_ADDR(FCIE2_BASE, 0x4D)
#define FCIE_CMDQ_FIFO_CTL              GET_REG_ADDR(FCIE2_BASE, 0x4E)
#define FCIE_HALT_CTL                   GET_REG_ADDR(FCIE2_BASE, 0x4F)
#define FCIE_DCMD_CTL                   GET_REG_ADDR(FCIE2_BASE, 0x50)
#define FCIE_DCMD_IDX                   GET_REG_ADDR(FCIE2_BASE, 0x51)
#define FCIE_DCMD_ARG0                  GET_REG_ADDR(FCIE2_BASE, 0x52)
#define FCIE_DCMD_ARG1                  GET_REG_ADDR(FCIE2_BASE, 0x53)
#define FCIE_DCMD_RSP0                  GET_REG_ADDR(FCIE2_BASE, 0x54)
#define FCIE_DCMD_RSP1                  GET_REG_ADDR(FCIE2_BASE, 0x55)
#define FCIE_MMA_CTL                    GET_REG_ADDR(FCIE2_BASE, 0x56)
#define FCIE_RSP_IDX                    GET_REG_ADDR(FCIE2_BASE, 0x57)
#define FCIE_RSP_ARG0                   GET_REG_ADDR(FCIE2_BASE, 0x58)
#define FCIE_RSP_ARG1                   GET_REG_ADDR(FCIE2_BASE, 0x59)
#define FCIE_DEBUG_MOD                  GET_REG_ADDR(FCIE2_BASE, 0x5A)

//------------------------------------------------------------------
/* FCIE_CMDQ_INT        0x40 */
/* FCIE_CMDQ_INT_EN     0x41 */
#define BIT_CMDQ_END_INT                BIT0
#define BIT_HALT_END_INT                BIT1
#define BIT_DCMD_END_INT                BIT2

/* FCIE_CMDQ_STS        0x42 */
#define BIT_RSP_IDX_ERR                 BIT0
#define BIT_RSP_R1_ERR                  BIT1

/* FCIE_CMDQ_CTL        0x4D */
#define BIT_CMDQ_EN                     BIT0

/* FCIE_CMDQ_FIFO_CTL   0x4E */
#define BIT_CMD_FIFO_FULL_TRIG          BIT0

/* FCIE_HALT_CTL        0x4F */
#define BIT_HALT_EN                     BIT0
#define BIT_HALT_OPCODE_MASK            (BIT7|BIT6|BIT5|BIT4)
#define BIT_HALT_TASK_ID_MASK           (BIT15|BIT14|BIT13|BIT12|BIT11|BIT10|BIT9|BIT8)

/* FCIE_DCMD_CTL        0x50 */
#define BIT_DCMD_EN                     BIT0

/* FCIE_DCMD_IDX        0x51 */
#define BIT_DCMD_IDX_MASK               (BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0)
#define BIT_DCMD_RSP_IDX_MASK           (BIT15|BIT14|BIT13|BIT12|BIT11|BIT10|BIT9|BIT8)

/* FCIE_DEBUG_MOD       0x5A */
#define BIT_CMDQ_DBUG_MOD_MASK          (BIT1|BIT0)

struct _LinkDescriptor
{
	U32	u32_End     : 1;
	U32	u32_MiuSel  : 2;
	U32             : 1;
    U32 u32_Link    : 1;
    U32 u32_Dir     : 1;
    U32             : 10;
	U32 u32_JobCnt  : 16;
	U32 u32_Address;
    U32 u32_Length;
    U32 u32_Dummy;
};

#endif /* __eMMC_REG_CMDQ_H__ */
