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

#ifndef __HWREG_H__
#define __HWREG_H__

#include "mhal_arch.h"

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_S4LE)
    #include "s4le/riu_map.h"
    #include "s4le/c_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7)
    #include "s7/riu_map.h"
    #include "s7/c_riubase.h"
    #include "s7/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_U3)
    #include "u3/riu_map.h"
    #include "u3/c_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD)
    #include "s7ld/riu_map.h"
    #include "s7ld/c_riubase.h"
    #include "s7ld/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LA)
    #include "s7la/riu_map.h"
    #include "s7la/c_riubase.h"
    #include "s7la/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7J)
    #include "s7j/riu_map.h"
    #include "s7j/c_riubase.h"
    #include "s7j/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_A3)
    #include "a3/riu_map.h"
    #include "a3/c_riubase.h"
    #include "a3/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_A5)
    #include "a5/riu_map.h"
    #include "a5/c_riubase.h"
    #include "a5/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_A5P)
    #include "a5p/riu_map.h"
    #include "a5p/c_riubase.h"
    #include "a5p/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S8)
    #include "s8/riu_map.h"
    #include "s8/c_riubase.h"
    #include "s8/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_S7L)
    #include "s7l/riu_map.h"
    #include "s7l/c_riubase.h"
    #include "s7l/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_U4)
    #include "s7l/riu_map.h"
    #include "s7l/c_riubase.h"
    #include "s7l/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_K1)
    #include "k1/riu_map.h"
    #include "k1/c_riubase.h"
    #include "k1/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KENYA)
    #include "kenya/riu_map.h"
    #include "kenya/c_riubase.h"
    #include "kenya/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KELTIC)
    #include "keltic/riu_map.h"
    #include "keltic/c_riubase.h"
    #include "keltic/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KAPPA)
    #include "kappa/riu_map.h"
    #include "kappa/c_riubase.h"
    #include "kappa/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KRITI)
    #include "kriti/riu_map.h"
    #include "kriti/c_riubase.h"
    #include "kriti/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_K5AP)
    #include "k5ap/riu_map.h"
    #include "k5ap/c_riubase.h"
    #include "k5ap/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KRATOS)
    #include "kratos/riu_map.h"
    #include "kratos/c_riubase.h"
    #include "kratos/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KIWI)
    #include "kiwi/riu_map.h"
    #include "kiwi/c_riubase.h"
    #include "kiwi/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_K5TN)
    #include "k5tn/riu_map.h"
    #include "k5tn/c_riubase.h"
    #include "k5tn/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KERES)
    #include "keres/riu_map.h"
    #include "keres/c_riubase.h"
    #include "keres/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_K2)
    #include "k2/riu_map.h"
    #include "k2/c_riubase.h"
    #include "k2/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MARIA10)
    #include "maria10/riu_map.h"
    #include "maria10/c_riubase.h"
    #include "maria10/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_J2)
    #include "j2/riu_map.h"
    #include "j2/c_riubase.h"
    #include "j2/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_T11)
    #include "t11/riu_map.h"
    #include "t11/c_riubase.h"
    #include "t11/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_A1)
    #include "a1/riu_map.h"
    #include "a1/c_riubase.h"
    #include "a1/c_pm_riubase.h"
    #include "a1/c_dmdmcu_riubase.h"
    #include "a1/c_vdmcu_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_A6)
    #include "a6/riu_map.h"
    #include "a6/c_riubase.h"
    #include "a6/c_pm_riubase.h"
    #include "a6/c_dmdmcu_riubase.h"
    #include "a6/c_vdmcu_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_A7)
    #include "a7/riu_map.h"
    #include "a7/c_riubase.h"
    #include "a7/c_pm_riubase.h"
    #include "a7/c_dmdmcu_riubase.h"
    #include "a7/c_vdmcu_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_AMETHYST)
    #include "amethyst/riu_map.h"
    #include "amethyst/c_riubase.h"
    #include "amethyst/c_pm_riubase.h"
    #include "amethyst/c_dmdmcu_riubase.h"
    #include "amethyst/c_vdmcu_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_AGATE)
    #include "agate/riu_map.h"
    #include "agate/c_riubase.h"
    #include "agate/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_EAGLE)
    #include "eagle/riu_map.h"
    #include "eagle/c_riubase.h"
    #include "eagle/c_pm_riubase.h"
//  #include "eagle/c_dmdmcu_riubase.h"
    #include "eagle/c_vdmcu_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_EMERALD)
    #include "emerald/riu_map.h"
    #include "emerald/c_riubase.h"
    #include "emerald/c_pm_riubase.h"
    #include "emerald/c_dmdmcu_riubase.h"
    #include "emerald/c_vdmcu_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KAISER)
    #include "kaiser/riu_map.h"
    #include "kaiser/c_riubase.h"
    #include "kaiser/c_pm_riubase.h"
    #include "kaiser/c_vdmcu_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MACAW12)
    #include "macaw12/riu_map.h"
    #include "macaw12/c_riubase.h"
    #include "macaw12/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_EDISON)
    #include "edison/riu_map.h"
    #include "edison/c_riubase.h"
    #include "edison/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_EINSTEIN)
    #include "einstein/riu_map.h"
    #include "einstein/c_riubase.h"
    #include "einstein/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_NAPOLI)
    #include "napoli/riu_map.h"
    #include "napoli/c_riubase.h"
    #include "napoli/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_EDEN)
    #include "eden/riu_map.h"
    #include "eden/c_riubase.h"
    #include "eden/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_NASA)
    #include "nasa/riu_map.h"
    #include "nasa/c_riubase.h"
    #include "nasa/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_EULER)
    #include "euler/riu_map.h"
    #include "euler/c_riubase.h"
    #include "euler/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_EIFFEL)
    #include "eiffel/riu_map.h"
    #include "eiffel/c_riubase.h"
    #include "eiffel/c_pm_riubase.h"
//  #include "eiffel/c_dmdmcu_riubase.h"
    #include "eiffel/c_vdmcu_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_NIKE)
    #include "nike/riu_map.h"
    #include "nike/c_riubase.h"
    #include "nike/c_pm_riubase.h"
    #include "nike/c_dmd_riubase.h"
    #include "nike/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MADISON)
    #include "madison/riu_map.h"
    #include "madison/c_riubase.h"
    #include "madison/c_pm_riubase.h"
    #include "madison/c_dmd_riubase.h"
    #include "madison/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MONET)
    #include "monet/riu_map.h"
    #include "monet/c_riubase.h"
    #include "monet/c_pm_riubase.h"
    #include "monet/c_dmd_riubase.h"
    #include "monet/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MOONEY)
    #include "mooney/riu_map.h"
    #include "mooney/c_riubase.h"
    #include "mooney/c_pm_riubase.h"
    #include "mooney/c_dmd_riubase.h"
    #include "mooney/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7221)
    #include "m7221/riu_map.h"
    #include "m7221/c_riubase.h"
    #include "m7221/c_pm_riubase.h"
    #include "m7221/c_dmd_riubase.h"
    #include "m7221/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MESSI)
    #include "messi/riu_map.h"
    #include "messi/c_riubase.h"
    #include "messi/c_pm_riubase.h"
    #include "messi/c_dmd_riubase.h"
    #include "messi/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_CELTICS)
    #include "celtics/riu_map.h"
    #include "celtics/c_riubase.h"
    #include "celtics/c_pm_riubase.h"
    #include "celtics/c_dmd_riubase.h"
    #include "celtics/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KANO)
    #include "kano/riu_map.h"
    #include "kano/c_riubase.h"
    #include "kano/c_pm_riubase.h"
    #include "kano/c_dmd_riubase.h"
    #include "kano/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_K6)
    #include "k6/riu_map.h"
    #include "k6/c_riubase.h"
    #include "k6/c_pm_riubase.h"
    #include "k6/c_dmd_riubase.h"
    #include "k6/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_K6LITE)
    #include "k6lite/riu_map.h"
    #include "k6lite/c_riubase.h"
    #include "k6lite/c_pm_riubase.h"
    #include "k6lite/c_dmd_riubase.h"
    #include "k6lite/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_K7U)
    #include "k7u/riu_map.h"
    #include "k7u/c_riubase.h"
    #include "k7u/c_pm_riubase.h"
    #include "k7u/c_dmd_riubase.h"
    #include "k7u/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MIAMI)
    #include "miami/riu_map.h"
    #include "miami/c_riubase.h"
    #include "miami/c_pm_riubase.h"
    #include "miami/c_dmd_riubase.h"
    #include "miami/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_NADAL)
    #include "nadal/riu_map.h"
    #include "nadal/c_riubase.h"
    #include "nadal/c_pm_riubase.h"
    #include "nadal/c_dmd_riubase.h"
    #include "nadal/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_NUGGET)
    #include "nugget/riu_map.h"
    #include "nugget/c_riubase.h"
    #include "nugget/c_pm_riubase.h"
    #include "nugget/c_dmdmcu_riubase.h"
    #include "nugget/c_vdmcu_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_M5321)
    #include "m5321/riu_map.h"
    #include "m5321/c_riubase.h"
    #include "m5321/c_pm_riubase.h"
    #include "m5321/c_dmdmcu_riubase.h"
    #include "m5321/c_vdmcu_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_NIKON)
    #include "nikon/riu_map.h"
    #include "nikon/c_riubase.h"
    #include "nikon/c_pm_riubase.h"
    #include "nikon/c_dmdmcu_riubase.h"
    #include "nikon/c_vdmcu_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MILAN)
    #include "milan/riu_map.h"
    #include "milan/c_riubase.h"
    #include "milan/c_pm_riubase.h"
    #include "milan/c_dmdmcu_riubase.h"
    #include "milan/c_vdmcu_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MARLON)
    #include "marlon/riu_map.h"
    #include "marlon/c_riubase.h"
    #include "marlon/c_pm_riubase.h"
    #include "marlon/c_dmdmcu_riubase.h"
    #include "marlon/c_vdmcu_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MONACO)
    #include "monaco/riu_map.h"
    #include "monaco/c_riubase.h"
    #include "monaco/c_pm_riubase.h"
    #include "monaco/c_dmd_riubase.h"
    #include "monaco/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_CLIPPERS)
    #include "clippers/riu_map.h"
    #include "clippers/c_riubase.h"
    #include "clippers/c_pm_riubase.h"
    #include "clippers/c_dmd_riubase.h"
    #include "clippers/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUJI)
    #include "muji/riu_map.h"
    #include "muji/c_riubase.h"
    #include "muji/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MANHATTAN)
    #include "manhattan/riu_map.h"
    #include "manhattan/c_riubase.h"
    #include "manhattan/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MASERATI)
    #include "maserati/riu_map.h"
    #include "maserati/c_riubase.h"
    #include "maserati/c_pm_riubase.h"
    #include "maserati/c_dmd_riubase.h"
    #include "maserati/c_m2_riubase.h"
    #include "maserati/c_frc_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MCLAREN)
    #include "maserati/riu_map.h"
    #include "maserati/c_riubase.h"
    #include "maserati/c_pm_riubase.h"
    #include "maserati/c_dmd_riubase.h"
    #include "maserati/c_m2_riubase.h"
    #include "maserati/c_frc_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MACAN)
    #include "macan/riu_map.h"
    #include "macan/c_riubase.h"
    #include "macan/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_M3822)
    #include "m3822/riu_map.h"
    #include "m3822/c_riubase.h"
    #include "m3822/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUNICH)
    #include "munich/riu_map.h"
    #include "munich/c_riubase.h"
    #include "munich/c_pm_riubase.h"
    #include "munich/c_dmd_riubase.h"
    #include "munich/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MATISSE)
    #include "matisse/riu_map.h"
    #include "matisse/c_riubase.h"
    #include "matisse/c_pm_riubase.h"
    #include "matisse/c_dmd_riubase.h"
    #include "matisse/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MUSTANG)
    #include "mustang/riu_map.h"
    #include "mustang/c_riubase.h"
    #include "mustang/c_pm_riubase.h"
    #include "mustang/c_dmd_riubase.h"
    #include "mustang/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MARVEL)
    #include "marvel/riu_map.h"
    #include "marvel/c_riubase.h"
    #include "marvel/c_pm_riubase.h"
    #include "marvel/c_dmd_riubase.h"
    #include "marvel/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_CLIPPERS)
    #include "clippers/riu_map.h"
    #include "clippers/c_riubase.h"
    #include "clippers/c_pm_riubase.h"
    #include "clippers/c_dmd_riubase.h"
    #include "clippers/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_KRIS)
    #include "kris/riu_map.h"
    #include "kris/c_riubase.h"
    #include "kris/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_K1P)
    #include "k1p/riu_map.h"
    #include "k1p/c_riubase.h"
    #include "k1p/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MAYA)
    #include "maya/riu_map.h"
    #include "maya/c_riubase.h"
    #include "maya/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_WHISKY)
    #include "whisky/riu_map.h"
    #include "whisky/c_riubase.h"
    #include "whisky/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_CURRY)
    #include "curry/riu_map.h"
    #include "curry/c_riubase.h"
    #include "curry/c_pm_riubase.h"
    #include "curry/c_dmd_riubase.h"
    #include "curry/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_C2P)
    #include "c2p/riu_map.h"
    #include "c2p/c_riubase.h"
    #include "c2p/c_pm_riubase.h"
    #include "c2p/c_dmd_riubase.h"
    #include "c2p/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_C2S)
    #include "c2s/riu_map.h"
    #include "c2s/c_riubase.h"
    #include "c2s/c_pm_riubase.h"
    #include "c2s/c_dmd_riubase.h"
    #include "c2s/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MELODY)
    #include "melody/riu_map.h"
    #include "melody/c_riubase.h"
    #include "melody/c_m2_riubase.h"
 #elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MYSTIC)
    #include "mystic/riu_map.h"
    #include "mystic/c_riubase.h"
    #include "mystic/c_pm_riubase.h"
    #include "mystic/c_dmd_riubase.h"
    #include "mystic/c_m2_riubase.h"
    #include "mystic/c_frc_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MAXIM)
    #include "maxim/riu_map.h"
    #include "maxim/c_riubase.h"
    #include "maxim/c_pm_riubase.h"
    #include "maxim/c_dmd_riubase.h"
    #include "maxim/c_m2_riubase.h"
    #include "maxim/c_frc_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_MARCUS)
    #include "marcus/riu_map.h"
    #include "marcus/c_riubase.h"
    #include "marcus/c_pm_riubase.h"
    #include "marcus/c_dmd_riubase.h"
    #include "marcus/c_m2_riubase.h"
    #include "marcus/c_frc_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7622)
    #include "m7622/riu_map.h"
    #include "m7622/c_riubase.h"
    #include "m7622/c_pm_riubase.h"
    #include "m7622/c_dmd_riubase.h"
    #include "m7622/c_m2_riubase.h"
    #include "m7622/c_frc_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7322)
    #include "m7322/riu_map.h"
    #include "m7322/c_riubase.h"
    #include "m7322/c_pm_riubase.h"
    #include "m7322/c_dmd_riubase.h"
    #include "m7322/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7632)
    #include "m7632/riu_map.h"
    #include "m7632/c_riubase.h"
    #include "m7632/c_pm_riubase.h"
    #include "m7632/c_dmd_riubase.h"
    #include "m7632/c_m2_riubase.h"
#elif (CHIP_FAMILY_TYPE == CHIP_FAMILY_M7332)
    #include "m7332/riu_map.h"
    #include "m7332/c_riubase.h"
    #include "m7332/c_pm_riubase.h"
    #include "m7332/c_dmd_riubase.h"
    #include "m7332/c_m2_riubase.h"
#endif

#endif /* __HWREG_H__ */
