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
#ifndef __MSTAR_MBX_HEAD_H__
#define __MSTAR_MBX_HEAD_H__

#define DEBUG 0

#if DEBUG //debug
#include "mbx_debug.h"
#elif defined(CONFIG_MSTAR_CHIP) //Kernel
#include "mbx_linux.h"
#elif defined(CFG_MSTAR_BOARD) //optee
#include "mbx_optee.h"
#else //Mboot
#include "mbx_mboot.h"
#endif

#include <config.h>

typedef enum {
	CMD_R2T_GENERAL = 0x6006,
	CMD_R2T_RPC_DONE,
	CMD_T2R_GENERAL_DONE,
	CMD_T2R_RPC,
	CMD_NULL = 0x8898,
} OPTEE_MBX_CMD_ID;

typedef struct {		// Compatible struct smc_param
	uint64_t version;	// version check
	uint64_t cmd_id;	// OPTEE_MBX_CMD_ID
	uint64_t a0;		// funcid
	uint64_t a1;
	uint64_t a2;
	uint64_t a3;
	uint64_t a4;
	uint64_t a5;
	uint64_t a6;
	uint64_t a7;
	uint64_t reserve[8];
} mbx_smc_args;

TEE_Result register_mbx_base(mbx_smc_args *base);
TEE_Result send_mbx(thread_smc_args *args,OPTEE_MBX_CMD_ID id, uint32_t mbx_slot);

extern mbx_smc_args *Sec_ARM_MBX_Base;
/* Example 1 Mboot
 * struct thread_smc_args args;
 * args.a0 = fase_call_id;
 * args.a1 = parameter; //value or point depend on case
 * args.a2 = parameter; //value or point depend on case
 * args.a3 = parameter; //value or point depend on case Max parameter size
 * register_mbx_base(MBX_BASE); //ask optee owner Sec_ARM Mbx Address depend on chip
 * if(send_mbx(&args, CMD_R2T_GENERAL, 0) != 0) //mbx_slot always = 0
 * {
 * 		printf("Error\n");
 * }
 * //Get result from args.a0
 * //Get return parameter from args.a1
 * //Get return parameter from args.a2
 * //Get return parameter from args.a3
 */
#endif
