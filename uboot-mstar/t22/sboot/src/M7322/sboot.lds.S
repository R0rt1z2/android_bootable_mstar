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


OUTPUT_ARCH(arm)
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")

ENTRY(_vector)

MEMORY
{
#if (CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH == 1)
    boot :  ORIGIN = 0x1fc00000,                LENGTH = 56K
#if (CONFIG_TEE_LOADER == 1)
    tee_loader :  ORIGIN = CONFIG_TEE_LOADER_START_ADDRESS,        LENGTH = 36K
    ram :  ORIGIN = CONFIG_BOOTRAM_LOADADDR,        LENGTH = 36K
    TLbss :ORIGIN = CONFIG_TEE_LOADER_START_ADDRESS+LENGTH(tee_loader),    LENGTH = 64K
#else
    ram :   ORIGIN = CONFIG_BOOTRAM_LOADADDR,   LENGTH = 32K
#endif
#elif (CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH == 1)
    boot :  ORIGIN = 0x1fc00000,                LENGTH = 56K
#ifdef CONFIG_DOUBLE_MBOOT
    ram1 :  ORIGIN = CONFIG_BOOTRAM_LOADADDR + 32k,   LENGTH = 5K
    ram2 :  ORIGIN = CONFIG_BOOTRAM_LOADADDR,   LENGTH = 32K
#elif (CONFIG_TEE_LOADER == 1)
    tee_loader :  ORIGIN = CONFIG_TEE_LOADER_START_ADDRESS,        LENGTH = 36K
    ram :  ORIGIN = CONFIG_BOOTRAM_LOADADDR,        LENGTH = 36K
    TLbss :ORIGIN = CONFIG_TEE_LOADER_START_ADDRESS+LENGTH(tee_loader),    LENGTH = 64K
#else
    ram :  ORIGIN = CONFIG_BOOTRAM_LOADADDR,    LENGTH = 32K
#endif
#else
    boot :  ORIGIN = 0x00000000,                LENGTH = 8K
    rom :   ORIGIN = 0x00000000+0x2000,         LENGTH = 18K
    ram :   ORIGIN = CONFIG_BOOTRAM_LOADADDR,   LENGTH = 128K
#endif
}

_CONFIG_SRAM_START_ADDRESS = ORIGIN(boot);
_CONFIG_SRAM_CODE_SIZE     = LENGTH(boot);
_CONFIG_SRAM_SECURE_SIZE = 512;
_CONFIG_HASH1_SIZE = (_CONFIG_SRAM_CODE_SIZE - _CONFIG_SRAM_SECURE_SIZE);
_CONFIG_HASH1_END = (_CONFIG_SRAM_START_ADDRESS + _CONFIG_HASH1_SIZE);

SECTIONS
{
#if (CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH == 1)
    .text1 :
    {
        _ld_boot_start = ABSOLUTE(.);
#ifdef CONFIG_MSTAR_BUILD_TEE_LOADER_DAT
        LONG(0x00000000);
        _ld_bootrom_start = ABSOLUTE(.);
        _ld_program_reload_addr = ABSOLUTE(.);
        _ld_bootrom_end = ABSOLUTE(.);
        _ld_bootrom_size = _ld_bootrom_end - _ld_bootrom_start;
        _ld_miu_flow_start = ABSOLUTE(.);
        _ld_miu_flow_end = ABSOLUTE(.);
        _ld_miu_flow_size = _ld_miu_flow_end - _ld_miu_flow_start;
        _ld_miu_str_flow_start = ABSOLUTE(.);
#else
        *boot.o             (.text*)

        . = ALIGN(16);
        _ld_boot_end = ABSOLUTE(.);
        _ld_boot_size = _ld_boot_end - _ld_boot_start;

        _ld_bootrom_start = ABSOLUTE(.);
        *bootrom.o              (.text*)
        *bootrom_cache.o        (.text*)
        *drvRIU.o               (.text*)
        *drvAESDMA.o            (.text*)
        *drvAESDMA.o            (.rodata*)
        *halAESDMA.o            (.text*)
#ifdef CONFIG_TEE_LOADER
        *drvSEAL.o              (.text*)
        *halSEAL.o              (.text*)
        *halSEAL.o              (.rodata*)
        *halAESDMA_v2.o         (.text*)
        *drvAESDMA_v2.o         (.text*)
        *drvAESDMA_v2.o         (.rodata*)
        *(EMMCCODE)
#endif
        *drvNAND_prg.o          (.text*)
        *drvNAND_hal.o          (.text*)
        *drvNAND_hal_v5.o       (.text*)
        *drvNAND_platform.o     (.text*)
        *_dvmd_tls.o            (.text*)
        *_udivsi3.o             (.text*)        
        *drvNAND_prg.o          (.data*)
        *drvNAND_prg.o          (.bss*)
        *drvNAND_hal.o          (.bss*)
        *drvNAND_hal_v5.o       (.bss*)
        *drvNAND_platform.o     (.bss*)
        *bootrom_data_miu.o     (.rodata*)

        *bootrom_pll.o          (.text*)
        *bootrom_clock.o        (.text*)
        *bootrom_peripheral.o   (.text*)
        *bootrom.o              (.rodata*)
/*      *drvPadConf.o           (.rodata*) */

        KEEP (*(SORT_BY_NAME(.rodata.padInitTbl_PreInit)))
        KEEP (*(SORT_BY_NAME(.rodata.padInitTbl)))
        KEEP (*(SORT_BY_NAME(.rodata.analogPadInitTbl)))

        _ld_bootrom_end = ABSOLUTE(.);
        _ld_bootrom_size = _ld_bootrom_end - _ld_bootrom_start;

        _ld_miu_flow_start = ABSOLUTE(.);

        *bootrom_miu.o          (.text*)
        *bootrom_dqs.o     (.text*)
        *bootrom_dqs.o     (.rodata*)

        _ld_miu_flow_end = ABSOLUTE(.);
        _ld_miu_flow_size = _ld_miu_flow_end - _ld_miu_flow_start;

        _ld_miu_str_flow_start = ABSOLUTE(.);

        *bootrom_miu_str.o      (.text*)
        *bootrom_data_miu_str.o (.rodata*)
#endif

        _ld_miu_str_flow_end = ABSOLUTE(.);
        _ld_miu_str_flow_size = _ld_miu_str_flow_end - _ld_miu_str_flow_start;

    } > boot

#ifdef CONFIG_TEE_LOADER
    .tee_loader : AT (LOADADDR(.text1) + LENGTH(boot))
    {
        _ld_TEE_LDR_load_start = .;
#ifdef CONFIG_MSTAR_BUILD_TEE_LOADER_DAT
        INCLUDE "../tv_tee_loader/ld/M7322/tee_ldr.ld"
#else
        _ld_TEE_LDR_run_base = .;
        _ld_TEE_LDR_run_start = _ld_TEE_LDR_run_base + 0x100;
        *TEE_loader.o       (tee_loader_dat)
#endif
    . = ALIGN(0x10);
    } > tee_loader
    . = ALIGN(0x400);
     _ld_TEE_LDR_load_end = .;
     _ld_TEE_LDR_size = (_ld_TEE_LDR_load_end - _ld_TEE_LDR_load_start);
#ifdef CONFIG_MSTAR_BUILD_TEE_LOADER_DAT
    .TLbss :
    {
        . = ALIGN(16);
        _ld_TLbss_start = ABSOLUTE(.);
        *(COMMON*)
        *(.bss*)
        . = ALIGN(16);
        _ld_TLbss_end = ABSOLUTE(.);
    } > TLbss
#endif
    .text2 : AT (LOADADDR(.tee_loader)+LENGTH(tee_loader))
#else
    .text2 : AT (LOADADDR(.text1) + 0xE000)
#endif
    {
        _ld_bootram_start = ABSOLUTE(.);
#ifdef CONFIG_MSTAR_BUILD_TEE_LOADER_DAT
        LONG(0x00000000);
        _ld_bootdata_start = .;
        _ld_bootdata_end = .;
        _ld_bootbss_start = ABSOLUTE(.);
        _ld_bootbss_end = ABSOLUTE(.);
#else
#ifdef CONFIG_TEE_LOADER
        *reeloader_entry.o (.text*)
        *reeloader_entry.o (.data*)
#endif
        *bootram.o          (.text*)

        *nandflash.o        (.text*)
        *nandflash.o        (.rodata*)
        *uart16550.o        (.text*)
        *setSPI.o           (.text*)
        *setSPI.o           (.rodata*)
        *apiBDMA.o          (.text*)
        *apiBDMA.o          (.rodata*)
        *apiBDMA.o          (.data*)
        *apiBDMA.o          (.bss*)

        *ns_uboot_loader.o          (.text*)
        *ns_uboot_loader.o          (.data*)
        *ns_uboot_loader.o          (.rodata*)
        *ns_uboot_loader.o          (.bss*)
        *drvSEAL.o          (.text*)
        *drvSEAL.o          (.rodata*)
        *drvSEAL.o          (.data*)
        *drvSEAL.o          (.bss*)
        *halSEAL.o          (.text*)
        *halSEAL.o          (.rodata*)
        *halSEAL.o          (.data*)
        *halSEAL.o          (.bss*)
        *halAESDMA_v2.o     (.text*)
        *drvAESDMA_v2.o     (.text*)
        *drvAESDMA_v2.o     (.rodata*)

        *MSDecompress.o     (.text*)
        *MSDecompress.o     (.rodata*)
        *MSDecompress.o     (.data*)
        *MSDecompress.o     (.bss*)

#ifdef CONFIG_MSTAR_FAST_STR
        *apiSecureR2.o      (.text*)
        *apiSecureR2.o      (.rodata*)
        *apiSecureR2.o      (.data*)
        *apiSecureR2.o      (.bss*)
        *strBootTEE.o       (.text*)
        *strBootTEE.o       (.rodata*)
        *strBootTEE.o       (.data*)
		*strBootTEE.o       (.bss*)
        *fastSTRSetting.o      (.text*)
        *fastSTRSetting.o      (.rodata*)
        *fastSTRSetting.o      (.data*)
        *fastSTRSetting.o      (.bss*)
#endif
        *(NANDCODE)

        *(.ARM.exidx*)

        . = ALIGN(16);
        _ld_bootdata_start = .;
        *(.data*)
        . = ALIGN(16);
        _ld_bootdata_end = .;

        _ld_bootbss_start = ABSOLUTE(.);
        *(.bss*)
        . = ALIGN(16);
        _ld_bootbss_end = ABSOLUTE(.);
        . = ALIGN(0x400);
        . += 2k;
        _ld_bootstack_end = ABSOLUTE(.);
#endif
        . = LENGTH(ram) - 1024;
        /* Reserved for Secure Booting */
        . = LENGTH(ram);
        _ld_bootram_end = ABSOLUTE(.);
        _ld_bootram_size = _ld_bootram_end - _ld_bootram_start;

    } > ram

#elif (CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH == 1)
    .text1 :
    {
#ifdef CONFIG_MSTAR_BUILD_TEE_LOADER_DAT
        LONG(0x00000000);
        _ld_bootrom_start = ABSOLUTE(.);
        _ld_program_reload_addr = ABSOLUTE(.);
        _ld_bootrom_end = ABSOLUTE(.);
        _ld_bootrom_size = _ld_bootrom_end - _ld_bootrom_start;
        _ld_miu_flow_start = ABSOLUTE(.);
        _ld_miu_flow_end = ABSOLUTE(.);
        _ld_miu_flow_size = _ld_miu_flow_end - _ld_miu_flow_start;
        _ld_miu_str_flow_start = ABSOLUTE(.);
#else
        *boot.o                 (.text*)

        . = ALIGN(16);
        _ld_bootrom_start = ABSOLUTE(.);

        *bootrom.o              (.text*)
        *bootrom_cache.o        (.text*)
        *drvRIU.o               (.text*)
        *drvAESDMA.o            (.text*)
        *drvAESDMA.o            (.rodata*)
        *halAESDMA.o            (.text*)
#ifdef CONFIG_TEE_LOADER
        *drvSEAL.o              (.text*)
        *halSEAL.o              (.text*)
        *halSEAL.o              (.rodata*)
        *halAESDMA_v2.o         (.text*)
        *drvAESDMA_v2.o         (.text*)
        *drvAESDMA_v2.o         (.rodata*)
        *(EMMCCODE)
        . = ALIGN(0x400);
#endif
        *eMMC_prg.o             (.text*)
        *eMMC_hal.o             (.text*)
        *eMMC_platform.o        (.text*)
        *_dvmd_tls.o            (.text*)
        *_udivsi3.o             (.text*)        
        *eMMC_RPMB.o            (.text*)
        *eMMC_prg.o             (.data*)
        *eMMC_hal.o             (.data*)
        *eMMC_platform.o        (.data*)
        *eMMC_RPMB.o            (.data*)
        *eMMC_prg.o             (.bss*)
        *eMMC_hal.o             (.bss*)
        *eMMC_platform.o        (.bss*)
        *eMMC_RPMB.o            (.bss*)
/*      *bootrom_data_miu.o     (.rodata*) */
#ifdef CONFIG_ONEBIN_ENABLE
        *drvLoadTable.o         (.text*)
#endif

        _ld_program_reload_addr = ABSOLUTE(.);
        *bootrom_pll.o          (.text*)
        *bootrom_clock.o        (.text*)
        *bootrom_peripheral.o   (.text*)
        *bootrom.o              (.rodata*)
/*      *drvPadConf.o           (.rodata*) */
#ifndef CONFIG_ONEBIN_ENABLE
        KEEP (*(SORT_BY_NAME(.rodata.padInitTbl_PreInit)))
#endif
#ifdef CONFIG_ONEBIN_ENABLE
        KEEP (*(SORT_BY_NAME(.bss.RamBuffer)))
#else
        KEEP (*(SORT_BY_NAME(.rodata.padInitTbl)))
        KEEP (*(SORT_BY_NAME(.rodata.analogPadInitTbl)))
#endif

        _ld_bootrom_end = ABSOLUTE(.);
        _ld_bootrom_size = _ld_bootrom_end - _ld_bootrom_start;


        _ld_miu_flow_start = ABSOLUTE(.);
#ifdef CONFIG_ONEBIN_ENABLE
        KEEP (*(SORT_BY_NAME(.bss.MIUBuffer)))
#endif
        *bootrom_miu.o          (.text*)
        *bootrom_data_miu.o     (.rodata*)

        *bootrom_dqs.o     (.text*)
        *bootrom_dqs.o     (.rodata*)
        _ld_miu_flow_end = ABSOLUTE(.);
        _ld_miu_flow_size = _ld_miu_flow_end - _ld_miu_flow_start;

        _ld_miu_str_flow_start = ABSOLUTE(.);
#ifdef CONFIG_ONEBIN_ENABLE
        KEEP (*(SORT_BY_NAME(.bss.MIUSTRBuffer)))
#endif
        *bootrom_miu_str.o      (.text*)
        *bootrom_data_miu_str.o (.rodata*)
#endif

        _ld_miu_str_flow_end = ABSOLUTE(.);
        _ld_miu_str_flow_size = _ld_miu_str_flow_end - _ld_miu_str_flow_start;

    } > boot

#ifdef CONFIG_DOUBLE_MBOOT
    .text2 : AT (LOADADDR(.text1) + 0xE000)
    {
        _ld_bootram1_start = ABSOLUTE(.);

#ifdef CONFIG_DUAL_SYSTEM
        *bootram_hash_dualsystem.o          (.text*)
        *bootram_hash_dualsystem.o          (.rodata*)
#else
        *bootram_hash.o          (.text*)
        *bootram_hash.o          (.rodata*)
#endif

        . = LENGTH(ram1) - 1024;
        /* Reserved for Secure Booting */
        . = LENGTH(ram1);
        _ld_bootram1_end = ABSOLUTE(.);
        _ld_bootram1_size = _ld_bootram1_end - _ld_bootram1_start;

    } > ram1

    .text3 : AT (LOADADDR(.text1) + 0xF400)
    {
        _ld_bootram2_start = .;
#ifdef CONFIG_MSTAR_BUILD_TEE_LOADER_DAT
        LONG(0x00000000);
        _ld_bootdata_start = .;
        _ld_bootdata_end = .;
        _ld_bootbss_start = ABSOLUTE(.);
        _ld_bootbss_end = ABSOLUTE(.);
#else
#ifdef CONFIG_TEE_LOADER
        *reeloader_entry.o (.text*)
        *reeloader_entry.o (.data*)
#endif
        *bootram.o          (.text*)
        *nandflash.o        (.text*)
        *nandflash.o        (.rodata*)
        *uart16550.o        (.text*)
        *setSPI.o           (.text*)
        *setSPI.o           (.rodata*)
        *apiBDMA.o          (.text*)
        *apiBDMA.o          (.rodata*)
        *apiBDMA.o          (.data*)
        *apiBDMA.o          (.bss*)

        *ns_uboot_loader.o          (.text*)
        *ns_uboot_loader.o          (.data*)
        *ns_uboot_loader.o          (.rodata*)
        *ns_uboot_loader.o          (.bss*)
        *drvSEAL.o          (.text*)
        *drvSEAL.o          (.rodata*)
        *drvSEAL.o          (.data*)
        *drvSEAL.o          (.bss*)
        *halSEAL.o          (.text*)
        *halSEAL.o          (.rodata*)
        *halSEAL.o          (.data*)
        *halSEAL.o          (.bss*)
        *halAESDMA_v2.o     (.text*)
        *drvAESDMA_v2.o     (.text*)
        *drvAESDMA_v2.o     (.rodata*)

        *MSDecompress.o     (.text*)
        *MSDecompress.o     (.rodata*)
        *MSDecompress.o     (.data*)
        *MSDecompress.o     (.bss*)

#ifdef CONFIG_MSTAR_FAST_STR
        *apiSecureR2.o      (.text*)
        *apiSecureR2.o      (.rodata*)
        *apiSecureR2.o      (.data*)
        *apiSecureR2.o      (.bss*)
        *strBootTEE.o       (.text*)
        *strBootTEE.o       (.rodata*)
        *strBootTEE.o       (.data*)
        *fastSTRSetting.o      (.text*)
        *fastSTRSetting.o      (.rodata*)
        *fastSTRSetting.o      (.data*)
        *fastSTRSetting.o      (.bss*)
#endif
        *(EMMCCODE)
        *(.ARM.exidx*)

        . = ALIGN(16);
        _ld_bootdata_start = .;
        *(.data*)
        . = ALIGN(16);
        _ld_bootdata_end = .;

        _ld_bootbss_start = ABSOLUTE(.);
        *(.bss*)
        . = ALIGN(16);
        _ld_bootbss_end = ABSOLUTE(.);
        . = ALIGN(0x400);
        . += 2k;
        _ld_bootstack_end = ABSOLUTE(.);
#endif

        . = LENGTH(ram2) - 1024;
        /* Reserved for Secure Booting */
        . = LENGTH(ram2);
        _ld_bootram2_end = ABSOLUTE(.);
        _ld_bootram2_size = _ld_bootram2_end - _ld_bootram2_start;

    } > ram2
#else
#ifdef CONFIG_TEE_LOADER
    .tee_loader : AT (LOADADDR(.text1) + LENGTH(boot))
    {
        _ld_TEE_LDR_load_start = .;
#ifdef CONFIG_MSTAR_BUILD_TEE_LOADER_DAT
        INCLUDE "../tv_tee_loader/ld/M7322/tee_ldr.ld"
#else
        _ld_TEE_LDR_run_base = .;
        _ld_TEE_LDR_run_start = _ld_TEE_LDR_run_base + 0x100;
        *TEE_loader.o       (tee_loader_dat)
#endif
    . = ALIGN(0x10);
    } > tee_loader
    . = ALIGN(0x400);
     _ld_TEE_LDR_load_end = .;
     _ld_TEE_LDR_size = (_ld_TEE_LDR_load_end - _ld_TEE_LDR_load_start);
#ifdef CONFIG_MSTAR_BUILD_TEE_LOADER_DAT
    .TLbss :
    {
        . = ALIGN(16);
        _ld_TLbss_start = ABSOLUTE(.);
        *(COMMON*)
        *(.bss*)
        . = ALIGN(16);
        _ld_TLbss_end = ABSOLUTE(.);
    } > TLbss
#endif
    .text2 : AT (LOADADDR(.tee_loader)+LENGTH(tee_loader))
#else
    .text2 : AT (LOADADDR(.text1) + 0xE000)
#endif
    {
        _ld_bootram_start = .;
#ifdef CONFIG_MSTAR_BUILD_TEE_LOADER_DAT
        LONG(0x00000000);
        _ld_bootdata_start = .;
        _ld_bootdata_end = .;
        _ld_bootbss_start = ABSOLUTE(.);
        _ld_bootbss_end = ABSOLUTE(.);
#else
#ifdef CONFIG_TEE_LOADER
        *reeloader_entry.o (.text*)
        *reeloader_entry.o (.data*)
#endif
        *bootram.o          (.text*)
        *bootram_cus_uboot2.o        (.text*)
        *bootram_cus_uboot2.o        (.rodata*)
        *bootram_cus_uboot2.o        (.data*)
        *bootram_cus_uboot2.o        (.bss*)
        *nandflash.o        (.text*)
        *nandflash.o        (.rodata*)
        *uart16550.o        (.text*)
        *setSPI.o           (.text*)
        *setSPI.o           (.rodata*)
        *apiBDMA.o          (.text*)
        *apiBDMA.o          (.rodata*)
        *apiBDMA.o          (.data*)
        *apiBDMA.o          (.bss*)

        *ns_uboot_loader.o          (.text*)
        *ns_uboot_loader.o          (.data*)
        *ns_uboot_loader.o          (.rodata*)
        *ns_uboot_loader.o          (.bss*)
        *drvSEAL.o          (.text*)
        *drvSEAL.o          (.rodata*)
        *drvSEAL.o          (.data*)
        *drvSEAL.o          (.bss*)
        *halSEAL.o          (.text*)
        *halSEAL.o          (.rodata*)
        *halSEAL.o          (.data*)
        *halSEAL.o          (.bss*)
        *halAESDMA_v2.o     (.text*)
        *drvAESDMA_v2.o     (.text*)
        *drvAESDMA_v2.o     (.rodata*)

        *MSDecompress.o     (.text*)
        *MSDecompress.o     (.rodata*)
        *MSDecompress.o     (.data*)
        *MSDecompress.o     (.bss*)

#ifdef CONFIG_MSTAR_FAST_STR
        *apiSecureR2.o      (.text*)
        *apiSecureR2.o      (.rodata*)
        *apiSecureR2.o      (.data*)
        *apiSecureR2.o      (.bss*)
        *strBootTEE.o       (.text*)
        *strBootTEE.o       (.rodata*)
        *strBootTEE.o       (.data*)
        *fastSTRSetting.o      (.text*)
        *fastSTRSetting.o      (.rodata*)
        *fastSTRSetting.o      (.data*)
        *fastSTRSetting.o      (.bss*)
#endif
        *(EMMCCODE)
        *(.ARM.exidx*)

        . = ALIGN(16);
        _ld_bootdata_start = .;
        *(.data*)
        . = ALIGN(16);
        _ld_bootdata_end = .;

        _ld_bootbss_start = ABSOLUTE(.);
        *(.bss*)
        . = ALIGN(16);
        _ld_bootbss_end = ABSOLUTE(.);
        . = ALIGN(0x400);
        . += 2k;
        _ld_bootstack_end = ABSOLUTE(.);
#endif

        . = LENGTH(ram) - 1024;
        /* Reserved for Secure Booting */
        . = LENGTH(ram);
        _ld_bootram_end = ABSOLUTE(.);
        _ld_bootram_size = _ld_bootram_end - _ld_bootram_start;

    } > ram
#endif

#else
    .text1 :
    {
        _ld_boot_start = ABSOLUTE(.);
        *boot.o             (.text*)
        . = ALIGN(16);
        _ld_boot_end = ABSOLUTE(.);
        _ld_boot_size = _ld_boot_end - _ld_boot_start;

    } > boot

    .text2 : AT ( LOADADDR(.text1) + 0x2000 )
    {
        _ld_bootrom_start = ABSOLUTE(.);
        *drvRIU.o           (.text*)
        *bootrom.o          (.text*)
        *bootrom_pll.o      (.text*)
        *bootrom_clock.o    (.text*)
        *bootrom_peripheral.o   (.text*)
        *bootrom_cache.o    (.text*)
        *bootrom_miu.o      (.text*)
        *bootrom_miu_str.o  (.text*)

        *bootrom_dqs.o      (.text*)

        *bootrom.o          (.rodata*)
        *bootrom_data_miu.o (.rodata*)
/*      *drvPadConf.o       (.rodata*) */

        KEEP (*(SORT_BY_NAME(.rodata.padInitTbl_PreInit)))
        KEEP (*(SORT_BY_NAME(.rodata.padInitTbl)))
        KEEP (*(SORT_BY_NAME(.rodata.analogPadInitTbl)))

        . = ALIGN(16);
        _ld_bootrom_end = ABSOLUTE(.);
        _ld_bootrom_size = _ld_bootrom_end - _ld_bootrom_start;
    } > rom

    .text3 : AT (LOADADDR(.text2) + SIZEOF(.text2))
    {
        _ld_bootram_start = ABSOLUTE(.);
        *bootram.o          (.text*)
        *nandflash.o        (.text*)
        *nandflash.o        (.rodata*)
        *uart16550.o        (.text*)
        *setSPI.o           (.text*)
        *setSPI.o           (.rodata*)
        *apiBDMA.o          (.text*)
        *apiBDMA.o          (.rodata*)

        *MSDecompress.o     (.text*)
        *MSDecompress.o     (.rodata*)

        *(.ARM.exidx*)

    } > ram

    .data :
    {
        . = ALIGN(16);
        _ld_bootdata_start = ABSOLUTE(.);
        *(.data*)
        . = ALIGN(16);
        _ld_bootdata_end = ABSOLUTE(.);
        _ld_bootram_end = ABSOLUTE(.);
        _ld_bootram_size = _ld_bootram_end - _ld_bootram_start;
    } > ram

    .bss :
    {
        . = ALIGN(16);
        _ld_bootbss_start = ABSOLUTE(.);
        *(.bss*)
        . = ALIGN(16);
        _ld_bootbss_end = ABSOLUTE(.);
    } > ram

#endif

    /DISCARD/ :
    {
        /* ABI crap starts here */

        *(.reginfo)
        *(.note*d)
    }

#if (CONFIG_MSTAR_ROM_BOOT_WITH_NAND_FLASH == 1)
    .padding : AT(LOADADDR(.text1) + 0x8000 - 4)
    {
        /* LONG(0x0) */
    } > ram
#elif (CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH == 1)
#ifdef CONFIG_DOUBLE_MBOOT
    .padding : AT(LOADADDR(.text1) + 0x8000 - 4)
    {
        /* LONG(0x0) */
    } > ram1
    .padding : AT(LOADADDR(.text1) + 0x9400 - 4)
    {
        /* LONG(0x0) */
    } > ram2
#else
    .padding : AT(LOADADDR(.text1) + 0x8000 - 4)
    {
        /* LONG(0x0) */
    } > ram
#endif
#else
    .padding : AT(LOADADDR(.text1) + 0x10000 - 4)
    {
        LONG(0x0)
    } > ram
#endif
}
