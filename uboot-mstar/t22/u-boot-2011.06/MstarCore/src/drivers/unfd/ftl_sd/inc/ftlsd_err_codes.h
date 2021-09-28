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
#ifndef _FTLSd_ERR_CODES_
#define _FTLSd_ERR_CODES_

#define  FTLSd_OK                             0

#define  FTLSd_ERR_NOT_READY                  (0x0000 | FTLSd_ST_PLAT)
#define  FTLSd_ERR_NO_FTLSd_PART              (0x0001 | FTLSd_ST_PLAT)
#define  FTLSd_ERR_OUT_OF_RANGE               (0x0002 | FTLSd_ST_PLAT)
#define  FTLSd_ERR_NAND_PARAM                 (0x0003 | FTLSd_ST_PLAT)
#define  FTLSd_ERR_PART_PARAM                 (0x0004 | FTLSd_ST_PLAT)
#define  FTLSd_ERR_DEVST_CORRUPT              (0x0005 | FTLSd_ST_PLAT)
#define  FTLSd_ERR_GET_MEM_FAIL               (0x0006 | FTLSd_ST_PLAT)
#define  FTLSd_ERR_TEST_FAIL                  (0x0007 | FTLSd_ST_PLAT)


#define  FTLSd_ERR_PARAMETER                  (0x0010 | FTLSd_ST_PLAT)
#define  FTLSd_ERR_W_OUT_RANGE                (0x0011 | FTLSd_ST_PLAT)
#define  FTLSd_ERR_R_OUT_RANGE                (0x0012 | FTLSd_ST_PLAT)
#define  FTLSd_ERR_R_SECTOR                   (0x0013 | FTLSd_ST_PLAT)


#define  FTLSd_ERR_W                          (0x8000 | FTLSd_ST_PLAT)
#define  FTLSd_ERR_R                          (0x8001 | FTLSd_ST_PLAT)
#define  FTLSd_ERR_GETMEM                     (0x8002 | FTLSd_ST_PLAT)

#define  FTLSd_ERR_FATAL                      (0xFFFF | FTLSd_ST_PLAT)

#endif // _FTLSd_ERR_CODES_

