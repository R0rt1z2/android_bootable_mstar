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

    //************************************************************
    //***             RIU Base address of each IP              ***
    //***                (Byte address [23:0])                 ***
    //************************************************************

#define    RIUBASE_RIU_DBG             0x100100    //0x100100 ~ 0x1001ff
#define    RIUBASE_VD_MHEG5            0x100300    //0x100300 ~ 0x1003ff
#define    RIUBASE_POR_STATUS          0x100500    //0x100500 ~ 0x10053f
#define    RIUBASE_INTR_CPUINT         0x100540    //0x100540 ~ 0x10057f
#define    RIUBASE_MIU2                0x100600    //0x100600 ~ 0x1006ff
#define    RIUBASE_USB0                0x100700    //0x100700 ~ 0x10077f
#define    RIUBASE_USB1                0x100780    //0x100780 ~ 0x1007ff
#define    RIUBASE_BDMA                0x100900    //0x100900 ~ 0x10097f
#define    RIUBASE_UART0               0x100980    //0x100980 ~ 0x1009ff
#define    RIUBASE_CLKGEN0             0x100b00    //0x100b00 ~ 0x100bff
#define    RIUBASE_DSCRMB              0x100c00    //0x100c00 ~ 0x100cff
#define    RIUBASE_UHC1                0x100d00    //0x100d00 ~ 0x100dff
#define    RIUBASE_MHEG5               0x100f00    //0x100f00 ~ 0x100fff
#define    RIUBASE_MVD                 0x101100    //0x101100 ~ 0x1011ff
#define    RIUBASE_MIU                 0x101200    //0x101200 ~ 0x1012ff
#define    RIUBASE_MVOPSUB             0x101300    //0x101300 ~ 0x1013ff
#define    RIUBASE_MVOP                0x101400    //0x101400 ~ 0x1014ff
#define    RIUBASE_TSP0                0x101500    //0x101500 ~ 0x1015ff
#define    RIUBASE_TSP1                0x101600    //0x101600 ~ 0x1016ff
#define    RIUBASE_JPD                 0x101700    //0x101700 ~ 0x1017ff
#define    RIUBASE_SEMAPH              0x101800    //0x101800 ~ 0x10183f
#define    RIUBASE_MAU0                0x101840    //0x101840 ~ 0x10185f
#define    RIUBASE_MAU1                0x101860    //0x101860 ~ 0x10187f
#define    RIUBASE_ECBRIDGE            0x101880    //0x101880 ~ 0x1018ff
#define    RIUBASE_INTR_CTRL           0x101900    //0x101900 ~ 0x1019ff
#define    RIUBASE_HDMI2               0x101a00    //0x101a00 ~ 0x101aff
#define    RIUBASE_HVD                 0x101b00    //0x101b00 ~ 0x101bff
#define    RIUBASE_TSP2                0x101c00    //0x101c00 ~ 0x101cff
#define    RIUBASE_MIPS                0x101d00    //0x101d00 ~ 0x101dff
#define    RIUBASE_CHIP                0x101e00    //0x101e00 ~ 0x101eff
#define    RIUBASE_GOP                 0x101f00    //0x101f00 ~ 0x101fff
#define    RIUBASE_EMAC0               0x102000    //0x102000 ~ 0x1020ff
#define    RIUBASE_EMAC1               0x102100    //0x102100 ~ 0x1021ff
#define    RIUBASE_EMAC2               0x102200    //0x102200 ~ 0x1022ff
#define    RIUBASE_EMAC3               0x102300    //0x102300 ~ 0x1023ff
#define    RIUBASE_UHC0                0x102400    //0x102400 ~ 0x1024ff
#define    RIUBASE_ADC_ATOP            0x102500    //0x102500 ~ 0x1025ff
#define    RIUBASE_ADC_DTOP            0x102600    //0x102600 ~ 0x1026ff
#define    RIUBASE_HDMI                0x102700    //0x102700 ~ 0x1027ff
#define    RIUBASE_GE0                 0x102800    //0x102800 ~ 0x1028ff
#define    RIUBASE_SMART               0x102900    //0x102900 ~ 0x1029ff
#define    RIUBASE_CI                  0x102a00    //0x102a00 ~ 0x102aff
#define    RIUBASE_CHIPGPIO            0x102b00    //0x102b00 ~ 0x102bff
#define    RIUBASE_SC0                 0x102e00    //0x102e00 ~ 0x102eff
#define    RIUBASE_SC1                 0x102f00    //0x102f00 ~ 0x102fff
#define    RIUBASE_SC2                 0x103000    //0x103000 ~ 0x1030ff
#define    RIUBASE_SC3                 0x103100    //0x103100 ~ 0x1031ff
#define    RIUBASE_SC4                 0x103200    //0x103200 ~ 0x1032ff
#define    RIUBASE_CLKGEN1             0x103300    //0x103300 ~ 0x10337f
#define    RIUBASE_MAILBOX             0x103380    //0x103380 ~ 0x1033ff
#define    RIUBASE_MIIC                0x103420    //0x103420 ~ 0x10343f
#define    RIUBASE_PCM                 0x103440    //0x103440 ~ 0x10345f
#define    RIUBASE_VDMCU51_IF          0x103460    //0x103460 ~ 0x10347f
#define    RIUBASE_DMDMCU51_IF         0x103480    //0x103480 ~ 0x10349f
#define    RIUBASE_URDMA               0x1034c0    //0x1034c0 ~ 0x1034ff
#define    RIUBASE_AFEC                0x103500    //0x103500 ~ 0x1035ff
#define    RIUBASE_COMB                0x103600    //0x103600 ~ 0x1036ff
#define    RIUBASE_VBI                 0x103700    //0x103700 ~ 0x1037ff
#define    RIUBASE_SCM                 0x103800    //0x103800 ~ 0x1038ff
#define    RIUBASE_UTMI2               0x103900    //0x103900 ~ 0x10397f
#define    RIUBASE_PATGEN              0x103980    //0x103980 ~ 0x1039ff
#define    RIUBASE_UTMI1               0x103a00    //0x103a00 ~ 0x103a7f
#define    RIUBASE_UTMI                0x103a80    //0x103a80 ~ 0x103aff
#define    RIUBASE_VE_0                0x103b00    //0x103b00 ~ 0x103bff
#define    RIUBASE_REG_PIU_NONPM       0x103c00    //0x103c00 ~ 0x103cff
#define    RIUBASE_VE_1                0x103e00    //0x103e00 ~ 0x103eff
#define    RIUBASE_VE_2                0x103f00    //0x103f00 ~ 0x103fff
#define    RIUBASE_MPIF                0x110400    //0x110400 ~ 0x1104ff
#define    RIUBASE_GPD                 0x110500    //0x110500 ~ 0x1105ff
#define    RIUBASE_UART1               0x110600    //0x110600 ~ 0x11063f
#define    RIUBASE_UART2               0x110640    //0x110640 ~ 0x11067f
#define    RIUBASE_FUART               0x110680    //0x110680 ~ 0x1106bf
#define    RIUBASE_GE1                 0x110700    //0x110700 ~ 0x1107ff
#define    RIUBASE_G3D                 0x110800    //0x110800 ~ 0x1108ff
#define    RIUBASE_DVI_ATOP            0x110900    //0x110900 ~ 0x1109ff
#define    RIUBASE_DVI_DTOP            0x110a00    //0x110a00 ~ 0x110a7f
#define    RIUBASE_DVIEQ               0x110a80    //0x110a80 ~ 0x110abf
#define    RIUBASE_HDCP                0x110ac0    //0x110ac0 ~ 0x110aff
#define    RIUBASE_NR_HSD              0x110b00    //0x110b00 ~ 0x110b7f
#define    RIUBASE_ANA_MISC            0x110c00    //0x110c00 ~ 0x110cff
#define    RIUBASE_MIU_ATOP            0x110d00    //0x110d00 ~ 0x110dff
#define    RIUBASE_NR                  0x110e00    //0x110e00 ~ 0x110eff
#define    RIUBASE_DI                  0x110f00    //0x110f00 ~ 0x110fff
#define    RIUBASE_MFE0                0x111000    //0x111000 ~ 0x1110ff
#define    RIUBASE_MFE1                0x111100    //0x111100 ~ 0x1111ff
#define    RIUBASE_ADC_DTOPB           0x111200    //0x111200 ~ 0x1112ff
#define    RIUBASE_NFIE0               0x111300    //0x111300 ~ 0x1113ff
#define    RIUBASE_NFIE1               0x111400    //0x111400 ~ 0x1114ff
#define    RIUBASE_NFIE2               0x111500    //0x111500 ~ 0x1115ff
#define    RIUBASE_ON0                 0x111600    //0x111600 ~ 0x1116ff
#define    RIUBASE_ON1                 0x111700    //0x111700 ~ 0x1117ff
#define    RIUBASE_MIIC0               0x111800    //0x111800 ~ 0x1118ff
#define    RIUBASE_MIIC1               0x111900    //0x111900 ~ 0x1119ff
#define    RIUBASE_MIIC2               0x111a00    //0x111a00 ~ 0x111aff
#define    RIUBASE_MIIC3               0x111b00    //0x111b00 ~ 0x111bff
#define    RIUBASE_CLKGEN_DMD          0x111f00    //0x111f00 ~ 0x111fff
#define    RIUBASE_DEMOD_0             0x112000    //0x112000 ~ 0x1120ff
#define    RIUBASE_DEMOD_1             0x112100    //0x112100 ~ 0x1121ff
#define    RIUBASE_DEMOD_2             0x112200    //0x112200 ~ 0x1122ff
#define    RIUBASE_DEMOD_3             0x112300    //0x112300 ~ 0x1123ff
#define    RIUBASE_DEMOD_4             0x112400    //0x112400 ~ 0x1124ff
#define    RIUBASE_DEMOD_5             0x112500    //0x112500 ~ 0x1125ff
#define    RIUBASE_DEMOD_6             0x112600    //0x112600 ~ 0x1126ff
#define    RIUBASE_DEMOD_7             0x112700    //0x112700 ~ 0x1127ff
#define    RIUBASE_DMD_ANA_MISC        0x112800    //0x112800 ~ 0x1128ff
#define    RIUBASE_AUR20               0x112900    //0x112900 ~ 0x1129ff
#define    RIUBASE_VIVALDI0            0x112a00    //0x112a00 ~ 0x112aff
#define    RIUBASE_VIVALDI1            0x112b00    //0x112b00 ~ 0x112bff
#define    RIUBASE_VIVALDI2            0x112c00    //0x112c00 ~ 0x112cff
#define    RIUBASE_VIVALDI3            0x112d00    //0x112d00 ~ 0x112dff
#define    RIUBASE_VIVALDI4            0x112e00    //0x112e00 ~ 0x112eff
#define    RIUBASE_VIVALDI5            0x112f00    //0x112f00 ~ 0x112fff
#define    RIUBASE_AUR21               0x113000    //0x113000 ~ 0x1130ff
#define    RIUBASE_AUR22               0x113100    //0x113100 ~ 0x1131ff
#define    RIUBASE_DVI_ATOP_1          0x113200    //0x113200 ~ 0x1132ff
#define    RIUBASE_DVI_DTOP_1          0x113300    //0x113300 ~ 0x11337f
#define    RIUBASE_DVIEQ_1             0x113380    //0x113380 ~ 0x1133bf
#define    RIUBASE_HDCP_1              0x1133c0    //0x1133c0 ~ 0x1133ff
#define    RIUBASE_DVI_ATOP_2          0x113400    //0x113400 ~ 0x1134ff
#define    RIUBASE_DVI_DTOP_2          0x113500    //0x113500 ~ 0x11357f
#define    RIUBASE_DVIEQ_2             0x113580    //0x113580 ~ 0x1135bf
#define    RIUBASE_HDCP_2              0x1135c0    //0x1135c0 ~ 0x1135ff
#define    RIUBASE_DVI_PS              0x113600    //0x113600 ~ 0x1136ff
#define    RIUBASE_DVI_DTOP_3          0x113700    //0x113700 ~ 0x11377f
#define    RIUBASE_DVIEQ_3             0x113780    //0x113780 ~ 0x1137bf
#define    RIUBASE_HDCP_3              0x1137c0    //0x1137c0 ~ 0x1137ff
#define    RIUBASE_USB2                0x113800    //0x113800 ~ 0x1138ff
#define    RIUBASE_UHC2                0x113900    //0x113900 ~ 0x1139ff
#define    RIUBASE_DSCRMB2             0x113c00    //0x113c00 ~ 0x113cff
#define    RIUBASE_DSCRMB3             0x113d00    //0x113d00 ~ 0x113dff
#define    RIUBASE_GPD0                0x113e00    //0x113e00 ~ 0x113eff
#define    RIUBASE_GPD1                0x113f00    //0x113f00 ~ 0x113fff
#define    RIUBASE_GOP4G_0             0x120200    //0x120200 ~ 0x1202ff
#define    RIUBASE_GOP4G_1             0x120300    //0x120300 ~ 0x1203ff
#define    RIUBASE_GOP4G_ST            0x120400    //0x120400 ~ 0x1204ff
#define    RIUBASE_GOP2G_0             0x120500    //0x120500 ~ 0x1205ff
#define    RIUBASE_GOP2G_1             0x120600    //0x120600 ~ 0x1206ff
#define    RIUBASE_GOP2G_ST            0x120700    //0x120700 ~ 0x1207ff
#define    RIUBASE_GOP1G_0             0x120800    //0x120800 ~ 0x1208ff
#define    RIUBASE_GOP1G_1             0x120900    //0x120900 ~ 0x1209ff
#define    RIUBASE_GOP1G_ST            0x120a00    //0x120a00 ~ 0x120aff
#define    RIUBASE_GOP1GX_0            0x120b00    //0x120b00 ~ 0x120bff
#define    RIUBASE_GOP1GX_1            0x120c00    //0x120c00 ~ 0x120cff
#define    RIUBASE_GOP1GX_ST           0x120d00    //0x120d00 ~ 0x120dff
#define    RIUBASE_GOPD                0x120e00    //0x120e00 ~ 0x120eff
#define    RIUBASE_SPARE0              0x121000    //0x121000 ~ 0x1210ff
#define    RIUBASE_SPARE1              0x121100    //0x121100 ~ 0x1211ff
#define    RIUBASE_ALBANY0             0x122800    //0x122800 ~ 0x1228ff
#define    RIUBASE_ALBANY1             0x122900    //0x122900 ~ 0x1229ff
#define    RIUBASE_MOBF                0x122f00    //0x122f00 ~ 0x122fff
#define    RIUBASE_DC_SCL              0x123000    //0x123000 ~ 0x1230ff
#define    RIUBASE_MSC                 0x130000    //0x130000 ~ 0x13ffff
#define    RIUBASE_GPUAPB              0x140000    //0x140000 ~ 0x14ffff
