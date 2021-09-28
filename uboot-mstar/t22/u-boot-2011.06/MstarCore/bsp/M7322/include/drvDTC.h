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
#ifndef _DRV_DTC_H_
#define _DRV_DTC_H_

    #if defined(__aarch64__)

        #define DTC_MS_U32_d "d"
        #define DTC_MS_U32_i "i"
        #define DTC_MS_U32_x "x"
        #define DTC_MS_U32_X "X"
        #define DTC_MS_U32_u "u"

        #define DTC_MS_U64_d "ld"
        #define DTC_MS_U64_i "li"
        #define DTC_MS_U64_x "lx"
        #define DTC_MS_U64_X "lX"
        #define DTC_MS_U64_u "lu"

        #define DTC_MS_S32_d "d"
        #define DTC_MS_S32_i "i"
        #define DTC_MS_S32_x "x"
        #define DTC_MS_S32_X "X"
        #define DTC_MS_S32_u "u"

        #define DTC_MS_S64_d "ld"
        #define DTC_MS_S64_i "li"
        #define DTC_MS_S64_x "lx"
        #define DTC_MS_S64_X "lX"
        #define DTC_MS_S64_u "lu"

        #define DTC_MS_PHY_d DTC_MS_U64_d
        #define DTC_MS_PHY_i DTC_MS_U64_i
        #define DTC_MS_PHY_x DTC_MS_U64_x
        #define DTC_MS_PHY_X DTC_MS_U64_X
        #define DTC_MS_PHY_u DTC_MS_U64_u

        #define DTC_MS_VIRT_d "zd"
        #define DTC_MS_VIRT_i "zi"
        #define DTC_MS_VIRT_x "zx"
        #define DTC_MS_VIRT_X "zX"
        #define DTC_MS_VIRT_u "zu"

        #define DTC_MS_PHYADDR_d "zd"
        #define DTC_MS_PHYADDR_i "zi"
        #define DTC_MS_PHYADDR_x "zx"
        #define DTC_MS_PHYADDR_X "zX"
        #define DTC_MS_PHYADDR_u "zu"

    #elif defined(MSOS_TYPE_NUTTX)

        #define DTC_MS_U32_d "ld"
        #define DTC_MS_U32_i "li"
        #define DTC_MS_U32_x "lx"
        #define DTC_MS_U32_X "lX"
        #define DTC_MS_U32_u "lu"

        #define DTC_MS_U64_d "lld"
        #define DTC_MS_U64_i "lli"
        #define DTC_MS_U64_x "llx"
        #define DTC_MS_U64_X "llX"
        #define DTC_MS_U64_u "llu"

        #define DTC_MS_S32_d "ld"
        #define DTC_MS_S32_i "li"
        #define DTC_MS_S32_x "lx"
        #define DTC_MS_S32_X "lX"
        #define DTC_MS_S32_u "lu"

        #define DTC_MS_S64_d "lld"
        #define DTC_MS_S64_i "lli"
        #define DTC_MS_S64_x "llx"
        #define DTC_MS_S64_X "llX"
        #define DTC_MS_S64_u "llu"

        #define DTC_MS_PHY_d DTC_MS_U32_d
        #define DTC_MS_PHY_i DTC_MS_U32_i
        #define DTC_MS_PHY_x DTC_MS_U32_x
        #define DTC_MS_PHY_X DTC_MS_U32_X
        #define DTC_MS_PHY_u DTC_MS_U32_u

        #define DTC_MS_VIRT_d DTC_MS_U32_d
        #define DTC_MS_VIRT_i DTC_MS_U32_i
        #define DTC_MS_VIRT_x DTC_MS_U32_x
        #define DTC_MS_VIRT_X DTC_MS_U32_X
        #define DTC_MS_VIRT_u DTC_MS_U32_u

        #define DTC_MS_PHYADDR_d DTC_MS_PHY_d
        #define DTC_MS_PHYADDR_i DTC_MS_PHY_i
        #define DTC_MS_PHYADDR_x DTC_MS_PHY_x
        #define DTC_MS_PHYADDR_X DTC_MS_PHY_X
        #define DTC_MS_PHYADDR_u DTC_MS_PHY_u

    #else

        #if (defined(CONFIG_PURE_SN) || defined(CONFIG_MBOOT))

            #define DTC_MS_U32_d "ld"
            #define DTC_MS_U32_i "li"
            #define DTC_MS_U32_x "lx"
            #define DTC_MS_U32_X "lX"
            #define DTC_MS_U32_u "lu"

            #define DTC_MS_U64_d "lld"
            #define DTC_MS_U64_i "lli"
            #define DTC_MS_U64_x "llx"
            #define DTC_MS_U64_X "llX"
            #define DTC_MS_U64_u "llu"

            #define DTC_MS_S32_d "ld"
            #define DTC_MS_S32_i "li"
            #define DTC_MS_S32_x "lx"
            #define DTC_MS_S32_X "lX"
            #define DTC_MS_S32_u "lu"

            #define DTC_MS_S64_d "lld"
            #define DTC_MS_S64_i "lli"
            #define DTC_MS_S64_x "llx"
            #define DTC_MS_S64_X "llX"
            #define DTC_MS_S64_u "llu"

            #define DTC_MS_PHY_d DTC_MS_U32_d
            #define DTC_MS_PHY_i DTC_MS_U32_i
            #define DTC_MS_PHY_x DTC_MS_U32_x
            #define DTC_MS_PHY_X DTC_MS_U32_X
            #define DTC_MS_PHY_u DTC_MS_U32_u

            #define DTC_MS_VIRT_d DTC_MS_U32_d
            #define DTC_MS_VIRT_i DTC_MS_U32_i
            #define DTC_MS_VIRT_x DTC_MS_U32_x
            #define DTC_MS_VIRT_X DTC_MS_U32_X
            #define DTC_MS_VIRT_u DTC_MS_U32_u

            #define DTC_MS_PHYADDR_d DTC_MS_PHY_d
            #define DTC_MS_PHYADDR_i DTC_MS_PHY_i
            #define DTC_MS_PHYADDR_x DTC_MS_PHY_x
            #define DTC_MS_PHYADDR_X DTC_MS_PHY_X
            #define DTC_MS_PHYADDR_u DTC_MS_PHY_u

        #else

            #define DTC_MS_U32_d "d"
            #define DTC_MS_U32_i "i"
            #define DTC_MS_U32_x "x"
            #define DTC_MS_U32_X "X"
            #define DTC_MS_U32_u "u"

            #define DTC_MS_U64_d "lld"
            #define DTC_MS_U64_i "lli"
            #define DTC_MS_U64_x "llx"
            #define DTC_MS_U64_X "llX"
            #define DTC_MS_U64_u "llu"

            #define DTC_MS_S32_d "d"
            #define DTC_MS_S32_i "i"
            #define DTC_MS_S32_x "x"
            #define DTC_MS_S32_X "X"
            #define DTC_MS_S32_u "u"

            #define DTC_MS_S64_d "lld"
            #define DTC_MS_S64_i "lli"
            #define DTC_MS_S64_x "llx"
            #define DTC_MS_S64_X "llX"
            #define DTC_MS_S64_u "llu"

            #define DTC_MS_PHY_d DTC_MS_U64_d
            #define DTC_MS_PHY_i DTC_MS_U64_i
            #define DTC_MS_PHY_x DTC_MS_U64_x
            #define DTC_MS_PHY_X DTC_MS_U64_X
            #define DTC_MS_PHY_u DTC_MS_U64_u

            #define DTC_MS_VIRT_d "zd"
            #define DTC_MS_VIRT_i "zi"
            #define DTC_MS_VIRT_x "zx"
            #define DTC_MS_VIRT_X "zX"
            #define DTC_MS_VIRT_u "zu"

            #define DTC_MS_PHYADDR_d "zd"
            #define DTC_MS_PHYADDR_i "zi"
            #define DTC_MS_PHYADDR_x "zx"
            #define DTC_MS_PHYADDR_X "zX"
            #define DTC_MS_PHYADDR_u "zu"

        #endif

    #endif

#endif